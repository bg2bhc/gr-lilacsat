/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_PIPE_SINK_PDU_IMPL_H
#define INCLUDED_LILACSAT_PIPE_SINK_PDU_IMPL_H

#include <gnuradio/lilacsat/pipe_sink_pdu.h>

namespace gr {
namespace lilacsat {

class pipe_sink_pdu_impl : public pipe_sink_pdu {
private:
    pmt::pmt_t d_in_port;
    const std::string d_label;
    const std::string d_path;
    bool d_debug;
    void pmt_in_callback(pmt::pmt_t msg);

public:
    pipe_sink_pdu_impl(const std::string& label,
                       const std::string& path,
                       uint16_t permission,
                       bool debug);
    ~pipe_sink_pdu_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_PIPE_SINK_PDU_IMPL_H */
