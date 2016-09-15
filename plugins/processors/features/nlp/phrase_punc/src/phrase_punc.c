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
/* Phrase position in utterance feature processor.                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "phrase_punc.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SPhrasePuncFeatProcClass PhrasePuncFeatProcClass;

/* SPhrasePuncFeatProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_phrase_punc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&PhrasePuncFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phrase_punc_class_reg",
			  "Failed to register SPhrasePuncFeatProcClass");
}


S_LOCAL void _s_phrase_punc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&PhrasePuncFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phrase_punc_class_free",
			  "Failed to free SPhrasePuncFeatProcClass");
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

static void Initialize(SFeatProcessor *self, SMap* features, s_erc *error)
{
	S_CLR_ERR(error);

	SPhrasePuncFeatProc *castSelf = S_CAST(self, SPhrasePuncFeatProc, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to S_CAST failed"))
		goto quit_error;

	castSelf->symbols =  S_CAST( SMapGetObject(features , "list punctuation", error),
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
	/* Search for the punctuation in the SMap, using it as a Key */
	s_bool found= SMapObjectPresent(map, str, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "searchStringMap",
			  "Call to \"SMapObjectPresent\" failed"))
		return FALSE;

	return found;
}


static char* setSentenceType(SItem *phrase, SMap *puncMap, s_erc *error)
{
	S_CLR_ERR(error);

	char* result = "0";

	/* types: "decl, "excl", "interrog" */
	/* stop at sentence's last token */
	const SItem *wordFromCurrentPhrase = SItemPathToItem(phrase, "daughtern", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setSentenceType",
		      "Call to \"SItemPathToItem\" failed"))
		return "";

	SItem *wordAsToken = SItemAs(wordFromCurrentPhrase, "Token", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setSentenceType",
		      "Call to \"SItemAs\" failed"))
		return "";

	SItem *tokenItem = SItemParent(wordAsToken, error);
	SItem *firstTokenItem = tokenItem;
	tokenItem = SItemNext(tokenItem, error);

	s_bool isPunct = SItemFeatureIsPresent(tokenItem, "IsPunctuation", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "setSentenceType",
		      "Call to \"SItemFeatureIsPresent\" failed"))
		return "";

	const char *punctStr = SItemGetName(tokenItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "setSentenceType",
			  "Call to \"SItemGetName\" failed"))
		return "";

	s_bool found = searchStringMap(puncMap, punctStr, error);
	if (S_CHK_ERR(error, S_CONTERR,
			  "setSentenceType",
			  "Call to \"searchStringMap\" failed"))
		return "";

	result = punctStr;


	if( found == TRUE)
	{
		result = SMapGetString ( puncMap, punctStr, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "setSentenceType",
				  "Call to \"SMapGetString\" failed"))
			return "";
	}
	else
	{
		result = "0";
	}

	return result;
}

static SObject *Run(const SFeatProcessor *self, const SItem *item,
					s_erc *error)
{
	SObject *extractedFeat = NULL;
	char* type;

	SPhrasePuncFeatProc *castSelf = S_CAST(self, SPhrasePuncFeatProc, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Initialize",
		      "Call to S_CAST failed"))
		goto quit_error;

	type = setSentenceType(item, castSelf->symbols, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SetSentenceType\" failed"))
		goto quit_error;

	if( type == "" )
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
/* SPhrasePuncFeatProc class initialization                                         */
/*                                                                                  */
/************************************************************************************/

static SPhrasePuncFeatProcClass PhrasePuncFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SPhrasePuncFeatProc",
		sizeof(SPhrasePuncFeatProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SFeatProcessorClass */
	Initialize,          /* initialize */
	Run                  /* run     */
};
