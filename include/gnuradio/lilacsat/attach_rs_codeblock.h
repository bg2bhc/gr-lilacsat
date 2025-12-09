/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_ATTACH_RS_CODEBLOCK_H
#define INCLUDED_LILACSAT_ATTACH_RS_CODEBLOCK_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API attach_rs_codeblock : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<attach_rs_codeblock> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::attach_rs_codeblock.
     *
     * To avoid accidental use of raw pointers, lilacsat::attach_rs_codeblock's
     * constructor is in a private implementation
     * class. lilacsat::attach_rs_codeblock::make is the public interface for
     * creating new instances.
     */
    static sptr make(int interleaving);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_ATTACH_RS_CODEBLOCK_H */
