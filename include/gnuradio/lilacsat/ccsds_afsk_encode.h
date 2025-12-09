/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_CCSDS_AFSK_ENCODE_H
#define INCLUDED_LILACSAT_CCSDS_AFSK_ENCODE_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API ccsds_afsk_encode : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<ccsds_afsk_encode> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::ccsds_afsk_encode.
     *
     * To avoid accidental use of raw pointers, lilacsat::ccsds_afsk_encode's
     * constructor is in a private implementation
     * class. lilacsat::ccsds_afsk_encode::make is the public interface for
     * creating new instances.
     */
    static sptr make(int bitrate,
                     int frame_len,
                     int preamble_len,
                     int trailer_len,
                     bool continous,
                     bool padding_zero,
                     bool using_m,
                     bool using_convolutional_code);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_CCSDS_AFSK_ENCODE_H */
