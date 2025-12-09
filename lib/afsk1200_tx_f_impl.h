/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_AFSK1200_TX_F_IMPL_H
#define INCLUDED_LILACSAT_AFSK1200_TX_F_IMPL_H

#include <gnuradio/lilacsat/afsk1200_tx_f.h>

extern "C"
{
	#include "afsk/afsk.h"
	#include "afsk/ax25.h"
	#include "afsk/aprs.h"
}

namespace gr {
namespace lilacsat {

class afsk1200_tx_f_impl : public afsk1200_tx_f {
private:
    pmt::pmt_t d_in_port;
	pmt::pmt_t d_ptt_port;
	const std::string d_destination;
	const std::string d_source;
	const std::string d_repeater1;
	const std::string d_repeater2;
	int d_ptt_mode;
	const std::vector<uint8_t> d_msg_ptt_on;
	const std::vector<uint8_t> d_msg_ptt_off;
	Afsk afsk;
	AX25Ctx ax25;
	AX25Call path[4];
	char d_ptt;
	      
	void pmt_in_callback(pmt::pmt_t msg);
	int setcall(AX25Call *ax25call, std::string str);
	static void AX25_MsgCallBack(void *obj_ptr, struct AX25Msg *msg);

public:
      afsk1200_tx_f_impl(const std::string& destination, const std::string& source, const std::string& repeater1, const std::string& repeater2, bool padding_zero, int ptt_mode, const std::vector<uint8_t> msg_ptt_on, const std::vector<uint8_t> msg_ptt_off);
      ~afsk1200_tx_f_impl();

      // Where all the action really happens
      int work(int noutput_items,
               gr_vector_const_void_star& input_items,
               gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_AFSK1200_TX_F_IMPL_H */
