/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_SYNC_DET_B_H
#define INCLUDED_LILACSAT_SYNC_DET_B_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API sync_det_b : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<sync_det_b> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::sync_det_b.
     *
     * To avoid accidental use of raw pointers, lilacsat::sync_det_b's
     * constructor is in a private implementation
     * class. lilacsat::sync_det_b::make is the public interface for
     * creating new instances.
     */
    static sptr make(uint32_t sync_word, uint32_t len, uint8_t nrz, uint8_t descrambling);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_SYNC_DET_B_H */
