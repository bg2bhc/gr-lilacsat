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

#include <math.h>

#include <gnuradio/io_signature.h>
#include "angle_cf_impl.h"

namespace gr {
  namespace lilacsat {

    angle_cf::sptr
    angle_cf::make()
    {
      return gnuradio::get_initial_sptr
        (new angle_cf_impl());
    }

    /*
     * The private constructor
     */
    angle_cf_impl::angle_cf_impl()
      : gr::sync_block("angle_cf",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {}

    /*
     * Our virtual destructor.
     */
    angle_cf_impl::~angle_cf_impl()
    {
    }

    #define	PI	3.141592653589793
    int
    angle_cf_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        float *out = (float *) output_items[0];
	int i;
	float real, imag;

        // Do <+signal processing+>
	for(i=0; i<noutput_items; i++)
	{
		real = ((float *)in)[i*2];
		imag = ((float *)in)[i*2+1];
		
		if(real == 0) // Y Axis
		{
			if(imag >= 0)	out[i] = PI/2;
			else		out[i] = -PI/2;
		}
		else
		{
			out[i] = atan(imag/real);
			if(real < 0)
			{
				if(imag >= 0)	out[i] += PI;
				else		out[i] -= PI;
			}
		}
	}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

