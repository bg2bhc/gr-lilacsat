# coding:utf-8
'''
@ 用于建立连接，
从GRC 通过 TCP 接收数据，
并且缓存到data_buf
'''
import threading
import socket
import select


class LinkToGRC(threading.Thread):
    '''
    @ 从grc接收数据socket类
    @ 成员变量：
        index ：0~4
    '''
    def __init__(self, data_buf_list, host, port, index):
        threading.Thread.__init__(self)

        self.data_buf_list = data_buf_list
        self.name = 'Port' + str(index + 1)
        self.index = index
        self.host = host
        self.port = port
        self.thread_stop = False
        self.grc_listener = None
        self.connection = None
        self.create_socket()
        self.is_connected = False

    def get_port(self):
        '''
        用于判断是否重复监听端口
        '''
        return self.port

    def append_data_buf(self, buf):
        '''
        在上层配置该类对象时，如果判断端口已经被监听，则添加发送出的buf
        '''
        self.data_buf_list.append(buf)

    def create_socket(self):
        '''
        @ 建立socket连接
        '''
        try:
            self.grc_listener = socket.socket(
                socket.AF_INET, socket.SOCK_STREAM)
            # 为了实现socket.close() 之后可以直接重新使用端口
            self.grc_listener.setsockopt(
                socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.grc_listener.bind((self.host, self.port))
            self.grc_listener.listen(1)
        except socket.error as msg:
            print(
                "[" + self.name + "]"
                + " Fialed To Create Socket Or Listen" + str(msg))
            self.grc_listener.close()

    def run(self):
        '''
        @ 重写的thread的run函数，负责处理连接
        '''
        select_input = list()
        select_input.append(self.grc_listener)
        while not self.thread_stop:
            infds, outfds, errfds = select.select(
                select_input, [], [], 5)
            if len(infds) != 0:
                if not self.is_connected:
                    conn, addr = self.grc_listener.accept()
                    self.is_connected = True
                    self.connection = conn
                    select_input.append(conn)
                    print('connect ' + str(addr))
                data = conn.recv(1024)

                if not data:  # 如果客户端主动断开连接， 则服务器在下一个循环重新等待客户连接
                    self.is_connected = False
                    conn.close()
                    self.connection = None
                    select_input.remove(conn)
                    print('logout')
                else:
                    if self.data_buf_list is not None:
                        for each in self.data_buf_list:
                            each.put(data)

    def stop(self):
        '''
        @ 停止循环
        '''
        self.thread_stop = True
        self.grc_listener.close()
        if self.connection is not None:
            self.connection.close()

    def get_index(self):
        '''
        @ 返回代理index 0～4
        '''
        return self.index
