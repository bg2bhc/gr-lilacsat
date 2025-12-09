/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_KISS_ENCODE_PDU_IMPL_H
#define INCLUDED_LILACSAT_KISS_ENCODE_PDU_IMPL_H

#include <gnuradio/lilacsat/kiss_encode_pdu.h>

extern "C"
{
    #include "kiss/kiss.h"
}

namespace gr {
namespace lilacsat {

class kiss_encode_pdu_impl : public kiss_encode_pdu {
private:
      int d_const_length;
      pmt::pmt_t d_in_port;
      pmt::pmt_t d_out_port;

      void pmt_in_callback(pmt::pmt_t msg);
public:
    kiss_encode_pdu_impl(int const_length);
    ~kiss_encode_pdu_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_KISS_ENCODE_PDU_IMPL_H */
