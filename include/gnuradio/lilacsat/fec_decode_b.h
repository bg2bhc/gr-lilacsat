/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_FEC_DECODE_B_H
#define INCLUDED_LILACSAT_FEC_DECODE_B_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API fec_decode_b : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<fec_decode_b> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::fec_decode_b.
     *
     * To avoid accidental use of raw pointers, lilacsat::fec_decode_b's
     * constructor is in a private implementation
     * class. lilacsat::fec_decode_b::make is the public interface for
     * creating new instances.
     */
    static sptr
    make(int frame_len, bool using_m, bool using_convolutional_code, bool pass_all);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_FEC_DECODE_B_H */
