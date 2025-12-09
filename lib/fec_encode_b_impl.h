/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_FEC_ENCODE_B_IMPL_H
#define INCLUDED_LILACSAT_FEC_ENCODE_B_IMPL_H

#include <gnuradio/lilacsat/fec_encode_b.h>

extern "C"
{
	#include "ccsds/ccsds.h"
}

namespace gr {
namespace lilacsat {

class fec_encode_b_impl : public fec_encode_b {
private:
    pmt::pmt_t d_in_port;
    pmt::pmt_t d_ptt_port;
    Ccsds cc;
    char d_ptt;
    void pmt_in_callback(pmt::pmt_t msg);

public:
    fec_encode_b_impl(int frame_len,
                      int preamble_len,
                      int trailer_len,
                      bool continous,
                      bool padding_zero,
                      bool using_m,
                      bool using_convolutional_code);
    ~fec_encode_b_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_FEC_ENCODE_B_IMPL_H */
