/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "ccsds_afsk_encode_impl.h"
#include <gnuradio/io_signature.h>
#include <stdio.h>
namespace gr {
namespace lilacsat {

using output_type = float;
ccsds_afsk_encode::sptr ccsds_afsk_encode::make(int bitrate,
                                                int frame_len,
                                                int preamble_len,
                                                int trailer_len,
                                                bool continous,
                                                bool padding_zero,
                                                bool using_m,
                                                bool using_convolutional_code) {
    return gnuradio::make_block_sptr<ccsds_afsk_encode_impl>(bitrate,
                                                             frame_len,
                                                             preamble_len,
                                                             trailer_len,
                                                             continous,
                                                             padding_zero,
                                                             using_m,
                                                             using_convolutional_code);
}


/*
 * The private constructor
 */
ccsds_afsk_encode_impl::ccsds_afsk_encode_impl(int bitrate,
                                               int frame_len,
                                               int preamble_len,
                                               int trailer_len,
                                               bool continous,
                                               bool padding_zero,
                                               bool using_m,
                                               bool using_convolutional_code)
    : gr::sync_block("ccsds_afsk_encode",
                     gr::io_signature::make(
                         0 /* min inputs */, 0 /* max inputs */, 0),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type))) {
						 
						 
						 d_in_port = pmt::mp("in");
        message_port_register_in(d_in_port);
        d_ptt_port = pmt::mp("ptt");
        message_port_register_out(d_ptt_port);

        set_msg_handler(d_in_port, [this](pmt::pmt_t msg) { this->pmt_in_callback(msg); } );
        set_output_multiple(16);

		switch(bitrate)
		{
			case 1200:
				cc.bitrate = 1200;
				cc.mark_freq = 1200;
				cc.space_freq = 2200;
				break;
			case 2400:
				cc.bitrate = 2400;
				cc.mark_freq = 1200;
				cc.space_freq = 2400;
				break;
			case 24001:
				cc.bitrate = 2400;
				cc.mark_freq = 1050;
				cc.space_freq = 2250;
				break;
			default:
				exit(0);
				break;
		} 

		ccsds_afsk_init(&cc, 0x1ACFFC1D, frame_len, this, 0);

        cc.cfg_preamble_len = preamble_len;
        cc.cfg_trailer_len = trailer_len;
        cc.cfg_continous = continous;
        cc.cfg_padding_zero = padding_zero;
        cc.cfg_using_m = using_m;
        cc.cfg_using_convolutional_code = using_convolutional_code;
		}

    void ccsds_afsk_encode_impl::pmt_in_callback(pmt::pmt_t msg){
        pmt::pmt_t meta(pmt::car(msg));
		pmt::pmt_t bytes(pmt::cdr(msg));
	
		size_t msg_len, n_path;
		const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);
        ccsds_afsk_send(&cc, (uint8_t *)bytes_in);
    }
/*
 * Our virtual destructor.
 */
ccsds_afsk_encode_impl::~ccsds_afsk_encode_impl() {}

int ccsds_afsk_encode_impl::work(int noutput_items,
                                 gr_vector_const_void_star& input_items,
                                 gr_vector_void_star& output_items) {
    auto out = static_cast<output_type*>(output_items[0]);
      unsigned char msg_ptt_on[] = {0xFE, 0xFE, 0x7C, 0xE0, 0x1C, 0x00, 0x01, 0xFD};
      unsigned char msg_ptt_off[] = {0xFE, 0xFE, 0x7C, 0xE0, 0x1C, 0x00, 0x00, 0xFD};

      int n_ret;
    // Do <+signal processing+>

      switch(cc.bitrate)
      {
		case 1200:
			n_ret = ccsds_afsk_tx_proc(&cc, out, noutput_items);
			break;
		case 2400:
		case 24001:
			n_ret = ccsds_afsk_tx_proc_gmsk(&cc, out, noutput_items);
			break;
		default:
			exit(0);
			break;
      } 
    
      if((d_ptt == 0) && (n_ret != 0))
      {
		message_port_pub(d_ptt_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(sizeof(msg_ptt_on), (const uint8_t *)msg_ptt_on)));
		d_ptt = 1;
      }
      else if((d_ptt != 0) && (n_ret == 0))
      {
		message_port_pub(d_ptt_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(sizeof(msg_ptt_off), (const uint8_t *)msg_ptt_off)));
		d_ptt = 0;
      }
      
      if(n_ret == 0) usleep(1000);
      
      // Tell runtime system how many output items we produced.
      return n_ret;
}

} /* namespace lilacsat */
} /* namespace gr */
