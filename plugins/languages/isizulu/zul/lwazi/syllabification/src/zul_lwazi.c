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

/* sonorant + consonant = syllabic consonant */
static s_bool phone_is_syllabic_consonant(const SPhoneset *phoneset, const char *phone,
										  s_erc *error)
{
	s_bool present;


	S_CLR_ERR(error);
	present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
														   "class_consonantal",
														   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_syllabic_consonant",
				  "Call to method \"phone_has_feature\" failed"))
		return FALSE;

	if (!present)
		return FALSE;

	present = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, phone,
														   "class_sonorant",
														   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "phone_is_syllabic_consonant",
				  "Call to method \"phone_has_feature\" failed"))
		return FALSE;

	if (!present)
		return FALSE;

	return TRUE;
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
		s_bool current_is_vowel;


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

		/* test for SC -> S.C, where C is a consonant type */
		if (list_size > 1)
		{
			s_bool sylcon;


			sylcon = phone_is_syllabic_consonant(phoneset, phone_string, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"phone_is_syllabic_consonant\" failed"))
				goto quit_error;

			if (sylcon)
			{
				const char *second_phone_string;
				s_bool second_is_vowel;


				second_phone_string = SObjectGetString(SListNth(phoneListCopy,
																list_size - 1,
																error), error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"SObjectGetString/SListNth\" failed"))
					goto quit_error;

				second_is_vowel = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset,
																			   second_phone_string,
																			   "vowel",
																			   error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to method \"phone_has_feature\" failed"))
					goto quit_error;

				if (!second_is_vowel)
				{
					/* this is a syllable boundary */

					/*
					 * we add the phone_string so that it is
					 * independent from the phoneList
					 */
					SListPush(syl, SObjectSetString(phone_string, error), error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "Syllabify",
								  "Call to \"SListPush/SObjectSetString\" failed"))
						goto quit_error;

					if (list_size > 0)
					{
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
					}

					continue;
				}
			}
		}

		current_is_vowel = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset,
																		phone_string,
																		"vowel",
																		error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to method \"phone_has_feature\" failed"))
			goto quit_error;


		/* test for VCC -> VC.C */
		if ((list_size > 2) && current_is_vowel)
		{
			const char *second_phone_string;
			const char *third_phone_string;
			s_bool second_is_vowel;
			s_bool third_is_vowel;


			second_phone_string = SObjectGetString(SListNth(phoneListCopy,
															list_size - 1,
															error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"SObjectGetString/SListNth\" failed"))
				goto quit_error;

			second_is_vowel = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset,
																		   second_phone_string,
																		   "vowel",
																		   error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to method \"phone_has_feature\" failed"))
				goto quit_error;

			if (!second_is_vowel)
			{
				third_phone_string = SObjectGetString(SListNth(phoneListCopy,
															   list_size - 2,
															   error), error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to \"SObjectGetString/SListNth\" failed"))
					goto quit_error;

				third_is_vowel = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset,
																			  third_phone_string,
																			  "vowel",
																			  error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Syllabify",
							  "Call to method \"phone_has_feature\" failed"))
					goto quit_error;

				if (!third_is_vowel)
				{
					/* pop second, no need to
					 * delete as it still belongs to
					 * original phone list
					 */
					SListPop(phoneListCopy, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "Syllabify",
								  "Call to \"SListPop\" failed"))
						goto quit_error;

					list_size--;  /* popped 1 */


					/* add to syllable, phone_string first */
					SListPush(syl, SObjectSetString(phone_string, error), error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "Syllabify",
								  "Call to \"SListPush/SObjectSetString\" failed"))
						goto quit_error;

					/* add to syllable */
					SListPush(syl, SObjectSetString(second_phone_string, error), error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "Syllabify",
								  "Call to \"SListPush/SObjectSetString\" failed"))
						goto quit_error;

					if (list_size > 0)
					{
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
					}

					continue;
				}
			}
		}

		/* test for VA -> V.A, where A is any type */
		if (current_is_vowel)
		{
			/* add to syllable */
			SListPush(syl, SObjectSetString(phone_string, error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"SListPush/SObjectSetString\" failed"))
				goto quit_error;

			if (list_size > 0)
			{
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
			}

			continue;
		}

		/* add all types except vowels (which should be caught above) to current syllable */
		if (!current_is_vowel)
		{
			/* add to syllable */
			SListPush(syl, SObjectSetString(phone_string, error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"SListPush/SObjectSetString\" failed"))
				goto quit_error;
		}
	}

	S_DELETE(phoneListCopy, "Syllabify", error);
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
	GetName,             /* get_name        */
	GetDescription,      /* get_description */
	GetLanguage,         /* get_language    */
	GetLangCode,         /* get_lang_code   */
	SGetVersion,         /* get_version     */
	GetFeature,          /* get_feature     */
	Syllabify            /* syllabify       */
};
