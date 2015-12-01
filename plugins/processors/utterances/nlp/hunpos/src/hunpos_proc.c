/************************************************************************************/
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                Simone Daminato                                                   */
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
/* DATE    : May 2012                                                               */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A part-of-speech tagger utterance processor using hunpos.                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "hunpos_proc.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

/* default values for hunpos params */
#define SPCT_DEF_MAX_GUESSED_TAGS 3
#define SPCT_DEF_THETA 1000
#define SPCT_DEF_MAX_TAG_LENGTH 100


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SHunposUttProc class declaration. */
static SHunposUttProcClass HunposUttProcClass;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void load_hunpos_data(const SMap *data, SHunposUttProc *hunposProc,
							 const char *voice_base_path, s_erc *error);

static void clear_hunpos_data(SHunposUttProc *hunposProc, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_hunpos_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&HunposUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_hunpos_utt_proc_class_reg",
			  "Failed to register SHunposUttProcClass");
}


S_LOCAL void _s_hunpos_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&HunposUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_hunpos_utt_proc_class_free",
			  "Failed to free SHunposUttProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/


static void load_hunpos_data(const SMap *data, SHunposUttProc *hunposProc,
							 const char *voice_base_path, s_erc *error)
{
	const SObject *tmp;
	const char *mf_data = NULL;


	S_CLR_ERR(error);


	/* max guessed tags */
	hunposProc->max_guessed_tags = SMapGetIntDef(data, "max guessed tags",
												 SPCT_DEF_MAX_GUESSED_TAGS, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_hunpos_data",
				  "Call to \"SMapGetIntDef\" failed"))
		return;

	/* theta */
	hunposProc->theta = SMapGetIntDef(data, "theta",
									  SPCT_DEF_THETA, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_hunpos_data",
				  "Call to \"SMapGetIntDef\" failed"))
		return;

	/* model file */
	tmp = SMapGetObjectDef(data, "model file", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_hunpos_data",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (tmp == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "load_hunpos_data",
				  "Failed to find 'model file' hunpos data entry");
		return;
	}

	mf_data = SObjectGetString(tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_hunpos_data",
				  "Call to \"SObjectGetString\" failed"))
		return;

	hunposProc->model_file = s_path_combine(voice_base_path, mf_data,
											error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_hunpos_data",
				  "Call to \"s_path_combine\" failed"))
		return;
}


static void clear_hunpos_data(SHunposUttProc *hunposProc, s_erc *error)
{
	S_CLR_ERR(error);

	if (hunposProc->model_file != NULL)
		S_FREE(hunposProc->model_file);

	if (hunposProc->hunpos_instance != NULL)
		hunpos_tagger_destroy(hunposProc->hunpos_instance, error);
		hunposProc->hunpos_instance = NULL;
		if (S_CHK_ERR(error, S_CONTERR,
				  "clear_hunpos_data",
				  "Call to \"hunpos_tagger_destroy\" failed"))
			return;
}


static const char* read_token(void* data, int n)
{
	s_erc error;
	SItem** nodeList = data;
	return SItemGetName(nodeList[n], &error);
}

static int set_tag(void* data, int n, const char * tag)
{
	s_erc error;
	SItem** nodeList = data;
	SItemSetString(nodeList[n], "POS", tag, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
		"set_tag",
	       "Call to \"SItemSetString\" failed"))
		return 1;
	return 0;
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/


static void Init(void *obj, s_erc *error)
{
	SHunposUttProc *self = obj;


	S_CLR_ERR(error);
	self->model_file = NULL;
	self->hunpos_instance = NULL;
}


static void Destroy(void *obj, s_erc *error)
{
	SHunposUttProc *self = obj;


	S_CLR_ERR(error);
	clear_hunpos_data(self, error);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void Initialize(SUttProcessor *self, const SVoice *voice, s_erc *error)
{
	SHunposUttProc *hunposProc = (SHunposUttProc*)self;
	const SMap *hunpos_data;
	const SObject *vcfgObject;
	char *voice_base_path;

	S_CLR_ERR(error);

	/* get voice base path */
	vcfgObject = SVoiceGetFeature(voice, "config_file", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SVoiceGetFeature\" failed, failed to get voice config file"))
		return;

	voice_base_path = s_get_base_path(SObjectGetString(vcfgObject, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"s_get_base_path/SObjectGetString\" failed"))
		return;

	/* get the hunpos data map */
	hunpos_data = S_MAP(SVoiceGetFeature(voice, "hunpos data", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SVoiceGetFeature\" failed"))
		goto quit_error;

	if (hunpos_data == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Initialize",
				  "Failed to get \"hunpos data\" map from voice features");
		goto quit_error;
	}

	load_hunpos_data(hunpos_data, hunposProc, voice_base_path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"load_hunpos_data\" failed"))
		goto quit_error;


	/* create the hunpos instance */
	hunposProc->hunpos_instance = hunpos_tagger_new( hunposProc->model_file, NULL, hunposProc->max_guessed_tags, hunposProc->theta, error );
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"hunpos_tagger_new\" failed"))
		goto quit_error;

	/* all OK */
	S_FREE(voice_base_path);
	return;

	/* error clean up */
quit_error:
	clear_hunpos_data(hunposProc, error);
	if (voice_base_path != NULL)
		S_FREE(voice_base_path);
}


static void Run(const SUttProcessor *self, SUtterance *utt,
				s_erc *error)
{
	SHunposUttProc *hunposProc = (SHunposUttProc*)self;
	const SRelation *tokenRel;
	s_bool is_present;
	const SItem *tokenItem;
	const SItem **dataList;
	int tokenCount;
	int i = 0;


	S_CLR_ERR(error);

	/* we require the token relation */
	is_present = SUtteranceRelationIsPresent(utt, "Token", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto quit_error;

	if (!is_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Token' relation in utterance");
		goto quit_error;
	}

	tokenRel = SUtteranceGetRelation(utt, "Token", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
		goto quit_error;

	/* Get the tokens.
	 * start at the first item in the token relation
	 */
	tokenItem = SRelationHead(tokenRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	/* count tokens */
	while (tokenItem != NULL)
	{
		tokenCount++;
		tokenItem = SItemNext(tokenItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	/* alloc the needed structure */
	dataList = S_MALLOC(SItem*, tokenCount);
	/* restart from the begin */
	tokenItem = SRelationHead(tokenRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
		"Run",
	       "Call to \"SRelationHead\" failed"))
		goto quit_error;
	while (tokenItem != NULL)
	{
		dataList[i] = tokenItem;
		tokenItem = SItemNext(tokenItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
		i++;
	}

	/* tag the data */
	int errors;
	hunpos_tagger_tag(hunposProc->hunpos_instance, tokenCount, dataList, &read_token, dataList, &set_tag, &errors);

	S_FREE(dataList);

	/* here all is OK */
	return;

	/* error clean-up code */
quit_error:
	if (dataList != NULL)
		S_FREE(dataList);
}


/************************************************************************************/
/*                                                                                  */
/* SHunposUttProc class initialization                                              */
/*                                                                                  */
/************************************************************************************/

static SHunposUttProcClass HunposUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:SHunposUttProc",
		sizeof(SHunposUttProc),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SUttProcessorClass */
	Initialize,          /* initialize */
	Run                  /* run        */
};

