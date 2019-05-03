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
#include "two_bit_dpd_precoder_impl.h"

namespace gr {
  namespace lilacsat {

    two_bit_dpd_precoder::sptr
    two_bit_dpd_precoder::make(unsigned char init_value)
    {
      return gnuradio::get_initial_sptr
        (new two_bit_dpd_precoder_impl(init_value));
    }

    /*
     * The private constructor
     */
    two_bit_dpd_precoder_impl::two_bit_dpd_precoder_impl(unsigned char init_value)
      : gr::sync_block("two_bit_dpd_precoder",
              gr::io_signature::make(0, 0, sizeof(char)),
              gr::io_signature::make(0, 0, sizeof(char))),
		d_init_value(init_value)
    {
	d_in_port = pmt::mp("in");
      	message_port_register_in(d_in_port);

	d_out_port = pmt::mp("out");	      
      	message_port_register_out(d_out_port);

	set_msg_handler(d_in_port, boost::bind(&two_bit_dpd_precoder_impl::pmt_in_callback, this ,_1) );
    }

    /*
     * Our virtual destructor.
     */
    two_bit_dpd_precoder_impl::~two_bit_dpd_precoder_impl()
    {
    }


    void two_bit_dpd_precoder_impl::pmt_in_callback(pmt::pmt_t msg)
    {
	pmt::pmt_t meta(pmt::car(msg));
	pmt::pmt_t bytes(pmt::cdr(msg));

	size_t msg_len, i, j;
	const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);
	uint8_t bytes_out[1024];
	uint8_t current_bit_in, current_bit_out, d_last_bit_out = d_init_value;

	if(msg_len>1024) return;

	for(i=0; i<msg_len; i++)
	{
		bytes_out[i] = 0;

		for(j=0; j<8; j++)
		{
			current_bit_in = ((bytes_in[i]<<j)&0x80)?1:0;

			if(current_bit_in)
			{
				current_bit_out = !d_last_bit_out;
			}
			else
			{
				current_bit_out = d_last_bit_out;
			}
			
			d_last_bit_out = current_bit_out;
			
			bytes_out[i] = bytes_out[i] | ( (current_bit_out?0x80:0)>>j );
		}
	}

	//bytes_in, msg_len

	two_bit_dpd_precoder_impl::message_port_pub(two_bit_dpd_precoder_impl::d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(msg_len, (const uint8_t *)bytes_out)));
    }

    int
    two_bit_dpd_precoder_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

