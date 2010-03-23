/************************************************************************************/
/* Copyright (c) 2010 The Department of Arts and Culture,                           */
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
/* DATE    : March 2010                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Syllable stress in feature processor.                                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "syl_stress_in.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSylStressInFeatProcClass SylStressInFeatProcClass; /* SSylStressInFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_bool syl_is_stressed(const SItem *syl, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_syl_stress_in_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&SylStressInFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_stress_in_class_reg",
			  "Failed to register SSylStressInFeatProcClass");
}


S_LOCAL void _s_syl_stress_in_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&SylStressInFeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_stress_in_class_free",
			  "Failed to free SSylStressInFeatProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/*
 * this is for Georg's stress levels:
 * unstressed : FALSE
 * primary    : TRUE
 * secondary  : TRUE
 */
static s_bool syl_is_stressed(const SItem *syl, s_erc *error)
{
	const char *stress_feat;


	S_CLR_ERR(error);
	stress_feat = SItemGetString(syl, "stress", error);
	if (S_CHK_ERR(error, S_CONTERR,
					"syl_is_stressed",
					"Call to \"SItemGetString\" failed"))
		return FALSE;

	if (s_strcmp(stress_feat, "unstressed", error) == 0)
	{
		S_CHK_ERR(error, S_CONTERR,
				  "syl_is_stressed",
				  "Call to \"s_strcmp\" failed");
		return FALSE;
	}

	if (s_strcmp(stress_feat, "primary", error) == 0)
	{
		if (S_CHK_ERR(error, S_CONTERR,
					  "syl_is_stressed",
					  "Call to \"s_strcmp\" failed"))
			return FALSE;

		return TRUE;
	}

	if (s_strcmp(stress_feat, "secondary", error) == 0)
	{
		if (S_CHK_ERR(error, S_CONTERR,
					  "syl_is_stressed",
					  "Call to \"s_strcmp\" failed"))
			return FALSE;

		return TRUE;
	}

	return FALSE; /* unknown */
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
	const SItem *phraseItem;
	const SItem *wordItem;
	const SItem *sylStructWordItem;
	const SItem *syllableItem;
	sint32 num_stressed = 0;
	s_bool is_current_syl = FALSE;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	/* get current phrase */
	phraseItem = s_path_to_item(item, "R:SylStructure.parent.R:Phrase.parent",
								error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_path_to_item\" failed"))
		goto quit_error;

	if (phraseItem == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to get phrase of given syllable");
		goto quit_error;
	}

	wordItem = SItemDaughter(phraseItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemDaughter\" failed"))
		goto quit_error;

	while (wordItem != NULL)
	{
		sylStructWordItem = SItemAs(wordItem, "SylStructure", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemAs\" failed"))
			goto quit_error;

		/* get syllables */
		syllableItem = SItemDaughter(sylStructWordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemDaughter\" failed"))
			goto quit_error;

		while (syllableItem != NULL)
		{
			s_bool is_stressed;


			is_current_syl = SItemEqual(syllableItem, item, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemEqual\" failed"))
				goto quit_error;

			if (is_current_syl)
				break;

			is_stressed = syl_is_stressed(syllableItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"syl_is_stressed\" failed"))
				goto quit_error;

			if (is_stressed)
				num_stressed++;

			syllableItem = SItemNext(syllableItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemNext\" failed"))
				goto quit_error;
		}

		if (is_current_syl)
			break;

		wordItem = SItemNext(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	extractedFeat = SObjectSetInt(num_stressed, error);
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

	self = NULL; /* compiler noise about unused parameters */
	return NULL;
}


/************************************************************************************/
/*                                                                                  */
/* SwordDurFeatProc class initialization                                            */
/*                                                                                  */
/************************************************************************************/

static SSylStressInFeatProcClass SylStressInFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SSylStressInFeatProc",
		sizeof(SSylStressInFeatProc),
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
