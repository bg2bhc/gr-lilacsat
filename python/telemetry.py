#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2016 Daniel Estevez <daniel@destevez.net>
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from construct import *

Hk_STM32 = Struct(Const(b'\x1c\xa1'),\
                  'config' / Int8ul,\
                  'flag_direct_ins' / Int8ul,\
                  'payload_mode' / Int8ul,\
                  'tx_mode' / Int8ul,\
                  'gain_tx' / Int16sl,\
                  'i_3v3' / Int16sl,\
                  'u_3v3' / Int16sl,\
                  'i_vbat_tx' / Int16sl,\
                  'u_vbat_tx' / Int16sl,\
                  'i_vbat_rx' / Int16sl,\
                  'u_vbat_rx' / Int16sl,\
                  't_stm32' / Int16sl,\
                  't_pa' / Int16sl,\
                  'n_tx_rf' / Int16ul,\
                  'n_rx_rf' / Int16ul,\
                  'n_tx_err_rf' / Int16ul,\
                  'n_tx_err_rf' / Int16ul,\
                  'n_tx_i2c' / Int16ul,\
                  'n_rx_i2c' / Int16ul,\
                  'n_tx_err_i2c' / Int16ul,\
                  'n_rx_err_i2c' / Int16ul,\
                  'n_tc' / Int32ul,\
                  'dc_fm_tc' / Int16sl,\
                  'dc_fm_ham' / Int16sl,\
                  'rssi_fm_tc' / Int32ul,\
                  'rssi_fm_ham' / Int32ul,\
                  'reset_flag' / Int8ul,\
                  'sys_flag' / Int8ul,\
                  'dma_overflow' / Int16ul,\
                  'runtime' / Int32ul,\
                  'reset_count' / Int32ul,\
                  'ctcss_count' / Int32ul,\
                  'ctcss_det' / Float32l)

Cfg = Struct(Const(b'\x1c\xa2'),\
             'gain_tx_HI' / Int16sl,\
             'gain_tx_LO' / Int16sl,\
             'bias_I' / Int16sl,\
             'bias_Q' / Int16sl,\
             'threshold_u_vbat_rx_powerdown' / Int16sl,\
             'threshold_u_vbat_rx_repeateroff' / Int16sl,\
             'threshold_t_pa' / Int8sl,\
             'cam_ham_interval' / Int8ul,\
             'cam_ham_en' / Int8ul,\
             'ctcss_en' / Int8ul,\
             'ctcss_n_integration' / Int8ul,\
             'ctcss_n_tail' / Int8ul,\
             'ctcss_coeff' / Float32l,\
             'ctcss_threshold' / Float32l,\
             'gain_fmdm_ham' / Float32l,\
             'gain_fmdm_tc' / Float32l,\
             'interval_hk_OBC' / Int32ul,\
             'interval_hk_TLM' / Int32ul,\
             'interval_hk_BEACON' / Int32ul,\
             'message' / String(28),\
             'cam_delay' / Int32ul,\
             'crc' / Int32ul)

Hk_AVR = Struct(Const(b'\x1c\xa3'),\
                'adf7021_ld' / Int8ul,\
                'err_flag' / Int8ul,\
                'n_tx_i2c' / Int16ul,\
                'n_rx_i2c' / Int16ul,\
                'n_tx_232' / Int16ul,\
                'n_rx_232' / Int16ul,\
                'runtime' / Int32ul,\
                'rssi_analog' / Int8ul,\
                'n_rssi_const' / Int8ul,\
                'unlock_count' / Int8ul,\
                'reset_flag' / Int8ul,\
                'reset_count' / Int32ul )

def beacon_parse(data):
    for beacon in [Hk_STM32, Cfg, Hk_AVR]:
        try:
            return beacon.parse(data)
        except Exception:
            pass
    return None
