/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_ATTACH_PREAMBLE_AND_TAILER_IMPL_H
#define INCLUDED_LILACSAT_ATTACH_PREAMBLE_AND_TAILER_IMPL_H

#include <gnuradio/lilacsat/attach_preamble_and_tailer.h>

namespace gr {
namespace lilacsat {

class attach_preamble_and_tailer_impl : public attach_preamble_and_tailer {
private:
      unsigned char d_len_preamble;
      unsigned char d_len_tailer;
      unsigned char d_padding_type;
      pmt::pmt_t d_in_port;
      pmt::pmt_t d_out_port;

      void pmt_in_callback(pmt::pmt_t msg);
public:
    attach_preamble_and_tailer_impl(int len_preamble, int len_tailer, bool padding_type);
    ~attach_preamble_and_tailer_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_ATTACH_PREAMBLE_AND_TAILER_IMPL_H */
