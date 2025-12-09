/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_SEND_FILE_PDU_IMPL_H
#define INCLUDED_LILACSAT_SEND_FILE_PDU_IMPL_H

#include <gnuradio/lilacsat/send_file_pdu.h>

#include <stdio.h>

namespace gr {
namespace lilacsat {

class send_file_pdu_impl : public send_file_pdu {
private:
      pmt::pmt_t d_out_port;
      FILE *d_fp;
      int d_pdu_len;
      int d_period;
      int d_terminal;
      void run();
      gr::thread::thread d_thread;

public:
    send_file_pdu_impl(const std::string& path, int pdu_len, int period, int terminal);
    ~send_file_pdu_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_SEND_FILE_PDU_IMPL_H */
