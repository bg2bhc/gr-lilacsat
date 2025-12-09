/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "vitfilt27_bb_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lilacsat {

using input_type = unsigned char;
using output_type = unsigned char;
vitfilt27_bb::sptr vitfilt27_bb::make() {
    return gnuradio::make_block_sptr<vitfilt27_bb_impl>();
}


/*
 * The private constructor
 */
vitfilt27_bb_impl::vitfilt27_bb_impl()
    : gr::sync_decimator("vitfilt27_bb",
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
vitfilt27_bb_impl::~vitfilt27_bb_impl() {}

int vitfilt27_bb_impl::work(int noutput_items,
                            gr_vector_const_void_star& input_items,
                            gr_vector_void_star& output_items) {
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

	int i, j;
	unsigned char current_in[16];    
	// Do <+signal processing+>
	for(i=0; i<noutput_items; i++)
	{
		for(j=0; j<16; j++) current_in[j] = (in[i*16+j])?255:0;
		vitfilt27_decode(&vi, current_in, (unsigned char *)out+i, 16);
	}
    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
