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

#ifndef INCLUDED_LILACSAT_PLAN13_CC_IMPL_H
#define INCLUDED_LILACSAT_PLAN13_CC_IMPL_H

#include <lilacsat/plan13_cc.h>

#include "Plan13.h"

namespace gr {
  namespace lilacsat {

    class plan13_cc_impl : public plan13_cc
    {
     private:
	bool d_enable;
	const std::string& d_tle1;
	const std::string& d_tle2;
	float d_lon;
	float d_lat;
	float d_height;
	float d_fc;
	float d_samp_rate;
	bool d_txrx;
	bool d_verbose;

	Plan13 p13;
	double d_YE;
	double d_TE;
	double d_IN;
	double d_RA;
	double d_EC;
	double d_WP;
	double d_MA;
	double d_MM;
	double d_M2;
	double d_RV;
	double d_ALON;

	pmt::pmt_t d_freq_port;
	pmt::pmt_t d_rotator_port;

	void ssplit(char *dest, char *src, int n);
	void freq_cmd_gen(unsigned char *dest, unsigned long freq);

     public:
      plan13_cc_impl(bool enable, const std::string& tle1, const std::string& tle2, float lon, float lat, float height, float fc, float samp_rate, bool txrx, bool verbose);
      ~plan13_cc_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_PLAN13_CC_IMPL_H */

