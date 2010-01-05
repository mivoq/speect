######################################################################################
## Copyright (c) 2009 The Department of Arts and Culture,                           ##
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
## DATE    : December 2009                                                          ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Example of saving an utterance in EBML format.                                   ##
##                                                                                  ##
##                                                                                  ##
######################################################################################

from __future__ import print_function
import speect
import utt_ebml


def runme():
    # create the utterance
    utt = speect.SUtterance()

    # crate a "Word" relation
    relWord = speect.SRelation()

    # connect it to the utterance
    utt.relations["Word"] = relWord

    # append an item to the Word relation
    item = relWord.append()

    # give the item a name
    item.features["name"] = speect.SString("toplevel")

    # create a daughter item
    idaughter = item.add_daughter()

    # and give it a name
    idaughter.features["name"] = speect.SString("lowerlevel")

    # create a Syllable relation,
    # note that this is a different way of doing it
    utt.relations["Syllable"] = speect.SRelation()

    # get a handle to the Syllable relation
    relSyll = utt.relations["Syllable"]

    # append an item to the Syllable relation,
    # which shares it's content with the daughter
    # item in the Word relation
    item = relSyll.append(to_share=idaughter)

    # print the utterance 
    print(utt)

    # and save it to "py_ebml.utt"
    utt.save_ebml("py_ebml.utt")


# main function
if __name__ == '__main__':
    runme()
    speect.py_speect_quit()
