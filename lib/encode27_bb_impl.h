/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_ENCODE27_BB_IMPL_H
#define INCLUDED_LILACSAT_ENCODE27_BB_IMPL_H

#include <gnuradio/lilacsat/encode27_bb.h>

extern "C"
{
	#include "ccsds/viterbi27.h"
}

namespace gr {
namespace lilacsat {

class encode27_bb_impl : public encode27_bb {
private:
    unsigned char encstate;

public:
    encode27_bb_impl();
    ~encode27_bb_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_ENCODE27_BB_IMPL_H */
