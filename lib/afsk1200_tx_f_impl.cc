/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "afsk1200_tx_f_impl.h"
#include <gnuradio/io_signature.h>

#define ERROR() std::cout << "Error!" << std::endl; // exit(0);

namespace gr {
namespace lilacsat {

using output_type = float;
afsk1200_tx_f::sptr
afsk1200_tx_f::make(const std::string& destination,
                    const std::string& source,
                    const std::string& repeater1,
                    const std::string& repeater2,
                    bool padding_zero,
                    int ptt_mode,
                    const std::vector<uint8_t> msg_ptt_on,
                    const std::vector<uint8_t> msg_ptt_off) {
    return gnuradio::make_block_sptr<afsk1200_tx_f_impl>(destination,
                                                         source,
                                                         repeater1,
                                                         repeater2,
                                                         padding_zero,
                                                         ptt_mode,
                                                         msg_ptt_on,
                                                         msg_ptt_off);
}


/*
 * The private constructor
 */
afsk1200_tx_f_impl::afsk1200_tx_f_impl(const std::string& destination,
                                       const std::string& source,
                                       const std::string& repeater1,
                                       const std::string& repeater2,
                                       bool padding_zero,
                                       int ptt_mode,
                                       const std::vector<uint8_t> msg_ptt_on,
                                       const std::vector<uint8_t> msg_ptt_off)
    : gr::sync_block("afsk1200_tx_f",
                     gr::io_signature::make(0 /* min inputs */, 0 /* max inputs */, 0),
                     gr::io_signature::make(
                         1 /* min outputs */, 1 /*max outputs */, sizeof(output_type))),
      d_destination(destination),
      d_source(source),
      d_repeater1(repeater1),
      d_repeater2(repeater2),
      d_ptt_mode(ptt_mode),
      d_msg_ptt_on(msg_ptt_on),
      d_msg_ptt_off(msg_ptt_off),
      d_ptt(0) {
    d_in_port = pmt::mp("in");
    d_ptt_port = pmt::mp("ptt");
    message_port_register_in(d_in_port);
    message_port_register_in(d_ptt_port);
    set_msg_handler(d_in_port, [this](pmt::pmt_t msg) { this->pmt_in_callback(msg); });

    afsk_init(&afsk);
    ax25_init(&ax25, &afsk.fd, (void*)0, 0);
    afsk.cfg_padding_zero = padding_zero;
}

/*
 * Our virtual destructor.
 */
afsk1200_tx_f_impl::~afsk1200_tx_f_impl() {}

int afsk1200_tx_f_impl::setcall(AX25Call* ax25call, std::string str) {
    int i;
    if (!str.size())
        return -1; // 长度为空
    for (i = 0; i < str.size(); i++) {
        if (i < 6 && (((str[i] >= '0') && (str[i] <= '9')) ||
                      ((str[i] >= 'A') && (str[i] <= 'Z')))) {
            ax25call->call[i] = str[i];

            if (i == (str.size() - 1)) {
                if (i < 5) {
                    for (int j = i + 1; j < 6; j++)
                        ax25call->call[j] = ' ';
                }
                ax25call->ssid = 0;
            }
        } else if ((str[i] == '-') && (i > 2)) {
            if (i < 6) {
                for (int j = i; j < 6; j++)
                    ax25call->call[j] = ' ';
            }

            if (str[str.size() - 1] == '*') {
                if (str.size() == i + 3) {
                    if ((str[i + 1] >= '0') && (str[i + 1] <= '9')) {
                        ax25call->ssid = str[i + 1];
                    } else {
                        return -2; // SSID数值过大
                    }
                } else if (str.size() == i + 4) {
                    if ((str[i + 1] == '1') && (str[i + 2] >= '0') &&
                        (str[i + 2] <= '5')) {
                        ax25call->ssid = 10 + str[i + 2];
                    } else {
                        return -2; // SSID数值过大
                    }
                } else {
                    return -3; // SSID字节数过多
                }
                ax25call->ssid = ax25call->ssid ^ 0x40;
            } else {
                if (str.size() == i + 2) {
                    if ((str[i + 1] >= '0') && (str[i + 1] <= '9')) {
                        ax25call->ssid = str[i + 1];
                    }
                } else if (str.size() == i + 3) {
                    if ((str[i + 1] == '1') && (str[i + 2] >= '0') &&
                        (str[i + 2] <= '5')) {
                        ax25call->ssid = 10 + str[i + 2];
                    } else {
                        return -2; // SSID数值过大
                    }
                } else {
                    return -3; // SSID字节数过多
                }
            }

            break;
        } else if ((str[i] == '*') && (i > 2) && (str.size() == (i + 1))) {
            if (i < 6) {
                for (int j = i; j < 6; j++)
                    ax25call->call[j] = ' ';
            }
            ax25call->ssid = 0x40;
        } else {
            return -4; // 非法字符或超长
            break;
        }
    }

    return 0;
}

void afsk1200_tx_f_impl::pmt_in_callback(pmt::pmt_t msg) {
    pmt::pmt_t meta(pmt::car(msg));
    pmt::pmt_t bytes(pmt::cdr(msg));

    size_t msg_len, n_path;
    const uint8_t* bytes_in = pmt::u8vector_elements(bytes, msg_len);

    // int i = countof(d_destination);
    // printf("text: %c\n", (*d_destination)[0]);
    // std::cout << d_destination.size() << std::endl;

    // std::cout << d_source.size() << std::endl;
    // std::cout << afsk1200_tx_f_impl::setcall(path+1, d_source) << std::endl;
    if (afsk1200_tx_f_impl::setcall(path, d_destination) ||
        afsk1200_tx_f_impl::setcall(path + 1, d_source))
        ERROR();
    if (afsk1200_tx_f_impl::setcall(path + 2, d_repeater1))
        n_path = 2;
    else if (afsk1200_tx_f_impl::setcall(path + 3, d_repeater2))
        n_path = 3;
    else
        n_path = 4;

    ax25_sendVia(&ax25, path, n_path, bytes_in, msg_len);
}


int afsk1200_tx_f_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items) {
    auto out = static_cast<output_type*>(output_items[0]);
    int n_ret = afsk_tx_proc(&afsk, out, noutput_items);
    pmt::pmt_t p_dict = pmt::make_dict();
    p_dict = pmt::dict_add(p_dict, pmt::mp("data"), pmt::from_double(0.0));
    if ((d_ptt == 0) && (n_ret != 0)) {
        if (d_ptt_mode == 0) {
            message_port_pub(
                d_ptt_port,
                pmt::cons(pmt::intern("data"),
                          pmt::init_u8vector(d_msg_ptt_off.size(), d_msg_ptt_on)));
        } else if (d_ptt_mode == 1) {
            message_port_pub(d_ptt_port,
                             pmt::cons(pmt::intern("set_rts"),
                                       pmt::init_u8vector(0, (const uint8_t*)0)));
        } else if (d_ptt_mode == 2) {
            message_port_pub(d_ptt_port,
                             pmt::cons(pmt::intern("set_dtr"),
                                       pmt::init_u8vector(0, (const uint8_t*)0)));
        }

        d_ptt = 1;
    } else if ((d_ptt != 0) && (n_ret == 0)) {
        if (d_ptt_mode == 0) //
        {
            message_port_pub(
                d_ptt_port,
                pmt::cons(pmt::intern("data"),
                          pmt::init_u8vector(d_msg_ptt_off.size(), d_msg_ptt_off)));
        } else if (d_ptt_mode == 1) {
            message_port_pub(d_ptt_port,
                             pmt::cons(pmt::intern("clear_rts"),
                                       pmt::init_u8vector(0, (const uint8_t*)0)));
        } else if (d_ptt_mode == 2) {
            message_port_pub(d_ptt_port,
                             pmt::cons(pmt::intern("clear_dtr"),
                                       pmt::init_u8vector(0, (const uint8_t*)0)));
        }

        d_ptt = 0;
    }

    if (n_ret == 0)
        usleep(1000);

    // Tell runtime system how many output items we produced.
    return n_ret;
}

} /* namespace lilacsat */
} /* namespace gr */
