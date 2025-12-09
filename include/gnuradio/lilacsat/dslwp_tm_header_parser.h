/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_DSLWP_TM_HEADER_PARSER_H
#define INCLUDED_LILACSAT_DSLWP_TM_HEADER_PARSER_H

#include <gnuradio/block.h>
#include <gnuradio/lilacsat/api.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API dslwp_tm_header_parser : virtual public gr::block {
public:
    typedef std::shared_ptr<dslwp_tm_header_parser> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::dslwp_tm_header_parser.
     *
     * To avoid accidental use of raw pointers, lilacsat::dslwp_tm_header_parser's
     * constructor is in a private implementation
     * class. lilacsat::dslwp_tm_header_parser::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_DSLWP_TM_HEADER_PARSER_H */
