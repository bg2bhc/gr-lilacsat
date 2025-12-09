/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_CODEC2_ENCODE_FB_H
#define INCLUDED_LILACSAT_CODEC2_ENCODE_FB_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_decimator.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API codec2_encode_fb : virtual public gr::sync_decimator {
public:
    typedef std::shared_ptr<codec2_encode_fb> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::codec2_encode_fb.
     *
     * To avoid accidental use of raw pointers, lilacsat::codec2_encode_fb's
     * constructor is in a private implementation
     * class. lilacsat::codec2_encode_fb::make is the public interface for
     * creating new instances.
     */
    static sptr make(int mode);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_CODEC2_ENCODE_FB_H */
