/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_ANGLE_CF_IMPL_H
#define INCLUDED_LILACSAT_ANGLE_CF_IMPL_H

#include <gnuradio/lilacsat/angle_cf.h>

namespace gr {
namespace lilacsat {

class angle_cf_impl : public angle_cf {
private:
    // Nothing to declare in this block.

public:
    angle_cf_impl();
    ~angle_cf_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_ANGLE_CF_IMPL_H */
