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
/* Generate HTS Labels for Lwazi voices (simple3).                                  */
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

/* SHTSLabelsSimple3FeatProc class declaration. */
static SHTSLabelsSimple3FeatProcClass HTSLabelsSimple3FeatProcClass;

static const char *none_string = "x";


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static sint32 get_stress_level(const SObject *stressFeat, s_erc *error);

static sint32 get_accent_level(const SObject *accentFeat, s_erc *error);

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


/* /D:d1_d2
 *
 * d1 gpos (guess part-of-speech) of the previous word
 * d2 the number of syllables in the previous word
 */
static char *create_D_context(const SItem *item, s_erc *error);

static char *create_D_context_pause(const SItem *item, s_erc *error);


/* /E:e1+e2@e3+e4&e5+e6#e7+e8
 *
 * e1 gpos (guess part-of-speech) of the current word
 * e2 the number of syllables in the current word
 * e3 position of the current word in the current phrase (forward)
 * e4 position of the current word in the current phrase (backward)
 * e5 the number of content words before the current word in the current phrase
 * e6 the number of content words after the current word in the current phrase
 * e7 the number of words from the previous content word to the current word
 * e8 the number of words from the current word to the next content word
 */
static char *create_E_context(const SItem *item, s_erc *error);

static char *create_E_context_pause(const SItem *item, s_erc *error);


/* /F:f1_f2
 *
 * f1 gpos (guess part-of-speech) of the next word
 * f2 the number of syllables in the next word
 */
static char *create_F_context(const SItem *item, s_erc *error);

static char *create_F_context_pause(const SItem *item, s_erc *error);


/* /G:g1_g2
 *
 * g1 the number of syllables in the previous phrase
 * g2 the number of words in the previous phrase
 */
static char *create_G_context(const SItem *item, s_erc *error);

static char *create_G_context_pause(const SItem *item, s_erc *error);


/* /H:h1=h2@h3=h4|h5
 *
 * h1 the number of syllables in the current phrase
 * h2 the number of words in the current phrase
 * h3 position of the current phrase in utterence (forward)
 * h4 position of the current phrase in utterence (backward)
 * h5 TOBI endtone of the current phrase
 */
static char *create_H_context(const SItem *item, s_erc *error);

static char *create_H_context_pause(const SItem *item, s_erc *error);


/* /I:i1_i2
 *
 * i1 the number of syllables in the next phrase
 * i2 the number of words in the next phrase
 */
static char *create_I_context(const SItem *item, s_erc *error);

static char *create_I_context_pause(const SItem *item, s_erc *error);


/* /J:j1+j2-j3
 *
 * j1 the number of syllables in this utterence
 * j2 the number of words in this utternce
 * j3 the number of phrases in this utterence
 */
static char *create_J_context(const SItem *item, s_erc *error);

/* note that pauses use create_J_context as well */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_hts_labels_simple3_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&HTSLabelsSimple3FeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_hts_labels_simple3_class_reg",
			  "Failed to register SHTSLabelsSimple3FeatProcClass");
}


S_LOCAL void _s_hts_labels_simple3_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&HTSLabelsSimple3FeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_hts_labels_simple3_class_free",
			  "Failed to free SHTSLabelsSimple3FeatProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/*
 * this is for Georg's stress levels:
 * unstressed : 0
 * primary    : 1
 * secondary  : 2
 */
static sint32 get_stress_level(const SObject *stressFeat, s_erc *error)
{
	const char *stress_feat;


	S_CLR_ERR(error);
	stress_feat = SObjectGetString(stressFeat, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_stress_level",
				  "Call to \"SObjectGetString\" failed"))
		return 0;

	if (s_strcmp(stress_feat, "unstressed", error) == 0)
	{
		S_CHK_ERR(error, S_CONTERR,
				  "get_stress_level",
				  "Call to \"s_strcmp\" failed");
		return 0;
	}

	if (s_strcmp(stress_feat, "primary", error) == 0)
	{
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_stress_level",
					  "Call to \"s_strcmp\" failed"))
			return 0;

		return 1;
	}

	if (s_strcmp(stress_feat, "secondary", error) == 0)
	{
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_stress_level",
					  "Call to \"s_strcmp\" failed"))
			return 0;

		return 2;
	}

	return 0; /* unknown */
}


/*
 * this is for Georg's accent levels:
 * unaccented : 0
 * accented   : 1
 */
static sint32 get_accent_level(const SObject *accentFeat, s_erc *error)
{
	const char *accent_feat;


	S_CLR_ERR(error);
	accent_feat = SObjectGetString(accentFeat, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_accent_level",
				  "Call to \"SObjectGetString\" failed"))
		return 0;

	if (s_strcmp(accent_feat, "unaccented", error) == 0)
	{
		S_CHK_ERR(error, S_CONTERR,
				  "get_accent_level",
				  "Call to \"s_strcmp\" failed");
		return 0;
	}

	if (s_strcmp(accent_feat, "accented", error) == 0)
	{
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_accent_level",
					  "Call to \"s_strcmp\" failed"))
			return 0;

		return 1;
	}

	return 0; /* unknown */
}


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
		return NULL;
	}

	return syl_context;

	S_UNUSED(item);
}


/* /A:a1_a2_a3
 *
 * a1 whether the previous syllable stressed or not (0: not stressed, 1: stressed)
 * a2 whether the previous syllable accented or not (0: not accented, 1: accented)
 * a3 the number of phonemes in the previous syllable
 */
static char *create_A_context(const SItem *item, s_erc *error)
{
	const SObject *featPath;
	SObject *dFeat;
	char *a_context;
	sint32 a1;
	sint32 a2;
	sint32 a3;


	S_CLR_ERR(error);

	/*  a1 */
	featPath = s_path_to_feature(item, "R:SylStructure.parent.R:Syllable.p.stress",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_A_context",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		a1 = get_stress_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_A_context",
					  "Call to \"get_stress_level\" failed"))
			return NULL;
	}
	else
	{
		a1 = 0;
	}

	/*  a2 */
	featPath = s_path_to_feature(item, "R:SylStructure.parent.R:Syllable.p.accent",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_A_context",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		a2 = get_accent_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_A_context",
					  "Call to \"get_accent_level\" failed"))
			return NULL;
	}
	else
	{
		a2 = 0;
	}

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

	s_asprintf(&a_context, error, "/A:%d_%d_%d", a1, a2, a3);
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
	const SObject *featPath;
	SObject *dFeat;
	char *a_context;
	sint32 a1;
	sint32 a2;
	sint32 a3;


	S_CLR_ERR(error);

	/*  a1 */
	featPath = s_path_to_feature(item, "p.R:SylStructure.parent.R:Syllable.stress",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_A_context_pause",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		a1 = get_stress_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_A_context_pause",
					  "Call to \"get_stress_level\" failed"))
			return NULL;
	}
	else
	{
		a1 = 0;
	}

	/*  a2 */
	featPath = s_path_to_feature(item, "p.R:SylStructure.parent.R:Syllable.accent",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_A_context_pause",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		a2 = get_accent_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_A_context_pause",
					  "Call to \"get_accent_level\" failed"))
			return NULL;
	}
	else
	{
		a2 = 0;
	}

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

	s_asprintf(&a_context, error, "/A:%d_%d_%d", a1, a2, a3);
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
	const SObject *featPath;
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

	/* b1 */
	featPath = s_path_to_feature(item, "R:SylStructure.parent.R:Syllable.stress",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		b1 = get_stress_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"get_stress_level\" failed"))
			return NULL;
	}
	else
	{
		b1 = 0;
	}

	/*  b2 */
	featPath = s_path_to_feature(item, "R:SylStructure.parent.R:Syllable.accent",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		b2 = get_accent_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"get_accent_level\" failed"))
			return NULL;
	}
	else
	{
		b2 = 0;
	}

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


	/* b8 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_stress_in",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b8 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_B_context", error);
	}
	else
	{
		b8 = 0;
	}

	/* b9 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_stress_out",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b9 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_B_context", error);
	}
	else
	{
		b9 = 0;
	}

	/* b10 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_accent_in",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b10 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_B_context", error);
	}
	else
	{
		b10 = 0;
	}


	/* b11 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_accent_out",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b11 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_B_context", error);
	}
	else
	{
		b11 = 0;
	}


	/* b12 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_stress_all_in",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b12 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_B_context", error);
	}
	else
	{
		b12 = 0;
	}


	/* b13 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_stress_all_out",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b13 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_B_context", error);
	}
	else
	{
		b13 = 0;
	}


	/* b14 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_accent_all_in",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b14 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_B_context", error);
	}
	else
	{
		b14 = 0;
	}


	/* b15 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.R:Syllable.syllable_accent_all_out",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_B_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		b15 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_B_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_B_context", error);
	}
	else
	{
		b15 = 0;
	}


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
	s_asprintf(&b_context, error, "/B:%d-%d-%d@%d-%d&%d-%d#%d-%d$%d-%d!%d-%d;%d-%d|%s",
			   b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16);
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
		return NULL;
	}

	return b_context;

	S_UNUSED(item);
}


/* /C:c1+c2+c3
 *
 * c1 whether the next syllable stressed or not (0: not stressed, 1: stressed)
 * c2 whether the next syllable accented or not (0: not accented, 1: accented)
 * c3 the number of phonemes in the next syllable
 */
static char *create_C_context(const SItem *item, s_erc *error)
{
	const SObject *featPath;
	SObject *dFeat;
	char *c_context;
	sint32 c1;
	sint32 c2;
	sint32 c3;


	S_CLR_ERR(error);

	/* c1 */
	featPath = s_path_to_feature(item, "R:SylStructure.parent.R:Syllable.n.stress",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_C_context",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		c1 = get_stress_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_C_context",
					  "Call to \"get_stress_level\" failed"))
			return NULL;

		if (c1 > 0)
			c1 = 1;
	}
	else
	{
		c1 = 0;
	}

	/* c2 */
	featPath = s_path_to_feature(item, "R:SylStructure.parent.R:Syllable.n.accent",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_C_context",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		c2 = get_accent_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_C_context",
					  "Call to \"get_accent_level\" failed"))
			return NULL;
	}
	else
	{
		c2 = 0;
	}

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

	s_asprintf(&c_context, error, "/C:%d+%d+%d", c1, c2, c3);
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
	const SObject *featPath;
	SObject *dFeat;
	char *c_context;
	sint32 c1;
	sint32 c2;
	sint32 c3;


	S_CLR_ERR(error);

	/* c1 */
	featPath = s_path_to_feature(item, "n.R:SylStructure.parent.R:Syllable.stress",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_C_context_pause",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		c1 = get_stress_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_C_context_pause",
					  "Call to \"get_stress_level\" failed"))
			return NULL;

		if (c1 > 0)
			c1 = 1;
	}
	else
	{
		c1 = 0;
	}

	/* c2 */
	featPath = s_path_to_feature(item, "n.R:SylStructure.parent.R:Syllable.accent",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_C_context_pause",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		c2 = get_accent_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_C_context_pause",
					  "Call to \"get_accent_level\" failed"))
			return NULL;
	}
	else
	{
		c2 = 0;
	}

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

	s_asprintf(&c_context, error, "/C:%d+%d+%d", c1, c2, c3);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_C_context_pause",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return c_context;
}


/* /D:d1_d2
 *
 * d1 gpos (guess part-of-speech) of the previous word
 * d2 the number of syllables in the previous word
 */
static char *create_D_context(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	const SObject *featPath;
	char *d_context;
	const char *d1;
	sint32 d2;


	S_CLR_ERR(error);

	/* d1 */
	featPath = s_path_to_feature(item, "R:SylStructure.parent.parent.R:Word.p.pos",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_D_context",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		d1 = SObjectGetString(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_D_context",
					  "Call to \"SObjectGetString\" failed"))
			return NULL;
	}
	else
	{
		d1 = none_string;
	}


	/* d2 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Word.p.word_num_syls",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_D_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		d2 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_D_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_D_context", error);
	}
	else
	{
		d2 = 0;
	}

	s_asprintf(&d_context, error, "/D:%s_%d", d1, d2);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_D_context",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return d_context;
}


/* differs from create_D_context in that previous phone's word is queried
 * and not previous word */
static char *create_D_context_pause(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	const SObject *featPath;
	char *d_context;
	const char *d1;
	sint32 d2;


	S_CLR_ERR(error);

	/* d1 */
	featPath = s_path_to_feature(item, "p.R:SylStructure.parent.parent.R:Word.pos",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_D_context_pause",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		d1 = SObjectGetString(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_D_context_pause",
					  "Call to \"SObjectGetString\" failed"))
			return NULL;
	}
	else
	{
		d1 = none_string;
	}

	/* d2 */
	dFeat = s_path_to_featproc(item, "p.R:SylStructure.parent.parent.R:Word.word_num_syls",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_D_context_pause",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		d2 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_D_context_pause",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_D_context_pause", error);
	}
	else
	{
		d2 = 0;
	}

	s_asprintf(&d_context, error, "/D:%s_%d", d1, d2);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_D_context_pause",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return d_context;
}


/* /E:e1+e2@e3+e4&e5+e6#e7+e8
 *
 * e1 gpos (guess part-of-speech) of the current word
 * e2 the number of syllables in the current word
 * e3 position of the current word in the current phrase (forward)
 * e4 position of the current word in the current phrase (backward)
 * e5 the number of content words before the current word in the current phrase
 * e6 the number of content words after the current word in the current phrase
 * e7 the number of words from the previous content word to the current word
 * e8 the number of words from the current word to the next content word
 */
static char *create_E_context(const SItem *item, s_erc *error)
{
	const SObject *featPath;
	SObject *dFeat;
	char *e_context;
	const char *e1;
	sint32 e2;
	sint32 e3;
	sint32 e4;
	sint32 e5;
	sint32 e6;
	sint32 e7;
	sint32 e8;


	S_CLR_ERR(error);

	featPath = s_path_to_feature(item, "R:SylStructure.parent.parent.R:Word.pos",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_E_context",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		e1 = SObjectGetString(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_E_context",
					  "Call to \"SObjectGetString\" failed"))
			return NULL;
	}
	else
	{
		e1 = none_string;
	}

	/* e2 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Word.word_num_syls",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_E_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		e2 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_E_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_E_context", error);
	}
	else
	{
		e2 = 0;
	}

	/* e3 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Word.word_pos_phrase",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_E_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		e3 = SObjectGetInt(dFeat, error) + 1; /* it seems as if HTS likes indexing from 1 */
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_E_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_E_context", error);
	}
	else
	{
		e3 = 0;
	}

	/* e4 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Word.word_pos_phrase_rev",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_E_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		e4 = SObjectGetInt(dFeat, error) + 1; /* it seems as if HTS likes indexing from 1 */
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_E_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_E_context", error);
	}
	else
	{
		e4 = 0;
	}


	/* e5 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Word.word_content_in",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_E_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		e5 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_E_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_E_context", error);
	}
	else
	{
		e5 = 0;
	}


	/* e6 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Word.word_content_out",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_E_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		e6 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_E_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_E_context", error);
	}
	else
	{
		e6 = 0;
	}


	/* e7 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Word.word_content_all_in",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_E_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		e7 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_E_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_E_context", error);
	}
	else
	{
		e7 = 0;
	}


	/* e8 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Word.word_content_all_out",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_E_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		e8 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_E_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_E_context", error);
	}
	else
	{
		e8 = 0;
	}

	s_asprintf(&e_context, error, "/E:%s+%d@%d+%d&%d+%d#%d+%d",
			   e1, e2, e3, e4, e5, e6, e7, e8);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_E_context",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return e_context;
}

/* differs from create_E_context in that all is "x", pause does not
 * have a word */
static char *create_E_context_pause(const SItem *item, s_erc *error)
{
	char *e_context;
	const char *all_x = "/E:0+0@0+0&0+0#0+0";


	S_CLR_ERR(error);
	e_context = s_strdup(all_x, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_E_context_pause",
				  "Call to \"s_strdup\" failed"))
	{
		return NULL;
	}

	return e_context;

	S_UNUSED(item);
}


/* /F:f1_f2
 *
 * f1 gpos (guess part-of-speech) of the next word
 * f2 the number of syllables in the next word
 */
static char *create_F_context(const SItem *item, s_erc *error)
{
	const SObject *featPath;
	SObject *dFeat;
	char *f_context;
	const char *f1;
	sint32 f2;


	S_CLR_ERR(error);

	/* f1 */
	featPath = s_path_to_feature(item, "R:SylStructure.parent.parent.R:Word.n.pos",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_F_context",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		f1 = SObjectGetString(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_F_context",
					  "Call to \"SObjectGetString\" failed"))
			return NULL;
	}
	else
	{
		f1 = none_string;
	}

	/* f2 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Word.n.word_num_syls",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_F_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		f2 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_F_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_F_context", error);
	}
	else
	{
		f2 = 0;
	}

	s_asprintf(&f_context, error, "/F:%s_%d", f1, f2);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_F_context",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return f_context;
}

/* differs from create_F_context in that next phone's word is queried
 * and not next word */
static char *create_F_context_pause(const SItem *item, s_erc *error)
{
	const SObject *featPath;
	SObject *dFeat;
	char *f_context;
	const char *f1;
	sint32 f2;


	S_CLR_ERR(error);

	/* f1 */
	featPath = s_path_to_feature(item, "n.R:SylStructure.parent.parent.R:Word.pos",
								 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_F_context_pause",
				  "Call to \"s_path_to_feature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		f1 = SObjectGetString(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_F_context_pause",
					  "Call to \"SObjectGetString\" failed"))
			return NULL;
	}
	else
	{
		f1 = none_string;
	}

	/* f2 */
	dFeat = s_path_to_featproc(item, "n.R:SylStructure.parent.parent.R:Word.word_num_syls",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_F_context_pause",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		f2 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_F_context_pause",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_F_context_pause", error);
	}
	else
	{
		f2 = 0;
	}

	s_asprintf(&f_context, error, "/F:%s_%d", f1, f2);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_F_context_pause",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return f_context;
}


/* /G:g1_g2
 *
 * g1 the number of syllables in the previous phrase
 * g2 the number of words in the previous phrase
 */
static char *create_G_context(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	char *g_context;
	sint32 g1;
	sint32 g2;


	S_CLR_ERR(error);


	/* g1 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Phrase.parent.p.phrase_num_syls",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_G_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		g1 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_G_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_G_context", error);
	}
	else
	{
		g1 = 0;
	}


	/* g2 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Phrase.parent.p.phrase_num_words",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_G_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		g2 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_G_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_G_context", error);
	}
	else
	{
		g2 = 0;
	}

	s_asprintf(&g_context, error, "/G:%d_%d", g1, g2);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_G_context",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return g_context;
}

/* differs from create_G_context in that previous phone's phrase is queried
 * and not previous phrase */
static char *create_G_context_pause(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	char *g_context;
	sint32 g1;
	sint32 g2;


	S_CLR_ERR(error);


	/* g1 */
	dFeat = s_path_to_featproc(item, "p.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_syls",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_G_context_pause",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		g1 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_G_context_pause",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_G_context_pause", error);
	}
	else
	{
		g1 = 0;
	}


	/* g2 */
	dFeat = s_path_to_featproc(item, "p.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_words",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_G_context_pause",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		g2 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_G_context_pause",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_G_context_pause", error);
	}
	else
	{
		g2 = 0;
	}

	s_asprintf(&g_context, error, "/G:%d_%d", g1, g2);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_G_context_pause",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return g_context;
}


/* /H:h1=h2@h3=h4|h5
 *
 * h1 the number of syllables in the current phrase
 * h2 the number of words in the current phrase
 * h3 position of the current phrase in utterence (forward)
 * h4 position of the current phrase in utterence (backward)
 * h5 TOBI endtone of the current phrase
 */
static char *create_H_context(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	char *h_context;
	sint32 h1;
	sint32 h2;
	sint32 h3;
	sint32 h4;
	const char *h5;


	S_CLR_ERR(error);

	/* h1 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_syls",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_H_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		h1 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_H_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_H_context", error);
	}
	else
	{
		h1 = 0;
	}


	/* h2 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_words",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_H_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		h2 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_H_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_H_context", error);
	}
	else
	{
		h2 = 0;
	}

	/* h3 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Phrase.parent.phrase_pos_utt",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_H_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		h3 = SObjectGetInt(dFeat, error) + 1; /* it seems as if HTS likes indexing from 1 */
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_H_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_H_context", error);
	}
	else
	{
		h3 = 0;
	}

	/* h4 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Phrase.parent.phrase_pos_utt_rev",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_H_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		h4 = SObjectGetInt(dFeat, error) + 1; /* it seems as if HTS likes indexing from 1 */
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_H_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_H_context", error);
	}
	else
	{
		h4 = 0;
	}

	/* we currently cannot compute h5 */
	h5 = NULL;

	s_asprintf(&h_context, error, "/H:%d=%d@%d=%d|x", h1, h2, h3, h4);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_H_context",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return h_context;
}

/* differs from create_H_context in that either next or previous
 * phone's phrase is used (if prev exits, use, else next)
 */
static char *create_H_context_pause(const SItem *item, s_erc *error)
{
	const SItem *tmp;
	s_bool use_next;
	SObject *dFeat;
	char *h_context;
	sint32 h1;
	sint32 h2;
	sint32 h3;
	sint32 h4;
	const char *h5;


	S_CLR_ERR(error);
	tmp = SItemPrev(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_H_context_pause",
				  "Call to \"SItemPrev\" failed"))
		return NULL;

	if (tmp != NULL)
		use_next = FALSE;
	else
		use_next = TRUE;

	/* h1 */
	if (use_next)
	{
		dFeat = s_path_to_featproc(item, "n.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_syls",
								   error);
	}
	else
	{
		dFeat = s_path_to_featproc(item, "p.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_syls",
								   error);
	}

	if (S_CHK_ERR(error, S_CONTERR,
				  "create_H_context_pause",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		h1 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_H_context_pause",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_H_context_pause", error);
	}
	else
	{
		h1 = 0;
	}


	/* h2 */
	if (use_next)
	{
		dFeat = s_path_to_featproc(item, "n.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_words",
								   error);
	}
	else
	{
		dFeat = s_path_to_featproc(item, "p.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_words",
								   error);
	}

	if (S_CHK_ERR(error, S_CONTERR,
				  "create_H_context_pause",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		h2 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_H_context_pause",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_H_context_pause", error);
	}
	else
	{
		h2 = 0;
	}

	/* h3 */
	if (use_next)
	{
		dFeat = s_path_to_featproc(item, "n.R:SylStructure.parent.parent.R:Phrase.parent.phrase_pos_utt",
								   error);
	}
	else
	{
		dFeat = s_path_to_featproc(item, "p.R:SylStructure.parent.parent.R:Phrase.parent.phrase_pos_utt",
								   error);
	}

	if (S_CHK_ERR(error, S_CONTERR,
				  "create_H_context_pause",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		h3 = SObjectGetInt(dFeat, error) + 1; /* it seems as if HTS likes indexing from 1 */
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_H_context_pause",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_H_context_pause", error);
	}
	else
	{
		h3 = 0;
	}

	/* h4 */
	if (use_next)
	{
		dFeat = s_path_to_featproc(item, "n.R:SylStructure.parent.parent.R:Phrase.parent.phrase_pos_utt_rev",
								   error);
	}
	else
	{
		dFeat = s_path_to_featproc(item, "p.R:SylStructure.parent.parent.R:Phrase.parent.phrase_pos_utt_rev",
								   error);
	}

	if (S_CHK_ERR(error, S_CONTERR,
				  "create_H_context_pause",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		h4 = SObjectGetInt(dFeat, error) + 1; /* it seems as if HTS likes indexing from 1 */
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_H_context_pause",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_H_context_pause", error);
	}
	else
	{
		h4 = 0;
	}

	/* we currently cannot compute h5 */
	h5 = NULL;

	s_asprintf(&h_context, error, "/H:%d=%d@%d=%d|x", h1, h2, h3, h4);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_H_context_pause",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return h_context;
}


/* /I:i1_i2
 *
 * i1 the number of syllables in the next phrase
 * i2 the number of words in the next phrase
 */
static char *create_I_context(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	char *i_context;
	sint32 i1;
	sint32 i2;


	S_CLR_ERR(error);


	/* i1 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Phrase.parent.n.phrase_num_syls",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_I_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		i1 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_I_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_I_context", error);
	}
	else
	{
		i1 = 0;
	}


	/* i2 */
	dFeat = s_path_to_featproc(item, "R:SylStructure.parent.parent.R:Phrase.parent.n.phrase_num_words",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_I_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		i2 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_I_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_I_context", error);
	}
	else
	{
		i2 = 0;
	}

	s_asprintf(&i_context, error, "/I:%d_%d", i1, i2);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_I_context",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return i_context;
}

/* differs from create_I_context in that next phone's phrase is queried
 * and not next phrase */
static char *create_I_context_pause(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	char *i_context;
	sint32 i1;
	sint32 i2;


	S_CLR_ERR(error);


	/* i1 */
	dFeat = s_path_to_featproc(item, "n.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_syls",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_I_context_pause",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		i1 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_I_context_pause",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_I_context_pause", error);
	}
	else
	{
		i1 = 0;
	}


	/* i2 */
	dFeat = s_path_to_featproc(item, "n.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_words",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_I_context_pause",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		i2 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_I_context_pause",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_I_context_pause", error);
	}
	else
	{
		i2 = 0;
	}

	s_asprintf(&i_context, error, "/I:%d_%d", i1, i2);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_I_context_pause",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return i_context;
}


/* /J:j1+j2-j3
 *
 * j1 the number of syllables in this utterence
 * j2 the number of words in this utternce
 * j3 the number of phrases in this utterence
 */
static char *create_J_context(const SItem *item, s_erc *error)
{
	SObject *dFeat;
	char *j_context;
	sint32 j1;
	sint32 j2;
	sint32 j3;


	S_CLR_ERR(error);


	/* j1 */
	dFeat = s_path_to_featproc(item, "utt_num_syls",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_J_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		j1 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_J_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_J_context", error);
	}
	else
	{
		j1 = 0;
	}


	/* j2 */
	dFeat = s_path_to_featproc(item, "utt_num_words",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_J_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		j2 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_J_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_J_context", error);
	}
	else
	{
		j2 = 0;
	}

	/* j3 */
	dFeat = s_path_to_featproc(item, "utt_num_phrases",
							   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_J_context",
				  "Call to \"s_path_to_featproc\" failed"))
		return NULL;

	if (dFeat != NULL)
	{
		j3 = SObjectGetInt(dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_J_context",
					  "Call to \"SObjectGetInt\" failed"))
			return NULL;

		S_DELETE(dFeat, "create_J_context", error);
	}
	else
	{
		j3 = 0;
	}

	s_asprintf(&j_context, error, "/J:%d+%d-%d", j1, j2, j3);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_J_context",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return j_context;
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

		/* D context */
		tmp = create_D_context_pause(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_D_context_pause\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* E context */
		tmp = create_E_context_pause(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_E_context_pause\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* F context */
		tmp = create_F_context_pause(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_F_context_pause\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* G context */
		tmp = create_G_context_pause(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_G_context_pause\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* H context */
		tmp = create_H_context_pause(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_H_context_pause\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* I context */
		tmp = create_I_context_pause(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_I_context_pause\" failed"))
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

		/* D context */
		tmp = create_D_context(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_D_context\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* E context */
		tmp = create_E_context(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_E_context\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* F context */
		tmp = create_F_context(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_F_context\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* G context */
		tmp = create_G_context(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_G_context\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* H context */
		tmp = create_H_context(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_H_context\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);

		/* I context */
		tmp = create_I_context(segItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_I_context\" failed"))
			goto quit_error;

		s_sappend(&hts_label, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_sappend\" failed"))
			goto quit_error;

		S_FREE(tmp);
	}

	/* J context */
	tmp = create_J_context(segItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"create_J_context\" failed"))
		goto quit_error;

	s_sappend(&hts_label, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_sappend\" failed"))
		goto quit_error;

	S_FREE(tmp);

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

	return NULL;

	S_UNUSED(self);
}


/************************************************************************************/
/*                                                                                  */
/* SHTSLabelsSimple3FeatProc class initialization                                   */
/*                                                                                  */
/************************************************************************************/

static SHTSLabelsSimple3FeatProcClass HTSLabelsSimple3FeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SHTSLabelsSimple3FeatProc",
		sizeof(SHTSLabelsSimple3FeatProc),
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
