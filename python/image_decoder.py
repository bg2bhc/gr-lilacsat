#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2016 Daniel Estevez
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

import struct
import os.path

import numpy
from gnuradio import gr
import pmt

class image_decoder(gr.basic_block):
    """
    docstring for block image_decoder
    """
    def __init__(self, path='/tmp'):
        gr.basic_block.__init__(self,
            name="image_decoder",
            in_sig=[],
            out_sig=[])

        self.path = path
        self.message_port_register_in(pmt.intern('in'))
        self.set_msg_handler(pmt.intern('in'), self.handle_msg)
        self.files = dict()
        self.remaining = dict()

    def handle_msg(self, msg_pmt):
        msg = pmt.cdr(msg_pmt)
        if not pmt.is_u8vector(msg):
            print "[ERROR] Received invalid message type. Expected u8vector"
            return
        packet = bytearray(pmt.u8vector_elements(msg))

        dst = (packet[1] & 0xf0) >> 4
        if dst != 6:
            return

        # TODO check CRC

        image_id = packet[4]
        length = struct.unpack('<H', packet[9:11])[0]
        index = struct.unpack('<H', packet[12:14])[0]
        data = packet[15:-8]

        if image_id not in self.files:
            self.files[image_id] = open(os.path.join(self.path, str(image_id) + '.jpg'), 'wb', 0)
            self.remaining[image_id] = length

        f = self.files[image_id]
        f.seek(index)
        f.write(data)

        self.remaining[image_id] = self.remaining[image_id] - len(data)
        if self.remaining[image_id] <= 0:
            # image finished
            print 'Finished downloading image', image_id
            f.close()
            del self.remaining[image_id]
            del self.files[image_id]
