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
/* Segment position in current word feature processor.                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "seg_pos_word.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSegPosWordFeatProcClass SegPosWordFeatProcClass; /* SSegPosWordFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_seg_pos_word_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&SegPosWordFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_seg_pos_word_class_reg",
			  "Failed to register SSegPosWordFeatProcClass");
}


S_LOCAL void _s_seg_pos_word_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&SegPosWordFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_seg_pos_word_class_free",
			  "Failed to free SSegPosWordFeatProcClass");
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
	int count = -1;

	S_CLR_ERR(error);

	if( item == NULL )
		goto quit_error;

	SItem *phone = SItemPathToItem (item, "R:SylStructure", error);;
	if (S_CHK_ERR(error, S_CONTERR,
			  "Run",
			  "Call to \"SItemPathToItem\" failed"))
		goto quit_error;

	SItem *sylStart = SItemPathToItem (item, "R:SylStructure.parent.p", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "Run",
			  "Call to \"SItemPathToItem\" failed"))
		goto quit_error;

	SItem *sylEnd = SItemPathToItem (item, "R:SylStructure.parent.parent.p.daughter", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "Run",
			  "Call to \"SItemPathToItem\" failed"))
		goto quit_error;

	/* Phones left in the current syllable*/
	while (phone != NULL)
	{
		count++;
		phone = SItemPrev ( phone, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPrev\" failed"))
			goto quit_error;
	}

	/* Count Phones from the next syllables of the word */
	while ( sylStart != sylEnd && sylStart != NULL )
	{
		phone = SItemPathToItem (sylStart, "daughter", error);;
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPathToItem\" failed"))
			goto quit_error;

		while (phone != NULL)
		{
			count++;
			phone = SItemNext ( phone, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
				goto quit_error;
		}

		sylStart = SItemPrev (sylStart ,error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPrev\" failed"))
			goto quit_error;
	}

	if( count > -1 )
	{
		extractedFeat = SObjectSetInt ( count, error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectSetInt\" failed"))
			goto quit_error;

		return extractedFeat;
	}

	return NULL;

	/* error cleanup */
quit_error:
	if (extractedFeat != NULL)
		S_DELETE(extractedFeat, "Run", error);

	return NULL;

	S_UNUSED(self);
}


/************************************************************************************/
/*                                                                                  */
/* SSegPosWordFeatproc class initialization                                     */
/*                                                                                  */
/************************************************************************************/

static SSegPosWordFeatProcClass SegPosWordFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SSegPosWordFeatProc",
		sizeof(SSegPosWordFeatProc),
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
