/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "print_to_file_b_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace lilacsat {

using input_type = char;
print_to_file_b::sptr print_to_file_b::make(const std::string& path, int16_t format) {
    return gnuradio::make_block_sptr<print_to_file_b_impl>(path, format);
}


/*
 * The private constructor
 */
print_to_file_b_impl::print_to_file_b_impl(const std::string& path, int16_t format)
    : gr::sync_block("print_to_file_b",
                     gr::io_signature::make(
                         1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                     gr::io_signature::make(
                         0 /* min outputs */, 0 /*max outputs */, 0)) {
	fp = fopen(path.data(), "rw+");
}

/*
 * Our virtual destructor.
 */
print_to_file_b_impl::~print_to_file_b_impl() {
	fclose(fp);
}


int print_to_file_b_impl::work(int noutput_items,
                               gr_vector_const_void_star& input_items,
                               gr_vector_void_star& output_items) {
    auto in = static_cast<const input_type*>(input_items[0]);
	int i;
    // Do <+signal processing+>
	for(i=0; i<noutput_items; i++)
	{
		fprintf(fp, "%02x ", in[i] & 0xff);
	}

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace lilacsat */
} /* namespace gr */
