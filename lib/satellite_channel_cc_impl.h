/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_SATELLITE_CHANNEL_CC_IMPL_H
#define INCLUDED_LILACSAT_SATELLITE_CHANNEL_CC_IMPL_H

#include <gnuradio/lilacsat/satellite_channel_cc.h>
#include <math.h>
#include <stdio.h>

#define	r_earth	6371e3
#define	GM	3.978e14
#define	PI	3.141592653589793
#define c	3e8

namespace gr {
namespace lilacsat {

class satellite_channel_cc_impl : public satellite_channel_cc {
private:
	double d_theta;// 地心角
	double d_theta0;
	double d_d_theta;
	float d_omega;// 卫星角速度
	double d_current_phase;// DCO相位

	float d_r_semi_long_axis;
	float d_alpha_aos;
	float d_fc;
	float d_samp_rate;
	float d_d0;
	bool d_doppler;
	bool d_link_loss;
	bool d_verbose;
	long d_nout;
	long d_nsecond;

public:
    satellite_channel_cc_impl(float r_semi_long_axis,
                              float alpha_aos,
                              float fc,
                              float samp_rate,
                              float d0,
                              bool doppler,
                              bool link_loss,
                              bool verbose);
    ~satellite_channel_cc_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_SATELLITE_CHANNEL_CC_IMPL_H */
