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
} SVoice;


%types(SVoice = SObject);

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
		S_DELETE($self, "~SVoice()", &error);
	}


	SUtterance *synth(const char *utt_type, const char *input, s_erc *error)
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


	PyObject *get_data(const char *key, s_erc *error)
	{
		const SObject *sdata;
		PyObject *pdata;



		sdata = SVoiceGetData($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		pdata = sobject_2_pyobject(sdata, error);
		if (*error != S_SUCCESS)
			return NULL;

		return pdata;
	}
};
