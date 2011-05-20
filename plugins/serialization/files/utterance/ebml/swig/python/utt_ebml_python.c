/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                                                                                  */
/* Permission is hereby granted, free of charge, to any person obtaining a copy     */
/* of this software and associated documentation files (the "Software"), to deal    */
/* in the Software without restriction, including without limitation the rights     */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        */
/* copies of the Software, and to permit persons to whom the Software is            */
/* furnished to do so, subject to the following conditions:                         */
/* The above copyright notice and this permission notice shall be included in       */
/* all copies or substantial portions of the Software.                              */
/*                                                                                  */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN        */
/* THE SOFTWARE.                                                                    */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* AUTHOR  : Aby Louw                                                               */
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SUtterance EBML class Python wrapper.                */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Extend the SUtterance class                                                      */
/*                                                                                  */
/************************************************************************************/

%pythoncode
%{
import speect

def load_utt_ebml(path):
    """
    load_ebml(path)

    Load an utterance from a file in the EBML format.

    :param path: Full path and file name of the file where the utterance
                 should be loaded from.
    :type path: string
    :rtype: ``SUtterance``
    :note: All the features in the utterance (including in the relations and items) must
           have appropriate EBML format serialization formatters registered.
    """
    if not isinstance(path, str):
        raise TypeError("Argument \"path\" must be a string")

    return _load_utterance_ebml(path)


def save_utt_ebml(self, path):
    """
    save_ebml(path)

    Save the utterance to a file in the EBML format.

    :param path: Full path and file name of the file where the utterance
                 should be saved to.
    :type path: string
    :note: All the features in the utterance (including in the relations and items) must
           have appropriate EBML format serialization formatters registered.
    """
    if not isinstance(path, str):
        raise TypeError("Argument \"path\" must be a string")

    return _save_utterance_ebml(self, path)


# add the functions to the Speect SUtterance class
setattr(speect.SUtterance, "load_ebml", staticmethod(load_utt_ebml))
setattr(speect.SUtterance, "save_ebml", save_utt_ebml)
%}
