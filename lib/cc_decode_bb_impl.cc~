/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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
#include "cc_decode_bb_impl.h"

#include <stdio.h>

extern char sequence[255];
extern unsigned char m_decode_tab[];

#define	LEN_FRAME	116
#define	SYNC_WORD	0x1ACFFC1D

namespace gr {
  namespace lilacsat {

    cc_decode_bb::sptr
    cc_decode_bb::make()
    {
      return gnuradio::get_initial_sptr
        (new cc_decode_bb_impl());
    }

    /*
     * The private constructor
     */
    cc_decode_bb_impl::cc_decode_bb_impl()
      : gr::block("cc_decode_bb",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(1, 1, 7*sizeof(char)))
    {
		d_rx_bit_state = 0;
	  	d_mask_bit_out = 0x80;
	  	d_syncing = 0;
      	d_buffer_sync_det = 0;
	  	d_n_out = 0;

		d_out_port = pmt::mp("out");	      
      	message_port_register_out(d_out_port);

		kiss_init(&d_ki, (void *)this, &cc_decode_bb_impl::kiss_msg_callback);
	}

    /*
     * Our virtual destructor.
     */
    cc_decode_bb_impl::~cc_decode_bb_impl()
    {
    }

    void
    cc_decode_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    void cc_decode_bb_impl::kiss_msg_callback(void *obj_ptr, char *ptr, uint16_t len)
    {
		cc_decode_bb_impl *obj_ptr_loc = (cc_decode_bb_impl *)obj_ptr;
		//obj_ptr_loc->message_port_pub(obj_ptr_loc->d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(len, (const uint8_t *)ptr)));
    }

    int
    cc_decode_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const char *in = (const char *) input_items[0];
        char *out = (char *) output_items[0];
		int i, j, n_codec2_out = 0;
		uint8_t mask_bit_in, current_in;

        // Do <+signal processing+>
        // Tell runtime system how many input items we consumed on
        // each input stream.
		fprintf(stdout, "%d\n", *in);

		for (i = 0; i < noutput_items/8; i++)
		{
			for(j=0; j<8; j++)
            {
                current_in <<= 1;
                current_in += (*(in++))?1:0;
            }

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
					    uint8_t di = (d_n_out + 4) % 24;

						d_buf_sync_out[d_n_out] ^= sequence[d_n_out];

						if(di >= 17)
						{
							out[di-17] = d_buf_sync_out[d_n_out];

							if(di == 23)
							{
								if(out[di-17] & 0x01)
								{
									out[di-17] &= 0xf0;
									n_codec2_out++;
									out += 7;
								}
							}
						}
						else
						{
							kiss_rx_byte(&d_ki, d_buf_sync_out[d_n_out]);
						}

						d_mask_bit_out = 0x80;
					    d_n_out++;

					    if(d_n_out == LEN_FRAME)
					    {
					        d_syncing = 0;
					        // byte_corr = decode_rs(d_buf_sync_out, (int *)0, 0, RS_BLOCK_LENGTH-d_len_frame-RS_LENGTH);
					        // d_hook(d_buf_sync_out, d_len_frame, byte_corr, d_obj_ptr);
message_port_pub(d_out_port, pmt::cons(pmt::make_dict(), pmt::init_u8vector(LEN_FRAME, (const uint8_t *)d_buf_sync_out)));
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
					switch(d_buffer_sync_det ^ SYNC_WORD)
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
							fprintf(stdout, "sync\n");
						}
					}

				}
			}
		}

        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return n_codec2_out;
    }

  } /* namespace lilacsat */
} /* namespace gr */

