/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "dslwp_tm_header_parser_impl.h"
#include <gnuradio/io_signature.h>
extern "C"
{
    #include "dslwp_tm_header.h"
}

#include <stdio.h>

namespace gr {
namespace lilacsat {

using input_type = char;
using output_type = char;
dslwp_tm_header_parser::sptr dslwp_tm_header_parser::make() {
    return gnuradio::make_block_sptr<dslwp_tm_header_parser_impl>();
}


/*
 * The private constructor
 */
dslwp_tm_header_parser_impl::dslwp_tm_header_parser_impl()
    : gr::block("dslwp_tm_header_parser",
                gr::io_signature::make(
                    0 /* min inputs */, 0 /* max inputs */, sizeof(input_type)),
                gr::io_signature::make(
                    0 /* min outputs */, 0 /*max outputs */, sizeof(output_type))) {
	d_in_port = pmt::mp("in");
    message_port_register_in(d_in_port);

	d_out_port_0 = pmt::mp("out 0");	      
    message_port_register_out(d_out_port_0);

	d_out_port_1 = pmt::mp("out 1");	      
    message_port_register_out(d_out_port_1);

	d_out_port_2 = pmt::mp("out 2");	      
    message_port_register_out(d_out_port_2);
	
	set_msg_handler(d_in_port, [this](pmt::pmt_t msg) { this->pmt_in_callback(msg); });
}

/*
 * Our virtual destructor.
 */
dslwp_tm_header_parser_impl::~dslwp_tm_header_parser_impl() {}

void dslwp_tm_header_parser_impl::forecast(int noutput_items,
                                           gr_vector_int& ninput_items_required) {
    /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
}


void dslwp_tm_header_parser_impl::pmt_in_callback(pmt::pmt_t msg) {
	pmt::pmt_t meta(pmt::car(msg));
	pmt::pmt_t bytes(pmt::cdr(msg));

	unsigned int i;
	size_t msg_len;
	tm_header_t header;
	const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);

//	for(i=0; i<LEN_TM_HEADER; i++)
//	{
//		((uint8_t *)&header)[i] = bytes_in[i];
//	}


	((uint8_t *)&header)[0] = bytes_in[4];
	((uint8_t *)&header)[1] = bytes_in[3];
	((uint8_t *)&header)[2] = bytes_in[2];
	((uint8_t *)&header)[3] = bytes_in[1];
	((uint8_t *)&header)[4] = bytes_in[0];

	fprintf(stdout, "version_number = 0x%x\n", header.version_number);
	fprintf(stdout, "spacecraft_id = 0x%x\n", header.spacecraft_id);
	fprintf(stdout, "virtual_channel_id = 0x%x\n", header.virtual_channel_id);
	fprintf(stdout, "ab_flag = 0x%x\n", header.ab_flag);
	fprintf(stdout, "master_frame_count = %d\n", header.master_frame_count);
	fprintf(stdout, "virtual_channel_frame_count = %d\n", header.virtual_channel_frame_count);
	fprintf(stdout, "first_header_pointer = %d\n", header.first_header_pointer);

	switch(header.virtual_channel_id)
	{
		case 0:
		{
			dslwp_tm_header_parser_impl::message_port_pub(dslwp_tm_header_parser_impl::d_out_port_0, pmt::cons(pmt::make_dict(), pmt::init_u8vector(msg_len-LEN_TM_HEADER, (const uint8_t *)bytes_in+LEN_TM_HEADER)));
			break;
		}
		case 1:
		{
			dslwp_tm_header_parser_impl::message_port_pub(dslwp_tm_header_parser_impl::d_out_port_1, pmt::cons(pmt::make_dict(), pmt::init_u8vector(msg_len-LEN_TM_HEADER, (const uint8_t *)bytes_in+LEN_TM_HEADER)));
			break;
		}
		case 2:
		{
			dslwp_tm_header_parser_impl::message_port_pub(dslwp_tm_header_parser_impl::d_out_port_2, pmt::cons(pmt::make_dict(), pmt::init_u8vector(msg_len-LEN_TM_HEADER, (const uint8_t *)bytes_in+LEN_TM_HEADER)));
			break;
		}
	}

}
int dslwp_tm_header_parser_impl::general_work(int noutput_items,
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
