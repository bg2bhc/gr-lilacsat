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
#include "plan13_cc_impl.h"

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <memory.h>

namespace gr {
  namespace lilacsat {

    plan13_cc::sptr
    plan13_cc::make(bool enable, const std::string& tle1, const std::string& tle2, float lon, float lat, float height, float fc, float samp_rate, bool txrx, bool verbose)
    {
      return gnuradio::get_initial_sptr
        (new plan13_cc_impl(enable, tle1, tle2, lon, lat, height, fc, samp_rate, txrx, verbose));
    }

    /*
     * The private constructor
     */
    plan13_cc_impl::plan13_cc_impl(bool enable, const std::string& tle1, const std::string& tle2, float lon, float lat, float height, float fc, float samp_rate, bool txrx, bool verbose)
      : gr::sync_block("plan13_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
		d_enable(enable), d_tle1(tle1), d_tle2(tle2), d_lon(lon), d_lat(lat), d_height(height), d_fc(fc), d_samp_rate(samp_rate), d_txrx(txrx), d_verbose(verbose)
    {
	char buffer[20];
	int tmp;

	d_freq_port = pmt::mp("freq");
      	message_port_register_out(d_freq_port);

	d_rotator_port = pmt::mp("rotator");
      	message_port_register_out(d_rotator_port);

	p13.setFrequency(d_fc, d_fc);
	p13.setLocation(d_lon, d_lat, d_height);
	
	ssplit(buffer, (char *)tle1.data()+18, 2);
	sscanf(buffer, "%lf", &d_YE);
	d_YE += 2000.0;

	ssplit(buffer, (char *)tle1.data()+20, 12);
	sscanf(buffer, "%lf", &d_TE);

	ssplit(buffer, (char *)tle1.data()+33, 10);
	sscanf(buffer, "%lf", &d_M2);

	ssplit(buffer, (char *)tle2.data()+8, 8);
	sscanf(buffer, "%lf", &d_IN);

	ssplit(buffer, (char *)tle2.data()+17, 8);
	sscanf(buffer, "%lf", &d_RA);

	ssplit(buffer, (char *)tle2.data()+26, 7);
	sscanf(buffer, "%lf", &d_EC);
	d_EC *= 0.0000001;

	ssplit(buffer, (char *)tle2.data()+34, 8);
	sscanf(buffer, "%lf", &d_WP);

	ssplit(buffer, (char *)tle2.data()+43, 8);
	sscanf(buffer, "%lf", &d_MA);

	ssplit(buffer, (char *)tle2.data()+52, 11);
	sscanf(buffer, "%lf", &d_MM);

	ssplit(buffer, (char *)tle2.data()+63, 5);
	sscanf(buffer, "%lf", &d_RV);

	d_ALON = 180.0;

	if(d_verbose)
	{
		fprintf(stdout, "Elements Set To:\n");
		fprintf(stdout, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n", d_YE, d_TE, d_IN, d_RA, d_EC, d_WP, d_MA, d_MM, d_M2, d_RV, d_ALON);
	}

	k_real = 1;
	k_imag = 0;
	current_phase = 0;
	f_doppler = 0.0;
	f_doppler_var = 0.0;
	sample_in_second = 0;
	init = 0;
    }

    void plan13_cc_impl::ssplit(char *dest, char *src, int n)
    {
	memcpy(dest, src, n);
	dest[n] = 0;
    }

    void plan13_cc_impl::freq_cmd_gen(unsigned char *dest, unsigned long freq)
    {
	dest[0] = 0xFE;
	dest[1] = 0xFE;
	dest[2] = 0x7C;
	dest[3] = 0xE0;
	dest[4] = 0x00;
	
	dest[9]  = freq / 1000000000*16;
	freq     = freq % 1000000000;

	dest[9] += freq / 100000000;
	freq     = freq % 100000000;

	dest[8]  = freq / 10000000*16;
	freq     = freq % 10000000;

	dest[8] += freq / 1000000;
	freq     = freq % 1000000;

	dest[7]  = freq / 100000*16;
	freq     = freq % 100000;

	dest[7] += freq / 10000;
	freq     = freq % 10000;

	dest[6]  = freq / 1000*16;
	freq     = freq % 1000;

	dest[6] += freq / 100;
	freq     = freq % 100;

	dest[5]  = freq / 10*16;
	freq     = freq % 10;

	dest[5] += freq;

	dest[10]  = 0xFD;
    }

    /*
     * Our virtual destructor.
     */
    plan13_cc_impl::~plan13_cc_impl()
    {
    }

    int
    plan13_cc_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

	int i;
	unsigned char msg_freq[] = {0xFE, 0xFE, 0x7C, 0xE0, 0x00, 0x00, 0x50, 0x37, 0x44, 0x01, 0xFD};

	if(d_enable && noutput_items)
	{
		time_new = time(NULL);
		if(!init)
		{
			init = 1;
			sample_in_second = 0;
			time_curr = time_new;		
		}
		else if( ((time_new - time_curr)>1) || ((time_curr - time_new)>1) )
		{
			sample_in_second = 0;
			time_curr = time_new;

			tblock_curr =  gmtime(&time_curr);
			p13.setTime(tblock_curr->tm_year+1900, tblock_curr->tm_mon+1, tblock_curr->tm_mday, tblock_curr->tm_hour, tblock_curr->tm_min, tblock_curr->tm_sec);
			p13.setElements(d_YE, d_TE, d_IN, d_RA, d_EC, d_WP, d_MA, d_MM, d_M2, d_RV, d_ALON);
			p13.initSat();
			p13.satvec();
			p13.rangevec();
			if(d_txrx)
			{
				f_doppler = (double)p13.txOutLong - (double)p13.rxFrequencyLong;
				freq_cmd_gen((unsigned char *)msg_freq, p13.txOutLong);
			}
			else
			{
				f_doppler = (double)p13.rxFrequencyLong - (double)p13.rxOutLong;
				freq_cmd_gen((unsigned char *)msg_freq, p13.rxOutLong);
			}

			message_port_pub(d_freq_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(sizeof(msg_freq), (const uint8_t *)msg_freq)));

			if(d_verbose)
			{
				fprintf(stdout, "%02d:%02d:%02d ", tblock_curr->tm_hour, tblock_curr->tm_min, tblock_curr->tm_sec);
				fprintf(stdout, "AZ: %lf ", p13.AZ);
				fprintf(stdout, "EL: %lf ", p13.EL);
				fprintf(stdout, "SLAT: %lf ", p13.SLAT);
				fprintf(stdout, "SLON: %lf ", p13.SLON);
				fprintf(stdout, "RR: %lf ", p13.RR);
				fprintf(stdout, "Doppler: %lf\n", f_doppler);
			}

			// Linear Interpolation
			time_next = time_curr + 1;
			tblock_next =  gmtime(&time_next);
			p13.setTime(tblock_next->tm_year+1900, tblock_next->tm_mon+1, tblock_next->tm_mday, tblock_next->tm_hour, tblock_next->tm_min, tblock_next->tm_sec); 
			p13.setElements(d_YE, d_TE, d_IN, d_RA, d_EC, d_WP, d_MA, d_MM, d_M2, d_RV, d_ALON);
			p13.initSat();
			p13.satvec();
			p13.rangevec();
			if(d_txrx)
			{
				f_doppler_var = ((double)p13.txOutLong - (double)p13.txFrequencyLong - f_doppler)/d_samp_rate;
			}
			else
			{
				f_doppler_var = ((double)p13.rxFrequencyLong - (double)p13.rxOutLong - f_doppler)/d_samp_rate;
			}	
		}
	}

    // Do <+signal processing+>
	for(i=0; i<noutput_items; i++)
	{
		if(d_enable)
		{
			if(sample_in_second >= d_samp_rate)
			{
				sample_in_second = 0;
				time_curr++;

				time_new = time(NULL);

				if( ((time_new - time_curr)>1) || ((time_curr - time_new)>1) )
				{
					time_curr = time_new;
				}

				tblock_curr =  gmtime(&time_curr);
				p13.setTime(tblock_curr->tm_year+1900, tblock_curr->tm_mon+1, tblock_curr->tm_mday, tblock_curr->tm_hour, tblock_curr->tm_min, tblock_curr->tm_sec);
				p13.setElements(d_YE, d_TE, d_IN, d_RA, d_EC, d_WP, d_MA, d_MM, d_M2, d_RV, d_ALON);
				p13.initSat();
				p13.satvec();
				p13.rangevec();
				if(d_txrx)
				{
					f_doppler = (double)p13.txOutLong - (double)p13.rxFrequencyLong;
					freq_cmd_gen((unsigned char *)msg_freq, p13.txOutLong);
				}
				else
				{
					f_doppler = (double)p13.rxFrequencyLong - (double)p13.rxOutLong;
					freq_cmd_gen((unsigned char *)msg_freq, p13.rxOutLong);
				}

				message_port_pub(d_freq_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(sizeof(msg_freq), (const uint8_t *)msg_freq)));

				if(d_verbose)
				{
					fprintf(stdout, "%02d:%02d:%02d ", tblock_curr->tm_hour, tblock_curr->tm_min, tblock_curr->tm_sec);
					fprintf(stdout, "AZ: %lf ", p13.AZ);
					fprintf(stdout, "EL: %lf ", p13.EL);
					fprintf(stdout, "SLAT: %lf ", p13.SLAT);
					fprintf(stdout, "SLON: %lf ", p13.SLON);
					fprintf(stdout, "RR: %lf ", p13.RR);
					fprintf(stdout, "Doppler: %lf\n", f_doppler);
				}

				// Linear Interpolation
				time_next = time_curr + 1;
				tblock_next =  gmtime(&time_next);
				p13.setTime(tblock_next->tm_year+1900, tblock_next->tm_mon+1, tblock_next->tm_mday, tblock_next->tm_hour, tblock_next->tm_min, tblock_next->tm_sec); 
				p13.setElements(d_YE, d_TE, d_IN, d_RA, d_EC, d_WP, d_MA, d_MM, d_M2, d_RV, d_ALON);
				p13.initSat();
				p13.satvec();
				p13.rangevec();
				if(d_txrx)
				{
					f_doppler_var = ((double)p13.txOutLong - (double)p13.txFrequencyLong - f_doppler)/d_samp_rate;
				}
				else
				{
					f_doppler_var = ((double)p13.rxFrequencyLong - (double)p13.rxOutLong - f_doppler)/d_samp_rate;
				}
			}	

			current_phase += 2*M_PI*f_doppler/d_samp_rate;
		
			sample_in_second++;
			f_doppler += f_doppler_var;
	
			k_real = cos(current_phase);
			k_imag = sin(current_phase);
		}

		*((float*)output_items[0]+2*i) = k_real * *((float*)input_items[0]+2*i) - k_imag * *((float*)input_items[0]+2*i+1);
		*((float*)output_items[0]+2*i+1) = k_real * *((float*)input_items[0]+2*i+1) + k_imag * *((float*)input_items[0]+2*i);
	}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

