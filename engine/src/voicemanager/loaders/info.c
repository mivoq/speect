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
/* A loader for the voice information in the voice configuration file.              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/strings/utf8.h"
#include "base/utils/alloc.h"
#include "voicemanager/loaders/info.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_voice_info *create_voice_info(s_erc *error);

static void delete_voice_info(s_voice_info *voice_info);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL s_voice_info *_s_parse_voice_info(const SMap *voiceConfig, s_erc *error)
{
	const char *tmp_string;
	const SMap *info;
	const SObject *tmp;
	s_bool key_present;
	s_voice_info *voice_info;


	S_CLR_ERR(error);

	/* look for "voice-definition" key in voiceConfig map */
	key_present = SMapObjectPresent(voiceConfig, "voice-definition", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_parse_voice_info",
				  "Call to \"SMapObjectPresent\" failed for \'info\' key"))
		return NULL;

	if (!key_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "_s_parse_voice_info",
				  "Voice configuration does not have an \'voice-definition\' entry");
		return NULL;
	}

	tmp = SMapGetObject(voiceConfig, "voice-definition", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_parse_voice_info",
				  "Call to \"SMapGetObject\" failed for \'info\' key"))
		return NULL;

	info = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_parse_voice_info",
				  "Call to \"S_CAST (SMap)\" failed for \'info\' object"))
		return NULL;

	voice_info = create_voice_info(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_parse_voice_info",
				  "Call to \"create_voice_info\" failed"))
		return NULL;

	/* look for "name" key in info map */
	key_present = SMapObjectPresent(info, "name", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_parse_voice_info",
				  "Call to \"SMapObjectPresent\" failed for \'name\' key"))
	{
		delete_voice_info(voice_info);
		return NULL;
	}

	if (key_present)
	{
		tmp_string = SMapGetString(info, "name", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"SMapGetString\" failed for \'name\' key"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}

		S_FREE(voice_info->name); /* free "undefined" */
		voice_info->name = s_strdup(tmp_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"s_strdup\" failed for \'name\' key"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}
	}

	/* look for "description" key in info map */
	key_present = SMapObjectPresent(info, "description", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_parse_voice_info",
				  "Call to \"SMapObjectPresent\" failed for \'description\' key"))
	{
		delete_voice_info(voice_info);
		return NULL;
	}

	if (key_present)
	{
		tmp_string = SMapGetString(info, "description", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"SMapGetString\" failed for \'description\' key"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}

		S_FREE(voice_info->description); /* free "undefined" */
		voice_info->description = s_strdup(tmp_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"s_strdup\" failed for \'description\' key"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}
	}

	/* look for "gender" key in info map */
	key_present = SMapObjectPresent(info, "gender", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_parse_voice_info",
				  "Call to \"SMapObjectPresent\" failed for \'gender\' key"))
	{
		delete_voice_info(voice_info);
		return NULL;
	}

	if (key_present)
	{
		tmp_string = SMapGetString(info, "gender", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"SMapGetString\" failed for \'gender\' key"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}

		S_FREE(voice_info->gender); /* free "undefined" */
		voice_info->gender = s_strdup(tmp_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"s_strdup\" failed for \'gender\' key"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}
	}

	/* look for "language" key in info map */
	key_present = SMapObjectPresent(info, "language", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_parse_voice_info",
				  "Call to \"SMapObjectPresent\" failed for \'language\' key"))
	{
		delete_voice_info(voice_info);
		return NULL;
	}

	if (key_present)
	{
		tmp_string = SMapGetString(info, "language", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"SMapGetString\" failed for \'language\' key"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}

		S_FREE(voice_info->language); /* free "undefined" */
		voice_info->language = s_strdup(tmp_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"s_strdup\" failed for \'language\' key"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}
	}

	/* look for "lang_code" key in info map */
	key_present = SMapObjectPresent(info, "lang_code", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_parse_voice_info",
				  "Call to \"SMapObjectPresent\" failed for \'lang_code\' key"))
	{
		delete_voice_info(voice_info);
		return NULL;
	}

	if (key_present)
	{
		tmp_string = SMapGetString(info, "lang_code", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"SMapGetString\" failed for \'lang_code\' key"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}

		S_FREE(voice_info->lang_code); /* free "undefined" */
		voice_info->lang_code = s_strdup(tmp_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"s_strdup\" failed for \'lang_code\' key"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}
	}

	/* look for "version" key in info map */
	key_present = SMapObjectPresent(info, "version", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_parse_voice_info",
				  "Call to \"SMapObjectPresent\" failed for \'version\' key"))
	{
		delete_voice_info(voice_info);
		return NULL;
	}

	if (key_present)
	{
		const SMap *versionMap;

		tmp = SMapGetObject(info, "version", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"SMapGetObject\" failed for \'version\' key"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}

		versionMap = S_CAST(tmp, SMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"S_CAST (SMap)\" failed for \'version\' object"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}

		/* get major */
		voice_info->version.major = (uint8)SMapGetInt(versionMap, "major", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"SMapGetInt\" failed for \'version.major\' object"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}

		/* get minor */
		voice_info->version.minor = (uint8)SMapGetInt(versionMap, "minor", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_parse_voice_info",
					  "Call to \"SMapGetInt\" failed for \'version.minor\' object"))
		{
			delete_voice_info(voice_info);
			return NULL;
		}
	}

	return voice_info;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_voice_info *create_voice_info(s_erc *error)
{
	s_voice_info *voice_info;


	S_CLR_ERR(error);

	voice_info = S_MALLOC(s_voice_info, 1);

	if (voice_info == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "create_voice_info",
				  "Failed to allocate memory for \'s_voice_info\' object");
		return NULL;
	}

	voice_info->name = NULL;
	voice_info->description = NULL;
	voice_info->gender = NULL;
	voice_info->language = NULL;
	voice_info->lang_code = NULL;

	/* add defaults */
	voice_info->name = s_strdup("undefined", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_voice_info",
				  "Call to \"s_strdup\" failed"))
	{
		delete_voice_info(voice_info);
		return NULL;
	}

	voice_info->description = s_strdup("undefined", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_voice_info",
				  "Call to \"s_strdup\" failed"))
	{
		delete_voice_info(voice_info);
		return NULL;
	}

	voice_info->gender = s_strdup("undefined", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_voice_info",
				  "Call to \"s_strdup\" failed"))
	{
		delete_voice_info(voice_info);
		return NULL;
	}

	voice_info->language = s_strdup("undefined", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_voice_info",
				  "Call to \"s_strdup\" failed"))
	{
		delete_voice_info(voice_info);
		return NULL;
	}

	voice_info->lang_code = s_strdup("undefined", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_voice_info",
				  "Call to \"s_strdup\" failed"))
	{
		delete_voice_info(voice_info);
		return NULL;
	}

	voice_info->version.major = 0;
	voice_info->version.minor = 0;

	return voice_info;
}


static void delete_voice_info(s_voice_info *voice_info)
{
	if (voice_info == NULL)
		return;

	if (voice_info->name != NULL)
		S_FREE(voice_info->name);

	if (voice_info->description != NULL)
		S_FREE(voice_info->description);

	if (voice_info->gender != NULL)
		S_FREE(voice_info->gender);

	if (voice_info->language != NULL)
		S_FREE(voice_info->language);

	if (voice_info->lang_code != NULL)
		S_FREE(voice_info->lang_code);

	S_FREE(voice_info);
}

