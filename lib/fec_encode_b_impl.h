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

#ifndef INCLUDED_LILACSAT_fec_encode_b_IMPL_H
#define INCLUDED_LILACSAT_fec_encode_b_IMPL_H

#include <lilacsat/fec_encode_b.h>

extern "C"
{
	#include "ccsds.h"
}

namespace gr {
  namespace lilacsat {

    class fec_encode_b_impl : public fec_encode_b
    {
     private:
      pmt::pmt_t d_in_port;
      pmt::pmt_t d_ptt_port;
      Ccsds cc;
      char d_ptt;
      void pmt_in_callback(pmt::pmt_t msg);

     public:
      fec_encode_b_impl(int frame_len, int preamble_len, int trailer_len, bool continous, bool padding_zero, bool using_m, bool using_convolutional_code);
      ~fec_encode_b_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_fec_encode_b_IMPL_H */

