/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_RS_DECODE_PDU_H
#define INCLUDED_LILACSAT_RS_DECODE_PDU_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API rs_decode_pdu : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<rs_decode_pdu> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::rs_decode_pdu.
     *
     * To avoid accidental use of raw pointers, lilacsat::rs_decode_pdu's
     * constructor is in a private implementation
     * class. lilacsat::rs_decode_pdu::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_RS_DECODE_PDU_H */
