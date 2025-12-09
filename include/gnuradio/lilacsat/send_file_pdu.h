/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_SEND_FILE_PDU_H
#define INCLUDED_LILACSAT_SEND_FILE_PDU_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API send_file_pdu : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<send_file_pdu> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::send_file_pdu.
     *
     * To avoid accidental use of raw pointers, lilacsat::send_file_pdu's
     * constructor is in a private implementation
     * class. lilacsat::send_file_pdu::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::string& path, int pdu_len, int period, int terminal);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_SEND_FILE_PDU_H */
