/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_PDU_F32_TO_U8_H
#define INCLUDED_LILACSAT_PDU_F32_TO_U8_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API pdu_f32_to_u8 : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<pdu_f32_to_u8> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::pdu_f32_to_u8.
     *
     * To avoid accidental use of raw pointers, lilacsat::pdu_f32_to_u8's
     * constructor is in a private implementation
     * class. lilacsat::pdu_f32_to_u8::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_PDU_F32_TO_U8_H */
