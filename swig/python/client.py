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
## Simple client to make TTS requests and play the result..                         ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


""" Simple client to make TTS requests and play the result..
"""

import sys
import cPickle as pickle
import socket
import time
import ossaudiodev
from optparse import OptionParser
import wave


__author__     = "HLT - Meraka Institute CSIR"
__copyright__  = "Copyright 2009, " + __author__
__credits__    = [__author__]
__license__    = "mit"
__version__    = "0.1"
__email__      = "dvniekerk@csir.co.za"


NAME = "client.py"
RECV_SIZE = 1024
DEF_PORT = 22222
DEF_HOST = "localhost"


def setopts():
    """ Setup all possible command line options....
    """
    usage = 'USAGE: %s [options] VOICENAME "Input text."' % (NAME)
    version = NAME + " " + __version__
    parser = OptionParser(usage=usage, version=version)
    parser.add_option("-s",
                      "--save",
                      dest="wavefilename",
                      default=None,
                      help="Save waveform to RIFF wave file.",
                      metavar="WAVEFILE")
    parser.add_option("-p",
                      "--port",
                      type="int",
                      dest="port",
                      default=DEF_PORT,
                      help="Specify the port number to connect to. [%default]",
                      metavar="PORTNUM")
    parser.add_option("-a",
                      "--address",
                      dest="host",
                      default=DEF_HOST,
                      help="Specify the host address to connect to. [%default]",
                      metavar="HOSTADDRESS")
    parser.add_option("-l",
                      "--listvoices",
                      action="store_true",
                      dest="listvoices",
                      help="Request a list of loaded voices from the server.")
    return parser


class TTSClient(object):
    END_OF_MESSAGE_STRING = "<EoM>"
    
    def __init__(self, host=DEF_HOST, port=DEF_PORT, recv_size=RECV_SIZE):
        self.host = host
        self.port = port
        self.recv_size = recv_size
    
    def request(self, rtype="listvoices", voice=None, text=None):
        try:
            text = unicode(text, "latin-1").encode("utf-8")
        except TypeError:
            pass
        message = {"type": rtype,
                   "voicename": voice,
                   "text": text}

        fulls = pickle.dumps(message)
        #create a socket
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        #connect to server
        s.connect((self.host, self.port))
        #send..
        s.sendall(fulls)
        s.sendall(TTSClient.END_OF_MESSAGE_STRING)
        #recv reply..
        replymsgfull = str()
        while True:
            replymsgpart = s.recv(self.recv_size)
            if replymsgpart:
                replymsgfull += replymsgpart
            else:
                break
        #close connection..
        s.close()
        #recover reply..
        reply = pickle.loads(replymsgfull)
        if not reply["success"]:
            raise Exception("Request failed..")
        
        return reply

    def write_audio(self, voice, text, filename):
        reply = self.request("synth", voice, text)
        if reply["sampletype"] != "int16":
            raise Exception("Client currently only supports 16bit samplesize")
        outwf = wave.open(filename, "w")
        outwf.setparams((1, 2, reply["samplerate"], len(reply["samples"]) / 2, "NONE", "not compressed"))
        outwf.writeframesraw(reply["samples"])
        outwf.close()
        


if __name__ == "__main__":
    parser = setopts()
    opts, args = parser.parse_args()


    #create message
    if not opts.listvoices:
        if len(args) < 2:
            parser.print_usage()
            sys.exit()
        voicename, text = args

    host = opts.host
    port = opts.port
    client = TTSClient(host, port)

    if opts.listvoices:
        reply = client.request("listvoices")
        print("\n".join(sorted(reply["voicelist"])))
    else:
        if opts.wavefilename:
            #assume that we only want to save - not play..
            client.write_audio(voicename, text, opts.wavefilename)
        else:
            #init audio device and play samples..
            reply = client.request("synth", voicename, text)
            if reply["sampletype"] != "int16":
                raise Exception("Client currently only supports 16bit samplesize")
            dsp = ossaudiodev.open("/dev/dsp", "w")
            dsp.setparameters(ossaudiodev.AFMT_S16_LE,
                              1,
                              reply["samplerate"],
                              True)
            dsp.writeall(reply["samples"])
            dsp.close()
