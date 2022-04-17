// This file is modified from BeRTOS.

#include "afsk.h"
#include "fifobuf.h"
#include "ax25.h"
#include "bertos.h"
#include "wavegen.h"
#include <string.h> /* memset */
//#include <windows.h>
#include <stdio.h>
#include <math.h>

#define TX_EN() //printf("TX ON!\n")
#define TX_DIS()

#define	FLAG_AFSK_FRAME_START()
#define	FLAG_AFSK_FRAME_END()

#define SYS_DELAY(int)

#define BIT_DIFFER(bitline1, bitline2) (((bitline1) ^ (bitline2)) & 0x01)
#define EDGE_FOUND(bitline)            BIT_DIFFER((bitline), (bitline) >> 1)


#define PHASE_BIT    8
#define PHASE_INC    1

#define PHASE_MAX    (SAMPLEPERBIT * PHASE_BIT)
#define PHASE_THRES  (PHASE_MAX / 2) // - PHASE_BIT / 2)

// Modulator constants
#define MARK_FREQ  1200
#define MARK_INC   (uint16_t)(DIV_ROUND(SIN_LEN * (uint32_t)MARK_FREQ, CONFIG_AFSK_DAC_SAMPLERATE))

#define SPACE_FREQ 2200
#define SPACE_INC  (uint16_t)(DIV_ROUND(SIN_LEN * (uint32_t)SPACE_FREQ, CONFIG_AFSK_DAC_SAMPLERATE))

#define DAC_SAMPLEPERBIT (CONFIG_AFSK_DAC_SAMPLERATE / BITRATE)


inline Afsk *AFSK_CAST(KFile *fd)
{
//  ASSERT(fd->_type == KFT_AFSK);
  return (Afsk *)fd;
}

/**
 * High-Level Data Link Control parsing function.
 * Parse bitstream in order to find characters.
 *
 * \param hdlc HDLC context.
 * \param bit  current bit to be parsed.
 * \param fifo FIFO buffer used to push characters.
 *
 * \return true if all is ok, false if the fifo is full.
 */
static bool hdlc_parse(Hdlc *hdlc, bool bit, FIFOBuffer *fifo)
{
	bool ret = true;

	hdlc->demod_bits <<= 1;
	hdlc->demod_bits |= bit ? 1 : 0;

	/* HDLC Flag */
	if (hdlc->demod_bits == HDLC_FLAG)
	{
		if (!fifo_isfull(fifo))
		{
			fifo_push(fifo, HDLC_FLAG);
			hdlc->rxstart = true;
			FLAG_AFSK_FRAME_START();
		}
		else
		{
			ret = false;
			hdlc->rxstart = false;
			FLAG_AFSK_FRAME_END();
		}

		hdlc->currchar = 0;
		hdlc->bit_idx = 0;
		return ret;
	}

	/* Reset */
	if ((hdlc->demod_bits & HDLC_RESET) == HDLC_RESET)
	{
		hdlc->rxstart = false;
		FLAG_AFSK_FRAME_END();
		return ret;
	}

	if (!hdlc->rxstart)
		return ret;

	/* Stuffed bit */
	if ((hdlc->demod_bits & 0x3f) == 0x3e)
		return ret;

	if (hdlc->demod_bits & 0x01)
		hdlc->currchar |= 0x80;

	if (++hdlc->bit_idx >= 8)
	{
		if ((hdlc->currchar == HDLC_FLAG
			|| hdlc->currchar == HDLC_RESET
			|| hdlc->currchar == AX25_ESC))
		{
			if (!fifo_isfull(fifo))
				fifo_push(fifo, AX25_ESC);
			else
			{
				hdlc->rxstart = false;
				FLAG_AFSK_FRAME_END();
				ret = false;
			}
		}

		if (!fifo_isfull(fifo))
			fifo_push(fifo, hdlc->currchar);
		else
		{
			hdlc->rxstart = false;
			FLAG_AFSK_FRAME_END();
			ret = false;
		}

		hdlc->currchar = 0;
		hdlc->bit_idx = 0;
	}
	else
		hdlc->currchar >>= 1;

	return ret;
}


/**
 * ADC ISR callback.
 * This function has to be called by the ADC ISR when a sample of the configured
 * channel is available.
 * \param af Afsk context to operate on.
 * \param curr_sample current sample from the ADC.
 */
void afsk_rx_proc(Afsk *af, float *pSrc, uint32_t blocksize)
{
	while(blocksize>0)
	{
		uint8_t bits;

	/*
	 * Frequency discriminator and LP IIR filter.
	 * This filter is designed to work
	 * at the given sample rate and bit rate.
	 */

		/*
		* Frequency discrimination is achieved by simply multiplying
		* the sample with a delayed sample of (samples per bit) / 2.
		* Then the signal is lowpass filtered with a first order,
		* 600 Hz filter. The filter implementation is selectable
		* through the CONFIG_AFSK_FILTER config variable.
		*/

		af->iir_x[0] = af->iir_x[1];

		af->iir_x[1] = ((int32_t)fifo_pop_q15(&af->delay_fifo) * (int32_t)((*pSrc)*32768)) >> 2;

		af->iir_y[0] = af->iir_y[1];

		/*
		 * This should be (af->iir_y[0] * 0.438) but
		 * (af->iir_y[0] >> 1) is a faster approximation :-)
		 */
		af->iir_y[1] = af->iir_x[0] + af->iir_x[1] + (af->iir_y[0] >> 1);

		/* Save this sampled bit in a delay line */
		af->sampled_bits <<= 1;
		af->sampled_bits |= (af->iir_y[1] > 0) ? 1 : 0;

		/* Store current ADC sample in the af->delay_fifo */
		fifo_push_q15(&af->delay_fifo, (int16_t)((*pSrc)*32768));

		/* If there is an edge, adjust phase sampling */
		if (EDGE_FOUND(af->sampled_bits))
		{
			if (af->curr_phase < PHASE_THRES)
				af->curr_phase += PHASE_INC;
			else
				af->curr_phase -= PHASE_INC;
		}
		af->curr_phase += PHASE_BIT;

		/* sample the bit */
		if (af->curr_phase >= PHASE_MAX)
		{
			af->curr_phase %= PHASE_MAX;

			/* Shift 1 position in the shift register of the found bits */
			af->found_bits <<= 1;

			/*
			* Determine bit value by reading the last 3 sampled bits.
			* If the number of ones is two or greater, the bit value is a 1,
			* otherwise is a 0.
			* This algorithm presumes that there are 8 samples per bit.
			*/
			bits = (af->sampled_bits & 0x07);
			if (bits == 0x07 // 111, 3 bits set to 1
			|| bits == 0x06 // 110, 2 bits
			|| bits == 0x05 // 101, 2 bits
			|| bits == 0x03 // 011, 2 bits
			)
				af->found_bits |= 1;

			/*
			* NRZI coding: if 2 consecutive bits have the same value
			* a 1 is received, otherwise it's a 0.
			*/
			if (!hdlc_parse(&af->hdlc, !EDGE_FOUND(af->found_bits), &af->rx_fifo))
				af->status |= AFSK_RXFIFO_OVERRUN;

		}
		blocksize--;
		pSrc++;
	}
}

static void afsk_txStart(Afsk *af)
{
	if (!af->sending)
	{
		af->phase_inc = MARK_INC;
		af->phase_acc = 0;
		af->stuff_cnt = 0;
		af->sending = true;
		af->preamble_len = DIV_ROUND(CONFIG_AFSK_PREAMBLE_LEN * BITRATE, 8000);
		TX_EN();
//		AFSK_DAC_IRQ_START(af->dac_ch);
	}
	af->trailer_len  = DIV_ROUND(CONFIG_AFSK_TRAILER_LEN  * BITRATE, 8000);
}


#define BIT_STUFF_LEN 5

#define SWITCH_TONE(inc)  (((inc) == MARK_INC) ? SPACE_INC : MARK_INC)

/**
 * DAC ISR callback.
 * This function has to be called by the DAC ISR when a sample of the configured
 * channel has been converted out.
 *
 * \param af Afsk context to operate on.
 *
 * \return The next DAC output sample.
 */
unsigned int afsk_tx_proc(Afsk *af, float *pDst, uint32_t blocksize)
{
	unsigned int i;	
	for(i=0; i<blocksize; i++)
	{

		/* Check if we are at a start of a sample cycle */
		if (af->sample_count == 0)
		{
			if (af->tx_bit == 0)
			{
				/* We have just finished transimitting a char, get a new one. */
				if (fifo_isempty(&af->tx_fifo) && af->trailer_len == 0)
				{
										
					af->sending = false;
				  	TX_DIS();
					if(af->cfg_padding_zero)
					{
						*pDst = 0;
						pDst++;
						continue;
					}
					else break;
				}
				else
				{
					/*
					* If we have just finished sending an unstuffed byte,
					* reset bitstuff counter.
					*/
					if (!af->bit_stuff)
						af->stuff_cnt = 0;

					af->bit_stuff = true;

					/*
					* Handle preamble and trailer
					*/
					if (af->preamble_len == 0)
					{
						if (fifo_isempty(&af->tx_fifo))
						{
							af->trailer_len--;
							af->curr_out = HDLC_FLAG;
						}
						else
							af->curr_out = fifo_pop(&af->tx_fifo);
					}
					else
					{
						af->preamble_len--;
						af->curr_out = HDLC_FLAG;
					}

					/* Handle char escape */
					if (af->curr_out == AX25_ESC)
					{
						if (fifo_isempty(&af->tx_fifo))
						{
							af->sending = false;
						  	TX_DIS();
							if(af->cfg_padding_zero)
							{
								*pDst = 0;
								pDst++;
								continue;
							}
							else break;
						}
						else
							af->curr_out = fifo_pop(&af->tx_fifo);
					}
					else if (af->curr_out == HDLC_FLAG || af->curr_out == HDLC_RESET)
						/* If these chars are not escaped disable bit stuffing */
						af->bit_stuff = false;
				}
				/* Start with LSB mask */
				af->tx_bit = 0x01;
			}

			/* check for bit stuffing */
			if (af->bit_stuff && af->stuff_cnt >= BIT_STUFF_LEN)
			{
				/* If there are more than 5 ones in a row insert a 0 */
				af->stuff_cnt = 0;
				/* switch tone */
				af->phase_inc = SWITCH_TONE(af->phase_inc);
			}
			else
			{
				/*
				* NRZI: if we want to transmit a 1 the modulated frequency will stay
				* unchanged; with a 0, there will be a change in the tone.
				*/
				if (af->curr_out & af->tx_bit)
				{
					/*
					* Transmit a 1:
					* - Stay on the previous tone
					* - Increase bit stuff counter
					*/
					af->stuff_cnt++;
				}
				else
				{
					/*
					* Transmit a 0:
					* - Reset bit stuff counter
					* - Switch tone
					*/
					af->stuff_cnt = 0;
					af->phase_inc = SWITCH_TONE(af->phase_inc);
				}

				/* Go to the next bit */
				af->tx_bit <<= 1;
			}
			af->sample_count = DAC_SAMPLEPERBIT;
		}

		/* Get new sample and put it out on the DAC */
		af->phase_acc += af->phase_inc;
		af->phase_acc %= SIN_LEN;

		af->sample_count--;

		*pDst = ((float)sinetable[af->phase_acc])/32768;

		pDst++;
	}
	return i;
}



static size_t afsk_read(KFile *fd, void *_buf, size_t size)
{
	Afsk *af = AFSK_CAST(fd);
	uint8_t *buf = (uint8_t *)_buf;

	#if CONFIG_AFSK_RXTIMEOUT == 0
	while (size-- && !fifo_isempty(&af->rx_fifo))
	#else
	while (size--)
	#endif
	{
		#if CONFIG_AFSK_RXTIMEOUT != -1
		// ticks_t start = timer_clock();
		#endif

		while (fifo_isempty(&af->rx_fifo))
		{
			/* cpu_relax() */;
			SYS_DELAY(100);

			#if CONFIG_AFSK_RXTIMEOUT != -1
			// if (timer_clock() - start > ms_to_ticks(CONFIG_AFSK_RXTIMEOUT))
			// 	return buf - (uint8_t *)_buf;
			#endif
		};

		*buf++ = fifo_pop(&af->rx_fifo);
	}

	return buf - (uint8_t *)_buf;
}

static size_t afsk_write(KFile *fd, const void *_buf, size_t size)
{
	Afsk *af = AFSK_CAST(fd);
	const uint8_t *buf = (const uint8_t *)_buf;

	while (size--)
	{
		while (fifo_isfull(&af->tx_fifo))
		{
			SYS_DELAY(100);
		}

		fifo_push(&af->tx_fifo, *buf++);
		afsk_txStart(af);
	}

	return buf - (const uint8_t *)_buf;
}

static int afsk_flush(KFile *fd)
{
	Afsk *af = AFSK_CAST(fd);
	while (af->sending)
		SYS_DELAY(100);
	return 0;
}

static int afsk_error(KFile *fd)
{
	Afsk *af = AFSK_CAST(fd);
	int err;

	err = af->status;
	return err;
}

static void afsk_clearerr(KFile *fd)
{
	Afsk *af = AFSK_CAST(fd);
	af->status = 0;
}


/**
 * Initialize an AFSK1200 modem.
 * \param af Afsk context to operate on.
 * \param adc_ch  ADC channel used by the demodulator.
 * \param dac_ch  DAC channel used by the modulator.
 */
void afsk_init(Afsk *af)
{
	int i;

//	#if CONFIG_AFSK_RXTIMEOUT != -1
//	MOD_CHECK(timer);
//	#endif
	memset(af, 0, sizeof(*af));
//	af->adc_ch = adc_ch;
//	af->dac_ch = dac_ch;

	fifo_init_q15(&af->delay_fifo, (int16_t *)af->delay_buf, (SAMPLEPERBIT / 2 + 1));
	fifo_init(&af->rx_fifo, af->rx_buf, sizeof(af->rx_buf));

	/* Fill sample FIFO with 0 */
	for (i = 0; i < SAMPLEPERBIT / 2; i++)
		fifo_push_q15(&af->delay_fifo, 0);

	fifo_init(&af->tx_fifo, af->tx_buf, sizeof(af->tx_buf));

//	AFSK_ADC_INIT(adc_ch, af);
//	AFSK_DAC_INIT(dac_ch, af);
//	AFSK_STROBE_INIT();
//	LOG_INFO("MARK_INC %d, SPACE_INC %d\n", MARK_INC, SPACE_INC);

	DB(af->fd._type = KFT_AFSK);
	af->fd.write = afsk_write;
	af->fd.read = afsk_read;
	af->fd.flush = afsk_flush;
	af->fd.error = afsk_error;
	af->fd.clearerr = afsk_clearerr;
	af->phase_inc = MARK_INC;
}

void direwolf_afsk_rx_proc(Afsk *af, float *pSrc, uint32_t blocksize)
{
	while(blocksize>0)
	{
		demod_afsk_process_sample (0, 0, (int)(*pSrc * 32767.0), &af->direwolf_state);

		blocksize--;
		pSrc++;
	}
}

static void direwolf_afsk_demod_callback(void *obj_ptr, int demod_data)
{
    Afsk *af = (Afsk *)obj_ptr;

    af->found_bits <<= 1;
    if(demod_data) af->found_bits |= 1;
    hdlc_parse(&af->hdlc, !EDGE_FOUND(af->found_bits), &af->rx_fifo);
}

void direwolf_afsk_init(Afsk *af)
{
	int i;

//	#if CONFIG_AFSK_RXTIMEOUT != -1
//	MOD_CHECK(timer);
//	#endif
	memset(af, 0, sizeof(*af));
//	af->adc_ch = adc_ch;
//	af->dac_ch = dac_ch;

    demod_afsk_init (9600, 1200, 1200, 2200, 'E', &af->direwolf_state, (void *)af, direwolf_afsk_demod_callback);

	//fifo_init_q15(&af->delay_fifo, (int16_t *)af->delay_buf, (SAMPLEPERBIT / 2 + 1));
	fifo_init(&af->rx_fifo, af->rx_buf, sizeof(af->rx_buf));

	/* Fill sample FIFO with 0 */
	//for (i = 0; i < SAMPLEPERBIT / 2; i++)
	//	fifo_push_q15(&af->delay_fifo, 0);
	fifo_init(&af->tx_fifo, af->tx_buf, sizeof(af->tx_buf));

//	AFSK_ADC_INIT(adc_ch, af);
//	AFSK_DAC_INIT(dac_ch, af);
//	AFSK_STROBE_INIT();
//	LOG_INFO("MARK_INC %d, SPACE_INC %d\n", MARK_INC, SPACE_INC);

	DB(af->fd._type = KFT_AFSK);
	af->fd.write = afsk_write;
	af->fd.read = afsk_read;
	af->fd.flush = afsk_flush;
	af->fd.error = afsk_error;
	af->fd.clearerr = afsk_clearerr;
	af->phase_inc = MARK_INC;
}


