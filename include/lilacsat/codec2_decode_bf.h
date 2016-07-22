/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_LILACSAT_CODEC2_DECODE_BF_H
#define INCLUDED_LILACSAT_CODEC2_DECODE_BF_H

#include <lilacsat/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
  namespace lilacsat {

    /*!
     * \brief <+description of block+>
     * \ingroup lilacsat
     *
     */
    class LILACSAT_API codec2_decode_bf : virtual public gr::sync_interpolator
    {
     public:
      typedef boost::shared_ptr<codec2_decode_bf> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lilacsat::codec2_decode_bf.
       *
       * To avoid accidental use of raw pointers, lilacsat::codec2_decode_bf's
       * constructor is in a private implementation
       * class. lilacsat::codec2_decode_bf::make is the public interface for
       * creating new instances.
       */
      static sptr make(int mode);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_CODEC2_DECODE_BF_H */

