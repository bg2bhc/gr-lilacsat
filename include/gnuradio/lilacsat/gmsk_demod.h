/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_GMSK_DEMOD_H
#define INCLUDED_LILACSAT_GMSK_DEMOD_H

#include <gnuradio/block.h>
#include <gnuradio/lilacsat/api.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API gmsk_demod : virtual public gr::block {
public:
    typedef std::shared_ptr<gmsk_demod> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::gmsk_demod.
     *
     * To avoid accidental use of raw pointers, lilacsat::gmsk_demod's
     * constructor is in a private implementation
     * class. lilacsat::gmsk_demod::make is the public interface for
     * creating new instances.
     */
    static sptr
    make(float dtll_gain, int pd_N_avg, float pd_k1, float pd_k2, float vco_gain);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_GMSK_DEMOD_H */
