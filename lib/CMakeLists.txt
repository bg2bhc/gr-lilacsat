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
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND lilacsat_sources
  #  satellite_channel_cc_impl.cc
    afsk/afsk.c
    afsk/ax25.c
    afsk/crc_ccitt.c
    afsk/fifobuf.c
    afsk/kfile.c
    afsk/wavegen.c
    ccsds/rs.c
    ccsds/ccsds.c
    ccsds/tab.c
    ccsds/metrics.c
    ccsds/randomizer.c
    kiss/kiss.c
    ccsds/viterbi27.c
    afsk1200_rx_f_impl.cc
    afsk1200_tx_f_impl.cc
    fec_decode_b_impl.cc
    fec_encode_b_impl.cc
    satellite_channel_cc_impl.cc
    bpsk_demod_cb_impl.cc
    kiss_decode_pdu_impl.cc
    kiss_encode_pdu_impl.cc
    vitfilt27_bb_impl.cc
    encode27_bb_impl.cc
    vitfilt27_fb_impl.cc
    angle_cf_impl.cc
    pipe_sink_pdu_impl.cc
    plan13/Plan13.cpp
    serial/serial.c
    serial_sink_pdu_impl.cc
    plan13_cc_impl.cc
    print_to_file_b_impl.cc
    codec2/codebook.c
    codec2/codebookd.c
    codec2/codebookdt.c
    codec2/codebookge.c
    codec2/codebookjnd.c
    codec2/codebookjvm.c
    codec2/codebooklspmelvq.c
    codec2/codebookmel.c
    codec2/codebookres.c
    codec2/codebookvq.c
    codec2/codebookvqanssi.c
    codec2/codec2.c
    codec2/dump.c
    codec2/interp.c
    codec2/kiss_fft.c
    codec2/lpc.c
    codec2/lsp.c
    codec2/nlp.c
    codec2/pack.c
    codec2/phase.c
    codec2/postfilter.c
    codec2/quantise.c
    codec2/sine.c
    codec2_decode_bf_impl.cc
    codec2_encode_fb_impl.cc
    cc_decode_bb_impl.cc
    sync_det_b_impl.cc
    lilacsat1_frame_depack_impl.cc
    gmsk_demod_impl.cc
    dslwp_tm_header_parser_impl.cc
    file_sink_pdu_impl.cc )

set(lilacsat_sources "${lilacsat_sources}" PARENT_SCOPE)
if(NOT lilacsat_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT lilacsat_sources)

add_library(gnuradio-lilacsat SHARED ${lilacsat_sources})
target_link_libraries(gnuradio-lilacsat ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-lilacsat PROPERTIES DEFINE_SYMBOL "gnuradio_lilacsat_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-lilacsat PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-lilacsat
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_lilacsat_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_lilacsat.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_lilacsat.cc
)

add_executable(test-lilacsat ${test_lilacsat_sources})

target_link_libraries(
  test-lilacsat
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-lilacsat
)

GR_ADD_TEST(test_lilacsat test-lilacsat)
