/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_FEC_DECODE_B_IMPL_H
#define INCLUDED_LILACSAT_FEC_DECODE_B_IMPL_H

#include <gnuradio/lilacsat/fec_decode_b.h>
#include <stdio.h>

extern "C"
{
	#include "ccsds/ccsds.h"
}
namespace gr {
namespace lilacsat {

class fec_decode_b_impl : public fec_decode_b {
private:
    pmt::pmt_t d_out_port;
    Ccsds cc;
    bool d_pass_all;
    static void callback(unsigned char *buf, unsigned short len, int16_t byte_corr, void *obj_ptr);

public:
    fec_decode_b_impl(int frame_len,
                      bool using_m,
                      bool using_convolutional_code,
                      bool pass_all);
    ~fec_decode_b_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_FEC_DECODE_B_IMPL_H */
