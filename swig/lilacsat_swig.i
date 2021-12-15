/* -*- c++ -*- */

#define LILACSAT_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "lilacsat_swig_doc.i"

%{
#include "lilacsat/afsk1200_rx_f.h"
#include "lilacsat/afsk1200_tx_f.h"
#include "lilacsat/angle_cf.h"
#include "lilacsat/attach_preamble_and_tailer.h"
#include "lilacsat/attach_rs_codeblock.h"
#include "lilacsat/bpsk_demod_cb.h"
#include "lilacsat/cc_decode_bb.h"
#include "lilacsat/codec2_decode_bf.h"
#include "lilacsat/codec2_encode_fb.h"
#include "lilacsat/dslwp_tm_header_parser.h"
#include "lilacsat/dslwp_tm_parser.h"
#include "lilacsat/encode27_bb.h"
#include "lilacsat/fec_decode_b.h"
#include "lilacsat/fec_encode_b.h"
#include "lilacsat/file_sink_pdu.h"
#include "lilacsat/gmsk_demod.h"
#include "lilacsat/kiss_decode_pdu.h"
#include "lilacsat/kiss_encode_pdu.h"
#include "lilacsat/lilacsat1_frame_depack.h"
#include "lilacsat/pdu_f32_to_u8.h"
#include "lilacsat/pipe_sink_pdu.h"
#include "lilacsat/plan13_cc.h"
#include "lilacsat/print_to_file_b.h"
#include "lilacsat/rs_decode_pdu.h"
#include "lilacsat/satellite_channel_cc.h"
#include "lilacsat/serial_sink_pdu.h"
#include "lilacsat/sync_det_b.h"
#include "lilacsat/two_bit_dpd_frame_recovery.h"
#include "lilacsat/two_bit_dpd_precoder.h"
#include "lilacsat/vitfilt27_bb.h"
#include "lilacsat/vitfilt27_fb.h"
#include "lilacsat/ccsds_ssdv_encode.h"
#include "lilacsat/ccsds_ssdv_decode.h"
%}

%include "lilacsat/afsk1200_rx_f.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, afsk1200_rx_f);
%include "lilacsat/afsk1200_tx_f.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, afsk1200_tx_f);
%include "lilacsat/angle_cf.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, angle_cf);
%include "lilacsat/attach_preamble_and_tailer.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, attach_preamble_and_tailer);
%include "lilacsat/attach_rs_codeblock.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, attach_rs_codeblock);
%include "lilacsat/bpsk_demod_cb.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, bpsk_demod_cb);

%include "lilacsat/cc_decode_bb.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, cc_decode_bb);
%include "lilacsat/codec2_decode_bf.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, codec2_decode_bf);
%include "lilacsat/codec2_encode_fb.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, codec2_encode_fb);
%include "lilacsat/dslwp_tm_header_parser.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, dslwp_tm_header_parser);
%include "lilacsat/dslwp_tm_parser.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, dslwp_tm_parser);
%include "lilacsat/encode27_bb.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, encode27_bb);
%include "lilacsat/fec_decode_b.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, fec_decode_b);
%include "lilacsat/fec_encode_b.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, fec_encode_b);

%include "lilacsat/file_sink_pdu.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, file_sink_pdu);
%include "lilacsat/gmsk_demod.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, gmsk_demod);
%include "lilacsat/kiss_decode_pdu.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, kiss_decode_pdu);
%include "lilacsat/kiss_encode_pdu.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, kiss_encode_pdu);
%include "lilacsat/lilacsat1_frame_depack.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, lilacsat1_frame_depack);
%include "lilacsat/pdu_f32_to_u8.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, pdu_f32_to_u8);
%include "lilacsat/pipe_sink_pdu.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, pipe_sink_pdu);
%include "lilacsat/plan13_cc.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, plan13_cc);
%include "lilacsat/print_to_file_b.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, print_to_file_b);
%include "lilacsat/rs_decode_pdu.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, rs_decode_pdu);
%include "lilacsat/satellite_channel_cc.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, satellite_channel_cc);
%include "lilacsat/serial_sink_pdu.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, serial_sink_pdu);
%include "lilacsat/sync_det_b.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, sync_det_b);
%include "lilacsat/two_bit_dpd_frame_recovery.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, two_bit_dpd_frame_recovery);
%include "lilacsat/two_bit_dpd_precoder.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, two_bit_dpd_precoder);
%include "lilacsat/vitfilt27_bb.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, vitfilt27_bb);
%include "lilacsat/vitfilt27_fb.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, vitfilt27_fb);

%include "lilacsat/ccsds_ssdv_encode.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, ccsds_ssdv_encode);
%include "lilacsat/ccsds_ssdv_decode.h"
GR_SWIG_BLOCK_MAGIC2(lilacsat, ccsds_ssdv_decode);
