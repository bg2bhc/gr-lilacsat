/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_TWO_BIT_DPD_PRECODER_H
#define INCLUDED_LILACSAT_TWO_BIT_DPD_PRECODER_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API two_bit_dpd_precoder : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<two_bit_dpd_precoder> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::two_bit_dpd_precoder.
     *
     * To avoid accidental use of raw pointers, lilacsat::two_bit_dpd_precoder's
     * constructor is in a private implementation
     * class. lilacsat::two_bit_dpd_precoder::make is the public interface for
     * creating new instances.
     */
    static sptr make(unsigned char init_value);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_TWO_BIT_DPD_PRECODER_H */
