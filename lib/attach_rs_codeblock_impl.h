/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_ATTACH_RS_CODEBLOCK_IMPL_H
#define INCLUDED_LILACSAT_ATTACH_RS_CODEBLOCK_IMPL_H

#include <gnuradio/lilacsat/attach_rs_codeblock.h>

namespace gr {
namespace lilacsat {

class attach_rs_codeblock_impl : public attach_rs_codeblock {
private:
    pmt::pmt_t d_in_port;
    pmt::pmt_t d_out_port;

    void pmt_in_callback(pmt::pmt_t msg);

public:
    attach_rs_codeblock_impl(int interleaving);
    ~attach_rs_codeblock_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_ATTACH_RS_CODEBLOCK_IMPL_H */
