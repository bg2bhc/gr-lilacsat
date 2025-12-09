/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_CODEC2_DECODE_BF_H
#define INCLUDED_LILACSAT_CODEC2_DECODE_BF_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API codec2_decode_bf : virtual public gr::sync_interpolator {
public:
    typedef std::shared_ptr<codec2_decode_bf> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::codec2_decode_bf.
     *
     * To avoid accidental use of raw pointers, lilacsat::codec2_decode_bf's
     * constructor is in a private implementation
     * class. lilacsat::codec2_decode_bf::make is the public interface for
     * creating new instances.
     */
    static sptr make(int mode);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_CODEC2_DECODE_BF_H */
