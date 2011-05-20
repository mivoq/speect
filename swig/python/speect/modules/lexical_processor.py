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
## A simple lexical processer that implements the grapheme to phoneme conversion    ##
## and inserts this information into the utterance structure                        ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

import speect
import speect.uttproc_cb   # utterance callback interface
import speect.lexicon
import speect.addendum


def get_lexical_objects(utt):
    voice = utt.voice
    if voice == None:
        raise RuntimeError("Failed to find 'voice' of utterance")

    lexicon = voice.data_get("lexicon")
    addendum = voice.data_get("addendum")
    g2p = voice.data_get("g2p")
    syllab = voice.data_get("syllabification")

    # now check for least requirements 
    if (lexicon is None) and (addendum is None) and (g2p is None):
        raise RuntimeError("No grapheme to phoneme conversion options (lexicon, addendum, g2p) found in voice")
    
    return lexicon, addendum, g2p, syllab


def utt_processor(utt):
    # check that we have "Word" relation
    word_rel = utt.relation_get("Word")
    if word_rel is None:
        raise RuntimeError("Failed to find 'Word' relation of utterance")

    # get the lexical objects
    lexicon, addendum, g2p, syllab = get_lexical_objects(utt)

    # create relations
    syllable_rel = utt.relation_new("Syllable")
    syl_struct_rel = utt.relation_new("SylStructure")
    segment_rel = utt.relation_new("Segment")

    # start at the first item in the word relation,
    # iterate over the word relation and fill in the
    # phones and the associated structure.
    for word_item in word_rel:
        phones = None
        syllabified = False

        # actual word
        word = word_item["name"]

        # get phone sequence for word 
        if addendum is not None:
            phones, syllabified = addendum.get_word(word)

        if (phones is None) and (lexicon is not None):
            phones, syllabified = lexicon.get_word(word)

        if phones is None:
            phones = g2p.apply(word)

        if phones is None:
            res = "Failed to get phone sequence for word '" + word + "'"
            raise RuntimeError(res)

        if not syllabified:
            if syllab is not None:
                syllablesPhones = syllab.syllabify(word_item, phones)
            else:
                syllablesPhones = list()
                syllablesPhones.append(phones)
        else:
            syllablesPhones = phones

        # create new syllable structure word item, shares content
        # with word item.
        syl_structure_word_item = syl_struct_rel.append(word_item)

        # iterate over syllables 
        for syls in syllablesPhones:
            
            # new item in syllable relation 
            syllable_item = syllable_rel.append()
            syllable_item["name"] = "syl"

            # daughter of above item, but in SylStructure
            syl_struct_syl_item = syl_structure_word_item.add_daughter(syllable_item)

            # iterate over phones and add segments
            for phon in syls:

                # create new segment with phone
                segment_item = segment_rel.append()
                segment_item["name"] = phon

                # and add as daughter in syllable structure
                syl_struct_seg_item = syl_struct_syl_item.add_daughter(segment_item)
            
   
