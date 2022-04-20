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

#ifndef __CCSDS_AFSK_H__
#define __CCSDS_AFSK_H__

#include <inttypes.h>

#include "ccsds/viterbi27.h"
#include "ccsds/randomizer.h"
#include "ccsds/rs.h"
#include "ccsds/tab.h"
#include "afsk/fifobuf.h"
#include "afsk/direwolf_demod_afsk.h"

//#define USING_RS

#define LENTH_BUF_SYNC_OUT 1024

#define VITERBI_RATE        2
#define VITERBI_TAIL        1
#define VITERBI_CONSTRAINT  7

#define BITS_PER_BYTE       8
#define MAX_FEC_LENGTH      255

#define RS_LENGTH           32
#define RS_BLOCK_LENGTH     255

#define PACKET_LENGTH       8
#define LENGTH_SYNC_WORD    4

#define CONFIG_CCSDS_PREAMBLE_LEN 300
#define CONFIG_CCSDS_TRAILER_LEN 100

#define CONFIG_CCSDS_TX_BUFLEN 4096
#define CONFIG_CCSDS_RX_BUFLEN 4096


// Modulator constants

#define SAMPLERATE 9600

#define BITRATE    2400

#define SAMPLEPERBIT (SAMPLERATE / BITRATE)

#define CONFIG_AFSK_DAC_SAMPLERATE 9600

#define DIV_ROUND(dividend, divisor)  (((dividend) + (divisor) / 2) / (divisor))

#define MARK_FREQ  1200
#define MARK_INC   (int16_t)(DIV_ROUND(SIN_LEN * (int32_t)MARK_FREQ, CONFIG_AFSK_DAC_SAMPLERATE))

#define SPACE_FREQ 2400
#define SPACE_INC  (int16_t)(DIV_ROUND(SIN_LEN * (int32_t)SPACE_FREQ, CONFIG_AFSK_DAC_SAMPLERATE))

#define FC_PHASE_INC  (int16_t)(DIV_ROUND(SIN_LEN * ( (int32_t)MARK_FREQ + (int32_t)SPACE_FREQ )/2, SAMPLERATE))
#define DIVISION_INC  (int16_t)(DIV_ROUND(SIN_LEN * ( (int32_t)SPACE_FREQ - (int32_t)MARK_FREQ )/2, SAMPLERATE))

#if SAMPLEPERBIT==8
	#define LEN_RECEIVER_FILTER			39 // SPS = 8
	#ifdef BT0R35
		#define LEN_PULSE_SHARPING		25 // BT = 0.35
	#else
		#define LEN_PULSE_SHARPING		19 // BT = 0.5
	#endif
#endif

#if SAMPLEPERBIT==4
	#define LEN_RECEIVER_FILTER			37 // SPS = 4
	#ifdef BT0R35
		#define LEN_PULSE_SHARPING		13 // BT = 0.35
	#else
		#define LEN_PULSE_SHARPING		9 // BT = 0.5
	#endif
#endif

#define DAC_SAMPLEPERBIT (CONFIG_AFSK_DAC_SAMPLERATE / BITRATE)

#define BIT_DIFFER(bitline1, bitline2) (((bitline1) ^ (bitline2)) & 0x01)
#define EDGE_FOUND(bitline)            BIT_DIFFER((bitline), (bitline) >> 1)

#if SAMPLEPERBIT==8
	#define PHASE_BIT    8
#endif
#if SAMPLEPERBIT==4
	#define PHASE_BIT    16
#endif

#define PHASE_INC    1

#define PHASE_MAX    (SAMPLEPERBIT * PHASE_BIT)
#define PHASE_THRES  (PHASE_MAX / 2) // - PHASE_BIT / 2)

typedef void (*afsk_sync_hook_t)(uint8_t *, uint16_t, int16_t byte_corr, void *obj_ptr);

typedef struct Ccsds_afsk
{
    FIFOBuffer tx_fifo;
    uint8_t tx_buf[CONFIG_CCSDS_TX_BUFLEN];
    uint8_t encstate;
    uint8_t sending;
    uint16_t preamble_len;
    uint16_t trailer_len;
    uint8_t sample_count;
    uint8_t bit_count;
    uint8_t current_data;
    int16_t phase_inc;
    int16_t phase_acc;
    FIFOBuffer rx_fifo;
    uint8_t rx_buf[CONFIG_CCSDS_RX_BUFLEN];
    v27 vi;
    uint8_t bit_count_rx;
    uint8_t current_byte_rx;
    FIFOBuffer_q15 delay_fifo;
    int16_t delay_buf[SAMPLEPERBIT / 2 + 1];
    int32_t iir_x[2];
    int32_t iir_y[2];
    uint8_t sampled_bits;
    int8_t curr_phase;
    uint32_t sync_word;
    uint16_t len_frame;
    uint8_t syncing;
    uint32_t buffer_sync_det;
    uint16_t n_out;
    uint8_t mask_bit_out;
    uint8_t buf_sync_out[LENTH_BUF_SYNC_OUT];
    uint8_t rx_bit_state;
    uint8_t tx_bit_state;
    void *obj_ptr;
    afsk_sync_hook_t hook;
    uint16_t cfg_preamble_len;
    uint16_t cfg_trailer_len;
    uint8_t cfg_continous;
    uint8_t cfg_padding_zero;
    uint8_t cfg_using_m;
    uint8_t cfg_using_convolutional_code;

	struct demodulator_state_s direwolf_state;

    FIFOBuffer_q15 delay_fifo_i;
    int16_t delay_buf_i[SAMPLEPERBIT + 1];
    FIFOBuffer_q15 delay_fifo_q;
    int16_t delay_buf_q[SAMPLEPERBIT + 1];
    uint16_t phase_acc_lo;
	int16_t d_pulse_sharping[LEN_PULSE_SHARPING];
	int32_t d_receiver_filter[LEN_RECEIVER_FILTER];

} Ccsds_afsk;

void ccsds_afsk_init(Ccsds_afsk *cc, uint32_t sync_word, uint16_t len_frame, void *obj_ptr, afsk_sync_hook_t hook);

void ccsds_afsk_send(Ccsds_afsk *cc, uint8_t *message);
void ccsds_afsk_pull(Ccsds_afsk *cc);

//unsigned int ccsds_tx_proc(Ccsds *cc, unsigned char *symbols, unsigned int nbytes);
unsigned int ccsds_afsk_tx_proc(Ccsds_afsk *cc, float *pDst, unsigned int blocksize);
//void ccsds_rx_proc(Ccsds *cc, unsigned char *syms, unsigned int n_syms);
void ccsds_afsk_rx_proc(Ccsds_afsk *cc, float *pSrc, unsigned int blocksize);

void direwolf_ccsds_afsk_init(Ccsds_afsk *cc, uint32_t sync_word, uint16_t len_frame, void *obj_ptr, afsk_sync_hook_t hook);
void direwolf_ccsds_afsk_rx_proc(Ccsds_afsk *cc, float *pSrc, unsigned int blocksize);

void ccsds_afsk_init_dpd(Ccsds_afsk *cc, uint32_t sync_word, uint16_t len_frame, void *obj_ptr, afsk_sync_hook_t hook);
void ccsds_afsk_rx_proc_dpd(Ccsds_afsk *cc, float *pSrc, unsigned int blocksize);
unsigned int ccsds_afsk_tx_proc_gmsk(Ccsds_afsk *cc, float *pDst, unsigned int blocksize);

#endif

