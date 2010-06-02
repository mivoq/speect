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
/* DATE    : November 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SUtterance Python wrapper.                           */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%define voice_ctor_DOCSTRING
"""
SVoice(path[, load_data=TRUE])

Load a voice, from the voice configuration at the given path. If any
of the voice's data objects have already been loaded, then they are shared.
This reduces the need for multiple redundant calls.

:param path: The full path and name of the voice to load.
:type path: string
:param load_data: This flag currently defaults to ``TRUE`` and should not be used.
:type load_data: bool
:return: Voice object
:rtype: SVoice
:raises: RuntimeError if Speect was unable to load the voice.
"""
%enddef

%feature("autodoc", voice_ctor_DOCSTRING) SVoice;


%define voice_synth_DOCSTRING
"""
synth(input[, utt_type='text'])


Synthesize an utterance of the given utterance type with the voice.

:param input: The text to synthesize.
:type input: string.
:param utt_type: The utterance type to synthesize.
:type utt_type: string.
:return: The synthesized utterance.
:rtype: SUtterance
"""
%enddef

%feature("autodoc", voice_synth_DOCSTRING) SVoice::synth;


%define voice_name_DOCSTRING
"""
name()

Get the voice's name.

:return: The name of the voice.
:rtype: string
"""
%enddef

%feature("autodoc", voice_name_DOCSTRING) SVoice::name;


%define voice_description_DOCSTRING
"""
description()

Get the voice's description.

:return: The description of the voice.
:rtype: string
"""
%enddef

%feature("autodoc", voice_description_DOCSTRING) SVoice::description;


%define voice_gender_DOCSTRING
"""
gender()

Get the voice's gender.

:return: The gender of the voice.
:rtype: string
"""
%enddef

%feature("autodoc", voice_gender_DOCSTRING) SVoice::gender;


%define voice_language_DOCSTRING
"""
language()

Get the voice's language.

:return: The language of the voice.
:rtype: string
"""
%enddef

%feature("autodoc", voice_language_DOCSTRING) SVoice::language;


%define voice_lang_code_DOCSTRING
"""
lang_code()

Get the voice's ``ISO 639-2`` language code.

:return: The ``ISO 639-2`` language code of the voice.
:rtype: string
"""
%enddef

%feature("autodoc", voice_lang_code_DOCSTRING) SVoice::lang_code;


%define voice_version_DOCSTRING
"""
version()

Get the voice's version.

:return: The version of the voice (major, minor).
:rtype: tuple
"""
%enddef

%feature("autodoc", voice_version_DOCSTRING) SVoice::version;


%define voice_data_get_DOCSTRING
"""
data_get(key)

Return the voice data object reference by the given key.

:param key: The key of the data object as referenced in the voice.
:type key: string
:return: The data object or ``None`` if no such object exists.
"""
%enddef

%feature("autodoc", voice_data_get_DOCSTRING) SVoice::data_get;


%define voice_data_set_DOCSTRING
"""
data_set(key, object)

Set a data object, with the given key, in the voice.

:param key: The key of the data object.
:type key: string
:param object: A Swig Object of type SObject (or an object that inherits from ``SObject`` in the Speect Engine).
:type object: Swig Object of type SObject
:note: If the given key references a data object present in the voice, then that data object will be deleted.
"""
%enddef

%feature("autodoc", voice_data_set_DOCSTRING) SVoice::data_set;


%define voice_data_del_DOCSTRING
"""
data_del(key)

Delete the voice data object referenced by the given key.

:param key: The key of the data object.
:type key: string
"""
%enddef

%feature("autodoc", voice_data_del_DOCSTRING) SVoice::data_del;


%define voice_uttType_get_DOCSTRING
"""
uttType_get(key)

Return the voice utterance type definition reference by the given key.

:param key: The key of the utterance type definition as referenced in the voice.
:type key: string
:return: The utterance type definition or ``None`` if no such definition exists.
:rtype: list
"""
%enddef

%feature("autodoc", voice_uttType_get_DOCSTRING) SVoice::uttType_get;


%define voice_uttType_set_DOCSTRING
"""
uttType_set(key, uttType)

Set an utterance type definition, with the given key, in the voice.

:param key: The key of the utterance type.
:type key: string
:param uttType: The utterance type definition.
:type uttType: list
"""
%enddef

%feature("autodoc", voice_uttType_set_DOCSTRING) SVoice::uttType_set;


%define voice_uttType_del_DOCSTRING
"""
uttType_del(key)

Delete the voice utterance type definition referenced by the given key.

:param key: The key of the utterance type definition.
:type key: string
"""
%enddef

%feature("autodoc", voice_uttType_del_DOCSTRING) SVoice::uttType_del;


%define voice_uttProcessor_get_DOCSTRING
"""
uttProcessor_get(key)

Return the voice utterance processor reference by the given key.

:param key: The key of the utterance processor as referenced in the voice.
:type key: string
:return: The utterance processor or ``None`` if no such definition exists.
:rtype: SUttProcessor
"""
%enddef

%feature("autodoc", voice_uttProcessor_get_DOCSTRING) SVoice::uttProcessor_get;


%define voice_uttProcessor_set_DOCSTRING
"""
uttProcessor_set(key, uttProc)

Set an utterance processor, with the given key, in the voice.

:param key: The key of the utterance processor.
:type key: string
:param uttProc: The utterance processor.
:type uttProc: SUttProcessor
"""
%enddef

%feature("autodoc", voice_uttProcessor_set_DOCSTRING) SVoice::uttProcessor_set;


%define voice_uttProcessor_del_DOCSTRING
"""
uttProcessor_del(key)

Delete the voice utterance processor referenced by the given key.

:param key: The key of the utterance processor.
:type key: string
"""
%enddef

%feature("autodoc", voice_uttProcessor_del_DOCSTRING) SVoice::uttProcessor_del;


%define voice_featProcessor_get_DOCSTRING
"""
featProcessor_get(key)

Return the voice feature processor reference by the given key.

:param key: The key of the feature processor as referenced in the voice.
:type key: string
:return: The feature processor or ``None`` if no such definition exists.
:rtype: SFeatProcessor
"""
%enddef

%feature("autodoc", voice_featProcessor_get_DOCSTRING) SVoice::featProcessor_get;


%define voice_featProcessor_set_DOCSTRING
"""
featProcessor_set(key, featProc)

Set a feature processor, with the given key, in the voice.

:param key: The key of the feature processor.
:type key: string
:param featProc: The feature processor.
:type featProc: SFeatProcessor
"""
%enddef

%feature("autodoc", voice_featProcessor_set_DOCSTRING) SVoice::featProcessor_set;


%define voice_featProcessor_del_DOCSTRING
"""
featProcessor_del(key)

Delete the voice feature processor referenced by the given key.

:param key: The key of the feature processor.
:type key: string
"""
%enddef

%feature("autodoc", voice_featProcessor_del_DOCSTRING) SVoice::featProcessor_del;


%define voice_uttTypes_DOCSTRING
"""
Get a list of keys of the utterance types that are defined for the voice.

:return: A list of keys of the utterance types defined for the voice.
:rtype: list
"""
%enddef

%feature("autodoc", voice_uttTypes_DOCSTRING) uttTypes;


%define voice_uttProcs_DOCSTRING
"""
Get a list of keys of the utterance processors that are defined for the voice.

:return: A list of keys of the utterance processors defined for the voice.
:rtype: list
"""
%enddef

%feature("autodoc", voice_uttProcs_DOCSTRING) uttProcessors;


%define voice_dataObjects_DOCSTRING
"""
Get a list of keys of the data objects that are defined for the voice.

:return: A list of keys of the data objects defined for the voice.
:rtype: list
"""
%enddef

%feature("autodoc", voice_dataObjects_DOCSTRING) dataObjects;


%define voice_featProcessors_DOCSTRING
"""
Get a list of keys of the feature processors that are defined for the voice.

:return: A list of keys of the feature processors defined for the voice.
:rtype: list
"""
%enddef

%feature("autodoc", voice_featProcessors_DOCSTRING) featProcessors;


%define voice_features_DOCSTRING
"""
Get the features that are defined for the voice.

:return: A map of the voice features.
:rtype: SMap
"""
%enddef

%feature("autodoc", voice_features_DOCSTRING) features;



typedef struct
{
	SMap *features;
	%extend
	{
		SList *uttTypes;
		SList *uttProcessors;
		SList *dataObjects;
		SList *featProcessors;
	}
} SVoice;


%types(SVoice = SObject);

%newobject synth;


%extend SVoice
{
	SVoice(const char *path, s_bool load_data=TRUE, s_erc *error)
	{
		SVoice *voice;

		voice = s_vm_load_voice(path, load_data, error);
		if (*error != S_SUCCESS)
			return NULL;

		return voice;
	}


	~SVoice()
	{
		s_erc error;

		S_CLR_ERR(&error);
		S_DELETE($self, "~SVoice", &error);
	}


	SUtterance *synth(const char *input, const char *utt_type="text", s_erc *error)
	{
		SUtterance *utt;
		SObject *inputObject;


		inputObject = SObjectSetString(input, error);
		if (*error != S_SUCCESS)
			return NULL;

		utt = SVoiceSynthUtt($self, utt_type, inputObject, error);
		if (*error != S_SUCCESS)
		{
			if (utt != NULL)
				S_DELETE(utt, "SVoice::synth", error);
			return NULL;
		}

		return utt;
	}

	const char *name(s_erc *error)
	{
		return SVoiceGetName($self, error);
	}


	const char *description(s_erc *error)
	{
		return SVoiceGetDescription($self, error);
	}


	const char *gender(s_erc *error)
	{
		return SVoiceGetGender($self, error);
	}


	const char *language(s_erc *error)
	{
		return SVoiceGetLanguage($self, error);
	}


	const char *lang_code(s_erc *error)
	{
		return SVoiceGetLangCode($self, error);
	}


	s_version *version(s_erc *error)
	{
		return (s_version*)SVoiceGetVersion($self, error);
	}


	PyObject *data_get(const char *key, s_erc *error)
	{
		const SObject *sdata;
		PyObject *pdata;



		sdata = SVoiceGetData($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		pdata = sobject_2_pyobject(sdata, error, FALSE);
		if (*error != S_SUCCESS)
			return NULL;

		return pdata;
	}


	void data_set(const char *key, const SObject *object,  s_erc *error)
	{
		SVoiceSetData($self, key, object, error);
	}


	void data_del(const char *key, s_erc *error)
	{
		SVoiceDelData($self, key, error);
	}


	const SList *uttType_get(const char *key, s_erc *error)
	{
		const SList *uttType;


		uttType = SVoiceGetUttType($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		return uttType;
	}


	void uttType_set(const char *key, const SList *uttType, s_erc *error)
	{
		SVoiceSetUttType($self, key, uttType, error);
	}


	void uttType_del(const char *key, s_erc *error)
	{
		SVoiceDelUttType($self, key, error);
	}


	const SUttProcessor *uttProcessor_get(const char *key, s_erc *error)
	{
		const SUttProcessor *uttProc;


		uttProc = SVoiceGetUttProc($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		return uttProc;
	}


	void uttProcessor_set(const char *key, const SUttProcessor *uttProc, s_erc *error)
	{
		SVoiceSetUttProc($self, key, uttProc, error);
	}


	void uttProcessor_del(const char *key, s_erc *error)
	{
		SVoiceDelUttProc($self, key, error);
	}


	const SFeatProcessor *featProcessor_get(const char *key, s_erc *error)
	{
		const SFeatProcessor *featProc;


		featProc = SVoiceGetFeatProc($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		return featProc;
	}


	void featProcessor_set(const char *key, const SFeatProcessor *featProc, s_erc *error)
	{
		SVoiceSetFeatProc($self, key, featProc, error);
	}


	void featProcessor_del(const char *key, s_erc *error)
	{
		SVoiceDelFeatProc($self, key, error);
	}
};


%{
	SList *SVoice_uttTypes_get(const SVoice *voice)
	{
		s_erc error;
		SList *uttTypes;


		S_CLR_ERR(&error);
		uttTypes = SVoiceGetUttTypesKeys(voice, &error);
		if (error != S_SUCCESS)
			return NULL;

		return uttTypes;
	}


	void SVoice_uttTypes_set(const SVoice *voice, SList *list)
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE(list, "SVoice_uttTypes_set", &error);
	}


	SList *SVoice_uttProcessors_get(const SVoice *voice)
	{
		s_erc error;
		SList *uttProcessors;


		S_CLR_ERR(&error);
		uttProcessors = SVoiceGetUttProcKeys(voice, &error);
		if (error != S_SUCCESS)
			return NULL;

		return uttProcessors;
	}


	void SVoice_uttProcessors_set(const SVoice *voice, SList *list)
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE(list, "SVoice_uttProcessors_set", &error);
	}


	SList *SVoice_dataObjects_get(const SVoice *voice)
	{
		s_erc error;
		SList *data;


		S_CLR_ERR(&error);
		data = SVoiceGetDataKeys(voice, &error);
		if (error != S_SUCCESS)
			return NULL;

		return data;
	}


	void SVoice_dataObjects_set(const SVoice *voice, SList *list)
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE(list, "SVoice_dataObjects_set", &error);
	}


	SList *SVoice_featProcessors_get(const SVoice *voice)
	{
		s_erc error;
		SList *featProcessors;


		S_CLR_ERR(&error);
		featProcessors = SVoiceGetFeatProcKeys(voice, &error);
		if (error != S_SUCCESS)
			return NULL;

		return featProcessors;
	}


	void SVoice_featProcessors_set(const SVoice *voice, SList *list)
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE(list, "SVoice_featProcessors_set", &error);
	}
%}
