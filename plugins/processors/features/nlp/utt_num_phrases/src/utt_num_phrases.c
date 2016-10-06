/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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
/* DATE    : February 2010                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Utt num phrases feature processor.                                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "utt_num_phrases.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SUttNumPhrasesFeatProcClass UttNumPhrasesFeatProcClass; /* SUttNumPhrasesFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_utt_num_phrases_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&UttNumPhrasesFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_utt_num_phrases_class_reg",
			  "Failed to register SUttNumPhrasesFeatProcClass");
}


S_LOCAL void _s_utt_num_phrases_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&UttNumPhrasesFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_utt_num_phrases_class_free",
			  "Failed to free SUttNumPhrasesFeatProcClass");
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
	const SItem *itr;
	const SItem *itemInSentenceRel;
	const SItem *itemFinal = NULL;
	sint32 count;


	S_CLR_ERR(error);

	if ( item == NULL )
		return NULL;

	itemInSentenceRel = SItemAs(item, "Sentence", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemRelation\" failed"))
		goto quit_error;

	/* If the itemInSentenceRel is NULL then the item is not a phrase
	 * */
	if ( itemInSentenceRel == NULL )
		return NULL;

	SItem * sentenceItem = SItemParent (itemInSentenceRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemParent\" failed"))
		goto quit_error;

	itr = SItemDaughter (sentenceItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemDaughter\" failed"))
		goto quit_error;

	SItem * nextSentence = SItemNext (sentenceItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemNext\" failed"))
		goto quit_error;

	if ( nextSentence != NULL )
		itemFinal = SItemDaughter (nextSentence, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemDaughter\" failed"))
			goto quit_error;

	count = 0;
	while (itr != itemFinal && itr != NULL)
	{
		count++;
		itr = SItemNext(itr, error);
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
/* SUttNumPhrasesFeatProc class initialization                                      */
/*                                                                                  */
/************************************************************************************/

static SUttNumPhrasesFeatProcClass UttNumPhrasesFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SUttNumPhrasesFeatProc",
		sizeof(SUttNumPhrasesFeatProc),
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
