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
## AUTHOR  : Aby Louw                                                               ##
## DATE    : October 2010                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## A simple tokenizer that splits tokens on whitespace and adds pre- and            ##
## post-punctuation into a Token relation.                                          ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


pre_punctuation = ['\\', '`', '(', '{', '[']
post_punctuation = ['\\', '`', '.', ',', ':', ';', '!', '?', '(', ')', '{', '}', '[', ']']

def utt_processor(utt):
    # check that we have "input" feature
    if not "input" in utt.features:
        raise RuntimeError("Failed to find 'input' feature of utterance")

    # get input
    text = utt.features["input"]

    # create token relation
    token_rel = utt.relation_new("Token")

    # first split on spaces
    tokens = text.split(' ')
    
    whitespace = '' # doesn't start with whitespace
    for token in tokens:
        if token == '':
            whitespace += ' ' 
            continue
        else:
            # create token item
            token_item = token_rel.append()     
        
        if whitespace != '':
            token_item["whitespace"] = whitespace
            whitespace = ' ' # now tokens start with whitespace
            
        start = 0
        end = 0
        for char in token:
            if char in pre_punctuation:
                start +=1
            else:
                break

        if start != 0:
            prepunc = token[0:start]
            token_item["prepunc"] = prepunc

        for char in reversed(token):
            if char in post_punctuation:
                end +=1
            else:
                break

        if end != 0:
            postpunc = token[len(token) - end:]
            token_item["postpunc"] = postpunc

        token_text = token[start:len(token)-end]
        token_item["name"] = token_text

        
