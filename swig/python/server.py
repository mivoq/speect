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
"""

import os
import sys
import socket
import cPickle as pickle
import threading
import optparse
import ConfigParser
import logging

import speect
import speect.audio


__author__     = "HLT - Meraka Institute CSIR"
__copyright__  = "Copyright 2009, " + __author__
__credits__    = [__author__]
__license__    = "mit"
__version__    = "0.1"
__email__      = "dvniekerk@csir.co.za"


NAME = "server.py"
DEF_LOG = os.path.join(os.environ.get("HOME"), ".speectserver/server.log")
DEF_LOGLEVEL = 20
DEF_CONF = os.path.join(os.environ.get("HOME"), ".speectserver/server.conf")
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
        log.info("Server initialised.")

    def loadvoice(self, voicename, voicefilename):
        log.info("Loading voice from file '%s'" % (voicefilename))
        v = speect.SVoice(voicefilename, True)
        log.info("Voice '%s' loaded." % (voicename))
        self.voices[voicename] = v

    def get_voicelist(self):
        return self.voices.keys()

    def _socksetup(self, lport):
        self.lsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.lport = lport
        self.lsocket.bind(("", self.lport))

    
    def run(self):
        while True:
            try:
                log.info("Waiting for connections...")
                self.lsocket.listen(5)
                c = TTSHandler(self.lsocket.accept(), self) 
                c.start() 
                self.threads.append(c)
            except KeyboardInterrupt: #send SIGINT to shutdown server properly...
                log.info("Server stopped.")
                break
        self.lsocket.close()
        for c in self.threads: 
            c.join()
        #TODO: shutdown Speect...
        
            
    def synth(self, request):
        log.info("Synthesis request: %s" % request)
        try:
            utt = self.voices[request["voicename"]].synth(request["text"])
            speectwaveform = utt.features["audio"]
            if not speectwaveform:
                log.error("Synthesis failed.")
                reply = {"success": False,
                         "sampletype": None,
                         "samplerate": None,
                         "samples": None}
            else:
                log.info("Synthesis successful.")
                reply = {"success": True}
                reply.update(speectwaveform.get_audio_waveform())
        except RunTimeError:
            log.error("Synthesis failed.")
            reply = {"success": False,
                     "sampletype": None,
                     "samplerate": None,
                     "samples": None}
        return reply


class TTSHandler(threading.Thread): 
    def __init__(self, sock_addr, tts_server): 
        threading.Thread.__init__(self) 
        self.csocket, self.address = sock_addr
        self.rx_size = RECV_SIZE
        self.tts_server = tts_server


    def run(self):
        log.info("Connection made %s running %s to handle request" % (self.address, self))
        request = self.rx_req()
        if request["type"] == "synth":
            log.info("Synthesis request received successfully.")
            reply = self.tts_server.synth(request)
        elif request["type"] == "listvoices":
            log.info("Listvoices request received successfully.")
            reply = {"success": True,
                     "voicelist": self.tts_server.get_voicelist()}
        self.tx_reply(reply)
        log.info("Reply sent successfully.")
        #remove self from tts_server thread list...
        for i, t in enumerate(self.tts_server.threads):
            if t is self:
                self.tts_server.threads.pop(i)
                        

    def tx_reply(self, reply):
        replymsg = pickle.dumps(reply)
        self.csocket.sendall(replymsg)
        self.csocket.close()
        

    def rx_req(self):
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

def setopts():
    """ Setup all possible command line options....
    """
    usage = 'USAGE: %s [options]' % (NAME)
    version = NAME + " " + __version__
    parser = optparse.OptionParser(usage=usage, version=version)
    parser.add_option("-c",
                      "--configfile",
                      dest="configfn",
                      default=DEF_CONF,
                      help="Config file to load.",
                      metavar="CONFFILENAME")
    parser.add_option("-p",
                      "--port",
                      type="int",
                      dest="port",
                      default=DEF_PORT,
                      help="Specify the port number to serve on. [%default]",
                      metavar="PORTNUM")
    parser.add_option("-l",
                      "--logfile",
                      dest="logfn",
                      default=DEF_LOG,
                      help="File to write logs to file.",
                      metavar="LOGFILENAME")
    parser.add_option("-v",
                      "--logverbosity",
                      dest="loglevel",
                      default=DEF_LOGLEVEL,
                      help="Logging level.",
                      metavar="LOGLEVEL")
    return parser


if __name__ == "__main__":
    parser = setopts()
    opts, args = parser.parse_args()

    #loadconf
    config = ConfigParser.RawConfigParser()
    with open(opts.configfn) as conffh:
        config.readfp(conffh)

    #setup logging...
    try:
        fmt = "%(asctime)s [%(levelname)s] %(message)s"
        log = logging.getLogger(NAME)
        formatter = logging.Formatter(fmt)
        ofstream = logging.FileHandler(opts.logfn, "a")
        ofstream.setFormatter(formatter)
        log.addHandler(ofstream)
        # Console output.
        console = logging.StreamHandler()
        log.setLevel(opts.loglevel)
        console.setFormatter(formatter)
        log.addHandler(console)
    except Exception, e:
        print "ERROR: Could not create logging instance.\n\tReason: %s" %e
        sys.exit(1)

    #setup server...
    tts_server = TTSServer(lport=opts.port)
    for voicename in config.sections():
        voicedefinitionfile = config.get(voicename, "voice_definition")
        tts_server.loadvoice(voicename, voicedefinitionfile)

    tts_server.run()
    
