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
/* Stress feature processor.                                                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "stressed.h"
#include "../../plugins/linguistic/phoneset/src/phoneset.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SStressedFeatProcClass StressedFeatProcClass; /* SStressedFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_stressed_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&StressedFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_stressed_class_reg",
			  "Failed to register SStressedFeatProcClass");
}


S_LOCAL void _s_stressed_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&StressedFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_stressed_class_free",
			  "Failed to free SStressedFeatProcClass");
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
	const char* stress = "0";
	s_bool isVowel, isStressed;

	S_CLR_ERR(error);

	SItem *phone = item;

	if (item == NULL)
		goto quit_error;

	/* Extract Phoneset from Voice*/

	SVoice* voice = SItemVoice (item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemGetVoice\" failed"))
		goto quit_error;

	SPhoneset* phoneset = (SPhoneset*)SVoiceGetData(voice, "phoneset", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SVoiceGetData\" failed"))
		goto quit_error;

	const char* phone_value = SItemGetName(phone,error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemGetName\" failed"))
		goto quit_error;

	isVowel = S_PHONESET_CALL(phoneset, phone_has_feature)
							  (phoneset,
							   phone_value,
							   "vowel",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"phone_has_feature\" failed"))
		goto quit_error;

	if( isVowel == TRUE )
	{
		isStressed = S_PHONESET_CALL(phoneset, phone_has_feature)
								  (phoneset,
								   phone_value,
								   "stressed",
								   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"phone_has_feature\" failed"))
			goto quit_error;

		if(isStressed)
			stress = "+";
		else
			stress = "-";

	}

	extractedFeat = SObjectSetString(stress, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectSetInt\" failed"))
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
/* SStressedFeatproc class initialization                                     */
/*                                                                                  */
/************************************************************************************/

static SStressedFeatProcClass StressedFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SStressedFeatProc",
		sizeof(SStressedFeatProc),
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
