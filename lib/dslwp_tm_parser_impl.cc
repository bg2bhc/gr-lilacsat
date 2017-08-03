/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
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
#include "dslwp_tm_parser_impl.h"

#include "stdio.h"

extern "C"
{
    #include "dslwp_hk.h"
}

#define sw16(x) (uint16_t)( ( ( ((uint16_t)x) & ((uint16_t)0x00FFU) ) << 8 ) | ( ( ((uint16_t)x) & ((uint16_t)0xFF00U) ) >> 8 ) )
#define sw32(x) (uint32_t)( ( ( ((uint32_t)x) & ((uint32_t)0x000000FFU) ) << 24 ) | ( ( ((uint32_t)x) & ((uint32_t)0x0000FF00U) ) << 8 ) | ( ( ((uint32_t)x) & ((uint32_t)0x00FF0000U) ) >> 8 ) | ( ( ((uint32_t)x) & ((uint32_t)0xFF000000U) ) >> 24 ) )

namespace gr {
  namespace lilacsat {

    dslwp_tm_parser::sptr
    dslwp_tm_parser::make()
    {
      return gnuradio::get_initial_sptr
        (new dslwp_tm_parser_impl());
    }

    /*
     * The private constructor
     */
    dslwp_tm_parser_impl::dslwp_tm_parser_impl()
      : gr::block("dslwp_tm_parser",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
	d_in_port = pmt::mp("in");
      	message_port_register_in(d_in_port);
	set_msg_handler(d_in_port, boost::bind(&dslwp_tm_parser_impl::pmt_in_callback, this ,_1) );
    }

    /*
     * Our virtual destructor.
     */
    dslwp_tm_parser_impl::~dslwp_tm_parser_impl()
    {
    }

    void dslwp_tm_parser_impl::pmt_in_callback(pmt::pmt_t msg)
    {
	pmt::pmt_t meta(pmt::car(msg));
	pmt::pmt_t bytes(pmt::cdr(msg));

	unsigned int i;
	size_t msg_len;
	const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);

	
	if(msg_len == sizeof(hk_uv_t))
	{
		hk_uv_t hk_uv;
		for(i=0; i<sizeof(hk_uv_t); i++)
		{
			((uint8_t *)&hk_uv)[i] = bytes_in[i];
		}

		fprintf(stdout, "\n**** V/U Transceiver Telemetry\n");
		fprintf(stdout, "config = 0x%02x\n", hk_uv.config);
		fprintf(stdout, "tx_mode = 0x%02x\n", hk_uv.tx_mode);
		fprintf(stdout, "tx_gain = 0x%02x\n", hk_uv.tx_gain);
		fprintf(stdout, "flag_7021 = 0x%02x\n", hk_uv.flag_7021);
		fprintf(stdout, "n_cmd_buf = 0x%02x\n", hk_uv.n_cmd_buf);
		fprintf(stdout, "n_cmd_dropped = 0x%02x\n", hk_uv.n_cmd_dropped);
		fprintf(stdout, "i_bus_rx = %f\n", ((float)hk_uv.i_bus_rx) * 3.2f);
		fprintf(stdout, "u_bus_rx = %f\n", ((float)hk_uv.u_bus_rx) * 0.16f);
		fprintf(stdout, "i_bus_tx = %f\n", ((float)hk_uv.i_bus_tx) * 3.2f);
		fprintf(stdout, "u_bus_tx = %f\n", ((float)hk_uv.u_bus_tx) * 0.16f);
		fprintf(stdout, "t_pa = %d\n", hk_uv.t_pa);
		fprintf(stdout, "t_tx7021 = 0x%02x\n", hk_uv.t_tx7021);
		fprintf(stdout, "n_422_tx = %d\n", hk_uv.n_422_tx);
		fprintf(stdout, "n_422_rx = %d\n", hk_uv.n_422_rx);
		fprintf(stdout, "n_422_rx_pkg_err = %d\n", hk_uv.n_422_rx_pkg_err);
		fprintf(stdout, "n_422_rx_exe_err = %d\n", hk_uv.n_422_rx_exe_err);
		fprintf(stdout, "cmd_422_last_rx = 0x%02x\n", hk_uv.cmd_422_last_rx);
		fprintf(stdout, "n_rf_tx = %d\n", hk_uv.n_rf_tx);
		fprintf(stdout, "n_rf_tx_dropped = %d\n", hk_uv.n_rf_tx_dropped);
		fprintf(stdout, "n_rf_rx = %d\n", hk_uv.n_rf_rx);
		fprintf(stdout, "n_rf_rx_pkg_err = %d\n", hk_uv.n_rf_rx_pkg_err);
		fprintf(stdout, "n_rf_rx_exe_err = %d\n", hk_uv.n_rf_rx_exe_err);
		fprintf(stdout, "n_rf_rx_fec_err = %d\n", hk_uv.n_rf_rx_fec_err);
		fprintf(stdout, "cmd_rf_last_rx = 0x%02x\n", hk_uv.cmd_rf_last_rx);
		fprintf(stdout, "n_ul = %d\n", sw32(hk_uv.n_ul));
		fprintf(stdout, "fc_tc = 0x%04x\n", hk_uv.fc_tc);
		fprintf(stdout, "fc_ham = 0x%04x\n", hk_uv.fc_ham);
		fprintf(stdout, "rssi_tc = 0x%04x\n", hk_uv.rssi_tc);
		fprintf(stdout, "rssi_ham = 0x%04x\n", hk_uv.rssi_ham);
		fprintf(stdout, "cam_mode = 0x%02x\n", hk_uv.cam_mode);
		fprintf(stdout, "cam_task_flag = 0x%02x\n", hk_uv.cam_task_flag);
		fprintf(stdout, "cam_err_flag = 0x%02x\n", hk_uv.cam_err_flag);
		fprintf(stdout, "cam_pic_len = %d\n", hk_uv.cam_pic_len_2*2048+hk_uv.cam_pic_len_1*256+hk_uv.cam_pic_len_0);
		fprintf(stdout, "n_reset = %d\n", hk_uv.n_reset);
		fprintf(stdout, "flag_reset = 0x%02x\n", hk_uv.flag_reset);
		fprintf(stdout, "flag_sys = 0x%02x\n", hk_uv.flag_sys);
		fprintf(stdout, "n_dma_overflow = %d\n", hk_uv.n_dma_overflow);
		fprintf(stdout, "runtime = %f\n", ((float)sw32(hk_uv.runtime))*0.004f);
	}
    }

    void
    dslwp_tm_parser_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    dslwp_tm_parser_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      //const <+ITYPE+> *in = (const <+ITYPE+> *) input_items[0];
      //<+OTYPE+> *out = (<+OTYPE+> *) output_items[0];

      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace lilacsat */
} /* namespace gr */

