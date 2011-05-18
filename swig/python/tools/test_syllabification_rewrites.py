#!/usr/bin/env python
######################################################################################
## Copyright (c) 2011 The Department of Arts and Culture,                           ##
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
## AUTHOR  : Aby Louw                                                               ##
## DATE    : May 2011                                                               ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Test a syllabification rewrites ruleset on an given input phone string list      ##
##                                                                                  ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

import sys
import optparse
import speect
import speect.syllabification
import speect.syllab_rewrites_json


__author__     = "HLT - Meraka Institute CSIR"
__copyright__  = "Copyright 2011, " + __author__
__credits__    = [__author__]
__license__    = "mit"
__version__    = "0.1"

DEF_RULES = "syllabification.json"
NAME = "test_syllab.py"



def do_it(rulesfile, phonestring):
    plist = phonestring.strip().split(",")
    syllab = speect.syllabification.SSyllabification.load_rewrites_json(rulesfile)
    syls = syllab.syllabify(None, plist)
    for i in syls:
        print i


def setopts():
    """ Setup all possible command line options....
    """
    usage = 'USAGE: %s [options]' % (NAME)
    version = NAME + " " + __version__
    parser = optparse.OptionParser(usage=usage, version=version)
    parser.add_option("-p",
                      "--phonestring",
                      dest="phonestring",
                      help="Comma separated phone string to syllabify.")
    parser.add_option("-r",
                      "--rulesfile",
                      dest="rulesfile",
                      default=DEF_RULES,
                      help="Syllabification rewrite rules file.")
    return parser


if __name__ == "__main__":
    parser = setopts()

    if (len(sys.argv) < 2):
        parser.print_help()
        parser.error("you must specify a comma separated phone string")
        
    opts, args = parser.parse_args()
    do_it(opts.rulesfile, opts.phonestring)
