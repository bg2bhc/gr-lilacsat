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

import numpy
from gnuradio import gr
import pmt

import telemetry
from construct.core import ConstError
from satellites.csp_header import CSP
import struct

class telemetry_parser(gr.basic_block):
    """
    docstring for block telemetry_parser
    """
    def __init__(self):
        gr.basic_block.__init__(self,
            name="telemetry_parser",
            in_sig=[],
            out_sig=[])

        self.message_port_register_in(pmt.intern('in'))
        self.set_msg_handler(pmt.intern('in'), self.handle_msg)

    def handle_msg(self, msg_pmt):
        msg = pmt.cdr(msg_pmt)
        if not pmt.is_u8vector(msg):
            print "[ERROR] Received invalid message type. Expected u8vector"
            return
        packet = bytearray(pmt.u8vector_elements(msg))

        if len(packet) < 4:
            return
        csp = CSP(packet[:4])

        packet_number = struct.unpack('<I', packet[-8:-4])[0]

        if csp.destination == 6:
            print 'Packet number', packet_number, '(camera)'
            return
        
        # destination 5 is used for telemetry
        if csp.destination != 5:
            return

        data = telemetry.beacon_parse(packet[4:])
        if data:
            print 'Packet number', packet_number, '(telemetry)'
            print '--------------------------------------------'
            print(data)
            print
        else:
            print 'Could not parse beacon'
            print

        
