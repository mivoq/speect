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
