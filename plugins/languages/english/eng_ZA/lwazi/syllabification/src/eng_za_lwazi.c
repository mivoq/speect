/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A South African English (eng-za) syllabification class                           */
/* implementation for the Lwazi project. Based on T.A. Hall,                        */
/* "English syllabification as the interaction of markedness                        */
/* constraints*", Studia Linguistica, vol. 60, 2006, pp. 1-33                       */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include "eng_za_lwazi.h"
#include "phoneset.h"


/************************************************************************************/
/*                                                                                  */
/* Data Types                                                                       */
/*                                                                                  */
/************************************************************************************/

typedef enum
{
	S_VCV_CLUSTER,
	S_VCCV_CLUSTER,
	S_VCCCV_CLUSTER,
	S_VCCCCV_CLUSTER,
	S_VCGV_CLUSTER,
	S_VCCGV_CLUSTER,
	S_VCCCGV_CLUSTER,
	S_VV_CLUSTER,
	S_UNKNOWN_CLUSTER
} s_cluster;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSyllabEngZaLwaziClass SyllabEngZaLwaziClass; /* SSyllabEngZaLwazi class declaration. */


/************************************************************************************/
/*                                                                                  */
/*  Static function prototypes                                                      */
/*                                                                                  */
/************************************************************************************/

static s_bool phone_is_liquid(const SPhoneset *phoneset, const char *phone, s_erc *error);

static s_bool phone_is_syllabic_consonant(const char *phone, s_erc *error);

static s_bool phone_is_approximant_liquid(const SPhoneset *phoneset, const char *phone,
										  s_erc *error);

static s_bool phone_is_obstrudent(const SPhoneset *phoneset, const char *phone,
								  s_erc *error);

static uint8 phone_sonority_level(const SPhoneset *phoneset, const char *phone,
								  s_erc *error);

static s_bool test_phone_cluster(const SPhoneset *phoneset, const char *cluster,
								 const char *cluster_name, s_erc *error);

static s_bool permissible_consonant_clusters(const SPhoneset *phoneset,
											 const char *c_1, const char *c_rest,
											 s_erc *error);

static void pop_CV(SList *syllables, SList **syl, s_erc *error);

static void pop_CCV(SList *syllables, SList **syl, s_erc *error);

static void pop_CCCV(SList *syllables, SList **syl, s_erc *error);

static void process_VCV(SList *syllables, SList **syl,
						char *current_cluster, s_erc *error);

static void process_VCCV(const SPhoneset *phoneset, SList *syllables,
						 SList **syl, char *current_cluster, s_erc *error);

static void process_VCCCV(const SPhoneset *phoneset, SList *syllables, SList **syl,
						  char *current_cluster, s_erc *error);

static void process_VCCCCV(SList *syllables, SList **syl,
						   char *current_cluster, s_erc *error);

static void process_VCGV(const SPhoneset *phoneset, SList *syllables, SList **syl,
						 char *current_cluster, s_erc *error);

static void process_VCCGV(SList *syllables, SList **syl,
						  char *current_cluster, s_erc *error);

static void process_VCCCGV(SList *syllables, SList **syl,
						   char *current_cluster, s_erc *error);

static void process_VV(SList *syllables, SList **syl,
					   char *current_cluster, s_erc *error);

static s_cluster get_cluster_type(const char *current_cluster, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_syllab_eng_za_lwazi_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&SyllabEngZaLwaziClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syllab_eng_za_lwazi_class_reg",
			  "Failed to register SSyllabEngZaLwaziClass");
}


S_LOCAL void _s_syllab_eng_za_lwazi_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&SyllabEngZaLwaziClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syllab_eng_za_lwazi_class_free",
			  "Failed to free SSyllabEngZaLwaziClass");
}


/************************************************************************************/
/*                                                                                  */
/*  Static function implementations                                                 */
/*                                                                                  */
/************************************************************************************/

static s_bool phone_is_liquid(const SPhoneset *phoneset, const char *phone, s_erc *error)
{
	s_bool present;


	S_CLR_ERR(error);
	present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
														   "manner_trill",
														   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_liquid",
				  "Call to method \"phone_has_feature\" failed"))
		return FALSE;

	if (present)
		return TRUE;

	present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
														   "manner_flap",
														   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_liquid",
				  "Call to method \"phone_has_feature\" failed"))
		return FALSE;

	if (present)
		return TRUE;

	present = phone_is_approximant_liquid(phoneset, phone, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_liquid",
				  "Call to \"phone_is_approximant_liquid\" failed"))
		return FALSE;

	if (present)
		return TRUE;

	return FALSE;
}


/* ABY: I think this is only /l/ and /n/. */
static s_bool phone_is_syllabic_consonant(const char *phone, s_erc *error)
{
	int comp;

	S_CLR_ERR(error);

	comp = s_strcmp(phone, "l", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_syllabic_consonant",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (comp == 0)
		return TRUE;

	comp = s_strcmp(phone, "n", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_syllabic_consonant",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (comp == 0)
		return TRUE;

	return FALSE;
}


static s_bool phone_is_approximant_liquid(const SPhoneset *phoneset, const char *phone,
										  s_erc *error)
{
	s_bool present_aprox;
	s_bool present_liquid;


	present_aprox = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
																 "manner_approximant",
																 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_approximant_liquid",
				  "Call to method \"phone_has_feature\" failed"))
		return FALSE;

	present_liquid = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
																  "manner_liquid",
																  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_approximant_liquid",
				  "Call to method \"phone_has_feature\" failed"))
		return FALSE;

	if (present_aprox && present_liquid)
		return TRUE;

	return FALSE;
}


static s_bool phone_is_obstrudent(const SPhoneset *phoneset, const char *phone,
								  s_erc *error)
{
	s_bool present_son;
	s_bool present_syl;
	s_bool present_cons;


	S_CLR_ERR(error);

	present_son = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
															   "class_sonorant",
															   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_glide",
				  "Call to method \"phone_has_feature\" failed"))
		return FALSE;


	present_syl = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
															   "class_syllabic",
															   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_glide",
				  "Call to method \"phone_has_feature\" failed"))
		return FALSE;

	present_cons = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
																"class_consonantal",
																error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_glide",
				  "Call to method \"phone_has_feature\" failed"))
		return FALSE;

	if (!present_son && !present_syl && present_cons)
		return TRUE;

	return FALSE;
}


static uint8 phone_sonority_level(const SPhoneset *phoneset, const char *phone, s_erc *error)
{
	s_bool present;


	S_CLR_ERR(error);
	present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
														   "vowel",
														   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_sonority_level",
				  "Call to method \"phone_has_feature\" failed"))
		return 0;

	if (present)
	{
		present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
															   "height_low",
															   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "phone_sonority_level",
					  "Call to method \"phone_has_feature\" failed"))
			return 0;

		if (present)
			return 9;

		present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
															   "height_mid",
															   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "phone_sonority_level",
					  "Call to method \"phone_has_feature\" failed"))
			return 0;

		if (present)
			return 8;


		present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
															   "height_high",
															   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "phone_sonority_level",
					  "Call to method \"phone_has_feature\" failed"))
			return 0;

		if (present)
			return 7;

		return 7; /* for diphthongs ???*/
	}

	present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
														   "manner_liquid",
														   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_sonority_level",
				  "Call to method \"phone_has_feature\" failed"))
		return 0;

	if (present)
		return 6;

	present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
														   "manner_nasal",
														   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_sonority_level",
				  "Call to method \"phone_has_feature\" failed"))
		return 0;

	if (present)
		return 5;


	present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
														   "manner_fricative",
														   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_sonority_level",
				  "Call to method \"phone_has_feature\" failed"))
		return 0;

	if (present)
	{
		present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
															   "voiced",
															   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "phone_sonority_level",
					  "Call to method \"phone_has_feature\" failed"))
			return 0;

		if (present)
			return 4;

		return 3;
	}

	present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
														   "manner_plosive",
														   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_sonority_level",
				  "Call to method \"phone_has_feature\" failed"))
		return 0;

	if (present)
	{
		present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
															   "voiced",
															   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "phone_sonority_level",
					  "Call to method \"phone_has_feature\" failed"))
			return 0;

		if (present)
			return 2;

		return 1;
	}

	return 0;
}


static s_bool test_phone_cluster(const SPhoneset *phoneset, const char *cluster,
								 const char *cluster_name, s_erc *error)
{
	const SObject *tmp;
	const SList *clusterList;
	SIterator *itr;
	const char *wellformed;
	int comp;


	S_CLR_ERR(error);

	tmp = S_PHONESET_CALL(phoneset, get_feature)(phoneset,
												 cluster_name,
												 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_phone_cluster",
				  "Call to SPhoneset method \"get_feature\" failed"))
		return FALSE;

	clusterList = S_CAST(tmp, SList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_phone_cluster",
				  "Call to \"S_CAST (SList)\" failed"))
		return FALSE;

	itr = S_ITERATOR_GET(clusterList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "test_phone_cluster",
				  "Call to \"S_ITERATOR_GET\" failed"))
		return FALSE;

	while (itr != NULL)
	{
		tmp = SIteratorObject(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "test_phone_cluster",
					  "Call to \"SIteratorObject\" failed"))
		{
			S_DELETE(itr, "test_phone_cluster", error);
			return FALSE;
		}

		wellformed = SObjectGetString(tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "test_phone_cluster",
					  "Call to \"SObjectGetString\" failed"))
		{
			S_DELETE(itr, "test_phone_cluster", error);
			return FALSE;
		}

		comp = s_strcmp(cluster, wellformed, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "test_phone_cluster",
					  "Call to \"SObjectGetString\" failed"))
		{
			S_DELETE(itr, "test_phone_cluster", error);
			return FALSE;
		}

		if (comp == 0)
		{
			S_DELETE(itr, "test_phone_cluster", error);
			return TRUE;
		}

		itr = SIteratorNext(itr);
	}

	return FALSE;
}


/* ABY check this */
static s_bool permissible_consonant_clusters(const SPhoneset *phoneset,
											 const char *c_1, const char *c_rest,
											 s_erc *error)
{
	char cluster[10] = "\0";
	s_bool test_result;
	int comp;


	S_CLR_ERR(error);

	s_strcat(cluster, c_1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "permissible_consonant_clusters",
				  "Call to \"s_strcat\" failed"))
		return FALSE;

	s_strcat(cluster, c_rest, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "permissible_consonant_clusters",
				  "Call to \"s_strcat\" failed"))
		return FALSE;

	/* /s/ clusters */
	comp = s_strcmp(c_1, "s", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "permissible_consonant_clusters",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (comp == 0)
	{
		test_result = test_phone_cluster(phoneset, cluster,
										 "wellformed_s_clusters", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "permissible_consonant_clusters",
					  "Call to \"test_phone_cluster\" failed"))
			return FALSE;

		return test_result;
	}

	/* plosive clusters */
	test_result = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, c_1,
															   "manner_plosive",
															   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "permissible_consonant_clusters",
				  "Call to SPhoneset method \"phone_has_feature\" failed"))
		return FALSE;

	if (test_result)
	{
		test_result = test_phone_cluster(phoneset, cluster,
										 "wellformed_plosive_clusters", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "permissible_consonant_clusters",
					  "Call to \"test_phone_cluster\" failed"))
			return FALSE;

		return test_result;
	}

	/* fricative clusters not /s/ */
	test_result = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, c_1,
															   "manner_fricative",
															   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "permissible_consonant_clusters",
				  "Call to SPhoneset method \"phone_has_feature\" failed"))
		return FALSE;

	if (test_result)
	{
		test_result = test_phone_cluster(phoneset, cluster,
										 "wellformed_fricative_clusters", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "permissible_consonant_clusters",
					  "Call to \"test_phone_cluster\" failed"))
			return FALSE;

		return test_result;
	}

	/* other clusters */
	test_result = test_phone_cluster(phoneset, cluster,
									 "wellformed_other_clusters", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "permissible_consonant_clusters",
				  "Call to \"test_phone_cluster\" failed"))
		return FALSE;

	return test_result;
}


static void pop_CV(SList *syllables, SList **syl, s_erc *error)
{
	SList *nextSyl;

	S_CLR_ERR(error);

	/*
	 * pop CV and put it back into the phonelist
	 * create a new syl and put into syllables.
	 */
	nextSyl = S_LIST(S_NEW(SListList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CV",
				  "Failed to create new 'SList' object"))
		return;

	SListPush(nextSyl, SListPop((*syl), error), error); /* V */
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CV",
				  "Call to \"SListPush/SListPop\" failed"))
		return;

	SListPush(nextSyl, SListPop((*syl), error), error); /* C */
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CV",
				  "Call to \"SListPush/SListPop\" failed"))
		return;

	SListReverse(nextSyl, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CV",
				  "Call to \"SListReverse\" failed"))
		return;

	SListPush(syllables, S_OBJECT(nextSyl), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CV",
				  "Call to \"SListPush\" failed"))
		return;

	(*syl) = nextSyl;
}


static void pop_CCV(SList *syllables, SList **syl, s_erc *error)
{
	SList *nextSyl;

	S_CLR_ERR(error);

	/*
	 * pop CCV and put it back into the phonelist
	 * create a new syl and put into syllables.
	 */
	nextSyl = S_LIST(S_NEW(SListList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CCV",
				  "Failed to create new 'SList' object"))
		return;

	SListPush(nextSyl, SListPop((*syl), error), error); /* V */
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CCV",
				  "Call to \"SListPush/SListPop\" failed"))
		return;

	SListPush(nextSyl, SListPop((*syl), error), error); /* C */
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CCV",
				  "Call to \"SListPush/SListPop\" failed"))
		return;

	SListPush(nextSyl, SListPop((*syl), error), error); /* C */
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CCV",
				  "Call to \"SListPush/SListPop\" failed"))
		return;

	SListReverse(nextSyl, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CCV",
				  "Call to \"SListReverse\" failed"))
		return;

	SListPush(syllables, S_OBJECT(nextSyl), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CCV",
				  "Call to \"SListPush\" failed"))
		return;

	(*syl) = nextSyl;
}


static void pop_CCCV(SList *syllables, SList **syl, s_erc *error)
{
	SList *nextSyl;

	S_CLR_ERR(error);

	/*
	 * pop CCCV and put it back into the phonelist
	 * create a new syl and put into syllables.
	 */
	nextSyl = S_LIST(S_NEW(SListList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CCV",
				  "Failed to create new 'SList' object"))
		return;

	SListPush(nextSyl, SListPop((*syl), error), error); /* V */
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CCV",
				  "Call to \"SListPush/SListPop\" failed"))
		return;

	SListPush(nextSyl, SListPop((*syl), error), error); /* C */
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CCV",
				  "Call to \"SListPush/SListPop\" failed"))
		return;

	SListPush(nextSyl, SListPop((*syl), error), error); /* C */
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CCV",
				  "Call to \"SListPush/SListPop\" failed"))
		return;

	SListPush(nextSyl, SListPop((*syl), error), error); /* C */
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CCV",
				  "Call to \"SListPush/SListPop\" failed"))
		return;

	SListReverse(nextSyl, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CCV",
				  "Call to \"SListReverse\" failed"))
		return;

	SListPush(syllables, S_OBJECT(nextSyl), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "pop_CCV",
				  "Call to \"SListPush\" failed"))
		return;

	(*syl) = nextSyl;
}


static void process_VCV(SList *syllables, SList **syl,
						char *current_cluster, s_erc *error)
{
	S_CLR_ERR(error);

	/* always split V.CV */
	pop_CV(syllables, syl, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCV",
				  "Call to \"pop_CV\" failed"))
		return;

	/* fix current_cluster */
	s_strzcpy(current_cluster, "CV", 2, error);
	S_CHK_ERR(error, S_CONTERR,
			  "process_VCV",
			  "Call to \"s_strzcpy\" failed");
}


static void process_VCCV(const SPhoneset *phoneset, SList *syllables,
						 SList **syl, char *current_cluster, s_erc *error)
{
	const SObject *C1;
	const SObject *C2;
	const char *c1_string;
	const char *c2_string;
	s_bool is_permissible_cluster;
	uint8 c1_level;
	uint8 c2_level;

	S_CLR_ERR(error);

	C1 = SListNth((*syl), SListSize((*syl), error) - 3, error); /* third from last */
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCV",
				  "Call to \"SListNth/SListSize\" failed"))
		return;

	c1_string = SObjectGetString(C1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCV",
				  "Call to \"SObjectGetString\" failed"))
		return;

	C2 = SListNth((*syl), SListSize((*syl), error) - 2, error); /* second from last */
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCV",
				  "Call to \"SListNth/SListSize\" failed"))
		return;

	c2_string = SObjectGetString(C2, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCV",
				  "Call to \"SObjectGetString\" failed"))
		return;

	is_permissible_cluster = permissible_consonant_clusters(phoneset, c1_string, c2_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCV",
				  "Call to \"permissible_consonant_clusters\" failed"))
		return;

	c1_level = phone_sonority_level(phoneset, c1_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCV",
				  "Call to \"phone_sonority_level\" failed"))
		return;

	c2_level = phone_sonority_level(phoneset, c2_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCV",
				  "Call to \"phone_sonority_level\" failed"))
		return;

	/*
	 * Tautosyllabic CC clusters  VCCV -> V.CCV
	 * -------------------------
	 * (a) the CC exhibits a sonority rise (going from left to right) and the
	 *     CC is a permissible word-initial cluster of English, or
	 * (b) the CC consists of [s] + voiceless stop.
	 */

	/* case (i) */
	{
		if ((is_permissible_cluster == TRUE)
			&& (c1_level < c2_level))
		{
			pop_CCV(syllables, syl, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_VCCV",
						  "Call to \"pop_CCV\" failed"))
				return;

			s_strzcpy(current_cluster, "CCV", 3, error);
			S_CHK_ERR(error, S_CONTERR,
					  "process_VCCV",
					  "Call to \"s_strzcpy\" failed");
			return;
		}
	}

	/* case (ii) */
	{
		int scomp;
		s_bool c2_is_voiced;
		s_bool c2_is_stop;


		scomp = s_strcmp(c1_string, "s", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_VCCV",
					  "Call to \"s_strcmp\" failed"))
			return;

		c2_is_voiced = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, c2_string,
																	"voiced",
																	error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_VCCV",
					  "Call to SPhoneset method \"phone_has_feature\" failed"))
			return;

		c2_is_stop = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, c2_string,
																  "manner_plosive",
																  error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_VCCV",
					  "Call to SPhoneset method \"phone_has_feature\" failed"))
			return;

		if ((scomp == 0)
			&& (!c2_is_voiced)
			&& (c2_is_stop))
		{
			pop_CCV(syllables, syl, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_VCCV",
						  "Call to \"pop_CCV\" failed"))
				return;

			s_strzcpy(current_cluster, "CCV", 3, error);
			S_CHK_ERR(error, S_CONTERR,
					  "process_VCCV",
					  "Call to \"s_strzcpy\" failed");
			return;
		}
	}

	/*
	 * Heterosyllabic CC clusters  VCCV -> VC.CV
	 * --------------------------
	 * There are three types of heterosyllabic CC sequences:
	 * (a) CC clusters in which the second C is less sonorous than the first (e.g.
	 *     /lk/ in bulky)
	 * (b) CC clusters in which the second C is more sonorous than the first and
	 *     which are not well-formed onsets of English e.g. /tl/ in atlas)
	 * (c) CC clusters in which the two C's are equally sonorous (e.g. /pk/ in
	 *     napkin)
	 */

	/* case (a) */
	{
		if (c1_level > c2_level)
		{
			pop_CV(syllables, syl, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_VCCV",
						  "Call to \"pop_CV\" failed"))
				return;

			s_strzcpy(current_cluster, "CV", 2, error);
			S_CHK_ERR(error, S_CONTERR,
					  "process_VCCV",
					  "Call to \"s_strzcpy\" failed");
			return;
		}
	}

	/* case (b) */
	{
		if ((is_permissible_cluster == FALSE)
			&& (c1_level < c2_level))
		{
			pop_CV(syllables, syl, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_VCCV",
						  "Call to \"pop_CV\" failed"))
				return;

			s_strzcpy(current_cluster, "CV", 2, error);
			S_CHK_ERR(error, S_CONTERR,
					  "process_VCCV",
					  "Call to \"s_strzcpy\" failed");
			return;
		}
	}

	/* case (c) */
	{
		if (c1_level == c2_level)
		{
			pop_CV(syllables, syl, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_VCCV",
						  "Call to \"pop_CV\" failed"))
				return;

			s_strzcpy(current_cluster, "CV", 2, error);
			S_CHK_ERR(error, S_CONTERR,
					  "process_VCCV",
					  "Call to \"s_strzcpy\" failed");
			return;
		}
	}
}


static void process_VCCCV(const SPhoneset *phoneset, SList *syllables, SList **syl,
						  char *current_cluster, s_erc *error)
{
	const SObject *C1;
	const SObject *C2;
	const SObject *C3;
	const char *c1_string;
	const char *c2_string;
	const char *c3_string;
	s_bool c1_is_obstrudent;
	s_bool c2_is_obstrudent;
	s_bool c3_is_obstrudent;
	s_bool is_permissible_cluster;


	S_CLR_ERR(error);

	C1 = SListNth((*syl), SListSize((*syl), error) - 4, error); /* fourth from last */
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCV",
				  "Call to \"SListNth/SListSize\" failed"))
		return;

	c1_string = SObjectGetString(C1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCV",
				  "Call to \"SObjectGetString\" failed"))
		return;

	C2 = SListNth((*syl), SListSize((*syl), error) - 3, error); /* third from last */
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCV",
				  "Call to \"SListNth/SListSize\" failed"))
		return;

	c2_string = SObjectGetString(C2, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCV",
				  "Call to \"SObjectGetString\" failed"))
		return;

	C3 = SListNth((*syl), SListSize((*syl), error) - 2, error); /* second from last */
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCV",
				  "Call to \"SListNth/SListSize\" failed"))
		return;

	c3_string = SObjectGetString(C3, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCV",
				  "Call to \"SObjectGetString\" failed"))
		return;

	c1_is_obstrudent = phone_is_obstrudent(phoneset, c1_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCV",
				  "Call to \"phone_is_obstrudent\" failed"))
		return;

	c2_is_obstrudent = phone_is_obstrudent(phoneset, c2_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCV",
				  "Call to \"phone_is_obstrudent\" failed"))
		return;

	c3_is_obstrudent = phone_is_obstrudent(phoneset, c3_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCV",
				  "Call to \"phone_is_obstrudent\" failed"))
		return;

	/*
	 * CCC sequences in which all three segments are obstruents  VCCCV -> VC.CCV
	 */
	if (c1_is_obstrudent && c2_is_obstrudent && c3_is_obstrudent)
	{
		pop_CCV(syllables, syl, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_VCCCV",
					  "Call to \"pop_CCV\" failed"))
			return;

		s_strzcpy(current_cluster, "CCV", 3, error);
		S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCV",
				  "Call to \"s_strzcpy\" failed");
		return;
	}

	is_permissible_cluster = permissible_consonant_clusters(phoneset, c2_string, c3_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCV",
				  "Call to \"permissible_consonant_clusters\" failed"))
		return;

	if (is_permissible_cluster)
	{
		s_bool c1_is_nasal;
		s_bool c1_is_liquid;


		c1_is_nasal = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, c1_string,
																   "manner_nasal",
																   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_VCCCV",
					  "Call to SPhoneset method \"phone_has_feature\" failed"))
			return;

		c1_is_liquid = phone_is_liquid(phoneset, c1_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_VCCCV",
					  "Call to \"phone_is_liquid\" failed"))
			return;

		if (c1_is_nasal || c1_is_liquid)
		{
			uint8 c2_level;
			uint8 c3_level;
			s_bool c3_is_plosive;
			int scomp;


			c2_level = phone_sonority_level(phoneset, c2_string, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_VCCCV",
						  "Call to \"phone_sonority_level\" failed"))
				return;

			c3_level = phone_sonority_level(phoneset, c3_string, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_VCCCV",
						  "Call to \"phone_sonority_level\" failed"))
				return;


			if (c2_level < c3_level)
			{
				/*
				 * VCCCV -> VC.CCV
				 * CCC clusters in which C2 and C3 are well-formed onsets of English
				 * and in which C2 and C3 rise in sonority (from left to right)
				 * These words can have either a nasal or a liquid as the
				 * first C.
				 */
				pop_CCV(syllables, syl, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "process_VCCCV",
							  "Call to \"pop_CCV\" failed"))
					return;

				s_strzcpy(current_cluster, "CCV", 3, error);
				S_CHK_ERR(error, S_CONTERR,
						  "process_VCCCV",
						  "Call to \"s_strzcpy\" failed");
				return;
			}


			c3_is_plosive = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, c3_string,
																		 "manner_plosive",
																		 error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_VCCCV",
						  "Call to SPhoneset method \"phone_has_feature\" failed"))
				return;

			scomp = s_strcmp(c2_string, "s", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_VCCCV",
						  "Call to \"s_strcmp\" failed"))
				return;


			/* ABY: hack, stop with a plosive */
			if ((scomp == 0) && c3_is_plosive)
			{
				pop_CCV(syllables, syl, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "process_VCCCV",
							  "Call to \"pop_CCV\" failed"))
					return;

				s_strzcpy(current_cluster, "CCV", 3, error);
				S_CHK_ERR(error, S_CONTERR,
						  "process_VCCCV",
						  "Call to \"s_strzcpy\" failed");
				return;
			}
		}

		return;
	}
	else
	{
		uint8 c2_level;
		uint8 c3_level;


		/*
		 * VCCCV -> VCC.CV
		 * (a) C1 C2 C3 clusters in which C2 and C3 are not well-formed onsets of English
		 *     and in which C2 and C3 rise in sonority (<) (from left to right).
		 *
		 * (b) C2 and C3 exhibit a sonority plateau (=) and are not well-formed onsets
		 *     of English.
		 */

		c2_level = phone_sonority_level(phoneset, c2_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_VCCCV",
					  "Call to \"phone_sonority_level\" failed"))
			return;

		c3_level = phone_sonority_level(phoneset, c3_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_VCCCV",
					  "Call to \"phone_sonority_level\" failed"))
			return;

		if (c2_level <= c3_level)
		{
			pop_CV(syllables, syl, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_VCCCV",
						  "Call to \"pop_CV\" failed"))
				return;

			s_strzcpy(current_cluster, "CV", 2, error);
			S_CHK_ERR(error, S_CONTERR,
					  "process_VCCCV",
					  "Call to \"s_strzcpy\" failed");
			return;
		}
	}

	return;
}


static void process_VCCCCV(SList *syllables, SList **syl,
						   char *current_cluster, s_erc *error)
{
	/*
	 * Tautomorphemic clusters
	 * -----------------------
	 * Always VCCCCV -> VC.CCCV
	 */
	S_CLR_ERR(error);

	pop_CCCV(syllables, syl, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCCV",
				  "Call to \"pop_CCCV\" failed"))
		return;

	s_strzcpy(current_cluster, "CCCV", 4, error);
	S_CHK_ERR(error, S_CONTERR,
			  "process_VCCCCV",
			  "Call to \"s_strzcpy\" failed");
}


static void process_VCGV(const SPhoneset *phoneset, SList *syllables, SList **syl,
						 char *current_cluster, s_erc *error)
{
	const SObject *C;
	const SObject *G;
	const char *c_string;
	const char *g_string;
	s_bool c_is_stop;
	s_bool is_permissible_cluster;


	S_CLR_ERR(error);

	C = SListNth((*syl), SListSize((*syl), error) - 3, error); /* third from last */
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCGV",
				  "Call to \"SListNth/SListSize\" failed"))
		return;

	c_string = SObjectGetString(C, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCGV",
				  "Call to \"SObjectGetString\" failed"))
		return;

	G = SListNth((*syl), SListSize((*syl), error) - 2, error); /* second from last */
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCGV",
				  "Call to \"SListNth/SListSize\" failed"))
		return;

	g_string = SObjectGetString(G, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCGV",
				  "Call to \"SObjectGetString\" failed"))
		return;

	is_permissible_cluster = permissible_consonant_clusters(phoneset, c_string, g_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCGV",
				  "Call to \"permissible_consonant_clusters\" failed"))
		return;

	c_is_stop = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, c_string,
															 "manner_plosive",
															 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCGV",
				  "Call to SPhoneset method \"phone_has_feature\" failed"))
		return;

	/*
	 * VCGV -> V.CGV
	 * (a) VCGV sequences in which C is a stop
	 *     where CG is a well-formed onset.
	 */
	if (c_is_stop && is_permissible_cluster)
	{
		/* same as pop_CCV */
		pop_CCV(syllables, syl, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_VCGV",
					  "Call to \"pop_CCV\" failed"))
			return;

		/* fix current_cluster */
		s_strzcpy(current_cluster, "CGV", 3, error);
		S_CHK_ERR(error, S_CONTERR,
				  "process_VCGV",
				  "Call to \"s_strzcpy\" failed");
		return;
	}

	/*
	 * VCGV -> VC.GV
	 * (b) VCGV sequences in which C not a stop
	 *     where CG is not a well-formed onset.
	 *
	 * (c) VCGV sequences in which C not a stop
	 *     where CG is a well-formed onset.
	 */
	if (!c_is_stop)
	{
		/* same as pop_CV */
		pop_CV(syllables, syl, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_VCGV",
					  "Call to \"pop_CV\" failed"))
			return;

		/* fix current_cluster */
		s_strzcpy(current_cluster, "GV", 2, error);
		S_CHK_ERR(error, S_CONTERR,
				  "process_VCGV",
				  "Call to \"s_strzcpy\" failed");
		return;
	}
}


static void process_VCCGV(SList *syllables, SList **syl,
						  char *current_cluster, s_erc *error)
{
	const SObject *C;
	const char *c_string;
	int scomp;


	S_CLR_ERR(error);

	C = SListNth((*syl), SListSize((*syl), error) - 4, error); /* fourth from last */
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCGV",
				  "Call to \"SListNth/SListSize\" failed"))
		return;

	c_string = SObjectGetString(C, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCGV",
				  "Call to \"SObjectGetString\" failed"))
		return;

	scomp = s_strcmp(c_string, "s", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCGV",
				  "Call to \"s_strcmp\" failed"))
		return;

	/* VCCGV in which C is anything other than [s]  VCCGV -> VC.CGV */
	if (scomp != 0)
	{
		/* same as pop_CCV */
		pop_CCV(syllables, syl, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_VCCGV",
					  "Call to \"pop_CCV\" failed"))
			return;

		/* fix current_cluster */
		s_strzcpy(current_cluster, "CGV", 3, error);
		S_CHK_ERR(error, S_CONTERR,
				  "process_VCCGV",
				  "Call to \"s_strzcpy\" failed");
	}
	else
	{
		/* VCCGV in which C is [s]  VCCGV -> V.CCGV */

		/* same as pop_CCCV */
		pop_CCCV(syllables, syl, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_VCCGV",
					  "Call to \"pop_CCCV\" failed"))
			return;

		/* fix current_cluster */
		s_strzcpy(current_cluster, "CCGV", 4, error);
		S_CHK_ERR(error, S_CONTERR,
				  "process_VCCGV",
				  "Call to \"s_strzcpy\" failed");
	}
}


static void process_VCCCGV(SList *syllables, SList **syl,
						   char *current_cluster, s_erc *error)
{
	const SObject *C;
	const char *c_string;
	int scomp;


	S_CLR_ERR(error);

	C = SListNth((*syl), SListSize((*syl), error) - 4, error); /* fourth from last */
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCGV",
				  "Call to \"SListNth/SListSize\" failed"))
		return;

	c_string = SObjectGetString(C, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCGV",
				  "Call to \"SObjectGetString\" failed"))
		return;

	scomp = s_strcmp(c_string, "s", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCGV",
				  "Call to \"s_strcmp\" failed"))
		return;

	/* VCCCGV -> VC.CCGV */
	if (scomp == 0) /* == "s" */
	{
		/* same as pop_CCCV */
		pop_CCCV(syllables, syl, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_VCCCGV",
					  "Call to \"pop_CCCV\" failed"))
			return;

		/* fix current_cluster */
		s_strzcpy(current_cluster, "CCGV", 4, error);
		S_CHK_ERR(error, S_CONTERR,
				  "process_VCCCGV",
				  "Call to \"s_strzcpy\" failed");
	}
}


static void process_VV(SList *syllables, SList **syl,
					   char *current_cluster, s_erc *error)
{
	/* VV -> V.V */
	SList *nextSyl;


	S_CLR_ERR(error);
	nextSyl = S_LIST(S_NEW(SListList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VV",
				  "Failed to create new 'SListList' object"))
		return;

	SListPush(nextSyl, SListPop((*syl), error), error); /* V */
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VV",
				  "Call to \"SListPush/SListPop\" failed"))
	{
		S_DELETE(nextSyl, "process_VV", error);
		return;
	}

	SListPush(syllables, S_OBJECT(nextSyl), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_VV",
				  "Call to \"SListPush\" failed"))
	{
		S_DELETE(nextSyl, "process_VV", error);
		return;
	}

	(*syl) = nextSyl;

	/* fix current_cluster */
	s_strzcpy(current_cluster, "V", 1, error);
	S_CHK_ERR(error, S_CONTERR,
			  "process_VV",
			  "Call to \"s_strzcpy\" failed");
}


static s_cluster get_cluster_type(const char *current_cluster, s_erc *error)
{
	char *tmp_str;

	S_CLR_ERR(error);

	tmp_str = s_strstr(current_cluster, "VCV", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_cluster_type",
				  "Call to \"s_strstr\" failed"))
		return S_UNKNOWN_CLUSTER;

	if (tmp_str != NULL)
		return S_VCV_CLUSTER;

	tmp_str = s_strstr(current_cluster, "VCCV", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_cluster_type",
				  "Call to \"s_strstr\" failed"))
		return S_UNKNOWN_CLUSTER;

	if (tmp_str != NULL)
		return S_VCCV_CLUSTER;

	tmp_str = s_strstr(current_cluster, "VCCCV", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_cluster_type",
				  "Call to \"s_strstr\" failed"))
		return S_UNKNOWN_CLUSTER;

	if (tmp_str != NULL)
		return S_VCCCV_CLUSTER;

	tmp_str = s_strstr(current_cluster, "VCCCCV", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_cluster_type",
				  "Call to \"s_strstr\" failed"))
		return S_UNKNOWN_CLUSTER;

	if (tmp_str != NULL)
		return S_VCCCCV_CLUSTER;

	tmp_str = s_strstr(current_cluster, "VCGV", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_cluster_type",
				  "Call to \"s_strstr\" failed"))
		return S_UNKNOWN_CLUSTER;

	if (tmp_str != NULL)
		return S_VCGV_CLUSTER;

	tmp_str = s_strstr(current_cluster, "VCCGV", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_cluster_type",
				  "Call to \"s_strstr\" failed"))
		return S_UNKNOWN_CLUSTER;

	if (tmp_str != NULL)
		return S_VCCGV_CLUSTER;

	tmp_str = s_strstr(current_cluster, "VCCCGV", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_cluster_type",
				  "Call to \"s_strstr\" failed"))
		return S_UNKNOWN_CLUSTER;

	if (tmp_str != NULL)
		return S_VCCCGV_CLUSTER;

	tmp_str = s_strstr(current_cluster, "VV", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_cluster_type",
				  "Call to \"s_strstr\" failed"))
		return S_UNKNOWN_CLUSTER;

	if (tmp_str != NULL)
		return S_VV_CLUSTER;

	return S_UNKNOWN_CLUSTER;
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
	char current_cluster[100] = "\0";
	s_bool prev_is_obstrudent = FALSE;
	s_bool prev_is_nasal = FALSE;
	size_t list_size;
	s_bool testrv;
	s_cluster cluster_type;


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
	syllables = S_LIST(S_NEW(SListList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Failed to create new 'SList' object"))
		goto quit_error;

	/* create syllable */
	syl = S_LIST(S_NEW(SListList, error));
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


	/* memset(current_cluster, 0, 100); */

	list_size = SListSize(phoneListCopy, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Call to \"SListSize\" failed"))
		goto quit_error;

	while (list_size > 0)
	{
		/* pop phone */
		tmp = SListPop(phoneListCopy, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"SListPop\" failed"))
			goto quit_error;

		phone_string = SObjectGetString(tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"SObjectGetString\" failed"))
			goto quit_error;

		/*
		 * add the phone to the syllable for now, we add the
		 * phone_string so that it is independent from the phoneList
		 */
		SListPush(syl, SObjectSetString(phone_string, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"SListPush/SObjectSetString\" failed"))
			goto quit_error;

		/*
		 * C for a consonant, V represents a syllable peak (i.e. short
		 * vowel, syllabic sonorant, long vowel, diphthong), G represents glide.
		 */

		/*
		 * ABY: hacks for syllabic consonants
		 * for bottle, button etc.
		 */
		testrv = phone_is_syllabic_consonant(phone_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"phone_is_syllabic_consonant\" failed"))
			goto quit_error;

		list_size = SListSize(phoneListCopy, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"SListSize\" failed"))
			goto quit_error;

		if (prev_is_obstrudent
		    && testrv
		    && (list_size == 0))
		{
			s_strcat(current_cluster, "V", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"s_strcat\" failed"))
				goto quit_error;
		}

		if (prev_is_nasal
		    && testrv
		    && (list_size == 0))
		{
			s_strcat(current_cluster, "V", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"s_strcat\" failed"))
				goto quit_error;
		}

		prev_is_obstrudent = phone_is_obstrudent(phoneset, phone_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"phone_is_obstrudent\" failed"))
			goto quit_error;

		prev_is_nasal = S_PHONESET_CALL(phoneset,
										phone_has_feature)(phoneset, phone_string,
														   "manner_nasal", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"phone_has_feature\" failed"))
			goto quit_error;

		/*
		 * hacks end
		 */
		testrv = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone_string,
															  "vowel", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"phone_has_feature\" failed"))
			goto quit_error;

		if (testrv)
		{
			s_strcat(current_cluster, "V", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"s_strcat\" failed"))
				goto quit_error;
		}
		else
		{
			testrv = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone_string,
																  "manner_glide", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"phone_has_feature\" failed"))
				goto quit_error;

			if (testrv)
			{
				s_strcat(current_cluster, "G", error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"s_strcat\" failed"))
					goto quit_error;
			}
			else
			{
				s_strcat(current_cluster, "C", error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"s_strcat\" failed"))
					goto quit_error;
			}
		}

		cluster_type = get_cluster_type(current_cluster, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"get_cluster_type\" failed"))
			goto quit_error;

		switch (cluster_type)
		{
		case S_VCV_CLUSTER:
		{
			process_VCV(syllables, &syl, current_cluster, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"process_VCV\" failed"))
				goto quit_error;

			break;
		}
		case S_VCCV_CLUSTER:
		{
			process_VCCV(phoneset, syllables, &syl, current_cluster, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"process_VCCV\" failed"))
				goto quit_error;

			break;
		}
		case S_VCCCV_CLUSTER:
		{
			process_VCCCV(phoneset, syllables, &syl, current_cluster, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"process_VCCCV\" failed"))
				goto quit_error;

			break;
		}
		case S_VCCCCV_CLUSTER:
		{
			process_VCCCCV(syllables, &syl, current_cluster, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"process_VCCCCV\" failed"))
				goto quit_error;

			break;
		}
		case S_VCGV_CLUSTER:
		{
			process_VCGV(phoneset, syllables, &syl, current_cluster, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"process_VCGV\" failed"))
				goto quit_error;

			break;
		}
		case S_VCCGV_CLUSTER:
		{
			process_VCCGV(syllables, &syl, current_cluster, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"process_VCCGV\" failed"))
				goto quit_error;

			break;
		}
		case S_VCCCGV_CLUSTER:
		{
			process_VCCCGV(syllables, &syl, current_cluster, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"process_VCCCGV\" failed"))
				goto quit_error;

			break;
		}
		case S_VV_CLUSTER:
		{
			process_VV(syllables, &syl, current_cluster, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"process_VV\" failed"))
				goto quit_error;

			break;
		}
		case S_UNKNOWN_CLUSTER:
		{
			break;
		}
		}

		list_size = SListSize(phoneListCopy, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"SListSize\" failed"))
			goto quit_error;
	}

	/* check if there are any phones in the last syllable */
	list_size = SListSize(syl, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Call to \"SListSize\" failed"))
		goto quit_error;

	if (list_size == 0)
	{
		/* nothing, so pop and delete */
		SListPop(syllables, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"SListPop\" failed"))
			goto quit_error;

		S_DELETE(syl, "Syllabify", error);
	}

	S_DELETE(phoneListCopy, "Syllabify", error);
	return syllables;

quit_error:
	if (syllables != NULL)
		S_DELETE(syllables, "Syllabify", error);

	if (phoneListCopy != NULL)
		S_DELETE(phoneListCopy, "Syllabify", error);

	return NULL;

	S_UNUSED(self);
}


/************************************************************************************/
/*                                                                                  */
/* SSyllabEngZaLwazi class initialization                                           */
/*                                                                                  */
/************************************************************************************/

static SSyllabEngZaLwaziClass SyllabEngZaLwaziClass =
{
	/* SObjectClass */
	{
		"SSyllabification:SSyllabEngZaLwazi",
		sizeof(SSyllabEngZaLwazi),
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
