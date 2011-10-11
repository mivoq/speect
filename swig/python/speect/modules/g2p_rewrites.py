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
## A G2P rule rewriting module.                                                     ##
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

class NoRuleFound(Exception):
    pass

class GraphemeNotDefined(Exception):
    pass


class G2P_Rewrites(rewrites.Rewrites):
    """ Class to contain and implement the application of rewrite
        rules to predict pronunciations of isolated words...

        ruleset is a dict of lists where the each list contains all
        RewriteRules associated with a specific grapheme...
        
    """

    WHITESPACE_CHAR = "#"
    
    def __init__(self, sets=None):

        super(G2P_Rewrites, self).__init__(sets)
        self.features = {}
        self.gnulls = {}
        


    def sortAllRules(self):
        """ Make sure that all rulelists associated with each grapheme
        are sorted in the correct order for application (i.e. from
        most specific context to least specific context - based on the
        RewriteRule.ordinal)
        """
        for g in self.ruleset:
            self.ruleset[g].sort(key=lambda x: x.ordinal, reverse=True)


    def applyGnulls(self, word):
        """ Apply gnulls to word if applicable...
        """

        if self.gnulls:
            for gnull in self.gnulls:
                word = re.sub(gnull, self.gnulls[gnull], word)

        return word

    
    def apply(self, word):
        """ Predict phone sequence given word...
        """
        phones = []

        #append and prepend whitespace_char
        word = word.join([self.WHITESPACE_CHAR, self.WHITESPACE_CHAR])

        #apply gnulls
        word = self.applyGnulls(word)

        #find matching rule and thus phoneme for each grapheme..
        tmp = self.rewrite(list(word))

        # remove '' "phones"
        for i in tmp:
            if i != '':
                phones.append(i)

        return phones


