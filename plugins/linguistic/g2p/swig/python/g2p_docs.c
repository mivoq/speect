/************************************************************************************/
/* Copyright (c) 2010 The Department of Arts and Culture,                           */
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
/* Python documentation strings for SG2P.                                           */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%define g2p_DOCSTRING
"""
An g2p (grapheme-to-phoneme) class.
"""
%enddef

%feature("autodoc", g2p_DOCSTRING) SG2P;


%define g2p_name_DOCSTRING
"""
name()

Get the name of the g2p.

:return: The g2p name.
:rtype: string
"""
%enddef

%feature("autodoc", g2p_name_DOCSTRING) SG2P::name;


%define g2p_description_DOCSTRING
"""
description()

Get a description of the g2p.

:return: The g2p description.
:rtype: string
"""
%enddef

%feature("autodoc", g2p_description_DOCSTRING) SG2P::description;


%define g2p_language_DOCSTRING
"""
language()

Get the g2p language.

:return: The g2p language.
:rtype: string
"""
%enddef

%feature("autodoc", g2p_language_DOCSTRING) SG2P::language;


%define g2p_lang_code_DOCSTRING
"""
lang_code()

Get the g2p's ``ISO 639-2`` language code.

:return: The ``ISO 639-2`` language code of the g2p.
:rtype: string
"""
%enddef

%feature("autodoc", g2p_lang_code_DOCSTRING) SG2P::lang_code;


%define g2p_version_DOCSTRING
"""
version()

Get the g2p's version.

:return: The version of the g2p (major, minor).
:rtype: tuple
"""
%enddef

%feature("autodoc", g2p_version_DOCSTRING) SG2P::version;


%define g2p_apply_DOCSTRING
"""
apply(word)

Apply the g2p to the given word to get a phone list.

:param word: The word to convert to a phone list.
:type word: string
:return: A list of phones for the given word or ``None`` if the
	     g2p failed. If the g2p failed a warning will be set in the
	     error log.
:rtype: list
"""
%enddef

%feature("autodoc", g2p_apply_DOCSTRING) SG2P::apply;


%define g2p_apply_at_DOCSTRING
"""
apply_at(word, index)

Apply the g2p to the grapheme at given index in the given word
to get a phone. This method can be seen as a single step of
the ``SG2P::apply`` method. It returns the phone of the grapheme at
the index.


:param word: The word to get a phone from at the given grapheme index.
:type word: string
:param index: The index of the grapheme, in the word, for which
	          the phone is requested.
:return: The phone of the grapheme at the index in the word.
:rtype: string
:note: Indexing starts at 0.
:note: The returned phone can be ``None``, this is not necessarily an error.
"""
%enddef

%feature("autodoc", g2p_apply_at_DOCSTRING) SG2P::apply_at;


%define g2p_features_DOCSTRING
"""
Get the features that are defined for the g2p.

:return: A map of the g2p features.
:rtype: SMap
"""
%enddef

%feature("autodoc", g2p_features_DOCSTRING) features;
