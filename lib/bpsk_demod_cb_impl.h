/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_BPSK_DEMOD_CB_IMPL_H
#define INCLUDED_LILACSAT_BPSK_DEMOD_CB_IMPL_H

#include <gnuradio/lilacsat/bpsk_demod_cb.h>

namespace gr {
namespace lilacsat {

class bpsk_demod_cb_impl : public bpsk_demod_cb {
private:
    // Nothing to declare in this block.

public:
    bpsk_demod_cb_impl();
    ~bpsk_demod_cb_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_BPSK_DEMOD_CB_IMPL_H */
