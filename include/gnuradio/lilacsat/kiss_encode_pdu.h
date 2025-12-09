/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_KISS_ENCODE_PDU_H
#define INCLUDED_LILACSAT_KISS_ENCODE_PDU_H

#include <gnuradio/block.h>
#include <gnuradio/lilacsat/api.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API kiss_encode_pdu : virtual public gr::block {
public:
    typedef std::shared_ptr<kiss_encode_pdu> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::kiss_encode_pdu.
     *
     * To avoid accidental use of raw pointers, lilacsat::kiss_encode_pdu's
     * constructor is in a private implementation
     * class. lilacsat::kiss_encode_pdu::make is the public interface for
     * creating new instances.
     */
    static sptr make(int const_length);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_KISS_ENCODE_PDU_H */
