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
/* DATE    : December 2010                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Phrase num syllables feature processor.                                          */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "phrase_num_syls.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SPhraseNumSylsFeatProcClass PhraseNumSylsFeatProcClass; /* SPhraseNumSylsFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

/* This is the same as the word_num_syls feature processor, but having
 * it here will be faster
 */
static sint32 word_num_syls(const SItem *item, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_phrase_num_syls_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&PhraseNumSylsFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phrase_num_syls_class_reg",
			  "Failed to register SPhraseNumSylsFeatProcClass");
}


S_LOCAL void _s_phrase_num_syls_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&PhraseNumSylsFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phrase_num_syls_class_free",
			  "Failed to free SPhraseNumSylsFeatProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static sint32 word_num_syls(const SItem *item, s_erc *error)
{
	const SItem *itemInSylStructRel;
	const SItem *itrItem;
	sint32 count;


	S_CLR_ERR(error);

	if (item == NULL)
		return 0;

	itemInSylStructRel = SItemAs(item, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		return 0;

	if (itemInSylStructRel == NULL)
		return 0;

	itrItem = SItemDaughter(itemInSylStructRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemDaughter\" failed"))
		return 0;

	count = 0;
	while (itrItem != NULL)
	{
		count++;
		itrItem = SItemNext(itrItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			return 0;
	}

	/* all OK here */
	return count;
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
	const SItem *itemInPhraseRel;
	const SItem *itrItem;
	sint32 count;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	itemInPhraseRel = SItemAs(item, "Phrase", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	if (itemInPhraseRel == NULL)
	{
		extractedFeat = SObjectSetInt(0, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectSetInt\" failed"))
			goto quit_error;
	}

	itrItem = SItemDaughter(itemInPhraseRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemDaughter\" failed"))
		goto quit_error;

	count = 0;
	while (itrItem != NULL)
	{
		count += word_num_syls(itrItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"word_num_syls\" failed"))
			goto quit_error;

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
/* SPhraseNumSylsFeatProc class initialization                                      */
/*                                                                                  */
/************************************************************************************/

static SPhraseNumSylsFeatProcClass PhraseNumSylsFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SPhraseNumSylsFeatProc",
		sizeof(SPhraseNumSylsFeatProc),
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
