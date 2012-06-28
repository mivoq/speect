/************************************************************************************/
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
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
/* DATE    : June 2012                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A simple sentence boundary detection feature processor.                          */
/* Split text based on periods.                                                     */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "sentence_boundary_simple.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SSentenceBoundarySimpleFeatProc class declaration. */

static SSentenceBoundarySimpleFeatProcClass SentenceBoundarySimpleFeatProcClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_sentence_boundary_simple_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&SentenceBoundarySimpleFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_sentence_boundary_simple_class_reg",
			  "Failed to register SSentenceBoundarySimpleFeatProcClass");
}


S_LOCAL void _s_sentence_boundary_simple_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&SentenceBoundarySimpleFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_sentence_boundary_simple_class_free",
			  "Failed to free SSentenceBoundarySimpleFeatProcClass");
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
	SItem *modifiedItem = (SItem*)item; /* we are modifying the
										 * incomming item */
	const char *text = NULL;
	int counter = 0;
	const char *delimiter = ".";
	const char *sentence = NULL;
	char *context = NULL;
	char *input_copy = NULL;
	const char *count_str = "sentence_%d";


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	text = SItemGetString(item, "text", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemGetString\" failed"))
		return NULL;

	/* split text based on periods */
	input_copy = s_strdup(text, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_strdup\" failed"))
		return NULL;

	sentence = s_strtok_r(input_copy, delimiter, &context, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_strdup\" failed"))
	{
		S_FREE(input_copy);
		return NULL;
	}

	while (sentence != NULL)
	{
		char *tmp;


		s_asprintf(&tmp, error, count_str, counter++);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_asprintf\" failed"))
		{
			S_FREE(input_copy);
			return NULL;
		}

		SItemSetString(modifiedItem, tmp, sentence, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemSetString\" failed"))
		{
			S_FREE(input_copy);
			S_FREE(tmp);
			return NULL;
		}

		S_FREE(tmp);

		sentence = s_strtok_r(NULL, delimiter, &context, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_strdup\" failed"))
		{
			S_FREE(input_copy);
			return NULL;
		}
	}

	S_FREE(input_copy);
	SItemSetInt(modifiedItem, "sentences", counter, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Run",
			  "Call to \"SItemSetInt\" failed");

	return NULL;

	S_UNUSED(self);
}


/************************************************************************************/
/*                                                                                  */
/* SSentenceBoundarySimpleFeatProc class initialization                             */
/*                                                                                  */
/************************************************************************************/

static SSentenceBoundarySimpleFeatProcClass SentenceBoundarySimpleFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SSentenceBoundarySimpleFeatProc",
		sizeof(SSentenceBoundarySimpleFeatProc),
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
