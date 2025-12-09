/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_TWO_BIT_DPD_FRAME_RECOVERY_H
#define INCLUDED_LILACSAT_TWO_BIT_DPD_FRAME_RECOVERY_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API two_bit_dpd_frame_recovery : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<two_bit_dpd_frame_recovery> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of
     * lilacsat::two_bit_dpd_frame_recovery.
     *
     * To avoid accidental use of raw pointers, lilacsat::two_bit_dpd_frame_recovery's
     * constructor is in a private implementation
     * class. lilacsat::two_bit_dpd_frame_recovery::make is the public interface for
     * creating new instances.
     */
    static sptr make(float snr_threshold);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_TWO_BIT_DPD_FRAME_RECOVERY_H */
