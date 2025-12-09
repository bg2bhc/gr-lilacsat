/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_LILACSAT1_FRAME_DEPACK_IMPL_H
#define INCLUDED_LILACSAT_LILACSAT1_FRAME_DEPACK_IMPL_H

#include <gnuradio/lilacsat/lilacsat1_frame_depack.h>

#define	LENTH_BUF_CODEC2	700

extern "C"
{
    #include "afsk/fifobuf.h"
	#include "kiss/kiss.h"
}
namespace gr {
namespace lilacsat {

class lilacsat1_frame_depack_impl : public lilacsat1_frame_depack {
private:
      pmt::pmt_t d_in_port;
      pmt::pmt_t d_out_port;

      void pmt_in_callback(pmt::pmt_t msg);

	  Kiss			d_ki;
      static void	kiss_msg_callback(void *obj_ptr, char *ptr, uint16_t len);

	  uint8_t		d_buf[LENTH_BUF_CODEC2];
	  FIFOBuffer 	d_fifo;
	  uint8_t 		d_out_c2[7];
	  uint8_t		d_term;
public:
    lilacsat1_frame_depack_impl();
    ~lilacsat1_frame_depack_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_LILACSAT1_FRAME_DEPACK_IMPL_H */
