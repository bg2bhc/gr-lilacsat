/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_AFSK1200_RX_F_H
#define INCLUDED_LILACSAT_AFSK1200_RX_F_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API afsk1200_rx_f : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<afsk1200_rx_f> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::afsk1200_rx_f.
     *
     * To avoid accidental use of raw pointers, lilacsat::afsk1200_rx_f's
     * constructor is in a private implementation
     * class. lilacsat::afsk1200_rx_f::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_AFSK1200_RX_F_H */
