/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "send_file_pdu_impl.h"
#include <gnuradio/io_signature.h>
#include <thread>
namespace gr {
namespace lilacsat {

send_file_pdu::sptr
send_file_pdu::make(const std::string& path, int pdu_len, int period, int terminal) {
    return gnuradio::make_block_sptr<send_file_pdu_impl>(path, pdu_len, period, terminal);
}


/*
 * The private constructor
 */
send_file_pdu_impl::send_file_pdu_impl(const std::string& path,
                                       int pdu_len,
                                       int period,
                                       int terminal)
    : gr::sync_block("send_file_pdu",
                     gr::io_signature::make(
                         0 /* min inputs */, 0 /* max inputs */, 0),
                     gr::io_signature::make(
                         0 /* min outputs */, 0 /*max outputs */, 0)) {
d_out_port = pmt::mp("out");	      
      	message_port_register_out(d_out_port);
      	
      	
      	d_fp = fopen(path.data(), "rb");
      	d_pdu_len = pdu_len;
      	d_period = period;
      	d_terminal = terminal;
      	
      	d_thread = gr::thread::thread([this] {run(); });						 
}
    void send_file_pdu_impl::run()
    {
    	uint8_t buffer[1024];
    	
    	while(1)
    	{    	
	    	std::this_thread::sleep_for(
	    	  std::chrono::milliseconds(static_cast<long>(d_period)));
	    	
	    	int len = fread(buffer, 1, d_pdu_len, d_fp);
	    	if(len == d_pdu_len)
	    	{
	    		send_file_pdu_impl::message_port_pub(send_file_pdu_impl::d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(len, (const uint8_t *)buffer))); 
	    	}
    	}
    }

/*
 * Our virtual destructor.
 */
send_file_pdu_impl::~send_file_pdu_impl() {}

int send_file_pdu_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items) {
        // Do <+signal processing+>

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
