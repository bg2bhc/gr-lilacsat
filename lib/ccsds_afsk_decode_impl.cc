/* -*- c++ -*- */
/*
 * Copyright 2021 gr-lilacsat author.
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
#include "ccsds_afsk_decode_impl.h"

#include <time.h>

namespace gr {
  namespace lilacsat {

    ccsds_afsk_decode::sptr
    ccsds_afsk_decode::make(int bitrate, int frame_len, bool using_m, bool using_convolutional_code, bool pass_all)
    {
      return gnuradio::get_initial_sptr
        (new ccsds_afsk_decode_impl(bitrate, frame_len, using_m, using_convolutional_code, pass_all));
    }


    /*
     * The private constructor
     */
    ccsds_afsk_decode_impl::ccsds_afsk_decode_impl(int bitrate, int frame_len, bool using_m, bool using_convolutional_code, bool pass_all)
      : gr::sync_block("ccsds_afsk_decode",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(0,0,0)), d_pass_all(pass_all)
      {
        d_out_port = pmt::mp("out");
        message_port_register_out(d_out_port);

        set_output_multiple(16);

		switch(bitrate)
		{
			case 1200:
				cc.bitrate = 1200;
				cc.mark_freq = 1200;
				cc.space_freq = 2200;
				direwolf_ccsds_afsk_init(&cc, 0x1ACFFC1D, frame_len, this, callback);
				break;
			case 2400:
				cc.bitrate = 2400;
				cc.mark_freq = 1200;
				cc.space_freq = 2400;
				ccsds_afsk_init_dpd(&cc, 0x1ACFFC1D, frame_len, this, callback);
				break;
			case 24001:
				cc.bitrate = 2400;
				cc.mark_freq = 1050;
				cc.space_freq = 2250;
				ccsds_afsk_init_dpd(&cc, 0x1ACFFC1D, frame_len, this, callback);
				break;
			default:
				exit(0);
				break;
		}

        cc.cfg_using_m = using_m;
        cc.cfg_using_convolutional_code = using_convolutional_code;
      }

    /*
     * Our virtual destructor.
     */
    ccsds_afsk_decode_impl::~ccsds_afsk_decode_impl()
    {
    }

    void ccsds_afsk_decode_impl::callback(unsigned char *buf, unsigned short len, int16_t byte_corr, void *obj_ptr)
    {
        static time_t time_curr;
        static struct tm *tblock_curr;
        ccsds_afsk_decode_impl *obj_ptr_loc = (ccsds_afsk_decode_impl *)obj_ptr;

        time_curr = time(NULL);
        tblock_curr =  gmtime(&time_curr);

        fprintf(stdout, "\n**** %02d:%02d:%02d, byte_corr = %d\n", tblock_curr->tm_hour, tblock_curr->tm_min, tblock_curr->tm_sec, byte_corr);

        if (byte_corr != -1 || obj_ptr_loc->d_pass_all)
        {
            obj_ptr_loc->message_port_pub(obj_ptr_loc->d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(len, buf)));
        }
    }
    int
    ccsds_afsk_decode_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      float *in = (float *) input_items[0];

      // Do <+signal processing+>
      switch(cc.bitrate)
      {
		case 1200:
			direwolf_ccsds_afsk_rx_proc(&cc, in, noutput_items);
			break;
		case 2400:
		case 24001:
			ccsds_afsk_rx_proc_dpd(&cc, in, noutput_items);
			break;
		default:
			exit(0);
			break;
      }
      ccsds_afsk_pull(&cc);
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

