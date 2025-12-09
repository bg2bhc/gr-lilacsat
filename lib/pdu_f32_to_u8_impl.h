/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_PDU_F32_TO_U8_IMPL_H
#define INCLUDED_LILACSAT_PDU_F32_TO_U8_IMPL_H

#include <gnuradio/lilacsat/pdu_f32_to_u8.h>

namespace gr {
namespace lilacsat {

class pdu_f32_to_u8_impl : public pdu_f32_to_u8 {
private:
      pmt::pmt_t d_in_port;
      pmt::pmt_t d_out_port;
      void pmt_in_callback(pmt::pmt_t msg);

public:
    pdu_f32_to_u8_impl();
    ~pdu_f32_to_u8_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_PDU_F32_TO_U8_IMPL_H */
