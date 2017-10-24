/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Textexp Normalization utterance processor.                                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "normalizer_proc.h"
#include "textexp/textexp.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SNormalizationUttProc class declaration. */
static STextexpNormalizationUttProcClass TextexpNormalizationUttProcClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_textexp_normalization_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&TextexpNormalizationUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
	          "_s_normalization_utt_proc_class_reg",
	          "Failed to register SNormalizationUttProcClass");
}


S_LOCAL void _s_textexp_normalization_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&TextexpNormalizationUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
	          "_s_normalization_utt_proc_class_free",
	          "Failed to free SNormalizationUttProcClass");
}

/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}

typedef struct token_expansion_support_data {
	s_buffer *curr_string_buffer;
	SRelation *wordRel;
	SItem *tokenItem;
	s_erc *error;
} token_expansion_support_data;

static textexp_token_result_type_t _token_expansion_callback (const char*token, void* token_expansion_support_data_ptr, int token_position){
	token_expansion_support_data* data = (token_expansion_support_data*) token_expansion_support_data_ptr;

	size_t token_size = s_strsize(token, data->error);
	const uchar* string = NULL;
	if (S_CHK_ERR(data->error, S_CONTERR,
	              "Run",
	              "Call to \"s_strsize\" failed"))
		goto quit_error;

	s_buffer_append(data->curr_string_buffer, token, token_size, data->error);
	if (S_CHK_ERR(data->error, S_CONTERR,
	              "Run",
	              "Call to \"s_buffer_append\" failed"))
		goto quit_error;

	if( (token_position==TEXTEXP_TOKEN_END) || (token_position==TEXTEXP_TOKEN_SEQUENCE_END) ) {
		SItem *wordItem = SItemAddDaughter(data->tokenItem, NULL, data->error);
		if (S_CHK_ERR(data->error, S_CONTERR,
		              "Run",
		              "Call to \"SItemAddDaughter\" failed"))
			goto quit_error;

		s_buffer_append(data->curr_string_buffer, "", 1, data->error);
		if (S_CHK_ERR(data->error, S_CONTERR,
		              "Run",
		              "Call to \"s_buffer_append\" failed"))
			goto quit_error;

		string = s_buffer_data(data->curr_string_buffer, data->error);
		if (S_CHK_ERR(data->error, S_CONTERR,
		              "Run",
		              "Call to \"s_buffer_data\" failed"))
			goto quit_error;

		SItemSetString(wordItem, "name", (char*) string, data->error);
		if (S_CHK_ERR(data->error, S_CONTERR,
		              "Run",
		              "Call to \"SItemSetObject\" failed"))
			goto quit_error;

		/* and create a new word item in word relation, shared
		 * content it token relation's word item.
		 */
		SRelationAppend(data->wordRel, wordItem, data->error);
		if (S_CHK_ERR(data->error, S_CONTERR,
		              "Run",
		              "Call to \"SItemSetObject\" failed"))
			goto quit_error;

		s_buffer_clear(data->curr_string_buffer, data->error);
		if (S_CHK_ERR(data->error, S_CONTERR,
		              "Run",
		              "Call to \"s_buffer_clear\" failed"))
			goto quit_error;
	}
	return TEXTEXP_TOKEN_RESULT_SUCCESS;

quit_error:
	return TEXTEXP_TOKEN_RESULT_ERROR_ABORT;
}

static void Run(const SUttProcessor *self, SUtterance *utt, s_erc *error)
{
	const SVoice *voice;
	const SFeatProcessor *normProcessor = NULL;
	const SRelation *tokenRel;
	SRelation *wordRel = NULL;
	s_bool is_present;
	SItem *tokenItem;
	const SObject *token;
	const char *token_string;
	char *token_string_lc = NULL;
	long value = 0;

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

	/* see if there is a normalization feature processor in the voice */
	voice = SUtteranceVoice(utt, error);
	if (S_CHK_ERR(error, S_CONTERR,
	              "Run",
	              "Call to \"SUtteranceVoice\" failed"))
		goto quit_error;

	if (voice != NULL)
	{
		normProcessor = SVoiceGetFeatProc(voice, "word-normalization", error);
		if (S_CHK_ERR(error, S_CONTERR,
		              "Run",
		              "Call to \"SVoiceGetFeatProc\" failed"))
			goto quit_error;
	}

	/* create Word relation */
	wordRel = SUtteranceNewRelation(utt, "Word", error);
	if (S_CHK_ERR(error, S_CONTERR,
	              "Run",
	              "Call to \"SUtteranceNewRelation\" failed"))
		goto quit_error;

	/* start at the first item in the token relation, cast away
	 * const, we want to add daughter items
	 */
	tokenItem = (SItem*)SRelationHead(tokenRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
	              "Run",
	              "Call to \"SRelationHead\" failed"))
		goto quit_error;

	while (tokenItem != NULL)
	{
		/* Ignore punctuation: check if we should skip this token */
		s_bool should_skip = FALSE;
		is_present = SItemFeatureIsPresent(tokenItem, "IsPunctuation", error);
		if (S_CHK_ERR(error, S_CONTERR,
		              "Run",
		              "Call to \"SItemFeatureIsPresent\" failed"))
			goto quit_error;
		if (is_present)
		{
			sint32 value = SItemGetInt(tokenItem, "IsPunctuation", error);
			if (S_CHK_ERR(error, S_CONTERR,
			              "Run",
			              "Call to \"SItemGetInt\" failed"))
				goto quit_error;
			if (value > 0)
				should_skip = TRUE;
		}

		if (!should_skip)
		{
			/* Give the item to the normalization feature processor, if
			 * any. Normalization feature processor can set daughter items
			 * in Word relation, return would be NULL.
			 */
			if (normProcessor)
			{
				SFeatProcessorRun(normProcessor, tokenItem, error);
				if (S_CHK_ERR(error, S_CONTERR,
				              "Run",
				              "Call to \"SFeatProcessorRun\" failed"))
					goto quit_error;
			}
			else
			{
				/* we can't do normalization, hope for the best and set
				 * the token as the word. Token is name feature of token
				 * item.
				 */
				token = SItemGetObject(tokenItem, "name", error);
				if (S_CHK_ERR(error, S_CONTERR,
				              "Run",
				              "Call to \"SItemGetObject\" failed"))
					goto quit_error;

				/* get and lowercase token string */
				token_string = SObjectGetString(token, error);
				if (S_CHK_ERR(error, S_CONTERR,
				              "Run",
				              "Call to \"SItemGetObject\" failed"))
					goto quit_error;

				if (textexp_detect_long_it(token_string, &value)==0) {
					s_buffer *curr_string_buffer = s_buffer_new(error);
					if (S_CHK_ERR(error, S_CONTERR,
					              "Run",
					              "Call to \"s_buffer_new\" failed"))
						goto quit_error;

					struct token_expansion_support_data numexptoken = {curr_string_buffer, wordRel, tokenItem, error};
					textexp_expand_long_it(value, 0, _token_expansion_callback, &numexptoken);
					s_buffer_delete(numexptoken.curr_string_buffer, error);
					if (S_CHK_ERR(numexptoken.error, S_CONTERR,
					              "Run",
					              "Call to \"textexp_expand_long_it\" failed"))
						goto quit_error;
					if (S_CHK_ERR(error, S_CONTERR,
					              "Run",
					              "Call to \"s_buffer_delete\" failed"))
						goto quit_error;
				}
				else {
					/* create word item as daughter of token item, NULL
					 * shared content
					 */
					SItem *wordItem = SItemAddDaughter(tokenItem, NULL, error);
					if (S_CHK_ERR(error, S_CONTERR,
					              "Run",
					              "Call to \"SItemAddDaughter\" failed"))
						goto quit_error;
					token_string_lc = s_strdup(token_string, error);
					if (S_CHK_ERR(error, S_CONTERR,
					              "Run",
					              "Call to \"s_strdup\" failed"))
						goto quit_error;

					s_strlwr(token_string_lc, error);
					if (S_CHK_ERR(error, S_CONTERR,
					              "Run",
					              "Call to \"s_strlwr\" failed"))
						goto quit_error;

					/* set the token lowercase as the word item's name */
					SItemSetString(wordItem, "name", token_string_lc, error);
					if (S_CHK_ERR(error, S_CONTERR,
					              "Run",
					              "Call to \"SItemSetObject\" failed"))
						goto quit_error;

					S_FREE(token_string_lc);

					/* and create a new word item in word relation, shared
					 * content it token relation's word item.
					 */
					SRelationAppend(wordRel, wordItem, error);
					if (S_CHK_ERR(error, S_CONTERR,
					              "Run",
					              "Call to \"SItemSetObject\" failed"))
						goto quit_error;
				}
			}
		}

		tokenItem = SItemNext(tokenItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
		              "Run",
		              "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	/* here all is OK */
	return;

	/* error clean-up code */
quit_error:
	if (wordRel != NULL)
	{
		SUtteranceDelRelation(utt, "Word", error);
		S_CHK_ERR(error, S_CONTERR,
		          "Run",
		          "Call to \"SUtteranceDelRelation\" failed");
	}

	if (token_string_lc != NULL)
		S_FREE(token_string_lc);

	S_UNUSED(self);
}


/************************************************************************************/
/*                                                                                  */
/* SNormalizationUttProc class initialization                                       */
/*                                                                                  */
/************************************************************************************/

static STextexpNormalizationUttProcClass TextexpNormalizationUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:STextexpNormalizationUttProc",
		sizeof(STextexpNormalizationUttProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SUttProcessorClass */
	NULL,                /* initialize */
	Run                  /* run        */
};
