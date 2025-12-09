/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_FILE_SINK_PDU_H
#define INCLUDED_LILACSAT_FILE_SINK_PDU_H

#include <gnuradio/block.h>
#include <gnuradio/lilacsat/api.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API file_sink_pdu : virtual public gr::block {
public:
    typedef std::shared_ptr<file_sink_pdu> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::file_sink_pdu.
     *
     * To avoid accidental use of raw pointers, lilacsat::file_sink_pdu's
     * constructor is in a private implementation
     * class. lilacsat::file_sink_pdu::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::string& path, int16_t format);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_FILE_SINK_PDU_H */
