/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pdu_f32_to_u8_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lilacsat {

pdu_f32_to_u8::sptr pdu_f32_to_u8::make() {
    return gnuradio::make_block_sptr<pdu_f32_to_u8_impl>();
}


/*
 * The private constructor
 */
pdu_f32_to_u8_impl::pdu_f32_to_u8_impl()
    : gr::sync_block("pdu_f32_to_u8",
                     gr::io_signature::make(
                         0 /* min inputs */, 0 /* max inputs */, 0),
                     gr::io_signature::make(
                         0 /* min outputs */, 0 /*max outputs */, 0)) {
	d_in_port = pmt::mp("in");
      	message_port_register_in(d_in_port);

	d_out_port = pmt::mp("out");	      
      	message_port_register_out(d_out_port);

	set_msg_handler(d_in_port, [this](pmt::pmt_t msg) { this->pmt_in_callback(msg); } );

}

/*
 * Our virtual destructor.
 */
pdu_f32_to_u8_impl::~pdu_f32_to_u8_impl() {}

void pdu_f32_to_u8_impl::pmt_in_callback(pmt::pmt_t msg) {
	pmt::pmt_t meta(pmt::car(msg));
	pmt::pmt_t bytes(pmt::cdr(msg));

	size_t msg_len;
	const float* floats_in = pmt::f32vector_elements(bytes, msg_len);

	pdu_f32_to_u8_impl::message_port_pub(pdu_f32_to_u8_impl::d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(msg_len*4, (const uint8_t *)floats_in)));
}
int pdu_f32_to_u8_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items) {
        // Do <+signal processing+>

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
