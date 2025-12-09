/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_FEC_ENCODE_B_H
#define INCLUDED_LILACSAT_FEC_ENCODE_B_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API fec_encode_b : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<fec_encode_b> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::fec_encode_b.
     *
     * To avoid accidental use of raw pointers, lilacsat::fec_encode_b's
     * constructor is in a private implementation
     * class. lilacsat::fec_encode_b::make is the public interface for
     * creating new instances.
     */
    static sptr make(int frame_len,
                     int preamble_len,
                     int trailer_len,
                     bool continous,
                     bool padding_zero,
                     bool using_m,
                     bool using_convolutional_code);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_FEC_ENCODE_B_H */
