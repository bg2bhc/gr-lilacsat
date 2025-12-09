/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "kiss_encode_pdu_impl.h"
#include <gnuradio/io_signature.h>
#include <unistd.h>
namespace gr {
namespace lilacsat {

kiss_encode_pdu::sptr kiss_encode_pdu::make(int const_length) {
    return gnuradio::make_block_sptr<kiss_encode_pdu_impl>(const_length);
}


/*
 * The private constructor
 */
kiss_encode_pdu_impl::kiss_encode_pdu_impl(int const_length)
    : gr::block("kiss_encode_pdu",
                gr::io_signature::make(
                    0 /* min inputs */, 0 /* max inputs */, 0),
                gr::io_signature::make(
                    0 /* min outputs */, 0 /*max outputs */, 0)),
					d_const_length(const_length) {
	d_in_port = pmt::mp("in");
      	message_port_register_in(d_in_port);

	d_out_port = pmt::mp("out");	      
      	message_port_register_out(d_out_port);

	set_msg_handler(d_in_port,  [this](pmt::pmt_t msg) { this->pmt_in_callback(msg); });
}

/*
 * Our virtual destructor.
 */
kiss_encode_pdu_impl::~kiss_encode_pdu_impl() {}

void kiss_encode_pdu_impl::forecast(int noutput_items,
                                    gr_vector_int& ninput_items_required) {
    /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
}

void kiss_encode_pdu_impl::pmt_in_callback(pmt::pmt_t msg) {
	pmt::pmt_t meta(pmt::car(msg));
	pmt::pmt_t bytes(pmt::cdr(msg));

	size_t msg_len, n_path;
	const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);

	unsigned char buf_kiss[1024];
	int len_kiss, i, sent=0;

	len_kiss = kiss_tx_packet((unsigned char *)bytes_in, msg_len, buf_kiss);

	if(d_const_length == -1)
	{
		kiss_encode_pdu_impl::message_port_pub(kiss_encode_pdu_impl::d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(len_kiss, (const uint8_t *)buf_kiss)));
	}
	else if(d_const_length > 0)
	{
		while(sent < len_kiss)
		{
			if(len_kiss - sent < d_const_length)
			{
				for(i=0; i<d_const_length-len_kiss+sent; i++)	buf_kiss[len_kiss+i] = 0xC0;
			}
			kiss_encode_pdu_impl::message_port_pub(kiss_encode_pdu_impl::d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(d_const_length, (const uint8_t *)buf_kiss+sent)));
			sent += d_const_length;
			usleep(10);
		}
	}
}
int kiss_encode_pdu_impl::general_work(int noutput_items,
                                       gr_vector_int& ninput_items,
                                       gr_vector_const_void_star& input_items,
                                       gr_vector_void_star& output_items) {
    // Do <+signal processing+>
    // Tell runtime system how many input items we consumed on
    // each input stream.
    consume_each(noutput_items);

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
