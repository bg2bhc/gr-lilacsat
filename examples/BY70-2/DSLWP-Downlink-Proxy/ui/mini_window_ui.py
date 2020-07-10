# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mini_window.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_new_server_window(object):
    def setupUi(self, new_server_window):
        new_server_window.setObjectName(_fromUtf8("new_server_window"))
        new_server_window.resize(394, 231)
        new_server_window.setMinimumSize(QtCore.QSize(394, 231))
        new_server_window.setMaximumSize(QtCore.QSize(394, 231))
        self.Server_Name = QtGui.QLineEdit(new_server_window)
        self.Server_Name.setGeometry(QtCore.QRect(110, 30, 241, 27))
        self.Server_Name.setObjectName(_fromUtf8("Server_Name"))
        self.URL = QtGui.QLineEdit(new_server_window)
        self.URL.setGeometry(QtCore.QRect(110, 80, 241, 27))
        self.URL.setObjectName(_fromUtf8("URL"))
        self.port = QtGui.QLineEdit(new_server_window)
        self.port.setGeometry(QtCore.QRect(110, 130, 241, 27))
        self.port.setObjectName(_fromUtf8("port"))
        self.label = QtGui.QLabel(new_server_window)
        self.label.setGeometry(QtCore.QRect(20, 30, 91, 17))
        self.label.setObjectName(_fromUtf8("label"))
        self.label_2 = QtGui.QLabel(new_server_window)
        self.label_2.setGeometry(QtCore.QRect(20, 80, 91, 17))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.label_3 = QtGui.QLabel(new_server_window)
        self.label_3.setGeometry(QtCore.QRect(20, 130, 91, 17))
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.create_server = QtGui.QPushButton(new_server_window)
        self.create_server.setGeometry(QtCore.QRect(170, 200, 97, 26))
        self.create_server.setObjectName(_fromUtf8("create_server"))
        self.close = QtGui.QPushButton(new_server_window)
        self.close.setGeometry(QtCore.QRect(280, 200, 97, 26))
        self.close.setObjectName(_fromUtf8("close"))
        self.kiss_decoder_enable = QtGui.QCheckBox(new_server_window)
        self.kiss_decoder_enable.setGeometry(QtCore.QRect(160, 170, 191, 22))
        self.kiss_decoder_enable.setObjectName(_fromUtf8("kiss_decoder_enable"))

        self.retranslateUi(new_server_window)
        QtCore.QMetaObject.connectSlotsByName(new_server_window)

    def retranslateUi(self, new_server_window):
        new_server_window.setWindowTitle(_translate("new_server_window", "New Server", None))
        self.label.setText(_translate("new_server_window", "Server name", None))
        self.label_2.setText(_translate("new_server_window", "Server URL", None))
        self.label_3.setText(_translate("new_server_window", "Server Port", None))
        self.create_server.setText(_translate("new_server_window", "OK", None))
        self.close.setText(_translate("new_server_window", "Cancel", None))
        self.kiss_decoder_enable.setText(_translate("new_server_window", "Kiss Decoder Enable", None))

