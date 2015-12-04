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
#define SPCT_DEF_MAX_TOKENS_NUMBER 100


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

	/* max tokens */
	hunposProc->max_tokens_number = SMapGetIntDef(data, "max_tokens_number",
												 SPCT_DEF_MAX_TOKENS_NUMBER, error);
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


static const char* read_token(void* data, int n, int* error)
{
	s_erc tmperror;
	SItem** nodeList = data;
	const char* name =  SItemGetName(nodeList[n], &tmperror);
	if (S_CHK_ERR(&tmperror, S_CONTERR,
			"read_token",
			"Call to \"SItemGetName\" failed"))
		*error = 1;
	return name;
}

static void set_tag(void* data, int n, const char * tag, int* error)
{
	s_erc tmperror;
	SItem** nodeList = data;
	SItemSetString(nodeList[n], "POS", tag, &tmperror);
	if (S_CHK_ERR(&tmperror, S_CONTERR,
			"set_tag",
			"Call to \"SItemSetString\" failed"))
		*error = 1;
}

/**
 * Prepare the structure for the next call to hunpos.
 * @private
 *
 * @param relation_head starting SItem for this new phrase. It must be a Phrase SItem if @p is_phrase_present is true, a Token otherwise.
 * @param data data structure to fill with new SItem pointers.
 * @param is_phrase_present tells if we're using phrases or directly tokens
 * @param error Error code.
 *
 */
static void call_hunpos(const SHunposUttProc *hunposProc, const SItem* relation_head, const SItem** data, s_bool is_phrase_present, s_erc *error)
{
	const SItem* phrase_start_item;
	const SItem* current_token;
	const SItem* start_token; /* start point of the next phrase */
	const SItem* last_safe_cut_token; /* last token from where we can cut for the next phrase */
	int last_safe_cut_count;
	int tokens_count;
	S_CLR_ERR(error);

	phrase_start_item = relation_head;

	/* we already have the starting item if we have only tokens, else grab the token */
	start_token = phrase_start_item;
	if (is_phrase_present)
	{
		/* go grab the first token of this phrase */
		start_token = SItemPathToItem(relation_head, "R:Phrase.daughter.R:Token.parent", error);
		if (S_CHK_ERR(error, S_CONTERR,
			"call_hunpos",
		"Call to \"SItemPathToItem\" failed"))
			return;
	}

	current_token = start_token;

	/* for each phrase */
	while (current_token != NULL)
	{
		tokens_count = 0;
		last_safe_cut_count = 0;
		start_token = current_token;
		last_safe_cut_token = current_token;

		/* we stop collecting tokens if the next item is null or if we reached the maximum number of tokens allowed */
		while (current_token != NULL && tokens_count < hunposProc->max_tokens_number)
		{
			data[tokens_count] = current_token;

			char* NAME = SItemGetName(current_token, error);

			/* check if it's a safe cut point */
			s_bool is_present = SItemFeatureIsPresent(current_token, "IsPunctuation", error);
			if (S_CHK_ERR(error, S_CONTERR,
				"call_hunpos",
				"Call to \"SItemFeatureIsPresent\" failed"))
				return;
			if (is_present)
			{
				/* is it a punctiation token? */
				sint32 is_punctuation = SItemGetInt(current_token, "IsPunctuation", error);
				if (is_punctuation > 0)
				{
					last_safe_cut_token = current_token;
					last_safe_cut_count = tokens_count + 1;
				}
			}

			current_token = SItemNext(current_token, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "call_hunpos",
					  "Call to \"SItemNext\" failed"))
				return;

			/* if we're using phrases, check if the current phrase finished */
			if (is_phrase_present)
			{
				if (current_token != NULL)
				{
					const SItem* next_token_parent = SItemPathToItem(current_token, "R:Token.daughter.R:Phrase.parent", error);
					if (S_CHK_ERR(error, S_CONTERR,
								"call_hunpos",
								"Call to \"SItemPathToItem\" failed"))
						return;
					if (next_token_parent != phrase_start_item)
					{
						/* save the next phrase start and stop the cycle*/
						phrase_start_item = next_token_parent;
						start_token = current_token;
						current_token = NULL;
					}
				}
				else
				{
					phrase_start_item = NULL;
					start_token = NULL;
				}
			}

			tokens_count++;
		}

		/* do we need to cut it? */
		if (current_token != NULL && last_safe_cut_token != start_token)
		{
			tokens_count = last_safe_cut_count;
			current_token = SItemNext(last_safe_cut_token, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "call_hunpos",
					  "Call to \"SItemNext\" failed"))
				return;
		}

		/* do the tagging */
		int hunpos_error = 0;
		hunpos_tagger_tag(hunposProc->hunpos_instance, tokens_count, data, &read_token, data, &set_tag, &hunpos_error);
		if (hunpos_error !=0)
		{
			S_CTX_ERR(error, S_FAILURE,
				  "call_hunpos",
				  "Call to \"hunpos_tagger_tag\" failed");
			return;
		}

		/* if we're using phrases, go on with the next one */
		if (is_phrase_present && current_token == NULL)
			current_token = start_token;
	}

	return;
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
	int hunpos_error = 0;
	hunposProc->hunpos_instance = hunpos_tagger_new( hunposProc->model_file, NULL, hunposProc->max_guessed_tags, hunposProc->theta, &hunpos_error );
	if (hunpos_error !=0)
	{
		S_CTX_ERR(error, S_FAILURE,
			  "Initialize",
			  "Call to \"hunpos_tagger_new\" failed");
		goto quit_error;
	}

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
	const SRelation *relation;
	s_bool is_present;
	s_bool is_phrase_present;
	const SItem *current_item;
	const SItem **data_list;


	S_CLR_ERR(error);

	/* we require the token relation */
	is_present = SUtteranceRelationIsPresent(utt, "Token", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		return;

	if (!is_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Token' relation in utterance");
		return;
	}

	/* check if phrase relation is present */
	is_phrase_present = SUtteranceRelationIsPresent(utt, "Phrase", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		return;

	if (is_phrase_present)
	{
		relation = SUtteranceGetRelation(utt, "Phrase", error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
			return;
	}
	else
	{
		relation = SUtteranceGetRelation(utt, "Token", error);
		if (S_CHK_ERR(error, S_CONTERR,
					"Run",
					"Call to \"SUtteranceGetRelation\" failed"))
			return;
	}

	/* Get the items.
	 * start at the first item in the relation
	 */
	current_item = SRelationHead(relation, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		return;


	/* count tokens
	while (current_item != NULL)
	{
		tokenCount++;
		current_item = SItemNext(current_item, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemNext\" failed"))
			goto quit_error;
	}*/

	/* alloc the needed structure */
	data_list = S_MALLOC(SItem*, hunposProc->max_tokens_number);
	/* restart from the begin and add them to the structure
	current_item = SRelationHead(relation, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;
	while (current_item != NULL)
	{
		data_list[i] = current_item;
		current_item = SItemNext(current_item, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
		i++;
	}*/

	/* tag the data */

	call_hunpos(hunposProc, current_item, data_list, is_phrase_present, error);

	S_FREE(data_list);

	/* here all is OK */
	return;
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

