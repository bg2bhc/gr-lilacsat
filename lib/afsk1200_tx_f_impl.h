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

#ifndef INCLUDED_LILACSAT_AFSK1200_TX_F_IMPL_H
#define INCLUDED_LILACSAT_AFSK1200_TX_F_IMPL_H

#include <lilacsat/afsk1200_tx_f.h>

extern "C"
{
	#include "afsk/afsk.h"
	#include "afsk/ax25.h"
	#include "afsk/aprs.h"
}

namespace gr {
  namespace lilacsat {

    class afsk1200_tx_f_impl : public afsk1200_tx_f
    {
     private:
	pmt::pmt_t d_in_port;
	pmt::pmt_t d_ptt_port;
	const std::string d_destination;
	const std::string d_source;
	const std::string d_repeater1;
	const std::string d_repeater2;
	Afsk afsk;
	AX25Ctx ax25;
	AX25Call path[4];
	char d_ptt;
	      
	void pmt_in_callback(pmt::pmt_t msg);
	int setcall(AX25Call *ax25call, std::string str);
	static void AX25_MsgCallBack(void *obj_ptr, struct AX25Msg *msg);

     public:
      afsk1200_tx_f_impl(const std::string& destination, const std::string& source, const std::string& repeater1, const std::string& repeater2, bool padding_zero);
      ~afsk1200_tx_f_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_AFSK1200_TX_F_IMPL_H */

