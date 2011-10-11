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
/* Python documentation strings for SAddendum.                                      */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%define addendum_DOCSTRING
"""
An addendum class, for accessing a defined addendum.
"""
%enddef

%feature("autodoc", addendum_DOCSTRING) SAddendum;


%define addendum_name_DOCSTRING
"""
name()

Get the name of the addendum.

:return: The addendum name.
:rtype: string
"""
%enddef

%feature("autodoc", addendum_name_DOCSTRING) SAddendum::name;


%define addendum_description_DOCSTRING
"""
description()

Get a description of the addendum.

:return: The addendum description.
:rtype: string
"""
%enddef

%feature("autodoc", addendum_description_DOCSTRING) SAddendum::description;


%define addendum_language_DOCSTRING
"""
language()

Get the addendum language.

:return: The addendum language.
:rtype: string
"""
%enddef

%feature("autodoc", addendum_language_DOCSTRING) SAddendum::language;


%define addendum_lang_code_DOCSTRING
"""
lang_code()

Get the addendum's ``ISO 639-2`` language code.

:return: The ``ISO 639-2`` language code of the addendum.
:rtype: string
"""
%enddef

%feature("autodoc", addendum_lang_code_DOCSTRING) SAddendum::lang_code;


%define addendum_version_DOCSTRING
"""
version()

Get the addendum's version.

:return: The version of the addendum (major, minor).
:rtype: tuple
"""
%enddef

%feature("autodoc", addendum_version_DOCSTRING) SAddendum::version;


%define addendum_add_word_DOCSTRING
"""
add_word(word, features)

Add a word to the addendum.

:param word: The word to add.
:type word: string
:param features: Specific features of the word. Features are used to distinguish
                 homographs (i.e. part-of-speech). At least a \"phones\" or a \"syllables\"
                 feature must exists in the given ``features`` dictionary.
:type features: dictionary
:note: The addendum takes hold a the ``features`` object.
:note: The added entry is not saved in the addendum file (not persistent).
"""
%enddef

%feature("autodoc", addendum_add_word_DOCSTRING) SAddendum::add_word;


%define addendum_features_DOCSTRING
"""
Get the features that are defined for the addendum.

:return: A map of the addendum features.
:rtype: SMap
"""
%enddef

%feature("autodoc", addendum_features_DOCSTRING) features;
