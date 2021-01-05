/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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
#include "ccsds_pseudo_randomizer_impl.h"
#include <stdio.h>

namespace gr {
  namespace lilacsat {

    ccsds_pseudo_randomizer::sptr
    ccsds_pseudo_randomizer::make(int data_format)
    {
      return gnuradio::get_initial_sptr
        (new ccsds_pseudo_randomizer_impl(data_format));
    }

    /*
     * The private constructor
     */
    ccsds_pseudo_randomizer_impl::ccsds_pseudo_randomizer_impl(int data_format)
      : gr::sync_block("ccsds_pseudo_randomizer",
              gr::io_signature::make(0, 0, sizeof(float)),
              gr::io_signature::make(0, 0, sizeof(float))),
              d_data_format(data_format)
    {
		d_in_port = pmt::mp("in");
      	message_port_register_in(d_in_port);

		d_out_port = pmt::mp("out");	      
      	message_port_register_out(d_out_port);

		set_msg_handler(d_in_port, boost::bind(&ccsds_pseudo_randomizer_impl::pmt_in_callback, this ,_1) );
    }

    /*
     * Our virtual destructor.
     */
    ccsds_pseudo_randomizer_impl::~ccsds_pseudo_randomizer_impl()
    {
    }

    void ccsds_pseudo_randomizer_impl::pmt_in_callback(pmt::pmt_t msg)
    {
		pmt::pmt_t meta(pmt::car(msg));
		pmt::pmt_t bytes(pmt::cdr(msg));		

		switch(d_data_format)
		{
			case 1:
			{
				size_t msg_len;
				const uint8_t* bits_in = pmt::u8vector_elements(bytes, msg_len);

				uint8_t *buffer = (uint8_t *)malloc(sizeof(uint8_t)*msg_len);

				for(int i=0; i<msg_len; i++)
				{
					buffer[i] = (sequence[(i/8)%255]&(0x80>>(i%8))) ? !bits_in[i] : bits_in[i];
				}

				ccsds_pseudo_randomizer_impl::message_port_pub(ccsds_pseudo_randomizer_impl::d_out_port, pmt::cons(meta, pmt::init_u8vector(msg_len, buffer)));

				free(buffer);

				break;
			}
			case 2:
			{				
				size_t msg_len;
				const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);

				uint8_t *buffer = (uint8_t *)malloc(sizeof(uint8_t)*msg_len);

				for(int i=0; i<msg_len; i++)
				{
					buffer[i] = bytes_in[i] ^ sequence[i%255];
				}

				ccsds_pseudo_randomizer_impl::message_port_pub(ccsds_pseudo_randomizer_impl::d_out_port, pmt::cons(meta, pmt::init_u8vector(msg_len, buffer)));

				free(buffer);

				break;
			}
			case 3:
			{
				size_t msg_len;
				const float* bits_in = pmt::f32vector_elements(bytes, msg_len);

				float *buffer = (float *)malloc(sizeof(float)*msg_len);

				for(int i=0; i<msg_len; i++)
				{
					buffer[i] = (sequence[(i/8)%255]&(0x80>>(i%8))) ? -bits_in[i] : bits_in[i];
				}

				ccsds_pseudo_randomizer_impl::message_port_pub(ccsds_pseudo_randomizer_impl::d_out_port, pmt::cons(meta, pmt::init_f32vector(msg_len, buffer)));

				free(buffer);

				break;
			}
			default:
			{
				fprintf(stdout, "\n**** ERROR: Unknown data format!\n");
			}
		}
    }

    int
    ccsds_pseudo_randomizer_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace dslwp */
} /* namespace gr */

