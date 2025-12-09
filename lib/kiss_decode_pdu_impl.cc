/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "kiss_decode_pdu_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lilacsat {

kiss_decode_pdu::sptr kiss_decode_pdu::make() {
    return gnuradio::make_block_sptr<kiss_decode_pdu_impl>();
}


/*
 * The private constructor
 */
kiss_decode_pdu_impl::kiss_decode_pdu_impl()
    : gr::block("kiss_decode_pdu",
                gr::io_signature::make(
                    0 /* min inputs */, 0 /* max inputs */, 0),
                gr::io_signature::make(
                    0 /* min outputs */, 0 /*max outputs */, 0)) {
	d_in_port = pmt::mp("in");
      	message_port_register_in(d_in_port);

	d_out_port = pmt::mp("out");	      
      	message_port_register_out(d_out_port);

	set_msg_handler(d_in_port, [this](pmt::pmt_t msg) { this->pmt_in_callback(msg); });

	kiss_init(&ki, (void *)this, &kiss_decode_pdu_impl::kiss_msg_callback);
		
}

void kiss_decode_pdu_impl::kiss_msg_callback(void *obj_ptr, char *ptr, uint16_t len) {
	kiss_decode_pdu_impl *obj_ptr_loc = (kiss_decode_pdu_impl *)obj_ptr;
	obj_ptr_loc->message_port_pub(obj_ptr_loc->d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(len, (const uint8_t *)ptr)));
}

/*
 * Our virtual destructor.
 */
kiss_decode_pdu_impl::~kiss_decode_pdu_impl() {}

void kiss_decode_pdu_impl::forecast(int noutput_items,
                                    gr_vector_int& ninput_items_required) {
    /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
}

void kiss_decode_pdu_impl::pmt_in_callback(pmt::pmt_t msg) {
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
int kiss_decode_pdu_impl::general_work(int noutput_items,
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
