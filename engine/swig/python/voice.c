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
/* C convenience functions for SVoice Python wrapper.                               */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	SObject *py_svoice_load(const char *path, s_bool load_data)
	{
		s_erc rv = S_SUCCESS;
		SVoice *voice;


		voice = s_vm_load_voice(path, load_data, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to load SVoice");
			return NULL;
		}

		return S_OBJECT(voice);
	}


	const SObject *py_svoice_features(SObject *self)
	{
		SVoice *voice = S_VOICE(self);


		if (voice == NULL)
			return NULL;

		return (const SObject*)voice->features;
	}


	const SObject *py_svoice_featprocs(SObject *self)
	{
		SVoice *voice = S_VOICE(self);


		if (voice == NULL)
			return NULL;

		return (const SObject*)voice->featProcessors;
	}


	const SObject *py_svoice_uttprocs(SObject *self)
	{
		SVoice *voice = S_VOICE(self);


		if (voice == NULL)
			return NULL;

		return (const SObject*)voice->uttProcessors;
	}


	const SObject *py_svoice_utttypes(SObject *self)
	{
		SVoice *voice = S_VOICE(self);


		if (voice == NULL)
			return NULL;

		return (const SObject*)voice->uttTypes;
	}


	SObject *py_svoice_data(SObject *self)
	{
		SVoice *voice = S_VOICE(self);
		SMap *dataConfig;
		s_erc rv = S_SUCCESS;

		if (voice == NULL)
			return NULL;

		dataConfig = SVoiceGetDataConfig(voice, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get the voice data config");
			return NULL;
		}

		return S_OBJECT(dataConfig);
	}


	void py_svoice_set_feature(SObject *self, const char *featname, const SObject *feat)
	{
		s_erc rv = S_SUCCESS;


		SVoiceSetFeature(S_VOICE(self), featname, feat, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to set voice feature");
	}


	const SObject *py_svoice_get_feature(const SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;
		const SObject *feat;


		feat = SVoiceGetFeature(S_VOICE(self), name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get voice feature");
			return NULL;
		}

		return feat;
	}


	void py_svoice_del_feature(const SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;


		SVoiceDelFeature(S_VOICE(self), name, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to delete voice feature");
	}


	void py_svoice_set_featproc(SObject *self, const char *featname, const SObject *featProc)
	{
		s_erc rv = S_SUCCESS;
		s_bool type_ok;


		type_ok = SObjectIsType(featProc, "SFeatProcessor", &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to check feature processor object type");
			return;
		}

		if (!type_ok)
		{
			PyErr_SetString(PyExc_RuntimeError, "Given feature processor object not of type 'SFeatProcessor'");
			return;
		}

		SVoiceSetFeatProc(S_VOICE(self), featname, S_FEATPROCESSOR(featProc), &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to set voice feature processor");
	}


	const SObject *py_svoice_get_featproc(const SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;
		const SFeatProcessor *featProc;


		featProc = SVoiceGetFeatProc(S_VOICE(self), name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get voice feature processor");
			return NULL;
		}

		return S_OBJECT(featProc);
	}


	void py_svoice_del_featproc(const SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;


		SVoiceDelFeatProc(S_VOICE(self), name, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to delete voice feature processor");
	}


	void py_svoice_set_uttproc(SObject *self, const char *featname, const SObject *uttProc)
	{
		s_erc rv = S_SUCCESS;
		s_bool type_ok;


		type_ok = SObjectIsType(uttProc, "SUttProcessor", &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to check utterance processor object type");
			return;
		}

		if (!type_ok)
		{
			PyErr_SetString(PyExc_RuntimeError, "Given utterance processor object not of type 'SUttProcessor'");
			return;
		}

		SVoiceSetUttProc(S_VOICE(self), featname, S_UTTPROCESSOR(uttProc), &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to set voice utterance processor");
	}


	const SObject *py_svoice_get_uttproc(const SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;
		const SUttProcessor *uttProc;


		uttProc = SVoiceGetUttProc(S_VOICE(self), name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get voice utterance processor");
			return NULL;
		}

		return S_OBJECT(uttProc);
	}


	void py_svoice_del_uttproc(const SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;


		SVoiceDelUttProc(S_VOICE(self), name, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to delete voice utterance processor");
	}


	void py_svoice_set_utttype(SObject *self, const char *featname, const SObject *utttype)
	{
		s_erc rv = S_SUCCESS;
		s_bool type_ok;


		type_ok = SObjectIsType(utttype, "SList", &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to check utterance type object type");
			return;
		}

		if (!type_ok)
		{
			PyErr_SetString(PyExc_RuntimeError, "Given utterance type object not of type 'SList'");
			return;
		}

		SVoiceSetUttType(S_VOICE(self), featname, S_LIST(utttype), &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to set voice utterance type");
	}


	const SObject *py_svoice_get_utttype(const SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;
		const SList *utttype;


		utttype = SVoiceGetUttType(S_VOICE(self), name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get voice utterance type");
			return NULL;
		}

		return S_OBJECT(utttype);
	}


	void py_svoice_del_utttype(const SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;


		SVoiceDelUttType(S_VOICE(self), name, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to delete voice utterance type");
	}


	void py_svoice_set_data(SObject *self, const char *featname, const SObject *data)
	{
		s_erc rv = S_SUCCESS;


		SVoiceSetData(S_VOICE(self), featname, data, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to set voice data");
			return;
		}
	}


	const SObject *py_svoice_get_data(const SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;
		const SObject *data;


		data = SVoiceGetData(S_VOICE(self), name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get voice data");
			return NULL;
		}

		return data;
	}


	void py_svoice_del_data(const SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;


		SVoiceDelData(S_VOICE(self), name, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to delete voice data");
	}


	const char *py_svoice_get_name(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		const char *name;


		name = SVoiceGetName(S_VOICE(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get voice name");
			return NULL;
		}

		return name;
	}


	const char *py_svoice_get_description(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		const char *description;


		description = SVoiceGetDescription(S_VOICE(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get voice description");
			return NULL;
		}

		return description;
	}


	const char *py_svoice_get_gender(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		const char *gender;


		gender = SVoiceGetGender(S_VOICE(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get voice gender");
			return NULL;
		}

		return gender;
	}


	const char *py_svoice_get_language(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		const char *language;


		language = SVoiceGetLanguage(S_VOICE(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get voice language");
			return NULL;
		}

		return language;
	}

	const char *py_svoice_get_lang_code(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		const char *lang_code;


		lang_code = SVoiceGetLangCode(S_VOICE(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get voice language code");
			return NULL;
		}

		return lang_code;
	}


	const s_version *py_svoice_get_version(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		const s_version *version;


		version = SVoiceGetVersion(S_VOICE(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get voice version");
			return NULL;
		}

		return version;
	}


	SObject *py_svoice_synth_utt(const SObject *self, const char *utt_type, SObject *input)
	{
		s_erc rv = S_SUCCESS;
		SUtterance *utt;

		utt = SVoiceSynthUtt(S_VOICE(self), utt_type, input, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get voice version");
			if (utt != NULL)
				S_DELETE(utt, "py_svoice_synth_utt", &rv);
			return NULL;
		}

		return S_OBJECT(utt);
	}
/*
 * Do not delete this delimiter, required for SWIG
 */
%}
