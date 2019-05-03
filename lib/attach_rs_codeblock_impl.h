/* -*- c++ -*- */
/* 
 * Copyright 2019 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_LILACSAT_ATTACH_RS_CODEBLOCK_IMPL_H
#define INCLUDED_LILACSAT_ATTACH_RS_CODEBLOCK_IMPL_H

#include <lilacsat/attach_rs_codeblock.h>

namespace gr {
  namespace lilacsat {

    class attach_rs_codeblock_impl : public attach_rs_codeblock
    {
     private:
      pmt::pmt_t d_in_port;
      pmt::pmt_t d_out_port;

      void pmt_in_callback(pmt::pmt_t msg);

     public:
      attach_rs_codeblock_impl(int interleaving);
      ~attach_rs_codeblock_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_ATTACH_RS_CODEBLOCK_IMPL_H */

