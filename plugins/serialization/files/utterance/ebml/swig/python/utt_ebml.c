/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* C convenience functions for saving/loading utterances in EBML format.            */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%define sutterance_ebml_save_DOCSTRING
"""
save_ebml(path)

Save the utterance to a file in the EBML format.

:param path: Full path and file name of the file where the utterance should be saved to.
:type path: string
:note: All the features in the utterance (including in the relations and items) must
       have appropriate EBML format serialization formatters registered.
"""
%enddef

%feature("autodoc", sutterance_ebml_save_DOCSTRING) sutterance_ebml_save;


%define sutterance_ebml_load_DOCSTRING
"""
load_ebml(path)

Load the utterance from a file in the EBML format.

:param path: Full path and file name of the file where the utterance should be loaded from.
:type path: string
:note: All the features in the utterance (including in the relations and items) must
       have appropriate EBML format serialization formatters registered.
"""
%enddef

%feature("autodoc", sutterance_ebml_load_DOCSTRING) sutterance_ebml_load;


/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	void sutterance_ebml_save(SUtterance *utt, const char *path, s_erc *error)
	{
		SObjectSave(S_OBJECT(utt), path, "spct_utt", error);
	}


	SUtterance *sutterance_ebml_load(const char *path, s_erc *error)
	{
		SObject *loadedUtt;

		loadedUtt = SObjectLoad(path, "spct_utt", error);
		if (*error != S_SUCCESS)
			return NULL;

		return S_UTTERANCE(loadedUtt);
	}
/*
 * Do not delete this delimiter, required for SWIG
 */
%}


%pythoncode
%{
# add the functions to the Speect SUtterance class
setattr(speect.SUtterance, "load_ebml", staticmethod(sutterance_ebml_load))
setattr(speect.SUtterance, "save_ebml", sutterance_ebml_save)
%}
