/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_VITFILT27_FB_IMPL_H
#define INCLUDED_LILACSAT_VITFILT27_FB_IMPL_H

#include <gnuradio/lilacsat/vitfilt27_fb.h>

extern "C"
{
	#include "ccsds/viterbi27.h"
}
namespace gr {
namespace lilacsat {

class vitfilt27_fb_impl : public vitfilt27_fb {
private:
    v27 vi;

public:
    vitfilt27_fb_impl();
    ~vitfilt27_fb_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_VITFILT27_FB_IMPL_H */
