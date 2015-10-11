# gr-lilacsat
---------------------------------------
OOT Module for Harbin Institute of Technology LilacSat-2 satellite telemetry decoding.

Tested on GRC 3.7.8. gr-fcdproplus needed for use with FCDPP. 

frontend_rx_\*.grc in examples folder for different devices:
* frontend_rx_fcdpp.grc for Funcube Dongle Pro Plus;
* frontend_rx_uhd.grc for USRP;
* frontend_rx_rtl.grc for RTL-SDR.

demod_node\*_\*.grc in examples folder for different modulation and rate:
* demod_node1_bpsk_9k6.grc for 437.200 MHz 9600 bps RRC-BPSK telemetry;
* demod_node1_afsk.grc for 437.200 MHz 1200 bps AFSK-FM telemetry;
* demod_node1_ccsds_fm.grc for 437.200 MHz 300 bps subaudio baseband telemetry;
* demod_node4_4k8.grc for 437.225 MHz 4800 bps GFSK telemetry.
 
Use frontend_rx_\*.grc, demod_node1_bpsk_9k6.grc and demod_node4_4k8.grc by default.

proxy_publish is also included in examples folder for upload telemetry for display.

A Live CD is also provided: http://lilacsat.hit.edu.cn/?page_id=257

Manual: http://lilacsat.hit.edu.cn/wp-content/uploads/2015/09/LilacSat-2_Live_CD_User_Manual.pdf

Visit http://lilacsat.hit.edu.cn/ for more information.

