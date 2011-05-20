/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
/* Syllable position type feature processor.                                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "syl_pos_type.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSylPosTypeFeatProcClass SylPosTypeFeatProcClass; /* SSylPosTypeFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_syl_pos_type_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&SylPosTypeFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_pos_type_class_reg",
			  "Failed to register SSylPosTypeFeatProcClass");
}


S_LOCAL void _s_syl_pos_type_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&SylPosTypeFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_pos_type_class_free",
			  "Failed to free SSylPosTypeFeatProcClass");
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
	const SItem *itemInSylStructRel;
	const SItem *prevItem;
	const SItem *nextItem;


	S_CLR_ERR(error);
	if (item == NULL)
		return NULL;

	itemInSylStructRel = SItemAs(item, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	if (itemInSylStructRel == NULL)
		return NULL;

	nextItem = SItemNext(itemInSylStructRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemNext\" failed"))
		goto quit_error;

	prevItem = SItemPrev(itemInSylStructRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPrev\" failed"))
		goto quit_error;

	if (nextItem == NULL)
	{
		if (prevItem == NULL)
		{
			extractedFeat = SObjectSetString("single", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SObjectSetString\" failed"))
				goto quit_error;
		}
		else
		{
			extractedFeat = SObjectSetString("final", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SObjectSetString\" failed"))
				goto quit_error;
		}
	}
	else if (prevItem == NULL)
	{
		extractedFeat = SObjectSetString("initial", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectSetString\" failed"))
			goto quit_error;
	}
	else
	{
		extractedFeat = SObjectSetString("mid", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectSetString\" failed"))
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
/* SSylPosTypeFeatProc class initialization                                         */
/*                                                                                  */
/************************************************************************************/

static SSylPosTypeFeatProcClass SylPosTypeFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SSylPosTypeFeatProc",
		sizeof(SSylPosTypeFeatProc),
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
