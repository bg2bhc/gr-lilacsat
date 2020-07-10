# coding:utf-8
'''
@ 用于KISS解码功能
KISS 协议 数据包以C0开头 ，以C0结尾
当数据中有C0时， 以 DB DC替换
当数据中有DB时， 以 DB DD替换
'''
KISS_FEND = '\xC0'
KISS_FESC = '\xDB'
KISS_TFEND = '\xDC'
KISS_TFESC = '\xDD'


class KISS_Decoder():
    '''
    @ KISS协议解码器
    '''
    def __init__(self):
        self.InEscMode = False
        self.DataBuf = ""
        self.DecodedLength = 0

    def AppendStream(self, stream_data):
        '''
        @ stream_data : kiss 编码的数据，只需要讲KISS数据帧，按照顺序通过
        stream_data传入，当解码完毕后会自动返回数据包，并重置解码器等待下一次解码
        '''
        for b in stream_data:
            if not self.InEscMode:
                if b == KISS_FEND:
                    if self.DecodedLength != 0:
                        data = self.DataBuf
                        self.reset_kiss()
                        return data  # 解码结束

                elif b == KISS_FESC:
                    self.InEscMode = True

                else:
                    self.DataBuf = self.DataBuf + b
                    self.DecodedLength += 1

            else:
                if b == KISS_TFEND:
                    self.DataBuf = self.DataBuf + KISS_FEND
                elif b == KISS_TFESC:
                    self.DataBuf = self.DataBuf + KISS_FESC

                self.DecodedLength += 1
                self.InEscMode = False

    def reset_kiss(self):
        self.DataBuf = ""
        self.DecodedLength = 0
