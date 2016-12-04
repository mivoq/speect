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

static s_bool isASCIIUpper (char c)
{
	/* Here we rely on the fact that most compilers use ASCII */
	if( c >= 'A' && c <= 'Z' )
		return TRUE;

	return FALSE;
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

/* isFinalToken returns true if the word token is the last of the sentence
 *
 * */
static s_bool isFinalToken (const SItem *word, s_erc *error)
{
	const SItem *last = SItemPathToItem(word, "parent.daughtern", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "isFinalToken",
		      "Call to \"SItemPathToItem\" failed"))
		return FALSE;

	return word == last;
}

/* setProsodyPosition should find the type of the prosody position and
 * set the token "accent" attribute to a TOBI format
 *
 * Nucleus = last token with accent "tone"
 *
 * Values:
 * 	-nuclearParagraphFinal
 * 	-nuclearNonParagraphFinal
 *  -prenuclear
 *  -postnuclear
 *
 * */
static s_bool setProsodyPosition(const SItem *word, s_bool nucleusAssigned, s_erc *error)
{
	s_bool result = FALSE;
	s_bool isFinalWord= isFinalToken(word, error);

	SItem *wordInToken = SItemAs(word, "Token", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setProsodyPosition",
		      "Call to \"SItemAs\" failed"))
		return FALSE;
	SItem *tokenItem = SItemParent(wordInToken, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setProsodyPosition",
		      "Call to \"SItemParent\" failed"))
		return FALSE;

	const char *accent= SItemGetString(tokenItem, "accent", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setProsodyPosition",
		      "Call to \"SItemGetString\" failed"))
		return FALSE;

	const char *prosodicPositionType;

	if(nucleusAssigned)
		prosodicPositionType = "prenuclear";
	else
	{
		if( s_strcmp(accent, "tone", error) == 0)
		{
			nucleusAssigned = TRUE;

			if(isFinalWord)
				prosodicPositionType = "nuclearParagraphFinal";
			else
				prosodicPositionType = "nuclearNonParagraphFinal";
		}
		else
			prosodicPositionType = "postnuclear";
	}

	SItemSetString(tokenItem, "prosodic_position_type", prosodicPositionType, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Run",
		      "Call to \"SItemSetString\" failed"))
		return FALSE;

	/* Apply transformation rules of the accent*/
	const SItem *phrase= SItemPathToItem(word, "R:Phrase.parent", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setProsodyPosition",
		      "Call to \"SItemPathToItem\" failed"))
		return FALSE;

	SObject* extract = SItemPathToFeatProc(phrase, "phrase_type", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setProsodyPosition",
		      "Call to \"SItemPathToFeatProc\" failed")) {
		goto set_prosody_position_cleanup;
	}

	const char *type = SObjectGetString(extract, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setProsodyPosition",
		      "Call to \"SObjectGetString\" failed")) {
		goto set_prosody_position_cleanup;
	}

	if( s_strcmp( accent, "tone", error) == 0 )
	{
		/* Applying Rules for decl type sentence*/
		if( s_strcmp( type, "decl", error) == 0)
		{
			if( s_strcmp( prosodicPositionType, "prenuclear", error) == 0 )
			{
				SItemSetString(tokenItem, "accent", "L+H*", error);
			}
			else if( s_strcmp( prosodicPositionType, "nuclearParagraphFinal", error ) == 0 ||
			         s_strcmp( prosodicPositionType, "nuclearNonParagraphFinal", error ) == 0 )
			{
				SItemSetString(tokenItem, "accent", "H+L*", error);
			}
		}
		/* Applying Rules for excl type sentence*/
		else if( s_strcmp( type, "excl", error) == 0 )
		{
			if( s_strcmp( prosodicPositionType, "prenuclear", error) == 0 )
			{
				SItemSetString(tokenItem, "accent", "H*", error);
			}
			else if( s_strcmp( prosodicPositionType, "nuclearParagraphFinal", error ) == 0 ||
			         s_strcmp( prosodicPositionType, "nuclearNonParagraphFinal", error ) == 0 )
			{
				SItemSetString(tokenItem, "accent", "H+L*", error);
			}
		}
		/* Applying Rules for interrog type sentence*/
		else if( s_strcmp( type, "interrog", error) == 0 ||
		         s_strcmp( type, "interrogW", error) == 0 )
		{
			if( s_strcmp( prosodicPositionType, "prenuclear", error) == 0 )
			{
				SItemSetString(tokenItem, "accent", "H*", error);
			}
			else if( s_strcmp( prosodicPositionType, "nuclearParagraphFinal", error ) == 0 ||
			         s_strcmp( prosodicPositionType, "nuclearNonParagraphFinal", error ) == 0 )
			{
				SItemSetString(tokenItem, "accent", "H+L*", error);
			}
		}

		/* DEFAULT RULE to be applied if nothing worked*/
		else
		{
			SItemSetString(tokenItem, "accent", "H*", error);
		}

		/* We can check only here for errors because there is only one
		 * SItemSetString done (one for every leaf of the binary tree)
		 * */
		if (S_CHK_ERR(error, S_CONTERR,
			      "setProsodyPosition",
			      "Call to \"SItemSetString\" failed"))
			goto set_prosody_position_cleanup;
	}
	/* NO ELSE because if no accent is set, then no accent is needed to be setted
	 * */

	/*Now we iterate on the syllables of the word, to set the accent only on the
	 * stressed one and set 0 on the other.
	 * */
	const char* stressed;
	accent= SItemGetString(tokenItem, "accent", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setProsodyPosition",
		      "Call to \"SItemGetString\" failed"))
		goto set_prosody_position_cleanup;

	const SItem *syllEnd = SItemPathToItem(word, "n.R:SylStructure.daughter.R:Syllable", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setProsodyPosition",
		      "Call to \"SItemPathToItem\" failed"))
		goto set_prosody_position_cleanup;

	SItem *wordInSylStructure = SItemAs(word, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setProsodyPosition",
		      "Call to \"SItemAs\" failed"))
		goto set_prosody_position_cleanup;

	SItem *syllAsSylStructure = SItemDaughter(wordInSylStructure, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setProsodyPosition",
		      "Call to \"SItemDaughter\" failed"))
		goto set_prosody_position_cleanup;

	SItem *syll = SItemAs(syllAsSylStructure, "Syllable", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setProsodyPosition",
		      "Call to \"SItemAs\" failed"))
		goto set_prosody_position_cleanup;

	while (syll != syllEnd)
	{
		stressed = SItemGetString(syll, "stress", error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "setProsodyPosition",
			      "Call to \"SItemGetString\" failed"))
			goto set_prosody_position_cleanup;

		if(s_strcmp( stressed, "primary", error) == 0 && s_strcmp( accent, "", error) != 0 )
			SItemSetString(syll, "accent", accent, error);

		syll = SItemNext (syll, error);
	}

	if(extract != NULL) {
		S_DELETE(extract, "setProsodyPosition", error);
	}
	return nucleusAssigned;
set_prosody_position_cleanup:
	if(extract != NULL) {
		S_DELETE(extract, "setProsodyPosition", error);
	}
	return result;
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

/*
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
  * */

	const SItem* phrase = phraseItem;

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

				else
				{
					/* ******************************* */
					/* DEFAULT RULE:
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
			if (S_CHK_ERR(error, S_CONTERR,
				      "Run",
				      "Call to \"SItemNext\" failed"))
				goto quit_error;
			if(tokenItem == NULL) {
				isStopPunct = TRUE;
			}
		}

		phraseItem = SItemNext(phraseItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Run",
			      "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	/* Now we iterate to define the ProsodyPositionType and the real tobi Accent */
	/* The iteration start from the last token until the begin of the sentence */

	while (phrase != NULL)
	{
		/* Save the initial word of the current phrase */
		const SItem *wordBegin = SItemPathToItem(phrase, "daughter", error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Run",
			      "Call to \"SItemPathToItem\" failed"))
			goto quit_error;

		/* Save the final word of the current phrase */
		/* that will be used to iterate with setProsodyPosition*/
		const SItem *wordEnd = SItemPathToItem(phrase, "daughtern", error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Run",
			      "Call to \"SItemPathToItem\" failed"))
			goto quit_error;

		s_bool nucleusAssigned = FALSE;

		while (wordBegin != wordEnd)
		{
			nucleusAssigned = setProsodyPosition(wordEnd,nucleusAssigned, error);

			wordEnd = SItemPrev(wordEnd, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "Run",
				      "Call to \"SItemPrev\" failed"))
				goto quit_error;
		}

		/* call to setProsodyPosition on the first word, the flag of the cicle */
		setProsodyPosition(wordEnd,nucleusAssigned, error);

		phrase =  SItemNext(phrase, error);
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
