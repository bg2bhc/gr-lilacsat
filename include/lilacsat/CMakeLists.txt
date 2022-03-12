# Copyright 2011,2012 Free Software Foundation, Inc.
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

########################################################################
# Install public header files
########################################################################
install(FILES
    api.h
    afsk1200_rx_f.h
    afsk1200_tx_f.h
    fec_decode_b.h
    fec_encode_b.h
    satellite_channel_cc.h
    bpsk_demod_cb.h
    kiss_decode_pdu.h
    kiss_encode_pdu.h
    vitfilt27_bb.h
    encode27_bb.h
    vitfilt27_fb.h
    angle_cf.h
    pipe_sink_pdu.h
    serial_sink_pdu.h
    plan13_cc.h
    print_to_file_b.h
    codec2_decode_bf.h
    codec2_encode_fb.h
    cc_decode_bb.h
    sync_det_b.h
    sync_det_b.h
    lilacsat1_frame_depack.h
    gmsk_demod.h
    dslwp_tm_header_parser.h
    file_sink_pdu.h
    dslwp_tm_parser.h
    two_bit_dpd_precoder.h
    attach_preamble_and_tailer.h
    attach_rs_codeblock.h
    pdu_f32_to_u8.h
    two_bit_dpd_frame_recovery.h
    rs_decode_pdu.h
    ccsds_ssdv_decode.h
    ccsds_ssdv_encode.h DESTINATION include/lilacsat
)
