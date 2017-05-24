#!/usr/bin/python
#coding:utf-8

import socket
import time
import threading  
from LilacProxyCore import *
import struct
from StreamSpliterV2.StreamSpliter2 import StreamSpliter2
import os
import webbrowser  

from PyQt4.QtGui import *
from PyQt4 import QtGui,QtCore
import PyQt4
from DownLinkMain import Ui_Form
import sys
from _dbus_bindings import String

import xml.dom.minidom as minidom
from xml.dom.minidom import Document

import urllib2

import json

from kiss_decoder import KISS_Decoder

class MyForm(QtGui.QMainWindow):
    def __init__(self, parent=None): 
        QtGui.QWidget.__init__(self, parent) 
        self.ui = Ui_Form()    
        self.ui.setupUi(self)  
        self.ui.cmd_StartProxy.clicked.connect(self.StartProxy)
        self.ui.cmd_StartGNURadio.clicked.connect(self.StartGNURadio)
        self.ui.cmd_SaveSettings.clicked.connect(self.SaveData)
        self.ui.cmd_exit.clicked.connect(self.OnQuit)
        self.ui.cmd_update_orbit.clicked.connect(self.UpdateOrbit)
        self.ui.link_BY2HIT.clicked.connect(self.Link_BY2HIT)
        self.ui.link_HIT.clicked.connect(self.Link_HIT)
        self.ui.link_Lilac.clicked.connect(self.Link_Lilac)
        self.refreshtimer=QtCore.QTimer()
        QtCore.QObject.connect(self.refreshtimer,QtCore.SIGNAL("timeout()"), self.OnTimer)
        
        sys.stdout = EmittingStream(textWritten=self.normalOutputWritten)
        self.refreshtimer.start( 500 )
        
        
        self.HOST = 'lilacsat.hit.edu.cn'       
        self.PORT_C_DA = 60061
        self.PORT_C_DB = 60062
        self.PORT_S_D = 61261
        self.Proxy_D_S=None
        self.Proxy_D_S_B=None
        self.Proxy_D_LA=None
        self.Proxy_D_LB=None
        self.Kiss_Decoder = KISS_Decoder(self.handle_D_L2RA_Kiss)
        self.ProxyRunning = False
        
        try:
            dom = minidom.parse(os.path.split(os.path.realpath(__file__))[0] +"/settings.xml")
            domroot = dom.documentElement
            self.ui.txt_Name.setText(domroot.getElementsByTagName('nickname')[0].childNodes[0].nodeValue)
            self.ui.txt_Lon.setText(domroot.getElementsByTagName('lon')[0].childNodes[0].nodeValue)
            self.ui.txt_Lat.setText(domroot.getElementsByTagName('lat')[0].childNodes[0].nodeValue)
            self.ui.txt_Alt.setText(domroot.getElementsByTagName('alt')[0].childNodes[0].nodeValue)
            self.ui.txt_URL.setText(domroot.getElementsByTagName('host')[0].childNodes[0].nodeValue)
            self.ui.txt_URL_Back.setText(domroot.getElementsByTagName('back_host')[0].childNodes[0].nodeValue)
            self.ui.BackUrlEnable.setCheckState(int(domroot.getElementsByTagName('back_host_enable')[0].childNodes[0].nodeValue))
            self.ui.txt_Port.setText(domroot.getElementsByTagName('port')[0].childNodes[0].nodeValue)
            self.ui.txt_TLE.setText(domroot.getElementsByTagName('tle')[0].childNodes[0].nodeValue)
        except:
            pass

    
    def closeEvent(self,e):
        if self.Proxy_D_LA != None:
            self.Proxy_D_LA.stop()
        if self.Proxy_D_LB != None:
            self.Proxy_D_LB.stop()    
        if self.Proxy_D_S != None:
            self.Proxy_D_S.stop()
        if self.Proxy_D_S_B != None:
            self.Proxy_D_S_B.stop()
        
        
    def StartProxy(self):
        if self.ProxyRunning == False:
            self.ProxyRunning = True
            self.ui.cmd_StartProxy.setText("Stop Proxy")
            
            self.ChannelALog = open(os.path.split(os.path.realpath(__file__))[0] + "/ChannelADownLink.log", 'a')
            self.ChannelBLog = open(os.path.split(os.path.realpath(__file__))[0] + "/ChannelBDownLink.log", 'a')
            
            self.HOST = self.ui.txt_URL.text()
            self.HOST_BACK = self.ui.txt_URL_Back.text()
            self.PORT_S_D = int(self.ui.txt_Port.text())
            self.NickName = str(self.ui.txt_Name.text())
            self.NickName += "\x00" + " "*(15-len(self.NickName))
            self.Lon = float(str(self.ui.txt_Lon.text()))
            self.Lat = float(str(self.ui.txt_Lat.text()))
            self.Proxy_D_S = ProxyConnectInterface(self.HOST,self.PORT_S_D,None,None,False,"D_S")
            self.Proxy_D_S_B = ProxyConnectInterface(self.HOST_BACK,self.PORT_S_D,None,None,False,"D_S_B")
            self.Proxy_D_LA = ProxyListenInterface("0.0.0.0",self.PORT_C_DA,self.handle_D_L2RA,None,"D_L")
            self.Proxy_D_LB = ProxyListenInterface("0.0.0.0",self.PORT_C_DB,self.handle_D_L2RB,None,"D_L")
            self.Proxy_D_S.start()
            self.Proxy_D_LA.start()
            self.Proxy_D_LB.start()
            if self.ui.BackUrlEnable.checkState() == 2:
                self.Proxy_D_S_B.start()
        else:
            self.ui.cmd_StartProxy.setEnabled(False)
            self.ui.cmd_StartProxy.setText("Stopping...")
            self.Proxy_D_S.stop()
            self.Proxy_D_LA.stop()
            self.Proxy_D_LB.stop()
            self.Proxy_D_S_B.stop()
            
            time.sleep(0.2)
            while(self.Proxy_D_S.isAlive() == True or self.Proxy_D_S_B.isAlive() == True or self.Proxy_D_LA.isAlive() == True or self.Proxy_D_LB.isAlive() == True):
                time.sleep(0.2)
            
            
            self.ProxyRunning = False
            self.ui.cmd_StartProxy.setText("Start Proxy")
            self.ui.cmd_StartProxy.setEnabled(True)
            
    def StartGNURadio(self):
        if os.fork() == 0:
            os.execvp("gnome-terminal",("gnome-terminal","-x","bash","-c", "/home/lilac/workspace_l1/ftp_server_simulator/Debug/ftp_server_simulator -a20 -s127.0.0.1 -fpacketlog_node4.db"))
    
    
    def SaveData(self):
        doc = Document()  #创建DOM文档对象

        settings = doc.createElement('settings') #创建根元素
        doc.appendChild(settings)
        
        entry = doc.createElement("nickname")
        entry.appendChild(doc.createTextNode(str(self.ui.txt_Name.text())))
        settings.appendChild(entry)
        
        entry = doc.createElement("lon")
        entry.appendChild(doc.createTextNode(str(self.ui.txt_Lon.text())))
        settings.appendChild(entry)
        
        entry = doc.createElement("lat")
        entry.appendChild(doc.createTextNode(str(self.ui.txt_Lat.text())))
        settings.appendChild(entry)
        
        entry = doc.createElement("alt")
        entry.appendChild(doc.createTextNode(str(self.ui.txt_Alt.text())))
        settings.appendChild(entry)
        
        entry = doc.createElement("host")
        entry.appendChild(doc.createTextNode(str(self.ui.txt_URL.text())))
        settings.appendChild(entry)
        
        entry = doc.createElement("back_host")
        entry.appendChild(doc.createTextNode(str(self.ui.txt_URL_Back.text())))
        settings.appendChild(entry)
        
        entry = doc.createElement("back_host_enable")
        entry.appendChild(doc.createTextNode(str(self.ui.BackUrlEnable.checkState())))
        settings.appendChild(entry)
        
        entry = doc.createElement("port")
        entry.appendChild(doc.createTextNode(str(self.ui.txt_Port.text())))
        settings.appendChild(entry)
        
        entry = doc.createElement("tle")
        entry.appendChild(doc.createTextNode(str(self.ui.txt_TLE.text())))
        settings.appendChild(entry)
          
        ########### 将DOM对象doc写入文件
        f = open(os.path.split(os.path.realpath(__file__))[0] +'/settings.xml','w')
        f.write(doc.toprettyxml(indent = ''))
        f.close()
        
        
    def UpdateOrbit(self):
        
        try:
            f=urllib2.urlopen(str(self.ui.txt_TLE.text()))
            tle = f.read()
            tle=tle.split("\n")
            
            tle1 = tle[1]
            tle2 = tle[2]
            

            with open(os.path.split(os.path.realpath(__file__))[0] + "/grc_param.py", 'w') as fp: 
                fp.write("lat=" + str(self.ui.txt_Lat.text()) + "\n")
                fp.write("lon=" + str(self.ui.txt_Lon.text()) + "\n")
                fp.write("alt=" + str(self.ui.txt_Alt.text()) + "\n")
                fp.write("tle_line1=\"" + tle1 + "\"\n")
                fp.write("tle_line2=\"" + tle2 + "\"\n")
                fp.close()
                
            print "Orbit Data Successfully Updated!"
            
        except Exception as msg:
            print msg
        
    
    def OnQuit(self):
        self.close()
        
    def Link_BY2HIT(self):
        webbrowser.open("http://www.by2hit.net")
    def Link_HIT(self):
        webbrowser.open("http://www.hit.edu.cn")
    def Link_Lilac(self):
        webbrowser.open("http://lilacsat.hit.edu.cn")
    
    def OnTimer(self):
        if self.Proxy_D_LA != None:
            if self.Proxy_D_LA.StateNow == 2:
                self.ui.lbl_status_gnu_A.setText("""Receiver A: <font color="green">Connected</font>""")
            else:
                self.ui.lbl_status_gnu_A.setText("Receiver A: Not Connected")
        else:
            self.ui.lbl_status_gnu_A.setText("Receiver A: Not Connected")
            
        
        if self.Proxy_D_LB != None:
            if self.Proxy_D_LB.StateNow == 2:
                self.ui.lbl_status_gnu_B.setText("""Receiver B: <font color="green">Connected</font>""")
            else:
                self.ui.lbl_status_gnu_B.setText("Receiver B: Not Connected")
        else:
            self.ui.lbl_status_gnu_B.setText("Receiver B: Not Connected")
        
        
        if self.Proxy_D_S  != None:    
            if self.Proxy_D_S.StateNow == 2:
                self.ui.lbl_status_server.setText("""Server: <font color="green">Connected</font>""")
            else:
                self.ui.lbl_status_server.setText("Server: Not Connected")
        else:
            self.ui.lbl_status_server.setText("Server: Not Connected")
        
        if self.Proxy_D_S_B  != None:    
            if self.Proxy_D_S_B.StateNow == 2:
                self.ui.lbl_status_server_back .setText("""BackUp Server: <font color="green">Connected</font>""")
            else:
                self.ui.lbl_status_server_back.setText("BackUp Server: Not Connected")
        else:
            self.ui.lbl_status_server_back.setText("BackUp Server: Not Connected")
                
        try:
            result = os.waitpid(-1,os.WNOHANG)
        except:
            pass
    
    #下行数据，本地端到远程端处理程序
    def handle_D_L2RA(self,caller,data):
        self.Kiss_Decoder.AppendStream(data)
        
        
        
    def handle_D_L2RA_Kiss(self,data):
        Length_Bin=struct.pack("I",len(data)+64)
        tmp_data="PaC_t" +Length_Bin+"\x01" + self.NickName + struct.pack("d",self.Lon) + struct.pack("d",self.Lat) + " "*32 +data
        self.Proxy_D_S.SendData(tmp_data)
        if self.ui.BackUrlEnable.checkState() == 2:
            self.Proxy_D_S_B.SendData(tmp_data)

        self.ChannelALog.write(str(time.strftime('%Y-%m-%d %H:%M:%S',time.gmtime())) +"\n")
        self.ChannelALog.write(self.NickName.replace("\x00", "") +"\n")
        self.ChannelALog.write(str(self.Lon) +"\n")
        self.ChannelALog.write(str(self.Lat) +"\n")
        log = " ".join(["%02X"%ord(i) for i in data]) + "\n"
        self.ChannelALog.write(log)
        print log
        self.ChannelALog.flush()
        

    def handle_D_L2RB(self,caller,data):
        Length_Bin=struct.pack("I",len(data)+64)
        tmp_data="PaC_t" +Length_Bin+"\x01" + self.NickName + struct.pack("d",self.Lon) + struct.pack("d",self.Lat) + " "*32 +data
        self.Proxy_D_S.SendData(tmp_data)
        if self.ui.BackUrlEnable.checkState() == 2:
            self.Proxy_D_S_B.SendData(tmp_data)

        self.ChannelBLog.write(str(time.strftime('%Y-%m-%d %H:%M:%S',time.gmtime())) +"\n")
        self.ChannelBLog.write(self.NickName.replace("\x00", "") +"\n")
        self.ChannelBLog.write(str(self.Lon) +"\n")
        self.ChannelBLog.write(str(self.Lat) +"\n")
        log = " ".join(["%02X"%ord(i) for i in data]) + "\n"
        self.ChannelBLog.write(log)
        print log
        self.ChannelBLog.flush()
    
        
    def normalOutputWritten(self, text):
        """Append text to the QTextEdit."""
        str_buf = self.ui.txt_log.toPlainText()
        str_buf = str_buf + text
        length = str_buf.count()

        maxLength = 2000
        if(length > maxLength):
            str_buf.remove(0, length - maxLength);
            
        self.ui.txt_log.setText(str_buf);    
        textCursor = self.ui.txt_log.textCursor();
        self.ui.txt_log.setText(str_buf);
        textCursor.setPosition(str_buf.count());
        self.ui.txt_log.setTextCursor(textCursor);


class EmittingStream(QtCore.QObject):
     
        textWritten = QtCore.pyqtSignal(str)
     
        def write(self, text):
            self.textWritten.emit(str(text))

    
if __name__ == "__main__":    
    app = QtGui.QApplication(sys.argv)  
    myapp = MyForm()    
    myapp.show()  
    sys.exit(app.exec_())

