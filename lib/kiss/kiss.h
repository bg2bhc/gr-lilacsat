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

#ifndef __KISS_H__
#define __KISS_H__

#include <inttypes.h>

#define LEN_KISS_BUFFER 1024

typedef void (*kiss_hook_t)(void *obj_ptr, char *ptr, uint16_t len);

typedef struct Kiss
{
	uint8_t escaping_rx;
	char buffer_rx[LEN_KISS_BUFFER];
	uint16_t len_rx;
	void *obj_ptr;
	kiss_hook_t hook;
} Kiss;

void kiss_init(Kiss *ki, void *obj_ptr, kiss_hook_t hook);
void kiss_rx_byte(Kiss *ki, unsigned char input);
uint16_t kiss_tx_packet(unsigned char *packet_in, uint16_t len, unsigned char *packet_out);

#endif

