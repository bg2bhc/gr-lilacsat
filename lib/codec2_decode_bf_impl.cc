/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "codec2_decode_bf_impl.h"
#include <gnuradio/io_signature.h>

#include <stdio.h>

#define	SAMPLES_PER_FRAME	320
#define	BITS_PER_FRAME		52
namespace gr {
namespace lilacsat {

using input_type = char;
using output_type = float;
codec2_decode_bf::sptr codec2_decode_bf::make(int mode) {
    return gnuradio::make_block_sptr<codec2_decode_bf_impl>(mode);
}


/*
 * The private constructor
 */
codec2_decode_bf_impl::codec2_decode_bf_impl(int mode)
    : gr::sync_interpolator(
          "codec2_decode_bf",
          gr::io_signature::make(
              1 /* min inputs */, 1 /* max inputs */, 8*sizeof(input_type)),
          gr::io_signature::make(
              1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)),
          SAMPLES_PER_FRAME /*<+interpolation+>*/) {
	codec2 = codec2_create(CODEC2_MODE_1300);
}

/*
 * Our virtual destructor.
 */
codec2_decode_bf_impl::~codec2_decode_bf_impl() {}

int codec2_decode_bf_impl::work(int noutput_items,
                                gr_vector_const_void_star& input_items,
                                gr_vector_void_star& output_items) {
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

	int i, j;
	int16_t buf[SAMPLES_PER_FRAME];

	// Do <+signal processing+>
	for(i=0; i<noutput_items/SAMPLES_PER_FRAME; i++)
	{
		codec2_decode(codec2, buf, (unsigned char *)in+i*8);
		for(j=0; j<SAMPLES_PER_FRAME; j++)
		{
			out[i*SAMPLES_PER_FRAME + j] = (float)buf[j]/32767.0;
		}
	}
    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
