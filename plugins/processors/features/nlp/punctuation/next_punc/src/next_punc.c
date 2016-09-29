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
/* Phrase num words feature processor.                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "next_punc.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SNextPuncFeatProcClass NextPuncFeatProcClass; /* SNextPuncFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_next_punc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&NextPuncFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_next_punc_class_reg",
			  "Failed to register SNextPuncFeatProcClass");
}


S_LOCAL void _s_next_punc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&NextPuncFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_next_punc_class_free",
			  "Failed to free SNextPuncFeatProcClass");
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

static void Initialize(SFeatProcessor *self,const SMap* features, s_erc *error)
{
	S_CLR_ERR(error);

	SNextPuncFeatProc *castSelf = S_CAST(self, SNextPuncFeatProc, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to S_CAST failed"))
		goto quit_error;

	castSelf->symbols =  S_CAST( SMapGetObject(features , "list punctuation", error),
								SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "Initialize",
			  "Call to \"SMapGetObject\" failed"))
		goto quit_error;

	castSelf->symbols = SMapCopy ( NULL , castSelf->symbols , error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "Initialize",
			  "Call to \"SMapCopy\" failed"))
		goto quit_error;

	/* error cleanup */
	quit_error:
		return;
}

static SObject *Run(const SFeatProcessor *self, const SItem *item,
					s_erc *error)
{
	SObject *extractedFeat = NULL;
	const SItem *itrItem;

	SNextPuncFeatProc *castSelf = S_CAST(self, SNextPuncFeatProc, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to S_CAST failed"))
		goto quit_error;

	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	itrItem = SItemPathToItem (item, "R:Token.parent", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to \"SItemPathToItem\" failed"))
		goto quit_error;

	s_bool found = FALSE;

	while (found == FALSE && itrItem != NULL)
	{
		const char * tokenKey = SItemGetName ( itrItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemGetName\" failed"))
			goto quit_error;

		s_bool isPresent = SMapObjectPresent ( castSelf->symbols, tokenKey, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SMapObjectPresent\" failed"))
			goto quit_error;

		if( isPresent )
		{
			found = TRUE;

			const char * result = SMapGetString ( castSelf->symbols, tokenKey, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SMapGetString\" failed"))
				goto quit_error;

			extractedFeat = SObjectSetString ( result, error );
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SObjectSetString\" failed"))
				goto quit_error;
		}

		itrItem = SItemNext(itrItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
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
/* SNextPuncFeat class initialization                                   */
/*                                                                                  */
/************************************************************************************/

static SNextPuncFeatProcClass NextPuncFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SNextPuncFeatProc",
		sizeof(SNextPuncFeatProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SFeatProcessorClass */
	Initialize,               /* initialize */
	Run                  /* run     */
};
