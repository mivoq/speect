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
/* DATE    : March 2010                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Word content out feature processor.                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "word_content_out.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SWordContentOutFeatProcClass WordContentOutFeatProcClass; /* SWordContentOutFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_bool word_is_content(const SItem *word, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_word_content_out_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&WordContentOutFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_word_content_out_class_reg",
			  "Failed to register SWordContentOutFeatProcClass");
}


S_LOCAL void _s_word_content_out_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&WordContentOutFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_word_content_out_class_free",
			  "Failed to free SWordContentOutFeatProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/*
 * Content word's "cat" feature is 'content'
 */
static s_bool word_is_content(const SItem *word, s_erc *error)
{
	const char *cat_feat;
	s_bool comp;


	S_CLR_ERR(error);
	cat_feat = SItemGetString(word, "cat", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "word_is_content",
				  "Call to \"SItemGetString\" failed"))
		return FALSE;


	comp = s_strcmp(cat_feat, "content", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "syl_is_stressed",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (comp == 0)
		return TRUE;

	return FALSE; /* not content word */
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
	const SItem *phraseItem;
	const SItem *wordItem;
	sint32 num_content = 0;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	/* get current phrase */
	phraseItem = s_path_to_item(item, "R:Phrase.parent",
								error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_path_to_item\" failed"))
		goto quit_error;

	if (phraseItem == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to get phrase of given word");
		goto quit_error;
	}

	/* last word in phrase */
	wordItem = SItemLastDaughter(phraseItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemLastDaughter\" failed"))
		goto quit_error;

	while (wordItem != NULL)
	{
		s_bool is_content;
		s_bool is_current_word;


		is_current_word = SItemEqual(wordItem, item, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemEqual\" failed"))
			goto quit_error;

		if (is_current_word)
			break;

		is_content = word_is_content(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"word_is_content\" failed"))
			goto quit_error;

		if (is_content)
			num_content++;

		wordItem = SItemPrev(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemPrev\" failed"))
			goto quit_error;
	}

	extractedFeat = SObjectSetInt(num_content, error);
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
/* SWordContentOutFeatProc class initialization                                     */
/*                                                                                  */
/************************************************************************************/

static SWordContentOutFeatProcClass WordContentOutFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SWordContentOutFeatProc",
		sizeof(SWordContentOutFeatProc),
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
