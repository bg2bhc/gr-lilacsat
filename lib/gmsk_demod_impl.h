/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_GMSK_DEMOD_IMPL_H
#define INCLUDED_LILACSAT_GMSK_DEMOD_IMPL_H

#include <gnuradio/lilacsat/gmsk_demod.h>

#define Tb 8

typedef struct
{
	uint16_t numTaps;         /**< number of filter coefficients in the filter. */
	float *pState;            /**< points to the state variable array. The array is of length numTaps. */
	float *pCoeffs;           /**< points to the coefficient array. The array is of length numTaps.*/
} fir_instance_f;

namespace gr {
namespace lilacsat {

class gmsk_demod_impl : public gmsk_demod {
private:
      float d_dtll_gain;
      int d_pd_N_avg;
      float d_pd_k1;
      float d_pd_k2;
      float d_vco_gain;

      double d_vco_phase;
      float d_buf_match_real[41], d_buf_match_imag[41];
      fir_instance_f d_state_match_real, d_state_match_imag;
      float d_buf_delay_real[Tb+2], d_buf_delay_imag[Tb+2];
      float d_dtll_phase;
      float d_buf_avg[50];
      double d_lf_out;
      float d_mean_out_last;

      uint64_t count;

      void fir_filt_f(fir_instance_f *instance, float *pSrc, float *pDst, uint32_t len);
public:
    gmsk_demod_impl(
        float dtll_gain, int pd_N_avg, float pd_k1, float pd_k2, float vco_gain);
    ~gmsk_demod_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_GMSK_DEMOD_IMPL_H */
