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
## A simple normalizer that does nothing else than creating a word relation and     ##
## populating it with the tokens from the token relation.                           ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

import speect
import speect.uttproc_cb   # utterance callback interface


pre_punctuation = ['\\', '`', '(', '{', '[']
post_punctuation = ['\\', '`', '.', ',', ':', ';', '!', '?', '(', ')', '{', '}', '[', ']']

def utt_processor(utt):
    # check that we have "Token" relation
    token_rel = utt.relation_get("Token")
    if token_rel is None:
        raise RuntimeError("Failed to find 'Token' relation of utterance")

    # create word relation
    word_rel = utt.relation_new("Word")

    for token_item in token_rel:

        # do some normalization with the token here...
        # ...
      
        # but for this example just set the token as the word. Token is name
        # feature of token item.
        token = token_item["name"]
        
	# create word item as daughter of token item, no shared content
        word_item = token_item.add_daughter()
        word_item["name"] = token

	# and create a new word item in word relation, shared
        # content is token relation's word item.
        word_rel.append(word_item)
