/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_SATELLITE_CHANNEL_CC_H
#define INCLUDED_LILACSAT_SATELLITE_CHANNEL_CC_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API satellite_channel_cc : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<satellite_channel_cc> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::satellite_channel_cc.
     *
     * To avoid accidental use of raw pointers, lilacsat::satellite_channel_cc's
     * constructor is in a private implementation
     * class. lilacsat::satellite_channel_cc::make is the public interface for
     * creating new instances.
     */
    static sptr make(float r_semi_long_axis,
                     float alpha_aos,
                     float fc,
                     float samp_rate,
                     float d0,
                     bool doppler,
                     bool link_loss,
                     bool verbose);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_SATELLITE_CHANNEL_CC_H */
