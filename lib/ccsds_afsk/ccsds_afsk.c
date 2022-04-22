/*
 * Copyright 2015 WEI Mingchuan, BG2BHC <bg2bhc@gmail.com>
 * Copyright 2015 HIT Research Center of Satellite Technology
 * Copyright 2015 HIT Amateur Radio Club, BY2HIT
 *
 * Harbin Institute of Technology <http://www.hit.edu.cn/>
 * LilacSat - HIT Student Satellites <http://lilacsat.hit.edu.cn/>
 * HIT Amateur Radio Club <http://www.by2hit.net/>
 *
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "ccsds_afsk.h"
#include "afsk/wavegen.h"
#include <stdio.h>

#define TX_EN()
#define TX_DIS()
#define OS_Delay()


extern char sequence[]; //伪随机序列

void ccsds_afsk_init(Ccsds_afsk *cc, uint32_t sync_word, uint16_t len_frame, void *obj_ptr, afsk_sync_hook_t hook)
{
    //float RATE=0.5;
    //float ebn0 = 12.0;
    //float esn0 = RATE*pow(10.0, ebn0/10);


    //gen_met(mettab, amp, esn0, 0.0, 4);

    vitfilt27_init(&(cc->vi));

    cc->sending = 0;
    fifo_init(&cc->tx_fifo, cc->tx_buf, sizeof(cc->tx_buf));
    fifo_init(&cc->rx_fifo, cc->rx_buf, sizeof(cc->rx_buf));

    fifo_init_q15(&cc->delay_fifo, (int16_t *)cc->delay_buf, (SAMPLEPERBIT / 2 + 1));
	/* Fill sample FIFO with 0 */
	for (int i = 0; i < SAMPLEPERBIT / 2; i++)
		fifo_push_q15(&cc->delay_fifo, 0);

    cc->sync_word = sync_word;
    cc->len_frame = len_frame;
    cc->syncing = 0;
    cc->buffer_sync_det = 0;
    cc->n_out = 0;
    cc->mask_bit_out = 0x80;
    cc->obj_ptr = obj_ptr;
    cc->hook = hook;
    cc->rx_bit_state = 0;
    cc->tx_bit_state = 0;

    cc->cfg_continous = 0;
    cc->cfg_padding_zero = 0;
    cc->cfg_using_m = 0;
    cc->cfg_using_convolutional_code = 0;
    cc->cfg_preamble_len = 75;
    cc->cfg_trailer_len = 30;

    cc->sample_count = 0;
    cc->bit_count = 0;
    cc->current_data = 0;
    cc->phase_inc = SPACE_INC;
    cc->phase_acc = 0;

    cc->bit_count_rx = 0;
    cc->curr_phase = 0;
}


static inline void ccsds_txwrite(Ccsds_afsk *cc, unsigned char c)
{
    if(!cc->sending)
    {
        cc->sending = 1;
        TX_EN();//Turn on transmitter
        cc->preamble_len = cc->cfg_preamble_len;
        cc->trailer_len = cc->cfg_trailer_len;
    }

    while(fifo_isfull(&(cc->tx_fifo)))
    {
        OS_Delay();
    }
    fifo_push(&(cc->tx_fifo), c);
}

static unsigned char m_encode_tab[] =
{
    0x00, 0x01, 0x03, 0x02, 0x07, 0x06, 0x04, 0x05,
    0x0F, 0x0E, 0x0C, 0x0D, 0x08, 0x09, 0x0B, 0x0A,
    0x1F, 0x1E, 0x1C, 0x1D, 0x18, 0x19, 0x1B, 0x1A,
    0x10, 0x11, 0x13, 0x12, 0x17, 0x16, 0x14, 0x15,
    0x3F, 0x3E, 0x3C, 0x3D, 0x38, 0x39, 0x3B, 0x3A,
    0x30, 0x31, 0x33, 0x32, 0x37, 0x36, 0x34, 0x35,
    0x20, 0x21, 0x23, 0x22, 0x27, 0x26, 0x24, 0x25,
    0x2F, 0x2E, 0x2C, 0x2D, 0x28, 0x29, 0x2B, 0x2A,
    0x7F, 0x7E, 0x7C, 0x7D, 0x78, 0x79, 0x7B, 0x7A,
    0x70, 0x71, 0x73, 0x72, 0x77, 0x76, 0x74, 0x75,
    0x60, 0x61, 0x63, 0x62, 0x67, 0x66, 0x64, 0x65,
    0x6F, 0x6E, 0x6C, 0x6D, 0x68, 0x69, 0x6B, 0x6A,
    0x40, 0x41, 0x43, 0x42, 0x47, 0x46, 0x44, 0x45,
    0x4F, 0x4E, 0x4C, 0x4D, 0x48, 0x49, 0x4B, 0x4A,
    0x5F, 0x5E, 0x5C, 0x5D, 0x58, 0x59, 0x5B, 0x5A,
    0x50, 0x51, 0x53, 0x52, 0x57, 0x56, 0x54, 0x55,
    0xFF, 0xFE, 0xFC, 0xFD, 0xF8, 0xF9, 0xFB, 0xFA,
    0xF0, 0xF1, 0xF3, 0xF2, 0xF7, 0xF6, 0xF4, 0xF5,
    0xE0, 0xE1, 0xE3, 0xE2, 0xE7, 0xE6, 0xE4, 0xE5,
    0xEF, 0xEE, 0xEC, 0xED, 0xE8, 0xE9, 0xEB, 0xEA,
    0xC0, 0xC1, 0xC3, 0xC2, 0xC7, 0xC6, 0xC4, 0xC5,
    0xCF, 0xCE, 0xCC, 0xCD, 0xC8, 0xC9, 0xCB, 0xCA,
    0xDF, 0xDE, 0xDC, 0xDD, 0xD8, 0xD9, 0xDB, 0xDA,
    0xD0, 0xD1, 0xD3, 0xD2, 0xD7, 0xD6, 0xD4, 0xD5,
    0x80, 0x81, 0x83, 0x82, 0x87, 0x86, 0x84, 0x85,
    0x8F, 0x8E, 0x8C, 0x8D, 0x88, 0x89, 0x8B, 0x8A,
    0x9F, 0x9E, 0x9C, 0x9D, 0x98, 0x99, 0x9B, 0x9A,
    0x90, 0x91, 0x93, 0x92, 0x97, 0x96, 0x94, 0x95,
    0xBF, 0xBE, 0xBC, 0xBD, 0xB8, 0xB9, 0xBB, 0xBA,
    0xB0, 0xB1, 0xB3, 0xB2, 0xB7, 0xB6, 0xB4, 0xB5,
    0xA0, 0xA1, 0xA3, 0xA2, 0xA7, 0xA6, 0xA4, 0xA5,
    0xAF, 0xAE, 0xAC, 0xAD, 0xA8, 0xA9, 0xAB, 0xAA,
};

static unsigned char m_decode_tab[] =
{
    0x00, 0x01, 0x03, 0x02, 0x06, 0x07, 0x05, 0x04,
    0x0C, 0x0D, 0x0F, 0x0E, 0x0A, 0x0B, 0x09, 0x08,
    0x18, 0x19, 0x1B, 0x1A, 0x1E, 0x1F, 0x1D, 0x1C,
    0x14, 0x15, 0x17, 0x16, 0x12, 0x13, 0x11, 0x10,
    0x30, 0x31, 0x33, 0x32, 0x36, 0x37, 0x35, 0x34,
    0x3C, 0x3D, 0x3F, 0x3E, 0x3A, 0x3B, 0x39, 0x38,
    0x28, 0x29, 0x2B, 0x2A, 0x2E, 0x2F, 0x2D, 0x2C,
    0x24, 0x25, 0x27, 0x26, 0x22, 0x23, 0x21, 0x20,
    0x60, 0x61, 0x63, 0x62, 0x66, 0x67, 0x65, 0x64,
    0x6C, 0x6D, 0x6F, 0x6E, 0x6A, 0x6B, 0x69, 0x68,
    0x78, 0x79, 0x7B, 0x7A, 0x7E, 0x7F, 0x7D, 0x7C,
    0x74, 0x75, 0x77, 0x76, 0x72, 0x73, 0x71, 0x70,
    0x50, 0x51, 0x53, 0x52, 0x56, 0x57, 0x55, 0x54,
    0x5C, 0x5D, 0x5F, 0x5E, 0x5A, 0x5B, 0x59, 0x58,
    0x48, 0x49, 0x4B, 0x4A, 0x4E, 0x4F, 0x4D, 0x4C,
    0x44, 0x45, 0x47, 0x46, 0x42, 0x43, 0x41, 0x40,
    0xC0, 0xC1, 0xC3, 0xC2, 0xC6, 0xC7, 0xC5, 0xC4,
    0xCC, 0xCD, 0xCF, 0xCE, 0xCA, 0xCB, 0xC9, 0xC8,
    0xD8, 0xD9, 0xDB, 0xDA, 0xDE, 0xDF, 0xDD, 0xDC,
    0xD4, 0xD5, 0xD7, 0xD6, 0xD2, 0xD3, 0xD1, 0xD0,
    0xF0, 0xF1, 0xF3, 0xF2, 0xF6, 0xF7, 0xF5, 0xF4,
    0xFC, 0xFD, 0xFF, 0xFE, 0xFA, 0xFB, 0xF9, 0xF8,
    0xE8, 0xE9, 0xEB, 0xEA, 0xEE, 0xEF, 0xED, 0xEC,
    0xE4, 0xE5, 0xE7, 0xE6, 0xE2, 0xE3, 0xE1, 0xE0,
    0xA0, 0xA1, 0xA3, 0xA2, 0xA6, 0xA7, 0xA5, 0xA4,
    0xAC, 0xAD, 0xAF, 0xAE, 0xAA, 0xAB, 0xA9, 0xA8,
    0xB8, 0xB9, 0xBB, 0xBA, 0xBE, 0xBF, 0xBD, 0xBC,
    0xB4, 0xB5, 0xB7, 0xB6, 0xB2, 0xB3, 0xB1, 0xB0,
    0x90, 0x91, 0x93, 0x92, 0x96, 0x97, 0x95, 0x94,
    0x9C, 0x9D, 0x9F, 0x9E, 0x9A, 0x9B, 0x99, 0x98,
    0x88, 0x89, 0x8B, 0x8A, 0x8E, 0x8F, 0x8D, 0x8C,
    0x84, 0x85, 0x87, 0x86, 0x82, 0x83, 0x81, 0x80
};

static inline void ccsds_rxwrite(Ccsds_afsk *cc, unsigned char c)
{
    if(fifo_isfull(&(cc->rx_fifo)))
    {
        fifo_flush(&(cc->rx_fifo));
    }
    fifo_push(&(cc->rx_fifo), c);
}

//unsigned int ccsds_tx_proc(Ccsds *cc, unsigned char *symbols, unsigned int nbits)
unsigned int ccsds_afsk_tx_proc(Ccsds_afsk *cc, float *pDst, unsigned int blocksize)
{
    //unsigned char current_data;
    //unsigned int i, j, nbytes;
    unsigned int i;

    /*if(cc->cfg_using_convolutional_code)
    {
        nbytes = nbits/16;
    }
    else
    {
        nbytes = nbits/8;
    }*/

    for(i=0; i<blocksize; i++)
    {
        if(cc->sample_count == 0)
        {
            if(cc->bit_count == 0)
            {
                if(cc->sending)
                {
                    if(cc->preamble_len > 0)
                    {
                        static unsigned char i = 0;
                        cc->current_data = sequence[i%255];
                        i++;
                        cc->preamble_len--;
                    }
                    else if(!fifo_isempty(&(cc->tx_fifo)))
                    {
                        cc->current_data = fifo_pop(&(cc->tx_fifo));
                        cc->trailer_len = cc->cfg_trailer_len;
                    }
                    else if(cc->trailer_len > 0)
                    {
                        static unsigned char i = 0;
                        cc->current_data = sequence[i%255];
                        i++;
                        if(!cc->cfg_continous) cc->trailer_len--;
                    }
                    else
                    {
                        cc->sending = 0;
                        TX_DIS();//Turn off transmitter
                    }
                }
                else
                {
                    if(cc->cfg_padding_zero)
                    {
                        cc->current_data = 0;//输出0
                    }
                    else
                    {
                        break;
                    }
                }

                if(cc->cfg_using_m)
                {
                    cc->current_data = m_encode_tab[cc->current_data];

                    if(cc->tx_bit_state)
                    {
                        cc->current_data = 0xFF-cc->current_data;
                    }

                    cc->tx_bit_state = cc->current_data&0x01;
                }

                /*if(cc->cfg_using_convolutional_code)
                {
                    encode27(&(cc->encstate), symbols, &current_data, 1);
                    symbols += 16;
                }
                else
                {
                    for(j=0; j<8; j++)
                    {
                        if(current_data & 0x80)
                        {
                            *(symbols++) = 1;
                        }
                        else
                        {
                            *(symbols++) = 0;
                        }
                        current_data <<= 1;
                    }
                }*/
                cc->bit_count = 8;
            }

            cc->phase_inc = (cc->current_data & 0x80) ? MARK_INC : SPACE_INC;
            cc->current_data <<= 1;
            cc->bit_count--;

            cc->sample_count = DAC_SAMPLEPERBIT;
        }
        /* Get new sample and put it out on the DAC */
        cc->phase_acc += cc->phase_inc;
        cc->phase_acc %= SIN_LEN;

        cc->sample_count--;

        *pDst = ((float)sinetable[cc->phase_acc])/32768;

        pDst++;
    }

    /*if(cc->cfg_using_convolutional_code)
    {
        return i * 16;
    }
    else
    {
        return i * 8;
    }*/

    return i;
}

/*blocksize must be a multiple of 16*/
//void ccsds_rx_proc(Ccsds *cc, unsigned char *syms, unsigned int n_syms)
void ccsds_afsk_rx_proc(Ccsds_afsk *cc, float *pSrc, unsigned int blocksize)
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

		cc->iir_x[0] = cc->iir_x[1];

		cc->iir_x[1] = ((int32_t)fifo_pop_q15(&cc->delay_fifo) * (int32_t)((*pSrc)*32768)) >> 2;

		cc->iir_y[0] = cc->iir_y[1];

		/*
		 * This should be (af->iir_y[0] * 0.438) but
		 * (af->iir_y[0] >> 1) is a faster approximation :-)
		 */
		cc->iir_y[1] = cc->iir_x[0] + cc->iir_x[1] + (cc->iir_y[0] >> 1);

		/* Save this sampled bit in a delay line */
		cc->sampled_bits <<= 1;
		cc->sampled_bits |= (cc->iir_y[1] > 0) ? 0 : 1;

		/* Store current ADC sample in the af->delay_fifo */
		fifo_push_q15(&cc->delay_fifo, (int16_t)((*pSrc)*32768));

		/* If there is an edge, adjust phase sampling */
		if (EDGE_FOUND(cc->sampled_bits))
		{
			if (cc->curr_phase < PHASE_THRES)
				cc->curr_phase += PHASE_INC;
			else
				cc->curr_phase -= PHASE_INC;
		}
		cc->curr_phase += PHASE_BIT;

		/* sample the bit */
		if (cc->curr_phase >= PHASE_MAX)
		{
			cc->curr_phase %= PHASE_MAX;

			/* Shift 1 position in the shift register of the found bits */
			//cc->found_bits <<= 1;

            if(cc->bit_count_rx == 0)
            {
                ccsds_rxwrite(cc, cc->current_byte_rx);
                cc->bit_count_rx = 8;
            }
            cc->bit_count_rx--;
            cc->current_byte_rx <<= 1;
            //cc->current_byte_rx += (*(syms++))?1:0;

			/*
			* Determine bit value by reading the last 3 sampled bits.
			* If the number of ones is two or greater, the bit value is a 1,
			* otherwise is a 0.
			* This algorithm presumes that there are 8 samples per bit.
			*/
			bits = (cc->sampled_bits & 0x07);
			if (bits == 0x07 // 111, 3 bits set to 1
			|| bits == 0x06 // 110, 2 bits
			|| bits == 0x05 // 101, 2 bits
			|| bits == 0x03 // 011, 2 bits
			)
				//cc->found_bits |= 1;
				cc->current_byte_rx += 1;

			/*
			* NRZI coding: if 2 consecutive bits have the same value
			* a 1 is received, otherwise it's a 0.
			*/
			//if (!hdlc_parse(&af->hdlc, !EDGE_FOUND(af->found_bits), &af->rx_fifo))
			//	af->status |= AFSK_RXFIFO_OVERRUN;

		}

        blocksize--;
		pSrc++;
    }
/*    unsigned int i, j;
    unsigned char current_out, syms_tmp[16];


    if(cc->cfg_using_convolutional_code)
    {
        for (i = 0; i < n_syms/16; i++)
        {
            for(j=0; j<16; j++)
            {
                syms_tmp[j] = ((*(syms++)) ? 0xff : 0x00);
            }

            vitfilt27_decode(&(cc->vi), syms_tmp, &current_out, 16);

            if(cc->cfg_using_m)
            {
                if(cc->rx_bit_state)
                {
                    cc->rx_bit_state = current_out&0x01;
                    current_out = m_decode_tab[0xFF-current_out];
                }
                else
                {
                    cc->rx_bit_state = current_out&0x01;
                    current_out = m_decode_tab[current_out];
                }
            }

            ccsds_rxwrite(cc, current_out);
        }
    }
    else
    {
        for (i = 0; i < n_syms/8; i++)
        {
            for(j=0; j<8; j++)
            {
                current_out <<= 1;
                current_out += (*(syms++))?1:0;
            }

            if(cc->cfg_using_m)
            {
                if(cc->rx_bit_state)
                {
                    cc->rx_bit_state = current_out&0x01;
                    current_out = m_decode_tab[0xFF-current_out];
                }
                else
                {
                    cc->rx_bit_state = current_out&0x01;
                    current_out = m_decode_tab[current_out];
                }
            }

            ccsds_rxwrite(cc, current_out);
        }
    }*/
}

void ccsds_afsk_send(Ccsds_afsk *cc, uint8_t *message)
{
    int i;

    #ifdef USING_RS
    static uint8_t rs_data[RS_LENGTH];

    encode_rs(message, rs_data, RS_BLOCK_LENGTH-RS_LENGTH-cc->len_frame );

    ccsds_txwrite(cc, (uint8_t)(((cc->sync_word)&0xff000000) >> 24));
    ccsds_txwrite(cc, (uint8_t)(((cc->sync_word)&0x00ff0000) >> 16));
    ccsds_txwrite(cc, (uint8_t)(((cc->sync_word)&0x0000ff00) >> 8));
    ccsds_txwrite(cc, (uint8_t)((cc->sync_word)&0x000000ff));

    for(i=0; i<cc->len_frame; i++) ccsds_txwrite(cc, message[i]^sequence[i%255]);
    for(i=0; i<RS_LENGTH; i++) ccsds_txwrite(cc, rs_data[i]^sequence[(i+cc->len_frame)%255]);
    #else
    ccsds_txwrite(cc, (uint8_t)(((cc->sync_word)&0xff000000) >> 24));
    ccsds_txwrite(cc, (uint8_t)(((cc->sync_word)&0x00ff0000) >> 16));
    ccsds_txwrite(cc, (uint8_t)(((cc->sync_word)&0x0000ff00) >> 8));
    ccsds_txwrite(cc, (uint8_t)((cc->sync_word)&0x000000ff));

    for(i=0; i<cc->len_frame; i++) ccsds_txwrite(cc, message[i]^sequence[i%255]);
    #endif
}

void ccsds_afsk_pull(Ccsds_afsk *cc)
{
    uint8_t current_in;
    int16_t byte_corr = 0;


    //while((!fifo_isempty(&(cc->rx_fifo))) || mask_bit_in)
    while(!fifo_isempty(&(cc->rx_fifo)))
    {
        uint8_t mask_bit_in = 0x80;

        current_in = fifo_pop(&(cc->rx_fifo));

        for(int i=0; i<8; i++)
        {
            if(cc->syncing)
            {
                if(current_in & mask_bit_in)
                {
                    cc->buf_sync_out[cc->n_out] |= cc->mask_bit_out;
                }
                else
                {
                    cc->buf_sync_out[cc->n_out] &= (~cc->mask_bit_out);
                }

                cc->mask_bit_out >>= 1;

                if(cc->mask_bit_out == 0x00)
                {
                    cc->mask_bit_out = 0x80;
                    cc->n_out++;

                    #ifdef USING_RS
                    if(cc->n_out == cc->len_frame+RS_LENGTH)
                    {
                        cc->syncing = 0;
						for(int i=0; i<cc->len_frame+RS_LENGTH; i++) cc->buf_sync_out[i] ^= sequence[i%255];
                        byte_corr = decode_rs(cc->buf_sync_out, (int *)0, 0, RS_BLOCK_LENGTH-cc->len_frame-RS_LENGTH);
                        cc->hook(cc->buf_sync_out, cc->len_frame, byte_corr, cc->obj_ptr);
                    }
                    #else
                    if(cc->n_out == cc->len_frame)
                    {
                        cc->syncing = 0;
						for(int i=0; i<cc->len_frame; i++) cc->buf_sync_out[i] ^= sequence[i%255];
                        cc->hook(cc->buf_sync_out, cc->len_frame, byte_corr, cc->obj_ptr);
                    }
                    #endif
                }
            }
            else
            {
                cc->buffer_sync_det <<= 1;

                if(current_in & mask_bit_in)
                {
                    cc->buffer_sync_det += 1;
                }
                /* Allow no error in sync word */
                /*     if(cc->buffer_sync_det == cc->sync_word)
                     {
                         cc->syncing = 1;
                         cc->buffer_sync_det = 0;
                         cc->mask_bit_out = 0x80;
                         cc->n_out = 0;
                         cc->len_frame = 3;
                     } */

                /* Allow 1 error in sync word */
                switch(cc->buffer_sync_det ^ cc->sync_word)
                {
                    case 0x00000000:
                    case 0x00000001:
                    case 0x00000002:
                    case 0x00000004:
                    case 0x00000008:
                    case 0x00000010:
                    case 0x00000020:
                    case 0x00000040:
                    case 0x00000080:
                    case 0x00000100:
                    case 0x00000200:
                    case 0x00000400:
                    case 0x00000800:
                    case 0x00001000:
                    case 0x00002000:
                    case 0x00004000:
                    case 0x00008000:
                    case 0x00010000:
                    case 0x00020000:
                    case 0x00040000:
                    case 0x00080000:
                    case 0x00100000:
                    case 0x00200000:
                    case 0x00400000:
                    case 0x00800000:
                    case 0x01000000:
                    case 0x02000000:
                    case 0x04000000:
                    case 0x08000000:
                    case 0x10000000:
                    case 0x20000000:
                    case 0x40000000:
                    case 0x80000000:
                    {
                        cc->syncing = 1;
                        cc->buffer_sync_det = 0;
                        cc->mask_bit_out = 0x80;
                        cc->n_out = 0;
                    }
                }

            }
            mask_bit_in >>= 1;
        }
    }

}


void direwolf_ccsds_afsk_rx_proc(Ccsds_afsk *cc, float *pSrc, unsigned int blocksize)
{
	while(blocksize>0)
	{
		demod_afsk_process_sample (0, 0, (int)(*pSrc * 32767.0), &cc->direwolf_state);

		blocksize--;
		pSrc++;
	}
}

static void direwolf_afsk_demod_callback(void *obj_ptr, int demod_data)
{
    Ccsds_afsk *cc = (Ccsds_afsk *)obj_ptr;

	if(cc->bit_count_rx == 0)
    {
        ccsds_rxwrite(cc, cc->current_byte_rx);
        cc->bit_count_rx = 8;
    }
    cc->bit_count_rx--;
    cc->current_byte_rx <<= 1;

	if(demod_data) cc->current_byte_rx += 1;

    //af->found_bits <<= 1;
    //if(demod_data) af->found_bits |= 1;
    //hdlc_parse(&af->hdlc, !EDGE_FOUND(af->found_bits), &af->rx_fifo);
}

void direwolf_ccsds_afsk_init(Ccsds_afsk *cc, uint32_t sync_word, uint16_t len_frame, void *obj_ptr, afsk_sync_hook_t hook)
{
    //float RATE=0.5;
    //float ebn0 = 12.0;
    //float esn0 = RATE*pow(10.0, ebn0/10);


    //gen_met(mettab, amp, esn0, 0.0, 4);

	demod_afsk_init (SAMPLERATE, BITRATE, MARK_FREQ, SPACE_FREQ, 'G', &cc->direwolf_state, (void *)cc, direwolf_afsk_demod_callback);

    vitfilt27_init(&(cc->vi));

    cc->sending = 0;
    fifo_init(&cc->tx_fifo, cc->tx_buf, sizeof(cc->tx_buf));
    fifo_init(&cc->rx_fifo, cc->rx_buf, sizeof(cc->rx_buf));

    //fifo_init_q15(&cc->delay_fifo, (int16_t *)cc->delay_buf, (SAMPLEPERBIT / 2 + 1));
	/* Fill sample FIFO with 0 */
	//for (int i = 0; i < SAMPLEPERBIT / 2; i++)
	//	fifo_push_q15(&cc->delay_fifo, 0);

    cc->sync_word = sync_word;
    cc->len_frame = len_frame;
    cc->syncing = 0;
    cc->buffer_sync_det = 0;
    cc->n_out = 0;
    cc->mask_bit_out = 0x80;
    cc->obj_ptr = obj_ptr;
    cc->hook = hook;
    cc->rx_bit_state = 0;
    cc->tx_bit_state = 0;

    cc->cfg_continous = 0;
    cc->cfg_padding_zero = 0;
    cc->cfg_using_m = 0;
    cc->cfg_using_convolutional_code = 0;
    cc->cfg_preamble_len = 75;
    cc->cfg_trailer_len = 30;

    cc->sample_count = 0;
    cc->bit_count = 0;
    cc->current_data = 0;
    cc->phase_inc = SPACE_INC;
    cc->phase_acc = 0;

    cc->bit_count_rx = 0;
    cc->curr_phase = 0;
}

void ccsds_afsk_init_dpd(Ccsds_afsk *cc, uint32_t sync_word, uint16_t len_frame, void *obj_ptr, afsk_sync_hook_t hook)
{
    //float RATE=0.5;
    //float ebn0 = 12.0;
    //float esn0 = RATE*pow(10.0, ebn0/10);


    //gen_met(mettab, amp, esn0, 0.0, 4);

    vitfilt27_init(&(cc->vi));

    cc->sending = 0;
    fifo_init(&cc->tx_fifo, cc->tx_buf, sizeof(cc->tx_buf));
    fifo_init(&cc->rx_fifo, cc->rx_buf, sizeof(cc->rx_buf));

    //fifo_init_q15(&cc->delay_fifo, (int16_t *)cc->delay_buf, (SAMPLEPERBIT / 2 + 1));
	/* Fill sample FIFO with 0 */
	//for (int i = 0; i < SAMPLEPERBIT / 2; i++)
	//	fifo_push_q15(&cc->delay_fifo, 0);

    fifo_init_q15(&cc->delay_fifo_i, (int16_t *)cc->delay_buf_i, SAMPLEPERBIT+1);
    for (int i = 0; i < SAMPLEPERBIT; i++)
		fifo_push_q15(&cc->delay_fifo_i, 0);

    fifo_init_q15(&cc->delay_fifo_q, (int16_t *)cc->delay_buf_q, SAMPLEPERBIT+1);
    for (int i = 0; i < SAMPLEPERBIT; i++)
		fifo_push_q15(&cc->delay_fifo_q, 0);

    cc->phase_acc_lo = 0;

    cc->sync_word = sync_word;
    cc->len_frame = len_frame;
    cc->syncing = 0;
    cc->buffer_sync_det = 0;
    cc->n_out = 0;
    cc->mask_bit_out = 0x80;
    cc->obj_ptr = obj_ptr;
    cc->hook = hook;
    cc->rx_bit_state = 0;
    cc->tx_bit_state = 0;

    cc->cfg_continous = 0;
    cc->cfg_padding_zero = 0;
    cc->cfg_using_m = 0;
    cc->cfg_using_convolutional_code = 0;
    cc->cfg_preamble_len = 75;
    cc->cfg_trailer_len = 30;

    cc->sample_count = 0;
    cc->bit_count = 0;
    cc->current_data = 0;
    cc->phase_inc = SPACE_INC;
    cc->phase_acc = 0;

    cc->bit_count_rx = 0;
    cc->curr_phase = 0;
}

#if SAMPLEPERBIT==8
	//int16_t taps_receiver_filter[LEN_RECEIVER_FILTER] = {-39, -253, -401, -395, -200, 140, 500, 714, 645, 246, -388, -1032, -1386, -1181, -276, 1266, 3170, 5015, 6352, 6840, 6352, 5015, 3170, 1266, -276, -1181, -1386, -1032, -388, 246, 645, 714, 500, 140, -200, -395, -401, -253, -39};
	int16_t taps_receiver_lpf[LEN_RECEIVER_LPF] = {-7, 174, 319, 386, 351, 214, 7, -221, -407, -497, -455, -280, -7, 301, 561, 695, 649, 407, 7, -468, -900, -1159, -1132, -751, -7, 1037, 2255, 3477, 4520, 5221, 5468, 5221, 4520, 3477, 2255, 1037, -7, -751, -1132, -1159, -900, -468, 7, 407, 649, 695, 561, 301, -7, -280, -455, -497, -407, -221, 7, 214, 351, 386, 319, 174, -7};
	#ifdef BT0R35
		int16_t taps_pulse_sharping[LEN_PULSE_SHARPING] = {2, 6, 19, 52, 132, 299, 607, 1105, 1804, 2643, 3471, 4087, 4316, 4087, 3471, 2643, 1804, 1105, 607, 299, 132, 52, 19, 6, 2};
	#else
		int16_t taps_pulse_sharping[LEN_PULSE_SHARPING] = {1, 5, 26, 112, 382, 1040, 2266, 3952, 5517, 6166, 5517, 3952, 2266, 1040, 382, 112, 26, 5, 1};
	#endif
#endif

#if SAMPLEPERBIT==4
	//int16_t taps_receiver_filter[LEN_RECEIVER_FILTER] = {-67, -99, 64, 214, 17, -336, -212, 399, 533, -310, -960, -48, 1431, 860, -1863, -2650, 2168, 10138, 14107, 10138, 2168, -2650, -1863, 860, 1431, -48, -960, -310, 533, 399, -212, -336, 17, 214, 64, -99, -67};
	#ifdef BT0R35
		int16_t taps_pulse_sharping[LEN_PULSE_SHARPING] = {3, 37, 264, 1213, 3609, 6941, 8632, 6941, 3609, 1213, 264, 37, 3};
	#else
		int16_t taps_pulse_sharping[LEN_PULSE_SHARPING] = {10, 225, 2080, 7903, 12332, 7903, 2080, 225, 10};
	#endif
#endif

/*blocksize must be a multiple of 16*/
//void ccsds_rx_proc(Ccsds *cc, unsigned char *syms, unsigned int n_syms)
void ccsds_afsk_rx_proc_dpd(Ccsds_afsk *cc, float *pSrc, unsigned int blocksize)
{
    while(blocksize>0)
    {
        uint8_t bits;

		int16_t mix_q = (int16_t)(((float)sinetable[cc->phase_acc_lo]) * (*pSrc) / 2.0);
        int16_t mix_i = (int16_t)(((float)sinetable[(cc->phase_acc_lo + SIN_LEN/4) % SIN_LEN]) * (*pSrc) / 2.0);

        cc->phase_acc_lo += FC_PHASE_INC;
        cc->phase_acc_lo %= SIN_LEN;

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

		int32_t acc_receiver_lpf_q = 0;
		for(int j=0; j<LEN_RECEIVER_LPF-1; j++)
		{
			cc->d_receiver_lpf_q[j] = cc->d_receiver_lpf_q[j+1];
			acc_receiver_lpf_q += ((int32_t)cc->d_receiver_lpf_q[j]) * ((int32_t)taps_receiver_lpf[j]);
		}
		cc->d_receiver_lpf_q[LEN_RECEIVER_LPF-1] = mix_q;
		acc_receiver_lpf_q += ((int32_t)mix_q) * ((int32_t)taps_receiver_lpf[LEN_RECEIVER_LPF-1]);
        
		int32_t acc_receiver_lpf_i = 0;
		for(int j=0; j<LEN_RECEIVER_LPF-1; j++)
		{
			cc->d_receiver_lpf_i[j] = cc->d_receiver_lpf_i[j+1];
			acc_receiver_lpf_i += ((int32_t)cc->d_receiver_lpf_i[j]) * ((int32_t)taps_receiver_lpf[j]);
		}
		cc->d_receiver_lpf_i[LEN_RECEIVER_LPF-1] = mix_i;
		acc_receiver_lpf_i += ((int32_t)mix_i) * ((int32_t)taps_receiver_lpf[LEN_RECEIVER_LPF-1]);

		int64_t demod_out = ((int64_t)acc_receiver_lpf_i * (int64_t)fifo_pop_q15(&cc->delay_fifo_q)) - ((int64_t)acc_receiver_lpf_q * (int64_t)fifo_pop_q15(&cc->delay_fifo_i));

		fifo_push_q15(&cc->delay_fifo_q, (int16_t)(acc_receiver_lpf_q>>15));
        fifo_push_q15(&cc->delay_fifo_i, (int16_t)(acc_receiver_lpf_i>>15));

		/*
		int32_t demod_out = (((int32_t)mix_i * (int32_t)fifo_pop_q15(&cc->delay_fifo_q)) >> 2) - (((int32_t)mix_q * (int32_t)fifo_pop_q15(&cc->delay_fifo_i)) >> 2);

		fifo_push_q15(&cc->delay_fifo_q, mix_q);
        fifo_push_q15(&cc->delay_fifo_i, mix_i);


		int64_t acc_receiver_filter = 0;
		for(int j=0; j<LEN_RECEIVER_FILTER-1; j++)
		{
			cc->d_receiver_filter[j] = cc->d_receiver_filter[j+1];
			acc_receiver_filter += ((int64_t)cc->d_receiver_filter[j]) * ((int64_t)taps_receiver_filter[j]);
		}
		cc->d_receiver_filter[LEN_RECEIVER_FILTER-1] = demod_out;
		acc_receiver_filter += ((int64_t)demod_out) * ((int64_t)taps_receiver_filter[LEN_RECEIVER_FILTER-1]);
		*/

		//cc->iir_x[0] = cc->iir_x[1];

		//cc->iir_x[1] = demod_out;

		//cc->iir_y[0] = cc->iir_y[1];

		/*
		 * This should be (af->iir_y[0] * 0.438) but
		 * (af->iir_y[0] >> 1) is a faster approximation :-)
		 */
		//cc->iir_y[1] = cc->iir_x[0] + cc->iir_x[1] + (cc->iir_y[0] >> 1);

		/* Save this sampled bit in a delay line */
		cc->sampled_bits <<= 1;
		//cc->sampled_bits |= (cc->iir_y[1] > 0) ? 1 : 0;
		//cc->sampled_bits |= (acc_receiver_filter > 0) ? 0 : 1;
		cc->sampled_bits |= (demod_out > 0) ? 0 : 1;

		/* Store current ADC sample in the af->delay_fifo */
		//fifo_push_q15(&cc->delay_fifo, (int16_t)((*pSrc)*32768));

		/* If there is an edge, adjust phase sampling */
		if (EDGE_FOUND(cc->sampled_bits))
		{
			if (cc->curr_phase < PHASE_THRES)
				cc->curr_phase += PHASE_INC;
			else
				cc->curr_phase -= PHASE_INC;
		}
		cc->curr_phase += PHASE_BIT;

		/* sample the bit */
		if (cc->curr_phase >= PHASE_MAX)
		{
			cc->curr_phase %= PHASE_MAX;

			/* Shift 1 position in the shift register of the found bits */
			//cc->found_bits <<= 1;

            if(cc->bit_count_rx == 0)
            {
                ccsds_rxwrite(cc, cc->current_byte_rx);
                cc->bit_count_rx = 8;
            }
            cc->bit_count_rx--;
            cc->current_byte_rx <<= 1;
            //cc->current_byte_rx += (*(syms++))?1:0;

			/*
			* Determine bit value by reading the last 3 sampled bits.
			* If the number of ones is two or greater, the bit value is a 1,
			* otherwise is a 0.
			* This algorithm presumes that there are 8 samples per bit.
			*/
			bits = (cc->sampled_bits & 0x07);
			if (bits == 0x07 // 111, 3 bits set to 1
			|| bits == 0x06 // 110, 2 bits
			|| bits == 0x05 // 101, 2 bits
			|| bits == 0x03 // 011, 2 bits
			)
				//cc->found_bits |= 1;
				cc->current_byte_rx += 1;

			/*
			* NRZI coding: if 2 consecutive bits have the same value
			* a 1 is received, otherwise it's a 0.
			*/
			//if (!hdlc_parse(&af->hdlc, !EDGE_FOUND(af->found_bits), &af->rx_fifo))
			//	af->status |= AFSK_RXFIFO_OVERRUN;

		}

        blocksize--;
		pSrc++;
    }
/*    unsigned int i, j;
    unsigned char current_out, syms_tmp[16];


    if(cc->cfg_using_convolutional_code)
    {
        for (i = 0; i < n_syms/16; i++)
        {
            for(j=0; j<16; j++)
            {
                syms_tmp[j] = ((*(syms++)) ? 0xff : 0x00);
            }

            vitfilt27_decode(&(cc->vi), syms_tmp, &current_out, 16);

            if(cc->cfg_using_m)

            {
                if(cc->rx_bit_state)
                {
                    cc->rx_bit_state = current_out&0x01;
                    current_out = m_decode_tab[0xFF-current_out];
                }
                else
                {
                    cc->rx_bit_state = current_out&0x01;
                    current_out = m_decode_tab[current_out];
                }
            }

            ccsds_rxwrite(cc, current_out);
        }
    }
    else
    {
        for (i = 0; i < n_syms/8; i++)
        {
            for(j=0; j<8; j++)
            {
                current_out <<= 1;
                current_out += (*(syms++))?1:0;
            }

            if(cc->cfg_using_m)
            {
                if(cc->rx_bit_state)
                {
                    cc->rx_bit_state = current_out&0x01;
                    current_out = m_decode_tab[0xFF-current_out];
                }
                else
                {
                    cc->rx_bit_state = current_out&0x01;
                    current_out = m_decode_tab[current_out];

                }
            }

            ccsds_rxwrite(cc, current_out);
        }
    }*/
}

//unsigned int ccsds_tx_proc(Ccsds *cc, unsigned char *symbols, unsigned int nbits)
unsigned int ccsds_afsk_tx_proc_gmsk(Ccsds_afsk *cc, float *pDst, unsigned int blocksize)
{
    //unsigned char current_data;
    //unsigned int i, j, nbytes;
    unsigned int i;

    /*if(cc->cfg_using_convolutional_code)
    {
        nbytes = nbits/16;
    }
    else
    {
        nbytes = nbits/8;
    }*/

    for(i=0; i<blocksize; i++)
    {
        if(cc->sample_count == 0)
        {
            if(cc->bit_count == 0)
            {
                if(cc->sending)
                {
                    if(cc->preamble_len > 0)
                    {
                        static unsigned char i = 0;
                        cc->current_data = sequence[i%255];
                        i++;
                        cc->preamble_len--;
                    }
                    else if(!fifo_isempty(&(cc->tx_fifo)))
                    {
                        cc->current_data = fifo_pop(&(cc->tx_fifo));
                        cc->trailer_len = cc->cfg_trailer_len;
                    }
                    else if(cc->trailer_len > 0)
                    {
                        static unsigned char i = 0;
                        cc->current_data = sequence[i%255];
                        i++;
                        if(!cc->cfg_continous) cc->trailer_len--;
                    }
                    else
                    {
                        cc->sending = 0;
                        TX_DIS();//Turn off transmitter
                    }
                }
                else
                {
                    if(cc->cfg_padding_zero)
                    {
                        cc->current_data = 0;//输出0
                    }
                    else
                    {
                        break;
                    }
                }

                if(cc->cfg_using_m)
                {
                    cc->current_data = m_encode_tab[cc->current_data];

                    if(cc->tx_bit_state)
                    {
                        cc->current_data = 0xFF-cc->current_data;
                    }

                    cc->tx_bit_state = cc->current_data&0x01;
                }

                /*if(cc->cfg_using_convolutional_code)
                {
                    encode27(&(cc->encstate), symbols, &current_data, 1);
                    symbols += 16;
                }
                else
                {
                    for(j=0; j<8; j++)
                    {
                        if(current_data & 0x80)
                        {
                            *(symbols++) = 1;
                        }
                        else
                        {
                            *(symbols++) = 0;
                        }
                        current_data <<= 1;
                    }
                }*/
                cc->bit_count = 8;
            }

            cc->phase_inc = (cc->current_data & 0x80) ? DIVISION_INC : -DIVISION_INC;
            cc->current_data <<= 1;
            cc->bit_count--;

            cc->sample_count = DAC_SAMPLEPERBIT;
        }
        /* Get new sample and put it out on the DAC */

		int32_t acc_pulse_sharping = 0;
		for(int j=0; j<LEN_PULSE_SHARPING-1; j++)
		{
			cc->d_pulse_sharping[j] = cc->d_pulse_sharping[j+1];
			acc_pulse_sharping += ((int32_t)cc->d_pulse_sharping[j]) * ((int32_t)taps_pulse_sharping[j]);
		}
		cc->d_pulse_sharping[LEN_PULSE_SHARPING-1] = cc->phase_inc;
		acc_pulse_sharping += ((int32_t)cc->phase_inc) * ((int32_t)taps_pulse_sharping[LEN_PULSE_SHARPING-1]);

        cc->phase_acc += ((int16_t)((acc_pulse_sharping + 16384) >> 15)) + FC_PHASE_INC;
		//cc->phase_acc += cc->phase_inc + FC_PHASE_INC;
        cc->phase_acc %= SIN_LEN;

        cc->sample_count--;

        *pDst = ((float)sinetable[cc->phase_acc])/57344.0;

        pDst++;
    }

    /*if(cc->cfg_using_convolutional_code)
    {
        return i * 16;
    }
    else
    {
        return i * 8;
    }*/

    return i;
}

