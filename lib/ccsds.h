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

#ifndef __CCSDS_H__
#define __CCSDS_H__

#include <inttypes.h>

#include "viterbi27.h"
#include "randomizer.h"
#include "rs.h"
#include "tab.h"
#include "fifobuf.h"

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

typedef void (*sync_hook_t)(uint8_t *, uint16_t, int16_t byte_corr, void *obj_ptr);

typedef struct Ccsds
{
    FIFOBuffer tx_fifo;
    uint8_t tx_buf[CONFIG_CCSDS_TX_BUFLEN];
    uint8_t encstate;
    uint8_t sending;
    uint16_t preamble_len;
    uint16_t trailer_len;
    FIFOBuffer rx_fifo;
    uint8_t rx_buf[CONFIG_CCSDS_RX_BUFLEN];
    v27 vi;
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
    sync_hook_t hook;
    uint16_t cfg_preamble_len;
    uint16_t cfg_trailer_len;
    uint8_t cfg_continous;
    uint8_t cfg_padding_zero;
    uint8_t cfg_using_m;
    uint8_t cfg_using_convolutional_code;
} Ccsds;

void ccsds_init(Ccsds *cc, uint32_t sync_word, uint16_t len_frame, void *obj_ptr, sync_hook_t hook);

void ccsds_send(Ccsds *cc, uint8_t *message);
void ccsds_pull(Ccsds *cc);

unsigned int ccsds_tx_proc(Ccsds *cc, unsigned char *symbols, unsigned int nbytes);
void ccsds_rx_proc(Ccsds *cc, unsigned char *syms, unsigned int n_syms);



#endif

