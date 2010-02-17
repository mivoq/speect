/************************************************************************************/
/* Copyright (c) 2010 The Department of Arts and Culture,                           */
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
/* Phrase position in utterance (reverse) feature processor.                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "phrase_pos_utt_rev.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SPhrasePosUttRevFeatProcClass PhrasePosUttRevFeatProcClass; /* SPhrasePosUttRevFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_phrase_pos_utt_rev_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&PhrasePosUttRevFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phrase_pos_utt_rev_class_reg",
			  "Failed to register SPhrasePosUttRevFeatProcClass");
}


S_LOCAL void _s_phrase_pos_utt_rev_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&PhrasePosUttRevFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phrase_pos_utt_rev_class_free",
			  "Failed to free SPhrasePosUttRevFeatProcClass");
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
	const SRelation *phraseRel;
	const SItem *itemInPhraseRel;
	const SItem *itr;
	sint32 count;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	/* phrase as in Phrase relation */
	itemInPhraseRel = SItemAs(item, "Phrase", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	if (itemInPhraseRel == NULL)
		return NULL;

	/* get Phrase relation */
	phraseRel = SItemRelation(itemInPhraseRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemRelation\" failed"))
		goto quit_error;

	if (phraseRel == NULL)
		return NULL;

	itr = SRelationTail(phraseRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	count = 0;
	while (itr != NULL)
	{
		s_bool is_equal;


		is_equal = SItemEqual(itr, itemInPhraseRel, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemEqual\" failed"))
			goto quit_error;

		if (is_equal)
			break;

		count++;

		itr = SItemPrev(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemPrev\" failed"))
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

	self = NULL; /* compiler noise about unused parameters */
	return NULL;
}


/************************************************************************************/
/*                                                                                  */
/* SPhrasePosUttRevFeatProc class initialization                                    */
/*                                                                                  */
/************************************************************************************/

static SPhrasePosUttRevFeatProcClass PhrasePosUttRevFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SPhrasePosUttRevFeatProc",
		sizeof(SPhrasePosUttRevFeatProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SFeatProcessorClass */
	Run                  /* run     */
};
