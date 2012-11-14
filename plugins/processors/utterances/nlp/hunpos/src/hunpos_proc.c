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

	/* initialize hunpos */
	hunposProc->handle = init_hunpos(hunposProc->model_file,
									 "",  /* no morph_table_file */
									 hunposProc->max_guessed_tags,
									 hunposProc->theta);

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
	const SVoice *voice;
	const SRelation *wordRel;
	const SFeatProcessor *wordCounter;
	s_bool is_present;
	char **words = NULL;
	char **tags = NULL;
	int nwords = 0;
	const SItem *wordItem;
	SItem *wordItemSet;
	int i;


	S_CLR_ERR(error);

	/* see if there is a word count feature processor in the voice */
	voice = SUtteranceVoice(utt, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceVoice\" failed"))
		goto quit_error;

	if (voice != NULL)
	{
		wordCounter = SVoiceGetFeatProc(voice, "utt_num_words", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SVoiceGetFeatProc\" failed"))
			goto quit_error;
	}

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

	/* if the utt_num_words feature processor is present, use it
	 * The reason for doing this is that *maybe* there is a language
	 * with some other way of getting the tokens for hunpos
	 */
	if (wordCounter != NULL)
	{
		SObject *extractedFeature = NULL;
		const SItem *wordHeadItem = NULL;


		wordHeadItem = SRelationHead(wordRel, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SRelationHead\" failed"))
			goto quit_error;

		extractedFeature = SFeatProcessorRun(wordCounter,
											 wordHeadItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SFeatProcessorRun\" failed"))
			goto quit_error;

		nwords = SObjectGetInt(extractedFeature, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectGetInt\" failed"))
		{
			S_DELETE(extractedFeature, "Run", error);
			goto quit_error;
		}

		S_DELETE(extractedFeature, "Run", error);
	}
	else /* no utt_num_words feature processor, use local one */
	{
		nwords = count_words(wordRel, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"count_words\" failed"))
			goto quit_error;
	}

	/* allocate memory for words and tags */
	words = S_MALLOC(char*, nwords);
	if (words == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "Run",
				  "Failed to allocate memory for \"char*\" object");
		goto quit_error;
	}

	tags = S_MALLOC(char*, nwords);
	if (tags == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "Run",
				  "Failed to allocate memory for \"char*\" object");
		goto quit_error;
	}

	/* Get the words.
	 * start at the first item in the token relation
	 */
	wordItem = SRelationHead(wordRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	i = 0;
	while (wordItem != NULL)
	{
		const char *item_name;


		item_name = SItemGetName(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemGetName\" failed"))
			goto quit_error;

		words[i] = s_strdup(item_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_strdup\" failed"))
			goto quit_error;

/*
		tags[i] = S_MALLOC(char, SPCT_DEF_MAX_TAG_LENGTH);
		if (tags[i] == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "Run",
					  "Failed to allocate memory for \"char\" object");
			goto quit_error;
		}
*/

		wordItem = SItemNext(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;

		i++;
	}

	/* let hunpos do its thing */
	tag(hunposProc->handle, nwords, words, tags);

	/* cycle through the word relation and add the POS feature
	 * also free up words and tags char**
	 */
	wordItemSet = (SItem*)SRelationHead(wordRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	i = 0;
	while (wordItemSet != NULL)
	{
		SItemSetString(wordItemSet, "POS", tags[i], error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemSetString\" failed"))
			goto quit_error;

		S_FREE(words[i]);
		/* S_FREE(tags[i]); */

		wordItemSet = SItemNext(wordItemSet, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;

		i++;
	}

	S_FREE(words);
	S_FREE(tags);

	/* here all is OK */
	return;

	/* error clean-up code */
quit_error:
	if (words != NULL)
	{
		for (i = 0; i < nwords; i++)
			S_FREE(words[i]);
		S_FREE(words);
	}

	if (tags != NULL)
	{
		for (i = 0; i < nwords; i++)
			S_FREE(tags[i]);
		S_FREE(tags);
	}
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
