# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
install(FILES
    lilacsat_afsk1200_rx_f.xml
    lilacsat_afsk1200_tx_f.xml
    lilacsat_fec_decode_b.xml
    lilacsat_fec_encode_b.xml
    lilacsat_satellite_channel_cc.xml
    lilacsat_bpsk_demod_cb.xml
    lilacsat_kiss_decode_pdu.xml
    lilacsat_kiss_encode_pdu.xml
    lilacsat_vitfilt27_bb.xml
    lilacsat_encode27_bb.xml
    lilacsat_vitfilt27_fb.xml
    lilacsat_angle_cf.xml
    lilacsat_pipe_sink_pdu.xml
    lilacsat_serial_sink_pdu.xml
    lilacsat_plan13_cc.xml DESTINATION share/gnuradio/grc/blocks
)
