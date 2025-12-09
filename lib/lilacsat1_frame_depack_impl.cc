/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "lilacsat1_frame_depack_impl.h"
#include <gnuradio/io_signature.h>
#include <stdio.h>
namespace gr {
namespace lilacsat {

using output_type = char;
lilacsat1_frame_depack::sptr lilacsat1_frame_depack::make() {
    return gnuradio::make_block_sptr<lilacsat1_frame_depack_impl>();
}


/*
 * The private constructor
 */
lilacsat1_frame_depack_impl::lilacsat1_frame_depack_impl()
    : gr::sync_block("lilacsat1_frame_depack",
                     gr::io_signature::make(
                         0 /* min inputs */, 0 /* max inputs */, 0),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, 8*sizeof(output_type))) {
	d_in_port = pmt::mp("in");
		message_port_register_in(d_in_port);

		d_out_port = pmt::mp("out");	      
		message_port_register_out(d_out_port);

		set_msg_handler(d_in_port, [this](pmt::pmt_t msg) { this->pmt_in_callback(msg); } );

		kiss_init(&d_ki, (void *)this, &lilacsat1_frame_depack_impl::kiss_msg_callback);
		fifo_init(&d_fifo, d_buf, LENTH_BUF_CODEC2);

		d_term = 1;
	}

/*
 * Our virtual destructor.
 */
lilacsat1_frame_depack_impl::~lilacsat1_frame_depack_impl() {}

void lilacsat1_frame_depack_impl::kiss_msg_callback(void *obj_ptr, char *ptr, uint16_t len) {
		lilacsat1_frame_depack_impl *obj_ptr_loc = (lilacsat1_frame_depack_impl *)obj_ptr;
		obj_ptr_loc->message_port_pub(obj_ptr_loc->d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(len, (const uint8_t *)ptr)));
}

void lilacsat1_frame_depack_impl::pmt_in_callback(pmt::pmt_t msg) {
		pmt::pmt_t meta(pmt::car(msg));
		pmt::pmt_t bytes(pmt::cdr(msg));

		size_t msg_len, n_path;
		const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);
		int i, j, i_data_field = 0;
        uint8_t buf_data_field[81];
		if(msg_len != 116)
		{
			return;
		}

		for(i=0; i<msg_len; i++)
		{
			uint8_t di = (i + 4) % 24;

			if(di >= 17)
			{
				d_out_c2[di-17] = bytes_in[i];

				if(di == 23)
				{
					if(d_out_c2[6] & 0x01)
					{
						d_out_c2[6] &= 0xf0;
						if(fifo_room(&d_fifo) >= 7)
						{
							for(j=0; j<7; j++)
							{
								fifo_push(&d_fifo, d_out_c2[j]);
							}
						}
					}
				}
			}
			else
			{
				//kiss_rx_byte(&d_ki, bytes_in[i]);
                buf_data_field[i_data_field++] = bytes_in[i];                
			}
		}
        this->message_port_pub(this->d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(81, (const uint8_t *)buf_data_field)));
}
int lilacsat1_frame_depack_impl::work(int noutput_items,
                                      gr_vector_const_void_star& input_items,
                                      gr_vector_void_star& output_items) {
        auto out = static_cast<output_type*>(output_items[0]);
		int i, j, k=0;

    // Do <+signal processing+>

	for(i=0; i<noutput_items; i++)
		{
			if(d_term)
			{
				if((fifo_len(&d_fifo) - fifo_room(&d_fifo)) >= 70)
				{
					for(j=0; j<7; j++)
					{
						out[8*i+j] = fifo_pop(&d_fifo);
					}
					d_term = 0;
				}
				else
				{
					d_term = 1;
					return i;
				}
			}
			else
			{
				if((fifo_len(&d_fifo) - fifo_room(&d_fifo)) >= 7)
				{
					for(j=0; j<7; j++)
					{
						out[8*i+j] = fifo_pop(&d_fifo);
					}
					d_term = 0;
				}
				else
				{
					d_term = 1;
					return i;
				}
			} 
		}
    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
