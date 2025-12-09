/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_CCSDS_AFSK_ENCODE_IMPL_H
#define INCLUDED_LILACSAT_CCSDS_AFSK_ENCODE_IMPL_H

#include <gnuradio/lilacsat/ccsds_afsk_encode.h>
extern "C"
{
	#include "ccsds_afsk/ccsds_afsk.h"
}

namespace gr {
namespace lilacsat {

class ccsds_afsk_encode_impl : public ccsds_afsk_encode {
private:
    // Nothing to declare in this block.
        pmt::pmt_t d_in_port;
        pmt::pmt_t d_ptt_port;
        Ccsds_afsk cc;
        char d_ptt;

        void pmt_in_callback(pmt::pmt_t msg);
public:
    ccsds_afsk_encode_impl(int bitrate,
                           int frame_len,
                           int preamble_len,
                           int trailer_len,
                           bool continous,
                           bool padding_zero,
                           bool using_m,
                           bool using_convolutional_code);
    ~ccsds_afsk_encode_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_CCSDS_AFSK_ENCODE_IMPL_H */
