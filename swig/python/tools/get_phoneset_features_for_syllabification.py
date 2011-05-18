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
## DATE    : May 2011                                                               ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Get features from a phoneset that can be used in creating syllabification        ##
## rewrite rules. This is very basic and can/should be modified as seen fit         ##
## for different phonesets.                                                         ##
##                                                                                  ##
######################################################################################

import sys
import optparse
import speect
import speect.phoneset
import speect.phoneset_json


__author__     = "HLT - Meraka Institute CSIR"
__copyright__  = "Copyright 2011, " + __author__
__credits__    = [__author__]
__license__    = "mit"
__version__    = "0.1"

NAME = "get_phoneset_features_for_syllabification.py"


def phone_sonority_level(phoneset, phone):
    if phoneset.phone_has_feature(phone, "vowel"):
        if phoneset.phone_has_feature(phone, "height_low"):
            return 9
        elif phoneset.phone_has_feature(phone, "height_mid"):
            return 8
        elif phoneset.phone_has_feature(phone, "height_high"):
            return 7
        else:
            return 7 # diphtongs ?

    if phoneset.phone_has_feature(phone, "manner_liquid"):
        return 6

    if phoneset.phone_has_feature(phone, "manner_nasal"):
        return 5

    if phoneset.phone_has_feature(phone, "manner_fricative"):
        if phoneset.phone_has_feature(phone, "voiced"):
            return 4
        else:
            return 3

    if phoneset.phone_has_feature(phone, "manner_plosive"):
        if phoneset.phone_has_feature(phone, "voiced"):
            return 2
        else:
            return 1

    return 0


def test_phone_cluster(phoneset, cluster, cluster_name):
    clusterList = phoneset.features[cluster_name]
    for i in clusterList:
        if cluster == i:
            return True

    return False


def well_formed_onset_consonant_clusters(phoneset, c1, c2):
    cc = c1 + c2

    # "s" clusters 
    if c1 == "s":
        return test_phone_cluster(phoneset, cc, "wellformed_s_clusters")

    # plosive clusters
    if phoneset.phone_has_feature(c1, "manner_plosive"):
        return test_phone_cluster(phoneset, cc, "wellformed_plosive_clusters")

    # fricative clusters not /s/
    if phoneset.phone_has_feature(c1, "manner_fricative"):
        return test_phone_cluster(phoneset, cc, "wellformed_fricative_clusters")

    return False


def get_all_consonants(phoneset):
    phones = phoneset.get_phone_list()
    consonants = []
    for phone in phones:
        if phoneset.phone_has_feature(phone, "consonant"):
            consonants.append(phone)

    return consonants


def get_all_plosives(phoneset):
    phones = phoneset.get_phone_list()
    consonants = []
    for phone in phones:
        if phoneset.phone_has_feature(phone, "manner_plosive"):
            consonants.append(phone)

    return consonants


def get_unvoiced_of_selection(phoneset, selection):
    phones = selection
    unvoiced = []
    for phone in phones:
        if not phoneset.phone_has_feature(phone, "voiced"):
            unvoiced.append(phone)

    return unvoiced


def get_all_vowels(phoneset):
    phones = phoneset.get_phone_list()
    vowels = []
    for phone in phones:
        if phoneset.phone_has_feature(phone, "vowel"):
            vowels.append(phone)

    return vowels


def get_all_glides(phoneset):
    phones = phoneset.get_phone_list()
    glides = []
    for phone in phones:
        if phoneset.phone_has_feature(phone, "manner_glide"):
            glides.append(phone)

    return glides


def get_all_phones(phoneset):
    phones = phoneset.get_phone_list()
    all_phones = []
    for phone in phones:
        all_phones.append(phone)
    return all_phones


def phone_is_obstrudent(phoneset, phone):
    present_son = phoneset.phone_has_feature(phone, "class_sonorant")
    present_syl = phoneset.phone_has_feature(phone, "class_syllabic")
    present_cons = phoneset.phone_has_feature(phone, "class_consonantal")

    if (not present_son and not present_syl and present_cons):
        return True

    return False


def get_all_obstrudents(phoneset):
    phones = phoneset.get_phone_list()
    obstrudents = []
    for phone in phones:
        if phone_is_obstrudent(phoneset, phone):
            obstrudents.append(phone)
            
    return obstrudents


def get_all_nasals(phoneset):
    phones = phoneset.get_phone_list()
    nasals = []
    for phone in phones:
        if phoneset.phone_has_feature(phone, "manner_nasal"):
            nasals.append(phone)
            
    return nasals


def get_all_liquids(phoneset):
    phones = phoneset.get_phone_list()
    liquids = []
    for phone in phones:
        if phoneset.phone_has_feature(phone, "manner_trill"):
            liquids.append(phone)
            continue
        
        if phoneset.phone_has_feature(phone, "manner_flap"):
            liquids.append(phone)
            continue
        
        t3 = phoneset.phone_has_feature(phone, "manner_approximant")
        t4 = phoneset.phone_has_feature(phone, "manner_liquid")
        if (t3 and t4):
            liquids.append(phone)

    return liquids


def get_all_CC_variations(consonants):
    cc_consonants = []
    for c1 in consonants:
        for c2 in consonants:
            cc = list()
            cc.append(c1)
            cc.append(c2)
            cc_consonants.append(cc)

    return cc_consonants


def get_all_well_formed_onset_clusters(phoneset, cc_consonants):
    cc_consonants_permissible = []
    for cc in cc_consonants:
        if well_formed_onset_consonant_clusters(phoneset, cc[0], cc[1]):
            cc_consonants_permissible.append(cc)

    return cc_consonants_permissible


def get_all_CC_sonority_rise(phoneset, cc_consonants):
    cc_consonants_sonority_rise = []
    for cc in cc_consonants:
        slc1 = phone_sonority_level(phoneset, cc[0])
        slc2 = phone_sonority_level(phoneset, cc[1])
        if (slc1 < slc2):
            cc_consonants_sonority_rise.append(cc)

    return cc_consonants_sonority_rise


def get_all_CC_sonority_rise_equals(phoneset, cc_consonants):
    cc_consonants_sonority_re = []
    for cc in cc_consonants:
        slc1 = phone_sonority_level(phoneset, cc[0])
        slc2 = phone_sonority_level(phoneset, cc[1])
        if (slc1 <= slc2):
            cc_consonants_sonority_re.append(cc)

    return cc_consonants_sonority_re


def get_all_CC_sonority_equals(phoneset, cc_consonants):
    cc_consonants_sonority_e = []
    for cc in cc_consonants:
        slc1 = phone_sonority_level(phoneset, cc[0])
        slc2 = phone_sonority_level(phoneset, cc[1])
        if (slc1 == slc2):
            cc_consonants_sonority_e.append(cc)

    return cc_consonants_sonority_e


def get_all_CC_sonority_falls(phoneset, cc_consonants):
    cc_consonants_sonority_fall = []
    for cc in cc_consonants:
        slc1 = phone_sonority_level(phoneset, cc[0])
        slc2 = phone_sonority_level(phoneset, cc[1])
        if (slc1 > slc2):
            cc_consonants_sonority_fall.append(cc)

    return cc_consonants_sonority_fall


def get_all_NOT_well_formed_onset_clusters(phoneset, cc_consonants):
    cc_consonants_NOT_permissible = []
    for cc in cc_consonants:
        if not well_formed_onset_consonant_clusters(phoneset, cc[0], cc[1]):
            cc_consonants_NOT_permissible.append(cc)

    return cc_consonants_NOT_permissible


# modify this as required
def do_test(phonesetfile):

    # load phoneset
    phoneset = speect.phoneset.SPhoneset.load_json(phonesetfile)
    vowels = get_all_vowels(phoneset)
    consonants = get_all_consonants(phoneset)
    all_phones = get_all_phones(phoneset)
    glides = get_all_glides(phoneset)
    obstrudents = get_all_obstrudents(phoneset)
    nasals = get_all_nasals(phoneset)
    liquids = get_all_liquids(phoneset)
    plosives = get_all_plosives(phoneset)
    cc_consonants = get_all_CC_variations(consonants)
    well_formed_cc_consonants = get_all_well_formed_onset_clusters(phoneset, cc_consonants)
    well_formed_onset_sonority_rise = get_all_CC_sonority_rise(phoneset, well_formed_cc_consonants)
    not_well_formed_sonority_rise_equal = get_all_CC_sonority_rise_equals(phoneset, cc_consonants)
    cc_sonority_fall = get_all_CC_sonority_falls(phoneset, cc_consonants)
    not_well_formed_sonority_fall = get_all_NOT_well_formed_onset_clusters(phoneset, cc_sonority_fall)
    cc_sonority_rise = get_all_CC_sonority_rise(phoneset, cc_consonants)
    not_well_formed_sonority_rise = get_all_NOT_well_formed_onset_clusters(phoneset, cc_sonority_rise)
    cc_sonority_equal = get_all_CC_sonority_equals(phoneset, cc_consonants)
    not_well_formed_sonority_equal = get_all_NOT_well_formed_onset_clusters(phoneset, cc_sonority_equal)
    unvoiced_plosives = get_unvoiced_of_selection(phoneset, plosives)

    
    ## phones = phoneset.get_phone_list()
    ## phones.sort()

    ## for i in phones:
    ##     print "[\"\", \"%s\", \"\", \"%s\"]," % i

   
    #for i in tmp:
    #    print "[\"\", \"V %s\", \"%s V\", \"V %s -\"]," % (i[0], i[1], i[0])


    ## mydict = {}
    ## for i in well_formed_cc_consonants:
    ##     if not i[0] in mydict:
    ##         l = []
    ##         l.append(i[1])
    ##         mydict[i[0]] = l
    ##     else:
    ##         mydict[i[0]].append(i[1])

    ## index = []
    ## for i in mydict:
    ##     mydict[i].sort()
    ##     index.append(i)


    ## index.sort()
    ## for i in index:
    ##     string = "\"%s_OWF\" : " % i.capitalize()
    ##     print  string + str(mydict[i]) + ","

    


def setopts():
    """ Setup all possible command line options....
    """
    usage = 'USAGE: %s [options]' % (NAME)
    version = NAME + " " + __version__
    parser = optparse.OptionParser(usage=usage, version=version)
    parser.add_option("-p",
                      "--phonesetfile",
                      dest="phonesetfile",
                      help="Phoneset to extract features from.")
    return parser


if __name__ == "__main__":
    parser = setopts()

    if (len(sys.argv) < 2):
        parser.print_help()
        parser.error("you must specify a phonesetfile")
    opts, args = parser.parse_args()
    do_test(opts.phonesetfile)
