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
/* Stress feature processor.                                                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "syls_to_next_acc.h"

/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSylsToNextAccFeatProcClass SylsToNextAccFeatProcClass;
/* SSylsToNextAccFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_syls_to_next_acc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&SylsToNextAccFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syls_to_next_acc_class_reg",
			  "Failed to register SSylsToNextAccFeatProcClass");
}


S_LOCAL void _s_syls_to_next_acc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&SylsToNextAccFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syls_to_next_acc_class_free",
			  "Failed to free SSylsToNextAccFeatProcClass");
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
	int count = 0;
	s_bool found = FALSE;

	S_CLR_ERR(error);

	if (item == NULL)
		goto quit_error;

	SItem *iteratorSyl =  SItemPathToItem (item,  "R:Syllable.n", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "Run",
			  "Call to \"SItemPathToItem\" failed"))
		return NULL;

	while (found == FALSE && iteratorSyl != NULL)
	{
		count++;

		found = SItemFeatureIsPresent (iteratorSyl, "accent", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemFeatureIsPresent\" failed"))
			return NULL;

		iteratorSyl = SItemNext (iteratorSyl, error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPrev\" failed"))
			return NULL;
	}

	extractedFeat = SObjectSetInt ( count, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "Run",
			  "Call to \"SObjectSetInt\" failed"))
		return NULL;

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
/* SLongFeatproc class initialization                                               */
/*                                                                                  */
/************************************************************************************/

static SSylsToNextAccFeatProcClass SylsToNextAccFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SSylsToNextAccFeatProc",
		sizeof(SSylsToNextAccFeatProc),
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
