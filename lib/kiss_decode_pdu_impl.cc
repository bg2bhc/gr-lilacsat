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
#include "kiss_decode_pdu_impl.h"

namespace gr {
  namespace lilacsat {

    kiss_decode_pdu::sptr
    kiss_decode_pdu::make()
    {
      return gnuradio::get_initial_sptr
        (new kiss_decode_pdu_impl());
    }

    /*
     * The private constructor
     */
    kiss_decode_pdu_impl::kiss_decode_pdu_impl()
      : gr::block("kiss_decode_pdu",
              gr::io_signature::make(0, 0, sizeof(char)),
              gr::io_signature::make(0, 0, sizeof(char)))
    {
	d_in_port = pmt::mp("in");
      	message_port_register_in(d_in_port);

	d_out_port = pmt::mp("out");	      
      	message_port_register_out(d_out_port);

	set_msg_handler(d_in_port, boost::bind(&kiss_decode_pdu_impl::pmt_in_callback, this ,_1) );

	kiss_init(&ki, (void *)this, &kiss_decode_pdu_impl::kiss_msg_callback);
    }

    void kiss_decode_pdu_impl::kiss_msg_callback(void *obj_ptr, char *ptr, uint16_t len)
    {
	kiss_decode_pdu_impl *obj_ptr_loc = (kiss_decode_pdu_impl *)obj_ptr;
	obj_ptr_loc->message_port_pub(obj_ptr_loc->d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(len, (const uint8_t *)ptr)));
    }
    /*
     * Our virtual destructor.
     */
    kiss_decode_pdu_impl::~kiss_decode_pdu_impl()
    {
	
    }

    void
    kiss_decode_pdu_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    void kiss_decode_pdu_impl::pmt_in_callback(pmt::pmt_t msg)
    {
	pmt::pmt_t meta(pmt::car(msg));
	pmt::pmt_t bytes(pmt::cdr(msg));

	size_t msg_len, n_path;
	const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);
	int i;

	for(i=0; i<msg_len; i++)
	{
		kiss_rx_byte(&ki, bytes_in[i]);
	}
    }

    int
    kiss_decode_pdu_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        // const <+ITYPE*> *in = (const <+ITYPE*> *) input_items[0];
        // <+OTYPE*> *out = (<+OTYPE*> *) output_items[0];

        // Do <+signal processing+>
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

