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


#ifndef INCLUDED_LILACSAT_FILE_SINK_PDU_H
#define INCLUDED_LILACSAT_FILE_SINK_PDU_H

#include <lilacsat/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace lilacsat {

    /*!
     * \brief <+description of block+>
     * \ingroup lilacsat
     *
     */
    class LILACSAT_API file_sink_pdu : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<file_sink_pdu> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lilacsat::file_sink_pdu.
       *
       * To avoid accidental use of raw pointers, lilacsat::file_sink_pdu's
       * constructor is in a private implementation
       * class. lilacsat::file_sink_pdu::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string& path, int16_t format);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_FILE_SINK_PDU_H */

