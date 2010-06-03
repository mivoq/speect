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
/* DATE    : January 2009                                                           */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SPhoneset Python wrapper.                            */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%define phoneset_DOCSTRING
"""
A phoneset class. The phones in the phoneset have binary features,
i.e. a phone either has a specific named feature or it doesn't.
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
:return: True or False
:rtype: bool
"""
%enddef

%feature("autodoc", phoneset_phone_has_feature_DOCSTRING) SPhoneset::phone_has_feature;


%define phoneset_has_phone_DOCSTRING
"""
has_phone(phone)


:param phone: The phone for which the query is done.
:type phone: string
:return: True or False
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


typedef struct
{
	SMap *features;
} SPhoneset;

%nodefaultctor SPhoneset;

%types(SPhoneset = SObject);

%extend SPhoneset
{
	const char *name(s_erc *error)
	{
		const char *name;


		name = S_PHONESET_CALL($self, get_name)($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return name;
	}


	const char *description(s_erc *error)
	{
		const char *description;


		description = S_PHONESET_CALL($self, get_description)($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return description;
	}


	const char *language(s_erc *error)
	{
		const char *language;


		language = S_PHONESET_CALL($self, get_language)($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return language;
	}


	const char *lang_code(s_erc *error)
	{
		const char *lang_code;


		lang_code = S_PHONESET_CALL($self, get_lang_code)($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return lang_code;
	}


	s_version *version(s_erc *error)
	{
		return (s_version*)S_PHONESET_CALL($self, get_version)($self, error);
	}


	s_bool phone_has_feature(const char *phone, const char *feature, s_erc *error)
	{
		s_bool has_feature;


		has_feature = S_PHONESET_CALL($self, phone_has_feature)($self, phone, feature, error);
		if (*error != S_SUCCESS)
			return FALSE;

		return has_feature;
	}


	s_bool has_phone(const char *phone, s_erc *error)
	{
		s_bool has_phone;


		has_phone = S_PHONESET_CALL($self, has_phone)($self, phone, error);
		if (*error != S_SUCCESS)
			return FALSE;

		return has_phone;
	}
}

