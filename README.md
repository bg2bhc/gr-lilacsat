# gr-lilacsat
---------------------------------------
GNU Radio OOT Module for telemetry decoding of LilacSat-1, LilacSat-2 and BY70-1 satellites.

For on GRC 3.8.x. For 3.7.x please use the maint-3.7 branch.

Currently this OOT module does not work on ubuntu 16.04 LTS. Please contact me if you can find a solution.

This project is supported by Harbin Institute of Technology.

Visit http://lilacsat.hit.edu.cn/ for more information.

## Requirements
---------------------------------------

* [gr-csp](https://github.com/daniestevez/gr-csp/)
* [construct 2.8](https://construct.readthedocs.io/en/latest/) For telemetry parser
* [feh](https://feh.finalrewind.org/) For realtime image display

## Examples
---------------------------------------
frontend_rx_\*.grc in examples folder for different devices:
* frontend_rx_fcdpp.grc for Funcube Dongle Pro Plus (gr-fcdproplus needed);
* frontend_rx_uhd.grc for USRP;
* frontend_rx_rtl.grc for RTL-SDR.

demod_node\*_\*.grc in examples folder for different modulation and rate:
* demod_node1_bpsk_9k6.grc for 437.200 MHz 9600 bps RRC-BPSK telemetry;
* demod_node1_afsk.grc for 437.200 MHz 1200 bps AFSK-FM telemetry;
* demod_node1_ccsds_fm.grc for 437.200 MHz 300 bps subaudio baseband telemetry;
* demod_node4_4k8.grc for 437.225 MHz 4800 bps GFSK telemetry.
 
Use frontend_rx_\*.grc, demod_node1_bpsk_9k6.grc and demod_node4_4k8.grc by default.

proxy_publish is also included in examples folder for upload telemetry for display.

## LilacSat-2 Live CD
---------------------------------------
A Live CD is also provided: http://lilacsat.hit.edu.cn/?page_id=257

Manual: http://lilacsat.hit.edu.cn/wp-content/uploads/2015/09/LilacSat-2_Live_CD_User_Manual.pdf

## LilacSat-1 Downlink IQ Record
---------------------------------------
A IQ record of LilacSat-1 downlink: https://drive.google.com/open?id=0B_j7kp3QtCNaejVlWDFEVGxiR2M

It can be used for a evaluation of the FM uplink codec2 downlink repeater.

## BY70-1 Downlink Bit Steam Record
---------------------------------------
A bit stream record of BY70-1 downlink: https://drive.google.com/open?id=0B_j7kp3QtCNaa0I3Nng4b3Z5QTA

It can be used for a evaluation of the telemetry downlink and image downlink.
