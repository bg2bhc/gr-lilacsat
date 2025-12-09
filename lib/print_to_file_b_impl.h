/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_PRINT_TO_FILE_B_IMPL_H
#define INCLUDED_LILACSAT_PRINT_TO_FILE_B_IMPL_H

#include <gnuradio/lilacsat/print_to_file_b.h>

#include <stdio.h>

namespace gr {
namespace lilacsat {

class print_to_file_b_impl : public print_to_file_b {
private:
      FILE *fp;
      int16_t format;

public:
    print_to_file_b_impl(const std::string& path, int16_t format);
    ~print_to_file_b_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_PRINT_TO_FILE_B_IMPL_H */
