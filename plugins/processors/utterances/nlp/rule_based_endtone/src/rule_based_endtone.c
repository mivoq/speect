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
/* AUTHOR  : Federico Rossetto                                                      */
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

#include "rule_based_endtone.h"
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

static SRuleBasedEndtoneUttProcClass RuleBasedEndtoneUttProcClass;

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
S_LOCAL void _s_rule_based_endtone_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&RuleBasedEndtoneUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
		  "_s_rule_based_endtone_class_reg",
		  "Failed to register RuleBasedEndtoneUttProcClass");
}


S_LOCAL void _s_rule_based_endtone_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&RuleBasedEndtoneUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
		  "_s_rule_based_endtone_class_free",
		  "Failed to free RuleBasedEndtoneUttProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void setBoundary (SRelation * boundariesRelation, int bi, const char* tone, s_erc *error);

/* Set of functions used to verify conditions on some item's attributes
 * to apply the tobi-endtone's rules for the break-index and Endtone */

static s_bool matchStringValue (const SItem *item, const char* path, const char* value, s_erc *error);

static s_bool matchInlistStringValue (const SItem *item, const char* path, SMap* value, s_erc *error);

static s_bool matchIntValue (const SItem *item, const char* path, int value, s_erc *error);

static s_bool matchMajorEqualValue (const SItem *item, const char* path, int value, s_erc *error);

static int getFollowingTokensNum (const SItem *item, s_erc *error);

static int getPreviousTokensNum (const SItem *item, s_erc *error);

/* The function that applies all the rules on a single token */

static void applyRules (const SItem *token, SRelation * boundariesRelation, const SMap* symbols, s_erc *error);

/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Initialize(SUttProcessor *self, const SVoice *voice, s_erc *error)
{
	S_CLR_ERR(error);
	SRuleBasedEndtoneUttProc *castSelf = S_CAST(self, SRuleBasedEndtoneUttProc, error);
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

static s_bool searchStringMap(const SMap *map, const char *str, s_erc *error)
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

static s_bool matchStringValue (const SItem *item, const char* path, const char* value, s_erc *error)
{
	s_bool result = FALSE;

	const SObject *target_p = NULL;
	const SObject *target = SItemPathToFeature(item, path, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "matchStringValue",
			  "Call to \"SItemPathToFeature\" failed"))
		goto match_string_value_return;

	if (target == NULL)
	{
		target_p = SItemPathToFeatProc(item, path, error);
		target = target_p;
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"SItemPathToFeature\" failed"))
			goto match_string_value_return;
	}

	if (target != NULL)
	{
		/* The feature has been found */
		const char* match = SObjectGetString (target, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"SObjectGetString\" failed"))
			goto match_string_value_return;

		result = (s_strcmp(value, match, error) == 0);
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"s_strcmp\" failed"))
			goto match_string_value_return;
	}
match_string_value_return:
	if (target_p != NULL)
	{
		S_DELETE(target_p, "matchStringValue", error);
	}
	return result;
}

static s_bool matchInlistStringValue (const SItem *item, const char* path, SMap* value, s_erc *error)
{
	s_bool result = FALSE;

	const SObject *target_p = NULL;
	const SObject *target = SItemPathToFeature(item, path, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "matchStringValue",
			  "Call to \"SItemPathToFeature\" failed"))
		return FALSE;

	if (target == NULL)
	{
		target_p = SItemPathToFeatProc(item, path, error);
		target = target_p;
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"SItemPathToFeature\" failed"))
			goto match_in_list_string_value_return;
	}

	if (target != NULL)
	{
		/* The feature has been found */
		const char* match = SObjectGetString (target, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"SObjectGetString\" failed"))
			goto match_in_list_string_value_return;

		result = searchStringMap (value, match, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"searchStringMap\" failed"))
			goto match_in_list_string_value_return;

		return result;
	}

match_in_list_string_value_return:
	if (target_p != NULL)
	{
		S_DELETE(target_p, "matchInListStringValue", error);
	}
	return result;
}

static s_bool matchIntValue (const SItem *item, const char* path, int value, s_erc *error)
{
	s_bool result = FALSE;

	const SObject *target_p = NULL;
	const SObject *target = SItemPathToFeature(item, path, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "matchStringValue",
			  "Call to \"SItemPathToFeature\" failed"))
		goto match_int_value_return;

	if (target == NULL)
	{
		target_p = SItemPathToFeatProc(item, path, error);
		target = target_p;
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"SItemPathToFeature\" failed"))
			goto match_int_value_return;
	}

	if (target == NULL && s_strcmp (path, "PreviousTokens", error) == 0)
	{
		int tmp = getPreviousTokensNum(item, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"getPreviousTokensNum\" failed"))
			goto match_int_value_return;

		target_p = SObjectSetInt(tmp, error);
		target = target_p;
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"SObjectSetInt\" failed"))
			goto match_int_value_return;
	}

	if (target == NULL && s_strcmp (path, "FollowingTokens", error) == 0)
	{
		int tmp = getFollowingTokensNum(item, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"getPreviousTokensNum\" failed"))
			goto match_int_value_return;

		target_p = SObjectSetInt(tmp, error);
		target = target_p;
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"SObjectSetInt\" failed"))
			goto match_int_value_return;
	}

	if (target != NULL)
	{
		/* The feature has been found */
		int match = SObjectGetInt (target, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"SObjectGetInt\" failed"))
			goto match_int_value_return;

		result = ( match == value );

		goto match_int_value_return;
	}

match_int_value_return:
	if (target_p != NULL)
	{
		S_DELETE(target_p, "matchIntValue", error);
	}
	return result;
}

static s_bool matchMajorEqualValue (const SItem *item, const char* path, int value, s_erc *error)
{
	s_bool result = FALSE;

	const SObject *target_p = NULL;
	const SObject *target = SItemPathToFeature(item, path, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "matchStringValue",
			  "Call to \"SItemPathToFeature\" failed"))
		goto match_major_equal_value_return;

	if (target == NULL)
	{
		target_p = SItemPathToFeatProc(item, path, error);
		target = target_p;
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"SItemPathToFeature\" failed"))
			goto match_major_equal_value_return;
	}

	if (target == NULL && s_strcmp (path, "PreviousTokens", error) == 0)
	{
		int tmp = getPreviousTokensNum(item, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"getPreviousTokensNum\" failed"))
			goto match_major_equal_value_return;

		target_p = SObjectSetInt(tmp, error);
		target = target_p;
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"SObjectSetInt\" failed"))
			goto match_major_equal_value_return;
	}

	if (target == NULL && s_strcmp (path, "FollowingTokens", error) == 0)
	{
		int tmp = getFollowingTokensNum(item, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"getPreviousTokensNum\" failed"))
			goto match_major_equal_value_return;

		target_p = SObjectSetInt(tmp, error);
		target = target_p;
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"SObjectSetInt\" failed"))
			goto match_major_equal_value_return;
	}

	if (target != NULL)
	{
		/* The feature has been found */
		int match = SObjectGetInt (target, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "matchStringValue",
				  "Call to \"SObjectGetInt\" failed"))
			goto match_major_equal_value_return;

		result = ( match >= value );

		goto match_major_equal_value_return;
	}

match_major_equal_value_return:
	if (target_p != NULL)
	{
		S_DELETE(target_p, "matchMajorEqualValue", error);
	}
	return result;
}

static int getFollowingTokensNum (const SItem *item, s_erc *error)
{
	int count = -1;

	while ( item != NULL )
	{
		count += 1;

		item = SItemNext (item, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "getFollowingTokensNum",
				  "Call to \"SItemNext\" failed"))
			return 0;
	}
	return count;
}

static int getPreviousTokensNum (const SItem *item, s_erc *error)
{
	int count = -1;

	while ( item != NULL )
	{
		count += 1;

		item = SItemPrev (item, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "getFollowingTokensNum",
				  "Call to \"SItemPrev\" failed"))
			return 0;
	}
	return count;
}

static void setBoundary (SRelation * boundariesRelation, int bi, const char* tone, s_erc *error)
{
	if (boundariesRelation == NULL )
		return;

	SItem *boundaryItem = SRelationAppend ( boundariesRelation, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "setBoundary",
			  "Call to \"SRelationAppend\" failed"))
		return;

	if (boundaryItem != NULL)
	{
		SItemSetString ( boundaryItem, "name", "Boundary", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "setBoundary",
				  "Call to \"SItemSetString\" failed"))
			return;

		SItemSetInt ( boundaryItem, "breakindex", bi, error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "setBoundary",
				  "Call to \"SItemSetString\" failed"))
			return;

		if ( tone != NULL )
		{
			SItemSetString ( boundaryItem, "tobi_endtone", tone, error );
			if (S_CHK_ERR(error, S_CONTERR,
					  "setBoundary",
					  "Call to \"SItemSetString\" failed"))
				return;
		}
	}
}

static void applyRules (const SItem *token, SRelation * boundariesRelation, const SMap* symbols, s_erc *error)
{
	s_bool found = FALSE;
	SMap* posPunctuation = (SMap*) SMapGetObject ( symbols, "pos_punctuation", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"SMapGetObject\" failed"))
		return;

	SMap* FFelipses = (SMap*) SMapGetObject ( symbols, "FFelipses", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"SMapGetObject\" failed"))
		return;

	SMap* FCcolonSemicolon = (SMap*) SMapGetObject ( symbols, "FCcolon-semicolon", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"SMapGetObject\" failed"))
		return;

	SMap* FBopen = (SMap*) SMapGetObject ( symbols, "FBopen", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"SMapGetObject\" failed"))
		return;

	SMap* FBclosed = (SMap*) SMapGetObject ( symbols, "FBclosed", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"SMapGetObject\" failed"))
		return;

	/* Rule #1
	 * <prevTokens num="0"/>
	 * <attributes pos="INLIST:pos_punctuation"/>
	 * <action bi="0"/>
	 * */
	found = matchIntValue(token, "PreviousTokens", 0, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchInlistStringValue (token, "POS", posPunctuation, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 0, NULL, error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #2
	 * <attributes pos="INLIST:pos_punctuation"/>
	 * <nextAttributes pos="INLIST:pos_punctuation"/>
	 * <action bi="0"/>
	 * */
	found = matchInlistStringValue (token, "POS", posPunctuation, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchInlistStringValue (token, "n.POS", posPunctuation, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 0, NULL, error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #3
	 * <sentence type="decl"/>
	 * <folTokens num="0"/>
	 * <action bi="5" tone="L-L%"/>
	 *
	 * Rule #4
	 * <sentence type="excl"/>
	 * <folTokens num="0"/>
	 * <action bi="5" tone="L-L%"/>
	 *
	 * Rule #5
	 * <sentence type="interrogW"/>
	 * <folTokens num="0"/>
	 * <action bi="5" tone="L-L%"/>
	 * */

	found = matchStringValue (token, "daughter.R:Phrase.parent.phrase_type", "decl", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found || matchStringValue (token, "daughter.R:Phrase.parent.phrase_type", "excl", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found || matchStringValue (token, "daughter.R:Phrase.parent.phrase_type", "interrogW", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found || matchStringValue (token, "p.daughter.R:Phrase.parent.phrase_type", "decl", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found || matchStringValue (token, "p.daughter.R:Phrase.parent.phrase_type", "excl", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found || matchStringValue (token, "p.daughter.R:Phrase.parent.phrase_type", "interrogW", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchIntValue(token, "FollowingTokens", 0, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 5, "L-L%", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #6
	 * <sentence type="interrog"/>
	 * <folTokens num="0"/>
	 * <action bi="5" tone="L-H%"/>
	 * */

	found = matchStringValue (token, "daughter.R:Phrase.parent.phrase_type", "interrog", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found || matchStringValue (token, "p.daughter.R:Phrase.parent.phrase_type", "interrog", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchIntValue(token, "FollowingTokens", 0, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 5, "L-H%", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #7
	 * <text word="INLIST:FFelipses"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="4+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus1Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus2Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus3Attributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="4" tone="L-H%"/>
	 * */

	found = matchInlistStringValue ( token, "name", FFelipses, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 4, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 4, "L-H%", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #8
	 * <text word="INLIST:FFelipses"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="1+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="3" tone="H-"/>
	 * */

	found = matchInlistStringValue ( token, "name", FFelipses, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 3, "H-", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #9
	 * <attributes pos="FF"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="7+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus1Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus2Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus3Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus4Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus5Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus6Attributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="4" tone="L-L%"/>
	 * */

	found = matchStringValue ( token, "POS", "FF", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 7, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.p.p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 4, "L-L%", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #10
	 * <attributes pos="FF"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="1+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="3" tone="L-"/>
	 * */

		found = matchStringValue ( token, "POS", "FF", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 3, "L-", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #11
	 * <text word="INLIST:FCcolon-semicolon"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="4+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus1Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus2Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus3Attributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="4" tone="L-L%"/>
	 * */

	found = matchInlistStringValue ( token, "name", FCcolonSemicolon, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 4, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 4, "L-L%", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #12
	 * <text word="INLIST:FCcolon-semicolon"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="1+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="3" tone="L-"/>
	 * */

	found = matchInlistStringValue ( token, "name", FCcolonSemicolon, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 3, "L-", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #13
	 * <attributes pos="FC"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="4+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus1Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus2Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus3Attributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="4" tone="L-L%"/>
	 * */

	found = matchStringValue ( token, "POS", "FC", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 4, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 4, "L-L%", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #14
	 * <attributes pos="FC"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="1+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="3" tone="L-"/>
	 * */

	found = matchStringValue ( token, "POS", "FC", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 3, "L-", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #15
	 * <text word="INLIST:INLIST:FBopen"/> Errore di battitura?
	 * <folTokens num="1+"/>
	 * <prevTokens num="4+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus1Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus2Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus3Attributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="4" tone="L-L%"/>
	 * */

	found = matchInlistStringValue ( token, "name", FBopen, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 4, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 4, "L-L%", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #16
	 * <text word="INLIST:INLIST:FBopen"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="1+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="3" tone="L-"/>
	 * */

	found = matchInlistStringValue ( token, "name", FBopen, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 3, "L-", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #17
	 * <text word="INLIST:INLIST:FBclosed"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="4+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus1Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus2Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus3Attributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="4" tone="H-L%"/>
	 * */

	found = matchInlistStringValue ( token, "name", FBclosed, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 4, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 4, "H-L%", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #18
	 * <text word="INLIST:INLIST:FBclosed"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="1+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="3" tone="H-"/>
	 * */

		found = matchInlistStringValue ( token, "name", FBclosed, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 3, "H-", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #19
	 * <attributes pos="FB"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="4+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus1Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus2Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus3Attributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="4" tone="L-L%"/>
	 * */

	found = matchStringValue ( token, "POS", "FB", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 4, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.p.p.p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 4, "L-L%", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #20
	 * <attributes pos="FB"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="1+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="3" tone="H-"/>
	 * */

	found = matchStringValue ( token, "POS", "FB", error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "n.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && !matchInlistStringValue ( token, "p.POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 3, "H-", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #21
	 * <attributes pos="INLIST:pos_punctuation"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="1+"/>
	 * <action bi="3" tone="H-"/>
	 * */

	found = matchInlistStringValue ( token, "POS", posPunctuation, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchInlistStringValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "FollowingTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	found = found && matchMajorEqualValue(token, "PreviousTokens", 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "applyRules",
			  "Call to \"matchIntValue\" failed"))
		return;

	if ( found )
	{
		setBoundary ( boundariesRelation, 3, "H-", error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "applyRules",
				  "Call to \"setBoundary\" failed"))
			return;

		return;
	}

	/* Rule #22
	 * <attributes pos="INLIST:pos_punctuation"/>
	 * <folTokens num="1+"/>
	 * <prevTokens num="4+"/>
	 * <nextAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousAttributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus1Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus2Attributes pos="!INLIST:pos_punctuation"/>
	 * <previousMinus3Attributes pos="!INLIST:pos_punctuation"/>
	 * <action bi="4" tone="L-L%"/>
	 * */

	/* Rule not used, because it's a specification of the previous Rule! */



}

static void Run(const SUttProcessor *self, SUtterance *utt,
		s_erc *error)
{
	const SRelation *tokenRel = NULL;
	SRelation *boundariesRel = NULL;
	/* loop on phraseItem(s) */
	const SItem *tokenItem = NULL;
	const SItem *wordItem = NULL;
	/* 'prosSymbols' holds the complete map read from voice.json's 'list definitions' */
	const SMap *prosSymbols = NULL;

	SRuleBasedEndtoneUttProc * castSelf = (SRuleBasedEndtoneUttProc *) self;

	boundariesRel = SUtteranceNewRelation(utt, "Boundaries", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceNewRelation\" failed"))
		goto quit_error;

	tokenRel = SUtteranceGetRelation(utt, "Token", error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "Run",
			  "Call to \"SUtteranceGetRelation\" failed"))
		return;

	tokenItem = SRelationHead ( tokenRel, error );
	if (S_CHK_ERR(error, S_CONTERR,
			  "Run",
			  "Call to \"SRelationHead\" failed"))
		return;

	prosSymbols = castSelf->prosSymbols;

	while ( tokenItem != NULL )
	{
		wordItem = SItemDaughter ( tokenItem, error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemDaughter\" failed"))
			return;

		if ( wordItem != NULL )
		{
			SRelationAppend ( boundariesRel, wordItem, error );
			if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SRelationAppend\" failed"))
				return;
		}

		applyRules ( tokenItem, boundariesRel, prosSymbols, error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"applyRules\" failed"))
			return;

		tokenItem = SItemNext (tokenItem, error );
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemNext\" failed"))
			return;
	}

	/* error cleanup */

quit_error:
	return;


	S_UNUSED(self);
}


/************************************************************************************/
/*                                                                                  */
/* SRuleBasedEndtoneUttProc class initialization                                   */
/*                                                                                  */
/************************************************************************************/

static SRuleBasedEndtoneUttProcClass RuleBasedEndtoneUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:SRuleBasedEndtoneUttProc",
		sizeof(SRuleBasedEndtoneUttProc),
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
