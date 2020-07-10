# coding:utf-8
import Queue
import time
import codecs
import json
import struct
from core.kiss_decoder import KISS_Decoder

from core.connection_to_server_websocket import WebSocketClient


class server_data(object):
    '''
    @ 这个数据结构用来保持服务器列表，因为设计中要连接到多个服务器
    @ 数据存储： 外部采用服务器名作为key，构成字典， 字典的没一项是个list， 内容为 host，port, kiss_enable
    '''
    def __init__(self):
        self.server_dict = dict()

    def create(self, name, address, port, kiss_enable):
        name = str(name)
        address = str(address)
        port = int(port)
        self.server_dict[name] = [address, port, kiss_enable]

    def del_server(self, name):
        self.server_dict.pop(name)

    def select_server(self, name):
        if name in self.server_dict.keys():
            return self.server_dict[name]

    def show_all_names(self):
        '''
        @ 返回全部服务器的名字
        '''
        return self.server_dict.keys()


class port_data(object):
    '''
    @ 端口只有1～5, 采用list，每个list的元素为字典，
    @ 字典包括：
        port_num : 1~5
        port : grc_port
        satellite : e.g. DSLWP-A
        channel : e.g. 0
        server : Server
        protocol : 协议类型
        enable : 是否启动
    '''
    def __init__(self):
        self.port_data = list()
        for each in range(5):
            self.port_data.append('')

    def change_data(self, port_num, port, satellite, channel, server, protocol, enable):
        '''
        @ 输入：
            见class头的说明
        '''
        self.port_data[port_num] = dict()
        self.port_data[port_num]['port_num'] = port_num
        self.port_data[port_num]['port'] = port
        self.port_data[port_num]['satellite'] = satellite
        self.port_data[port_num]['channel'] = channel
        self.port_data[port_num]['server'] = server
        self.port_data[port_num]['protocol'] = protocol
        self.port_data[port_num]['enable'] = enable

    def get_enabled(self):
        '''
        @ 返回enabled选择的配置信息
        '''
        enabled_list = list()
        for each in self.port_data:
            if each['enable']:
                enabled_list.append(each)
        return enabled_list

    def return_data(self, name):
        '''
        输入 name  如 ‘enable’
        返回 5个端口的 ‘enable’的列表
        '''
        return_list = list()
        for each in self.port_data:
            return_list.append(each[name])
        return return_list


class DataBuf(object):
    '''
    @ 数据缓存区， 从grc返回的数据会暂存于此，每当有数据存入时，会触发‘传送’函数，将数据发往各个服务器
    @ enable_kiss_decode : 如果GRC的数据采用了Kiss， 则在加入队列之前进行解码
    @ self.sender : 发送的下行服务器
    '''
    def __init__(self, Name):
        self.name = Name
        self.data_buff = Queue.Queue()  # 队列中存储http_data
        self.sender = None
        self.enable_kiss_decode = False
        self.info = None  # info 保存应该发送给服务器的其他参数 e.g. sat_name
        self.http_data = None  # 在DSLWP项目中应该为json
        self.kiss = KISS_Decoder()
        self.logger = None

    def set_kiss_decode_mode(self, is_enable):
        '''
        @ 设置kiss decoder的状态
        '''
        self.enable_kiss_decode = is_enable

    def set_info(self, info):
        '''
        @ info 字典模式
        '''
        self.info = info

    def set_sender(self, sender):
        '''
        @ 输入 sender 应该是一个websocket 或 socket 对象， 应有 .send(data)函数
        '''
        self.sender = sender
    
    def set_logger(self, logger):
        self.logger = logger

    def send_signal(self):
        '''
        @ 向send_list中的各个对象启动SendData函数
        '''
        while not self.data_buff.empty():
            data = self.data_buff.get()
            if isinstance(self.sender, WebSocketClient):
                # 判断sender类型对数据做不同处理
                data = json.dumps(data)  # 转json
                self.sender.SendData(data)
            else:
                # 处理在SendData函数中
                self.sender.SendData(data)

    def put(self, data):
        '''
        @ 输入： 应为grc接收的数据
        '''
        if self.enable_kiss_decode:
            # 在这里进行kiss解码
            data = self.kiss.AppendStream(data)
        self.create_http_data(data)
        self.data_buff.put(self.http_data)
        self.logger(self.http_data, self.name)  # 存储日志
        if self.sender is not None:
            self.send_signal()

    def create_http_data(self, data):
        '''
        @ 生成数据包
        '''
        self.http_data = dict()
        self.http_data = self.info
        self.http_data['raw_data'] = codecs.encode(data, 'hex')
        self.http_data['proxy_receive_time'] = int(time.time()*1000)
        # self.http_data = json.dumps(self.http_data)
