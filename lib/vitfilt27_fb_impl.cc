/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "vitfilt27_fb_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lilacsat {

using input_type = float;
using output_type = unsigned char;
vitfilt27_fb::sptr vitfilt27_fb::make() {
    return gnuradio::make_block_sptr<vitfilt27_fb_impl>();
}


/*
 * The private constructor
 */
vitfilt27_fb_impl::vitfilt27_fb_impl()
    : gr::sync_decimator("vitfilt27_fb",
                         gr::io_signature::make(
                             1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                         gr::io_signature::make(1 /* min outputs */,
                                                1 /*max outputs */,
                                                sizeof(output_type)),
                         16 /*<+decimation+>*/) {
	vitfilt27_init(&vi);
	set_output_multiple(16);						 
}

/*
 * Our virtual destructor.
 */
vitfilt27_fb_impl::~vitfilt27_fb_impl() {}

int vitfilt27_fb_impl::work(int noutput_items,
                            gr_vector_const_void_star& input_items,
                            gr_vector_void_star& output_items) {
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);
	int i, j;
	float current_in[16];
	unsigned char current_in_uchar[16];

    // Do <+signal processing+>
	for(i=0; i<noutput_items; i++)
	{
		for(j=0; j<16; j++)
		{
			current_in[j] = in[i*16+j] * 127.5 + 127.5;
			if(current_in[j]<0.0) current_in[j] = 0;
			if(current_in[j]>255.0) current_in[j] = 255;

			current_in_uchar[j] = (unsigned char)(current_in[j]);
		}

		vitfilt27_decode(&vi, current_in_uchar, (unsigned char *)out+i, 16);
	}

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
