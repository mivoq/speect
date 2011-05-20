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
/* AUTHOR  : Aby Louw                                                               */
/* DATE    : August 2010                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Kiswahili (swa) syllabification class implementation for                       */
/* the Freedomfone project.                                                         */
/* Based on syllabification algorithm of the Festival Kiswahili voice               */
/* implementation of the University of Nairobi. See README for their license.       */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include "swa_freedomfone.h"
#include "phoneset.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSyllabSwaFreedomfoneClass SyllabSwaFreedomfoneClass; /* SSyllabSwaFreedomfone class declaration. */


/************************************************************************************/
/*                                                                                  */
/*  Static function prototypes                                                      */
/*                                                                                  */
/************************************************************************************/

static s_bool test_g_n_ng(const char *current, const char *prev, s_erc *error);

static s_bool test_h_cluster(const SPhoneset *phoneset,
							 const char *current, const char *prev,
							 s_erc *error);

static s_bool test_c_illformed(const SPhoneset *phoneset,
							   const char *phone, s_erc *error);

static int phone_type(const SPhoneset *phoneset, const char *phone, s_erc *error);

static s_bool stress_test1(const SPhoneset *phoneset,
						   SList *secondLast, const SList *last, s_erc *error);

static void add_syllable_stress(const SPhoneset *phoneset, SList *syllables, s_erc *error);

static s_bool consonant_vowel(const SPhoneset *phoneset, const SList *last,
							  s_erc *error);

static s_bool phone_is_n_m(const char *phone, s_erc *error);

static s_bool stress_test2(const SPhoneset *phoneset,
						   SList *secondLast, const SList *last,
						   s_erc *error);

static s_bool consonant_semi_vowel(const SPhoneset *phoneset, const SList *last,
								   s_erc *error);

static s_bool phone_is_y_w(const char *phone, s_erc *error);

static s_bool stress_test3(const SPhoneset *phoneset,
						   SList *secondLast, const SList *last,
						   s_erc *error);

/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_syllab_swa_freedomfone_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&SyllabSwaFreedomfoneClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syllab_swa_freedomfone_class_reg",
			  "Failed to register SSyllabSwaFreedomfoneClass");
}


S_LOCAL void _s_syllab_swa_freedomfone_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&SyllabSwaFreedomfoneClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syllab_swa_freedomfone_class_free",
			  "Failed to free SSyllabSwaFreedomfoneClass");
}


/************************************************************************************/
/*                                                                                  */
/*  Static function implementations                                                 */
/*                                                                                  */
/************************************************************************************/

static s_bool test_g_n_ng(const char *current, const char *prev, s_erc *error)
{
	int scomp;


	S_CLR_ERR(error);
	scomp = s_strcmp(current, "g", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_g_n_ng",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (scomp != 0)
		return FALSE;

	scomp = s_strcmp(prev, "n", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_g_n_ng",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (scomp == 0)
		return TRUE;

	scomp = s_strcmp(prev, "ng", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_g_n_ng",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (scomp == 0)
		return TRUE;

	return FALSE;
}


static s_bool test_h_cluster(const SPhoneset *phoneset,
							 const char *current, const char *prev,
							 s_erc *error)
{
	int scomp;
	s_bool in_list;
	const SObject *tmp;
	const SList *clusterList;
	SObject *prevObject;


	S_CLR_ERR(error);
	scomp = s_strcmp(current, "h", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_h_cluster",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (scomp != 0)
		return FALSE;

	tmp = S_PHONESET_CALL(phoneset, get_feature)(phoneset,
												 "wellformed_h_clusters",
												 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_h_cluster",
				  "Call to SPhoneset method \"get_feature\" failed"))
		return FALSE;

	clusterList = S_CAST(tmp, SList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_h_cluster",
				  "Call to \"S_CAST (SList)\" failed"))
		return FALSE;

	prevObject = SObjectSetString(prev, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_h_cluster",
				  "Call to \"SObjectSetString\" failed"))
		return FALSE;

	in_list = SListValPresent(clusterList, prevObject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_h_cluster",
				  "Call to \"SListValPresent\" failed"))
	{
		S_DELETE(prevObject, "test_h_cluster", error);
		return FALSE;
	}

	S_DELETE(prevObject, "test_h_cluster", error);
	if (in_list)
		return TRUE;

	return FALSE;
}


static s_bool test_c_illformed(const SPhoneset *phoneset,
							 const char *phone, s_erc *error)
{
	s_bool in_list;
	const SObject *tmp;
	const SList *clusterList;
	SObject *thisObject;


	S_CLR_ERR(error);
	tmp = S_PHONESET_CALL(phoneset, get_feature)(phoneset,
												 "illformed_c_clusters",
												 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_c_illformed",
				  "Call to SPhoneset method \"get_feature\" failed"))
		return FALSE;

	clusterList = S_CAST(tmp, SList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_c_illformed",
				  "Call to \"S_CAST (SList)\" failed"))
		return FALSE;

	thisObject = SObjectSetString(phone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_c_illformed",
				  "Call to \"SObjectSetString\" failed"))
		return FALSE;

	in_list = SListValPresent(clusterList, thisObject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_c_illformed",
				  "Call to \"SListValPresent\" failed"))
	{
		S_DELETE(thisObject, "test_c_illformed", error);
		return FALSE;
	}

	S_DELETE(thisObject, "test_c_illformed", error);
	if (in_list)
		return TRUE;

	return FALSE;
}


static int phone_type(const SPhoneset *phoneset, const char *phone, s_erc *error)
{
	s_bool is_vowel;


	S_CLR_ERR(error);
	is_vowel = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset,
															phone,
															"vowel",
															error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_type",
				  "Call to method \"phone_has_feature\" failed"))
		return 0;

	if (is_vowel)
		return 1;

	return 2;
}


/* test 1, "last syllable is a vowel" in scheme code
 *      ( ANY * [ a ] - V # = a1 )
 *   	( ANY * [ e ] - V # = e1 )
 *   	( ANY * [ i ] - V # = i1 )
 *   	( ANY * [ o ] - V # = o1 )
 *   	( ANY * [ u ] - V # = u1 )
 */
static s_bool stress_test1(const SPhoneset *phoneset,
						   SList *secondLast, const SList *last,
						   s_erc *error)
{
	size_t size;
	const char *lphone = NULL;
	int p_type;
	SObject *phoneO;
	char *stressed_phone = NULL;


	S_CLR_ERR(error);

	size = SListSize(last, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test1",
				  "Call to \"SListSize\" failed"))
		return FALSE;

	if (size != 1)
		return FALSE;

	lphone = SObjectGetString(SListNth(last, 0, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test1",
				  "Call to \"SListNth/SObjectGetString\" failed"))
		return FALSE;

	p_type = phone_type(phoneset, lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test1",
				  "Call to \"phone_type\" failed"))
		return FALSE;

	if (p_type != 1)
		return FALSE;

	/* check second last syllable */
	size = SListSize(secondLast, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test1",
				  "Call to \"SListSize\" failed"))
		return FALSE;

	phoneO = (SObject*)SListNth(secondLast, size - 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test1",
				  "Call to \"SListNth\" failed"))
		return FALSE;

	lphone = SObjectGetString(phoneO, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test1",
				  "Call to \"SObjectGetString\" failed"))
		return FALSE;

	p_type = phone_type(phoneset, lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test1",
				  "Call to \"phone_type\" failed"))
		return FALSE;

	if (p_type != 1)
		return FALSE;

	size = s_strzsize(lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test1",
				  "Call to \"s_strsize\" failed"))
		return FALSE;

	stressed_phone = S_CALLOC(char, size + 1);
	if (stressed_phone == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "stress_test1",
				  "Failed to allocate memory for 'char' object");
		return FALSE;
	}

	s_strcat(stressed_phone, lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test1",
				  "Call to \"s_strcat\" failed"))
	{
		S_FREE(stressed_phone);
		return FALSE;
	}

	s_strcat(stressed_phone, "1", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test1",
				  "Call to \"s_strcat\" failed"))
	{
		S_FREE(stressed_phone);
		return FALSE;
	}

	SObjectResetString(phoneO, stressed_phone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test1",
				  "Call to \"SObjectResetString\" failed"))
	{
		S_FREE(stressed_phone);
		return FALSE;
	}

	S_FREE(stressed_phone);
	return TRUE;
}


static s_bool consonant_vowel(const SPhoneset *phoneset, const SList *last,
							  s_erc *error)
{
	size_t size;
	const char *lphone = NULL;
	int p_type;


	S_CLR_ERR(error);

	size = SListSize(last, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "consonant_vowel",
				  "Call to \"SListSize\" failed"))
		return FALSE;

	if (size != 2)
		return FALSE;

	lphone = SObjectGetString(SListNth(last, size - 2, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "consonant_vowel",
				  "Call to \"SListNth/SObjectGetString\" failed"))
		return FALSE;

	p_type = phone_type(phoneset, lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "consonant_vowel",
				  "Call to \"phone_type\" failed"))
		return FALSE;

	if (p_type != 2)
		return FALSE;

	lphone = SObjectGetString(SListNth(last, size - 1, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "consonant_vowel",
				  "Call to \"SListNth/SObjectGetString\" failed"))
		return FALSE;

	p_type = phone_type(phoneset, lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "consonant_vowel",
				  "Call to \"phone_type\" failed"))
		return FALSE;

	if (p_type != 1)
		return FALSE;

	return TRUE;
}

static s_bool phone_is_n_m(const char *phone, s_erc *error)
{
	int scomp;


	S_CLR_ERR(error);
	scomp = s_strcmp(phone, "n", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_n_m",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (scomp == 0)
		return TRUE;

	scomp = s_strcmp(phone, "m", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_n_m",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (scomp == 0)
		return TRUE;

	return FALSE;
}


static s_bool phone_is_y_w(const char *phone, s_erc *error)
{
	int scomp;


	S_CLR_ERR(error);
	scomp = s_strcmp(phone, "y", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_y_w",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (scomp == 0)
		return TRUE;

	scomp = s_strcmp(phone, "w", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_y_w",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (scomp == 0)
		return TRUE;

	return FALSE;
}


/* test 2, "last syllable is a consonant followed by a vowel" in scheme code
 *      ( ANY * [ a ] - C V # = a1 )
 *   	( ANY * [ e ] - C V # = e1 )
 *   	( ANY * [ i ] - C V # = i1 )
 *   	( ANY * [ o ] - C V # = o1 )
 *   	( ANY * [ u ] - C V # = u1 )
 *   	( ANY * [ m ] - C V # = m1 )
 *   	( ANY * [ n ] - C V # = n1 )
 */
static s_bool stress_test2(const SPhoneset *phoneset,
						   SList *secondLast, const SList *last,
						   s_erc *error)
{
	s_bool is_good;
	s_bool n_or_m;
	size_t size;
	const char *lphone = NULL;
	int p_type;
	SObject *phoneO;
	char *stressed_phone = NULL;


	S_CLR_ERR(error);

	is_good = consonant_vowel(phoneset, last, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test2",
				  "Call to \"consonant_vowel\" failed"))
		return FALSE;

	if (!is_good)
		return FALSE;

	/* check second last syllable */
	size = SListSize(secondLast, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test2",
				  "Call to \"SListSize\" failed"))
		return FALSE;

	phoneO = (SObject*)SListNth(secondLast, size - 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test2",
				  "Call to \"SListNth\" failed"))
		return FALSE;

	lphone = SObjectGetString(phoneO, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test2",
				  "Call to \"SObjectGetString\" failed"))
		return FALSE;

	p_type = phone_type(phoneset, lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test2",
				  "Call to \"phone_type\" failed"))
		return FALSE;

	n_or_m = phone_is_n_m(lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test2",
				  "Call to \"phone_is_n_m\" failed"))
		return FALSE;

	if ((p_type != 1) && (!n_or_m))
		return FALSE;

	size = s_strzsize(lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test2",
				  "Call to \"s_strsize\" failed"))
		return FALSE;

	stressed_phone = S_CALLOC(char, size + 1);
	if (stressed_phone == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "stress_test2",
				  "Failed to allocate memory for 'char' object");
		return FALSE;
	}

	s_strcat(stressed_phone, lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test2",
				  "Call to \"s_strcat\" failed"))
	{
		S_FREE(stressed_phone);
		return FALSE;
	}

	s_strcat(stressed_phone, "1", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test2",
				  "Call to \"s_strcat\" failed"))
	{
		S_FREE(stressed_phone);
		return FALSE;
	}

	SObjectResetString(phoneO, stressed_phone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test2",
				  "Call to \"SObjectResetString\" failed"))
	{
		S_FREE(stressed_phone);
		return FALSE;
	}

	S_FREE(stressed_phone);
	return TRUE;
}


static s_bool consonant_semi_vowel(const SPhoneset *phoneset, const SList *last,
								   s_erc *error)
{
	size_t size;
	const char *lphone = NULL;
	int p_type;
	s_bool is_semi_vowel;


	S_CLR_ERR(error);

	size = SListSize(last, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "consonant_semi_vowel",
				  "Call to \"SListSize\" failed"))
		return FALSE;

	if (size != 3)
		return FALSE;

	lphone = SObjectGetString(SListNth(last, size - 3, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "consonant_semi_vowel",
				  "Call to \"SListNth/SObjectGetString\" failed"))
		return FALSE;

	p_type = phone_type(phoneset, lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "consonant_semi_vowel",
				  "Call to \"phone_type\" failed"))
		return FALSE;

	if (p_type != 2)
		return FALSE;

	lphone = SObjectGetString(SListNth(last, size - 2, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "consonant_semi_vowel",
				  "Call to \"SListNth/SObjectGetString\" failed"))
		return FALSE;

	is_semi_vowel = phone_is_y_w(lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "consonant_semi_vowel",
				  "Call to \"phone_is_y_w\" failed"))
		return FALSE;

	if (!is_semi_vowel)
		return FALSE;

	lphone = SObjectGetString(SListNth(last, size - 1, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "consonant_semi_vowel",
				  "Call to \"SListNth/SObjectGetString\" failed"))
		return FALSE;

	p_type = phone_type(phoneset, lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "consonant_semi_vowel",
				  "Call to \"phone_type\" failed"))
		return FALSE;

	if (p_type != 1)
		return FALSE;

	return TRUE;
}


/* test 3, "last syllable is a consonant followed a semi-vowel then a vowel" in scheme code
 *      ( ANY * [ a ] - C SV V# = a1 )
 *   	( ANY * [ e ] - C SV V # = e1 )
 *   	( ANY * [ i ] - C SV V # = i1 )
 *   	( ANY * [ o ] - C SV V # = o1 )
 *   	( ANY * [ u ] - C SV V # = u1 )
 *   	( ANY * [ m ] - C SV V # = m1 )
 *   	( ANY * [ n ] - C SV V # = n1 )
 */
static s_bool stress_test3(const SPhoneset *phoneset,
						   SList *secondLast, const SList *last,
						   s_erc *error)
{
	s_bool is_good;
	s_bool n_or_m;
	size_t size;
	const char *lphone = NULL;
	int p_type;
	SObject *phoneO;
	char *stressed_phone = NULL;


	S_CLR_ERR(error);

	is_good = consonant_semi_vowel(phoneset, last, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test3",
				  "Call to \"consonant_semi_vowel\" failed"))
		return FALSE;

	if (!is_good)
		return FALSE;

	/* check second last syllable */
	size = SListSize(secondLast, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test3",
				  "Call to \"SListSize\" failed"))
		return FALSE;

	phoneO = (SObject*)SListNth(secondLast, size - 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test3",
				  "Call to \"SListNth\" failed"))
		return FALSE;

	lphone = SObjectGetString(phoneO, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test3",
				  "Call to \"SObjectGetString\" failed"))
		return FALSE;

	p_type = phone_type(phoneset, lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test3",
				  "Call to \"phone_type\" failed"))
		return FALSE;

	n_or_m = phone_is_n_m(lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test3",
				  "Call to \"phone_is_n_m\" failed"))
		return FALSE;

	if ((p_type != 1) && (!n_or_m))
		return FALSE;

	size = s_strzsize(lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test3",
				  "Call to \"s_strsize\" failed"))
		return FALSE;

	stressed_phone = S_CALLOC(char, size + 1);
	if (stressed_phone == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "stress_test3",
				  "Failed to allocate memory for 'char' object");
		return FALSE;
	}

	s_strcat(stressed_phone, lphone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test3",
				  "Call to \"s_strcat\" failed"))
	{
		S_FREE(stressed_phone);
		return FALSE;
	}

	s_strcat(stressed_phone, "1", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test3",
				  "Call to \"s_strcat\" failed"))
	{
		S_FREE(stressed_phone);
		return FALSE;
	}

	SObjectResetString(phoneO, stressed_phone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "stress_test3",
				  "Call to \"SObjectResetString\" failed"))
	{
		S_FREE(stressed_phone);
		return FALSE;
	}

	S_FREE(stressed_phone);
	return TRUE;
}



static void add_syllable_stress(const SPhoneset *phoneset, SList *syllables, s_erc *error)
{
	size_t list_size;
	SList *secondLast;
	const SList *last;
	s_bool done;


	S_CLR_ERR(error);
	list_size = SListSize(syllables, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_syllable_stress",
				  "Call to \"SListSize\" failed"))
		return;

	if (list_size < 2)
		return;  /* no rules for single syllable words */

	/* get second last syllable */
	secondLast = (SList*)SListNth(syllables, list_size - 2, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_syllable_stress",
				  "Call to \"SListNth\" failed"))
		return;

	/* get last syllable */
	last = (const SList*)SListNth(syllables, list_size - 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_syllable_stress",
				  "Call to \"SListNth\" failed"))
		return;

	list_size = SListSize(last, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_syllable_stress",
				  "Call to \"SListSize\" failed"))
		return;

	if (list_size == 1)
	{
		/* test 1, "last syllable is a vowel" in scheme code */
		done = stress_test1(phoneset, secondLast, last, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_syllable_stress",
					  "Call to \"stress_test1\" failed"))
			return;

		if (done)
			return;
	}
	else if (list_size == 2)
	{
		/* test 2, "last syllable is a consonant followed by a vowel" in scheme code */
		done = stress_test2(phoneset, secondLast, last, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_syllable_stress",
					  "Call to \"stress_test2\" failed"))
			return;

		if (done)
			return;

	}
	else if (list_size == 3)
	{
		/* test 3, "ast syllable is a consonant followed a semi-vowel then a vowel" in scheme code */
		done = stress_test3(phoneset, secondLast, last, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_syllable_stress",
					  "Call to \"stress_test3\" failed"))
			return;
	}
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


static const char *GetName(const SSyllabification *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;

	return (const char*)self->info->name;
}


static const char *GetDescription(const SSyllabification *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;

	return (const char*)self->info->description;
}


static const char *GetLanguage(const SSyllabification *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;

	return (const char*)self->info->language;
}


static const char *GetLangCode(const SSyllabification *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;

	return (const char*)self->info->lang_code;
}


static const s_version *SGetVersion(const SSyllabification *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;

	return (const s_version*)&(self->info->version);
}


static const SObject *GetFeature(const SSyllabification *self, const char *key,
								 s_erc *error)
{
	const SObject *feature;


	S_CLR_ERR(error);
	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetFeature",
				  "Argument \"key\" is NULL");
		return NULL;
	}

	if (self->features == NULL)
		return NULL;

	feature = SMapGetObjectDef(self->features, key, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetFeature",
				  "Call to \"SMapGetObjectDef\" failed"))
		return NULL;

	return feature;
}


/**
 * return a vallist of vallists where the primary list is syllables and the secondary
 * lists are the phones in the syllables. for example :
 * for mathematics phonelist is : (m , ae , th, ax, m, ae, t, ih, k, s)
 * syllfunc returns : ((m, ae), (th, ax), (m, ae), (t, ih, k, s))
 */

static SList *Syllabify(const SSyllabification *self, const SItem *word,
						const SList *phoneList, s_erc *error)
{
	const SPhoneset *phoneset;
	const SVoice *voice;
	SList *phoneListCopy = NULL;
	SList *syllables = NULL;
	SList *syl;
	const SObject *tmp;
	const char *phone_string;
	size_t list_size;
	int prev_type = 0; /* 0 = not defined, 1 = vowel, 2 = consonant */
	char prev_consonant[4] = "\0";


	S_CLR_ERR(error);

	if (word == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "Syllabify",
				  "Argument \"word\" is NULL");
		goto quit_error;
	}

	if (phoneList == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "Syllabify",
				  "Argument \"phoneList\" is NULL");
		goto quit_error;
	}

	/* make a copy of the phone list with which we can work */
	phoneListCopy = SListCopy(NULL, phoneList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Call to \"SListCopy\" failed"))
		goto quit_error;

	voice = SItemVoice(word, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Call to \"SItemVoice\" failed"))
		goto quit_error;

	if (voice == NULL)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Syllabification requires a voice, through the voice "
				  "syllabification gets access to a phoneset");
		goto quit_error;
	}

	phoneset = (SPhoneset*)SVoiceGetData(voice, "phoneset", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Call to \"SVoiceGetFeature\" failed"))
		goto quit_error;

	if (phoneset == NULL)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Syllabification requires a phoneset, none found in voice features");
		goto quit_error;
	}

	/* create syllables list */
	syllables = (SList*)S_NEW("SListList", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Failed to create new 'SList' object"))
		goto quit_error;

	/* create syllable */
	syl = (SList*)S_NEW("SListList", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Failed to create new 'SList' object"))
		goto quit_error;

	SListPush(syllables, S_OBJECT(syl), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Call to \"SListPush\" failed"))
	{
		S_DELETE(syl, "Syllabify", error);
		goto quit_error;
	}

	SListReverse(phoneListCopy, error); /* so that we can use pop */
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Call to \"SListReverse\" failed"))
		goto quit_error;

	while ((list_size = SListSize(phoneListCopy, error)) > 0)
	{
		/* check for list size error here */
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"SListSize\" failed"))
			goto quit_error;

		/* pop phone */
		tmp = SListPop(phoneListCopy, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"SListPop\" failed"))
			goto quit_error;
		list_size--;  /* popped 1 */

		phone_string = SObjectGetString(tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"SObjectGetString\" failed"))
			goto quit_error;

		if (prev_type == 0)
		{
			/* add to current syllable */
			SListPush(syl, SObjectSetString(phone_string, error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"SListPush/SObjectSetString\" failed"))
				goto quit_error;

			prev_type = phone_type(phoneset, phone_string, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"phone_type\" failed"))
				goto quit_error;

			if (prev_type == 2)
			{
				s_strzcpy(prev_consonant, phone_string, 4, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"s_strzcpy\" failed"))
					goto quit_error;
			}

			continue;
		}

		/* If there is a vowel before any phone its a syl break
		 * triptongs and diptongs do not exist in Kiswahili
		 */
		if (prev_type == 1)
		{
			/* previous was a syllable boundary */
			/* new syllable */
			syl = (SList*)S_NEW("SListList", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Failed to create new 'SList' object"))
				goto quit_error;

			SListPush(syllables, S_OBJECT(syl), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"SListPush\" failed"))
			{
				S_DELETE(syl, "Syllabify", error);
				goto quit_error;
			}

			/*
			 * we add the phone_string so that it is
			 * independent from the phoneList
			 */
			SListPush(syl, SObjectSetString(phone_string, error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"SListPush/SObjectSetString\" failed"))
				goto quit_error;

			prev_type = phone_type(phoneset, phone_string, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"phone_type\" failed"))
				goto quit_error;

			if (prev_type == 2)
			{
				s_strzcpy(prev_consonant, phone_string, 4, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"s_strzcpy\" failed"))
					goto quit_error;
			}

			continue;
		}

		if (prev_type == 2)
		{
			s_bool btest;


			/* if current phone is g and previous was n or ng
			 * then this is NOT a syllable break
			 */
			btest = test_g_n_ng(phone_string, prev_consonant, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"test_g_n_ng\" failed"))
				goto quit_error;

			if (btest)
			{
				/* add to syllable */
				SListPush(syl, SObjectSetString(phone_string, error), error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"SListPush/SObjectSetString\" failed"))
					goto quit_error;

				prev_type = 2;
				s_strzcpy(prev_consonant, phone_string, 4, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"s_strzcpy\" failed"))
					goto quit_error;

				continue;
			}

			/* if current phone is h and with previous, "ch",
			 * "d", "dh", "g", "gh", "k", "kh", "s", "sh", "t",
			 * "th", makes well-formed cluster then this is NOT
			 * a syllable break
			 */
			btest = test_h_cluster(phoneset, phone_string, prev_consonant,
								   error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"test_h_cluster\" failed"))
				goto quit_error;

			if (btest)
			{
				/* add to syllable */
				SListPush(syl, SObjectSetString(phone_string, error), error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"SListPush/SObjectSetString\" failed"))
					goto quit_error;

				prev_type = 2;
				s_strzcpy(prev_consonant, phone_string, 4, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"s_strzcpy\" failed"))
					goto quit_error;

				continue;
			}


			btest = test_c_illformed(phoneset, phone_string, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"test_c_illformed\" failed"))
				goto quit_error;

			if (btest)
			{
				/* previous was a syllable boundary */
				/* new syllable */
				syl = (SList*)S_NEW("SListList", error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Failed to create new 'SList' object"))
					goto quit_error;

				SListPush(syllables, S_OBJECT(syl), error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"SListPush\" failed"))
				{
					S_DELETE(syl, "Syllabify", error);
					goto quit_error;
				}

				/*
				 * we add the phone_string so that it is
				 * independent from the phoneList
				 */
				SListPush(syl, SObjectSetString(phone_string, error), error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"SListPush/SObjectSetString\" failed"))
					goto quit_error;

				prev_type = phone_type(phoneset, phone_string, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"phone_type\" failed"))
					goto quit_error;

				if (prev_type == 2)
				{
					s_strzcpy(prev_consonant, phone_string, 4, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "Syllabify",
								  "Call to \"s_strzcpy\" failed"))
						goto quit_error;
				}

				continue;
			}

			/* All other phones just get added to current syllable
			 * we add the phone_string so that it is
			 * independent from the phoneList
			 */
			SListPush(syl, SObjectSetString(phone_string, error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"SListPush/SObjectSetString\" failed"))
				goto quit_error;

			prev_type = phone_type(phoneset, phone_string, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"phone_type\" failed"))
				goto quit_error;

			if (prev_type == 2)
			{
				s_strzcpy(prev_consonant, phone_string, 4, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"s_strzcpy\" failed"))
					goto quit_error;
			}
		}
	}


	S_DELETE(phoneListCopy, "Syllabify", error);

	add_syllable_stress(phoneset, syllables, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Call to \"add_syllable_stress\" failed"))
		goto quit_error;


	return syllables;

quit_error:
	if (syllables != NULL)
		S_DELETE(syllables, "Syllabify", error);

	if (phoneListCopy != NULL)
		S_DELETE(phoneListCopy, "Syllabify", error);

	return NULL;

	/* for unused compiler warning */
	self = NULL;
}


/************************************************************************************/
/*                                                                                  */
/* SSyllabSwaFreedomfone class initialization                                       */
/*                                                                                  */
/************************************************************************************/

static SSyllabSwaFreedomfoneClass SyllabSwaFreedomfoneClass =
{
	/* SObjectClass */
	{
		"SSyllabification:SSyllabSwaFreedomfone",
		sizeof(SSyllabSwaFreedomfone),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SSyllabificationClass */
	GetName,             /* get_name        */
	GetDescription,      /* get_description */
	GetLanguage,         /* get_language    */
	GetLangCode,         /* get_lang_code   */
	SGetVersion,         /* get_version     */
	GetFeature,          /* get_feature     */
	Syllabify            /* syllabify       */
};
