# Install script for directory: /home/lilac/gr-lilacsat/include/lilacsat

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/lilacsat" TYPE FILE FILES
    "/home/lilac/gr-lilacsat/include/lilacsat/api.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/afsk1200_rx_f.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/afsk1200_tx_f.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/fec_decode_b.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/fec_encode_b.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/satellite_channel_cc.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/bpsk_demod_cb.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/kiss_decode_pdu.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/kiss_encode_pdu.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/vitfilt27_bb.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/encode27_bb.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/vitfilt27_fb.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/angle_cf.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/pipe_sink_pdu.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/serial_sink_pdu.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/plan13_cc.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/print_to_file_b.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/codec2_decode_bf.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/codec2_encode_fb.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/cc_decode_bb.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/sync_det_b.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/sync_det_b.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/lilacsat1_frame_depack.h"
    "/home/lilac/gr-lilacsat/include/lilacsat/gmsk_demod.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

