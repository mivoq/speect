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
/* SWIG common C convenience functions for SVoice.                                  */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%newobject synth;


typedef struct
{
	SMap *features;
} SVoice;


%types(SVoice = SObject);


%extend SVoice
{
	/* SVoice() is in the language specific interfaces */

	~SVoice()
	{
		s_erc error;

		S_CLR_ERR(&error);
		S_DELETE($self, "SVoice::delete", &error);
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


	void resynth(SUtterance *utt, const char *utt_type, s_erc *error)
	{
		SVoiceReSynthUtt($self, utt_type, utt, error);
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


	void data_del(const char *key, s_erc *error)
	{
		SVoiceDelData($self, key, error);
	}



	void uttType_del(const char *key, s_erc *error)
	{
		SVoiceDelUttType($self, key, error);
	}


	void uttProcessor_set(const char *key, SUttProcessor *uttProc, s_erc *error)
	{
		SVoiceSetUttProc($self, key, uttProc, error);
		if (*error != S_SUCCESS)
			S_DELETE(uttProc, "SVoice::uttProcessor_set", error);
	}


	const SUttProcessor *uttProcessor_get(const char *key, s_erc *error)
	{
		const SUttProcessor *uttProc;


		uttProc = SVoiceGetUttProc($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		return uttProc;
	}


	void uttProcessor_del(const char *key, s_erc *error)
	{
		SVoiceDelUttProc($self, key, error);
	}


	void featProcessor_set(const char *key, SFeatProcessor *featProc, s_erc *error)
	{
		SVoiceSetFeatProc($self, key, featProc, error);
		if (*error != S_SUCCESS)
			S_DELETE(featProc, "SVoice::featProcessor_set", error);
	}


	const SFeatProcessor *featProcessor_get(const char *key, s_erc *error)
	{
		const SFeatProcessor *featProc;


		featProc = SVoiceGetFeatProc($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		return featProc;
	}


	void featProcessor_del(const char *key, s_erc *error)
	{
		SVoiceDelFeatProc($self, key, error);
	}
};
