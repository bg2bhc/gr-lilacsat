/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "pipe_sink_pdu_impl.h"
#include <gnuradio/io_signature.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace gr {
namespace lilacsat {

pipe_sink_pdu::sptr pipe_sink_pdu::make(const std::string& label,
                                        const std::string& path,
                                        uint16_t permission,
                                        bool debug) {
    return gnuradio::make_block_sptr<pipe_sink_pdu_impl>(label, path, permission, debug);
}


/*
 * The private constructor
 */
pipe_sink_pdu_impl::pipe_sink_pdu_impl(const std::string& label,
                                       const std::string& path,
                                       uint16_t permission,
                                       bool debug)
    : gr::block("pipe_sink_pdu",
                gr::io_signature::make(
                    0 /* min inputs */, 0 /* max inputs */, 0),
                gr::io_signature::make(
                    0 /* min outputs */, 0 /*max outputs */, 0)),
				d_label(label),
			    d_path(path),
			    d_debug(debug) {
	d_in_port = pmt::mp("in");
      		message_port_register_in(d_in_port);
		set_msg_handler(d_in_port, [this](pmt::pmt_t msg) { this->pmt_in_callback(msg); } );
//		printf("asasasasa");
		if(d_debug)
		{
//			fprintf(stdout, "pipe %s\n", path.data());
//			fprintf(stdout, "pipe %s\n", (&d_path));
//			fprintf(stdout, "pipe size %d\n", path.size());
//			path.data();

		}
		
		mkfifo((const char *)(d_path.data()), (mode_t)permission);
}

	void pipe_sink_pdu_impl::pmt_in_callback(pmt::pmt_t msg)
    {
		pmt::pmt_t meta(pmt::car(msg));
		pmt::pmt_t bytes(pmt::cdr(msg));
	
		int pipefd = 0, ret, i;
		size_t msg_len;
		const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);
		uint8_t buffer[256] = {0,};

		buffer[0] = (uint8_t)msg_len;
		for (i=0;i<buffer[0];i++){
			buffer[i+1] = bytes_in[i];
		}

		pipefd=open((const char *)(d_path.data()),O_RDWR);

		ret = write(pipefd,buffer,msg_len + 1);

		if(d_debug)
		{
			fprintf(stdout, "%02x", (unsigned int)msg_len);
			//fprintf(stdout, "Send %02x bytes to pipe %s", msg_len, d_label);
		}
		
		close(pipefd);
    }
/*
 * Our virtual destructor.
 */
pipe_sink_pdu_impl::~pipe_sink_pdu_impl() {}

void pipe_sink_pdu_impl::forecast(int noutput_items,
                                  gr_vector_int& ninput_items_required) {
    /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
}

int pipe_sink_pdu_impl::general_work(int noutput_items,
                                     gr_vector_int& ninput_items,
                                     gr_vector_const_void_star& input_items,
                                     gr_vector_void_star& output_items) {
    // Do <+signal processing+>
    // Tell runtime system how many input items we consumed on
    // each input stream.
    consume_each(noutput_items);

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
