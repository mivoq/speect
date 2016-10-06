/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Phrasing utterance processor.                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "phrasify_proc.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SPhrasingUttProc class declaration. */
static SPhrasingUttProcClass PhrasingUttProcClass;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void s_get_phrasing_symbols(const SUttProcessor *uttProc, const char **end_punc,
								   s_erc *error);

static const char * const s_default_phrasing_end_punc_chars = "?.,:;";


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_phrasing_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&PhrasingUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phrasing_utt_proc_class_reg",
			  "Failed to register SPhrasingUttProcClass");
}


S_LOCAL void _s_phrasing_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&PhrasingUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phrasing_utt_proc_class_free",
			  "Failed to free SPhrasingUttProcClass");
}

/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void s_get_phrasing_symbols(const SUttProcessor *uttProc, const char **end_punc,
								   s_erc *error)
{
	s_bool have_symbols;
	const SMap *phrasingSymbols;
	const char *symbols;


	S_CLR_ERR(error);

	/* set to default for now */
	*end_punc = s_default_phrasing_end_punc_chars;

	/* check if symbols are defined */
	have_symbols = SUttProcessorFeatureIsPresent(uttProc, "phrasing symbols", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_phrasing_symbols",
				  "Call to \"SUttProcessorFeatureIsPresent\" failed"))
		return;

	if (!have_symbols)
		return; /* no tokenizer symbols defined */

	phrasingSymbols = S_CAST(SUttProcessorGetFeature(uttProc, "phrasing symbols", error),
							 SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_phrasing_symbols",
				  "Failed to get 'phrasing symbols' SMap feature"))
		return;

	/* check if we have 'end-punctuation' symbols */
	have_symbols = SMapObjectPresent(phrasingSymbols, "end-punctuation", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_phrasing_symbols",
				  "Call to \"SMapObjectPresent\" failed"))
		return;

	if (have_symbols)
	{
		/* get them and set it in end_punc */
		symbols = SObjectGetString(SMapGetObject(phrasingSymbols, "end-punctuation", error),
								   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_phrasing_symbols",
					  "Failed to get 'end-punctuation' symbols"))
			return;
		*end_punc = symbols;
	}
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


static void Run(const SUttProcessor *self, SUtterance *utt,
				s_erc *error)
{
	const SRelation *wordRel;
	SItem *wordItem;
	const SItem *tokenItem;
	s_bool is_present;
	const char *end_punc;
	SRelation *phraseRelation = NULL;
	SItem *phraseItem = NULL;
	const char *post_punc;
	SRelation *sentenceRelation = NULL;
	SItem *sentenceItem = NULL;


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

	/* get phrasing symbols */
	s_get_phrasing_symbols(self, &end_punc, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_get_phrasing_symbols\" failed"))
		goto quit_error;

	/* create Phrase relation */
	phraseRelation = SUtteranceNewRelation(utt, "Phrase", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceNewRelation\" failed"))
		goto quit_error;

	/* create Sentence relation */
	sentenceRelation = SUtteranceNewRelation(utt, "Sentence", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceNewRelation\" failed"))
		goto quit_error;

	/* start at the first item in the word relation, cast away
	 * const, we want to add daughter items
	 */
	wordItem = (SItem*)SRelationHead(wordRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	while (wordItem != NULL)
	{
		SItem *lastWordInToken;
		SItem *wordAsToken;


		if (phraseItem == NULL)
		{
			/* if phrase item is NULL, create a new phrase item (NULL
			 * shared content) that is appended to phrase
			 * relation. Will happen in first pass.
			 */
			sentenceItem = SRelationAppend(sentenceRelation, NULL, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SRelationAppend\" failed"))
				goto quit_error;

			/* Added on top a sentence item, for now is one on one with the phrase item
			 * */
			phraseItem = SRelationAppend(phraseRelation, NULL, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SRelationAppend\" failed"))
				goto quit_error;

			/* add an item name, NB, no break */
			SItemSetString(phraseItem, "name", "NB", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemSetString\" failed"))
				goto quit_error;

			SItemAddDaughter(sentenceItem, phraseItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemAddDaughter\" failed"))
				goto quit_error;
		}

		/* Create a daughter item for the phrase item. Shared content
		 * is the word item.
		 */
		SItemAddDaughter(phraseItem, wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemAddDaughter\" failed"))
			goto quit_error;

		/* get word as in Token relation */
		wordAsToken = SItemAs(wordItem, "Token", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Failed to get word item's as in Token relation"))
			goto quit_error;

		/*
		 * get word's token which is the parent of wordAsToken.
		 */
		tokenItem = SItemParent(wordAsToken, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Failed to get word item's token item"))
			goto quit_error;

		/* get last word in token */
		lastWordInToken = SItemLastDaughter(tokenItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
			"Run",
		"Failed to get last daughter of token item"))
			goto quit_error;

		/* check if the next token is punctuation */
		is_present = FALSE;
		tokenItem = SItemNext(tokenItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
		if (tokenItem != NULL)
		{
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
				is_present = (value > 0);
			}
		}

		if ((is_present) && (wordAsToken == lastWordInToken))
		{
			char *ptr;


			post_punc = SItemGetName(tokenItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemGetName\" failed"))
				goto quit_error;

			/* check if it is in the end_punc list */
			ptr = s_strpbrk(post_punc, end_punc, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"s_strpbrk\" failed"))
				goto quit_error;

			if (ptr != NULL)
			{
				/* add a phrase break */
				SItemSetString(phraseItem, "name", "BB", error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to \"SItemSetString\" failed"))
					goto quit_error;

				/* set to NULL so that a new phrase item is created */
				phraseItem = NULL;
			}
		}

		wordItem = SItemNext(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	/* here all is OK */
	return;

	/* error clean-up code */
quit_error:
	if (phraseRelation != NULL)
	{
		SUtteranceDelRelation(utt, "Phrase", error);
		S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceDelRelation\" failed");
	}
}


/************************************************************************************/
/*                                                                                  */
/* SPhrasingUttProc class initialization                                            */
/*                                                                                  */
/************************************************************************************/

static SPhrasingUttProcClass PhrasingUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:SPhrasingUttProc",
		sizeof(SPhrasingUttProc),
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
