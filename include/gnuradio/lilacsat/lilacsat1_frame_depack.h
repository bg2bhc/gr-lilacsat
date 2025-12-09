/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_LILACSAT1_FRAME_DEPACK_H
#define INCLUDED_LILACSAT_LILACSAT1_FRAME_DEPACK_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API lilacsat1_frame_depack : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<lilacsat1_frame_depack> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::lilacsat1_frame_depack.
     *
     * To avoid accidental use of raw pointers, lilacsat::lilacsat1_frame_depack's
     * constructor is in a private implementation
     * class. lilacsat::lilacsat1_frame_depack::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_LILACSAT1_FRAME_DEPACK_H */
