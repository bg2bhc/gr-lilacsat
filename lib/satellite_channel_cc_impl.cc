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
#include "satellite_channel_cc_impl.h"

namespace gr {
  namespace lilacsat {

    satellite_channel_cc::sptr
    satellite_channel_cc::make(float r_semi_long_axis, float alpha_aos, float fc, float samp_rate, float d0, bool doppler, bool link_loss, bool verbose)
    {
      return gnuradio::get_initial_sptr
        (new satellite_channel_cc_impl(r_semi_long_axis, alpha_aos, fc, samp_rate, d0, doppler, link_loss, verbose));
    }

    /*
     * The private constructor
     */
    satellite_channel_cc_impl::satellite_channel_cc_impl(float r_semi_long_axis, float alpha_aos, float fc, float samp_rate, float d0, bool doppler, bool link_loss, bool verbose)
      : gr::sync_block("satellite_channel_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))), 
	d_r_semi_long_axis(r_semi_long_axis), d_alpha_aos(alpha_aos), d_fc(fc), d_samp_rate(samp_rate), d_d0(d0), d_doppler(doppler), d_link_loss(link_loss), d_verbose(verbose), d_nout((long)d_samp_rate), d_nsecond(0)
    {
	d_theta0 = asin(sin(PI/2+alpha_aos/180*PI) * r_earth / r_semi_long_axis) + alpha_aos/180*PI;
	d_omega = sqrt(GM/(pow(r_semi_long_axis, 3)));

	d_theta = d_theta0;
	d_d_theta = d_omega/samp_rate;
	d_current_phase = 0;
    }

    /*
     * Our virtual destructor.
     */
    satellite_channel_cc_impl::~satellite_channel_cc_impl()
    {
    }

    int
    satellite_channel_cc_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];
	
	int i;
	static float d, v, alpha, f_doppler = 0.0, loss = 1.0, k_real, k_imag;
	
	// Do <+signal processing+>
	for(i=0; i<noutput_items; i++)
	{
		d = sqrt(r_earth*r_earth + pow(d_r_semi_long_axis, 2) - 2*r_earth*d_r_semi_long_axis*sin(d_theta));
		if(d_theta <= PI/2) alpha = atan((sin(d_theta) - r_earth/d_r_semi_long_axis)/cos(d_theta));
		else alpha = atan((sin(PI-d_theta) - r_earth/d_r_semi_long_axis)/cos(PI-d_theta));
		v = d_omega*d_r_semi_long_axis*sin(d_theta-alpha);
		
		if(d_doppler)
		{
			f_doppler = v/c*d_fc;
			if(d_theta > PI/2) f_doppler = -f_doppler;
			d_current_phase += 2*PI*f_doppler/d_samp_rate;
		}
		
		k_real = cos(d_current_phase);
		k_imag = sin(d_current_phase);

		if(d_link_loss)
		{
			loss = d_d0 / d;
			k_real = k_real * loss;
			k_imag = k_imag * loss;
		}

		*((float*)output_items[0]+2*i) = k_real * *((float*)input_items[0]+2*i) - k_imag * *((float*)input_items[0]+2*i+1);
		*((float*)output_items[0]+2*i+1) = k_real * *((float*)input_items[0]+2*i+1) + k_imag * *((float*)input_items[0]+2*i);
        	
		if(d_verbose)
		{
			d_nout--;
			if(d_nout <= 0)
			{
				d_nout=(long)d_samp_rate;
				d_nsecond++;
				fprintf(stdout, "\n****Satellite****\nSecond :%ld, ELV: %f deg, Doupler: %f Hz, Loss: %f dB\n", d_nsecond, alpha/PI*180, f_doppler, 20*log10(loss));
			}
		}

		d_theta += d_d_theta;
		if(d_theta >= (PI-d_theta0))
		{
			d_theta = d_theta0;
			d_nsecond = 0;
		}
	}
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

