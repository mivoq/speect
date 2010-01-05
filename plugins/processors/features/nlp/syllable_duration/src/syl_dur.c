/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* Syllable duration feature processor.                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "syl_dur.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSylDurFeatProcClass SylDurFeatProcClass; /* SSylDurFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_syl_dur_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&SylDurFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_dur_class_reg",
			  "Failed to register SSylDurFeatProcClass");
}


S_LOCAL void _s_syl_dur_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&SylDurFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_dur_class_free",
			  "Failed to free SSylDurFeatProcClass");
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
	const SItem *itemInSylRel;
	const SItem *prevItem;
	const SItem *lastDaugthThisItem;
	const SItem *lastDaugthPrevItem;
	float end_curr;
	float end_prev;


	S_CLR_ERR(error);
	if (item == NULL)
		return NULL;

	itemInSylRel = SItemAs(item, "Syllable", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	if (itemInSylRel == NULL)
		return NULL;

	lastDaugthThisItem = SItemLastDaughter(itemInSylRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemLastDaughter\" failed"))
		goto quit_error;

	if (lastDaugthThisItem == NULL)
		return NULL;

	end_curr = SItemGetFloat(lastDaugthThisItem, "end", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemGetFloat\" failed"))
		goto quit_error;

	prevItem = SItemPrev(itemInSylRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPrev\" failed"))
		goto quit_error;

	if (prevItem == NULL)
	{
		extractedFeat = SObjectSetFloat(end_curr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectSetFloat\" failed"))
			goto quit_error;
	}
	else
	{
		lastDaugthPrevItem = SItemLastDaughter(prevItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemLastDaughter\" failed"))
			goto quit_error;

		if (lastDaugthPrevItem == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "Run",
					  "Previous syllable exists without a last daughter");
			goto quit_error;
		}

		end_prev = SItemGetFloat(lastDaugthPrevItem, "end", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemGetFloat\" failed"))
			goto quit_error;

		extractedFeat = SObjectSetFloat(end_curr - end_prev, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectSetFloat\" failed"))
			goto quit_error;
	}

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
/* SSylDurFeatProc class initialization                                             */
/*                                                                                  */
/************************************************************************************/

static SSylDurFeatProcClass SylDurFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SSylDurFeatProc",
		sizeof(SSylDurFeatProc),
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
