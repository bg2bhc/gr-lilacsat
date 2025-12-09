/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "sync_det_b_impl.h"
#include <gnuradio/io_signature.h>
extern char sequence[255];
extern unsigned char m_decode_tab[];
namespace gr {
namespace lilacsat {

using input_type = char;
sync_det_b::sptr
sync_det_b::make(uint32_t sync_word, uint32_t len, uint8_t nrz, uint8_t descrambling) {
    return gnuradio::make_block_sptr<sync_det_b_impl>(sync_word, len, nrz, descrambling);
}


/*
 * The private constructor
 */
sync_det_b_impl::sync_det_b_impl(uint32_t sync_word,
                                 uint32_t len,
                                 uint8_t nrz,
                                 uint8_t descrambling)
    : gr::sync_block("sync_det_b",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(
                         0 /* min outputs */, 0 /*max outputs */, 0)) {
	d_sync_word = sync_word;
		d_frame_length = len;
		d_nrz = nrz;
		d_descrambling = descrambling;
		d_rx_bit_state = 0;
	  	d_mask_bit_out = 0x80;
	  	d_syncing = 0;
	  	d_buffer_sync_det = 0;
	  	d_n_out = 0;

		d_out_port = pmt::mp("out");	      
	  	message_port_register_out(d_out_port);

	  	set_output_multiple(8);						 
}

/*
 * Our virtual destructor.
 */
sync_det_b_impl::~sync_det_b_impl() {}

int sync_det_b_impl::work(int noutput_items,
                          gr_vector_const_void_star& input_items,
                          gr_vector_void_star& output_items) {
    auto in = static_cast<const input_type*>(input_items[0]);
		int i, j;
		uint8_t mask_bit_in, current_in;
    // Do <+signal processing+>
		for (i = 0; i < noutput_items/8; i++)
		{
			for(j=0; j<8; j++)
		{
			current_in <<= 1;
			current_in += (*(in++))?1:0;
		}

			if(d_nrz == 1)
			{
				if(d_rx_bit_state)
				{
					d_rx_bit_state = current_in&0x01;
					current_in = m_decode_tab[0xFF-current_in];
				}
				else
				{
					d_rx_bit_state = current_in&0x01;
					current_in = m_decode_tab[current_in];
				}
			}

			for(mask_bit_in = 0x80; mask_bit_in; mask_bit_in >>= 1)
			{
				if(d_syncing)
				{
					if(current_in & mask_bit_in)
					{
						d_buf_sync_out[d_n_out] |= d_mask_bit_out;
					}
					else
					{
						d_buf_sync_out[d_n_out] &= (~d_mask_bit_out);
					}

					d_mask_bit_out >>= 1;

					if(d_mask_bit_out == 0x00)
					{
						if(d_descrambling) d_buf_sync_out[d_n_out] ^= sequence[d_n_out%255];

						d_mask_bit_out = 0x80;
						d_n_out++;

						if(d_n_out == d_frame_length)
						{
						    d_syncing = 0;
						    message_port_pub(d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(d_frame_length, (const uint8_t *)d_buf_sync_out)));
						}
					}
				}
				else
				{
					d_buffer_sync_det <<= 1;

					if(current_in & mask_bit_in)
					{
						d_buffer_sync_det += 1;
					}
					/* Allow no error in sync word */
					/* if(d_buffer_sync_det == d_sync_word) */
					/* Allow 1 error in sync word */
					switch(d_buffer_sync_det ^ d_sync_word)
					{
						case 0x00000000:
						case 0x00000001:
						case 0x00000002:
						case 0x00000004:
						case 0x00000008:
						case 0x00000010:
						case 0x00000020:
						case 0x00000040:
						case 0x00000080:
						case 0x00000100:
						case 0x00000200:
						case 0x00000400:
						case 0x00000800:
						case 0x00001000:
						case 0x00002000:
						case 0x00004000:
						case 0x00008000:
						case 0x00010000:
						case 0x00020000:
						case 0x00040000:
						case 0x00080000:
						case 0x00100000:
						case 0x00200000:
						case 0x00400000:
						case 0x00800000:
						case 0x01000000:
						case 0x02000000:
						case 0x04000000:
						case 0x08000000:
						case 0x10000000:
						case 0x20000000:
						case 0x40000000:
						case 0x80000000:
						{
							d_syncing = 1;
							d_buffer_sync_det = 0;
							d_mask_bit_out = 0x80;
							d_n_out = 0;
						}
					}

				}
			}
		}
    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
