/* -*- c++ -*- */
/* 
 * Copyright 2015 WEI Mingchuan, BG2BHC <bg2bhc@gmail.com>
 * Copyright 2015 MI Mingheng <mmh_web@163.com>
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
#include "serial_sink_pdu_impl.h"

#include <stdio.h>

extern "C"
{
	#include "serial/serial.h"
}

namespace gr {
  namespace lilacsat {

    serial_sink_pdu::sptr
    serial_sink_pdu::make(const std::string& path, unsigned int baudrate, bool debug)
    {
      return gnuradio::get_initial_sptr
        (new serial_sink_pdu_impl(path, baudrate, debug));
    }

    /*
     * The private constructor
     */
    serial_sink_pdu_impl::serial_sink_pdu_impl(const std::string& path, unsigned int baudrate, bool debug)
      : gr::block("serial_sink_pdu",
              gr::io_signature::make(0, 0, sizeof(char)),
              gr::io_signature::make(0, 0, sizeof(char))),
		d_path(path), d_baudrate(baudrate), d_debug(debug)
    {
	d_in_port = pmt::mp("in");
      	message_port_register_in(d_in_port);

	set_msg_handler(d_in_port, boost::bind(&serial_sink_pdu_impl::pmt_in_callback, this ,_1) );

	fd = serialOpen((const char *)d_path.data(), (unsigned int)d_baudrate);

    }

    /*
     * Our virtual destructor.
     */
    serial_sink_pdu_impl::~serial_sink_pdu_impl()
    {
    }

    void
    serial_sink_pdu_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    void serial_sink_pdu_impl::pmt_in_callback(pmt::pmt_t msg)
    {
	pmt::pmt_t meta(pmt::car(msg));
	pmt::pmt_t bytes(pmt::cdr(msg));
	size_t msg_len, n_path;
	const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);

	//int fd;
	int nwrite;

	// fd = serialOpen((const char *)d_path.data(), (unsigned int)d_baudrate);

	if (fd < 0)
	{	
		if (d_debug)
		{
			fprintf(stdout, "Can't Open Serial Port!\n");
		}
	}
	else
	{
		if(pmt::symbol_to_string(meta) == "data")
		{
			nwrite = serialWriteRaw(fd, (const char *)bytes_in, msg_len);
			if(d_debug)
			{
				if (nwrite != msg_len)
				{
					fprintf(stdout, "WriteError\n");
				}
			}

			if(d_debug)
			{
				fprintf(stdout, "BeginWaiting\n");
			}
			serialWaitUntilSent(fd);
			if(d_debug)
			{
				fprintf(stdout, "StopWaiting\n");
			}
		}
		else if(pmt::symbol_to_string(meta) == "set_rts")
		{
			setRTS(fd);
			if(d_debug)
			{
				fprintf(stdout, "SetRTS\n");
			}
		}
		else if(pmt::symbol_to_string(meta) == "clear_rts")
		{
			clearRTS(fd);
			if(d_debug)
			{
				fprintf(stdout, "ClearRTS\n");
			}
		}
		else if(pmt::symbol_to_string(meta) == "set_dtr")
		{
			setDTR(fd);
			if(d_debug)
			{
				fprintf(stdout, "SetDTR\n");
			}
		}
		else if(pmt::symbol_to_string(meta) == "clear_dtr")
		{
			clearDTR(fd);
			if(d_debug)
			{
				fprintf(stdout, "ClearDTR\n");
			}
		}

		// close(fd);
	}
    }

    int
    serial_sink_pdu_impl::general_work (int noutput_items,
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

