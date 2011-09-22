/************************************************************************************/
/* Copyright (c) 2011 The Department of Arts and Culture,                           */
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
/* DATE    : September 2011                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Segment phoneset feature, feature processor.                                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "seg_ph_feat.h"
#include "phoneset.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSegPhonesetFeatureFeatProcClass SegPhonesetFeatureFeatProcClass; /* SSegPhonesetFeatureFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_seg_ph_feat_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&SegPhonesetFeatureFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_seg_ph_feat_class_reg",
			  "Failed to register SSegPhonesetFeatureFeatProcClass");
}


S_LOCAL void _s_seg_ph_feat_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&SegPhonesetFeatureFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_seg_ph_feat_class_free",
			  "Failed to free SSegPhonesetFeatureFeatProcClass");
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
	const SVoice *voice;
	const SPhoneset *phoneset;
	const char *feature;
	const char *begin;
	const char *token;
	s_bool feature_value;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	voice = SItemVoice(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemVoice\" failed"))
		return NULL;

	if (voice == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Item voice is NULL, voice is required to get phoneset");
		return NULL;
	}

	phoneset = S_PHONESET(SVoiceGetData(voice, "phoneset", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SVoiceGetData\" failed"))
		return NULL;

	if (phoneset == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Phoneset is NULL, phoneset is required to get phone feature");
		return NULL;
	}

	token = SItemGetString(item, "_phoneset_feature", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemGetString\" failed"))
		return NULL;

	/* get part of token (actual feature), after _ */
	begin = s_strchr(token, '_', error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemPath",
				  "Call to \"s_strchr\" failed"))
		return NULL;

	if (begin != NULL)
	{
		feature = begin + 1;
	}
	else
	{
		/* no feature */
		return NULL;
	}

	feature_value = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset,
																 SItemGetName(item, error),
																 feature,
																 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"phone_has_feature/SItemGetName\" failed"))
		return NULL;

	if (feature_value == TRUE)
	{
		extractedFeat = SObjectSetInt(1, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectSetInt\" failed"))
			return NULL;
	}
	else
	{
		extractedFeat = SObjectSetInt(0, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectSetInt\" failed"))
			return NULL;
	}

	return extractedFeat;


	self = NULL; /* compiler noise about unused parameters */
	return NULL;
}


/************************************************************************************/
/*                                                                                  */
/* SSegPhonesetFeatureFeatProc class initialization                                 */
/*                                                                                  */
/************************************************************************************/

static SSegPhonesetFeatureFeatProcClass SegPhonesetFeatureFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SSegPhonesetFeatureFeatProc",
		sizeof(SSegPhonesetFeatureFeatProc),
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
