/************************************************************************************/
/* Copyright (c) 2015 Mivoq SRL <info@mivoq.it>                                     */
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                Matteo Lisotto                                                    */
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
/* AUTHOR  : Matteo Lisotto <matteo.lisotto@gmail.com>                              */
/* DATE    : 21 September 2015                                                      */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SUtterance HTS labels exporter class Python wrapper. */
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
from . import speect

def save_utt_htslabelsexport(self, path):
    """
    save_htslabels(path)

    Save the utterance's HTS labels to a file in the XML format.

    :param path: Full path and file name of the file where the utterance
                 should be saved to.
    :type path: string
    """
    if not isinstance(path, str):
        raise TypeError("Argument \"path\" must be a string")

    return _save_utterance_htslabelsexport(self, path)


# add the functions to the Speect SUtterance class
setattr(speect.SUtterance, "save_htslabels", save_utt_utt_htslabelsexport)
%}
