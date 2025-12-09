/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_TWO_BIT_DPD_PRECODER_IMPL_H
#define INCLUDED_LILACSAT_TWO_BIT_DPD_PRECODER_IMPL_H

#include <gnuradio/lilacsat/two_bit_dpd_precoder.h>

namespace gr {
namespace lilacsat {

class two_bit_dpd_precoder_impl : public two_bit_dpd_precoder {
private:
      unsigned char d_init_value;
      pmt::pmt_t d_in_port;
      pmt::pmt_t d_out_port;

      void pmt_in_callback(pmt::pmt_t msg);
public:
    two_bit_dpd_precoder_impl(unsigned char init_value);
    ~two_bit_dpd_precoder_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_TWO_BIT_DPD_PRECODER_IMPL_H */
