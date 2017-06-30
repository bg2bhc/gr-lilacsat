/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
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
#include "file_sink_pdu_impl.h"

namespace gr {
  namespace lilacsat {

    file_sink_pdu::sptr
    file_sink_pdu::make(const std::string& path, int16_t format)
    {
      return gnuradio::get_initial_sptr
        (new file_sink_pdu_impl(path, format));
    }

    /*
     * The private constructor
     */
    file_sink_pdu_impl::file_sink_pdu_impl(const std::string& path, int16_t format)
      : gr::block("file_sink_pdu",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
	fp = fopen(path.data(), "rw+");

	d_in_port = pmt::mp("in");
      	message_port_register_in(d_in_port);

	set_msg_handler(d_in_port, boost::bind(&file_sink_pdu_impl::pmt_in_callback, this ,_1) );
    }

    /*
     * Our virtual destructor.
     */
    file_sink_pdu_impl::~file_sink_pdu_impl()
    {
	fclose(fp);
    }

    void
    file_sink_pdu_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    void file_sink_pdu_impl::pmt_in_callback(pmt::pmt_t msg)
    {
	pmt::pmt_t meta(pmt::car(msg));
	pmt::pmt_t bytes(pmt::cdr(msg));

	unsigned int i;
	size_t msg_len;
	const uint8_t *bytes_in = pmt::u8vector_elements(bytes, msg_len);

	fwrite(bytes_in, 1, msg_len, fp);
	fflush(fp);
    }

    int
    file_sink_pdu_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      // const <+ITYPE+> *in = (const <+ITYPE+> *) input_items[0];
      // <+OTYPE+> *out = (<+OTYPE+> *) output_items[0];

      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

