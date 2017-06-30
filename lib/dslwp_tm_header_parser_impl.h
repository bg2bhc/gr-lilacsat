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

#ifndef INCLUDED_LILACSAT_DSLWP_TM_HEADER_PARSER_IMPL_H
#define INCLUDED_LILACSAT_DSLWP_TM_HEADER_PARSER_IMPL_H

#include <lilacsat/dslwp_tm_header_parser.h>

namespace gr {
  namespace lilacsat {

    class dslwp_tm_header_parser_impl : public dslwp_tm_header_parser
    {
     private:
      pmt::pmt_t d_in_port;
      pmt::pmt_t d_out_port_0;
      pmt::pmt_t d_out_port_1;
      pmt::pmt_t d_out_port_2;

      void pmt_in_callback(pmt::pmt_t msg);

     public:
      dslwp_tm_header_parser_impl();
      ~dslwp_tm_header_parser_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_DSLWP_TM_HEADER_PARSER_IMPL_H */

