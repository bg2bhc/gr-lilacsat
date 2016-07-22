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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "codec2_encode_fb_impl.h"

#define	SAMPLES_PER_FRAME	320
#define	BITS_PER_FRAME		52

namespace gr {
  namespace lilacsat {

    codec2_encode_fb::sptr
    codec2_encode_fb::make(int mode)
    {
      return gnuradio::get_initial_sptr
        (new codec2_encode_fb_impl(mode));
    }

    /*
     * The private constructor
     */
    codec2_encode_fb_impl::codec2_encode_fb_impl(int mode)
      : gr::sync_decimator("codec2_encode_fb",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, 8*sizeof(char)), SAMPLES_PER_FRAME)
    {
	codec2 = codec2_create(CODEC2_MODE_1300);
    }

    /*
     * Our virtual destructor.
     */
    codec2_encode_fb_impl::~codec2_encode_fb_impl()
    {
    }

    int
    codec2_encode_fb_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        char *out = (char *) output_items[0];
		int i, j;
		int16_t buf[SAMPLES_PER_FRAME];

		// Do <+signal processing+>
		for(i=0; i<noutput_items; i++)
		{
			for(j=0; j<SAMPLES_PER_FRAME; j++)
			{
				buf[j] = (int16_t)(in[i*SAMPLES_PER_FRAME+j]*(float)32767.0);
				codec2_encode(codec2, (unsigned char *)out+i*8, buf);
			}
		}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

