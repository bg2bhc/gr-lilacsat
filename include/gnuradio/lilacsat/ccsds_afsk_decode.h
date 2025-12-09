/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_CCSDS_AFSK_DECODE_H
#define INCLUDED_LILACSAT_CCSDS_AFSK_DECODE_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API ccsds_afsk_decode : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<ccsds_afsk_decode> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::ccsds_afsk_decode.
     *
     * To avoid accidental use of raw pointers, lilacsat::ccsds_afsk_decode's
     * constructor is in a private implementation
     * class. lilacsat::ccsds_afsk_decode::make is the public interface for
     * creating new instances.
     */
    static sptr make(int bitrate,
                     int frame_len,
                     bool using_m,
                     bool using_convolutional_code,
                     bool pass_all);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_CCSDS_AFSK_DECODE_H */
