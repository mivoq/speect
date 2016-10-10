/************************************************************************************/
/* Copyright (c) Mivoq SRL                                                          */
/*                                                                                  */
/* Contributors:  Federico Rossetto, Giulio Paci                                    */
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
/* AUTHOR  : Federico Rossetto                                                      */
/* DATE    : September 2016                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Phrase num words feature processor.                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "words_from_utterance_end.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SWordsFromUtteranceEndFeatProcClass WordsFromUtteranceEndFeatProcClass; /* SWordsFromUtteranceEndFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_words_from_utterance_end_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&WordsFromUtteranceEndFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_words_from_utterance_end_class_reg",
			  "Failed to register SWordsFromUtteranceEndFeatProcClass");
}


S_LOCAL void _s_words_from_utterance_end_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&WordsFromUtteranceEndFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_words_from_utterance_end_class_free",
			  "Failed to free SWordsFromUtteranceEndFeatProcClass");
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


static SObject *Run(const SFeatProcessor *self, const SItem *item,
					s_erc *error)
{
	SObject *extractedFeat = NULL;
	const SItem *itrItem;
	sint32 count;
	s_bool found = FALSE;
	s_bool wordFound = FALSE;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	itrItem = item;
	count = 0;

	const SItem* itrPhrase = SItemPathToItem(item, "R:Phrase.parent", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPathToItem\" failed"))
		goto quit_error;

	const SItem* itrLastPhrase = SItemPathToItem(item,
				             "R:Phrase.parent.R:Sentence.parent.n.daughter.R:Phrase",
				             error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPathToItem\" failed"))
		goto quit_error;

	const SItem* itrLastWord = SItemPathToItem(item, "R:Phrase.parent.daughtern", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPathToItem\" failed"))
		goto quit_error;

	wordFound =  SItemEqual(itrItem, itrLastWord, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemEqual\" failed"))
				goto quit_error;

	while( found == FALSE && itrPhrase != NULL )
	{
		while ( wordFound == FALSE && itrItem != NULL )
		{
			count++;

			itrItem = SItemNext(itrItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemNext\" failed"))
				goto quit_error;

			wordFound =  SItemEqual(itrItem, itrLastWord, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemEqual\" failed"))
				goto quit_error;
		}

		itrPhrase = SItemNext (itrPhrase, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;

		if (itrPhrase != NULL)
		{
			itrItem = SItemLastDaughter (itrPhrase, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemLastDaughter\" failed"))
				goto quit_error;
		}

		found = SItemEqual ( itrPhrase, itrLastPhrase, error );
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemEqual\" failed"))
			goto quit_error;

	}

	extractedFeat = SObjectSetInt(count, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectSetInt\" failed"))
		goto quit_error;

	/* all OK here */
	return extractedFeat;

	/* error cleanup */
quit_error:
	if (extractedFeat != NULL)
		S_DELETE(extractedFeat, "Run", error);

	return NULL;

	S_UNUSED(self);
}


/************************************************************************************/
/*                                                                                  */
/* SWordsFromUtteranceEndFeat class initialization                                   */
/*                                                                                  */
/************************************************************************************/

static SWordsFromUtteranceEndFeatProcClass WordsFromUtteranceEndFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SWordsFromUtteranceEndFeatProc",
		sizeof(SWordsFromUtteranceEndFeatProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SFeatProcessorClass */
	NULL,               /* initialize */
	Run                  /* run     */
};
