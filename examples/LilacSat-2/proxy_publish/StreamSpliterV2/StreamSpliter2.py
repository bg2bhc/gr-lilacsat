#coding:utf-8

import struct

class StreamSpliter2():
    
    #状态机说明：
    #0：空闲状态，还没有找到包头
    #1：已经找到包头，但是还没有获得包长   
    #2：已经获得包长和包类型，等待数据
    def __init__(self,callBack,maxPacketSize=1024):
        self.DataBuffer=''
        self.callBack=callBack
        self.haderFound=0
        self.PacketSize=0
        self.maxPacketSize=maxPacketSize
        self.StateNow=0

    '''重置各个变量的值,准备下一次解包'''    
    def NextPacket(self,cutNum):
        self.DataBuffer=self.DataBuffer[cutNum:]
        self.haderFound=0
        self.PacketSize=0
        self.PacketType=0
        self.StateNow=0;  

        
    def NewStream(self,data,Param=None):
        self.DataBuffer+=data
        
        while(True):
            #处于空闲状态，未找到包头
            if self.StateNow == 0:
                tmp1=self.DataBuffer.find('PaC_t')
                if (tmp1!=-1):
                    self.haderFound=tmp1+5#指向跳过包头的位置
                    self.StateNow=1
                else:
                    if (len(self.DataBuffer)>4):#没有找到包头,则一定可以忽略掉一些,避免缓冲区无限制加长
                        self.DataBuffer=self.DataBuffer[-4:]
                    return
                    
                    
            #已经找到包头，但是还没有获得包长                
            if self.StateNow ==1:
                
                if len(self.DataBuffer)-self.haderFound>5:
                    self.PacketSize=struct.unpack('i',self.DataBuffer[self.haderFound:self.haderFound+4])[0]
                    if (self.PacketSize<=0 or self.PacketSize>=self.maxPacketSize):
                        self.NextPacket(1)# 当前包头是坏的,换下一个
                        continue
                        
                    self.PacketType=ord(self.DataBuffer[self.haderFound+5])
                    self.StateNow=2;
                else:
                    return
             
            #已经获得包长和包类型，等待数据        
            if self.StateNow ==2 :
    
                if len(self.DataBuffer)-self.haderFound-5 >= self.PacketSize:
                    tmp2 = self.DataBuffer[self.haderFound+5 : self.haderFound+5+self.PacketSize]
                    self.callBack(tmp2,self.PacketSize,self.PacketType,Param)
                    self.NextPacket(self.haderFound+5+self.PacketSize)
                    self.StateNow=0
                else:
                    return
            