/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_ATTACH_PREAMBLE_AND_TAILER_H
#define INCLUDED_LILACSAT_ATTACH_PREAMBLE_AND_TAILER_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API attach_preamble_and_tailer : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<attach_preamble_and_tailer> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of
     * lilacsat::attach_preamble_and_tailer.
     *
     * To avoid accidental use of raw pointers, lilacsat::attach_preamble_and_tailer's
     * constructor is in a private implementation
     * class. lilacsat::attach_preamble_and_tailer::make is the public interface for
     * creating new instances.
     */
    static sptr make(int len_preamble, int len_tailer, bool padding_type);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_ATTACH_PREAMBLE_AND_TAILER_H */
