# coding:utf-8
'''
使用Socket连接服务器
'''
import threading
import Queue
import time
import select
import socket
import codecs
import struct

from temp_to_add_header import add_HTY_header

class SocketSender(threading.Thread):
    '''
    @ 用于连接Socket服务器
    '''
    def __init__(self, MyName, Protocol):
        threading.Thread.__init__(self)
        self.host = None
        self.port = None
        self.socket_sender = None
        self.thread_stop = False
        self.MyName = MyName
        self.Protocol = Protocol
        self.SendQueue = Queue.Queue()
        self.ReSendBuf = None
        self.is_connected = False

    def create_connection(self, Host, Port):
        '''
        @ 建立socket连接
        '''
        self.host = Host
        self.port = Port
        try:
            # 建立连接对象
            self.socket_sender = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error as msg:
            print("["+self.MyName+"]"+"Failed To Create Socket" + str(msg))
        
    def _connect(self):
        try:
            # 尝试连接服务器
            self.socket_sender.connect((self.host, self.port))
            time.sleep(2)
            while(self.socket_sender.getsockopt(socket.SOL_SOCKET, socket.SO_ERROR) < 0):
                print("Connection Not Ready...")
                time.sleep(0.1)
            print("["+self.MyName+"]"+"Connected...")
            self.is_connected = True
        except socket.error as msg:
            print("["+self.MyName+"]"+"Failed To Connect To ..."+str(msg))

    def run(self):
        while not self.thread_stop:
            if self.socket_sender is None:
                time.sleep(0.05)
                continue

            if not self.is_connected:
                self._connect()
                continue

            if not self.SendQueue.empty() or self.ReSendBuf is not None:
                try:
                    infds, outfds, errfds = select.select([self.socket_sender, ], [self.socket_sender, ], [], 5)
                except:
                    continue

                if len(outfds) != 0:
                    # 这里的局部变量data是要发送的数据
                    if self.ReSendBuf is not None:
                        data = self.ReSendBuf
                        print("["+self.MyName+"]" + "Trying To Send Resend Buffer")
                        self.ReSendBuf = None
                    else:
                        data = self.SendQueue.get()

                    try:
                        self.socket_sender.send(data)
                        print(self.socket_sender.recv(1024))
                        print("["+self.MyName+"]" + "Successfully Send Data Length = %d" % len(data))
                    except socket.error as msg:
                        # 数据未发送：重新发送
                        self.ReSendBuf = data
                        print("["+self.MyName+"]"+"Fialed To Send" + str(msg))

                if len(infds) != 0:
                    recv_data = self.socket_sender.recv(1024)
                    if len(recv_data) == 0:
                        self.is_connected = False
            else:
                time.sleep(0.05)

    def SendData(self, data):
        if self.Protocol == 'socket':
            raw_data = codecs.decode(data['raw_data'], 'hex')
            Length_Bin = struct.pack("I", len(data) + 64)
            tmp_data = "PaC_t" + Length_Bin + "\x01" + data['proxy_nickname'] + struct.pack("d", data['proxy_long']) + struct.pack("d", data['proxy_lat']) + " "*32 + raw_data
            data = tmp_data
        elif self.Protocol == 'HTY':
            raw_data = codecs.decode(data['raw_data'], 'hex')
            data = add_HTY_header(raw_data)
        self.SendQueue.put(data)

    def stop(self):
        self.thread_stop = True
        print("["+self.MyName+"]"+"Sender Is Stopping")
