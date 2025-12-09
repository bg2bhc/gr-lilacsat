/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_VITFILT27_FB_H
#define INCLUDED_LILACSAT_VITFILT27_FB_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_decimator.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API vitfilt27_fb : virtual public gr::sync_decimator {
public:
    typedef std::shared_ptr<vitfilt27_fb> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::vitfilt27_fb.
     *
     * To avoid accidental use of raw pointers, lilacsat::vitfilt27_fb's
     * constructor is in a private implementation
     * class. lilacsat::vitfilt27_fb::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_VITFILT27_FB_H */
