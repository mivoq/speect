#!/usr/bin/env python
######################################################################################
## Copyright (c) 2011 The Department of Arts and Culture,                           ##
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
## DATE    : October 2011                                                           ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## HRG example. Creating the utterance structure as given in the HRG                ##
## topic guide in the documentation.                                                ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


lexical_info = {
    "twenty": [ "t", "w", "eh", "n", "1", "t", "iy", "0"],
    "fifth" : [  "f", "@", "th", "1" ]
    }
    

import speect
import speect.utt_ebml # load the utt-ebml plug-in


# create a new utterance
utt = speect.SUtterance()

# create a new "Word" relation
word_rel = utt.relation_new("Word")

# create a new "Syllable" relation
syl_rel = utt.relation_new("Syllable")

# create a new "Segment" relation
segment_rel = utt.relation_new("Segment")

# create a new "SylStructure" relation
sylstruct_rel = utt.relation_new("SylStructure")

for word in lexical_info:

    # add word in word relation
    word_item = word_rel.append()

    # set it's name feature
    word_item["name"] = word

    # add word in syllable structure relation,
    # note the shared content word_item
    sylstruct_word_item = sylstruct_rel.append(word_item)
    
    # create a syllable item in the syllable relation
    syl_item = syl_rel.append()

    # syllable in syllable structure relation is daughter of
    # word item in syllable structure relation, note
    # shared content with syllable item
    sylstruct_syl_item = sylstruct_word_item.add_daughter(syl_item)
    
    phone_counter = 0

    for phone in lexical_info[word]:
        if not phone.isdigit():
            # create a segment item in the segment relation
            seg_item = segment_rel.append()
            seg_item["name"] = phone

            # segment item is daughter of syllable item in
            # syllable structure relation
            sylstruct_syl_item.add_daughter(seg_item)

        else:
            syl_item["stress"] = int(phone)

            if phone_counter < len(lexical_info[word]) - 1:
                # create a syllable item in the syllable relation
                syl_item = syl_rel.append()

                # syllable in syllable structure relation is daughter of
                # word item in syllable structure relation, note
                # shared content with syllable item
                sylstruct_syl_item = sylstruct_word_item.add_daughter(syl_item)

        phone_counter += 1
    

# print utterance
print(utt)

# save utterance
utt.save_ebml("test.utt")

del utt

