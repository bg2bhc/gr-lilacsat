/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_FILE_SINK_PDU_IMPL_H
#define INCLUDED_LILACSAT_FILE_SINK_PDU_IMPL_H

#include <gnuradio/lilacsat/file_sink_pdu.h>

namespace gr {
namespace lilacsat {

class file_sink_pdu_impl : public file_sink_pdu {
private:
    FILE *fp;
    int16_t format;
    pmt::pmt_t d_in_port;
    void pmt_in_callback(pmt::pmt_t msg);

public:
    file_sink_pdu_impl(const std::string& path, int16_t format);
    ~file_sink_pdu_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_FILE_SINK_PDU_IMPL_H */
