/************************************************************************************/
/* Copyright (c) 2011 The Department of Arts and Culture,                           */
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
/* DATE    : May 2011                                                               */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Python documentation strings for SSyllabification.                               */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%define syllabification_DOCSTRING
"""
A syllabification class.
"""
%enddef

%feature("autodoc", syllabification_DOCSTRING) SSYLLABIFICATION;


%define syllabification_name_DOCSTRING
"""
name()

Get the name of the syllabification.

:return: The syllabification name.
:rtype: string
"""
%enddef

%feature("autodoc", syllabification_name_DOCSTRING) SSYLLABIFICATION::name;


%define syllabification_description_DOCSTRING
"""
description()

Get a description of the syllabification.

:return: The syllabification description.
:rtype: string
"""
%enddef

%feature("autodoc", syllabification_description_DOCSTRING) SSYLLABIFICATION::description;


%define syllabification_language_DOCSTRING
"""
language()

Get the syllabification language.

:return: The syllabification language.
:rtype: string
"""
%enddef

%feature("autodoc", syllabification_language_DOCSTRING) SSYLLABIFICATION::language;


%define syllabification_lang_code_DOCSTRING
"""
lang_code()

Get the syllabification's ``ISO 639-2`` language code.

:return: The ``ISO 639-2`` language code of the syllabification.
:rtype: string
"""
%enddef

%feature("autodoc", syllabification_lang_code_DOCSTRING) SSYLLABIFICATION::lang_code;


%define syllabification_version_DOCSTRING
"""
version()

Get the syllabification's version.

:return: The version of the syllabification (major, minor).
:rtype: tuple
"""
%enddef

%feature("autodoc", syllabification_version_DOCSTRING) SSYLLABIFICATION::version;


%define syllabification_features_DOCSTRING
"""
Get the features that are defined for the syllabification.

:return: A map of the syllabification features.
:rtype: SMap
"""
%enddef

%feature("autodoc", syllabification_features_DOCSTRING) features;
