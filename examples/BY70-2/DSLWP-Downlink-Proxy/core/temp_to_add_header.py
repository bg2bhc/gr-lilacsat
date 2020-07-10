# coding:utf-8
'''
0x1A
0xCF
0xFC
0x10
'''
import struct


def add_HTY_header(data):
    data_len = struct.pack("I", len(data))[0]
    data = "\x1A" + "\xCF" + "\xFC" + "\x1D" + "\x00" + data_len + data
    # 数据填充
    while True:
        if len(data) < 222:
            data = data + '\x00'
        else:
            break

    return data