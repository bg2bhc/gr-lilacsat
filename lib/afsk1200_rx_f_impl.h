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

#ifndef INCLUDED_LILACSAT_AFSK1200_RX_F_IMPL_H
#define INCLUDED_LILACSAT_AFSK1200_RX_F_IMPL_H

#include <lilacsat/afsk1200_rx_f.h>
#include <stdio.h>

extern "C"
{
	#include "afsk/afsk.h"
	#include "afsk/ax25.h"
}

namespace gr {
  namespace lilacsat {

    class afsk1200_rx_f_impl : public afsk1200_rx_f
    {
     private:
      pmt::pmt_t d_out_port;
      Afsk afsk;
      AX25Ctx ax25;
      static void AX25_MsgCallBack(void *obj_ptr, struct AX25Msg *msg);
      void rx_init();

     public:
      afsk1200_rx_f_impl();
      ~afsk1200_rx_f_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_AFSK1200_RX_F_IMPL_H */

