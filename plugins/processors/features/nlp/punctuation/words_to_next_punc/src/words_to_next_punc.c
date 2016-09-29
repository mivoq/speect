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

#include "words_to_next_punc.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SWordsToNextPuncFeatProcClass WordsToNextPuncFeatProcClass; /* SWordsToNextPuncFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_words_to_next_punc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&WordsToNextPuncFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_words_to_next_punc_class_reg",
			  "Failed to register SWordsToNextPuncFeatProcClass");
}


S_LOCAL void _s_words_to_next_punc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&WordsToNextPuncFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_words_to_next_punc_class_free",
			  "Failed to free SWordsToNextPuncFeatProcClass");
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

static void Initialize(SFeatProcessor *self, const SMap* features, s_erc *error)
{
	S_CLR_ERR(error);

	SWordsToNextPuncFeatProc *castSelf = S_CAST(self, SWordsToNextPuncFeatProc, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to S_CAST failed"))
		goto quit_error;

	castSelf->symbols =  S_CAST( SMapGetObject(features , "pos punctuation", error),
								SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "Initialize",
			  "Call to \"SMapGetObject\" failed"))
		goto quit_error;

	castSelf->symbols = SMapCopy ( NULL , castSelf->symbols , error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "Initialize",
			  "Call to \"SMapCopy\" failed"))
		goto quit_error;

	/* error cleanup */
	quit_error:
		return;
}

static SObject *Run(const SFeatProcessor *self, const SItem *item,
					s_erc *error)
{
	SObject *extractedFeat = NULL;
	const SItem *itrItem;
	sint32 count;

	SWordsToNextPuncFeatProc *castSelf = S_CAST(self, SWordsToNextPuncFeatProc, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to S_CAST failed"))
		goto quit_error;

	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	itrItem = SItemPathToItem (item, "R:Token.parent", error);
	count = -1;
	s_bool found = FALSE;

	while (found == FALSE && itrItem != NULL)
	{
		count++;

		s_bool hasPos = SItemFeatureIsPresent ( itrItem, "POS", error);

		if (hasPos)
		{
			const char* keyPos = SItemGetString (itrItem, "POS", error);

			found= SMapObjectPresent(castSelf->symbols, keyPos, error);
		}

		itrItem = SItemNext(itrItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
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
/* SWordsToNextPuncFeat class initialization                                   */
/*                                                                                  */
/************************************************************************************/

static SWordsToNextPuncFeatProcClass WordsToNextPuncFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SWordsToNextPuncFeatProc",
		sizeof(SWordsToNextPuncFeatProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SFeatProcessorClass */
	Initialize,               /* initialize */
	Run                  /* run     */
};
