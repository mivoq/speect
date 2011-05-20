/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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
/* DATE    : June 2010                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Python documentation strings for SLexicon.                                       */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%define lexicon_DOCSTRING
"""
A lexicon class, for accessing a defined lexicon.
"""
%enddef

%feature("autodoc", lexicon_DOCSTRING) SLexicon;


%define lexicon_name_DOCSTRING
"""
name()

Get the name of the lexicon.

:return: The lexicon name.
:rtype: string
"""
%enddef

%feature("autodoc", lexicon_name_DOCSTRING) SLexicon::name;


%define lexicon_description_DOCSTRING
"""
description()

Get a description of the lexicon.

:return: The lexicon description.
:rtype: string
"""
%enddef

%feature("autodoc", lexicon_description_DOCSTRING) SLexicon::description;


%define lexicon_language_DOCSTRING
"""
language()

Get the lexicon language.

:return: The lexicon language.
:rtype: string
"""
%enddef

%feature("autodoc", lexicon_language_DOCSTRING) SLexicon::language;


%define lexicon_lang_code_DOCSTRING
"""
lang_code()

Get the lexicon's ``ISO 639-2`` language code.

:return: The ``ISO 639-2`` language code of the lexicon.
:rtype: string
"""
%enddef

%feature("autodoc", lexicon_lang_code_DOCSTRING) SLexicon::lang_code;


%define lexicon_version_DOCSTRING
"""
version()

Get the lexicon's version.

:return: The version of the lexicon (major, minor).
:rtype: tuple
"""
%enddef

%feature("autodoc", lexicon_version_DOCSTRING) SLexicon::version;


%define lexicon_features_DOCSTRING
"""
Get the features that are defined for the lexicon.

:return: A map of the lexicon features.
:rtype: SMap
"""
%enddef

%feature("autodoc", lexicon_features_DOCSTRING) features;
