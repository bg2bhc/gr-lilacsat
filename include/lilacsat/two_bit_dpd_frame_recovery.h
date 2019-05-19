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


#ifndef INCLUDED_LILACSAT_TWO_BIT_DPD_FRAME_RECOVERY_H
#define INCLUDED_LILACSAT_TWO_BIT_DPD_FRAME_RECOVERY_H

#include <lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace lilacsat {

    /*!
     * \brief <+description of block+>
     * \ingroup lilacsat
     *
     */
    class LILACSAT_API two_bit_dpd_frame_recovery : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<two_bit_dpd_frame_recovery> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lilacsat::two_bit_dpd_frame_recovery.
       *
       * To avoid accidental use of raw pointers, lilacsat::two_bit_dpd_frame_recovery's
       * constructor is in a private implementation
       * class. lilacsat::two_bit_dpd_frame_recovery::make is the public interface for
       * creating new instances.
       */
      static sptr make(float snr_threshold);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_TWO_BIT_DPD_FRAME_RECOVERY_H */

