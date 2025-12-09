/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "afsk1200_rx_f_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lilacsat {

using input_type = float;
afsk1200_rx_f::sptr afsk1200_rx_f::make() {
    return gnuradio::make_block_sptr<afsk1200_rx_f_impl>();
}


/*
 * The private constructor
 */
afsk1200_rx_f_impl::afsk1200_rx_f_impl()
    : gr::sync_block("afsk1200_rx_f",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(0 /* min outputs */, 0 /*max outputs */, 0)) {
    d_out_port = pmt::mp("out");
    message_port_register_out(d_out_port);
    afsk1200_rx_f_impl::rx_init();
}

/*
 * Our virtual destructor.
 */
afsk1200_rx_f_impl::~afsk1200_rx_f_impl() {}

void afsk1200_rx_f_impl::AX25_MsgCallBack(void* obj_ptr, struct AX25Msg* msg) {
    afsk1200_rx_f_impl* obj_ptr_loc = (afsk1200_rx_f_impl*)obj_ptr;
    obj_ptr_loc->message_port_pub(
        obj_ptr_loc->d_out_port,
        pmt::cons(pmt::make_dict(), pmt::init_u8vector(msg->len, msg->info)));
}

void afsk1200_rx_f_impl::rx_init() {
    direwolf_afsk_init(&afsk);
    /*
     * Here we initialize AX25 context, the channel (KFile) we are going to read messages
     * from and the callback that will be called on incoming messages.
     */
    ax25_init(&ax25, &afsk.fd, (void*)this, &afsk1200_rx_f_impl::AX25_MsgCallBack);
}
int afsk1200_rx_f_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items) {
    auto in = static_cast<const input_type*>(input_items[0]);
    direwolf_afsk_rx_proc(&afsk, in, noutput_items);
    ax25_poll(&ax25);
                
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
