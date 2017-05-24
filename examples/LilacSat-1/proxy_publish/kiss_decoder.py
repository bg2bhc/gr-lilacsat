#coding:utf-8

KISS_FEND = '\xC0'  
KISS_FESC = '\xDB' 
KISS_TFEND = '\xDC' 
KISS_TFESC = '\xDD'

class KISS_Decoder():
    

    
    def __init__(self,callback_function):
        self.InEscMode = False
        self.cbfun = callback_function
        self.DataBuf = ""
        self.DecodedLength = 0
        
    def AppendStream(self,stream_data):
        
        for b in stream_data:
            
            if (self.InEscMode == False):
                if (b == KISS_FEND):
                    
                    if (self.DecodedLength != 0 ):
                        self.cbfun(self.DataBuf)
                        
                    self.DataBuf = ""
                    self.DecodedLength = 0
                
                elif (b == KISS_FESC):
                    self.InEscMode = True
                
                else:
                    self.DataBuf = self.DataBuf + b
                    self.DecodedLength += 1
            
            else:
                if (b == KISS_TFEND):
                    self.DataBuf = self.DataBuf + KISS_FEND
                elif (b == KISS_TFESC):
                    self.DataBuf = self.DataBuf + KISS_FESC
                    
                self.DecodedLength += 1
                self.InEscMode = False
                    
                
        
        
