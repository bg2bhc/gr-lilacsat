/* -*- c++ -*- */
/* 
 * Copyright 2015 WEI Mingchuan, BG2BHC <bg2bhc@gmail.com>
 * Copyright 2015 HIT Research Center of Satellite Technology
 * Copyright 2015 HIT Amateur Radio Club, BY2HIT
 *
 * Harbin Institute of Technology <http://www.hit.edu.cn/>
 * LilacSat - HIT Student Satellites <http://lilacsat.hit.edu.cn/>
 * HIT Amateur Radio Club <http://www.by2hit.net/>
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

#ifndef INCLUDED_LILACSAT_GMSK_DEMOD_IMPL_H
#define INCLUDED_LILACSAT_GMSK_DEMOD_IMPL_H

#include <lilacsat/gmsk_demod.h>

#define Tb 8

typedef struct
{
	uint16_t numTaps;         /**< number of filter coefficients in the filter. */
	float *pState;            /**< points to the state variable array. The array is of length numTaps. */
	float *pCoeffs;           /**< points to the coefficient array. The array is of length numTaps.*/
} fir_instance_f;

namespace gr {
  namespace lilacsat {

    class gmsk_demod_impl : public gmsk_demod
    {
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
      gmsk_demod_impl(float dtll_gain, int pd_N_avg, float pd_k1, float pd_k2, float vco_gain);
      ~gmsk_demod_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_GMSK_DEMOD_IMPL_H */

