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
/* Phrase type feature processor.                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "phrase_type.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SPhraseTypeFeatProcClass PhraseTypeFeatProcClass;

/* SPhraseTypeFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_phrase_type_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&PhraseTypeFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phrase_type_class_reg",
			  "Failed to register SPhraseTypeFeatProcClass");
}


S_LOCAL void _s_phrase_type_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&PhraseTypeFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phrase_type_class_free",
			  "Failed to free SPhraseTypeFeatProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/
static void Destroy(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SPhraseTypeFeatProc *castSelf = S_CAST(obj, SPhraseTypeFeatProc, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Destroy",
		      "Call to S_CAST failed"))
		return;

	if(castSelf->symbols != NULL) {
		/* delete the symbols map */
		S_DELETE(castSelf->symbols, "Destroy", error);
		castSelf->symbols = NULL;
	}
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}

static void Initialize(SFeatProcessor *self, const SMap* features, s_erc *error)
{
	S_CLR_ERR(error);

	SPhraseTypeFeatProc *castSelf = S_CAST(self, SPhraseTypeFeatProc, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to S_CAST failed"))
		goto quit_error;

	castSelf->symbols =  S_CAST( SMapGetObject(features , "list definitions", error),
								SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "Initialize",
			  "Call to \"SMapGetObject\" failed"))
		goto quit_error;

	castSelf->symbols = SMapCopy ( NULL , castSelf->symbols , error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "Initialize",
			  "Call to \"SMapCopy\" failed"))
		goto quit_error;

	/* Get the iterator for the SMap */
	SIterator *itrList = S_ITERATOR_GET(castSelf->symbols, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to \"S_ITERATOR_GET\" failed"))
		goto quit_error;

	while( itrList != NULL ) {

		const char *curStr = SIteratorKey(itrList, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Initialize",
			      "Call to \"SIteratorKey\" failed"))
			goto quit_error;

		SList *valueList;
		valueList = S_CAST(SMapGetObject(castSelf->symbols, curStr, error), SList, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapGetObject\" failed"))
			goto quit_error;

		/* Initializing a new SMap */
		SMap *newCastMap= S_MAP(S_NEW(SMapHashTable, error));

		SIterator *itrValueList = S_ITERATOR_GET(valueList, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"S_ITERATOR_GET\" failed"))
			goto quit_error;

		while(itrValueList != NULL) {
			const SObject *curValueObj = SIteratorObject(itrValueList, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"SIteratorObject\" failed"))
				goto quit_error;

			const char *curValueStr = SObjectGetString(curValueObj, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"SObjectGetString\" failed"))
				goto quit_error;

			/* Insert the string inside the map */
			SMapSetString(newCastMap, curValueStr, curValueStr, error);
			if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"SMapSetObject\" failed"))
				goto quit_error;

			itrValueList = SIteratorNext(itrValueList);
		}

		/* Insertion of the SMap inside the SMap */
		SMapSetObject(castSelf->symbols, curStr, (SObject *) newCastMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"SMapSetObject\" failed"))
				goto quit_error;
		itrList = SIteratorNext(itrList);
	}

	/* error cleanup */
	quit_error:
		return;
}

static s_bool isASCIIUpper (char c)
{
	/* Here we rely on the fact that most compilers use ASCII */
	if( c >= 'A' && c <= 'Z' )
		return TRUE;

	return FALSE;
}

static s_bool searchStringMap(SMap *map, char *str, s_erc *error)
{
	S_CLR_ERR(error);
	/* Search for the word in the SMap, using it as a Key */
	s_bool found= SMapObjectPresent(map, str, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "searchStringMap",
			  "Call to \"SMapObjectPresent\" failed"))
		return FALSE;

	return found;
}

static char* filterPosTag(const char *posTagStr, s_erc *error)
{
	S_CLR_ERR(error);
	char *filteredPosTagStr = malloc(sizeof(char) * 10);
	s_strcpy( filteredPosTagStr, posTagStr, error);

	s_bool end = FALSE;
	s_bool firstUpper = FALSE;
	int i=0;

	while( !end )
	{
		if( !firstUpper )
		{
			if( isASCIIUpper(filteredPosTagStr[i]) )
				firstUpper = TRUE;
		}
		else
		{
			if( !isASCIIUpper(filteredPosTagStr[i]) )
			{
				filteredPosTagStr[i] = '\0';
				end = TRUE;
			}
		}
		i++;
		if( filteredPosTagStr[i] == '\0' )
			end = TRUE;
	}

	return filteredPosTagStr;
}

/* setSentenceType should be made out of two parts:
 * 	    1) the first section searchs for the last punctuation element of the sentence
 * 				-> if it is a '.' --> set "decl" type (where should I set this feature value?)
 * 				-> if it is a '!' --> set "excl" type (where should I set this feature value?)
 * 				-> if it is a '?' --> set "interrog" type (where should I set this feature value?)
 * 	    2) if the first part decides for "interrog" type, there should be other controls
 * 	       to establish the sentence's complete type
 * */
static char* setSentenceType(const SItem *phrase, SMap *prosSymbols, s_erc *error)
{
	S_CLR_ERR(error);

	char* result = "decl";

	/* types: "decl, "excl", "interrog" */
	/* stop at sentence's last token */
	const SItem *wordFromCurrentPhrase = SItemPathToItem(phrase, "daughter", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setSentenceType",
		      "Call to \"SItemPathToItem\" failed"))
		return NULL;

	SItem *wordAsToken = SItemAs(wordFromCurrentPhrase, "Token", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setSentenceType",
		      "Call to \"SItemAs\" failed"))
		return NULL;

	SItem *tokenItem = SItemParent(wordAsToken, error);
	SItem *firstTokenItem = tokenItem;

	s_bool isPunct = SItemFeatureIsPresent(tokenItem, "IsPunctuation", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setSentenceType",
		      "Call to \"SItemFeatureIsPresent\" failed"))
		return NULL;

	s_bool isFinalPunct = FALSE;

	while (isFinalPunct == FALSE)
	{
		isPunct = SItemFeatureIsPresent(tokenItem, "IsPunctuation", error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "setSentenceType",
			      "Call to \"SItemFeatureIsPresent\" failed"))
			return NULL;

		if (isPunct)
		{
			const char *punctStr = SItemGetName(tokenItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "setSentenceType",
				      "Call to \"SItemGetName\" failed"))
				return NULL;

			if (s_strcmp(punctStr, ".", error) == 0)
			{
				isFinalPunct = TRUE;
				result = "decl";
			}
			else if (s_strcmp(punctStr, "!", error) == 0)
			{
				isFinalPunct = TRUE;
				result = "excl";
			}
			else if (s_strcmp(punctStr, "?", error) == 0)
			{
				isFinalPunct = TRUE;
				const char *posValueStr = NULL;
				char *posValueStr_filtered = NULL;
				s_bool currPosInCurrList;
				s_bool have_symbols = FALSE;
				SMap* valueMap = NULL;

				have_symbols = SMapObjectPresent(prosSymbols, "firstPosInQuestionW", error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "SetSentenceType",
					      "Call to \"SMapObjectPresent\" failed"))
					goto quit_error;

				if (have_symbols)
				{
					valueMap = S_CAST(SMapGetObject(prosSymbols, "firstPosInQuestionW", error), SMap, error);
					if (S_CHK_ERR(error, S_CONTERR,
						      "SetSentenceType",
						      "Call to \"SMapGetObject\" failed"))
						goto quit_error;
				}
				else
					goto quit_error;

				posValueStr = SItemGetString(firstTokenItem, "POS", error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "SetSentenceType",
					      "Call to \"SItemGetString\" failed"))
					goto quit_error;

				/* filter the current POS tag, remember to free the memory
				 *  pointed to by 'posValueStr_filtered' pointer
                                 */
				posValueStr_filtered = filterPosTag(posValueStr, error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "SetSentenceType",
					      "Call to \"filterPosTag\" failed"))
					goto quit_error;

				currPosInCurrList = searchStringMap(valueMap, posValueStr_filtered, error);
				if (currPosInCurrList == TRUE)
				{
					result = "interrogW";
				}
				else
				{
					result = "interrog";
				}
				quit_error:
					if (posValueStr_filtered)
					{
						S_FREE(posValueStr_filtered);
					}
					break;
			}
		}

		tokenItem = SItemNext(tokenItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "setSentenceType",
			      "Call to \"SItemNext\" failed"))
			return NULL;
		if(tokenItem == NULL) {
			isFinalPunct = TRUE;
		}
	}
	return result;
}

static SObject *Run(const SFeatProcessor *self, const SItem *item,
					s_erc *error)
{
	SObject *extractedFeat = NULL;
	char* type;

	SPhraseTypeFeatProc *castSelf = S_CAST(self, SPhraseTypeFeatProc, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to S_CAST failed"))
		goto quit_error;

	type = setSentenceType(item, castSelf->symbols, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SetSentenceType\" failed"))
		goto quit_error;

	if( type == NULL )
		goto quit_error;

	extractedFeat = SObjectSetString( type, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectSetString\" failed"))
		goto quit_error;

	return extractedFeat;

	/* error cleanup */
quit_error:
	if (extractedFeat != NULL)
		S_DELETE(extractedFeat, "Run", error);

	return NULL;

	S_UNUSED(self);
	S_UNUSED(item);
}


/************************************************************************************/
/*                                                                                  */
/* SPhraseTypeFeatProc class initialization                                         */
/*                                                                                  */
/************************************************************************************/

static SPhraseTypeFeatProcClass PhraseTypeFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SPhraseTypeFeatProc",
		sizeof(SPhraseTypeFeatProc),
		{ 0, 1},
		NULL,            /* init    */
		Destroy,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SFeatProcessorClass */
	Initialize,          /* initialize */
	Run                  /* run     */
};
