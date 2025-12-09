/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_DSLWP_TM_HEADER_PARSER_IMPL_H
#define INCLUDED_LILACSAT_DSLWP_TM_HEADER_PARSER_IMPL_H

#include <gnuradio/lilacsat/dslwp_tm_header_parser.h>

namespace gr {
namespace lilacsat {

class dslwp_tm_header_parser_impl : public dslwp_tm_header_parser {
private:
    pmt::pmt_t d_in_port;
    pmt::pmt_t d_out_port_0;
    pmt::pmt_t d_out_port_1;
    pmt::pmt_t d_out_port_2;

    void pmt_in_callback(pmt::pmt_t msg);
public:
    dslwp_tm_header_parser_impl();
    ~dslwp_tm_header_parser_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_DSLWP_TM_HEADER_PARSER_IMPL_H */
