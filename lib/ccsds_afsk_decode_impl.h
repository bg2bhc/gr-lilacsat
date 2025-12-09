/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_CCSDS_AFSK_DECODE_IMPL_H
#define INCLUDED_LILACSAT_CCSDS_AFSK_DECODE_IMPL_H

#include <gnuradio/lilacsat/ccsds_afsk_decode.h>
extern "C"
{
	#include "ccsds_afsk/ccsds_afsk.h"
}
namespace gr {
namespace lilacsat {

class ccsds_afsk_decode_impl : public ccsds_afsk_decode {
private:
      pmt::pmt_t d_out_port;
      Ccsds_afsk cc;
      bool d_pass_all;
      static void callback(unsigned char *buf, unsigned short len, int16_t byte_corr, void *obj_ptr);

public:
    ccsds_afsk_decode_impl(int bitrate,
                           int frame_len,
                           bool using_m,
                           bool using_convolutional_code,
                           bool pass_all);
    ~ccsds_afsk_decode_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_CCSDS_AFSK_DECODE_IMPL_H */
