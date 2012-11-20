/************************************************************************************/
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
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

static int count_words(const SRelation *wordRel, s_erc *error);


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


static char *hunpos_tag(const char *words, const char *tag_model, s_erc *error)
{
	const char *cmd_line = "echo \"%s\" | hunpos-tag %s";
	char *cmd_str = NULL;
	FILE *read_fp;
    char buffer[BUFSIZ + 1];
    int chars_read;
	char *tagged = NULL;


	S_CLR_ERR(error);
    memset(buffer, '\0', sizeof(buffer));

	s_asprintf(&cmd_str, error, cmd_line, words, tag_model);
	if (S_CHK_ERR(error, S_CONTERR,
				  "hunpos_tag",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

    read_fp = popen(cmd_str, "r");
	if (read_fp == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "hunpos_tag",
				  "Call to \"popen\" failed");
		S_FREE(cmd_str);
		return NULL;
	}

	chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
	if (chars_read > 0)
	{
		tagged = s_strdup(buffer, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "hunpos_tag",
					  "Call to \"s_strdup\" failed"))
		{
			S_FREE(cmd_str);
			return NULL;
		}
	}
	pclose(read_fp);
	S_FREE(cmd_str);

	return tagged;
}


static char *concat_words(char *dest, const char *word, s_erc *error)
{
	size_t size;


	S_CLR_ERR(error);

	if (dest == NULL)
	{
		size = s_strsize(word, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "concat_words",
					  "Call to \"s_strsize\" failed"))
			return NULL;

		dest = S_CALLOC(char, size+2); /* one extra for newline and one for null */
		if (dest == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "concat_words",
					  "Failed to allocate memory for char");
			return NULL;
		}
	}
	else
	{
		char *tmp = dest;
		size_t tsize;
		size_t wsize;

		tsize = s_strsize(tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "concat_words",
					  "Call to \"s_strsize\" failed"))
			return NULL;

		wsize = s_strsize(word, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "concat_words",
					  "Call to \"s_strsize\" failed"))
			return NULL;

		size = tsize+wsize;

		dest = S_CALLOC(char, size+2); /* one extra for newline and one for null */
		if (dest == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "concat_words",
					  "Failed to allocate memory for char");
			return NULL;
		}

		s_strzcat(dest, tmp, size+2, error);
		S_FREE(tmp);
		if (S_CHK_ERR(error, S_CONTERR,
					  "concat_words",
					  "Call to \"s_strzcat\" failed"))
		{
			S_FREE(dest);
			return NULL;
		}
	}

	s_strzcat(dest, word, size+2, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "concat_words",
				  "Call to \"s_strzcat\" failed"))
	{
		S_FREE(dest);
		return NULL;
	}

	s_strzcat(dest, "\n", size+2, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "concat_words",
				  "Call to \"s_strzcat\" failed"))
	{
		S_FREE(dest);
		return NULL;
	}

	return dest;
}


static char **extract_pos(const char *tagmix, int num_words, s_erc *error)
{
	char **pos = NULL;
	STokenstream *ts = NULL;
	const SToken *token;
	const char *tmp;
	s_bool eof;
	int num_tokens = 0;
	int num_pos = 0;


	S_CLR_ERR(error);

	pos = S_CALLOC(char*, num_words + 1);
	if (pos == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "extract_pos",
				  "Failed to allocate memory for char*");
		goto quit;
	}

	/* create string tokenstream */
	ts = (STokenstream*)S_NEW(STokenstreamString, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "extract_pos",
				  "Failed to create new string tokenstream"))
		goto quit;

	/* initialize string tokenstream */
	STokenstreamStringInit((STokenstreamString**)&ts, tagmix, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "extract_pos",
				  "Failed to initialize string tokenstream"))
		goto quit;

	eof = STokenstreamQueryEOF(ts, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "extract_pos",
				  "Failed to query end of file"))
		goto quit;

	while (!eof)
	{
		token = STokenstreamPeekToken(ts, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "extract_pos",
					  "Failed to peek token"))
			goto quit;

		tmp = STokenGetString(token, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "extract_pos",
					  "Failed to get token string"))
			goto quit;

		if (tmp == NULL)
			break;
		else
		{
			token = STokenstreamGetToken(ts, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "extract_pos",
						  "Failed to get token"))
				goto quit;
		}

		num_tokens++;

		if (num_tokens % 2 == 0)
		{
			/* POS tag */
			tmp = STokenGetString(token, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "extract_pos",
						  "Call to \"STokenGetString\" failed"))
				goto quit;

			pos[num_pos++] = s_strdup(tmp, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "extract_pos",
						  "Call to \"s_strdup\" failed"))
				goto quit;
		}

		eof = STokenstreamQueryEOF(ts, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "extract_pos",
					  "Failed to query end of file"))
			goto quit;
	}

	S_DELETE(ts, "extract_pos", error);

	return pos;

	/*
	 * quit
	 */
quit:
	if (ts != NULL)
		S_DELETE(ts, "extract_pos", error);

	if (pos != NULL)
	{
		int i;

		for (i = 0; i < num_pos; i++)
			S_FREE(pos[i]);

		S_FREE(pos);
	}

	return NULL;
}


static int count_words(const SRelation *wordRel, s_erc *error)
{
	const SItem *itr;
	int count;


	S_CLR_ERR(error);

	itr = SRelationHead(wordRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "count_words",
				  "Call to \"SRelationHead\" failed"))
		return 0;

	count = 0;
	while (itr != NULL)
	{
		count++;
		itr = SItemNext(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "count_words",
					  "Call to \"SItemNext\" failed"))
			return 0;
	}

	return count;
}


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
	const SRelation *wordRel;
	s_bool is_present;
	char *words = NULL;
	char *tagmix = NULL;
	char **pos = NULL;
	const SItem *wordItem;
	SItem *wordItemSet;
	int i = 0;


	S_CLR_ERR(error);

	/* we require the word relation */
	is_present = SUtteranceRelationIsPresent(utt, "Word", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto quit_error;

	if (!is_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Word' relation in utterance");
		goto quit_error;
	}

	wordRel = SUtteranceGetRelation(utt, "Word", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
		goto quit_error;

	/* Get the words.
	 * start at the first item in the word relation
	 */
	wordItem = SRelationHead(wordRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	while (wordItem != NULL)
	{
		const char *item_name;


		item_name = SItemGetName(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemGetName\" failed"))
			goto quit_error;

		words = concat_words(words, item_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"concat_words\" failed"))
			goto quit_error;

		wordItem = SItemNext(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;

		i++;
	}

	/* add final newline */
	words = concat_words(words, "\n", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"concat_words\" failed"))
		goto quit_error;

	/* let hunpos do its thing */
	tagmix = hunpos_tag(words, hunposProc->model_file, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"hunpos_tag\" failed"))
		goto quit_error;

	/* get pos tags */
	pos = extract_pos(tagmix, i, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"extract_pos\" failed"))
		goto quit_error;

	/* Put tags back into word relation.
	 * start at the first item in the word relation
	 */
	i = 0;
	wordItemSet = (SItem*)SRelationHead(wordRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	while (wordItemSet != NULL)
	{
		SItemSetString(wordItemSet, "POS", pos[i], error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemSetString\" failed"))
			goto quit_error;

		S_FREE(pos[i]);

		wordItemSet = SItemNext(wordItemSet, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;

		i++;
	}

	S_FREE(pos);
	S_FREE(words);
	S_FREE(tagmix);

	/* here all is OK */
	return;

	/* error clean-up code */
quit_error:
	if (words != NULL)
		S_FREE(words);

	if (tagmix != NULL)
		S_FREE(tagmix);

	if (pos != NULL)
		S_FREE(pos);
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
