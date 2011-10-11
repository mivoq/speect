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
## A simple pause insertion processer that inserts pauses, into an utterance, based ##
## on the phrase breaks                                                             ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

import speect
import speect.uttproc_cb   # utterance callback interface
import speect.phoneset


def utt_processor(utt):
    # we require the word, segment and phrase relation 
    word_rel = utt.relation_get("Word")
    if word_rel is None:
        raise RuntimeError("Failed to find 'Word' relation of utterance")

    segment_rel = utt.relation_get("Segment")
    if segment_rel is None:
        raise RuntimeError("Failed to find 'Segment' relation of utterance")

    phrase_rel = utt.relation_get("Phrase")
    if phrase_rel is None:
        raise RuntimeError("Failed to find 'Phrase' relation of utterance")

    # get phoneset from voice
    voice = utt.voice
    if voice is None:
        raise RuntimeError("Failed to find 'voice' of utterance")
   
    phoneset = voice.data_get("phoneset")
    if phoneset is None:
        raise RuntimeError("Failed to find 'phoneset' of voice")
   
    # get silence phone
    silence_phone = phoneset.features["silence_phone"]
 
    # Insert initial silence 
    segment_item = segment_rel.head()
    if segment_item is None:
        silence_item = segment_rel.append()
    else:
        silence_item = segment_item.prepend()

    silence_item["name"] = silence_phone

    # now go through phrases
    for phrase_item in phrase_rel:
        word_item = phrase_item.daughter(-1)  # -1 for last daughter

        while word_item is not None:
            segment_item = word_item.path_to_item("R:SylStructure.daughtern.daughtern.R:Segment")
            
            if segment_item is not None:
                silence_item = segment_item.append()
                silence_item["name"] = silence_phone
                break
            
            word_item = word_item.prev()

            
    
