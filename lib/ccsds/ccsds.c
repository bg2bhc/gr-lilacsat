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

#include "ccsds.h"
#include <stdio.h>

#define TX_EN()
#define TX_DIS()
#define OS_Delay()

extern char sequence[]; //伪随机序列

void ccsds_init(Ccsds *cc, uint32_t sync_word, uint16_t len_frame, void *obj_ptr, sync_hook_t hook)
{
    //float RATE=0.5;
    //float ebn0 = 12.0;
    //float esn0 = RATE*pow(10.0, ebn0/10);


    //gen_met(mettab, amp, esn0, 0.0, 4);

    vitfilt27_init(&(cc->vi));

    cc->sending = 0;
    fifo_init(&cc->tx_fifo, cc->tx_buf, sizeof(cc->tx_buf));
    fifo_init(&cc->rx_fifo, cc->rx_buf, sizeof(cc->rx_buf));

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
    cc->cfg_padding_zero = 1;
    cc->cfg_using_m = 1;
    cc->cfg_using_convolutional_code = 1;
    cc->cfg_preamble_len = 600;
    cc->cfg_trailer_len = 1200;
}


static inline void ccsds_txwrite(Ccsds *cc, unsigned char c)
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

unsigned char m_encode_tab[] =
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

unsigned char m_decode_tab[] =
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

static inline void ccsds_rxwrite(Ccsds *cc, unsigned char c)
{
    if(fifo_isfull(&(cc->rx_fifo)))
    {
        fifo_flush(&(cc->rx_fifo));
    }
    fifo_push(&(cc->rx_fifo), c);
}

unsigned int ccsds_tx_proc(Ccsds *cc, unsigned char *symbols, unsigned int nbits)
{
    unsigned char current_data;
    unsigned int i, j, nbytes;

    if(cc->cfg_using_convolutional_code)
    {
        nbytes = nbits/16;
    }
    else
    {
        nbytes = nbits/8;
    }

    for(i=0; i<nbytes; i++)
    {
        if(cc->sending)
        {
            if(cc->preamble_len > 0)
            {
                current_data = 0x55;//输出0x55
                cc->preamble_len--;
            }
            else if(!fifo_isempty(&(cc->tx_fifo)))
            {
                current_data = fifo_pop(&(cc->tx_fifo));
                cc->trailer_len = cc->cfg_trailer_len;
            }
            else if(cc->trailer_len > 0)
            {
		//static unsigned char i;
		//current_data = sequence[i++];              
		current_data = 0x55;//输出0x55
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
                current_data = 0;//输出0
            }
            else
            {
                break;
            }
        }

        if(cc->cfg_using_m)
        {
            current_data = m_encode_tab[current_data];

            if(cc->tx_bit_state)
            {
                current_data = 0xFF-current_data;
            }

            cc->tx_bit_state = current_data&0x01;
        }

        if(cc->cfg_using_convolutional_code)
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
        }
    }

    if(cc->cfg_using_convolutional_code)
    {
        return i * 16;
    }
    else
    {
        return i * 8;
    }
}

/*blocksize must be a multiple of 16*/
void ccsds_rx_proc(Ccsds *cc, unsigned char *syms, unsigned int n_syms)
{
    unsigned int i, j;
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
    }
}

void ccsds_send(Ccsds *cc, uint8_t *message)
{
    int i;
    static uint8_t rs_data[RS_LENGTH];

    encode_rs(message, rs_data, RS_BLOCK_LENGTH-RS_LENGTH-cc->len_frame );

    ccsds_txwrite(cc, (uint8_t)(((cc->sync_word)&0xff000000) >> 24));
    ccsds_txwrite(cc, (uint8_t)(((cc->sync_word)&0x00ff0000) >> 16));
    ccsds_txwrite(cc, (uint8_t)(((cc->sync_word)&0x0000ff00) >> 8));
    ccsds_txwrite(cc, (uint8_t)((cc->sync_word)&0x000000ff));

    for(i=0; i<cc->len_frame; i++) ccsds_txwrite(cc, message[i]^sequence[i]);
    for(i=0; i<RS_LENGTH; i++) ccsds_txwrite(cc, rs_data[i]^sequence[i+cc->len_frame]);
}

void ccsds_pull(Ccsds *cc)
{
    uint8_t mask_bit_in = 0x80;
    uint8_t current_in;
    int16_t byte_corr;


    while((!fifo_isempty(&(cc->rx_fifo))) || mask_bit_in)
    {
        if(mask_bit_in == 0x00)
        {
            mask_bit_in = 0x80;
        }

        if(mask_bit_in == 0x80)
        {
            current_in = fifo_pop(&(cc->rx_fifo));
        }

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

                if(cc->n_out == cc->len_frame+RS_LENGTH)
                {
                    cc->syncing = 0;
                    ccsds_xor_sequence(cc->buf_sync_out, sequence, cc->len_frame+RS_LENGTH);
                    byte_corr = decode_rs(cc->buf_sync_out, (int *)0, 0, RS_BLOCK_LENGTH-cc->len_frame-RS_LENGTH);
                    cc->hook(cc->buf_sync_out, cc->len_frame, byte_corr, cc->obj_ptr);
                }
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





