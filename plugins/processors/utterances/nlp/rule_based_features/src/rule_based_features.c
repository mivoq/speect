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
/* AUTHOR  : Giovanni Mazzocchin                                                    */
/* DATE    : July 2016                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Get features from rules.                                                         */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "rule_based_features.h"
#include <ctype.h>

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

static SRuleBasedFeaturesUttProcClass RuleBasedFeaturesUttProcClass;

/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_rule_based_features_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&RuleBasedFeaturesUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
		  "_s_rule_based_features_class_reg",
		  "Failed to register RuleBasedFeaturesUttProcClass");
}


S_LOCAL void _s_rule_based_features_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&RuleBasedFeaturesUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
		  "_s_rule_based_features_class_free",
		  "Failed to free RuleBasedFeaturesUttProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Initialize(SUttProcessor *self, const SVoice *voice, s_erc *error)
{
	S_CLR_ERR(error);
	SRuleBasedFeaturesUttProc *castSelf = S_CAST(self, SRuleBasedFeaturesUttProc, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to S_CAST failed"))
		goto quit_error;

	castSelf->prosSymbols = NULL;

	s_bool have_symbols = SUttProcessorFeatureIsPresent(self, "list definitions", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to \"SUttProcessorFeatureIsPresent\" failed"))
		goto quit_error;

	/* no Prosody symbols defined */
	if (have_symbols == FALSE)
	{
		return;
	}

	/* get the complete 'list definitions' map and put it into 'prosSymbols' SMap */
	castSelf->prosSymbols = S_CAST(SUttProcessorGetFeature(self, "list definitions", error),
			     SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Failed to get 'list definitions' SMap feature"))
		goto quit_error;

	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to \"SUtteranceGetRelation\" failed"))
		goto quit_error;

	/* Get the iterator for the SMap */
	SIterator *itrList = S_ITERATOR_GET(castSelf->prosSymbols, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to \"S_ITERATOR_GET\" failed"))
		goto quit_error;

	/* Loop that takes all SList in the SMap and export the elements in SMaps */
	while( itrList != NULL ) {

		const char *curStr = SIteratorKey(itrList, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Initialize",
			      "Call to \"SIteratorKey\" failed"))
			goto quit_error;

		SList *valueList;
		valueList = S_CAST(SMapGetObject(castSelf->prosSymbols, curStr, error), SList, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapGetObject\" failed"))
			goto quit_error;

		/* Initializing a new SMap */
		SMap *newCastMap= S_MAP(S_NEW(SMapHashTable, error));

		SIterator *itrValueList = S_ITERATOR_GET(valueList, error);

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
		SMapSetObject(castSelf->prosSymbols, curStr, (SObject *) newCastMap, error);
		itrList = SIteratorNext(itrList);
	}

	/* error cleanup */
	quit_error:
		return;

	S_UNUSED(voice);
}

static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);

	SObjectDecRef(obj);
}

static void Destroy(void *obj, s_erc *error)
{
	S_CLR_ERR(error);

	S_UNUSED(obj);
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

	if (posTagStr[0] == 'N')
	{
		int i = 0;

		while (posTagStr[i] != '\0')
		{
			filteredPosTagStr[i] = posTagStr[i];
			i++;
		}

		filteredPosTagStr[i] = '\0';
	}

	else if (posTagStr[0] == 'E')
	{
		if (posTagStr[1] == 'A')
		{
			filteredPosTagStr[0] = 'E';
			filteredPosTagStr[1] = 'A';
			filteredPosTagStr[2] = '\0';
		}
		else
		{
			filteredPosTagStr[0] = 'E';
			filteredPosTagStr[1] = '\0';
		}
	}

	else if (posTagStr[0] == 'T')
	{
		filteredPosTagStr[0] = 'D';
		filteredPosTagStr[1] = '\0';
	}

	else if (posTagStr[0] == 'V')
	{
		int i = 0;
		while (posTagStr[i] != '\0' && !isdigit(posTagStr[i]))
		{
			filteredPosTagStr[i] = posTagStr[i];
			i++;
		}
		filteredPosTagStr[i] = '\0';

		/* index for the second to last cell */
		int j = i - 2;
		if (filteredPosTagStr[j] == 'm' || filteredPosTagStr[j] == 'f')
		{
			if (filteredPosTagStr[j + 1] == 's' || filteredPosTagStr[j + 1] == 'p')
			{
				filteredPosTagStr[j] = '\0';
			}

			if (filteredPosTagStr[1] == 'M' || filteredPosTagStr[1] == 'A')
			{
				int k = 2;
				while (filteredPosTagStr[k] != '\0')
				{
					filteredPosTagStr[k - 1] = filteredPosTagStr[k];
					k++;
				}
				filteredPosTagStr[k - 1] = '\0';
			}
		}
	}

	else if (posTagStr[0] == 'S')
	{
		if ((posTagStr[1] == 'm' || posTagStr[1] == 'f' || posTagStr[1] == 'n') &&
		    (posTagStr[2] == 'p' || posTagStr[2] == 's' || posTagStr[2] == 'n'))
		{
			filteredPosTagStr[0] = posTagStr[0];
			filteredPosTagStr[1] = posTagStr[1];
			filteredPosTagStr[2] = posTagStr[2];
			filteredPosTagStr[3] = '\0';
		}
		else
		{
			if (posTagStr[1] == 'A' || posTagStr[1] == 'W' || posTagStr[1] == 'P')
			{
				filteredPosTagStr[0] = posTagStr[0];
				filteredPosTagStr[1] = posTagStr[1];
				filteredPosTagStr[2] = '\0';
			}
			else
			{
				filteredPosTagStr[0] = posTagStr[0];
				filteredPosTagStr[1] = '\0';
			}
		}
	}

	else
	{
		filteredPosTagStr[0] = posTagStr[0];
		filteredPosTagStr[1] = '\0';
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
static void setSentenceType(SItem *phrase, const SMap *prosSymbols, s_erc *error)
{
	S_CLR_ERR(error);

	/* types: "decl, "excl", "interrog" */
	/* stop at sentence's last token */
	const SItem *wordFromCurrentPhrase = SItemPathToItem(phrase, "daughter", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setSentenceType",
		      "Call to \"SItemPathToItem\" failed"))
		return;

	SItem *wordAsToken = SItemAs(wordFromCurrentPhrase, "Token", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setSentenceType",
		      "Call to \"SItemAs\" failed"))
		return;

	SItem *tokenItem = SItemParent(wordAsToken, error);
	SItem *firstTokenItem = tokenItem;

	s_bool isPunct = SItemFeatureIsPresent(tokenItem, "IsPunctuation", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setSentenceType",
		      "Call to \"SItemFeatureIsPresent\" failed"))
		return;

	s_bool isFinalPunct = FALSE;

	while (isFinalPunct == FALSE)
	{
		isPunct = SItemFeatureIsPresent(tokenItem, "IsPunctuation", error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "setSentenceType",
			      "Call to \"SItemFeatureIsPresent\" failed"))
			return;

		if (isPunct)
		{
			const char *punctStr = SItemGetName(tokenItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "setSentenceType",
				      "Call to \"SItemGetName\" failed"))
				return;

			if (s_strcmp(punctStr, ".", error) == 0)
			{
				isFinalPunct = TRUE;
				SItemSetString(phrase, "type", "decl", error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "setSentenceType",
					      "Call to \"SItemSetString\" failed"))
					return;
			}
			else if (s_strcmp(punctStr, "!", error) == 0)
			{
				isFinalPunct = TRUE;
				SItemSetString(phrase, "type", "excl", error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "setSentenceType",
					      "Call to \"SItemSetString\" failed"))
					return;
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
					SItemSetString(phrase, "type", "interrogW", error);
					if (S_CHK_ERR(error, S_CONTERR,
						      "setSentenceType",
						      "Call to \"SItemSetString\" failed"))
						return;
				} else {
					SItemSetString(phrase, "type", "interrog", error);
					if (S_CHK_ERR(error, S_CONTERR,
						      "setSentenceType",
						      "Call to \"SItemSetString\" failed"))
						return;
                                }
			quit_error:
				if (posValueStr_filtered)
				{
					S_FREE(posValueStr_filtered);
				}
			}
		}

		tokenItem = SItemNext(tokenItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "setSentenceType",
			      "Call to \"SItemNext\" failed"))
			return;

	}
}

static void Run(const SUttProcessor *self, SUtterance *utt,
		s_erc *error)
{
	const SRelation *phraseRel = NULL;
	/* loop on phraseItem(s) */
	const SItem *phraseItem = NULL;
	s_bool have_symbols;
	s_bool isFeatPresent;
	/* 'prosSymbols' holds the complete map read from voice.json's 'list definitions' */
	const SMap *prosSymbols = NULL;
	/* 'posValueStr' string value of current POS tag value */
	const char *posValueStr = NULL;

	/* 'valueList' is used to represent JSON value lists for each
	   entry in 'list definitions' map:
	   e.g.) - ["S","SP"]
	   - ["è","ha","ho"]
	   - ["A","B","I","N","NO","S","SA","SP","SW","V","X"]
	*/
	/* 'valueMap' has the same use as valueList */
	SMap *valueMap = NULL;

	/* Cast self processor to the SRuleBasedFeaturesUttProc*/
	SRuleBasedFeaturesUttProc *castSelf = S_CAST(self, SRuleBasedFeaturesUttProc, error);

	/* get the complete 'list definitions' map and put it into 'prosSymbols' SMap */
	prosSymbols = castSelf->prosSymbols;
	phraseRel = SUtteranceGetRelation(utt, "Phrase", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Run",
		      "Call to \"SUtteranceGetRelation\" failed"))
		goto quit_error;

	phraseItem = SRelationHead(phraseRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Run",
		      "Call to \"SRelationHead\" failed"))
		goto quit_error;

	const SItem* phraseItem_copy = phraseItem;
	while (phraseItem_copy != NULL)
	{
		setSentenceType((SItem*)phraseItem_copy, prosSymbols, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Run",
			      "Call to \"setSentenceType\" failed"))
			goto quit_error;

		phraseItem_copy = SItemNext(phraseItem_copy, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Run",
			      "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	/* loop on phraseItem, and
	 * get the tokens for each phrase inside an inner loop,
	 * looking at puncutation marks
         */
	while (phraseItem != NULL)
	{
		/* call 'getSentenceType' on 'phraseItem'
		 * at each iteration
                 */
		const SItem *wordFromCurrentPhrase = SItemPathToItem(phraseItem, "daughter", error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Run",
			      "Call to \"SItemPathToItem\" failed"))
			goto quit_error;
		SItem *wordAsToken = SItemAs(wordFromCurrentPhrase, "Token", error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Run",
			      "Call to \"SItemAs\" failed"))
			goto quit_error;
		SItem *tokenItem = SItemParent(wordAsToken, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Run",
			      "Call to \"SItemParent\" failed"))
			goto quit_error;

		/* I need a loop that stops at phraseItem-pointed phrase's final token */
		s_bool isStopPunct = FALSE;
		while (isStopPunct == FALSE)
		{
			/* BEGIN OF RULES SECTION */
			isFeatPresent = SItemFeatureIsPresent(tokenItem, "POS", error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "Run",
				      "Call to \"SItemFeatureIsPresent\" failed"))
				goto quit_error;

			/* we can continue only if the POS tag has been obtained */
			if (isFeatPresent == TRUE)
			{
				char *posValueStr_filtered = NULL;
				s_bool currPosInCurrList;

				/* ******************************* */
				/* FIRST RULE:
				 *  if the current word item's pos value is contained in the list
				 *  "pos_tonal_accent" from "list definitions",
				 * 		then -->	set current's item "accent" attribute to the value "tone"
				 */

				/* check if we have 'pos_tonal_accent' symbol */
				have_symbols = SMapObjectPresent(prosSymbols, "pos_tonal_accent", error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "Run",
					      "Call to \"SMapObjectPresent\" failed"))
					goto quit_error;

				if (have_symbols)
				{
					valueMap = S_CAST(SMapGetObject(prosSymbols, "pos_tonal_accent", error), SMap, error);
					if (S_CHK_ERR(error, S_CONTERR,
						      "Run",
						      "Call to \"SMapGetObject\" failed"))
						goto quit_error;
				}

				/* 'posValueStr' holds POS tag's value */
				posValueStr = SItemGetString(tokenItem, "POS", error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "Run",
					      "Call to \"SItemGetString\" failed"))
					goto quit_error;

				/* filter the current POS tag, remember to free the memory
				 *  pointed to by 'posValueStr_filtered' pointer
                                 */
				posValueStr_filtered = filterPosTag(posValueStr, error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "Run",
					      "Call to \"filterPosTag\" failed"))
					goto quit_error;

				/* check if the current POS tag exists in 'pos_tonal_accent' map */
				currPosInCurrList = searchStringMap(valueMap, posValueStr_filtered, error);
				if (currPosInCurrList == TRUE)
				{
					SItemSetString(tokenItem, "accent", "tone", error);
					if (S_CHK_ERR(error, S_CONTERR,
						      "Run",
						      "Call to \"SItemSetString\" failed"))
						goto quit_error;
				}

				if (posValueStr_filtered)
				{
					S_FREE(posValueStr_filtered);
				}

				/* ******************************* */
				/* SECOND RULE:
				 *  if the current word item's pos value is contained in the list
				 *  "pos_no_accent" from "list definitions" map,
				 * 		then -->	set current's item "accent" attribute to the value ""
				 */

				have_symbols = SMapObjectPresent(prosSymbols, "pos_no_accent", error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "Run",
					      "Call to \"SMapObjectPresent\" failed"))
					goto quit_error;

				if (have_symbols)
				{
					valueMap = S_CAST(SMapGetObject(prosSymbols, "pos_no_accent", error), SMap, error);
					if (S_CHK_ERR(error, S_CONTERR,
						      "Run",
						      "Call to \"SMapGetObject\" failed"))
						goto quit_error;
				}

				/* 'posValueStr' holds POS tag's value */
				posValueStr = SItemGetString(tokenItem, "POS", error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "Run",
					      "Call to \"SItemGetString\" failed"))
					goto quit_error;

				/* filter the current POS tag, remember to free the memory
				 *  pointed to by 'posValueStr_filtered' pointer
			         */
				posValueStr_filtered = filterPosTag(posValueStr, error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "Run",
					      "Call to \"filterPosTag\" failed"))
					goto quit_error;

				/* check if the current POS tag exists in 'pos_no_accent' map */
				currPosInCurrList = searchStringMap(valueMap, posValueStr_filtered, error);
				if (currPosInCurrList == TRUE)
				{
					SItemSetString(tokenItem, "accent", "", error);
					if (S_CHK_ERR(error, S_CONTERR,
						      "Run",
						      "Call to \"SItemSetString\" failed"))
						goto quit_error;
				}
				else
				{
					/* ******************************* */
					/* THIRD RULE:
					 *   otherwise set "accent" feature to ""
					 */
					SItemSetString(tokenItem, "accent", "", error);
					if (S_CHK_ERR(error, S_CONTERR,
						      "Run",
						      "Call to \"SItemSetString\" failed"))
						goto quit_error;
				}

				if (posValueStr_filtered)
				{
					S_FREE(posValueStr_filtered);
				}

				/* ********************************** */
				/* FOURTH RULE:
				 *    * if the current word item is contained in a phrase with
				 type = "decl" && if "prosodicPosition"'s type == "prenuclear" && accent = "tone"
				 * 			----> then set accent = "L+H*"
				 *
				 *    * else if the current word item is contained in a phrase with
				 type = "decl" && if "prosodicPosition"'s type == "nuclearNonParagraphFinal" && accent = "tone"
				 * 			----> then set accent = "!H*"
				 *    NOTE: "prosodicPosition" feature is not currently available on Speect
				 */

			}

			/* END OF RULES SECTION */
			/* ****************************** */
			/* checks for inner loop */
			s_bool isPunct = SItemFeatureIsPresent(tokenItem, "IsPunctuation", error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "Run",
				      "Call to \"SItemFeatureIsPresent\" failed"))
				goto quit_error;

			if (isPunct)
			{
				/* get the punctuation mark as a string and
				 * set isStopPunct flag if it is ".", "?" or "!"
				 */
				const char *punctStr = SItemGetName(tokenItem, error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "Run",
					      "Call to \"SItemGetName\" failed"))
					goto quit_error;

				if (s_strcmp(punctStr, ".", error) == 0 || s_strcmp(punctStr, "?", error) == 0 ||
				    s_strcmp(punctStr, "!", error) == 0)
				{
					isStopPunct = TRUE;
				}
			}

			tokenItem = SItemNext(tokenItem, error);
		}

		phraseItem = SItemNext(phraseItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Run",
			      "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	/* error cleanup */

quit_error:
	return;


	S_UNUSED(self);
}


/************************************************************************************/
/*                                                                                  */
/* SRuleBasedFeaturesUttProc class initialization                                   */
/*                                                                                  */
/************************************************************************************/

static SRuleBasedFeaturesUttProcClass RuleBasedFeaturesUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:SRuleBasedFeaturesUttProc",
		sizeof(SRuleBasedFeaturesUttProc),
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
	Run                  /* run     */
};
