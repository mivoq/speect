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
## A simple tokenizer that splits tokens on whitespace and adds pre- and            ##
## post-punctuation into a Token relation.                                          ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


# note that the function name must be "load_voice"
def load_voice():  
    import speect
    import speect.uttproc_cb   # utterance callback interface
  
    # load voice json file
    voice = speect.SVoice("voice.hts.json") # load voice definition
    
    # load and add g2p
    import speect.modules.g2p_rewrites_festival as g2p_rewrites
    import ibibio_g2p
    ig2p = g2p_rewrites.G2P_Rewrites_Festival(ibibio_g2p.rules, ibibio_g2p.sets)

    # set g2p in voice
    voice.data_set("g2p", ig2p)

    # load and add syllabification
    import speect.modules.syllab_rewrites as syllab_rewrites
    import ibibio_syll
    isyll = syllab_rewrites.Syllab_Rewrites(ibibio_syll.rules, ibibio_syll.sets)

    # set syllabification in voice
    voice.data_set("syllabification", isyll)
    
    #
    # Create utterance processors 
    #

    # create tokenizer
    import speect.modules.tokenize_processor as tokenize_processor
    tok_utt_proc = speect.SUttProcessor.callback(tokenize_processor.utt_processor)

    # create normalizer
    import speect.modules.normalize_processor as normalize_processor
    norm_utt_proc = speect.SUttProcessor.callback(normalize_processor.utt_processor)

    # create phrasing processor
    import speect.modules.phrasing_processor as phrasing_processor
    phrasing_utt_proc = speect.SUttProcessor.callback(phrasing_processor.utt_processor)

    # create lexical lookup processor
    import speect.modules.lexical_processor as lexical_processor
    lexlookup_utt_proc = speect.SUttProcessor.callback(lexical_processor.utt_processor)

    # create pause insertion processor
    import speect.modules.pause_processor as pause_processor
    pause_utt_proc = speect.SUttProcessor.callback(pause_processor.utt_processor)

    #
    # Replace voice definition file processors with above
    #
    voice.uttProcessor_set("Tokenize", tok_utt_proc)
    voice.uttProcessor_set("Normalize", norm_utt_proc)
    voice.uttProcessor_set("Phrasify", phrasing_utt_proc)
    voice.uttProcessor_set("LexLookup", lexlookup_utt_proc)
    voice.uttProcessor_set("Pauses", pause_utt_proc)

    return voice
  
