#coding:utf-8

import time
import threading
import socket    
import select
import Queue


class SocketSender(threading.Thread):
    def __init__(self,Proxy_Interface,CB,MyName):
        threading.Thread.__init__(self)
        self.thread_stop = False
        self.MyName = MyName
        self.CallBackFun = CB
        self.Proxy_Interface = Proxy_Interface
        self.SendQueue = Queue.Queue()
        self.ReSendBuf = None
        
    
    def run(self): 
        while not self.thread_stop:
            if self.Proxy_Interface.sock == None or self.Proxy_Interface.StateNow != 2:
                time.sleep(0.05)
                continue
            
            if self.SendQueue.empty() == False or self.ReSendBuf != None:
                try:
                    infds,outfds,errfds = select.select([],[self.Proxy_Interface.sock,],[],5)
                except:
                    continue
                
                if len(outfds)!=0:
                    if self.ReSendBuf != None:
                        data = self.ReSendBuf
                        print("["+self.MyName+"]"+ "Trying To Send Resend Buffer")
                        self.ReSendBuf = None
                    else:
                        data = self.SendQueue.get()
                        
                    if self.CallBackFun != None:
                        data_s = self.CallBackFun(self.Proxy_Interface,data)
                    else:
                        data_s = data
                    
                    try:
                        self.Proxy_Interface.sock.send(data_s)
                        print("["+self.MyName+"]"+"Successfully Send Data Length = %d"%len(data)  )
                    except socket.error as msg:
                        self.ReSendBuf = data
                        print("["+self.MyName+"]"+"Fialed To Send" + str(msg))
            else:
                time.sleep(0.05)

    def SendData(self,data):
        self.SendQueue.put(data)
        
    def stop(self):     
        self.thread_stop = True
        print("["+self.MyName+"]"+"Sender Is Stopping")    



class ProxyConnectInterface(threading.Thread):
    def __init__(self,Host,Port,Rcv_CB,Snt_CB,AutoConnect,MyName):
        threading.Thread.__init__(self)
        self.StateNow=0
        self.thread_stop = False
        self.Host = Host
        self.Port = Port
        self.MyName = MyName
        self.Rcv_CB = Rcv_CB
        self.Snt_CB = Snt_CB
        self.sock = None
        self.sender = SocketSender(self,Snt_CB,MyName)
        self.AlwaysConnectToServer = AutoConnect
        self.CB_Connected = None
        
    def SetCB_Connected(self,CBFunction):
        self.CB_Connected = CBFunction

    def run(self): 
        self.sender.start()
        while not self.thread_stop: 
            if self.StateNow==0:#状态0:此时不需要与服务器建立连接
                if (self.AlwaysConnectToServer==True): #始终保持连接
                    self.TryTimeOut=0;
                    self.StateNow=1;
                else: #有发送请求时才发起连接
                    if self.sender.SendQueue.empty() == True:
                        time.sleep(0.05)
                    else:
                        self.TryTimeOut=0;
                        self.StateNow=1;
                continue
            elif self.StateNow ==1: #状态1：建立与远程服务器的链接
                try:
                    self.sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                except socket.error as msg:
                    print("["+self.MyName+"]"+"Fialed To Create Socket" + str(msg))
                    time.sleep(0.1)
                    continue
                
                try:
                    self.sock.connect((self.Host,self.Port))
                    while (self.sock.getsockopt(socket.SOL_SOCKET, socket.SO_ERROR) <0):
                            print "Connection Not Ready..."
                            time.sleep(0.1)
                            pass
                    if self.CB_Connected != None:
                        self.CB_Connected()
                        
                    self.StateNow=2
                    print ("["+self.MyName+"]"+"Connected...")
                except socket.error as msg:
                    print("["+self.MyName+"]"+"Failed To Connect To ..."+str(msg))
                    try:
                        self.sock.close()
                        self.sock = None
                    except:
                        pass
                    time.sleep(0.5)
                    self.TryTimeOut+=1
                    if (self.TryTimeOut>10):
                        print("["+self.MyName+"]"+"Try To Connect 10 Times But All Failed,Stop Trying Again...")
                        self.StateNow=0;
                    continue

            elif self.StateNow==2:#状态2：等待接收远程服务器发回的消息
                while (not self.thread_stop):
                    infds,outfds,errfds = select.select([self.sock,],[],[],5)
                    if len(infds)!=0:
                        try:
                            data = self.sock.recv(1024)
                            if data:#如果收到的数据不是空字符串
                                print("["+self.MyName+"]"+"Received Data ...")
                                if self.Rcv_CB != None:
                                    self.Rcv_CB(self,data)
                            else:#收到空字符串，认为是对方断开链接
                                self.sock.close()
                                self.sock = None
                                self.StateNow=0;
                                print ("["+self.MyName+"]"+"The Connection Is Closed...")
                                break;
                        except socket.error as msg:
                            self.sock.close()
                            self.sock = None
                            self.StateNow=0;
                            print("["+self.MyName+"]"+"Error In Recv ..."+str(msg))
                            break;
                         
                    else:
                        print ("["+self.MyName+"]"+"No Data In 5 Seconds...") 
                        pass
                    
  
        print("["+self.MyName+"]"+"Stopped")
        
        while self.sender.isAlive() == True:
            time.sleep(0.1)
        print("["+self.MyName+"]"+" Sender Stopped")

    def SendData(self,data):
        self.sender.SendData(data)       
        
    def stop(self):    
        self.sender.stop() 
        self.thread_stop = True 
        print("["+self.MyName+"]"+"Stopping")
        




class ProxyListenInterface(threading.Thread):
    def __init__(self,Host,Port,Rcv_CB,Snt_CB,MyName):
        threading.Thread.__init__(self)
        self.StateNow=0
        self.thread_stop = False
        self.Host = Host
        self.Port = Port
        self.MyName = MyName
        self.Rcv_CB = Rcv_CB
        self.Snt_CB = Snt_CB
        self.sender = SocketSender(self,Snt_CB,MyName)
        self.listen_sock = None
        self.sock = None
        self.CB_Connected = None
        
    def SetCB_Connected(self,CBFunction):
        self.CB_Connected = CBFunction

    def run(self): 
        self.sender.start()
        while not self.thread_stop: 
            if self.StateNow==0:#状态0:准备监听
                try:
                    self.listen_sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                    self.listen_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR  , 1)
                    self.listen_sock.bind((self.Host,self.Port))
                    self.listen_sock.listen(1)
                    self.StateNow=1
                except socket.error as msg:
                    print("["+self.MyName+"]"+"Fialed To Create Socket Or Linsten" + str(msg))
                    self.listen_sock.close()
                    time.sleep(0.1)
                    continue
                
                
                
            elif self.StateNow ==1: #状态1：等待连接
                
                while(not self.thread_stop):#等待本地连接
                    infds,outfds,errfds = select.select([self.listen_sock,],[],[],5)
                    if len(infds)!=0:
                        self.sock, addr = self.listen_sock.accept()
                        self.listen_sock.close()
                        self.listen_sock = None
                        
                        while (self.sock.getsockopt(socket.SOL_SOCKET, socket.SO_ERROR) <0):
                            print "Connection Not Ready..."
                            time.sleep(0.1)
                            pass
            
                        
                        print("["+self.MyName+"]"+"Connected To Proxy...")
                        if self.CB_Connected != None:
                            self.CB_Connected()
                        self.StateNow=2#切换到下一个状态，等待接收数据
                        break
                    else:
#                         print ("["+self.MyName+"]"+"No Connection In 5 Seconds...")
                        pass
                try:    
                    self.listen_sock.close()
                    self.listen_sock = None         
                except:
                    pass    

            elif self.StateNow==2:#状态2：等待接收远程服务器发回的消息
                while (not self.thread_stop):
                    infds,outfds,errfds = select.select([self.sock,],[],[],5)
                    if len(infds)!=0:
                        try:
                            data = self.sock.recv(1024)
                            if data:#如果收到的数据不是空字符串
                                print("["+self.MyName+"]"+"Received Data ...")
                                if self.Rcv_CB != None:
                                    self.Rcv_CB(self,data)
                            else:#收到空字符串，认为是对方断开链接
                                self.sock.close()
                                self.sock = None
                                self.StateNow=0;
                                print ("["+self.MyName+"]"+"The Connection Is Closed...")
                                break;
                        except socket.error as msg:
                            self.sock.close()
                            self.sock = None
                            self.StateNow=0;
                            print("["+self.MyName+"]"+"Error In Recv ..."+str(msg))
                            break;
                         
                    else:
                        #print ("["+self.MyName+"]"+"No Data In 5 Seconds...") 
                        pass
                    
                    
        print("["+self.MyName+"]"+"Stopped")
        
        while self.sender.isAlive() == True:
            time.sleep(0.1)
        print("["+self.MyName+"]"+" Sender Stopped")
                    
    def SendData(self,data):
        self.sender.SendData(data)
        
    def stop(self):  
        self.sender.stop()   
        self.thread_stop = True
        print("["+self.MyName+"]"+"Stopping")
        
