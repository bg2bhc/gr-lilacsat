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
#include "afsk1200_tx_f_impl.h"

#define ERROR()	std::cout << "Error!" << std::endl; //exit(0);


namespace gr {
  namespace lilacsat {

    afsk1200_tx_f::sptr
    afsk1200_tx_f::make(const std::string& destination, const std::string& source, const std::string& repeater1, const std::string& repeater2, bool padding_zero)
    {
      return gnuradio::get_initial_sptr
        (new afsk1200_tx_f_impl(destination, source, repeater1, repeater2, padding_zero));
    }

    /*
     * The private constructor
     */
    afsk1200_tx_f_impl::afsk1200_tx_f_impl(const std::string& destination, const std::string& source, const std::string& repeater1, const std::string& repeater2, bool padding_zero)
      : gr::sync_block("afsk1200_tx_f",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(float))),
              d_destination(destination),
              d_source(source),
              d_repeater1(repeater1),
              d_repeater2(repeater2),
              d_ptt(0)
    {
	d_in_port = pmt::mp("in");
      	message_port_register_in(d_in_port);

	d_ptt_port = pmt::mp("ptt");
      	message_port_register_out(d_ptt_port);

	set_msg_handler(d_in_port, boost::bind(&afsk1200_tx_f_impl::pmt_in_callback, this ,_1) );
	
	afsk_init(&afsk);
	
	ax25_init(&ax25, &afsk.fd, (void *)0, 0);

	afsk.cfg_padding_zero = padding_zero;
    }

    /*
     * Our virtual destructor.
     */
    afsk1200_tx_f_impl::~afsk1200_tx_f_impl()
    {
    }


    int afsk1200_tx_f_impl::setcall(AX25Call *ax25call, std::string str)
    {
	if(!str.size())	return -1;	//长度为空
	for(int i=0; i<str.size(); i++)
	{
		if(i<6 && (((str[i] >= '0') && (str[i] <= '9')) || ((str[i] >= 'A') && (str[i] <= 'Z'))))
		{
			ax25call->call[i] = str[i];
		}
		else if((str[i] == '-') && (i>2))
		{
			if(i<6)
			{
				for(int j=i; j<6; j++) ax25call->call[j] = ' ';
			}

			if(str.size()==i+2)
			{
				if((str[i+1] >= '0') && (str[i+1] <= '9'))
				{
					ax25call->ssid = str[i+1];
				}
			}
			else if(str.size()==i+3)
			{
				if((str[i+1] == '1') && (str[i+2] >= '0') && (str[i+2] <= '5'))					
				{
					ax25call->ssid = 10+str[i+2];
				}
				else
				{
					return -2;//SSID数值过大
				}
			}
			else
			{
				return -3;//SSID字节数过多
			}
			
			break;
		}
		else
		{
			return -4;//非法字符或超长
			break;
		}
	}
	return 0;
    }

    void afsk1200_tx_f_impl::pmt_in_callback(pmt::pmt_t msg)
    {
	pmt::pmt_t meta(pmt::car(msg));
	pmt::pmt_t bytes(pmt::cdr(msg));

	size_t msg_len, n_path;
	const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);

	// int i = countof(d_destination);
	// printf("text: %c\n", (*d_destination)[0]);
	// std::cout << d_destination.size() << std::endl;

	// std::cout << d_source.size() << std::endl;
	// std::cout << afsk1200_tx_f_impl::setcall(path+1, d_source) << std::endl;
	if(afsk1200_tx_f_impl::setcall(path, d_destination) || afsk1200_tx_f_impl::setcall(path+1, d_source)) ERROR();
	if(afsk1200_tx_f_impl::setcall(path+2, d_repeater1)) n_path = 2;
	else if(afsk1200_tx_f_impl::setcall(path+3, d_repeater2)) n_path = 3;
	else n_path = 4;

	ax25_sendVia(&ax25, path, n_path, bytes_in, msg_len);
    }

    int
    afsk1200_tx_f_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        float *out = (float *) output_items[0];
	int n_ret;
	unsigned char msg_ptt_on[] = {0xFE, 0xFE, 0x7C, 0xE0, 0x1C, 0x00, 0x01, 0xFD};
	unsigned char msg_ptt_off[] = {0xFE, 0xFE, 0x7C, 0xE0, 0x1C, 0x00, 0x00, 0xFD};

        // Do <+signal processing+>
	n_ret = afsk_tx_proc(&afsk, out, noutput_items);

	if((d_ptt == 0) && (n_ret != 0))
	{
		message_port_pub(d_ptt_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(sizeof(msg_ptt_on), (const uint8_t *)msg_ptt_on)));
		d_ptt = 1;
	}
	else if((d_ptt != 0) && (n_ret == 0))
	{
		message_port_pub(d_ptt_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(sizeof(msg_ptt_off), (const uint8_t *)msg_ptt_off)));
		d_ptt = 0;
	}
    if(n_ret == 0) usleep(1000);
        // Tell runtime system how many output items we produced.
        return n_ret;
    }

  } /* namespace lilacsat */
} /* namespace gr */

