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
/* A pause insertion utterance processor.                                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "pauses_proc.h"
#include "phoneset.h"

/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SPausesUttProc class declaration. */
static SPausesUttProcClass PausesUttProcClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_pauses_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&PausesUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_pauses_utt_proc_class_reg",
			  "Failed to register SPausesUttProcClass");
}


S_LOCAL void _s_pauses_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&PausesUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_pauses_utt_proc_class_free",
			  "Failed to free SPausesUttProcClass");
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
	const SRelation *wordRel;
	const SRelation *segmentRel;
	const SRelation *phraseRel;
	const SPhoneset *phoneset;
	const SVoice *voice;
	const char *silence_phone;
	const SObject *tmp;
	SItem *segmentItem;
	const SItem *phraseItem;
	const SItem *wordItem;
	SItem *silenceItem;


	S_CLR_ERR(error);

	/* we don't use this, set to NULL so that compiler doesn't
	 * complain.
	 */
	self = NULL;

	/* we require the word, segment and phrase relation */
	wordRel = SUtteranceGetRelation(utt, "Word", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
		return;

	if (wordRel == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Word' relation (required) from utterance");
		return;
	}

	segmentRel = SUtteranceGetRelation(utt, "Segment", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
		return;

	if (segmentRel == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Segment' relation (required) from utterance");
		return;
	}

	phraseRel = SUtteranceGetRelation(utt, "Phrase", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
		return;

	if (phraseRel == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Phrase' relation (required) from utterance");
		return;
	}

	voice = SUtteranceVoice(utt, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceVoice\" failed"))
		return;

	if (voice == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find voice (required) from utterance");
		return;
	}

	phoneset = (SPhoneset*)SVoiceGetData(voice, "phoneset", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SVoiceGetFeature\" failed"))
		return;

	if (phoneset == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find phoneset (required) from voice");
		return;
	}

	tmp = S_PHONESET_CALL(phoneset, get_feature)(phoneset, "silence_phone", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to method \"get_feature\" (SPhoneset) failed"))
		return;

	if (tmp == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'silence_phone' (required) from phoneset");
		return;
	}

	silence_phone = SObjectGetString(tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectGetString\" failed"))
		return;

	/* Insert initial silence */
	segmentItem = (SItem*)SRelationHead(segmentRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		return;

	if (segmentItem == NULL)
	{
		silenceItem = SRelationAppend((SRelation*)segmentRel, NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SRelationAppend\" failed"))
			return;
	}
	else
	{
		silenceItem = SItemPrepend(segmentItem, NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemPrepend\" failed"))
			return;
	}

	SItemSetName(silenceItem, silence_phone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemSetName\" failed"))
		return;

	phraseItem = SRelationHead(phraseRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		return;

	while (phraseItem != NULL)
	{
		wordItem = SItemLastDaughter(phraseItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemLastDaughter\" failed"))
			return;

		while (wordItem != NULL)
		{
			segmentItem = (SItem*)s_path_to_item(wordItem,
												 "R:SylStructure.daughtern.daughtern.R:Segment",
												 error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"s_path_to_item\" failed"))
				return;

			if (segmentItem != NULL)
			{
				segmentItem = SItemAppend(segmentItem, NULL, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to \"SItemAppend\" failed"))
					return;

				SItemSetName(segmentItem, silence_phone, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to \"SItemSetName\" failed"))
					return;

				break;
			}

			wordItem = SItemPrev(wordItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemPrev\" failed"))
				return;
		}

		phraseItem = SItemNext(phraseItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			return;
	}


	/* here all is OK */
	return;
}


/************************************************************************************/
/*                                                                                  */
/* SPausesUttProc class initialization                                              */
/*                                                                                  */
/************************************************************************************/

static SPausesUttProcClass PausesUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:SPausesUttProc",
		sizeof(SPausesUttProc),
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
