/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_BPSK_DEMOD_CB_H
#define INCLUDED_LILACSAT_BPSK_DEMOD_CB_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API bpsk_demod_cb : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<bpsk_demod_cb> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::bpsk_demod_cb.
     *
     * To avoid accidental use of raw pointers, lilacsat::bpsk_demod_cb's
     * constructor is in a private implementation
     * class. lilacsat::bpsk_demod_cb::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_BPSK_DEMOD_CB_H */
