/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_SERIAL_SINK_PDU_IMPL_H
#define INCLUDED_LILACSAT_SERIAL_SINK_PDU_IMPL_H

#include <gnuradio/lilacsat/serial_sink_pdu.h>

namespace gr {
namespace lilacsat {

class serial_sink_pdu_impl : public serial_sink_pdu {
private:
	std::string d_path;      
	bool d_debug;
	int d_baudrate;
	pmt::pmt_t d_in_port;
	int fd;

	void pmt_in_callback(pmt::pmt_t msg);
public:
    serial_sink_pdu_impl(const std::string& path, unsigned int baudrate, bool debug);
    ~serial_sink_pdu_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_SERIAL_SINK_PDU_IMPL_H */
