/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "angle_cf_impl.h"
#include <gnuradio/io_signature.h>
#define _USE_MATH_DEFINES
#include <math.h>

namespace gr {
namespace lilacsat {

using input_type = gr_complex;
using output_type = float;
angle_cf::sptr angle_cf::make() { return gnuradio::make_block_sptr<angle_cf_impl>(); }


/*
 * The private constructor
 */
angle_cf_impl::angle_cf_impl()
    : gr::sync_block("angle_cf",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type))) {}

/*
 * Our virtual destructor.
 */
angle_cf_impl::~angle_cf_impl() {}

int angle_cf_impl::work(int noutput_items,
                        gr_vector_const_void_star& input_items,
                        gr_vector_void_star& output_items) {
    auto in = static_cast<const input_type*>(input_items[0]);
    auto out = static_cast<output_type*>(output_items[0]);

    // Do <+signal processing+>
    for(int i=0; i<noutput_items; i++)
    {
        auto real = std::real(in[i]);
        auto imag = std::imag(in[i]);

        if(real == 0) // Y Axis
        {
            if(imag >= 0)   out[i] = M_PI/2;
            else        out[i] = -M_PI/2;
        }
        else
        {
            out[i] = atan(imag/real);
            if(real < 0)
            {
                if(imag >= 0)   out[i] += M_PI;
                else        out[i] -= M_PI;
            }
        }
    }
    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
