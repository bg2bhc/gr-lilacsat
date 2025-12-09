/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_TWO_BIT_DPD_FRAME_RECOVERY_IMPL_H
#define INCLUDED_LILACSAT_TWO_BIT_DPD_FRAME_RECOVERY_IMPL_H

#include <gnuradio/lilacsat/two_bit_dpd_frame_recovery.h>

namespace gr {
namespace lilacsat {

class two_bit_dpd_frame_recovery_impl : public two_bit_dpd_frame_recovery {
private:
    int i_dpd_buf;
      int i_pwr_buf;
      float pwr_asm;
      float buf_pwr_acc[8];
      float buf_pwr[128];
      float buf_corr[256];
      uint8_t flag_asm;
      uint8_t flag_peak;
      float d_snr_threshold;
      float corr_asm_last;
      int sample_in_symbol;
      int n_rx_bytes;
      int bit_in_byte;
      float corr_real_last;
      float corr_imag_last;
      pmt::pmt_t d_out_port;
      uint8_t buf_rx_frame[256];
      uint8_t *ptr_current_byte;

public:
    two_bit_dpd_frame_recovery_impl(float snr_threshold);
    ~two_bit_dpd_frame_recovery_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_TWO_BIT_DPD_FRAME_RECOVERY_IMPL_H */
