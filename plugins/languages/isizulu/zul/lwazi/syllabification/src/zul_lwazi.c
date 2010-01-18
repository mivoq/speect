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
/* DATE    : January 2010                                                           */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A South African isiZulu (zul) syllabification class                              */
/* implementation for the Lwazi project.                                            */
/* Very basic isiZulu syllabification, based on the basic syllabification scheme    */
/* devised by Etienne Barnard for isiZulu.                                          */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include "zul_lwazi.h"
#include "phoneset.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSyllabZulLwaziClass SyllabZulLwaziClass; /* SSyllabZulLwazi class declaration. */


/************************************************************************************/
/*                                                                                  */
/*  Static function prototypes                                                      */
/*                                                                                  */
/************************************************************************************/

static s_bool phone_is_syllabic_consonant(const SPhoneset *phoneset, const char *phone,
										  s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_syllab_zul_lwazi_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&SyllabZulLwaziClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syllab_zul_lwazi_class_reg",
			  "Failed to register SSyllabZulLwaziClass");
}


S_LOCAL void _s_syllab_zul_lwazi_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&SyllabZulLwaziClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syllab_zul_lwazi_class_free",
			  "Failed to free SSyllabZulLwaziClass");
}


/************************************************************************************/
/*                                                                                  */
/*  Static function implementations                                                 */
/*                                                                                  */
/************************************************************************************/

static s_bool phone_is_syllabic_consonant(const SPhoneset *phoneset, const char *phone,
										  s_erc *error);



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


/**
 * return a vallist of vallists where the primary list is syllables and the secondary
 * lists are the phones in the syllables. for example :
 * for mathematics phonelist is : (m , ae , th, ax, m, ae, t, ih, k, s)
 * syllfunc returns : ((m, ae), (th, ax), (m, ae), (t, ih, k, s))
 */

static SList *Syllabify(const SItem *word, const SList *phoneList, s_erc *error)
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
	syllables = (SList*)S_NEW("SListList", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Failed to create new 'SList' object"))
		goto quit_error;

	SListListInit(&syllables, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Failed to initialize new 'SList' object"))
		goto quit_error;

	/* create syllable */
	syl = (SList*)S_NEW("SListList", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Failed to create new 'SList' object"))
		goto quit_error;

	SListListInit(&syl, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Failed to initialize new 'SList' object"))
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
}


/************************************************************************************/
/*                                                                                  */
/* SSyllabZulLwazi class initialization                                             */
/*                                                                                  */
/************************************************************************************/

static SSyllabZulLwaziClass SyllabZulLwaziClass =
{
	/* SObjectClass */
	{
		"SSyllabification:SSyllabZulLwazi",
		sizeof(SSyllabZulLwazi),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SSyllabificationClass */
	Syllabify          /* syllabify */
};
