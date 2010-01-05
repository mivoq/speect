#!/usr/bin/env python
######################################################################################
## Copyright (c) 2009 The Department of Arts and Culture,                           ##
## The Government of the Republic of South Africa.                                  ##
##                                                                                  ##
## Contributors:  Meraka Institute, CSIR, South Africa.                             ##
##                                                                                  ##
## Permission is hereby granted, free of charge, to any person obtaining a copy     ##
## of this software and associated documentation files (the "Software"), to deal    ##
## in the Software without restriction, including without limitation the rights     ##
## to use, copy, modify, merge, publish, distribute, sublicense, and#or sell        ##
## copies of the Software, and to permit persons to whom the Software is            ##
## furnished to do so, subject to the following conditions:                         ##
## The above copyright notice and this permission notice shall be included in       ##
## all copies or substantial portions of the Software.                              ##
##                                                                                  ##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       ##
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         ##
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      ##
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           ##
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    ##
## OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN        ##
## THE SOFTWARE.                                                                    ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## AUTHOR  : Daniel van Niekerk                                                     ##
## DATE    : December 2009                                                          ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Simple server to load voices and serve TTS requests..                            ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


""" Simple server to load voices and serve TTS requests..

    To implement:
         `- feedback to client regarding success/failure..
         `- possibility of loading and removing voices while running..
"""

import socket
import cPickle as pickle
import threading
import speect
import audio


__author__     = "HLT - Meraka Institute CSIR"
__copyright__  = "Copyright 2009, " + __author__
__credits__    = [__author__]
__license__    = "mit"
__version__    = "0.1"
__email__      = "dvniekerk@csir.co.za"


DEF_PORT = 22222
RECV_SIZE = 1024
END_OF_MESSAGE_STRING = "<EoM>"


class TTSServer():
    
    def __init__(self, voicefilenames=[], lport=DEF_PORT):
        self.voices = {}
        self.threads = []
        #load voices..
        if voicefilenames:
            for voicefilename in voicefilenames:
                self.loadvoice(voicefilename)
        #socket setup..
        self._socksetup(lport)


    def loadvoice(self, voicefilename):
        #TODO: check this:
        str = "Loading voice from file \'" + voicefilename + "\'"
        print(str)
        v = speect.SVoice(file_name=voicefilename,
                          load_data=True)
        voicename = v.get_name()
        str = "Voice \'" + voicename + "\' loaded"
        print(str)
        self.voices[voicename] = v


    def _socksetup(self, lport):
        self.lsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.lport = lport
        self.lsocket.bind(("", self.lport))

    
    def run(self):
        while True:
            try:
                self.lsocket.listen(5)
                c = TTSHandler(self.lsocket.accept(), self) 
                c.start() 
                self.threads.append(c)
            except KeyboardInterrupt: #send SIGINT to shutdown server properly...
                break
        self.lsocket.close()
        for c in self.threads: 
            c.join()
        #TODO: shutdown Speect...
        
            
    def synth(self, requestmsg):
        inputSpeectString = speect.SString(requestmsg["text"])
        utt = self.voices[requestmsg["voicename"]].synth(utt_type="text",
                                                         input=inputSpeectString)
        speectwaveform = utt.get_feature("audio")
        if not speectwaveform:
            raise RuntimeError('Synthesis failed')

        waveform = speectwaveform.get_audio_waveform()
        return waveform



class TTSHandler(threading.Thread): 
    def __init__(self, sock_addr, tts_server): 
        threading.Thread.__init__(self) 
        self.csocket, self.address = sock_addr
        self.rx_size = RECV_SIZE
        self.tts_server = tts_server


    def run(self):
        requestmsg = self.rx_synthreq()
        waveform = self.tts_server.synth(requestmsg)
        self.tx_waveform(waveform)
        #remove self from tts_server thread list...
        for i, t in enumerate(self.tts_server.threads):
            if t is self:
                self.tts_server.threads.pop(i)
                        

    def tx_waveform(self, waveform):
        wavemsg = pickle.dumps(waveform)
        self.csocket.sendall(wavemsg)
        self.csocket.close()
        

    def rx_synthreq(self):
        fulls = str()
        while True:
            s = self.csocket.recv(self.rx_size)
            if s:
                fulls += s
                if fulls.endswith(END_OF_MESSAGE_STRING):
                    fulls = fulls.rstrip(END_OF_MESSAGE_STRING)
                    break
            else:
                break
        return pickle.loads(fulls)


if __name__ == "__main__":
    #Either CLI parms or config file should be loaded here and server
    #setup done based on that...
    tts_server = TTSServer()
    tts_server.loadvoice("/home/aby/Development/speect_release_december2009/sa_english/voice.conf")
    tts_server.run()
    
