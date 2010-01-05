/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* A loader for the voice data *configuration* in the voice configuration file.     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "voicemanager/loaders/data_config.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SMap *_s_load_voice_data_config(const SMap *voiceConfig, s_erc *error)
{
	SMap *data;
	const SObject *tmp;
	const SMap *voiceConfigData;
	s_bool key_present;


	S_CLR_ERR(error);

	/* create a map container for the data */
	data = S_MAP(S_NEW("SMapList", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_data",
				  "Failed to create new map for voice data"))
		return NULL;

	SMapListInit(&data, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_data",
				  "Failed to initialize new map for voice data"))
		return NULL;

	/* look for "data" key in voiceConfig map */
	key_present = SMapObjectPresent(voiceConfig, "data", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_data",
				  "Call to \"SMapObjectPresent\" failed for \'data\' key"))
	{
		S_DELETE(data, "_s_load_voice_data", error);
		return NULL;
	}

	if (!key_present)
	{
		/* no defined data  */
		return data;
	}

	/* get data from voiceConfig */
	tmp = SMapGetObject(voiceConfig, "data", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_data",
				  "Call to \"SMapGetObject\" failed"))
	{
		S_DELETE(data, "_s_load_voice_data", error);
		return NULL;
	}

	voiceConfigData = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_data",
				  "Call to \"S_CAST (SMap)\" failed for \'data\' object"))
	{
		S_DELETE(data, "_s_load_voice_data", error);
		return NULL;
	}

	/* make a copy and return the copy */
	SMapCopy(data, voiceConfigData, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_data",
				  "Call to \"SMapCopy\" failed"))
	{
		S_DELETE(data, "_s_load_voice_data", error);
		return NULL;
	}

	return data;
}



