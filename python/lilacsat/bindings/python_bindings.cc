/*
 * Copyright 2020 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <pybind11/pybind11.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

namespace py = pybind11;

// Headers for binding functions
/**************************************/
// The following comment block is used for
// gr_modtool to insert function prototypes
// Please do not delete
/**************************************/
// BINDING_FUNCTION_PROTOTYPES(
    void bind_afsk1200_tx_f(py::module& m);
    void bind_afsk1200_rx_f(py::module& m);
    void bind_angle_cf(py::module& m);
    void bind_attach_preamble_and_tailer(py::module& m);
    void bind_attach_rs_codeblock(py::module& m);
    void bind_bpsk_demod_cb(py::module& m);
    void bind_cc_decode_bb(py::module& m);
    void bind_ccsds_afsk_decode(py::module& m);
    void bind_ccsds_afsk_encode(py::module& m);
    void bind_codec2_decode_bf(py::module& m);
    void bind_codec2_encode_fb(py::module& m);
    void bind_dslwp_tm_header_parser(py::module& m);
    void bind_dslwp_tm_parser(py::module& m);
    void bind_encode27_bb(py::module& m);
    void bind_fec_decode_b(py::module& m);
    void bind_fec_encode_b(py::module& m);
    void bind_file_sink_pdu(py::module& m);
    void bind_gmsk_demod(py::module& m);
    void bind_kiss_decode_pdu(py::module& m);
    void bind_kiss_encode_pdu(py::module& m);
    void bind_lilacsat1_frame_depack(py::module& m);
    void bind_pdu_f32_to_u8(py::module& m);
    void bind_pipe_sink_pdu(py::module& m);
    void bind_plan13_cc(py::module& m);
    void bind_print_to_file_b(py::module& m);
    void bind_rs_decode_pdu(py::module& m);
    void bind_satellite_channel_cc(py::module& m);
    void bind_send_file_pdu(py::module& m);
    void bind_serial_sink_pdu(py::module& m);
    void bind_sync_det_b(py::module& m);
    void bind_two_bit_dpd_frame_recovery(py::module& m);
    void bind_two_bit_dpd_precoder(py::module& m);
    void bind_vitfilt27_bb(py::module& m);
    void bind_vitfilt27_fb(py::module& m);
// ) END BINDING_FUNCTION_PROTOTYPES


// We need this hack because import_array() returns NULL
// for newer Python versions.
// This function is also necessary because it ensures access to the C API
// and removes a warning.
void* init_numpy()
{
    import_array();
    return NULL;
}

PYBIND11_MODULE(lilacsat_python, m)
{
    // Initialize the numpy C API
    // (otherwise we will see segmentation faults)
    init_numpy();

    // Allow access to base block methods
    py::module::import("gnuradio.gr");

    /**************************************/
    // The following comment block is used for
    // gr_modtool to insert binding function calls
    // Please do not delete
    /**************************************/
    // BINDING_FUNCTION_CALLS(
    bind_afsk1200_tx_f(m);
    bind_afsk1200_rx_f(m);
    bind_angle_cf(m);
    bind_attach_preamble_and_tailer(m);
    bind_attach_rs_codeblock(m);
    bind_bpsk_demod_cb(m);
    bind_cc_decode_bb(m);
    bind_ccsds_afsk_decode(m);
    bind_ccsds_afsk_encode(m);
    bind_codec2_decode_bf(m);
    bind_codec2_encode_fb(m);
    bind_dslwp_tm_header_parser(m);
    bind_dslwp_tm_parser(m);
    bind_encode27_bb(m);
    bind_fec_decode_b(m);
    bind_fec_encode_b(m);
    bind_file_sink_pdu(m);
    bind_gmsk_demod(m);
    bind_kiss_decode_pdu(m);
    bind_kiss_encode_pdu(m);
    bind_lilacsat1_frame_depack(m);
    bind_pdu_f32_to_u8(m);
    bind_pipe_sink_pdu(m);
    bind_plan13_cc(m);
    bind_print_to_file_b(m);
    bind_rs_decode_pdu(m);
    bind_satellite_channel_cc(m);
    bind_send_file_pdu(m);
    bind_serial_sink_pdu(m);
    bind_sync_det_b(m);
    bind_two_bit_dpd_frame_recovery(m);
    bind_two_bit_dpd_precoder(m);
    bind_vitfilt27_bb(m);
    bind_vitfilt27_fb(m);
    // ) END BINDING_FUNCTION_CALLS
}