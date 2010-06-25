/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* A lexicon class implementation with the lexicon entries in a SMap                */
/* structure read from a JSON format file. Inherits from SLexicon.                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "lexicon_json.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SLexiconJSONClass LexiconJSONClass; /* SLexiconJSON class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

const SList *get_matched_word_info(const SMap *singleEntry, s_bool *syllabified,
								   s_erc *error);

static const SList *get_word_info(const SObject *wordEntries, const SMap *features,
								  s_bool *syllabified, s_erc *error);

static s_bool check_if_match(const SMap *singleEntry, const SMap *features,
							 s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_lexicon_json_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&LexiconJSONClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_lexicon_json_class_reg",
			  "Failed to register SLexiconJSONClass");
}


S_LOCAL void _s_lexicon_json_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&LexiconJSONClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_lexicon_json_class_free",
			  "Failed to free SLexiconJSONClass");
}

/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_bool check_if_match(const SMap *singleEntry, const SMap *features, s_erc *error)
{
	SIterator *itr;


	S_CLR_ERR(error);

	itr = S_ITERATOR_GET(features, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "check_if_match",
				  "Call to \"S_ITERATOR_GET\" failed"))
		return FALSE;

	for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
	{
		const SObject *entryObject;
		const SObject *featuresObject;
		const char *key;
		s_bool match;


		key = SIteratorKey(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "check_if_match",
					  "Call to \"SIteratorKey\" failed"))
		{
			S_DELETE(itr, "check_if_match", error);
			return FALSE;
		}


		featuresObject = SIteratorObject(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "check_if_match",
					  "Call to \"SIteratorObject\" failed"))
		{
			S_DELETE(itr, "check_if_match", error);
			return FALSE;
		}

		/* object must be present */
		entryObject = SMapGetObjectDef(singleEntry, key, NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "check_if_match",
					  "Call to \"SMapGetObjectDef\" failed"))
		{
			S_DELETE(itr, "check_if_match", error);
			return FALSE;
		}

		if (entryObject == NULL)
		{
			S_DELETE(itr, "check_if_match", error);
			return FALSE;
		}

		match = SObjectCompare(featuresObject, entryObject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "check_if_match",
					  "Call to \"SObjectCompare\" failed"))
		{
			S_DELETE(itr, "check_if_match", error);
			return FALSE;
		}

		if (!match)
		{
			S_DELETE(itr, "check_if_match", error);
			return FALSE;
		}

		/* continue if this one matched */
	}

	/* if we get here then they matched */
	return TRUE;
}


const SList *get_matched_word_info(const SMap *singleEntry, s_bool *syllabified,
								   s_erc *error)
{
	const SObject *wordInfoObject;
	const SList *wordInfo;


	S_CLR_ERR(error);

	/* try to get syllables */
	wordInfoObject = SMapGetObjectDef(singleEntry, "syllables", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_matched_word_info",
				  "Call to \"SMapGetObjectDef\" failed"))
		return NULL;

	if (wordInfoObject != NULL)
	{
		/* make sure it's a list */
		wordInfo = S_CAST(wordInfoObject, SList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_matched_word_info",
					  "'syllables' entry for word is not a list"))
			return NULL;

		*syllabified = TRUE;
		return wordInfo;
	}

	/* no syllables, try to get phones */
	wordInfoObject = SMapGetObjectDef(singleEntry, "phones", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_matched_word_info",
				  "Call to \"SMapGetObjectDef\" failed"))
		return NULL;

	if (wordInfoObject != NULL)
	{
		/* make sure it's a list */
		wordInfo = S_CAST(wordInfoObject, SList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_matched_word_info",
					  "'phones' entry for word is not a list"))
			return NULL;

		*syllabified = FALSE;
		return wordInfo;
	}

	/* the word must have either phones or syllables */
	S_CTX_ERR(error, S_FAILURE,
			  "get_matched_word_info",
			  "Word entry does not have 'phones' or 'syllables' defined");
	return NULL;
}


static const SList *get_word_info(const SObject *wordEntries, const SMap *features,
								  s_bool *syllabified, s_erc *error)
{
	const SList *wordEntryList;
	SIterator *itr;
	s_bool matches;
	const SList *wordInfo;
	const SObject *singleEntryObject;
	const SMap *singleEntryMap;


	S_CLR_ERR(error);
	/* make sure that wordEntries is a list */
	wordEntryList = S_CAST(wordEntries, SList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_word_info",
				  "Retrieved lexicon entry for word is not a list object"))
		return NULL;

	itr = S_ITERATOR_GET(wordEntryList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_word_info",
				  "Call to \"S_ITERATOR_GET\" failed"))
		return NULL;

	for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
	{
		singleEntryObject = SIteratorObject(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_word_info",
					  "Call to \"SIteratorObject\" failed"))
		{
			S_DELETE(itr, "get_word_info", error);
			return NULL;
		}

		/* make sure singleEntryObject is a map */
		singleEntryMap = S_CAST(singleEntryObject, SMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_word_info",
					  "Word information must be a map"))
		{
			S_DELETE(itr, "get_word_info", error);
			return NULL;
		}

		if (features == NULL)
			goto found_match; /* we take the first entry */

		/* return first entry that matches the given features */
		matches = check_if_match(singleEntryMap, features, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_word_info",
					  "Call to \"check_if_match\" failed"))
		{
			S_DELETE(itr, "get_word_info", error);
			return NULL;
		}

		if (!matches)
			continue;
		else
			goto found_match;
	}

found_match:
	if (itr != NULL)
	{
		S_DELETE(itr, "get_word_info", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_word_info",
					  "Failed to delete iterator"))
			return NULL;
	}

	wordInfo = get_matched_word_info(singleEntryMap, syllabified, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_word_info",
				  "Call to \"get_matched_word_info\" failed"))
		return NULL;

	return wordInfo;
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SLexiconJSON *self = obj;


	S_CLR_ERR(error);
	self->entries = NULL;
}



static void Destroy(void *obj, s_erc *error)
{
	SLexiconJSON *self = obj;


	S_CLR_ERR(error);
	if (self->entries != NULL)
		S_DELETE(self->entries, "Destroy", error);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static const char *GetName(const SLexicon *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->name;
}


static const char *GetDescription(const SLexicon *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->description;
}


static const char *GetLanguage(const SLexicon *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->language;
}


static const char *GetLangCode(const SLexicon *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->lang_code;
}


static const s_version *GetVersion(const SLexicon *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const s_version*)&(self->info->version);
}

static const SObject *GetFeature(const SLexicon *self, const char *key,
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


static SList *GetWord(const SLexicon *self, const char *word,
					  const SMap *features, s_bool *syllabified,
					  s_erc *error)
{
	const SList *list;
	SList *listCopy;
	const SObject *wordEntries;
	const SLexiconJSON *lex = S_LEXICON_JSON(self);


	S_CLR_ERR(error);
	if (word == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetWord",
				  "Argument \"word\" is NULL");
		return NULL;
	}

	if (syllabified == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetWord",
				  "Argument \"syllabified\" is NULL");
		return NULL;
	}

	if (lex->entries == NULL)
		return NULL;

	/* get word entry, default NULL */
	wordEntries = SMapGetObjectDef(lex->entries, word, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetWord",
				  "Call to \"SMapGetObjectDef\" failed"))
		return NULL;

	if (wordEntries == NULL)
		return NULL; /* word not found */

	list = get_word_info(wordEntries, features, syllabified, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetWord",
				  "Failed to get phones/syllables for word '%s'",
				  word))
		return NULL;

	listCopy = SListCopy(NULL, list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetWord",
				  "Call to \"SListCopy\" failed"))
		return NULL;

	return listCopy;
}


/************************************************************************************/
/*                                                                                  */
/* SLexicon class initialization                                                    */
/*                                                                                  */
/************************************************************************************/

static SLexiconJSONClass LexiconJSONClass =
{
	/* SObjectClass */
	{
		"SLexicon:SLexiconJSON",
		sizeof(SLexiconJSON),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SLexiconClass */
	GetName,             /* get_name        */
	GetDescription,      /* get_description */
	GetLanguage,         /* get_language    */
	GetLangCode,         /* get_lang_code   */
	GetVersion,          /* get_version     */
	GetFeature,          /* get_feature     */
	GetWord              /* get_word        */
};
