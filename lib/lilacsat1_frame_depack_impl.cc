/* -*- c++ -*- */
/* 
 * Copyright 2015 WEI Mingchuan, BG2BHC <bg2bhc@gmail.com>
 * Copyright 2015 HIT Research Center of Satellite Technology
 * Copyright 2015 HIT Amateur Radio Club, BY2HIT
 *
 * Harbin Institute of Technology <http://www.hit.edu.cn/>
 * LilacSat - HIT Student Satellites <http://lilacsat.hit.edu.cn/>
 * HIT Amateur Radio Club <http://www.by2hit.net/>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "lilacsat1_frame_depack_impl.h"

#include <stdio.h>

namespace gr {
  namespace lilacsat {

    lilacsat1_frame_depack::sptr
    lilacsat1_frame_depack::make()
    {
      return gnuradio::get_initial_sptr
        (new lilacsat1_frame_depack_impl());
    }

    /*
     * The private constructor
     */
    lilacsat1_frame_depack_impl::lilacsat1_frame_depack_impl()
      : gr::sync_block("lilacsat1_frame_depack",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, 8*sizeof(char)))
    {
		d_in_port = pmt::mp("in");
		message_port_register_in(d_in_port);

		d_out_port = pmt::mp("out");	      
		message_port_register_out(d_out_port);

		set_msg_handler(d_in_port, boost::bind(&lilacsat1_frame_depack_impl::pmt_in_callback, this ,_1) );

		kiss_init(&d_ki, (void *)this, &lilacsat1_frame_depack_impl::kiss_msg_callback);
		fifo_init(&d_fifo, d_buf, LENTH_BUF_CODEC2);

		d_term = 1;
	}

    /*
     * Our virtual destructor.
     */
    lilacsat1_frame_depack_impl::~lilacsat1_frame_depack_impl()
    {
    }

    void lilacsat1_frame_depack_impl::kiss_msg_callback(void *obj_ptr, char *ptr, uint16_t len)
    {
		lilacsat1_frame_depack_impl *obj_ptr_loc = (lilacsat1_frame_depack_impl *)obj_ptr;
		obj_ptr_loc->message_port_pub(obj_ptr_loc->d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(len, (const uint8_t *)ptr)));
    }

    void lilacsat1_frame_depack_impl::pmt_in_callback(pmt::pmt_t msg)
    {
		pmt::pmt_t meta(pmt::car(msg));
		pmt::pmt_t bytes(pmt::cdr(msg));

		size_t msg_len, n_path;
		const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);
		int i, j;
		if(msg_len != 116)
		{
			return;
		}

		for(i=0; i<msg_len; i++)
		{
			uint8_t di = (i + 4) % 24;

			if(di >= 17)
			{
				d_out_c2[di-17] = bytes_in[i];

				if(di == 23)
				{
					if(d_out_c2[6] & 0x01)
					{
						d_out_c2[6] &= 0xf0;
						if(fifo_room(&d_fifo) >= 7)
						{
							for(j=0; j<7; j++)
							{
								fifo_push(&d_fifo, d_out_c2[j]);
							}
						}
					}
				}
			}
			else
			{
				kiss_rx_byte(&d_ki, bytes_in[i]);
			}
		}
    }

    int
    lilacsat1_frame_depack_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        char *out = (char *) output_items[0];
		int i, j, k=0;

        // Do <+signal processing+>
		for(i=0; i<noutput_items; i++)
		{
			if(d_term)
			{
				if((fifo_len(&d_fifo) - fifo_room(&d_fifo)) >= 70)
				{
					for(j=0; j<7; j++)
					{
						out[8*i+j] = fifo_pop(&d_fifo);
					}
					d_term = 0;
				}
				else
				{
					d_term = 1;
					return i;
				}
			}
			else
			{
				if((fifo_len(&d_fifo) - fifo_room(&d_fifo)) >= 7)
				{
					for(j=0; j<7; j++)
					{
						out[8*i+j] = fifo_pop(&d_fifo);
					}
					d_term = 0;
				}
				else
				{
					d_term = 1;
					return i;
				}
			} 
		}
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

