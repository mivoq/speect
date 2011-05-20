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
/* Word content all in feature processor.                                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "word_content_all_out.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SWordContentAllOutFeatProcClass WordContentAllOutFeatProcClass; /* SWordContentAllOutFeatProc class declaration. */


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
S_LOCAL void _s_word_content_all_out_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&WordContentAllOutFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_word_content_all_out_class_reg",
			  "Failed to register SWordContentAllOutFeatProcClass");
}


S_LOCAL void _s_word_content_all_out_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&WordContentAllOutFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_word_content_all_out_class_free",
			  "Failed to free SWordContentAllOutFeatProcClass");
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
	const SItem *wordItem;
	sint32 num_words = 1; /* include current word */


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	/* get the word as in the word relation */
	wordItem = SItemAs(item, "Word", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	if (wordItem == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to get given word as in 'Word' relation");
		goto quit_error;
	}

	/* start with next word */
	wordItem = SItemNext(wordItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemNext\" failed"))
		goto quit_error;

	while (wordItem != NULL)
	{
		s_bool is_content;


		is_content = word_is_content(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"word_is_content\" failed"))
			goto quit_error;

		if (is_content)
			break;
		else
			num_words++;

		wordItem= SItemNext(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	extractedFeat = SObjectSetInt(num_words, error);
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
/* SWordContentAllOutFeatProc class initialization                                  */
/*                                                                                  */
/************************************************************************************/

static SWordContentAllOutFeatProcClass WordContentAllOutFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SWordContentAllOutFeatProc",
		sizeof(SWordContentAllOutFeatProc),
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
