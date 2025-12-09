/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "bpsk_demod_cb_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lilacsat {

using input_type = gr_complex;
using output_type = char;
bpsk_demod_cb::sptr bpsk_demod_cb::make() {
    return gnuradio::make_block_sptr<bpsk_demod_cb_impl>();
}


/*
 * The private constructor
 */
bpsk_demod_cb_impl::bpsk_demod_cb_impl()
    : gr::sync_block("bpsk_demod_cb",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type))) {}

/*
 * Our virtual destructor.
 */
bpsk_demod_cb_impl::~bpsk_demod_cb_impl() {}

int bpsk_demod_cb_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items) {
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

    // Do <+signal processing+>
	for(int i=0; i<noutput_items; i++)
	{
		if(in[i].real()>=0)
		{
			out[i] = 1;
		}
		else
		{
			out[i] = 0;
		}
	}

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
