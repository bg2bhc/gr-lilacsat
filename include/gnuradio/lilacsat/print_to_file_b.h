/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_PRINT_TO_FILE_B_H
#define INCLUDED_LILACSAT_PRINT_TO_FILE_B_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace lilacsat {

/*!
 * \brief <+description of block+>
 * \ingroup lilacsat
 *
 */
class LILACSAT_API print_to_file_b : virtual public gr::sync_block {
public:
    typedef std::shared_ptr<print_to_file_b> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of lilacsat::print_to_file_b.
     *
     * To avoid accidental use of raw pointers, lilacsat::print_to_file_b's
     * constructor is in a private implementation
     * class. lilacsat::print_to_file_b::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::string& path, int16_t format);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_PRINT_TO_FILE_B_H */
