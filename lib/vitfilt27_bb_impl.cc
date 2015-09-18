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
#include "vitfilt27_bb_impl.h"

namespace gr {
  namespace lilacsat {

    vitfilt27_bb::sptr
    vitfilt27_bb::make()
    {
      return gnuradio::get_initial_sptr
        (new vitfilt27_bb_impl());
    }

    /*
     * The private constructor
     */
    vitfilt27_bb_impl::vitfilt27_bb_impl()
      : gr::sync_decimator("vitfilt27_bb",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)), 16)
    {
	vitfilt27_init(&vi);
	set_output_multiple(16);
    }

    /*
     * Our virtual destructor.
     */
    vitfilt27_bb_impl::~vitfilt27_bb_impl()
    {
    }

    int
    vitfilt27_bb_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        unsigned const char *in = (const unsigned char *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];
	int i, j;
	unsigned char current_in[16];

        // Do <+signal processing+>
	for(i=0; i<noutput_items; i++)
	{
		for(j=0; j<16; j++) current_in[j] = (in[i*16+j])?255:0;
		vitfilt27_decode(&vi, current_in, (unsigned char *)out+i, 16);
	}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

