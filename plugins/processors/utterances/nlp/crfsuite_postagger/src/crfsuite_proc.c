/************************************************************************************/
/* Copyright (c) Mivoq SRL                                                          */
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
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
/* DATE    : October 2016                                                           */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A part-of-speech tagger utterance processor using crfsuite.                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "crfsuite_proc.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SCrfSuiteUttProc class declaration. */
static SCrfSuiteUttProcClass CrfSuiteUttProcClass;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static const char* removeDoubles ( const char* string, s_erc *error);

static const char* getFirstChars ( const char* string, int n,  s_erc *error);

static const char* getLastChars (const char* string, int n,  s_erc *error);

static s_bool hasCapital (const char* string,  s_erc *error);

static s_bool hasNumber (const char* string,  s_erc *error);

static s_bool hasSymbol (const char* string,  s_erc *error);

/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_crfsuite_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&CrfSuiteUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
		  "_s_crfsuite_utt_proc_class_reg",
		  "Failed to register SCrfSuiteUttProcClass");
}


S_LOCAL void _s_crfsuite_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&CrfSuiteUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
		  "_s_crfsuite_utt_proc_class_free",
		  "Failed to free SCrfSuiteUttProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static const char* removeDoubles ( const char* string, s_erc *error)
{
	char* result = NULL;
	char tmp[7];
	char tmp2[7];
	int i = 0;

	if (string == NULL)
		return NULL;

	while (string[i] != '\0')
	{
		/*check if next char is the same of the actual char, if so it skips it
		 * and check for the next one ( care for the UTF8 format )
		 */
		s_strncpy (tmp, string +i, 1, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "removeDoubles",
				  "Call to \"s_strncpy\" failed"))
			return NULL;

		s_strncpy (tmp2, string +i, 1, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "removeDoubles",
				  "Call to \"s_strncpy\" failed"))
			return NULL;

		int j = s_width(tmp2, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "removeDoubles",
				  "Call to \"s_width\" failed"))
			return NULL;

		while ( string[i] == string[i+j] )
		{
			s_strncpy (tmp2, string +i, 1, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "removeDoubles",
					  "Call to \"s_strncpy\" failed"))
				return NULL;
			j += s_width(tmp2, error);
		}

		s_sappend (&result, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "removeDoubles",
				  "Call to \"s_sappend\" failed"))
			return NULL;

		i += j;
		/*next char that is not repeated*/
	}

	result = s_strlwr( result, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "removeDoubles",
			  "Call to \"s_strlwr\" failed"))
		return NULL;

	return result;

}

static const char* getFirstChars ( const char* string, int n,  s_erc *error)
{
	char* result = NULL;
	char tmp[7];
	int i = 0, j = 0;

	if (string == NULL)
		return NULL;

	while (string[i] != '\0' && j < n )
	{
		s_strncpy (tmp, string +i, 1, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "getFirstChars",
				  "Call to \"s_strncpy\" failed"))
			return NULL;

		s_sappend (&result, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "getFirstChars",
				  "Call to \"s_sappend\" failed"))
			return NULL;

		j += 1;
		i += s_width(tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "getFirstChars",
				  "Call to \"s_width\" failed"))
			return NULL;
	}

	if ( j == n )
		return result;
	else
		return NULL;
}

static const char* getLastChars (const char* string, int n,  s_erc *error)
{
	char* result = NULL;
	char tmp[7];
	int i = 0;

	int j = s_strlen(string, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "getLastChars",
			  "Call to \"s_strlen\" failed"))
		return NULL;

	if (string == NULL || j < n )
		return NULL;

	while (string[i] != '\0' && j > 0 )
	{
		s_strncpy (tmp, string +i, 1, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "getLastChars",
				  "Call to \"s_strncpy\" failed"))
			return NULL;

		j -= 1;

		if ( j < n )
		{
			s_sappend (&result, tmp, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "getLastChars",
					  "Call to \"s_sappend\" failed"))
				return NULL;
		}

		i += s_width(tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "getLastChars",
				  "Call to \"s_width\" failed"))
			return NULL;
	}

		return result;
}

static s_bool hasCapital (const char* string,  s_erc *error)
{
	s_bool result;

	result = s_smatches ( string, ".*[A-Z]+.*", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "hasCapital",
			  "Call to \"s_smatches\" failed"))
		return FALSE;

	return result;
}

static s_bool hasNumber (const char* string,  s_erc *error)
{
	s_bool result;

	result = s_smatches ( string, ".*[0-9]+.*", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "hasCapital",
			  "Call to \"s_smatches\" failed"))
		return FALSE;

	return result;
}

static s_bool hasSymbol (const char* string,  s_erc *error)
{
	s_bool result;

	result = s_smatches ( string, ".*?[^a-zA-Z0-9].*", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "hasCapital",
			  "Call to \"s_smatches\" failed"))
		return FALSE;

	return result;
}

/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/


static void Destroy(void *obj, s_erc *error)
{
	SCrfSuiteUttProc *self = obj;

	S_CLR_ERR(error);
	S_UNUSED(self);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void Initialize(SUttProcessor *self, const SVoice *voice, s_erc *error)
{
	SCrfSuiteUttProc *crfsuiteProc = (SCrfSuiteUttProc*)self;
	const SObject* filepath;

	S_CLR_ERR(error);

	s_bool isPresent = SUttProcessorFeatureIsPresent (self, "model_file", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to \"SUtteranceFeatureIsPresent\" failed"))
		return;

	if (isPresent)
	{
		const SObject * vcfgObject = SVoiceGetFeature(voice, "config_file", error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SVoiceGetFeature\" failed, failed to get voice config file"))
			return;

		const char * voice_base_path = s_get_base_path(SObjectGetString(vcfgObject, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"s_get_base_path/SObjectGetString\" failed"))
			return;

		filepath = SUttProcessorGetFeature (self, "model_file", error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SUtteranceGetFeature\" failed"))
			return;

		const char * path = SObjectGetString (filepath, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SObjectGetString\" failed"))
			return;

		crfsuiteProc->model_file = s_path_combine(voice_base_path, path, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"s_path_combine\" failed"))
			return;


	}
	else
	{
		S_CTX_ERR(error, S_FAILURE,
			  "Initialize",
			  "Failed to load the CRFSuite model.");
		return;
	}

	return;

	S_UNUSED(voice);

}

static crfsuite_instance_t* create_phrase_instance ( SItem* phrase,
                                                 crfsuite_dictionary_t* attrs,
                                                 crfsuite_dictionary_t* labels,
                                                 s_erc *error)
{
	crfsuite_instance_t * result = malloc ( sizeof(crfsuite_instance_t) );
	int i = 0;
	int L = labels->num(labels);
	const SItem* itrItem = NULL;
	const SItem* itrItemNext = NULL;
	const SItem* finishItem = NULL;

	const char* lbl[] = {"num", "sym", "cap", "p1", "p2", "p3",
							"s1", "s2", "s3", "P1", "P2", "P3", "P4",
							"S1", "S2", "S3", "S4", "S5", "S6", "w" };

	const int words_length = 19;
	const char* words[19] = {			 NULL, NULL, NULL, NULL, NULL,
										NULL, NULL, NULL, NULL, NULL,
										NULL, NULL, NULL, NULL, NULL,
										NULL, NULL, NULL, NULL};
	int position = 0;

	int lbl_counter = 0;
	char buffer[8192];

	crfsuite_instance_init ( result );

	itrItemNext = SItemPathToItem (phrase, "daughter.R:Token.parent", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "create_phrase_instance",
			  "Call to \"SItemPathToItem\" failed"))
		return NULL;

	finishItem = SItemPathToItem (phrase, "n.daughter.R:Token.parent", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "create_phrase_instance",
			  "Call to \"SItemPathToItem\" failed"))
		return NULL;

	int counter = 0;

	while ( itrItemNext != finishItem && itrItemNext != NULL && counter < 9)
	{
		words[counter] = SItemGetName(itrItemNext, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_instance",
				  "Call to \"SItemGetName\" failed"))
			return NULL;

		counter++;

		itrItemNext = SItemNext(itrItemNext, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_instance",
				  "Call to \"SItemNext\" failed"))
			return NULL;
	}

	itrItem = SItemPathToItem (phrase, "daughter.R:Token.parent", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "create_phrase_instance",
			  "Call to \"SItemPathToItem\" failed"))
		return NULL;

	while ( itrItem != finishItem && itrItem != NULL)
	{
		/* Extraction of the features for each token */
		const char *tokenName = SItemGetName (itrItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_instance",
				  "Call to \"SItemGetName\" failed"))
			return NULL;

        lbl_counter = 0;

		/* Extraction of label's ID */
		int tokenID = labels->to_id(labels, "UNK");
		int attribute_id;

		/* If unknown the set the 0 labels (unknown) */
		if (tokenID < 0)
			tokenID = L;

		crfsuite_item_t itemToken;
		crfsuite_attribute_t attribute;

		crfsuite_item_init(&itemToken);

		const char *feat = NULL;
		s_bool found = FALSE;


		/* if token contains numbers */
		found = hasNumber (tokenName, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_instance",
				  "Call to \"hasNumber\" failed"))
			return NULL;

		sprintf(buffer, "%s[%d]=%s", lbl[lbl_counter], 0, found? "Y" : "N");
		lbl_counter++;


		attribute_id = attrs->to_id (attrs,buffer);
		crfsuite_attribute_set (&attribute, attribute_id, 1.0);
		crfsuite_item_append_attribute(&itemToken, &attribute);


		/* if token contains symbols */
		found = hasSymbol (tokenName, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_instance",
				  "Call to \"hasSymbol\" failed"))
			return NULL;

		sprintf(buffer, "%s[%d]=%s", lbl[lbl_counter], 0, found? "Y" : "N");
		lbl_counter++;

		attribute_id = attrs->to_id (attrs,buffer);
		crfsuite_attribute_set (&attribute, attribute_id, 1.0);
		crfsuite_item_append_attribute(&itemToken, &attribute);


		/* if token contains Capitals */
		found = hasCapital (tokenName, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_instance",
				  "Call to \"hasCapital\" failed"))
			return NULL;

		sprintf(buffer, "%s[%d]=%s", lbl[lbl_counter], 0, found? "Y" : "N");
		lbl_counter++;

		attribute_id = attrs->to_id (attrs,buffer);
		crfsuite_attribute_set (&attribute, attribute_id, 1.0);
		crfsuite_item_append_attribute(&itemToken, &attribute);


		/* Prefixes of 1 to 3 chars of token */
		i = 1;
		while ( i < 4 )
		{
			feat = getFirstChars ( tokenName, i, error );
			if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_instance",
					  "Call to \"getFirstChars\" failed"))
				return NULL;

			if ( feat == NULL )
				feat = "__nil__";

			sprintf(buffer, "%s[%d]=%s", lbl[lbl_counter], 0, feat);
			lbl_counter++;

			attribute_id = attrs->to_id (attrs,buffer);
			crfsuite_attribute_set (&attribute, attribute_id, 1.0);
			crfsuite_item_append_attribute(&itemToken, &attribute);

			i += 1;
		}

		/* Suffixes of 1 to 3 chars of token */
		i = 1;
		while ( i < 4 )
		{
			feat = getLastChars ( tokenName, i, error );
			if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_instance",
					  "Call to \"getLastChars\" failed"))
				return NULL;

			if ( feat == NULL )
				feat = "__nil__";

			sprintf(buffer, "%s[%d]=%s", lbl[lbl_counter], 0, feat);
			lbl_counter++;

			attribute_id = attrs->to_id (attrs,buffer);
			crfsuite_attribute_set (&attribute, attribute_id, 1.0);
			crfsuite_item_append_attribute(&itemToken, &attribute);

			i += 1;
		}

		/* Prefixes of 1 to 4 chars of token without duplicates adjacent */
		i = 1;
		while ( i < 5 )
		{
			feat = getFirstChars ( removeDoubles(tokenName, error), i, error );
			if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_instance",
					  "Call to \"removeDoubles\" failed"))
				return NULL;
			if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_instance",
					  "Call to \"getFirstChars\" failed"))
				return NULL;

			if ( feat == NULL )
				feat = "__nil__";

			sprintf(buffer, "%s[%d]=%s", lbl[lbl_counter], 0, feat);
			lbl_counter++;

			attribute_id = attrs->to_id (attrs,buffer);
			crfsuite_attribute_set (&attribute, attribute_id, 1.0);
			crfsuite_item_append_attribute(&itemToken, &attribute);

			i += 1;
		}

		/* Suffixes of 1 to 6 chars of token without duplicates adjacent */
		i = 1;
		while ( i < 7 )
		{
			feat = getLastChars ( removeDoubles(tokenName, error), i, error );
			if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_instance",
					  "Call to \"removeDoubles\" failed"))
				return NULL;
			if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_instance",
					  "Call to \"getFirstChars\" failed"))
				return NULL;

			if ( feat == NULL )
				feat = "__nil__";

			sprintf(buffer, "%s[%d]=%s", lbl[lbl_counter], 0, feat);
			lbl_counter++;

			attribute_id = attrs->to_id (attrs,buffer);
			crfsuite_attribute_set (&attribute, attribute_id, 1.0);
			crfsuite_item_append_attribute(&itemToken, &attribute);

			i += 1;
		}

		/* Words features */

		sprintf(buffer, "%s[%d]=%s", lbl[lbl_counter], 0, words[(position+0+words_length)%words_length]);

		attribute_id = attrs->to_id (attrs,buffer);
		crfsuite_attribute_set (&attribute, attribute_id, 1.0);
		crfsuite_item_append_attribute(&itemToken, &attribute);

		const char *tmp = words[(position-1+words_length)%words_length];
		if(tmp!=NULL)
		{
			sprintf(buffer, "%s[%d]=%s", lbl[lbl_counter], -1, tmp);

			attribute_id = attrs->to_id (attrs,buffer);
			crfsuite_attribute_set (&attribute, attribute_id, 1.0);
			crfsuite_item_append_attribute(&itemToken, &attribute);
		}

		tmp = words[(position+1+words_length)%words_length];
		if(tmp!=NULL)
		{
			sprintf(buffer, "%s[%d]=%s", lbl[lbl_counter], 1, tmp);

			attribute_id = attrs->to_id (attrs,buffer);
			crfsuite_attribute_set (&attribute, attribute_id, 1.0);
			crfsuite_item_append_attribute(&itemToken, &attribute);
		}

		tmp = words[(position-2+words_length)%words_length];
		if(tmp!=NULL)
		{
			sprintf(buffer, "%s[%d]=%s", lbl[lbl_counter], -2, tmp);

			attribute_id = attrs->to_id (attrs,buffer);
			crfsuite_attribute_set (&attribute, attribute_id, 1.0);
			crfsuite_item_append_attribute(&itemToken, &attribute);
		}

		tmp = words[(position+2+words_length)%words_length];
		if(tmp!=NULL)
		{
			sprintf(buffer, "%s[%d]=%s", lbl[lbl_counter], 2, tmp);

			attribute_id = attrs->to_id (attrs,buffer);
			crfsuite_attribute_set (&attribute, attribute_id, 1.0);
			crfsuite_item_append_attribute(&itemToken, &attribute);
		}

		const char * tmp1 = NULL;
		i = 0;
		while ( i < 4 )
		{
			tmp = words[(position-2+i+words_length)%words_length];
			tmp1 = words[(position-1+i+words_length)%words_length];
			if( tmp!=NULL && tmp1 != NULL )
			{
				sprintf(buffer, "%s[%d]|%s[%d]=%s|%s", lbl[lbl_counter], -2+i,
													   lbl[lbl_counter], -1+i,
													   tmp, tmp1 );

				attribute_id = attrs->to_id (attrs,buffer);
				crfsuite_attribute_set (&attribute, attribute_id, 1.0);
				crfsuite_item_append_attribute(&itemToken, &attribute);
			}
			i++;
		}

		const char * tmp2 = NULL;
		i = 0;
		while ( i < 3 )
		{
			tmp = words[(position-2+i+words_length)%words_length];
			tmp1 = words[(position-1+i+words_length)%words_length];
			tmp2 = words[(position+0+i+words_length)%words_length];
			if( tmp!=NULL && tmp1 != NULL && tmp2 != NULL )
			{
				sprintf(buffer, "%s[%d]|%s[%d]|%s[%d]=%s|%s|%s",
													   lbl[lbl_counter], -2+i,
													   lbl[lbl_counter], -1+i,
													   lbl[lbl_counter], +0+i,
													   tmp, tmp1, tmp2 );

				attribute_id = attrs->to_id (attrs,buffer);
				crfsuite_attribute_set (&attribute, attribute_id, 1.0);
				crfsuite_item_append_attribute(&itemToken, &attribute);
			}
			i++;
		}

		const char * tmp3 = NULL;
		i = 0;
		while ( i < 2 )
		{
			tmp  = words[(position-2+i+words_length)%words_length];
			tmp1 = words[(position-1+i+words_length)%words_length];
			tmp2 = words[(position+0+i+words_length)%words_length];
			tmp3 = words[(position+1+i+words_length)%words_length];
			if( tmp!=NULL && tmp1 != NULL && tmp2 != NULL && tmp3 != NULL )
			{
				sprintf(buffer, "%s[%d]|%s[%d]|%s[%d]|%s[%d]=%s|%s|%s|%s",
													   lbl[lbl_counter], -2+i,
													   lbl[lbl_counter], -1+i,
													   lbl[lbl_counter], +0+i,
													   lbl[lbl_counter], +1+i,
													   tmp, tmp1, tmp2, tmp3 );

				attribute_id = attrs->to_id (attrs,buffer);
				crfsuite_attribute_set (&attribute, attribute_id, 1.0);
				crfsuite_item_append_attribute(&itemToken, &attribute);
			}
			i++;
		}

		tmp  = words[(position-2+words_length)%words_length];
		tmp1 = words[(position-1+words_length)%words_length];
		tmp2 = words[(position+0+words_length)%words_length];
		tmp3 = words[(position+1+words_length)%words_length];
		const char * tmp4 = words[(position+2+words_length)%words_length];
		if( tmp!=NULL && tmp1 != NULL && tmp2 != NULL && tmp3 != NULL && tmp4 != NULL )
		{
			sprintf(buffer, "%s[%d]|%s[%d]|%s[%d]|%s[%d]|%s[%d]=%s|%s|%s|%s|%s",
												   lbl[lbl_counter], -2,
												   lbl[lbl_counter], -1,
												   lbl[lbl_counter], +0,
												   lbl[lbl_counter], +1,
												   lbl[lbl_counter], +2,
												   tmp, tmp1, tmp2, tmp3, tmp4 );

			attribute_id = attrs->to_id (attrs,buffer);
			crfsuite_attribute_set (&attribute, attribute_id, 1.0);
			crfsuite_item_append_attribute(&itemToken, &attribute);
		}

		tmp  = words[(position+0+words_length)%words_length];

		i = 1;
		while ( i < 10 )
		{
			tmp1  = words[(position-i+words_length)%words_length];
			if ( tmp1 != NULL )
			{
				sprintf(buffer, "%s[%d]|%s[%d]=%s|%s", lbl[lbl_counter], 0,
													   lbl[lbl_counter], 0-i,
													   tmp, tmp1 );

				attribute_id = attrs->to_id (attrs,buffer);
				crfsuite_attribute_set (&attribute, attribute_id, 1.0);
				crfsuite_item_append_attribute(&itemToken, &attribute);
			}
			i++;
		}

		i = 1;
		while ( i < 10 )
		{
			tmp1  = words[(position+i+words_length)%words_length];
			if ( tmp1 != NULL )
			{
				sprintf(buffer, "%s[%d]|%s[%d]=%s|%s", lbl[lbl_counter], 0,
													   lbl[lbl_counter], 0+i,
													   tmp, tmp1 );

				attribute_id = attrs->to_id (attrs,buffer);
				crfsuite_attribute_set (&attribute, attribute_id, 1.0);
				crfsuite_item_append_attribute(&itemToken, &attribute);
			}
			i++;
		}




		/* Update words array */
		position++;
		if ( itrItemNext != NULL )
		{
			words[position+9] = SItemGetName (itrItemNext, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_instance",
					  "Call to \"SItemNext\" failed"))
				return NULL;

			itrItemNext = SItemNext ( itrItemNext, error );
			if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_instance",
					  "Call to \"SItemNext\" failed"))
				return NULL;
		}
		else
			words[position+9] = NULL;

		itrItem = SItemNext ( itrItem, error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_instance",
				  "Call to \"SItemNext\" failed"))
			return NULL;

		crfsuite_instance_append(result, &itemToken, tokenID);
		crfsuite_item_finish(&itemToken);
	}

	return result;

}


static void Run(const SUttProcessor *self, SUtterance *utt,
		s_erc *error)
{
	SCrfSuiteUttProc *crfsuiteProc = (SCrfSuiteUttProc*)self;

	crfsuite_model_t * ptr_model = malloc (sizeof(crfsuite_tagger_t));
	crfsuite_tagger_t * ptr_tagger = malloc (sizeof(crfsuite_tagger_t));
	crfsuite_dictionary_t * ptr_attrs = malloc (sizeof(crfsuite_dictionary_t));
	crfsuite_dictionary_t * ptr_labels = malloc (sizeof(crfsuite_dictionary_t));
	crfsuite_instance_t * instance = malloc (sizeof(crfsuite_instance_t));

	/* Initialize model object */
	if ( crfsuite_create_instance_from_file( crfsuiteProc->model_file,
	                                              (void**)&ptr_model
	                                            ) != 0 ) {
		goto exit_cleanup;
	}

	const SRelation* phrase = SUtteranceGetRelation(utt, "Phrase", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "Run",
			  "Call to \"SUtteranceGetRelation\" failed"))
		return;

	SItem* itrPhrase = SRelationHead( phrase, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "Run",
			  "Call to \"SRelationHead\" failed"))
		return;

	while ( itrPhrase != NULL )
	{
		/* Obtain the dictionary interface representing the labels in the model. */
		if ( ptr_model->get_labels(ptr_model, &ptr_labels) != 0) {
			goto exit_cleanup;
		}

		/* Obtain the dictionary interface representing the attributes in the model. */
		if ( ptr_model->get_attrs(ptr_model, &ptr_attrs) != 0) {
			goto exit_cleanup;
		}

		/* Obtain the tagger interface. */
		if ( ptr_model->get_tagger(ptr_model, &ptr_tagger) != 0) {
			goto exit_cleanup;
		}

		instance = create_phrase_instance ( itrPhrase, ptr_attrs, ptr_labels, error );
		int *output = calloc(sizeof(int), instance->num_items);
		floatval_t score = 0;

		/* Set the instance to the tagger. */
		if ( ptr_tagger->set(ptr_tagger, instance) != 0) {
			goto exit_cleanup;
		}

		/* Obtain the viterbi label sequence. */
		if (ptr_tagger->viterbi(ptr_tagger, output, &score) != 0) {
			goto exit_cleanup;
		}

		/* Extract the output and insert in the POS attribute */

		const SItem* tokenTMP = SItemPathToItem ( itrPhrase, "daughter.R:Token", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPathToItem\" failed"))
			return;

		SItem* token = SItemParent (tokenTMP, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemParent\" failed"))
			return;

		const SItem* lastToken = SItemPathToItem ( itrPhrase, "n.daughter.R:Token.parent", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPathToItem\" failed"))
			return;

		int i = 0;

		while ( token != NULL && token != lastToken )
		{
			const char * str = malloc (sizeof (char)*16);
			ptr_labels->to_string (ptr_labels, output[i], &str);

			i += 1;

			SItemSetString (token, "POS", str, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemSetString\" failed"))
				return;

			token = SItemNext(token, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
				return;
		}

		free(output);
		crfsuite_instance_finish(instance);

		itrPhrase = SItemNext(itrPhrase, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemNext\" failed"))
			return;
	}

	/* here all is OK */

	S_UNUSED(utt);

exit_cleanup:
	if ( ptr_model != NULL )
		free ( ptr_model );

	if ( ptr_tagger != NULL )
		free ( ptr_tagger );

	if ( ptr_attrs != NULL )
		free ( ptr_attrs );

	if ( ptr_labels != NULL )
		free ( ptr_labels );

	if ( instance != NULL )
		free ( instance );

}


/************************************************************************************/
/*                                                                                  */
/* SCrfSuiteUttProc class initialization                                            */
/*                                                                                  */
/************************************************************************************/

static SCrfSuiteUttProcClass CrfSuiteUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:SCrfSuiteUttProc",
		sizeof(SCrfSuiteUttProc),
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
