/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_SERIAL_SINK_PDU_H
#define INCLUDED_LILACSAT_SERIAL_SINK_PDU_H

#include <gnuradio/block.h>
#include <gnuradio/lilacsat/api.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API serial_sink_pdu : virtual public gr::block {
public:
    typedef std::shared_ptr<serial_sink_pdu> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::serial_sink_pdu.
     *
     * To avoid accidental use of raw pointers, lilacsat::serial_sink_pdu's
     * constructor is in a private implementation
     * class. lilacsat::serial_sink_pdu::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::string& path, unsigned int baudrate, bool debug);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_SERIAL_SINK_PDU_H */
