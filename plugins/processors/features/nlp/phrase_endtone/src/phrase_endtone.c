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
/* Phrase type feature processor.                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "phrase_endtone.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SPhraseEndtoneFeatProcClass PhraseEndtoneFeatProcClass;

/* SPhraseEndtoneFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_phrase_endtone_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&PhraseEndtoneFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phrase_endtone_class_reg",
			  "Failed to register SPhraseEndtoneFeatProcClass");
}


S_LOCAL void _s_phrase_endtone_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&PhraseEndtoneFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phrase_endtone_class_free",
			  "Failed to free SPhraseEndtoneFeatProcClass");
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
	s_bool found = FALSE;

	SItem *boundary = SItemLastDaughter ( item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemLastDaughter\" failed"))
		goto quit_error;

	boundary = SItemAs (boundary, "Boundaries", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	SItem * boundaryNext = SItemNext (boundary, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemNext\" failed"))
		goto quit_error;

	if (boundaryNext != NULL)
		boundary = boundaryNext;

	while (!found && boundary != NULL)
	{
		found = SItemFeatureIsPresent( boundary, "tobi_endtone", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemFeatureIsPresent\" failed"))
			goto quit_error;

		if (!found)
		{
			boundary = SItemPrev(boundary, error );
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemPrev\" failed"))
				goto quit_error;
		}
	}

	if ( found )
	{
		const char* tone = SItemGetString( boundary, "tobi_endtone", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemGetFeature\" failed"))
			goto quit_error;

		extractedFeat = SObjectSetString( tone, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectSetString\" failed"))
			goto quit_error;
	}

	return extractedFeat;

	/* error cleanup */
quit_error:
	if (extractedFeat != NULL)
		S_DELETE(extractedFeat, "Run", error);

	return NULL;

	S_UNUSED(self);
	S_UNUSED(item);
}


/************************************************************************************/
/*                                                                                  */
/* SPhraseEndtoneFeatProc class initialization                                         */
/*                                                                                  */
/************************************************************************************/

static SPhraseEndtoneFeatProcClass PhraseEndtoneFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SPhraseEndtoneFeatProc",
		sizeof(SPhraseEndtoneFeatProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SFeatProcessorClass */
	NULL,                /* initialize */
	Run                  /* run     */
};
