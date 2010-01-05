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
END_OF_MESSAGE_STRING = "<EoM>"


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
    return parser


if __name__ == "__main__":
    parser = setopts()
    opts, args = parser.parse_args()

    if len(args) < 2:
        parser.print_usage()
        sys.exit()
        
    voicename, text = args
    host = opts.host
    port = opts.port

    #create message
    message = {"voicename": voicename,
               "text": text}
    fulls = pickle.dumps(message)
    #create a socket
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    #connect to server
    s.connect((host, port))
    #send..
    s.sendall(fulls)
    s.sendall(END_OF_MESSAGE_STRING)
    #recv waveform..
    wavemsgfull = str()
    while True:
        wavemsgpart = s.recv(RECV_SIZE)
        if wavemsgpart:
            wavemsgfull += wavemsgpart
        else:
            break
    #close connection..
    s.close()
    #recover waveform..
    waveform = pickle.loads(wavemsgfull)
    if waveform["sampletype"] != "int16":
        raise Exception("Client currently only supports 16bit samplesize")
    
    if opts.wavefilename:
        #assume that we only want to save - not play..
        outwf = wave.open(opts.wavefilename, "w")
        outwf.setparams((1, 2, waveform["samplerate"], len(waveform["samples"]) / 2, "NONE", "not compressed"))
        outwf.writeframesraw(waveform["samples"])
        outwf.close()
    else:
        #init audio device and play samples..
        dsp = ossaudiodev.open("/dev/dsp", "w")
        dsp.setparameters(ossaudiodev.AFMT_S16_LE,
                          1,
                          waveform["samplerate"],
                          True)
        dsp.writeall(waveform["samples"])
        dsp.close()

    
