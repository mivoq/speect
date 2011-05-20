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
	const SVoice *voice;
	const SPhoneset *phoneset;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	/* get the phoneset */
	voice = SItemVoice(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemVoice\" failed"))
		goto quit_error;

	if (voice == NULL)
		return NULL;

	phoneset = S_PHONESET(SVoiceGetData(voice, "phoneset", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SVoiceGetData\" failed"))
		goto quit_error;

	if (phoneset == NULL)
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
			extractedFeat = SObjectSetString(item_name, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SObjectSetString\" failed"))
				goto quit_error;

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

	self = NULL; /* compiler noise about unused parameters */
	return NULL;
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
