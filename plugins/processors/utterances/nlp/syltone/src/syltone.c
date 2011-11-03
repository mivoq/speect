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
/* DATE    : February 2010                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A syllable tone, post lex, utterance processor.                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "syltone.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSylToneUttProcClass SylToneUttProcClass; /* SSylToneUttProcClass class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_syltone_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&SylToneUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_sylltone_utt_proc_class_reg",
			  "Failed to register SSylToneUttProcClass");
}


S_LOCAL void _s_syltone_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&SylToneUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_sylltone_utt_proc_class_free",
			  "Failed to free SSylToneUttProcClass");
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


static void Run(const SUttProcessor *self, SUtterance *utt,
				s_erc *error)
{
	const SRelation *sylRel;
	const SRelation *sylStructRel;
	const SItem *sylItr;
	const SItem *word;
	const SItem *sylAsSylStruct;
	const SItem *tone;
	const SItem *toneAsSegment;
	const SItem *pos;
	const SItem *posAsSegment;
	s_bool is_present;


	S_CLR_ERR(error);


	/* we require the SylStructure relation */
	is_present = SUtteranceRelationIsPresent(utt, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto quit_error;

	if (!is_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Syllable' relation in utterance");
		goto quit_error;
	}

	sylStructRel = SUtteranceGetRelation(utt, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
		goto quit_error;

	/* and the Syllable relation */
	is_present = SUtteranceRelationIsPresent(utt, "Syllable", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto quit_error;

	if (!is_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Syllable' relation in utterance");
		goto quit_error;
	}

	sylRel = SUtteranceGetRelation(utt, "Syllable", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
		goto quit_error;

	/* first sort out the word POS */
	word = SRelationHead(sylStructRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	while (word != NULL)
	{
		/* get the last syllable */
		sylAsSylStruct = SItemLastDaughter(word, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemLastDaughter\" failed"))
			goto quit_error;

		/* and as in the Syllable relation */
		sylItr = SItemAs(sylAsSylStruct, "Syllable", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemAs\" failed"))
			goto quit_error;

		/* get the syllable's daughter */
		pos = SItemDaughter(sylAsSylStruct, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemDaughter\" failed"))
			goto quit_error;

		/* and as it is in the segment relation */
		posAsSegment = SItemAs(pos, "Segment", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemAs\" failed"))
			goto quit_error;

		/* set word pos, which is syllable's daughter's name */
		SItemSetObject((SItem*)word, "pos",
					   SItemGetObject(pos, "name", error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemSetObject/SItemGetObject\" failed"))
			goto quit_error;

		/* delete the pos */
		S_FORCE_DELETE(pos, "Run", error);
		S_FORCE_DELETE(posAsSegment, "Run", error);

		/* delete the syllable */
		S_FORCE_DELETE(sylItr, "Run", error);
		S_FORCE_DELETE(sylAsSylStruct, "Run", error);

		word = SItemNext(word, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	/* now sort out syllable tone */
	sylItr = SRelationHead(sylRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	while (sylItr != NULL)
	{
		sylAsSylStruct = SItemAs(sylItr, "SylStructure", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemAs\" failed"))
			goto quit_error;

		tone = SItemLastDaughter(sylAsSylStruct, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemLastDaughter\" failed"))
			goto quit_error;

		SItemSetObject((SItem*)sylAsSylStruct, "tone",
					   SItemGetObject(tone, "name", error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemSetObject/SItemGetObject\" failed"))
			goto quit_error;

		toneAsSegment = SItemAs(tone, "Segment", error);

		S_FORCE_DELETE(tone, "Run", error);
		S_FORCE_DELETE(toneAsSegment, "Run", error);

		sylItr = SItemNext(sylItr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	/* here all is OK */
	return;

	/* error clean-up code */
quit_error:
	S_UNUSED(self);
}


/************************************************************************************/
/*                                                                                  */
/* STokenizationUttProc class initialization                                        */
/*                                                                                  */
/************************************************************************************/

static SSylToneUttProcClass SylToneUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:SSylToneUttProc",
		sizeof(SSylToneUttProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SUttProcessorClass */
	NULL,                /* initialize */
	Run                  /* run        */
};
