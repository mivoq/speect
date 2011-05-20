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
## A rule rewriting module, can be used for G2P, syllabification, etc...            ##
##                                                                                  ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


class RewriteRule(object):
    """ Simply keeps rule info together...
        Rules are:    LC [ A ] RC => B     
    """

    def __init__(self, LC, A, RC, B, ordinal):

        # fix LC

        if LC != None:
            tmp = list()
            skip = False
            for i in range(0, len(LC)):
                if skip:
                    skip = False
                    continue
                if (i < len(LC) - 1) and ((LC[i + 1] == "*") or (LC[i + 1] == "+")):
                    tmp.append(LC[i + 1])
                    skip = True
                tmp.append(LC[i])
            tmp.reverse()
            self.LC = tmp
        else:
            self.LC = LC
        self.A = A
        self.RC = RC
        self.B = B
        self.ordinal = ordinal

    def __str__(self):
        """Print in 'semicolon format'...
        """
        ### PYTHON2 ###
        return ";".join([unicode(self.LC),
                         unicode(self.A),
                         unicode(self.RC),
                         unicode(self.B),
                         unicode(self.ordinal)])
        ### PYTHON2 ###

    def _itemMatch(self, PATT, THING, sets):

        if PATT == THING:
            return True

        if sets is None:
            return False

        if not PATT in sets:
            return False
        
        if not THING in sets[PATT]:
            return False

        return True
        

    def getRightHandSide(self):
        return self.B

  
    def _contextMatch(self, pattern, string, sets):
        """ Returns True if this rule matches the given context...
        """

        # pattern is rule left or right context
        # string is itape

        if len(pattern) == 0:
            # rule context is none, so match
            return True
        elif len(string) == 0:
            # rule context is not none and itape context is none
            return False
        elif len(pattern) > 1 and pattern[1] == "*":
            r = self._contextMatch(pattern[2:], string, sets)
            tmp = pattern[2:]
            tmp.insert(0, pattern[0])
            s = self._contextMatch(tmp, string, sets)
            t = self._itemMatch(pattern[0], string[0], sets) and self._contextMatch(pattern, string[1:], sets)
            return (r or s or t)
        elif len(pattern) > 1 and pattern[1] == "+":
            r = self._itemMatch(pattern[0], string[0], sets) 
            tmp = pattern[2:]
            tmp.insert(0, "*")
            tmp.insert(0, pattern[0])
            s = self._contextMatch(tmp, string[1:], sets)
            return (r and s)
        elif self._itemMatch(pattern[0], string[0], sets):
            return self._contextMatch(pattern[1:], string[1:], sets)
        else:
            return False

 
    def ruleMatches(self, LC, RC, sets):
        """ Returns this rule if it matches the given context...
        """

        # right context (actually A + RC) must be at least as long as rule's A
        if len(RC) < len(self.A):
            return None
        
        # check if [ A ] matches
        counter = 0
        a_match = []
        for c1, c2 in zip(self.A, RC):
            if not self._itemMatch(c1, c2, sets):
                return None, None
            a_match.append(c2)
            counter += 1

        # Check LC: LC may have some limited regex stuff  
        rLC = list(LC)
        rLC.reverse()
        if (self._contextMatch(self.LC, rLC,sets) and self._contextMatch(self.RC, RC[counter:],sets)):
            return a_match, RC[counter:]
        else:
            return None, None    
        


class Rewrites(object):
    """ Class to contain and implement the application of rewrite
        rules to predict pronunciations of isolated words...

        ruleset is a dict of lists where the each list contains all
        RewriteRules associated with a specific grapheme...
        
    """

    def __init__(self, sets=None):
        
        self.ruleset = {}
        self.sets = sets


    def _add_rule(self, index, rule):
        try:
            self.ruleset[index].append(rule)     
        except KeyError:
            self.ruleset[index] = []
            self.ruleset[index].append(rule)     


    def addRule(self, index, rule):
        if index in self.sets:
            for set_entry in self.sets[index]:
                self._add_rule(set_entry, rule)
        else:
            self._add_rule(index, rule)


    def itemMatch(self, PATT, THING, sets):

        if PATT == THING:
            return True

        if sets is None:
            return False

        if not PATT in sets:
            return False
        
        if not THING in sets[PATT]:
            return False

        return True


    def get_otape(self, rRHS, am):
        new_otape = []
        ruleRHS = list(rRHS)
        ruleRHS.reverse()
        a_match = list(am)
        a_match.reverse()
        while len(ruleRHS) > 0:
            p = ruleRHS.pop()
            if p == "-":
                new_otape.append(p)
            else:
                r = a_match.pop()
                new_otape.append(r)

        return new_otape


    def rewrite(self, itape):
        """ Run rules on itape (LC A RC) to create rewrites otape (B)
        
           :param itape: Input tape consisting of LC A RC
           :type itape: List
           :return: Output tape B
           :rtype: List
        """

 
        LC = itape[0:1]
        RC = itape[1:]
        otape = list()

        while len(RC) > 1:
            
            found_rule = False

            # search through rewrite rules to find matching one 
            for rule in self.ruleset[RC[0]]:
                a_match, newRC = rule.ruleMatches(LC, RC, self.sets)
                if newRC is not None:
                    found_rule = True
                    break
            
            if not found_rule:
                res = "Failed to find rewrite rule for itape sequence '" + str(itape) + "'"
                raise RuntimeError(res)
            
            otape += self.get_otape(rule.getRightHandSide(), a_match)
            LC = LC + RC[0:len(RC) - len(newRC)]
            RC = newRC
            
        return otape
                
    
  
