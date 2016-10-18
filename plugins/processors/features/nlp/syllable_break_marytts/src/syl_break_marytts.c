/************************************************************************************/
/* Copyright (c) Mivoq SRL                                                          */
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/* Contributors:  Federico Rossetto                                                 */
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
/* DATE    : October 2016                                                           */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Syllable break feature processor.                                                */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "syl_break_marytts.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSylBreakMaryttsFeatProcClass SylBreakMaryttsFeatProcClass; /* SSylBreakMaryttsFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_syl_break_marytts_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&SylBreakMaryttsFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_break_marytts_class_reg",
			  "Failed to register SSylBreakMaryttsFeatProcClass");
}


S_LOCAL void _s_syl_break_marytts_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&SylBreakMaryttsFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_break_marytts_class_free",
			  "Failed to free SSylBreakMaryttsFeatProcClass");
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
	const SItem *nextItem;
	const SItem *wordItem;
	const SItem *boundaryItem;
	int scomp;
	const char* break_name;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	itemInSylStructRel = SItemAs(item, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	if (itemInSylStructRel == NULL)
	{
		/* no sylstructure so maybe its standalone */
		extractedFeat = SObjectSetInt(1, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectSetInt\" failed"))
			goto quit_error;

		return extractedFeat;
	}

	nextItem = SItemNext(itemInSylStructRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemNext\" failed"))
		goto quit_error;

	if (nextItem != NULL)
	{
		/* word internal */
		extractedFeat = SObjectSetInt(0, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectSetInt\" failed"))
			goto quit_error;

		return extractedFeat;
	}

	wordItem = SItemParent(itemInSylStructRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemParent\" failed"))
		goto quit_error;

	if (wordItem == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Given syllable item does not have a parent word item");
		goto quit_error;
	}

	boundaryItem = SItemAs (wordItem, "Boundaries", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	if (boundaryItem == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Given word item does not have a associated boundary item");
		goto quit_error;
	}

	nextItem = SItemNext (boundaryItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemNext\" failed"))
		goto quit_error;

	if ( nextItem == NULL )
	{
		extractedFeat = SObjectSetInt ( 1, error );
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectSetInt\" failed"))
			goto quit_error;
	}
	else
	{
		break_name = SItemGetName (nextItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemGetName\" failed"))
			goto quit_error;

		scomp = s_strcmp(break_name, "Boundary", error) == 0;
		if ( scomp )
		{
			int tmp  = SItemGetInt (nextItem, "breakindex", error );
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemGetInt\" failed"))
				goto quit_error;

			if (tmp > 4)
			{
				extractedFeat = SObjectSetInt ( 4, error );
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to \"SObjectSetInt\" failed"))
					goto quit_error;
			}
			else
			{
				extractedFeat = SObjectSetInt ( tmp, error );
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to \"SObjectSetInt\" failed"))
					goto quit_error;
			}

		}
		else
		{
			extractedFeat = SObjectSetInt (1, error );
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemGetName\" failed"))
				goto quit_error;
		}
	}

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
/* SwordDurFeatProc class initialization                                            */
/*                                                                                  */
/************************************************************************************/

static SSylBreakMaryttsFeatProcClass SylBreakMaryttsFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SSylBreakMaryttsFeatProc",
		sizeof(SSylBreakMaryttsFeatProc),
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
