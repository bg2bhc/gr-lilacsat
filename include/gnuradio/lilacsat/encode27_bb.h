/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_ENCODE27_BB_H
#define INCLUDED_LILACSAT_ENCODE27_BB_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API encode27_bb : virtual public gr::sync_interpolator {
public:
    typedef std::shared_ptr<encode27_bb> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::encode27_bb.
     *
     * To avoid accidental use of raw pointers, lilacsat::encode27_bb's
     * constructor is in a private implementation
     * class. lilacsat::encode27_bb::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_ENCODE27_BB_H */
