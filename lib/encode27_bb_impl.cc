/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "encode27_bb_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lilacsat {

using input_type = char;
using output_type = char;
encode27_bb::sptr encode27_bb::make() {
    return gnuradio::make_block_sptr<encode27_bb_impl>();
}


/*
 * The private constructor
 */
encode27_bb_impl::encode27_bb_impl()
    : gr::sync_interpolator(
          "encode27_bb",
          gr::io_signature::make(
              1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
          gr::io_signature::make(
              1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)),
          16 /*<+interpolation+>*/) {
	set_output_multiple(16);
}

/*
 * Our virtual destructor.
 */
encode27_bb_impl::~encode27_bb_impl() {}

int encode27_bb_impl::work(int noutput_items,
                           gr_vector_const_void_star& input_items,
                           gr_vector_void_star& output_items) {
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

    // Do <+signal processing+>
	encode27(&encstate, (unsigned char *)out, (unsigned char *)in, noutput_items/16);

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
