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
/* Python documentation strings for SPhoneset.                                      */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%define phoneset_DOCSTRING
"""
An phoneset class, for accessing a defined phoneset.
"""
%enddef

%feature("autodoc", phoneset_DOCSTRING) SPhoneset;


%define phoneset_name_DOCSTRING
"""
name()

Get the name of the phoneset.

:return: The phoneset name.
:rtype: string
"""
%enddef

%feature("autodoc", phoneset_name_DOCSTRING) SPhoneset::name;


%define phoneset_description_DOCSTRING
"""
description()

Get a description of the phoneset.

:return: The phoneset description.
:rtype: string
"""
%enddef

%feature("autodoc", phoneset_description_DOCSTRING) SPhoneset::description;


%define phoneset_language_DOCSTRING
"""
language()

Get the phoneset language.

:return: The phoneset language.
:rtype: string
"""
%enddef

%feature("autodoc", phoneset_language_DOCSTRING) SPhoneset::language;


%define phoneset_lang_code_DOCSTRING
"""
lang_code()

Get the phoneset's ``ISO 639-2`` language code.

:return: The ``ISO 639-2`` language code of the phoneset.
:rtype: string
"""
%enddef

%feature("autodoc", phoneset_lang_code_DOCSTRING) SPhoneset::lang_code;


%define phoneset_version_DOCSTRING
"""
version()

Get the phoneset's version.

:return: The version of the phoneset (major, minor).
:rtype: tuple
"""
%enddef

%feature("autodoc", phoneset_version_DOCSTRING) SPhoneset::version;


%define phoneset_phone_has_feature_DOCSTRING
"""
phone_has_feature(phone, feature)

Query if the phone in the phoneset has the named feature. The
phones in the phoneset have binary features, i.e. a phone
either has a specific named feature or it doesn't.

:param phone: The phone for which the feature presence is queried.
:type phone: string
:param feature: The feature for which the query is done.
:type feature: string
:return: ``True`` if the feature is defined for the given phone, else ``False``.
         If the given phone is not in the phoneset then ``False`` is returned.
:rtype: bool
"""
%enddef

%feature("autodoc", phoneset_phone_has_feature_DOCSTRING) SPhoneset::phone_has_feature;


%define phoneset_has_phone_DOCSTRING
"""
has_phone(phone)

Query if the phone is present in the phoneset.

:param phone: The phone for which the query is done.
:type phone: string
:return: ``True`` if the phone is defined in the phoneset , else ``False``.
:rtype: bool
"""
%enddef

%feature("autodoc", phoneset_has_phone_DOCSTRING) SPhoneset::has_phone;


%define phoneset_features_DOCSTRING
"""
Get the features that are defined for the phoneset.

:return: A map of the phoneset features.
:rtype: SMap
"""
%enddef

%feature("autodoc", phoneset_features_DOCSTRING) features;
