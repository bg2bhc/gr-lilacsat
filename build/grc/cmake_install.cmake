# Install script for directory: /home/lilac/gr-lilacsat/grc

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gnuradio/grc/blocks" TYPE FILE FILES
    "/home/lilac/gr-lilacsat/grc/lilacsat_afsk1200_rx_f.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_afsk1200_tx_f.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_fec_decode_b.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_fec_encode_b.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_satellite_channel_cc.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_bpsk_demod_cb.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_kiss_decode_pdu.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_kiss_encode_pdu.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_vitfilt27_bb.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_encode27_bb.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_vitfilt27_fb.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_angle_cf.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_pipe_sink_pdu.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_serial_sink_pdu.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_plan13_cc.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_print_to_file_b.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_codec2_decode_bf.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_codec2_encode_fb.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_cc_decode_bb.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_sync_det_b.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_sync_det_b.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_lilacsat1_frame_depack.xml"
    "/home/lilac/gr-lilacsat/grc/lilacsat_gmsk_demod.xml"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

