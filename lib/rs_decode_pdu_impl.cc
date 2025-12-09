/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "rs_decode_pdu_impl.h"
#include <gnuradio/io_signature.h>

extern "C"
{
	#include "ccsds/ccsds.h"
}
namespace gr {
namespace lilacsat {

rs_decode_pdu::sptr rs_decode_pdu::make() {
    return gnuradio::make_block_sptr<rs_decode_pdu_impl>();
}


/*
 * The private constructor
 */
rs_decode_pdu_impl::rs_decode_pdu_impl()
    : gr::sync_block("rs_decode_pdu",
                     gr::io_signature::make(
                         0 /* min inputs */, 0 /* max inputs */, sizeof(char)),
                     gr::io_signature::make(
                         0 /* min outputs */, 0 /*max outputs */, sizeof(char))) {
	d_in_port = pmt::mp("in");
      	message_port_register_in(d_in_port);

	d_out_port = pmt::mp("out");	      
      	message_port_register_out(d_out_port);

	set_msg_handler(d_in_port, [this](pmt::pmt_t msg) { this->pmt_in_callback(msg); } );	 
	}

/*
 * Our virtual destructor.
 */
rs_decode_pdu_impl::~rs_decode_pdu_impl() {}

void rs_decode_pdu_impl::pmt_in_callback(pmt::pmt_t msg) {
	pmt::pmt_t meta(pmt::car(msg));
	pmt::pmt_t bytes(pmt::cdr(msg));

	size_t msg_len, i;
	const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);
	uint8_t buffer[256];
	int byte_corr;
	static time_t time_curr;
	static struct tm *tblock_curr;
	
	time_curr = time(NULL);
	tblock_curr =  gmtime(&time_curr);

	for(i=0; i<msg_len; i++)
	{
		buffer[i] = bytes_in[i];
	}

	byte_corr = decode_rs(buffer, (int *)0, 0, RS_BLOCK_LENGTH-msg_len);

	fprintf(stdout, "\n**** %02d:%02d:%02d, byte_corr = %d\n", tblock_curr->tm_hour, tblock_curr->tm_min, tblock_curr->tm_sec, byte_corr);

	if (byte_corr != -1)
	{
		rs_decode_pdu_impl::message_port_pub(rs_decode_pdu_impl::d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(msg_len-RS_LENGTH, (const uint8_t *)buffer)));
	}
}
int rs_decode_pdu_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items) {
    // Do <+signal processing+>

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
