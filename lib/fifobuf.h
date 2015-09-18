/**
 * \file
 * <!--
 * This file is part of BeRTOS.
 *
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2003, 2004 Develer S.r.l. (http://www.develer.com/)
 * Copyright 2001, 2008 Bernie Innocenti <bernie@codewiz.org>
 * -->
 *
 * \brief General pourpose FIFO buffer implemented with a ring buffer
 *
 * \li \c begin points to the first buffer element;
 * \li \c end points to the last buffer element (unlike the STL convention);
 * \li \c head points to the element to be extracted next;
 * \li \c tail points to the location following the last insertion;
 * \li when any of the pointers advances beyond \c end, it is reset
 *     back to \c begin.
 *
 * \code
 *
 *  +-----------------------------------+
 *  |  empty  |   valid data   |  empty |
 *  +-----------------------------------+
 *  ^         ^                ^        ^
 *  begin    head             tail     end
 *
 * \endcode
 *
 * The buffer is EMPTY when \c head and \c tail point to the same location:
 *		\code head == tail \endcode
 *
 * The buffer is FULL when \c tail points to the location immediately
 * after \c head:
 *		\code tail == head - 1 \endcode
 *
 * The buffer is also FULL when \c tail points to the last buffer
 * location and head points to the first one:
 *		\code head == begin && tail == end \endcode
 *
 * \version $Id: fifobuf.h 2506 2009-04-15 08:29:07Z duplo $
 * \author Bernie Innocenti <bernie@codewiz.org>
 */

#ifndef STRUCT_FIFO_H
#define STRUCT_FIFO_H

#include "bertos.h"

#include <inttypes.h>

//#include <cpu/irq.h>
//#incsude <cfg/debug.h>


typedef struct FIFOBuffer
{
	unsigned char * volatile head;
	unsigned char * volatile tail;
	unsigned char *begin;
	unsigned char *end;
} FIFOBuffer;

typedef struct FIFOBuffer_q15
{
	int16_t * volatile head;
	int16_t * volatile tail;
	int16_t *begin;
	int16_t *end;
} FIFOBuffer_q15;

#define ASSERT_VALID_FIFO(fifo) \
	ATOMIC( \
		ASSERT((fifo)->head >= (fifo)->begin); \
		ASSERT((fifo)->head <= (fifo)->end); \
		ASSERT((fifo)->tail >= (fifo)->begin); \
		ASSERT((fifo)->tail <= (fifo)->end); \
	)

inline bool fifo_isempty(const FIFOBuffer *fb);
inline bool fifo_isfull(const FIFOBuffer *fb);
inline void fifo_push(FIFOBuffer *fb, unsigned char c);
inline void fifo_push_q15(FIFOBuffer_q15 *fb, int16_t c);
inline unsigned char fifo_pop(FIFOBuffer *fb);
inline int16_t fifo_pop_q15(FIFOBuffer_q15 *fb);
inline void fifo_flush(FIFOBuffer *fb);
inline void fifo_init(FIFOBuffer *fb, unsigned char *buf, uint32_t size);
inline void fifo_init_q15(FIFOBuffer_q15 *fb, int16_t *buf, uint32_t size);
inline uint32_t fifo_len(FIFOBuffer *fb);


#endif /* STRUCT_FIFO_H */
