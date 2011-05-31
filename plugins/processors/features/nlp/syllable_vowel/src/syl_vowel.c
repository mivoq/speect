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
/* Syllable vowel feature processor.                                                */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "syl_vowel.h"
#include "phoneset.h"
#include "plugin_info.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSylVowelFeatProcClass SylVowelFeatProcClass; /* SSylVowelFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static const SPhoneset *_get_phoneset(const SItem *item, s_bool *multilingual, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_syl_vowel_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&SylVowelFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_vowel_class_reg",
			  "Failed to register SSylVowelFeatProcClass");
}


S_LOCAL void _s_syl_vowel_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&SylVowelFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_vowel_class_free",
			  "Failed to free SSylVowelFeatProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static const SPhoneset *_get_phoneset(const SItem *item, s_bool *multilingual, s_erc *error)
{
	const SPhoneset *phoneset;
	const SVoice *voice;
	s_bool is_present;


	S_CLR_ERR(error);

	/* get the voice */
	voice = SItemVoice(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_get_phoneset",
				  "Call to \"SItemVoice\" failed"))
		return NULL;

	if (voice == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "_get_phoneset",
				  "Item voice is NULL, voice is required to get phoneset");
		return NULL;
	}

	/*
	 * do we have a 'voices' feature in the voice,
	 * i.e. is this a multilingual voice
	 */
	is_present = SVoiceFeatureIsPresent(voice, "voices", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_get_phoneset",
				  "Call to \"SVoiceFeatureIsPresent\" failed"))
		return NULL;

	if (is_present)
	{
		/* This is a multilingual voice.
		 * Get language feature of item, which is language feature
		 * of item's token.
		 */
		const SItem *tokenItem;
		const char *lang;
		const SMap *voicesMap;
		const SVoice *thisVoice;


		(*multilingual) = TRUE;
		tokenItem = s_path_to_item(item, "R:SylStructure.parent.R:Token.parent",
								   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_get_phoneset",
					  "Call to \"s_path_to_item\" failed"))
			return NULL;

		if (tokenItem == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "_get_phoneset",
					  "Failed to find item's token, which is required to get language feature");
			return NULL;
		}

		lang = SItemGetString(tokenItem, "lang", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_get_phoneset",
					  "Call to \"SItemGetString\" failed"))
			return NULL;

		/* now get the phoneset */
		voicesMap = (const SMap*)SVoiceGetFeature(voice, "voices", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_get_phoneset",
					  "Call to \"SVoiceGetFeature\" failed"))
			return NULL;

		thisVoice = (const SVoice*)SMapGetObjectDef(voicesMap, lang, NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_get_phoneset",
					  "Call to \"SMapGetObjectDef\" failed"))
			return NULL;

		if (thisVoice == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "_get_phoneset",
					  "Failed to find the voice for language '%s', which is required to get the phoneset", lang);
			return NULL;
		}

		phoneset = S_PHONESET(SVoiceGetData(thisVoice, "phoneset", error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "_get_phoneset",
					  "Call to \"SVoiceGetData\" failed"))
			return NULL;
	}
	else
	{
		/* not multilingual voice */
		(*multilingual) = FALSE;

		phoneset = S_PHONESET(SVoiceGetData(voice, "phoneset", error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SVoiceGetData\" failed"))
			return NULL;
	}

	if (phoneset == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "_get_phoneset",
				  "Item phoneset is NULL, required to extract phone features");
		return NULL;
	}

	return phoneset;
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
	const SItem *segment;
	const SPhoneset *phoneset;
	s_bool multilingual = FALSE;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	/* get the phoneset */
	phoneset = _get_phoneset(item, &multilingual, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"_get_phoneset\" failed"))
		return NULL;

	itemInSylStructRel = SItemAs(item, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	segment = SItemDaughter(itemInSylStructRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemDaughter\" failed"))
		goto quit_error;

	while (segment != NULL)
	{
		s_bool is_vowel;
		const char *item_name;


		item_name = SItemGetName(segment, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemGetName\" failed"))
			goto quit_error;

		if (item_name == NULL)
			continue;

		is_vowel = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset,
																item_name,
																"vowel",
																error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"phone_has_feature\" failed"))
			goto quit_error;

		if (is_vowel)
		{
			if (multilingual)
			{
				extractedFeat = s_path_to_featproc(segment,
												   "segment_name_multilingual",
												   error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to \"s_path_to_featproc\" failed"))
					goto quit_error;
			}
			else
			{
				extractedFeat = SObjectSetString(item_name, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to \"SObjectSetString\" failed"))
					goto quit_error;
			}

			return extractedFeat;
		}

		segment = SItemNext(segment, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	/* did not find a vowel */
	extractedFeat = SObjectSetString("novowel", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectSetString\" failed"))
		goto quit_error;

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
/* SSylVowelFeatProc class initialization                                           */
/*                                                                                  */
/************************************************************************************/

static SSylVowelFeatProcClass SylVowelFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SSylVowelFeatProc",
		sizeof(SSylVowelFeatProc),
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
