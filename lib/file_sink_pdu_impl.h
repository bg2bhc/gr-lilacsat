/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_LILACSAT_FILE_SINK_PDU_IMPL_H
#define INCLUDED_LILACSAT_FILE_SINK_PDU_IMPL_H

#include <lilacsat/file_sink_pdu.h>

#include <stdio.h>

namespace gr {
  namespace lilacsat {

    class file_sink_pdu_impl : public file_sink_pdu
    {
     private:
      FILE *fp;
      int16_t format;
      pmt::pmt_t d_in_port;
      void pmt_in_callback(pmt::pmt_t msg);

     public:
      file_sink_pdu_impl(const std::string& path, int16_t format);
      ~file_sink_pdu_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_FILE_SINK_PDU_IMPL_H */

