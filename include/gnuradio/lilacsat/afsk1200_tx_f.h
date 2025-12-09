/* -*- c++ -*- */
/*
 * Copyright 2025 BG2BHC.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LILACSAT_AFSK1200_TX_F_H
#define INCLUDED_LILACSAT_AFSK1200_TX_F_H

#include <gnuradio/lilacsat/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace lilacsat {

    /*!
     * \brief <+description of block+>
     * \ingroup lilacsat
     *
     */
    class LILACSAT_API afsk1200_tx_f : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<afsk1200_tx_f> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lilacsat::afsk1200_tx_f.
       *
       * To avoid accidental use of raw pointers, lilacsat::afsk1200_tx_f's
       * constructor is in a private implementation
       * class. lilacsat::afsk1200_tx_f::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string& destination, const std::string& source, const std::string& repeater1, const std::string& repeater2, bool padding_zero, int ptt_mode, const std::vector<uint8_t> msg_ptt_on, const std::vector<uint8_t> msg_ptt_off);
    };

  } // namespace lilacsat
} // namespace gr

#endif /* INCLUDED_LILACSAT_AFSK1200_TX_F_H */
