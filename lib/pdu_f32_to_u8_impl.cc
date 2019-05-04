/* -*- c++ -*- */
/* 
 * Copyright 2019 <+YOU OR YOUR COMPANY+>.
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
#include "pdu_f32_to_u8_impl.h"

namespace gr {
  namespace lilacsat {

    pdu_f32_to_u8::sptr
    pdu_f32_to_u8::make()
    {
      return gnuradio::get_initial_sptr
        (new pdu_f32_to_u8_impl());
    }

    /*
     * The private constructor
     */
    pdu_f32_to_u8_impl::pdu_f32_to_u8_impl()
      : gr::sync_block("pdu_f32_to_u8",
              gr::io_signature::make(0, 0, sizeof(char)),
              gr::io_signature::make(0, 0, sizeof(char)))
    {
	d_in_port = pmt::mp("in");
      	message_port_register_in(d_in_port);

	d_out_port = pmt::mp("out");	      
      	message_port_register_out(d_out_port);

	set_msg_handler(d_in_port, boost::bind(&pdu_f32_to_u8_impl::pmt_in_callback, this ,_1) );
    }

    /*
     * Our virtual destructor.
     */
    pdu_f32_to_u8_impl::~pdu_f32_to_u8_impl()
    {
    }

    void pdu_f32_to_u8_impl::pmt_in_callback(pmt::pmt_t msg)
    {
	pmt::pmt_t meta(pmt::car(msg));
	pmt::pmt_t bytes(pmt::cdr(msg));

	size_t msg_len;
	const float* floats_in = pmt::f32vector_elements(bytes, msg_len);

	pdu_f32_to_u8_impl::message_port_pub(pdu_f32_to_u8_impl::d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(msg_len*4, (const uint8_t *)floats_in)));
    }

    int
    pdu_f32_to_u8_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

