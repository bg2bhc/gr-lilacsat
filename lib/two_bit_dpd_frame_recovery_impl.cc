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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "two_bit_dpd_frame_recovery_impl.h"
#include <stdio.h>

const float asm32_dpd[] = 
{
	0.000000, 0.879120, -0.526581, 0.719482, -0.813904, 0.392609, -0.876312, 0.141327, -0.859985, 0.033142, -0.845764, 0.005005, -0.842316, 0.000946, -0.845764, 0.005005, -0.859985, 0.033142, -0.876312, 0.141327, -0.813904, 0.392609, -0.526581, 0.719482, 0.000000, 0.879120, 0.526581, 0.719543, 0.814362, 0.393372, 0.880463, 0.147339, 0.878235, 0.067810, 0.880524, 0.147308, 0.815277, 0.393250, 0.533844, 0.720520, 0.034668, 0.899078, -0.427734, 0.838287, -0.603577, 0.760101, -0.420593, 0.837219, 0.067810, 0.878235, 0.608521, 0.594421, 0.843719, 0.000000, 0.608521, -0.594421, 0.067810, -0.878235, -0.420593, -0.837219, -0.603577, -0.760101, -0.427734, -0.838287, 0.034668, -0.899078, 0.533844, -0.720520, 0.815277, -0.393250, 0.880524, -0.147308, 0.878235, -0.067810, 0.880463, -0.147339, 0.814362, -0.393372, 0.526581, -0.719543, 0.000000, -0.879120, -0.526581, -0.719543, -0.814362, -0.393372, -0.880463, -0.147339, -0.878235, -0.067810, -0.880463, -0.147339, -0.814362, -0.393372, -0.526581, -0.719543, 0.000000, -0.879120, 0.526642, -0.719513, 0.815063, -0.392914, 0.885345, -0.142334, 0.899078, -0.034637, 0.941132, -0.006012, 0.971558, -0.000793, 0.945343, -0.000061, 0.919067, 0.000000, 0.945374, 0.000000, 0.972015, 0.000000, 0.945374, 0.000000, 0.919067, 0.000000, 0.945374, 0.000000, 0.972015, 0.000000, 0.945374, 0.000000, 0.919067, 0.000000, 0.945374, 0.000000, 0.972015, 0.000000, 0.945374, 0.000000, 0.919067, 0.000000, 0.945374, 0.000000, 0.972015, 0.000000, 0.945374, 0.000000, 0.919067, 0.000000, 0.945374, 0.000000, 0.972015, 0.000000, 0.945374, 0.000000, 0.919067, 0.000000, 0.945374, 0.000000, 0.972015, 0.000000, 0.945374, 0.000000, 0.919067, 0.000000, 0.945343, -0.000061, 0.971558, -0.000793, 0.941132, -0.006012, 0.899078, -0.034637, 0.885345, -0.142334, 0.815063, -0.392914, 0.526642, -0.719513, 0.000000, -0.879120, -0.526581, -0.719482, -0.813904, -0.392609, -0.876312, -0.141327, -0.859985, -0.033142, -0.845703, -0.004974, -0.841614, -0.000458, -0.840973, -0.000031, -0.840912, 0.000000, -0.840881, 0.000000, -0.840881, 0.000000, -0.840881, 0.000000, -0.840912, 0.000000, -0.840973, -0.000031, -0.841614, -0.000458, -0.845703, -0.004974, -0.859985, -0.033142, -0.876312, -0.141327, -0.813904, -0.392609, -0.526581, -0.719482, 0.000000, -0.879120, 0.526642, -0.719513, 0.815063, -0.392914, 0.885345, -0.142334, 0.899078, -0.034637, 0.941101, -0.005920, 0.971069, 0.000000, 0.941101, 0.005920, 0.899078, 0.034637, 0.885406, 0.142334, 0.815979, 0.392792, 0.533905, 0.720520, 0.034668, 0.899078, -0.427826, 0.838257, -0.604492, 0.760010, -0.427826, 0.838257, 0.034668, 0.899078, 0.533905, 0.720520, 0.815979, 0.392792, 0.885406, 0.142334
};

namespace gr {
  namespace lilacsat {

    two_bit_dpd_frame_recovery::sptr
    two_bit_dpd_frame_recovery::make(float snr_threshold)
    {
      return gnuradio::get_initial_sptr
        (new two_bit_dpd_frame_recovery_impl(snr_threshold));
    }

    /*
     * The private constructor
     */
    two_bit_dpd_frame_recovery_impl::two_bit_dpd_frame_recovery_impl(float snr_threshold)
      : gr::sync_block("two_bit_dpd_frame_recovery",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(0, 0, 0)), d_snr_threshold(snr_threshold)
    {
	d_out_port = pmt::mp("out");	      
      	message_port_register_out(d_out_port);
	i_dpd_buf = 0;
	i_pwr_buf = 0;
	pwr_asm = 0.0;
	flag_asm = 0;
	flag_peak = 0;
	sample_in_symbol = 0;
	n_rx_bytes = 0;
	bit_in_byte = 0;
	ptr_current_byte = buf_rx_frame;
    }

    /*
     * Our virtual destructor.
     */
    two_bit_dpd_frame_recovery_impl::~two_bit_dpd_frame_recovery_impl()
    {
    }

    int
    two_bit_dpd_frame_recovery_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      int i, j;

      // Do <+signal processing+>
      for(i=0; i<noutput_items; i++)
      {
	// Calculate power of ASM
	buf_pwr[i_pwr_buf] = in[i].real()*in[i].real() + in[i].imag()*in[i].imag();

	int n_acc_buf = i_pwr_buf>>4;
	int i_acc_start = n_acc_buf<<4;

	float *ptr_acc_buf = buf_pwr_acc + n_acc_buf;
	*ptr_acc_buf = 0;
	float *ptr_pwr_buf = buf_pwr + i_acc_start;
	for(j=0; j<8; j++)
	{
		*ptr_acc_buf += *ptr_pwr_buf;
		ptr_pwr_buf++;
		*ptr_acc_buf += *ptr_pwr_buf;
		ptr_pwr_buf++;
	}

	i_pwr_buf++;
	if(i_pwr_buf == 128) i_pwr_buf = 0;

	pwr_asm = 0;
	ptr_acc_buf = buf_pwr_acc;
	for(j=0; j<4; j++)
	{
		pwr_asm += *ptr_acc_buf;
		ptr_acc_buf++;
		pwr_asm += *ptr_acc_buf;
		ptr_acc_buf++;
	}

	// Save to buffer_corr
	buf_corr[i_dpd_buf] = in[i].real();
	buf_corr[i_dpd_buf+1] = in[i].imag();

	i_dpd_buf += 2;
	if(i_dpd_buf==256) i_dpd_buf = 0;

	// Search for ASM
	if(flag_asm)
	{
		if(sample_in_symbol == 2)
		{
			float rx_symbol = in[i].real() * corr_real_last + in[i].imag() * corr_imag_last;
			*ptr_current_byte = *ptr_current_byte | ( ((rx_symbol>0)?0x80:0)>>bit_in_byte );
			
			bit_in_byte++;
			
			if(bit_in_byte == 8)
			{
				bit_in_byte = 0;
				n_rx_bytes++;
				ptr_current_byte++;
				
				if(n_rx_bytes == 246)
				{	
					flag_asm = 0;
					two_bit_dpd_frame_recovery_impl::message_port_pub(two_bit_dpd_frame_recovery_impl::d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(246, buf_rx_frame)));
					ptr_current_byte = buf_rx_frame;
				}
				else
				{
					*ptr_current_byte = 0;
				}
			}
		}
		
		sample_in_symbol++;			
		if(sample_in_symbol == 4)
		{
			sample_in_symbol = 0;
		}
	}
	else
	{			
		int offset;
		float corr_asm, corr1_real, corr1_imag, corr2_real, corr2_imag, corr_real, corr_imag;
		offset = i_dpd_buf>>1;
		
		//arm_cmplx_dot_prod_q15 (buf_corr+i_dpd_buf, asm32_dpd, 128-offset, &corr1_real, &corr1_imag);
		corr1_real = 0;
		corr1_imag = 0;
		for(j=0; j<128-offset; j++)
		{
			corr1_real += buf_corr[i_dpd_buf + 2*j] * asm32_dpd[2*j] - buf_corr[i_dpd_buf + 2*j + 1] * asm32_dpd[2*j + 1];
			corr1_imag += buf_corr[i_dpd_buf + 2*j] * asm32_dpd[2*j + 1] + buf_corr[i_dpd_buf + 2*j + 1] * asm32_dpd[2*j];
		}
		
		//arm_cmplx_dot_prod_q15 (buf_corr, asm32_dpd+256-i_dpd_buf, offset, &corr2_real, &corr2_imag);
		corr2_real = 0;
		corr2_imag = 0;
		for(j=0; j<offset; j++)
		{
			corr2_real += buf_corr[2*j] * asm32_dpd[256-i_dpd_buf + 2*j] - buf_corr[2*j + 1] * asm32_dpd[256-i_dpd_buf + 2*j + 1];
			corr2_imag += buf_corr[2*j] * asm32_dpd[256-i_dpd_buf + 2*j + 1] + buf_corr[2*j + 1] * asm32_dpd[256-i_dpd_buf + 2*j];
		}
		
		corr_real = corr1_real + corr2_real;
		corr_imag = corr1_imag + corr2_imag;
		corr_asm = corr_real * corr_real + corr_imag * corr_imag;
		
		if(corr_asm_last > corr_asm)
		{
			if(flag_peak)
			{
				if( corr_asm_last > (pwr_asm * d_snr_threshold) )
				{
					fprintf(stdout, "corr_asm_last = %f\n", corr_asm_last);
					fprintf(stdout, "pwr_asm = %f\n", pwr_asm);
					flag_asm = 1;
					sample_in_symbol = 1;
					
					bit_in_byte = 0;
					n_rx_bytes = 0;
					
					*ptr_current_byte = 0;
				}
				flag_peak = 0;
			}
		}
		else
		{
			flag_peak = 1;
		}
		corr_asm_last = corr_asm;
		if(!flag_asm)
		{
			corr_real_last = corr_real;
			corr_imag_last = corr_imag;
		}
	}

      }


      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

