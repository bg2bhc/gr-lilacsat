/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "file_sink_pdu_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lilacsat {

file_sink_pdu::sptr file_sink_pdu::make(const std::string& path, int16_t format) {
    return gnuradio::make_block_sptr<file_sink_pdu_impl>(path, format);
}


/*
 * The private constructor
 */
file_sink_pdu_impl::file_sink_pdu_impl(const std::string& path, int16_t format)
    : gr::block("file_sink_pdu",
                gr::io_signature::make(
                    0 /* min inputs */, 0 /* max inputs */, 0),
                gr::io_signature::make(
                    0 /* min outputs */, 0 /*max outputs */, 0)) {
	fp = fopen(path.data(), "rw+");

	d_in_port = pmt::mp("in");
	message_port_register_in(d_in_port);

	set_msg_handler(d_in_port,  [this](pmt::pmt_t msg) { this->pmt_in_callback(msg); });
}

/*
 * Our virtual destructor.
 */
file_sink_pdu_impl::~file_sink_pdu_impl() {
	fclose(fp);
}

void file_sink_pdu_impl::forecast(int noutput_items,
                                  gr_vector_int& ninput_items_required) {
    /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
}

void file_sink_pdu_impl::pmt_in_callback(pmt::pmt_t msg) {
	pmt::pmt_t meta(pmt::car(msg));
	pmt::pmt_t bytes(pmt::cdr(msg));

	unsigned int i;
	size_t msg_len;
	const uint8_t *bytes_in = pmt::u8vector_elements(bytes, msg_len);

	fwrite(bytes_in, 1, msg_len, fp);
	fflush(fp);
}
int file_sink_pdu_impl::general_work(int noutput_items,
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
