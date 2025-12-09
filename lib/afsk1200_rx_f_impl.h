/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_AFSK1200_RX_F_IMPL_H
#define INCLUDED_LILACSAT_AFSK1200_RX_F_IMPL_H

#include <gnuradio/lilacsat/afsk1200_rx_f.h>

extern "C"
{
	#include "afsk/afsk.h"
	#include "afsk/ax25.h"
}

namespace gr {
namespace lilacsat {

class afsk1200_rx_f_impl : public afsk1200_rx_f {
private:
    pmt::pmt_t d_out_port;
    Afsk afsk;
    AX25Ctx ax25;
    static void AX25_MsgCallBack(void *obj_ptr, struct AX25Msg *msg);
    void rx_init();

public:
    afsk1200_rx_f_impl();
    ~afsk1200_rx_f_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_AFSK1200_RX_F_IMPL_H */
