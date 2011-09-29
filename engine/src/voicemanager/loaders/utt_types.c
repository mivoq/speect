/************************************************************************************/
/* Copyright (c) 2008-2011 The Department of Arts and Culture,                      */
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
/* DATE    : 29 October 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A loader for the voice utterance types in the voice configuration file.          */
/* This is (currently) an exact copy of the features loader in features.c,          */
/* with just the names that are changed.                                            */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "voicemanager/loaders/utt_types.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SMap *_s_get_voice_utterance_types(const SMap *voiceConfig, s_erc *error)
{
	SMap *uttTypes;
	const SObject *tmp;
	const SMap *voiceConfigUttTypes;
	s_bool key_present;


	S_CLR_ERR(error);

	/* create a map container for utterance types */
	uttTypes = S_MAP(S_NEW(SMapList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_get_voice_utterance_types",
				  "Failed to create new map for voice utterance types"))
		return NULL;

	/* look for "utterance-types" key in voiceConfig map */
	key_present = SMapObjectPresent(voiceConfig, "utterance-types", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_get_voice_utterance_types",
				  "Call to \"SMapObjectPresent\" failed for \'utterance-types\' key"))
		return NULL;

	if (!key_present)
	{
		/* no defined utterance types */
		return uttTypes;
	}

	/* get utterance types from voiceConfig */
	tmp = SMapGetObject(voiceConfig, "utterance-types", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_get_voice_utterance_types",
				  "Call to \"SMapGetObject\" failed"))
	{
		S_DELETE(uttTypes, "_s_get_voice_utterance_types", error);
		return NULL;
	}


	voiceConfigUttTypes = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_get_voice_utterance_types",
				  "Call to \"S_CAST (SMap)\" failed for \'utterance-types\' object"))
	{
		S_DELETE(uttTypes, "_s_get_voice_utterance_types", error);
		return NULL;
	}

	/* make a copy and return the copy */
	SMapCopy(uttTypes, voiceConfigUttTypes, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_get_voice_utterance_types",
				  "Call to \"SMapCopy\" failed"))
	{
		S_DELETE(uttTypes, "_s_get_voice_utterance_types", error);
		return NULL;
	}

	return uttTypes;
}
