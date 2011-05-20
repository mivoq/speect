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
/* Generate HTS Labels for Lwazi voices (simple1).                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "hts_labels.h"
#include "phoneset.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

#define MAX_PHONE_NAME_LENGTH 8


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SHTSLabelsSimple1FeatProc class declaration. */
static SHTSLabelsSimple1FeatProcClass HTSLabelsSimple1FeatProcClass;

static const char *none_string = "x";


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_bool segment_is_pause(const SItem *item, s_erc *error);


/* p1^p2-p3+p4=p5@
 *
 * p1 the phoneme identity before the previous phoneme
 * p2 the previous phoneme identity
 * p3 the current phoneme identity
 * p4 the next phoneme identity
 * p5 the phoneme after the next phoneme identity
 */
static char *create_phone_context(const SItem *item, s_erc *error);


/* p6_p7
 *
 * p6 position of the current phoneme identity in the current syllable (forward)
 * p7 position of the current phoneme identity in the current syllable (backward)
 */
static char *create_syl_context(const SItem *item, s_erc *error);

static char *create_syl_context_pause(const SItem *item, s_erc *error);


/* /A:a1_a2_a3
 *
 * a1 whether the previous syllable stressed or not (0: not stressed, 1: stressed)
 * a2 whether the previous syllable accented or not (0: not accented, 1: accented)
 * a3 the number of phonemes in the previous syllable
 */
static char *create_A_context(const SItem *item, s_erc *error);

static char *create_A_context_pause(const SItem *item, s_erc *error);


/* /B:b1-b2-b3@b4-b5&b6-b7#b8-b9$b10-b11!b12-b13;b14-b15|b16
 *
 * b1 whether the current syllable stressed or not (0: not stressed, 1: stressed)
 * b2 whether the current syllable accented or not (0: not accented, 1: accented)
 * b3 the number of phonemes in the current syllable
 * b4 position of the current syllable in the current word (forward)
 * b5 position of the current syllable in the current word (backward)
 * b6 position of the current syllable in the current phrase (forward)
 * b7 position of the current syllable in the current phrase (backward)
 * b8 the number of stressed syllables before the current syllable in the current phrase
 * b9 the number of stressed syllables after the current syllable in the current phrase
 * b10 the number of accented syllables before the current syllable in the current phrase
 * b11 the number of accented syllables after the current syllable in the current phrase
 * b12 the number of syllables from the previous stressed syllable to the current syllable
 * b13 the number of syllables from the current syllable to the next stressed syllable
 * b14 the number of syllables from the previous accented syllable to the current syllable
 * b15 the number of syllables from the current syllable to the next accented syllable
 * b16 name of the vowel of the current syllable
 */
static char *create_B_context(const SItem *item, s_erc *error);

static char *create_B_context_pause(const SItem *item, s_erc *error);


/* /C:c1+c2+c3
 *
 * c1 whether the next syllable stressed or not (0: not stressed, 1: stressed)
 * c2 whether the next syllable accented or not (0: not accented, 1: accented)
 * c3 the number of phonemes in the next syllable
 */
static char *create_C_context(const SItem *item, s_erc *error);

static char *create_C_context_pause(const SItem *item, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_hts_labels_simple1_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&HTSLabelsSimple1FeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_hts_labels_simple1_class_reg",
			  "Failed to register SHTSLabelsSimple1FeatProcClass");
}


S_LOCAL void _s_hts_labels_simple1_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&HTSLabelsSimple1FeatProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_hts_labels_simple1_class_free",
			  "Failed to free SHTSLabelsSimple1FeatProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_bool segment_is_pause(const SItem *item, s_erc *error)
{
	const SVoice *voice;
	const SPhoneset *phoneset;
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


/* p1^p2-p3+p4=p5@
 *
 * p1 the phoneme identity before the previous phoneme
 * p2 the previous phoneme identity
 * p3 the current phoneme identity
 * p4 the next phoneme identity
 * p5 the phoneme after the next phoneme identity
 */
static char *create_phone_context(const SItem *item, s_erc *error)
{
	char *p_context;
	char p1[MAX_PHONE_NAME_LENGTH] = "";
	char p2[MAX_PHONE_NAME_LENGTH] = "";
	char p3[MAX_PHONE_NAME_LENGTH] = "";
	char p4[MAX_PHONE_NAME_LENGTH] = "";
	char p5[MAX_PHONE_NAME_LENGTH] = "";
	const SObject *featPath;
	const char *tmp;


	S_CLR_ERR(error);

	/* p1 = p.p.name */
	featPath = s_path_to_feature(item, "p.p.name", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		s_strcpy(p1, SObjectGetString(featPath, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"s_strcpy/SObjectGetString\" failed"))
			return NULL;
	}
	else
	{
		s_strcpy(p1, none_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"s_strcpy\" failed"))
			return NULL;
	}

	/* p2 = p.name */
	featPath = s_path_to_feature(item, "p.name", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		s_strcpy(p2, SObjectGetString(featPath, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"s_strcpy/SObjectGetString\" failed"))
			return NULL;
	}
	else
	{
		s_strcpy(p2, none_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"s_strcpy\" failed"))
			return NULL;
	}

	/* p3 = name */
	tmp = SItemGetName(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SItemGetName\" failed"))
		return NULL;

	if (tmp != NULL)
	{
		s_strcpy(p3, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"s_strcpy\" failed"))
			return NULL;
	}
	else
	{
		s_strcpy(p3, none_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"s_strcpy\" failed"))
			return NULL;
	}

	/* p4 = n.name */
	featPath = s_path_to_feature(item, "n.name", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		s_strcpy(p4, SObjectGetString(featPath, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"s_strcpy/SObjectGetString\" failed"))
			return NULL;
	}
	else
	{
		s_strcpy(p4, none_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"s_strcpy\" failed"))
			return NULL;
	}

	/* p5 = n.n.name */
	featPath = s_path_to_feature(item, "n.n.name", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		s_strcpy(p5, SObjectGetString(featPath, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"s_strcpy/SObjectGetString\" failed"))
			return NULL;
	}
	else
	{
		s_strcpy(p5, none_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"s_strcpy\" failed"))
			return NULL;
	}

	s_asprintf(&p_context, error, "%s^%s-%s+%s=%s@", p1, p2, p3, p4, p5);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return p_context;
}


/* p6_p7
 *
 * p6 position of the current phoneme identity in the current syllable (forward)
 * p7 position of the current phoneme identity in the current syllable (backward)
 */
static char *create_syl_context(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	char *syl_context;
	sint32 p6;
	sint32 p7;


	S_CLR_ERR(error);

	/* p6 */
	dFeat = s_path_to_featproc(item, "segment_pos_syl", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		p6 = SObjectGetInt(dFeat, error) + 1; /* it seems as if HTS likes indexing from 1 */
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_syl_context", error);
	}
	else
	{
		p6 = 0;
	}

	/* p7 */
	dFeat = s_path_to_featproc(item, "segment_pos_syl_rev", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		p7 = SObjectGetInt(dFeat, error) + 1; /* it seems as if HTS likes indexing from 1 */
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_syl_context", error);
	}
	else
	{
		p7 = 0;
	}

	s_asprintf(&syl_context, error, "%d_%d", p6, p7);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return syl_context;
}

/* differs from create_phone_syl_context in that all is "x", pause does not
 * have a syllable */
static char *create_syl_context_pause(const SItem *item, s_erc *error)
{
	char *syl_context;
	const char *all_x = "x_x";


	S_CLR_ERR(error);
	syl_context = s_strdup(all_x, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context_pause",
				  "Call to \"s_strdup\" failed"))
	{
		item = NULL; /* compiler noise about unused parameters */
		return NULL;
	}

	return syl_context;
}


/* /A:a1_a2_a3
 *
 * a1 whether the previous syllable stressed or not (0: not stressed, 1: stressed)
 * a2 whether the previous syllable accented or not (0: not accented, 1: accented)
 * a3 the number of phonemes in the previous syllable
 */
static char *create_A_context(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	char *a_context;
	sint32 a1;
	sint32 a2;
	sint32 a3;


	S_CLR_ERR(error);

	/* we currently cannot compute a1 and a2 */
	a1 = 0;
	a2 = 0;

	/* a3 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.p.syllable_num_phones",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_A_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		a3 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_A_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_A_context", error);
	}
	else
	{
		a3 = 0;
	}

	/* we currently cannot compute a1 and a2 */
	s_asprintf(&a_context, error, "/A:0_0_%d", a3);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_A_context",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return a_context;
}

/* differs from create_A_context in that previous phone's syllable is queried
 * and not previous syllable */
static char *create_A_context_pause(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	char *a_context;
	sint32 a1;
	sint32 a2;
	sint32 a3;


	S_CLR_ERR(error);

	/* we currently cannot compute a1 and a2 */
	a1 = 0;
	a2 = 0;

	/* a3 */
	dFeat = s_path_to_featproc(item, "p.R:SylStructure.parent.R:Syllable.syllable_num_phones",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_A_context_pause",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		a3 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_A_context_pause",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_A_context_pause", error);
	}
	else
	{
		a3 = 0;
	}

	/* we currently cannot compute a1 and a2 */
	s_asprintf(&a_context, error, "/A:0_0_%d", a3);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_A_context_pause",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return a_context;
}


/* /B:b1-b2-b3@b4-b5&b6-b7#b8-b9$b10-b11!b12-b13;b14-b15|b16
 *
 * b1 whether the current syllable stressed or not (0: not stressed, 1: stressed)
 * b2 whether the current syllable accented or not (0: not accented, 1: accented)
 * b3 the number of phonemes in the current syllable
 * b4 position of the current syllable in the current word (forward)
 * b5 position of the current syllable in the current word (backward)
 * b6 position of the current syllable in the current phrase (forward)
 * b7 position of the current syllable in the current phrase (backward)
 * b8 the number of stressed syllables before the current syllable in the current phrase
 * b9 the number of stressed syllables after the current syllable in the current phrase
 * b10 the number of accented syllables before the current syllable in the current phrase
 * b11 the number of accented syllables after the current syllable in the current phrase
 * b12 the number of syllables from the previous stressed syllable to the current syllable
 * b13 the number of syllables from the current syllable to the next stressed syllable
 * b14 the number of syllables from the previous accented syllable to the current syllable
 * b15 the number of syllables from the current syllable to the next accented syllable
 * b16 name of the vowel of the current syllable
 */
static char *create_B_context(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	char *b_context;
	sint32 b1;
	sint32 b2;
	sint32 b3;
	sint32 b4;
	sint32 b5;
	sint32 b6;
	sint32 b7;
	sint32 b8;
	sint32 b9;
	sint32 b10;
	sint32 b11;
	sint32 b12;
	sint32 b13;
	sint32 b14;
	sint32 b15;
	const char *b16;


	S_CLR_ERR(error);

	/* no stress/accented methods */
	b1 = 0;
	b2 = 0;

	/* b3 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_num_phones",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b3 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_B_context", error);
	}
	else
	{
		b3 = 0;
	}

	/* b4 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_pos_word",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b4 = SObjectGetInt(dFeat, error) + 1; /* it seems as if HTS likes indexing from 1 */
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_B_context", error);
	}
	else
	{
		b4 = 0;
	}


	/* b5 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_pos_word_rev",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b5 = SObjectGetInt(dFeat, error) + 1; /* it seems as if HTS likes indexing from 1 */
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_B_context", error);
	}
	else
	{
		b5 = 0;
	}


	/* b6 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_pos_phrase",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b6 = SObjectGetInt(dFeat, error) + 1; /* it seems as if HTS likes indexing from 1 */
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_B_context", error);
	}
	else
	{
		b6 = 0;
	}


	/* b7 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_pos_phrase_rev",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b7 = SObjectGetInt(dFeat, error) + 1; /* it seems as if HTS likes indexing from 1 */
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_B_context", error);
	}
	else
	{
		b7 = 0;
	}

	/* no stress/accented methods */
	b8 = 0;
	b9 = 0;
	b10 = 0;
	b11 = 0;
	b12 = 0;
	b13 = 0;
	b14 = 0;
	b15 = 0;

	/* b16 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_vowel",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b16 = SObjectGetString(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetString\" failed"))
			return NULL;

	}
	else
	{
		b16 = "novowel";
	}

	/* we currently cannot compute b1, b2, b8, b9, b10, b11, b12, b13, b14 and b15 */
	s_asprintf(&b_context, error, "/B:0-0-%d@%d-%d&%d-%d#x-x$x-x!x-x;x-x|x",
			   b3, b4, b5, b6, b7);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_asprintf\" failed"))
	{
		S_DELETE(dFeat, "create_B_context", error);
		return NULL;
	}

	S_DELETE(dFeat, "create_B_context", error);
	return b_context;
}

/* differs from create_B_context in that all is "x", pause does not
 * have a syllable */
static char *create_B_context_pause(const SItem *item, s_erc *error)
{
	char *b_context;
	const char *all_x = "/B:x-x-x@x-x&x-x#x-x$x-x!x-x;x-x|x";


	S_CLR_ERR(error);
	b_context = s_strdup(all_x, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context_pause",
				  "Call to \"s_strdup\" failed"))
	{
		item = NULL; /* compiler noise about unused parameters */
		return NULL;
	}

	return b_context;
}


/* /C:c1+c2+c3
 *
 * c1 whether the next syllable stressed or not (0: not stressed, 1: stressed)
 * c2 whether the next syllable accented or not (0: not accented, 1: accented)
 * c3 the number of phonemes in the next syllable
 */
static char *create_C_context(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	char *c_context;
	sint32 c1;
	sint32 c2;
	sint32 c3;


	S_CLR_ERR(error);

	/* we currently cannot compute c1 and c2 */
	c1 = 0;
	c2 = 0;

	/* c3 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.n.syllable_num_phones",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_C_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		c3 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_C_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_C_context", error);
	}
	else
	{
		c3 = 0;
	}

	/* we currently cannot compute c1 and c2 */
	s_asprintf(&c_context, error, "/C:0+0+%d", c3);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_C_context",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return c_context;
}

/* differs from create_C_context in that next phone's syllable is queried
 * and not next syllable */
static char *create_C_context_pause(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	char *c_context;
	sint32 c1;
	sint32 c2;
	sint32 c3;


	S_CLR_ERR(error);

	/* we currently cannot compute c1 and c2 */
	c1 = 0;
	c2 = 0;

	/* c3 */
	dFeat = s_path_to_featproc(item, "n.R:SylStructure.parent.R:Syllable.syllable_num_phones",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_C_context_pause",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		c3 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_C_context_pause",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_C_context_pause", error);
	}
	else
	{
		c3 = 0;
	}

	/* we currently cannot compute c1 and c2 */
	s_asprintf(&c_context, error, "/C:0+0+%d", c3);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_C_context_pause",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return c_context;
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
	char *hts_label = NULL;
	char *tmp = NULL;
	const SItem *segItem;
	s_bool is_pause;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	segItem = SItemAs(item, "Segment", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	if (segItem == NULL)
		return NULL;

	is_pause = segment_is_pause(segItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"segment_is_pause\" failed"))
		goto quit_error;

	/* get phone context */
	tmp = create_phone_context(segItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"create_phone_context\" failed"))
		goto quit_error;

	s_sappend(&hts_label, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_sappend\" failed"))
		goto quit_error;

	S_FREE(tmp);

	if (is_pause)
	{
		/* syllable context */
		tmp = create_syl_context_pause(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_syl_context_pause\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* A context */
		tmp = create_A_context_pause(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_A_context_pause\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* B context */
		tmp = create_B_context_pause(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_B_context_pause\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* C context */
		tmp = create_C_context_pause(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_C_context_pause\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);
	}
	else
	{
		/* syllable context */
		tmp = create_syl_context(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_syl_context\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* A context */
		tmp = create_A_context(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_A_context\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* B context */
		tmp = create_B_context(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_B_context\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* C context */
		tmp = create_C_context(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_C_context\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);
	}

	extractedFeat = SObjectSetString(hts_label, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectSetString\" failed"))
		goto quit_error;

	S_FREE(hts_label);

	/* all OK here */
	return extractedFeat;

	/* error cleanup */
quit_error:
	if (tmp != NULL)
		S_FREE(tmp);

	if (hts_label != NULL)
		S_FREE(hts_label);

	if (extractedFeat != NULL)
		S_DELETE(extractedFeat, "Run", error);

	self = NULL; /* compiler noise about unused parameters */
	return NULL;
}


/************************************************************************************/
/*                                                                                  */
/* SHTSLabelsSimple1FeatProc class initialization                                   */
/*                                                                                  */
/************************************************************************************/

static SHTSLabelsSimple1FeatProcClass HTSLabelsSimple1FeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SHTSLabelsSimple1FeatProc",
		sizeof(SHTSLabelsSimple1FeatProc),
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
