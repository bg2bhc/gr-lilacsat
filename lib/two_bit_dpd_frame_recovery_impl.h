/* -*- c++ -*- */
/* 
 * Copyright 2019 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_LILACSAT_TWO_BIT_DPD_FRAME_RECOVERY_IMPL_H
#define INCLUDED_LILACSAT_TWO_BIT_DPD_FRAME_RECOVERY_IMPL_H

#include <lilacsat/two_bit_dpd_frame_recovery.h>

namespace gr {
  namespace lilacsat {

    class two_bit_dpd_frame_recovery_impl : public two_bit_dpd_frame_recovery
    {
     private:
      // Nothing to declare in this block.
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
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_TWO_BIT_DPD_FRAME_RECOVERY_IMPL_H */

