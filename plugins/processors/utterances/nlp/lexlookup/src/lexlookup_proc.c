/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                Simone Daminato                                                   */
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
/* A LexLookup utterance processor.                                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "syllabification.h"
#include "addendum.h"
#include "lexicon.h"
#include "g2p.h"
#include "lexlookup_proc.h"
#include "hrg/processors/featprocessor.h"
#include "phoneset.h"

/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SLexLookupUttProcClass LexLookupUttProcClass; /* SLexLookupUttProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void s_get_lexical_objects(const SUttProcessor *self, SUtterance *utt,
								  SG2P **g2p, SLexicon **lexicon, SAddendum **addendum,
								  SSyllabification **syllab, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_lexlookup_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&LexLookupUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_lexlookup_utt_proc_class_reg",
			  "Failed to register SLexLookupUttProcClass");
}


S_LOCAL void _s_lexlookup_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&LexLookupUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_lexlookup_utt_proc_class_free",
			  "Failed to free SLexLookupUttProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void s_get_lexical_objects(const SUttProcessor *self, SUtterance *utt,
								  SG2P **g2p, SLexicon **lexicon, SAddendum **addendum,
								  SSyllabification **syllab, s_erc *error)
{
	const SVoice *voice;


	S_CLR_ERR(error);

	voice = SUtteranceVoice(utt, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_lexical_objects",
				  "Call to \"SUtteranceVoice\" failed"))
		return;

	*g2p = (SG2P*)SVoiceGetData(voice , "g2p", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_lexical_objects",
				  "Call to \"SVoiceGetData\" failed"))
		return;

	*lexicon = (SLexicon*)SVoiceGetData(voice , "lexicon", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_lexical_objects",
				  "Call to \"SVoiceGetData\" failed"))
		return;

	*addendum = (SAddendum*)SVoiceGetData(voice , "addendum", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_lexical_objects",
				  "Call to \"SVoiceGetData\" failed"))
		return;

	/* first try for syllabification in voice data, new method */
	*syllab = (SSyllabification*)SVoiceGetData(voice , "syllabification", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_lexical_objects",
				  "Call to \"SVoiceGetData\" failed"))
		return;

	/* if not found try old method */
	if ((*syllab) == NULL)
	{
		*syllab = (SSyllabification*)SMapGetObjectDef(self->features , "_syll_func", NULL,
													  error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_lexical_objects",
					  "Call to \"SMapGetObjectDef\" failed"))
			return;
	}

	/* now check for least requirements */
	if ((*addendum == NULL)
		&& (*lexicon == NULL)
		&& (*g2p == NULL))
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_lexical_objects",
				  "No grapheme to phoneme conversion options (lexicon, addendum, g2p) "
				  "found in voice");
		return;
	}
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

/* we need to delete the syllabification function and plug-in if any */
static void Destroy(void *obj, s_erc *error)
{
	SUttProcessor *self = obj;
	const SObject *tmp;
	SPlugin *sylPlugin;


	S_CLR_ERR(error);

	/* check if a syllabification plug-in is defined as a feature */
	tmp = SMapGetObjectDef(self->features, "_syll_func_plugin", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Destroy",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (tmp == NULL)
		return;

	/* unlink it */
	sylPlugin = (SPlugin*)SMapObjectUnlink(self->features, "_syll_func_plugin", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Destroy",
				  "Call to \"SMapObjectUnlink\" failed"))
		return;

	/* get the syllabification function */
	tmp = SMapGetObjectDef(self->features, "_syll_func", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Destroy",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (tmp != NULL)
	{
		SMapObjectDelete(self->features, "_syll_func", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Destroy",
					  "Call to \"SMapObjectDelete\" failed"))
			return;
	}

	/* delete the plug-in */
	S_DELETE(sylPlugin, "Destroy", error);

	/* check if a stress plug-in is defined as a feature */
	tmp = SMapGetObjectDef(self->features, "_stress_func_plugin", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Destroy",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (tmp == NULL)
		return;

	/* unlink it */
	sylPlugin = (SPlugin*)SMapObjectUnlink(self->features, "_stress_func_plugin", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Destroy",
				  "Call to \"SMapObjectUnlink\" failed"))
		return;

	/* get the stress function */
	tmp = SMapGetObjectDef(self->features, "_stress_func", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Destroy",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (tmp != NULL)
	{
		SMapObjectDelete(self->features, "_stress_func", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Destroy",
					  "Call to \"SMapObjectDelete\" failed"))
			return;
	}

	/* delete the plug-in */
	S_DELETE(sylPlugin, "Destroy", error);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}

static void s_compute_stresses ( const SFeatProcessor* proc, SItem* word, s_erc *error )
{
	S_CLR_ERR(error);
	SItem *wordAsSylStructure = SItemAs(word, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_compute_stresses",
		      "Call to \"SItemAs\" failed"))
		return;

	SItem *syllable = SItemDaughter(wordAsSylStructure, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_compute_stresses",
		      "Call to \"SItemDaughter\" failed"))
		return;

	SObject* result = NULL;
	while (syllable != NULL)
	{
		result = SFeatProcessorRun ( proc, syllable, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_compute_stresses",
					  "Call to \"SItemPathToFeatProc\" failed"))
			goto s_compute_stresses_cleanup;

		const char* resultString = SObjectGetString ( result, error );
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_compute_stresses",
					  "Call to \"SObjectGetInt\" failed"))
			goto s_compute_stresses_cleanup;

		SItemSetString ( syllable, "stress", resultString, error );
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_compute_stresses",
					  "Call to \"SItemSetInt\" failed"))
			goto s_compute_stresses_cleanup;

		syllable = SItemNext ( syllable, error );
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_compute_stresses",
					  "Call to \"SItemNext\" failed"))
			goto s_compute_stresses_cleanup;
		if(result != NULL) {
			S_DELETE(result, "s_compute_stresses", error);
		}
	}
s_compute_stresses_cleanup:
	if(result != NULL) {
		S_DELETE(result, "s_compute_stresses", error);
	}

}

static void s_compute_phonetic_features (SItem* word, s_erc *error )
{
	SItem *syllable;
	SItem * phone;
	char* position_in_syllable_string = NULL;

	/* Extract Phoneset from Voice*/
	const SVoice* voice = SItemVoice (word, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_compute_phonetic_features",
				  "Call to \"SItemGetVoice\" failed"))
		return;

	const SPhoneset* phoneset = (SPhoneset*)SVoiceGetData(voice, "phoneset", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_compute_phonetic_features",
				  "Call to \"SVoiceGetData\" failed"))
		return;
	SItem *wordAsSylStructure = SItemAs(word, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_compute_stresses",
		      "Call to \"SItemAs\" failed"))
		return;

	syllable = SItemDaughter(wordAsSylStructure, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_compute_stresses",
		      "Call to \"SItemDaughter\" failed"))
		return;

	while (syllable != NULL)
	{
		phone = SItemDaughter(syllable, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "s_compute_phonetic_features",
			      "Call to \"SItemDaughter\" failed"))
			return;

		s_bool nucleusFound = FALSE;
		while (phone != NULL)
		{
			const char* phone_value = SItemGetName(phone, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "s_compute_phonetic_features",
				      "Call to \"SItemGetName\" failed"))
				return;
			s_bool isVowel = S_PHONESET_CALL(phoneset, phone_has_feature)
				(phoneset,
				 phone_value,
				 "vowel",
				 error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "s_compute_phonetic_features",
				      "Call to \"phone_has_feature\" failed"))
				return;

			if( isVowel )
			{
				nucleusFound = TRUE;
				position_in_syllable_string = "nucleus";
			}
			else
			{
				if( nucleusFound == TRUE )
					position_in_syllable_string = "coda";
				else
					position_in_syllable_string = "onset";
			}

			SItemSetString ( phone, "syllablepart", position_in_syllable_string, error );
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_compute_phonetic_features",
						  "Call to \"SItemSetString\" failed"))
				return;

			s_bool hasLong = S_PHONESET_CALL(phoneset, phone_has_feature)
				(phoneset,
				 phone_value,
				 "duration_long",
				 error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "s_compute_phonetic_features",
				      "Call to \"phone_has_feature\" failed"))
				return;

			s_bool hasShort = S_PHONESET_CALL(phoneset, phone_has_feature)
				(phoneset,
				 phone_value,
				 "duration_short",
				 error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "s_compute_phonetic_features",
				      "Call to \"phone_has_feature\" failed"))
				return;

			const char * feat = NULL;
			if( hasLong )
			{
				feat = "+";
			}
			else if( hasShort )
			{
				feat = "-";
			}

			if(feat != NULL)
			{
				SItemSetString ( phone, "duration", feat, error );
				if (S_CHK_ERR(error, S_CONTERR,
					      "s_compute_phonetic_features",
					      "Call to \"SItemSetString\" failed"))
					return;
			}

			phone = SItemNext ( phone, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "s_compute_phonetic_features",
				      "Call to \"SItemNext\" failed"))
				return;
		}

		syllable = SItemNext (syllable, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "s_compute_phonetic_features",
			      "Call to \"SItemNext\" failed"))
			return;
	}

}


static void Initialize(SUttProcessor *self, const SVoice *voice, s_erc *error)
{
	const SObject *tmp;
	const SMap *syllInfo;
	const char *plugin_name;
	const char *featproc_class;
	const char *class_name;
	SPlugin *sylPlugin;
	SFeatProcessor *stressProc;
	SSyllabification *syllab;


	S_CLR_ERR(error);

	/* check if a syllabification function is defined as a feature,
	 * and if so, create the syllabification object */
	tmp = SMapGetObjectDef(self->features, "syllabification function", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	/* nothing, return */
	if (tmp == NULL)
		return;

	syllInfo = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"S_CAST (SMap)\" failed"))
		return;

	plugin_name = SMapGetString(syllInfo, "plug-in", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapGetString\" failed"))
		return;

	if (plugin_name == NULL)
		return;

	class_name = SMapGetString(syllInfo, "class", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapGetString\" failed"))
		return;

	if (class_name == NULL)
		return;

	/* load the plug-in */
	sylPlugin = s_pm_load_plugin(plugin_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"s_pm_load_plugin\" failed"))
		return;

	/* create a syllabification object */
	syllab = (SSyllabification*)S_NEW_FROM_NAME(class_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Failed to create new '%s' object", class_name))
	{
		S_DELETE(sylPlugin, "Initialize", error);
		return;
	}

	/* add them to the features */
	SMapSetObject(self->features, "_syll_func_plugin", S_OBJECT(sylPlugin), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapSetObject\" failed"))
	{
		S_DELETE(syllab, "Initialize", error);
		S_DELETE(sylPlugin, "Initialize", error);
		return;
	}

	SMapSetObject(self->features, "_syll_func", S_OBJECT(syllab), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapSetObject\" failed"))
	{
		/* plugin will be deleted when utt processor is deleted */
		S_DELETE(syllab, "Initialize", error);
		return;
	}

	/* check if a stress function is defined as a feature,
	 * and if so, create the stress object */
	tmp = SMapGetObjectDef(self->features, "syllable stress processor", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	/* nothing, return */
	if (tmp == NULL)
		return;

	syllInfo = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"S_CAST (SMap)\" failed"))
		return;

	plugin_name = SMapGetString(syllInfo, "plug-in", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapGetString\" failed"))
		return;

	if (plugin_name == NULL)
		return;

	featproc_class = SMapGetString(syllInfo, "class", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapGetString\" failed"))
		return;

	/* load the plug-in */
	sylPlugin = s_pm_load_plugin(plugin_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"s_pm_load_plugin\" failed"))
		return;

	stressProc  = S_FEATPROCESSOR(S_NEW_FROM_NAME(featproc_class, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SNEW_FROM_NAME\" failed"))
		return;

	/* add them to the features */
	SMapSetObject(self->features, "_stress_func_plugin", S_OBJECT(sylPlugin), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapSetObject\" failed"))
	{
		S_DELETE(stressProc, "Initialize", error);
		S_DELETE(sylPlugin, "Initialize", error);
		return;
	}

	SMapSetObject(self->features, "_stress_func", S_OBJECT(stressProc), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapSetObject\" failed"))
	{
		S_DELETE(sylPlugin, "Initialize", error);
		return;
	}


	S_UNUSED(voice);
}


static void Run(const SUttProcessor *self, SUtterance *utt,
				s_erc *error)
{
	SG2P *g2p = NULL;
	SLexicon *lexicon = NULL;
	SAddendum *addendum = NULL;
	SSyllabification *syllab = NULL;
	const SRelation *wordRel;
	SRelation *syllableRel = NULL;
	SRelation *sylStructRel = NULL;
	SRelation *segmentRel = NULL;
	SItem *wordItem;
	SItem *wordItemcopy;
	char *downcase_word;
	SList *phones;
	s_bool syllabified;
	SList *syllablesPhones;
	SItem *sylStructureWordItem;
	SItem *syllableItem;
	SItem *sylStructSylItem;
	SItem *segmentItem;
	SIterator *sylItr = NULL;
	SIterator *phoneItr = NULL;
	const SObject *phone;
	s_bool is_present;


	S_CLR_ERR(error);
	s_get_lexical_objects(self, utt, &g2p, &lexicon, &addendum, &syllab, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_get_lexical_objects\" failed"))
		goto quit_error;

	/* we require the word relation */
	is_present = SUtteranceRelationIsPresent(utt, "Word", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto quit_error;

	if (!is_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Word' relation in utterance");
		goto quit_error;
	}

	wordRel = SUtteranceGetRelation(utt, "Word", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
		goto quit_error;

	/* create relations */
	syllableRel = SUtteranceNewRelation(utt, "Syllable", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceNewRelation\" failed"))
		goto quit_error;

	sylStructRel = SUtteranceNewRelation(utt, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceNewRelation\" failed"))
		goto quit_error;

	segmentRel = SUtteranceNewRelation(utt, "Segment", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceNewRelation\" failed"))
		goto quit_error;

	/* start at the first item in the word relation, cast away
	 * const, we want to add daughter items.
	 * iterate over the word relation and fill in the
	 * phones and the associated structure.
	 */
	wordItem = (SItem*)SRelationHead(wordRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	wordItemcopy = (SItem*)SRelationHead(wordRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	while (wordItem != NULL)
	{
		/* get word and downcase it */
		downcase_word = s_strlwr(s_strdup(SItemGetName(wordItem, error), error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Failed to down-case word item"))
			goto quit_error;

		if  (downcase_word == NULL || s_strcmp(downcase_word, "", error) == 0)
			goto continue_cycle;

		phones = NULL;
		syllabified = FALSE;

		/* get phone sequence for word */
		if (addendum != NULL)
		{
			phones = S_ADDENDUM_CALL(addendum, get_word)(addendum,
														 downcase_word,
														 NULL,
														 &syllabified,
														 error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to method \"get_word\" (SAddendum) failed"))
				goto quit_error;
		}

		if ((phones == NULL) && (lexicon != NULL))
		{
			phones = S_LEXICON_CALL(lexicon, get_word)(lexicon,
													   downcase_word,
													   NULL,
													   &syllabified,
													   error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to method \"get_word\" (SLexicon) failed"))
				goto quit_error;
		}

		if ((phones == NULL) && (g2p != NULL))
		{
			phones = S_G2P_CALL(g2p, apply)(g2p, downcase_word, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to method \"apply\" (SG2P) failed"))
				goto quit_error;
		}

		if (phones == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "Run",
					  "Failed to get phone sequence for word '%s'", downcase_word);
			S_FREE(downcase_word);
			continue;
		}

		S_FREE(downcase_word);

		/* syllabify phone sequence */
		if (syllabified == FALSE)
		{
			if (syllab != NULL)
			{
				syllablesPhones = S_SYLLABIFICATION_CALL(syllab, syllabify)(syllab,
																			wordItem,
																			phones,
																			error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to method \"syllabify\" failed"))
					goto quit_error;

				S_DELETE(phones, "Run", error);
			}
			else
			{
				syllablesPhones = S_LIST(S_NEW(SListList, error));
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Failed to create new 'SList' object"))
					goto quit_error;

				SListAppend(syllablesPhones, S_OBJECT(phones), error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to \"SListAppend\" failed"))
					goto quit_error;
			}
		}
		else
			syllablesPhones = (SList*)phones;

		/* create new syllable structure word item, shares content
		 * with word item.
		 */
		sylStructureWordItem = SRelationAppend(sylStructRel, wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SRelationAppend\" failed"))
			goto quit_error;

		/* iterate over syllables */
		sylItr = S_ITERATOR_GET(syllablesPhones, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"S_ITERATOR_GET\" failed"))
			goto quit_error;

		while (sylItr != NULL)
		{
			/* new item in syllable relation */
			syllableItem = SRelationAppend(syllableRel, NULL, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SRelationAppend\" failed"))
				goto quit_error;

			SItemSetName(syllableItem, "syl", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemSetName\" failed"))
				goto quit_error;

			/* daughter of above item, but in SylStructure */
			sylStructSylItem = SItemAddDaughter(sylStructureWordItem, syllableItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemAddDaughter\" failed"))
				goto quit_error;

			/* iterate over phones and add segments */
			phoneItr = S_ITERATOR_GET((SList*)SIteratorObject(sylItr, error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"S_ITERATOR_GET/SIteratorObject\" failed"))
				goto quit_error;

			while (phoneItr != NULL)
			{
				phone = SIteratorObject(phoneItr, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to \"SIteratorObject\" failed"))
					goto quit_error;

				segmentItem = SRelationAppend(segmentRel, NULL, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to \"SRelationAppend\" failed"))
					goto quit_error;

				SItemSetName(segmentItem, SObjectGetString(phone, error), error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to \"SItemSetName/SObjectGetString\" failed"))
					goto quit_error;

				SItemAddDaughter(sylStructSylItem, segmentItem, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to \"SItemAddDaughter\" failed"))
					goto quit_error;

				phoneItr = SIteratorNext(phoneItr);
			}


			sylItr = SIteratorNext(sylItr);
		}

		S_DELETE(syllablesPhones, "Run", error);
continue_cycle:
		wordItem = SItemNext(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	wordItem = wordItemcopy;
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPathToItem\" failed"))
		goto quit_error;

	is_present = SUttProcessorFeatureIsPresent(self, "_stress_func", error);
	if (S_CHK_ERR(error, S_CONTERR,
	              "Run",
	              "Call to \"SUttProcessorFeatureIsPresent\" failed"))
		goto quit_error;

	const SFeatProcessor* stress_featproc = NULL;
	if(is_present) {
		stress_featproc = SUttProcessorGetFeature(self, "_stress_func", error);
		if (S_CHK_ERR(error, S_CONTERR,
		              "Run",
		              "Call to \"SUttProcessorGetFeature\" failed"))
			goto quit_error;
	}

	while (wordItem != NULL)
	{

		if(stress_featproc) {
			s_compute_stresses(stress_featproc, wordItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
			              "Run",
			              "Call to \"s_compute_stresses\" failed"))
				goto quit_error;
		}

		s_compute_phonetic_features(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_compute_phonetic_features\" failed"))
			goto quit_error;

		wordItem = SItemNext(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	/* here all is OK */
	return;


	/* error clean-up code */
quit_error:
	if (syllableRel != NULL)
	{
		SUtteranceDelRelation(utt, "Syllable", error);
		S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceDelRelation\" failed");
	}

	if (sylStructRel != NULL)
	{
		SUtteranceDelRelation(utt, "SylStructure", error);
		S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceDelRelation\" failed");
	}

	if (segmentRel != NULL)
	{
		SUtteranceDelRelation(utt, "Segment", error);
		S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceDelRelation\" failed");
	}

	if (sylItr != NULL)
		S_DELETE(sylItr, "Run", error);

	if (phoneItr != NULL)
		S_DELETE(phoneItr, "Run", error);

	self = NULL;
}


/************************************************************************************/
/*                                                                                  */
/* SLexLookupUttProc class initialization                                           */
/*                                                                                  */
/************************************************************************************/

static SLexLookupUttProcClass LexLookupUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:SLexLookupUttProc",
		sizeof(SLexLookupUttProc),
		{ 0, 1},
		NULL,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SUttProcessorClass */
	Initialize,          /* initialize */
	Run                  /* run        */
};
