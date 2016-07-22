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

#define	KISS_FEND	0xC0
#define	KISS_FESC	0xDB
#define	KISS_TFEND	0xDC
#define	KISS_TFESC	0xDD

#include "kiss.h"

void kiss_init(Kiss *ki, void *obj_ptr, kiss_hook_t hook)
{
	ki->escaping_rx = 0;
	ki->len_rx = 0;
	ki->obj_ptr = obj_ptr;
	ki->hook = hook;
}

void kiss_rx_byte(Kiss *ki, unsigned char input)
{
	if(input == 0x09)
	{
		;
	}
	
	if(ki->escaping_rx)
	{
		ki->escaping_rx = 0;
		if(input == KISS_TFEND)
		{
			ki->buffer_rx[ki->len_rx] = KISS_FEND;
			ki->len_rx++;
		}
		else if(input == KISS_TFESC)
		{
			ki->buffer_rx[ki->len_rx] = KISS_FESC;
			ki->len_rx++;
		}
		else
		{
			ki->len_rx = 0;
		}
	}
	else if(input == KISS_FEND)
	{
		if(ki->len_rx)
		{
			ki->hook(ki->obj_ptr, ki->buffer_rx, ki->len_rx);
			ki->len_rx = 0;
		}
	}
	else if(input == KISS_FESC)
	{
		ki->escaping_rx = 1;
	}
	else
	{
		ki->buffer_rx[ki->len_rx] = input;
		ki->len_rx++;
	}
}

uint16_t kiss_tx_packet(unsigned char *packet_in, uint16_t len, unsigned char *packet_out)
{
	int i, len_out=0;
	
	*(packet_out++) = KISS_FEND;
	len_out++;
	
	for(i=0; i<len; i++)
	{
		if(packet_in[i] == KISS_FEND)
		{
			*(packet_out++) = KISS_FESC;
			*(packet_out++) = KISS_TFEND;
			len_out+=2;
		}
		else if(packet_in[i] == KISS_FESC)
		{
			*(packet_out++) = KISS_FESC;
			*(packet_out++) = KISS_TFESC;
			len_out+=2;
		}
		else
		{
			*(packet_out++) = packet_in[i];
			len_out++;
		}
	}
	*(packet_out++) = KISS_FEND;
	len_out++;
	
	return len_out;
}


