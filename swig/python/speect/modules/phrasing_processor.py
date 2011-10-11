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
## AUTHOR  : Aby Louw                                                               ##
## DATE    : October 2010                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## A simple phrasing processer that breaks an utterance into phrases based solely   ##
## on punctuation information                                                       ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

import speect
import speect.uttproc_cb   # utterance callback interface

end_punctuation = ['?', '.', ',', ':', ';']


def utt_processor(utt):
    # check that we have "Word" relation
    word_rel = utt.relation_get("Word")
    if word_rel is None:
        raise RuntimeError("Failed to find 'Word' relation of utterance")

    # create phrase relation
    phrase_rel = utt.relation_new("Phrase")

    phrase_item = None
    for word_item in word_rel:

        if phrase_item == None:
            # if phrase item is None, create a new phrase item (no
            # shared content) that is appended to phrase
            # relation. Will happen in first pass.
            phrase_item = phrase_rel.append()

        # Create a daughter item for the phrase item. Shared content
        # is the word item.
	phrase_item.add_daughter(word_item)

        # get word's token:
        #      first get word as in token relation,
        #      then get it's parent which is the token.
        token_item = word_item.as_relation("Token").parent()

	# get token's post punctuation 
        if "postpunc" in token_item:
            post_punc = token_item["postpunc"]

            # check if it is in the end_punctuation list 
            if post_punc in end_punctuation:
                # add a phrase break
                phrase_item["name"] = "BB"

                # set to None so that a new phrase item is created 
                phrase_item = None
