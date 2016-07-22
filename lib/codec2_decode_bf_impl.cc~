/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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
#include "codec2_decode_bf_impl.h"

#include <stdio.h>

#define	SAMPLES_PER_FRAME	320
#define	BITS_PER_FRAME		52

namespace gr {
  namespace lilacsat {

    codec2_decode_bf::sptr
    codec2_decode_bf::make(int mode)
    {
      return gnuradio::get_initial_sptr
        (new codec2_decode_bf_impl(mode));
    }

    /*
     * The private constructor
     */
    codec2_decode_bf_impl::codec2_decode_bf_impl(int mode)
      : gr::sync_interpolator("codec2_decode_bf",
              gr::io_signature::make(1, 1, 8*sizeof(char)),
              gr::io_signature::make(1, 1, sizeof(float)), SAMPLES_PER_FRAME)
    {
	codec2 = codec2_create(CODEC2_MODE_1300);
    }

    /*
     * Our virtual destructor.
     */
    codec2_decode_bf_impl::~codec2_decode_bf_impl()
    {
    }

    int
    codec2_decode_bf_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const char *in = (const char *) input_items[0];
        float *out = (float *) output_items[0];
	int i, j;
	int16_t buf[SAMPLES_PER_FRAME];

	// Do <+signal processing+>
	for(i=0; i<noutput_items/SAMPLES_PER_FRAME; i++)
	{
		codec2_decode(codec2, buf, (unsigned char *)in+i*8);
		for(j=0; j<SAMPLES_PER_FRAME; j++)
		{
			out[i*SAMPLES_PER_FRAME + j] = (float)buf[j]/32767.0;
		}
	}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

