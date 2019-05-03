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
#include "attach_rs_codeblock_impl.h"

extern "C"
{
	#include "ccsds/ccsds.h"
}

namespace gr {
  namespace lilacsat {

    attach_rs_codeblock::sptr
    attach_rs_codeblock::make(int interleaving)
    {
      return gnuradio::get_initial_sptr
        (new attach_rs_codeblock_impl(interleaving));
    }

    /*
     * The private constructor
     */
    attach_rs_codeblock_impl::attach_rs_codeblock_impl(int interleaving)
      : gr::sync_block("attach_rs_codeblock",
              gr::io_signature::make(0, 0, sizeof(char)),
              gr::io_signature::make(0, 0, sizeof(char)))
    {
	d_in_port = pmt::mp("in");
      	message_port_register_in(d_in_port);

	d_out_port = pmt::mp("out");	      
      	message_port_register_out(d_out_port);

	set_msg_handler(d_in_port, boost::bind(&attach_rs_codeblock_impl::pmt_in_callback, this ,_1) );
    }

    /*
     * Our virtual destructor.
     */
    attach_rs_codeblock_impl::~attach_rs_codeblock_impl()
    {
    }

    void attach_rs_codeblock_impl::pmt_in_callback(pmt::pmt_t msg)
    {
	pmt::pmt_t meta(pmt::car(msg));
	pmt::pmt_t bytes(pmt::cdr(msg));

	size_t msg_len, i;
	const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);
	uint8_t bytes_out[1024], rs_block[32];

	for(i=0; i<msg_len; i++)
	{
		bytes_out[i] = bytes_in[i];
	}

	encode_rs(bytes_out, bytes_out+msg_len, RS_BLOCK_LENGTH-RS_LENGTH-msg_len );

	//bytes_in, msg_len

	attach_rs_codeblock_impl::message_port_pub(attach_rs_codeblock_impl::d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(msg_len+RS_LENGTH, (const uint8_t *)bytes_out)));
    }
    int
    attach_rs_codeblock_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

