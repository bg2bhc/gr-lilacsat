/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_PLAN13_CC_H
#define INCLUDED_LILACSAT_PLAN13_CC_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API plan13_cc : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<plan13_cc> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::plan13_cc.
     *
     * To avoid accidental use of raw pointers, lilacsat::plan13_cc's
     * constructor is in a private implementation
     * class. lilacsat::plan13_cc::make is the public interface for
     * creating new instances.
     */
    static sptr make(bool enable,
                     const std::string& tle1,
                     const std::string& tle2,
                     float lon,
                     float lat,
                     float height,
                     float fc,
                     float samp_rate,
                     bool txrx,
                     bool verbose);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_PLAN13_CC_H */
