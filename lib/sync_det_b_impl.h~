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

#ifndef INCLUDED_LILACSAT_SYNC_DET_B_IMPL_H
#define INCLUDED_LILACSAT_SYNC_DET_B_IMPL_H

#include <lilacsat/sync_det_b.h>

#define	LENTH_BUF_SYNC_OUT	1024

namespace gr {
  namespace lilacsat {

    class sync_det_b_impl : public sync_det_b
    {
     private:
	  pmt::pmt_t	d_out_port;
	  uint32_t		d_sync_word;
	  uint8_t		d_frame_length;
	  uint8_t		d_nrz;
	  uint8_t		d_descrambling;
	  uint8_t		d_rx_bit_state;
	  uint8_t		d_mask_bit_out;
	  uint8_t		d_syncing;
      uint32_t		d_buffer_sync_det;
	  uint8_t		d_n_out;
	  uint8_t		d_buf_sync_out[LENTH_BUF_SYNC_OUT];

      static void	kiss_msg_callback(void *obj_ptr, char *ptr, uint16_t len);

     public:
      sync_det_b_impl(uint32_t sync_word, uint8_t len, uint8_t nrz, uint8_t descrambling);
      ~sync_det_b_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_SYNC_DET_B_IMPL_H */

