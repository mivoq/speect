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
/* DATE    : May 2011                                                               */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Segment name in multilingual format feature processor.                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "phoneset.h"
#include "seg_name_multilingual.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SSegNameMultilingualFeatProc class declaration. */
static SSegNameMultilingualFeatProcClass SegNameMultilingualFeatProcClass;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_bool segment_is_pause(const SItem *item, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_seg_name_multilingual_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&SegNameMultilingualFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_seg_name_multilingual_class_reg",
			  "Failed to register SSegNameMultilingualFeatProcClass");
}


S_LOCAL void _s_seg_name_multilingual_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&SegNameMultilingualFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_seg_name_multilingual_class_free",
			  "Failed to free SSegNameMultilingualFeatProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/* check if item is a pause in main language */
static s_bool segment_is_pause(const SItem *item, s_erc *error)
{
	const SPhoneset *phoneset;
	const SVoice *voice;
	s_bool is_pause;


	S_CLR_ERR(error);

	voice = SItemVoice(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "segment_is_pause",
				  "Call to \"SItemVoice\" failed"))
		return FALSE;

	if (voice == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "segment_is_pause",
				  "Item voice is NULL, voice is required to get phoneset");
		return FALSE;
	}

	phoneset = S_PHONESET(SVoiceGetData(voice, "phoneset", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "segment_is_pause",
				  "Call to \"SVoiceGetData\" failed"))
		return FALSE;

	if (phoneset == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "segment_is_pause",
				  "Phoneset is NULL, phoneset is required to get silence phone");
		return FALSE;
	}

	is_pause = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset,
															SItemGetName(item, error),
															"pause",
															error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "segment_is_pause",
				  "Call to \"phone_has_feature/SItemGetName\" failed"))
		return FALSE;

	return is_pause;
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
	const char *lang;
	const char *name;
	const SItem *tokenItem;
	char *lang_name = NULL;
	SObject *extractedFeat = NULL;
	s_bool is_pause;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	/* get item name */
	name = SItemGetString(item, "name", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemGetString\" failed"))
		return NULL;

	if (name == NULL)
		return NULL;


	is_pause = segment_is_pause(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"segment_is_pause\" failed"))
		return NULL;

	if (is_pause)
	{
		/* it is a pause in main language, so just return the name */
		extractedFeat = SObjectSetString(name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SObjectSetString\" failed"))
			return NULL;

		return extractedFeat;
	}

	/* not a pause */

	/* get item's token */
	tokenItem = s_path_to_item(item, "R:SylStructure.parent.parent.R:Token.parent",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_path_to_item\" failed"))
		return NULL;

	if (tokenItem == NULL)
		return NULL;

	/* get language feature */
	lang = SItemGetString(tokenItem, "lang", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemGetString\" failed"))
		return NULL;

	if (lang == NULL)
		return NULL;

	s_asprintf(&lang_name, error, "%s_%s", lang, name);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	extractedFeat = SObjectSetString(lang_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectSetString\" failed"))
	{
		S_FREE(lang_name);
		return NULL;
	}

	S_FREE(lang_name);
	return extractedFeat;

	self = NULL; /* compiler noise about unused parameters */
}


/************************************************************************************/
/*                                                                                  */
/* SSegNameMultilingualFeatProc class initialization                                */
/*                                                                                  */
/************************************************************************************/

static SSegNameMultilingualFeatProcClass SegNameMultilingualFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SSegNameMultilingualFeatProc",
		sizeof(SSegNameMultilingualFeatProc),
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
