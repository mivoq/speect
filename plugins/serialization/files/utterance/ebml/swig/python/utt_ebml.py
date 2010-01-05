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
## Python functions for loading/saving utterances in EBML format.                   ##
##                                                                                  ##
##                                                                                  ##
######################################################################################



%pythoncode
%{
def load_ebml_utterance(path):
    """
    Load an EBML format utterance from the given file name.
    @param path: The full path and filename of the utterance to load.
    @type path: str
    @return: The loaded utterance.
    @rtype: L{SUtterance}
    """

    if not isinstance(path, str):
        raise TypeError('path must be of type \'str\'')

    utt_object = py_sutterance_ebml_load(path)
    if not utt_object:
        string = "Speect failed to load utterance of type \'EBML\' from file \'" +\
                 path + "\'"
        raise RuntimeError(string)

    return speect.SUtterance(object=utt_object, owner=True)


def save_ebml_utterance(self, path):
    """
    Save an EBML format utterance to the given file name.
    @param path: The full path and filename where to save the utterance.
    @type path: str
    """

    if not isinstance(path, str):
        raise TypeError('path must be of type \'str\'')

    utt_object = self._get_speect_object()
    if not utt_object:
        raise RuntimeError("Utterance has no Speect object")

    py_sutterance_ebml_save(utt_object, path)
    
# add the functions to the Speect SUtterance class
setattr(speect.SUtterance, "load_ebml", staticmethod(load_ebml_utterance))
setattr(speect.SUtterance, "save_ebml", save_ebml_utterance)
%}
