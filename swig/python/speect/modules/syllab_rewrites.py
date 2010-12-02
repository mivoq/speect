#!/usr/bin/env python
######################################################################################
## Copyright (c) 2010 The Department of Arts and Culture,                           ##
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
## A simple rule rewrites implementation for syllabification.                       ##
##                                                                                  ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


### PYTHON2 ###
from __future__ import unicode_literals, division, print_function
import codecs
open = codecs.open
### PYTHON2 ###

import re
import speect.modules.rewrites as rewrites

left_context = 0
graphemes = 1
right_context = 2
phone = 3

class Syllab_Rewrites(rewrites.Rewrites):
    """ Class to contain and implement the application of rewrite
        rules to predict pronunciations of isolated words...

        ruleset is a dict of lists where the each list contains all
        RewriteRules associated with a specific grapheme...
        
    """

    WHITESPACE_CHAR = "#"
    
    def __init__(self, rules, sets=None):

        super(Syllab_Rewrites, self).__init__(sets)

        for fest_rule in rules:
            lc = fest_rule[left_context].strip()
            LC = list()
            for i in lc.split(' '):
                if i != '':
                    LC.append(i)

            g = fest_rule[graphemes].strip()
            G = list()
            for i in g.split(' '):
                if i != '':
                    G.append(i)

            rc = fest_rule[right_context].strip()
            RC = list()
            for i in rc.split(' '):
                if i != '':
                    RC.append(i)

            p = fest_rule[phone].strip()
            P = list()
            for i in p.split(' '):
                if i != '':
                    P.append(i)


            rule = rewrites.RewriteRule(LC, G, RC, P, 0)
            try:
                self.ruleset[G[0]].append(rule)     # we don't use the last field...
            except KeyError:
                self.ruleset[G[0]] = []
                self.ruleset[G[0]].append(rule)     # we don't use the last field...
       

    
    def syllabify(self, word, phones):
        """ Predict phone sequence given word...
        """

        # word is Speect item, not using for now
     
        phones.append("#")
        phones.reverse()
        phones.append("#")
        phones.reverse()

        #find matching rule and thus phoneme for each grapheme..
        tmp = self.rewrite(phones)

        syl = list()
        syllables = list()
        for i in tmp:
            if i != '-':
                syl.append(i)
            else:
                syllables.append(syl)
                syl = list()

        if len(syl) != 0:
            syllables.append(syl)
            

        return syllables


