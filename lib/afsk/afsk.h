// This file is modified from BeRTOS.

#ifndef __AFSK_H__
#define __AFSK_H__

/* Native includes */

#include "fifobuf.h"
#include "kfile.h"
#include "cfg_afsk.h"
#include "bertos.h"
#include "direwolf_demod_afsk.h"


/**
 * ADC sample rate.
 * The demodulator filters are designed to work at this frequency.
 * If you need to change this remember to update afsk_adc_isr().
 */
#define SAMPLERATE 9600

/**
 * Bitrate of the received/transmitted data.
 * The demodulator filters and decoderes are designed to work at this frequency.
 * If you need to change this remember to update afsk_adc_isr().
 */
#define BITRATE    1200

#define SAMPLEPERBIT (SAMPLERATE / BITRATE)

/**
 * HDLC (High-Level Data Link Control) context.
 * Maybe to be moved in a separate HDLC module one day.
 */
typedef struct Hdlc
{
	uint8_t demod_bits; ///< Bitstream from the demodulator.
	uint8_t bit_idx;    ///< Current received bit.
	uint8_t currchar;   ///< Current received character.
	bool rxstart;       ///< True if an HDLC_FLAG char has been found in the bitstream.
} Hdlc;

/**
 * RX FIFO buffer full error.
 */
#define AFSK_RXFIFO_OVERRUN BV(0)

/**
 * AFSK1200 modem context.
 */
typedef struct Afsk
{
	/** Base "class" */
	KFile fd;

	/** Current sample of bit for output data. */
	uint8_t sample_count;

	/** Current character to be modulated */
	uint8_t curr_out;

	/** Mask of current modulated bit */
	uint8_t tx_bit;

	/** True if bit stuff is allowed, false otherwise */
	bool bit_stuff;

	/** Counter for bit stuffing */
	uint8_t stuff_cnt;
	/**
	 * DDS phase accumulator for generating modulated data.
	 */
	uint16_t phase_acc;

	/** Current phase increment for current modulated bit */
	uint16_t phase_inc;

	/** Delay line used to delay samples by (SAMPLEPERBIT / 2) */
	FIFOBuffer_q15 delay_fifo;

	/**
	 * Buffer for delay FIFO.
	 * The 1 is added because the FIFO macros need
	 * 1 byte more to handle a buffer (SAMPLEPERBIT / 2) bytes long.
	 */
	int16_t delay_buf[SAMPLEPERBIT / 2 + 1];

	/** FIFO for received data */
	FIFOBuffer rx_fifo;

	/** FIFO rx buffer */
	uint8_t rx_buf[CONFIG_AFSK_RX_BUFLEN];

	/** FIFO for transmitted data */
	FIFOBuffer tx_fifo;

	/** FIFO tx buffer */
	uint8_t tx_buf[CONFIG_AFSK_TX_BUFLEN];

	/** IIR filter X cells, used to filter sampled data by the demodulator */
	int32_t iir_x[2];

	/** IIR filter Y cells, used to filter sampled data by the demodulator */
	int32_t iir_y[2];

	/**
	 * Bits sampled by the demodulator are here.
	 * Since ADC samplerate is higher than the bitrate, the bits here are
	 * SAMPLEPERBIT times the bitrate.
	 */
	uint8_t sampled_bits;

	/**
	 * Current phase, needed to know when the bitstream at ADC speed
	 * should be sampled.
	 */
	int8_t curr_phase;

	/** Bits found by the demodulator at the correct bitrate speed. */
	uint8_t found_bits;

	/** True while modem sends data */
	volatile bool sending;

	/**
	 * AFSK modem status.
	 * If 0 all is ok, otherwise errors are present.
	 */
	volatile int status;

	/** Hdlc context */
	Hdlc hdlc;

	/**
	 * Preamble length.
	 * When the AFSK modem wants to send data, before sending the actual data,
	 * shifts out preamble_len HDLC_FLAG characters.
	 * This helps to synchronize the demodulator filters on the receiver side.
	 */
	uint16_t preamble_len;

	/**
	 * Trailer length.
	 * After sending the actual data, the AFSK shifts out
	 * trailer_len HDLC_FLAG characters.
	 * This helps to synchronize the demodulator filters on the receiver side.
	 */
	uint16_t trailer_len;

	uint8_t cfg_padding_zero;

    struct demodulator_state_s direwolf_state;
} Afsk;


#define KFT_AFSK MAKE_ID('A', 'F', 'S', 'K')




void afsk_rx_proc(Afsk *af, float *pSrc, uint32_t blocksize);
unsigned int afsk_tx_proc(Afsk *af, float *pDst, uint32_t blocksize);
void afsk_init(Afsk *af);

void direwolf_afsk_rx_proc(Afsk *af, float *pSrc, uint32_t blocksize);
void direwolf_afsk_init(Afsk *af);

#endif









