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
## Implementation of a G2P rewriting module using the semicolon format of DictMaker.##
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


class G2P_Rewrites_Semicolon(g2p_rewrites.G2P_Rewrites):
    """Includes methods to load rules from "semicolon format" files...
    """


    def __init__(self, sets=None):
        super(G2P_Rewrites_Semicolon, self).__init__(sets)

    
    def load_simple_mapfile(self, maplocation):
        """ Load phone representation from simple text file...
            e.g:
                 1;p
                 0;0
                 3;bi

            Expecting one-to-one mappings...
        """

        mapping = {}

        with open(maplocation, "r", encoding="utf-8") as infh:
            for line in infh:
                a, b = line.strip().split(";")
                if a in mapping or b in mapping.values():
                    raise Exception("Mapping is not one-to-one..")
                else:
                    mapping[a] = b
        
        return mapping


    def load_simple_phonemapfile(self, maplocation):
        self.phonemap = self.load_simple_mapfile(maplocation)

    def load_simple_graphmapfile(self, maplocation):
        self.graphmap = self.load_simple_mapfile(maplocation)


    def load_gnulls(self, gnullslocation, wchar=g2p_rewrites.G2P_Rewrites.WHITESPACE_CHAR):
        """ Load gnulls mappings from simple text file...
            e.g
                uk;u0k
                ne;n0e
                ua;u0a
                u ;u0 
           
            NOTE: Significant whitespace at end of last entry...
        """
        
        mapping = {}

        with open(gnullslocation, "r", encoding="utf-8") as infh:
            for line in infh:
                a, b = line.rstrip("\n").split(";")
                a, b = (a.replace(" ", wchar), b.replace(" ", wchar))
                if a in mapping:
                    raise Exception("Keys are not unique...")
                else:
                    mapping[a] = b
        
        self.gnulls = mapping
        

    def load_ruleset_semicolon(self, filelocation, wchar=g2p_rewrites.G2P_Rewrites.WHITESPACE_CHAR):
        """ Load rules from semicolon delimited format
            "dictionarymaker format", replacing spaces with 'wchar':
            grapheme, left_context, right_context, phoneme, ordinal, number
            e.g:
                 a;;;a;0;1692
                 a;ntl;;n;1;1
                 b;;;b;0;241
                 c;;;5;0;4
                 d;;;d;0;231
                 e;;;e;0;446
        """

        self.ruleset = {}

        with open(filelocation, "r", encoding="utf-8") as infh:
            for line in infh:
                g, l, r, p, o, n = line.rstrip("\n").split(";")
                l, r = (l.replace(" ", wchar), r.replace(" ", wchar))
                plist = list()
                plist.append(p)
                rule = rewrites.RewriteRule(list(l), list(g), list(r), plist, int(o))
                try:
                    self.ruleset[g].append(rule)     # we don't use the last field...
                except KeyError:
                    self.ruleset[g] = []
                    self.ruleset[g].append(rule)     # we don't use the last field...

        self.sortAllRules()

    
    def map_phones(self):
        """ Apply self.phonemap to all phonemes in self.ruleset
        """
        
        for grapheme in self.ruleset:
            for rule in self.ruleset[grapheme]:
                newphone = self.phonemap[rule.B[0]]
                nlist = list()
                nlist.append(newphone)
                rule.B = nlist


    def map_graphs(self):
        """ Apply self.graphmap to all graphemes in self.ruleset
        """

        for k, v in self.graphmap.items():
            if k == v: continue
            self.ruleset[v] = self.ruleset[k]
            del self.ruleset[k]
            for g in self.ruleset:
                for r in self.ruleset[g]:
                    r.grapheme = re.sub(k, v, r.grapheme)
                    r.leftcontext = re.sub(k, v, r.leftcontext)
                    r.rightcontext = re.sub(k, v, r.rightcontext)
            if self.gnulls:
                for gk, gv in self.gnulls.items():
                    if (k in gk) or (k in gv):
                        del self.gnulls[gk]
                        gk = re.sub(k, v, gk)
                        gv = re.sub(k, v, gv)
                        self.gnulls.update({gk: gv})
