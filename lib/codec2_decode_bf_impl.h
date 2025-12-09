/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_CODEC2_DECODE_BF_IMPL_H
#define INCLUDED_LILACSAT_CODEC2_DECODE_BF_IMPL_H

#include <gnuradio/lilacsat/codec2_decode_bf.h>

extern "C"
{
	#include "codec2/codec2.h"
}

namespace gr {
namespace lilacsat {

class codec2_decode_bf_impl : public codec2_decode_bf {
private:
      struct CODEC2 *codec2;

public:
    codec2_decode_bf_impl(int mode);
    ~codec2_decode_bf_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_CODEC2_DECODE_BF_IMPL_H */
