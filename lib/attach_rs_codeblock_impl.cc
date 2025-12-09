/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "attach_rs_codeblock_impl.h"
#include <gnuradio/io_signature.h>

extern "C" {
#include "ccsds/ccsds.h"
}
namespace gr {
namespace lilacsat {


attach_rs_codeblock::sptr attach_rs_codeblock::make(int interleaving) {
    return gnuradio::make_block_sptr<attach_rs_codeblock_impl>(interleaving);
}


/*
 * The private constructor
 */
attach_rs_codeblock_impl::attach_rs_codeblock_impl(int interleaving)
    : gr::sync_block(
          "attach_rs_codeblock",
          gr::io_signature::make(0 /* min inputs */, 0 /* max inputs */, sizeof(char)),
          gr::io_signature::make(0 /* min outputs */, 0 /*max outputs */, sizeof(char))) {
    d_in_port = pmt::mp("in");
    message_port_register_in(d_in_port);

    d_out_port = pmt::mp("out");
    message_port_register_out(d_out_port);
    set_msg_handler(d_in_port, [this](pmt::pmt_t msg) { this->pmt_in_callback(msg); });
}

/*
 * Our virtual destructor.
 */
attach_rs_codeblock_impl::~attach_rs_codeblock_impl() {}

void attach_rs_codeblock_impl::pmt_in_callback(pmt::pmt_t msg) {
    pmt::pmt_t meta(pmt::car(msg));
    pmt::pmt_t bytes(pmt::cdr(msg));

    size_t msg_len, i;
    const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);
    uint8_t bytes_out[1024], rs_block[32];

    for (i = 0; i < msg_len; i++) {
        bytes_out[i] = bytes_in[i];
    }

    encode_rs(bytes_out, bytes_out + msg_len, RS_BLOCK_LENGTH - RS_LENGTH - msg_len);

    // bytes_in, msg_len

    attach_rs_codeblock_impl::message_port_pub(
        attach_rs_codeblock_impl::d_out_port,
        pmt::cons(pmt::make_dict(),
                  pmt::init_u8vector(msg_len + RS_LENGTH, (const uint8_t*)bytes_out)));
}
int attach_rs_codeblock_impl::work(int noutput_items,
                                   gr_vector_const_void_star& input_items,
                                   gr_vector_void_star& output_items) {
    // Do <+signal processing+>

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
