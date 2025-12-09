/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_CC_DECODE_BB_H
#define INCLUDED_LILACSAT_CC_DECODE_BB_H

#include <gnuradio/block.h>
#include <gnuradio/lilacsat/api.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API cc_decode_bb : virtual public gr::block {
public:
    typedef std::shared_ptr<cc_decode_bb> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::cc_decode_bb.
     *
     * To avoid accidental use of raw pointers, lilacsat::cc_decode_bb's
     * constructor is in a private implementation
     * class. lilacsat::cc_decode_bb::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_CC_DECODE_BB_H */
