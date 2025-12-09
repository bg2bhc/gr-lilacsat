/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "codec2_encode_fb_impl.h"
#include <gnuradio/io_signature.h>

#define	SAMPLES_PER_FRAME	320
#define	BITS_PER_FRAME		52

namespace gr {
namespace lilacsat {

using input_type = float;
using output_type = char;
codec2_encode_fb::sptr codec2_encode_fb::make(int mode) {
    return gnuradio::make_block_sptr<codec2_encode_fb_impl>(mode);
}


/*
 * The private constructor
 */
codec2_encode_fb_impl::codec2_encode_fb_impl(int mode)
    : gr::sync_decimator("codec2_encode_fb",
                         gr::io_signature::make(
                             1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                         gr::io_signature::make(1 /* min outputs */,
                                                1 /*max outputs */,
                                                8*sizeof(output_type)),
                         SAMPLES_PER_FRAME /*<+decimation+>*/) {
	codec2 = codec2_create(CODEC2_MODE_1300);
}

/*
 * Our virtual destructor.
 */
codec2_encode_fb_impl::~codec2_encode_fb_impl() {}

int codec2_encode_fb_impl::work(int noutput_items,
                                gr_vector_const_void_star& input_items,
                                gr_vector_void_star& output_items) {
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);
	int i, j;
	int16_t buf[SAMPLES_PER_FRAME];

    // Do <+signal processing+>
	for(i=0; i<noutput_items; i++)
	{
		for(j=0; j<SAMPLES_PER_FRAME; j++)
		{
			buf[j] = (int16_t)(in[i*SAMPLES_PER_FRAME+j]*(float)32767.0);
			codec2_encode(codec2, (unsigned char *)out+i*8, buf);
		}
	}

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
