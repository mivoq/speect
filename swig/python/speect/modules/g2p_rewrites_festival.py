#!/usr/bin/env python
######################################################################################
## Copyright (c) 2010-2011 The Department of Arts and Culture,                      ##
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
## AUTHOR  : Daniel van Niekerk, Aby Louw                                           ##
## DATE    : October 2010                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Implementation of a G2P rewriting module using the semicolon format of Festival. ##
##                                                                                  ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

### PYTHON2 ###
from __future__ import unicode_literals, division, print_function
import codecs
open = codecs.open
### PYTHON2 ###

import speect.modules.g2p_rewrites as g2p_rewrites
import speect.modules.rewrites as rewrites


left_context = 0
graphemes = 1
right_context = 2
phone = 3


class G2P_Rewrites_Festival(g2p_rewrites.G2P_Rewrites):
    """Includes methods to load rules from "Festival format" files...
    """


    def __init__(self, rules, sets=None):
        super(G2P_Rewrites_Festival, self).__init__(sets)

        for fest_rule in rules:
            lc = fest_rule[left_context].strip()
            LC = list()
            for i in lc:
                if i != ' ':
                    LC.append(i)

            g = fest_rule[graphemes].strip()
            G = list()
            for i in g:
                if i != ' ':
                    G.append(i)

            rc = fest_rule[right_context].strip()
            RC = list()
            for i in rc:
                if i != ' ':
                    RC.append(i)

            p = fest_rule[phone].strip()
            P = list()
            P.append(p)

            rule = rewrites.RewriteRule(LC, G, RC, P, 0)
            try:
                self.ruleset[G[0]].append(rule)     # we don't use the last field...
            except KeyError:
                self.ruleset[G[0]] = []
                self.ruleset[G[0]].append(rule)     # we don't use the last field...
       
            
