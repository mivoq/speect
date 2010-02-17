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
/* Syllable position in phrase (reverse) feature processor.                         */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "syl_pos_phrase_rev.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSylPosPhraseRevFeatProcClass SylPosPhraseRevFeatProcClass; /* SSylPosPhraseRevFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

/* This is the same as the syl_pos_word_rev feature processor, but having
 * it here will be faster
 */
static sint32 syll_pos_word_rev(const SItem *item, s_erc *error);


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
S_LOCAL void _s_syl_pos_phrase_rev_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&SylPosPhraseRevFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_pos_phrase_rev_class_reg",
			  "Failed to register SSylPosPhraseRevFeatProcClass");
}


S_LOCAL void _s_syl_pos_phrase_rev_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&SylPosPhraseRevFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_pos_phrase_rev_class_free",
			  "Failed to free SSylPosPhraseRevFeatProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static sint32 syll_pos_word_rev(const SItem *item, s_erc *error)
{
	const SItem *itemInSylStructRel;
	const SItem *wordItem;
	const SItem *itr;
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

	wordItem = SItemParent(itemInSylStructRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemParent\" failed"))
		return 0;

	if (wordItem == NULL)
		return 0;

	itr = SItemLastDaughter(wordItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemLastDaughter\" failed"))
		return 0;

	count = 0;
	while (itr != NULL)
	{
		s_bool is_equal;


		is_equal = SItemEqual(itr, itemInSylStructRel, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemEqual\" failed"))
			return 0;

		if (is_equal)
			break;

		count++;
		itr = SItemPrev(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemPrev\" failed"))
			return 0;
	}

	/* all OK here */
	return count;
}


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
	const SItem *syllWord;
	const SItem *itemInSylStructRel;
	const SItem *wordInPhrase;
	const SItem *phrase;
	const SItem *itr;
	sint32 count;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	/* syllable as in SylStructure */
	itemInSylStructRel = SItemAs(item, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	if (itemInSylStructRel == NULL)
		goto quit_null;

	/* syllable's word */
	syllWord = SItemParent(itemInSylStructRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemParent\" failed"))
		goto quit_error;

	if (syllWord == NULL)
		goto quit_null;

	/* as in phrase structure */
	wordInPhrase = SItemAs(syllWord, "Phrase", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	if (wordInPhrase == NULL)
		goto quit_null;

	/* get phrase */
	phrase = SItemParent(wordInPhrase, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemParent\" failed"))
		goto quit_error;

	if (phrase == NULL)
		goto quit_null;

	itr = SItemLastDaughter(phrase, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemLastDaughter\" failed"))
		goto quit_error;

	count = 0;
	while (itr != NULL)
	{
		s_bool is_equal;


		is_equal = SItemEqual(itr, syllWord, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemEqual\" failed"))
			goto quit_error;

		if (is_equal)
			break;

		count += word_num_syls(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"word_num_syls\" failed"))
			goto quit_error;

		itr = SItemPrev(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemPrev\" failed"))
			goto quit_error;
	}

	count += syll_pos_word_rev(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"syll_pos_word_rev\" failed"))
		goto quit_error;

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

	/* return 0 */
quit_null:
	extractedFeat = SObjectSetInt(0, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectSetInt\" failed"))
		goto quit_error;

	return extractedFeat;
}


/************************************************************************************/
/*                                                                                  */
/* SSylPosPhraseRevFeatProc class initialization                                    */
/*                                                                                  */
/************************************************************************************/

static SSylPosPhraseRevFeatProcClass SylPosPhraseRevFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SSylPosPhraseRevFeatProc",
		sizeof(SSylPosPhraseRevFeatProc),
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
