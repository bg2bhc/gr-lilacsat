/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_PLAN13_CC_IMPL_H
#define INCLUDED_LILACSAT_PLAN13_CC_IMPL_H

#include <gnuradio/lilacsat/plan13_cc.h>

#include "plan13/Plan13.h"

namespace gr {
namespace lilacsat {

class plan13_cc_impl : public plan13_cc {
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

	float k_real, k_imag;
	double  current_phase, f_doppler, f_doppler_var;
	int sample_in_second;
	char init;
	time_t time_curr, time_next, time_new;
	struct tm *tblock_curr, *tblock_next;

	pmt::pmt_t d_freq_port;
	pmt::pmt_t d_rotator_port;

	void ssplit(char *dest, char *src, int n);
	void freq_cmd_gen(unsigned char *dest, unsigned long freq);


public:
    plan13_cc_impl(bool enable,
                   const std::string& tle1,
                   const std::string& tle2,
                   float lon,
                   float lat,
                   float height,
                   float fc,
                   float samp_rate,
                   bool txrx,
                   bool verbose);
    ~plan13_cc_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_PLAN13_CC_IMPL_H */
