/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_RS_DECODE_PDU_IMPL_H
#define INCLUDED_LILACSAT_RS_DECODE_PDU_IMPL_H

#include <gnuradio/lilacsat/rs_decode_pdu.h>

namespace gr {
namespace lilacsat {

class rs_decode_pdu_impl : public rs_decode_pdu {
private:
      pmt::pmt_t d_in_port;
      pmt::pmt_t d_out_port;

      void pmt_in_callback(pmt::pmt_t msg);
public:
    rs_decode_pdu_impl();
    ~rs_decode_pdu_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_RS_DECODE_PDU_IMPL_H */
