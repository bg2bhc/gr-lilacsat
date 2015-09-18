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
#include "afsk1200_rx_f_impl.h"

namespace gr {
  namespace lilacsat {

    afsk1200_rx_f::sptr
    afsk1200_rx_f::make()
    {
      return gnuradio::get_initial_sptr
        (new afsk1200_rx_f_impl());
    }

    /*
     * The private constructor
     */
    afsk1200_rx_f_impl::afsk1200_rx_f_impl()
      : gr::sync_block("afsk1200_rx_f",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(0, 0, 0))
    {
	d_out_port = pmt::mp("out");
	      
      	message_port_register_out(d_out_port);
		
	afsk1200_rx_f_impl::rx_init();
    }

    /*
     * Our virtual destructor.
     */
    afsk1200_rx_f_impl::~afsk1200_rx_f_impl()
    {
    }

    void afsk1200_rx_f_impl::AX25_MsgCallBack(void *obj_ptr, struct AX25Msg *msg)
    {
	afsk1200_rx_f_impl *obj_ptr_loc = (afsk1200_rx_f_impl *)obj_ptr;
	obj_ptr_loc->message_port_pub(obj_ptr_loc->d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(msg->len, msg->info)));
    }

    void afsk1200_rx_f_impl::rx_init()
    {
	afsk_init(&afsk);
	/*
	 * Here we initialize AX25 context, the channel (KFile) we are going to read messages
	 * from and the callback that will be called on incoming messages.
	 */
	ax25_init(&ax25, &afsk.fd, (void *)this, &afsk1200_rx_f_impl::AX25_MsgCallBack);
    }

    int
    afsk1200_rx_f_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        float *in = (float *) input_items[0];

        afsk_rx_proc(&afsk, in, noutput_items);
        ax25_poll(&ax25);
				
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

