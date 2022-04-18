/* -*- c++ -*- */
/*
 * Copyright 2021 gr-lilacsat author.
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

#ifndef INCLUDED_LILACSAT_CCSDS_AFSK_DECODE_IMPL_H
#define INCLUDED_LILACSAT_CCSDS_AFSK_DECODE_IMPL_H

#include <lilacsat/ccsds_afsk_decode.h>
extern "C"
{
	#include "ccsds_afsk/ccsds_afsk.h"
}
namespace gr {
  namespace lilacsat {

    class ccsds_afsk_decode_impl : public ccsds_afsk_decode
    {
     private:
      pmt::pmt_t d_out_port;
      Ccsds_afsk cc;
      bool d_pass_all;
      static void callback(unsigned char *buf, unsigned short len, int16_t byte_corr, void *obj_ptr);

     public:
      ccsds_afsk_decode_impl(int frame_len, bool using_m, bool using_convolutional_code, bool pass_all);
      ~ccsds_afsk_decode_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_CCSDS_AFSK_DECODE_IMPL_H */

