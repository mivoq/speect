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

	castSelf->symbols = SMapCopy ( NULL , features, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "Initialize",
			  "Call to \"SMapCopy\" failed"))
		goto quit_error;

	/* Get the iterator for the SMap */
	SIterator *itrList = S_ITERATOR_GET(castSelf->symbols, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to \"S_ITERATOR_GET\" failed"))
		goto quit_error;

	while( itrList != NULL ) {

		const char *curStr = SIteratorKey(itrList, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Initialize",
			      "Call to \"SIteratorKey\" failed"))
			goto quit_error;

		SList *valueList;
		valueList = S_CAST(SMapGetObject(castSelf->symbols, curStr, error), SList, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapGetObject\" failed"))
			goto quit_error;

		/* Initializing a new SMap */
		SMap *newCastMap= S_MAP(S_NEW(SMapHashTable, error));

		SIterator *itrValueList = S_ITERATOR_GET(valueList, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"S_ITERATOR_GET\" failed"))
			goto quit_error;

		while(itrValueList != NULL) {
			const SObject *curValueObj = SIteratorObject(itrValueList, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"SIteratorObject\" failed"))
				goto quit_error;

			const char *curValueStr = SObjectGetString(curValueObj, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"SObjectGetString\" failed"))
				goto quit_error;

			/* Insert the string inside the map */
			SMapSetString(newCastMap, curValueStr, curValueStr, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"SMapSetObject\" failed"))
				goto quit_error;

			itrValueList = SIteratorNext(itrValueList);
		}

		/* Insertion of the SMap inside the SMap */
		SMapSetObject(castSelf->symbols, curStr, (SObject *) newCastMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"SMapSetObject\" failed"))
				goto quit_error;
		itrList = SIteratorNext(itrList);
	}

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
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPathToItem\" failed"))
		goto quit_error;

	SMap* posPunctuation = S_CAST( SMapGetObject ( castSelf->symbols, "pos punctuation", error ), SMap, error );
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SMapGetObject\" failed"))
		goto quit_error;
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"S_CAST\" failed"))
		goto quit_error;

	count = -1;
	s_bool found = FALSE;

	while (found == FALSE && itrItem != NULL)
	{
		count++;

		s_bool hasPos = SItemFeatureIsPresent ( itrItem, "POS", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemFeatureIsPresent\" failed"))
			goto quit_error;

		if (hasPos)
		{
			const char* keyPos = SItemGetString (itrItem, "POS", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemGetString\" failed"))
				goto quit_error;

			found= SMapObjectPresent(posPunctuation, keyPos, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SMapObjectPresent\" failed"))
				goto quit_error;
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

	S_UNUSED(self);

	/* error cleanup */
quit_error:
	if (extractedFeat != NULL)
		S_DELETE(extractedFeat, "Run", error);

	return NULL;
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
