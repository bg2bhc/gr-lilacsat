/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_ANGLE_CF_H
#define INCLUDED_LILACSAT_ANGLE_CF_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API angle_cf : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<angle_cf> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::angle_cf.
     *
     * To avoid accidental use of raw pointers, lilacsat::angle_cf's
     * constructor is in a private implementation
     * class. lilacsat::angle_cf::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_ANGLE_CF_H */
