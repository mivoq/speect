/************************************************************************************/
/* Copyright (c) 2008-2011 The Department of Arts and Culture,                      */
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
/* DATE    : 7 November 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Extract items, item features and item functions by navigating given paths on     */
/* the HRG structure.                                                               */
/*                                                                                  */
/* Loosely based on cst_ffunction of Flite, http://cmuflite.org (1.3)               */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*  Flite license, cst_ffunction                                                    */
/*                                                                                  */
/*                       Language Technologies Institute                            */
/*                          Carnegie Mellon University                              */
/*                             Copyright (c) 1999                                   */
/*                             All Rights Reserved.                                 */
/*                                                                                  */
/*       Permission is hereby granted, free of charge, to use and distribute        */
/*       this software and its documentation without restriction, including         */
/*       without limitation the rights to use, copy, modify, merge, publish,        */
/*       distribute, sublicense, and/or sell copies of this work, and to            */
/*       permit persons to whom this work is furnished to do so, subject to         */
/*       the following conditions:                                                  */
/*        1. The code must retain the above copyright notice, this list of          */
/*           conditions and the following disclaimer.                               */
/*        2. Any modifications must be clearly marked as such.                      */
/*        3. Original authors' names are not deleted.                               */
/*        4. The authors' names are not used to endorse or promote products         */
/*           derived from this software without specific prior written              */
/*           permission.                                                            */
/*                                                                                  */
/*       CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK               */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE            */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author:  Alan W Black                                                      */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "base/strings/utf8.h"
#include "hrg/item_path.h"
#include "hrg/utterance.h"
#include "hrg/processors/featprocessor.h"
#include "voicemanager/voice.h"


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

typedef enum
{
	S_IPATH_TOK_ERROR,
	S_IPATH_TOK_PREVIOUS,
	S_IPATH_TOK_NEXT,
	S_IPATH_TOK_RELATION,
	S_IPATH_TOK_DAUGHTER,
	S_IPATH_TOK_DAUGHTERN,
	S_IPATH_TOK_PARENT,
	S_IPATH_TOK_UNKNOWN
} s_ipath_tok;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_ipath_tok s_get_token(const char **path_ptr,
							   const char *set_ptr,
							   char **token, s_erc *error);

static void *s_internal_ff(const SItem *item, const char *path, int type, s_erc *error);

static SObject *s_feature_processor_request(const SItem *item, const char *name,
											s_erc *error);

static SObject *s_internal_ff_clever(const SItem *item, const char *path,
									 const SFeatProcessor *featProc, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/


S_API const SItem *SItemPathToItem(const SItem *item, const char *path, s_erc *error)
{
	const SItem *pItem;


	S_CLR_ERR(error);
	pItem = (const SItem*)s_internal_ff(item, path, 2, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SItemPathToItem",
			  "Call to \"s_internal_ff\" failed");

	return pItem;
}


S_API const SObject *SItemPathToFeature(const SItem *item, const char *path, s_erc *error)
{
	const SObject *feature;


	S_CLR_ERR(error);
	feature = (const SObject*)s_internal_ff(item, path, 1, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SItemPathToFeature",
			  "Call to \"s_internal_ff\" failed");

	return feature;
}


S_API SObject *SItemPathToFeatProc(const SItem *item, const char *path, s_erc *error)
{
	SObject *derivedFeature;


	S_CLR_ERR(error);
	derivedFeature = (SObject*)s_internal_ff(item, path, 0, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SItemPathToFeatProc",
			  "Call to \"s_internal_ff\" failed");

	return derivedFeature;
}


S_API SObject *SItemPath(const SItem *item, const char *path, s_erc *error)
{
	SObject *pObject;
	const char *end;
	const char *fobject;
	const SVoice *voice;
	const SFeatProcessor *featProc;
	int rv;


	S_CLR_ERR(error);
	if (path == NULL)
		return NULL;

	/* get last part of path, after . */
	end = s_strrchr(path, '.', error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemPath",
				  "Call to \"s_strrchr\" failed"))
		return NULL;

	if (end != NULL)
		fobject = end + 1;
	else
		fobject = path;


	/* get item's voice */
	voice = SItemVoice(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemPath",
				  "Call to \"SItemVoice\" failed"))
		return NULL;

	if (voice == NULL)
		return NULL;

	rv = s_strncmp(fobject, "ph_", 3, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemPath",
				  "Call to \"s_strncmp\" failed"))
		return NULL;

	if (rv == 0)
	{
		/* special case of phoneset feature processor */
		featProc = SVoiceGetFeatProc(voice, "segment_phoneset_feature", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SItemPath",
					  "Call to \"SVoiceGetFeatProc\" failed"))
			return NULL;
	}
	else
	{
		/* get feature processor */
		featProc = SVoiceGetFeatProc(voice, fobject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SItemPath",
					  "Call to \"SVoiceGetFeatProc\" failed"))
			return NULL;
	}

	pObject = s_internal_ff_clever(item, path, featProc, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemPath",
				  "Call to \"s_internal_ff_clever\" failed"))
		return NULL;

	if (pObject == NULL)
		return NULL;

	/* does this object have a dispose method ?*/
	if (S_OBJECT_METH_VALID(S_OBJECT(pObject), dispose))
	{
		/*
		 * we must increment it's reference so that it can
		 * safely be deleted with S_DELETE by the caller
		 */
		SObjectIncRef(pObject);
	}

	/* if no dispose method, then we don't need to worry */
	return pObject;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_ipath_tok s_get_token(const char **path_ptr, const char *set_ptr,
							   char **token, s_erc *error)
{
	int token_size;
	int tokcompare;


	S_CLR_ERR(error);

	if (set_ptr == NULL)
	{
		/* reached end */
		*token = s_strdup(*path_ptr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_token",
					  "Call to \"s_strdup\" failed"))
			return S_IPATH_TOK_ERROR;
	}
	else
	{
		token_size = s_strsize(*path_ptr, error) - s_strsize(set_ptr, error) + 1;
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_token",
					  "Call to \"s_strsize\" failed"))
			return S_IPATH_TOK_ERROR;

		if (token_size <= 1)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "s_get_token",
					  "token_size < 1");
			return S_IPATH_TOK_ERROR;
		}

		*token = S_MALLOC(char, token_size);
		if (*token == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "s_get_token",
					  "Failed to allocate memory for token");
			return S_IPATH_TOK_ERROR;
		}

		*token = s_strncpy(*token, *path_ptr, token_size - 1, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_token",
					  "Call to \"s_strncpy\" failed"))
			return S_IPATH_TOK_ERROR;

		*path_ptr = *path_ptr + s_strsize(*token, error) + 1;
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_token",
					  "Call to \"s_strsize\" failed"))
			return S_IPATH_TOK_ERROR;

	}

	/* previous */
	tokcompare = s_strcmp(*token, "p", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_token",
				  "Call to \"s_strcmp\" failed"))
		return S_IPATH_TOK_ERROR;
	else if (tokcompare == 0)
		return S_IPATH_TOK_PREVIOUS;

	/* next */
	tokcompare = s_strcmp(*token, "n", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_token",
				  "Call to \"s_strcmp\" failed"))
		return S_IPATH_TOK_ERROR;
	else if (tokcompare == 0)
		return S_IPATH_TOK_NEXT;

	/* relation */
	tokcompare = s_strcmp(*token, "R", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_token",
				  "Call to \"s_strcmp\" failed"))
		return S_IPATH_TOK_ERROR;
	else if (tokcompare == 0)
		return S_IPATH_TOK_RELATION;

	/* daughter */
	tokcompare = s_strcmp(*token, "daughter", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_token",
				  "Call to \"s_strcmp\" failed"))
		return S_IPATH_TOK_ERROR;
	else if (tokcompare == 0)
		return S_IPATH_TOK_DAUGHTER;

	/* daughtern */
	tokcompare = s_strcmp(*token, "daughtern", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_token",
				  "Call to \"s_strcmp\" failed"))
		return S_IPATH_TOK_ERROR;
	else if (tokcompare == 0)
		return S_IPATH_TOK_DAUGHTERN;

	/* parent */
	tokcompare = s_strcmp(*token, "parent", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_token",
				  "Call to \"s_strcmp\" failed"))
		return S_IPATH_TOK_ERROR;
	else if (tokcompare == 0)
		return S_IPATH_TOK_PARENT;

	/* unknown */
	return S_IPATH_TOK_UNKNOWN;
}


static SObject *s_feature_processor_request(const SItem *item, const char *name, s_erc *error)
{
	SObject *featureVal;
	const SVoice *voice;
	const SFeatProcessor *featProc;


	S_CLR_ERR(error);

	/* get item's voice */
	voice = SItemVoice(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_feature_processor_request",
				  "Call to \"SItemVoice\" failed"))
		return NULL;

	if (voice == NULL)
		return NULL;

	/* get feature processor */
	featProc = SVoiceGetFeatProc(voice, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_feature_processor_request",
				  "Call to \"SVoiceGetFeatProc\" failed"))
		return NULL;

	if (featProc == NULL)
		return NULL;

	/* execute feature processor on item */
	featureVal = SFeatProcessorRun(featProc, item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_feature_processor_request",
				  "Call to \"SFeatProcessorRun\" failed"))
		return NULL;

	return featureVal;
}


static void *s_internal_ff(const SItem *item, const char *path, int type, s_erc *error)
{
	const char *set_ptr = path;
	const char *set = ".:";
	char *token;
	SItem *pathItem = (SItem*)item;
	SItem *itr;
	void *void_v;
	s_ipath_tok token_state;
	const SObject *featureVal = NULL;
	s_bool feat_present;


	S_CLR_ERR(error);

	while ((path != NULL) && (set_ptr != NULL) && (pathItem != NULL))
	{
		set_ptr = s_strpbrk(path, set, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_internal_ff",
					  "Call to \"s_strpbrk\" failed"))
		{
			S_FREE(token);
			return NULL;
		}

		token_state = s_get_token(&path, set_ptr, &token, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_internal_ff",
					  "Call to \"s_get_token\" failed"))
		{
			S_FREE(token);
			return NULL;
		}

		switch (token_state)
		{
		case S_IPATH_TOK_ERROR:
		{
			S_CTX_ERR(error, S_CONTERR,
					  "s_get_token",
					  "Tokenize error in features path \"%s\"", path);
			S_FREE(token);
			return NULL;
		}
		case S_IPATH_TOK_PREVIOUS:
		{
			if (pathItem != NULL)
			{
				pathItem = SItemPrev(pathItem, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_internal_ff",
							  "Call to \"SItemPrev\" failed"))
				{
					S_FREE(token);
					return NULL;
				}
			}
			else
				S_WARNING(S_FAILURE, "s_internal_ff",
						  "Trying to get previous item of NULL item");
			break;
		}
		case S_IPATH_TOK_NEXT:
		{
			if (pathItem != NULL)
			{
				pathItem = SItemNext(pathItem, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_internal_ff",
							  "Call to \"SItemNext\" failed"))
				{
					S_FREE(token);
					return NULL;
				}
			}

			else
				S_WARNING(S_FAILURE, "s_internal_ff",
						  "Trying to get next item of NULL item");
			break;
		}
		case S_IPATH_TOK_RELATION:
		{
			S_FREE(token);

			if (set_ptr == NULL)
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff",
						  "Tokeninze error, expected relation"
						  " name in features path \"%s\"", path);
				return NULL;
			}

			set_ptr = s_strpbrk(path, set, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_internal_ff",
						  "Call to \"s_strpbrk\" failed"))
				return NULL;

			token_state = s_get_token(&path, set_ptr, &token, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_internal_ff",
						  "Call to \"s_get_token\" failed"))
			{
				S_FREE(token);
				return NULL;
			}

			/* relation names are not known to tokenizer */
			if (token_state != S_IPATH_TOK_UNKNOWN)
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff",
						  "Tokeninze error, expected relation"
						  " name in features path \"%s\"", path);
				S_FREE(token);
				return NULL;
			}

			if (pathItem != NULL)
			{
				pathItem = SItemAs(pathItem, token, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_internal_ff",
							  "Call to \"SItemAs\" failed"))
				{
					S_FREE(token);
					return NULL;
				}
			}
			else
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff",
						  "Trying to get NULL item in relation \"%s\"", token);
				S_FREE(token);
			}

			break;
		}
		case S_IPATH_TOK_DAUGHTER:
		{
			if (pathItem != NULL)
			{
				pathItem = SItemDaughter(pathItem, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_internal_ff",
							  "Call to \"SItemDaughter\" failed"))
				{
					return NULL;
				}
			}
			else
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff",
						  "Trying to get daughter of NULL item.", token);
			}

			break;
		}
		case S_IPATH_TOK_DAUGHTERN:
		{
			if (pathItem != NULL)
			{
				pathItem = SItemDaughter(pathItem, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_internal_ff",
							  "Call to \"SItemDaughter\" failed"))
				{
					return NULL;
				}
				/* now last one */
				for (itr = pathItem; itr != NULL; itr = itr->next)
					pathItem = itr;

			}
			else
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff",
						  "Trying to get daughter of NULL item.", token);
			}

			break;
		}
		case S_IPATH_TOK_PARENT:
		{
			if (pathItem != NULL)
			{
				pathItem = SItemParent(pathItem, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_internal_ff",
							  "Call to \"SItemParent\" failed"))
				{
					return NULL;
				}
			}
			else
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff",
						  "Trying to get parent of NULL item.", token);
			}

			break;
		}
		case S_IPATH_TOK_UNKNOWN:
		{
			if (pathItem != NULL)
			{
				/* feature function */
				if (type == 0)
				{
					featureVal = (const SObject*)s_feature_processor_request(pathItem, token, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_internal_ff",
								  "Call to \"s_feature_function_request\" failed"))
						featureVal = NULL;
				}
				/* item feature */
				else if (type == 1)
				{
					/* item feature */
					feat_present = SItemFeatureIsPresent(pathItem, token, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_internal_ff",
								  "Call to \"SItemFeatureIsPresent\" failed"))
					{
						featureVal = NULL;
					}
					else if (feat_present == TRUE)
					{
						featureVal = (const SObject*)SItemGetObject(pathItem, token, error);
						if (S_CHK_ERR(error, S_CONTERR,
									  "s_internal_ff",
									  "Call to \"SItemGetObject\" failed"))
							featureVal = NULL;
					}
				}
			}
			else
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff",
						  "Item in path \"%s\" is NULL", path);
			}
			break;
		}
		default:
		{
			S_CTX_ERR(error, S_CONTERR,
					  "s_internal_ff",
					  "Tokeninze error, should not get to this state");
			S_FREE(token);
			return NULL;
		}
		}

		S_FREE(token);
	}

	if (type == 2)
		void_v = (void*)pathItem;
	else if ((type == 0) || (type == 1))
		void_v = (void*)featureVal;
	else
		void_v = NULL;

	return void_v;
}



static SObject *s_internal_ff_clever(const SItem *item, const char *path,
									 const SFeatProcessor *featProc, s_erc *error)

{
	const char *set_ptr = path;
	const char *set = ".:";
	char *token;
	SItem *pathItem = (SItem*)item;
	SItem *itr;
	void *void_v = NULL;
	s_ipath_tok token_state;
	SObject *featureVal = NULL;
	s_bool feat_present;
	int type = 1; /* 1 is pathItem, 0 is featureVal */


	S_CLR_ERR(error);

	while ((path != NULL) && (set_ptr != NULL) && (pathItem != NULL))
	{
		set_ptr = s_strpbrk(path, set, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_internal_ff_clever",
					  "Call to \"s_strpbrk\" failed"))
		{
			S_FREE(token);
			return NULL;
		}

		token_state = s_get_token(&path, set_ptr, &token, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_internal_ff_clever",
					  "Call to \"s_get_token\" failed"))
		{
			S_FREE(token);
			return NULL;
		}

		switch (token_state)
		{
		case S_IPATH_TOK_ERROR:
		{
			S_CTX_ERR(error, S_CONTERR,
					  "s_get_token",
					  "Tokenize error in features path \"%s\"", path);
			S_FREE(token);
			return NULL;
		}
		case S_IPATH_TOK_PREVIOUS:
		{
			if (pathItem != NULL)
			{
				pathItem = SItemPrev(pathItem, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_internal_ff_clever",
							  "Call to \"SItemPrev\" failed"))
				{
					S_FREE(token);
					return NULL;
				}
			}
			else
				S_WARNING(S_FAILURE, "s_internal_ff_clever",
						  "Trying to get previous item of NULL item");
			break;
		}
		case S_IPATH_TOK_NEXT:
		{
			if (pathItem != NULL)
			{
				pathItem = SItemNext(pathItem, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_internal_ff_clever",
							  "Call to \"SItemNext\" failed"))
				{
					S_FREE(token);
					return NULL;
				}
			}

			else
				S_WARNING(S_FAILURE, "s_internal_ff_clever",
						  "Trying to get next item of NULL item");
			break;
		}
		case S_IPATH_TOK_RELATION:
		{
			S_FREE(token);

			if (set_ptr == NULL)
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff_clever",
						  "Tokeninze error, expected relation"
						  " name in features path \"%s\"", path);
				return NULL;
			}

			set_ptr = s_strpbrk(path, set, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_internal_ff_clever",
						  "Call to \"s_strpbrk\" failed"))
				return NULL;

			token_state = s_get_token(&path, set_ptr, &token, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_internal_ff_clever",
						  "Call to \"s_get_token\" failed"))
			{
				S_FREE(token);
				return NULL;
			}

			/* relation names are not known to tokenizer */
			if (token_state != S_IPATH_TOK_UNKNOWN)
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff_clever",
						  "Tokeninze error, expected relation"
						  " name in features path \"%s\"", path);
				S_FREE(token);
				return NULL;
			}

			if (pathItem != NULL)
			{
				pathItem = SItemAs(pathItem, token, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_internal_ff_clever",
							  "Call to \"SItemAs\" failed"))
				{
					S_FREE(token);
					return NULL;
				}
			}
			else
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff_clever",
						  "Trying to get NULL item in relation \"%s\"", token);
				S_FREE(token);
			}

			break;
		}
		case S_IPATH_TOK_DAUGHTER:
		{
			if (pathItem != NULL)
			{
				pathItem = SItemDaughter(pathItem, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_internal_ff_clever",
							  "Call to \"SItemDaughter\" failed"))
				{
					return NULL;
				}
			}
			else
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff_clever",
						  "Trying to get daughter of NULL item.", token);
			}

			break;
		}
		case S_IPATH_TOK_DAUGHTERN:
		{
			if (pathItem != NULL)
			{
				pathItem = SItemDaughter(pathItem, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_internal_ff_clever",
							  "Call to \"SItemDaughter\" failed"))
				{
					return NULL;
				}
				/* now last one */
				for (itr = pathItem; itr != NULL; itr = itr->next)
					pathItem = itr;

			}
			else
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff_clever",
						  "Trying to get daughter of NULL item.", token);
			}

			break;
		}
		case S_IPATH_TOK_PARENT:
		{
			if (pathItem != NULL)
			{
				pathItem = SItemParent(pathItem, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_internal_ff_clever",
							  "Call to \"SItemParent\" failed"))
				{
					return NULL;
				}
			}
			else
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff_clever",
						  "Trying to get parent of NULL item.", token);
			}

			break;
		}
		case S_IPATH_TOK_UNKNOWN:
		{
			if (pathItem != NULL)
			{
				type = 0;

				/* feature function */
				if (featProc != NULL)
				{
					/* maybe a phoneset feature ? */
					int rv;

					rv = s_strncmp(token, "ph_", 3, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_internal_ff_clever",
								  "Call to \"s_strncmp\" failed"))
					{
						featureVal = NULL;
					}
					else
					{
						if (rv == 0)
						{
							/* phoneset feature */
							SItemSetString(pathItem, "_phoneset_feature", token, error);
							if (S_CHK_ERR(error, S_CONTERR,
										  "s_internal_ff_clever",
										  "Call to \"SItemSetString\" failed"))
							{
								featureVal = NULL;
							}
							else
							{
								featureVal = SFeatProcessorRun(featProc, pathItem, error);
								if (S_CHK_ERR(error, S_CONTERR,
											  "s_internal_ff_clever",
											  "Call to \"SFeatProcessorRun\" failed"))
									featureVal = NULL;

								SItemDelFeature(pathItem, "_phoneset_feature", error);
								S_CHK_ERR(error, S_CONTERR,
										  "s_internal_ff_clever",
										  "Call to \"SItemDelFeature\" failed");
							}
						}
						else
						{
							/* execute feature processor on item */
							featureVal = SFeatProcessorRun(featProc, pathItem, error);
							if (S_CHK_ERR(error, S_CONTERR,
										  "s_internal_ff_clever",
										  "Call to \"SFeatProcessorRun\" failed"))
								featureVal = NULL;
						}
					}
				}
				/*  item feature */
				else
				{
					feat_present = SItemFeatureIsPresent(pathItem, token, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_internal_ff_clever",
								  "Call to \"SItemFeatureIsPresent\" failed"))
					{
						featureVal = NULL;
					}
					else if (feat_present == TRUE)
					{
						featureVal = (SObject*)SItemGetObject(pathItem, token, error);
						if (S_CHK_ERR(error, S_CONTERR,
									  "s_internal_ff_clever",
									  "Call to \"SItemGetObject\" failed"))
							featureVal = NULL;
					}
				}
			}
			else
			{
				S_CTX_ERR(error, S_CONTERR,
						  "s_internal_ff_clever",
						  "Item in path \"%s\" is NULL", path);
			}
			break;
		}
		default:
		{
			S_CTX_ERR(error, S_CONTERR,
					  "s_internal_ff_clever",
					  "Tokeninze error, should not get to this state");
			S_FREE(token);
			return NULL;
		}
		}

		S_FREE(token);
	}

	if (type == 1)
		void_v = (void*)pathItem;
	else if (type == 0)
		void_v = (void*)featureVal;
	else
		void_v = NULL;

	return void_v;
}
