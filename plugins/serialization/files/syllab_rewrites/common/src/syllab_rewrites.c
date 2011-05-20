/************************************************************************************/
/* Copyright (c) 2011 The Department of Arts and Culture,                           */
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
/* DATE    : May 2011                                                               */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A syllabification rewrites class implementation.                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "syllab_rewrites_rule.h"
#include "syllab_rewrites.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SSyllabificationRewrites class declaration. */
static SSyllabificationRewritesClass SyllabificationRewritesClass;


/************************************************************************************/
/*                                                                                  */
/*  Static function prototypes                                                      */
/*                                                                                  */
/************************************************************************************/

static s_str_list *s_copy_str_list_from_SList(const SList *sList, s_erc *error);

static s_str_list *run_rewrite_rules(const SSyllabificationRewrites *self,
									 const SList *phoneList,
									 const char *syllable_boundary_symbol,
									 s_erc *error);

static void replace_B(const s_str_list *B, s_str_list *A,
					  const char *syllable_boundary_symbol, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_syllabification_rewrites_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&SyllabificationRewritesClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syllabification_rewrites_class_reg",
			  "Failed to register SSyllabificationRewritesClass");
}


S_LOCAL void _s_syllabification_rewrites_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&SyllabificationRewritesClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syllabification_rewrites_class_free",
			  "Failed to free SSyllabificationRewritesClass");
}

/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void replace_B(const s_str_list *B, s_str_list *A,
					  const char *syllable_boundary_symbol, s_erc *error)
{
	const s_str_list_element *itr_A;
	const s_str_list_element *itr_B;


	itr_A = s_str_list_first(A, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "replace_B",
				  "Call to \"s_str_list_first\" failed"))
		return;

	itr_B = s_str_list_first(B, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "replace_B",
				  "Call to \"s_str_list_first\" failed"))
		return;

	while ((itr_A != NULL) && (itr_B != NULL))
	{
		const char *element_B;
		int rv;


		element_B = s_str_list_element_get(itr_B, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "replace_B",
					  "Call to \"s_str_list_element_get\" failed"))
			return;

		rv = s_strcmp(element_B, syllable_boundary_symbol, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "replace_B",
					  "Call to \"s_strcmp\" failed"))
			return;

		if (rv == 0) /* match */
		{
			itr_A = s_str_list_insert_before((s_str_list_element*)itr_A,
											 syllable_boundary_symbol,
											 error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "replace_B",
						  "Call to \"s_str_list_insert_before\" failed"))
				return;
		}

		itr_A = s_str_list_element_next(itr_A, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "replace_B",
					  "Call to \"s_str_list_element_next\" failed"))
			return;

		itr_B = s_str_list_element_next(itr_B, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "replace_B",
					  "Call to \"s_str_list_element_next\" failed"))
			return;
	}

	if (itr_B != NULL)
	{
		/* can only have one more boundary */
		s_str_list_append(A, syllable_boundary_symbol, error);
		S_CHK_ERR(error, S_CONTERR,
				  "replace_B",
				  "Call to \"s_str_list_append\" failed");
	}
}


static s_str_list *run_rewrite_rules(const SSyllabificationRewrites *self,
									 const SList *phoneList,
									 const char *syllable_boundary_symbol,
									 s_erc *error)
{
	s_str_list *LC = NULL;
	s_str_list *RC = NULL;
	s_str_list *otape = NULL;
	s_str_list *newRC = NULL;
	s_str_list *tmpLC = NULL;
	s_str_list *A = NULL;
	const s_str_list *B = NULL;
	uint32 rc_size;
	uint32 newrc_size;
	s_bool found_rule;
	SIterator *itr = NULL;


	S_CLR_ERR(error);

	/* create left context and add a # */
	LC = s_str_list_new(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "run_rewrite_rules",
				  "Call to \"s_str_list_new\" failed"))
		goto error_return;

	s_str_list_append(LC, "#", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "run_rewrite_rules",
				  "Call to \"s_str_list_append\" failed"))
		goto error_return;

	/* copy right context from phone-list and add # */
	RC = s_copy_str_list_from_SList(phoneList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "rewrite",
				  "Call to \"s_copy_str_list_from_SList\" failed"))
		goto error_return;

	s_str_list_append(RC, "#", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "run_rewrite_rules",
				  "Call to \"s_str_list_append\" failed"))
		goto error_return;

	/* create output tape */
	otape = s_str_list_new(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "run_rewrite_rules",
				  "Call to \"s_str_list_new\" failed"))
		goto error_return;

	rc_size = s_str_list_size(RC, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "run_rewrite_rules",
				  "Call to \"s_str_list_size\" failed"))
		goto error_return;

	while (rc_size > 1)
	{
		const char *RC0;
		const SList *ruleList;
		const SObject *tmp;


		RC0 = s_str_list_nth_string(RC, 0, error); /* RC[0] */

		/* get the list of rules for this phone */
		tmp = SMapGetObjectDef(self->rules, RC0, NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_rewrite_rules",
					  "Call to \"SMapGetObjectDef\" failed"))
			goto error_return;

		if (tmp == NULL) /* no rules found !!! */
		{
			S_CTX_ERR(error, S_FAILURE,
					  "run_rewrite_rules",
					  "No rewrite rules for phone '%s'", RC0);
			goto error_return;
		}

		ruleList = S_CAST(tmp, SList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_rewrite_rules",
					  "Call to \"S_CAST(SList)\" failed"))
			goto error_return;

		/* get iterator to rules */
		itr = S_ITERATOR_GET(ruleList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_rewrite_rules",
					  "Call to \"S_ITERATOR_GET\" failed"))
			goto error_return;

		found_rule = FALSE;

		/* search through rewrite rules to find matching one */
		for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
		{
			const SSyllabificationRewritesRule *rule;


			/* get the rule */
			rule = (SSyllabificationRewritesRule*)SIteratorObject(itr, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "run_rewrite_rules",
						  "Call to \"SIteratorObject\" failed"))
				goto error_return;

			/* try to match the rule */
			newRC = SSyllabificationRewritesRuleMatches(rule, LC, RC, &A,
														self->sets, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "run_rewrite_rules",
						  "Call to \"SSyllabificationRewritesRuleMatches\" failed"))
				goto error_return;

			if (newRC != NULL)
			{
				B = SSyllabificationRewritesRuleGetReplacement(rule, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "run_rewrite_rules",
							  "Call to \"SSyllabificationRewritesRuleGetReplacement\" failed"))
					goto error_return;

				found_rule = TRUE;
				break;
			}
		}

		if (!found_rule)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "run_rewrite_rules",
					  "Failed to find a matching rule for phone '%s'", RC0);
			goto error_return;
		}
		else
		{
			S_DELETE(itr, "run_rewrite_rules", error);
		}


		replace_B(B, A, syllable_boundary_symbol, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_rewrite_rules",
					  "Call to \"replace_B\" failed"))
			goto error_return;

		s_str_list_merge(otape, A, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_rewrite_rules",
					  "Call to \"s_str_list_merge\" failed"))
			goto error_return;

		s_str_list_delete(A, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_rewrite_rules",
					  "Call to \"s_str_list_delete\" failed"))
			goto error_return;

		A = NULL;

		newrc_size = s_str_list_size(newRC, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_rewrite_rules",
					  "Call to \"s_str_list_size\" failed"))
			goto error_return;

		tmpLC = s_str_list_slice(RC, 0, (rc_size-newrc_size), error); /* RC[0:len(RC) - len(newRC)] */
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_rewrite_rules",
					  "Call to \"s_str_list_slice\" failed"))
			goto error_return;

		s_str_list_merge(LC, tmpLC, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_rewrite_rules",
					  "Call to \"s_str_list_merge\" failed"))
			goto error_return;

		s_str_list_delete(tmpLC, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_rewrite_rules",
					  "Call to \"s_str_list_delete\" failed"))
			goto error_return;

		tmpLC = NULL;

		s_str_list_delete(RC, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_rewrite_rules",
					  "Call to \"s_str_list_delete\" failed"))
			goto error_return;

		RC = newRC;
		newRC = NULL;

		rc_size = newrc_size;
	}

	/* cleanup */
	{
		s_erc local_err = S_SUCCESS;

		if (LC != NULL)
			s_str_list_delete(LC, &local_err);

		if (RC != NULL)
			s_str_list_delete(RC, &local_err);
	}

	/* all OK, no errors */
	return otape;

/* errors here, clean up */
error_return:
	{
		s_erc local_err = S_SUCCESS;

		if (LC != NULL)
			s_str_list_delete(LC, &local_err);

		if (RC != NULL)
			s_str_list_delete(RC, &local_err);

		if (newRC != NULL)
			s_str_list_delete(newRC, &local_err);

		if (otape != NULL)
			s_str_list_delete(otape, &local_err);

		if (tmpLC != NULL)
			s_str_list_delete(tmpLC, &local_err);

		if (A != NULL)
			s_str_list_delete(A, &local_err);

		if (itr != NULL)
			S_DELETE(itr, "run_rewrite_rules", &local_err);
	}

	return NULL;
}


static s_str_list *s_copy_str_list_from_SList(const SList *sList, s_erc *error)
{
	s_str_list *string_list = NULL;
	SIterator *itr = NULL;


	S_CLR_ERR(error);
	itr = S_ITERATOR_GET(sList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_copy_str_list_from_SList",
				  "Failed to get iterator to list"))
		return NULL;

	string_list = s_str_list_new(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_copy_str_list_from_SList",
				  "Failed to get iterator to list"))
		goto error_return;

	/* iterate through list objects and add them to string list */
	for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
	{
		const SObject *tmp;


		tmp = SIteratorObject(itr, error);
		if (S_CHK_ERR(error,  S_CONTERR,
					  "s_copy_str_list_from_SList",
					  "Failed to get list iterator object"))
			goto error_return;

		s_str_list_append(string_list,
						  SObjectGetString(tmp, error),
						  error);
		if (S_CHK_ERR(error,  S_CONTERR,
					  "s_copy_str_list_from_SList",
					  "Call to \"SObjectGetString/s_str_list_append\" failed"))
			goto error_return;
	}

	/* all OK */
	return string_list;

	/* error, clean up */
error_return:
	{
		s_erc local_err = S_SUCCESS;

		if (itr != NULL)
			S_DELETE(itr, "main", &local_err);

		if (string_list != NULL)
			s_str_list_delete(string_list, &local_err);
	}

	return NULL;
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SSyllabificationRewrites *self = obj;


	S_CLR_ERR(error);
	self->rules = NULL;
	self->sets = NULL;
}


static void Destroy(void *obj, s_erc *error)
{
	SSyllabificationRewrites *self = obj;


	S_CLR_ERR(error);
	if (self->rules != NULL)
		S_DELETE(self->rules, "Destroy", error);

	if (self->sets != NULL)
		S_DELETE(self->sets, "Destroy", error);
}


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
	s_str_list *string_list = NULL;
	SList *syl = NULL;
	SList *syllables = NULL;
	s_bool got_more_syllables;
	const s_str_list_element *itr;
	const char *syllable_boundary_symbol;
	const SObject *tmp;


	S_CLR_ERR(error);
	if (phoneList == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "Syllabify",
				  "Argument \"phoneList\" is NULL");
		goto error_return;
	}

	/* get syllable boundary symbol */
	tmp = SMapGetObjectDef(self->features, "syllable-boundary-symbol", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Call to \"SMapGetObjectDef\" failed"))
		goto error_return;

	if (tmp != NULL)
	{
		syllable_boundary_symbol = SObjectGetString(tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"SObjectGetString\" failed"))
			goto error_return;
	}
	else
	{
		syllable_boundary_symbol = "-"; /* defaults to "-" */
	}

	string_list = run_rewrite_rules(S_SYLLABIFICATIONREWRITES(self),
									phoneList, syllable_boundary_symbol,
									error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Call to \"run_rewrite_rules\" failed"))
		goto error_return;

	/* create a new syllable */
	syl = (SList*)S_NEW("SListList", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Failed to create new list"))
		goto error_return;

	/* create a new syllables list */
	syllables = (SList*)S_NEW("SListList", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Failed to create new list"))
		goto error_return;

	/* iterate over otape */
	itr = s_str_list_first(string_list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Call to \"s_str_list_first\" failed"))
		goto error_return;

	got_more_syllables = FALSE;
	while (itr != NULL)
	{
		const char *element;
		int rv;


		element = s_str_list_element_get(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"s_str_list_element_get\" failed"))
			goto error_return;

		rv = s_strcmp(element, syllable_boundary_symbol, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"s_strcmp\" failed"))
			goto error_return;

		if (rv != 0) /* element != "-" */
		{
			got_more_syllables = TRUE;
			SListPush(syl, SObjectSetString(element, error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"SObjectSetString/SListPush\" failed"))
				goto error_return;
		}
		else
		{
			/* syllable break, add syl */
			SListPush(syllables, S_OBJECT(syl), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Call to \"SListPush\" failed"))
				goto error_return;

			got_more_syllables = FALSE;

			/* create a new syl */
			syl = (SList*)S_NEW("SListList", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Syllabify",
						  "Failed to create new list"))
				goto error_return;
		}

		itr = s_str_list_element_next(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"s_str_list_element_next\" failed"))
			goto error_return;
	}

	s_str_list_delete(string_list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Syllabify",
				  "Call to \"s_str_list_delete\" failed"))
		goto error_return;

	if (got_more_syllables)
	{
		/* add last syl */
		SListPush(syllables, S_OBJECT(syl), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Syllabify",
					  "Call to \"SListPush\" failed"))
			goto error_return;
	}
	else
	{
		/* nothing in syl, delete it */
		S_DELETE(syl, "Syllabify", error);
	}

	/* all OK, no errors */
	return syllables;

	/* error occurred, clean up */
error_return:
	{
		s_erc local_err = S_SUCCESS;

		if (string_list != NULL)
			s_str_list_delete(string_list, &local_err);

		if (syl != NULL)
			S_DELETE(syl, "Syllabify", &local_err);

		if (syllables != NULL)
			S_DELETE(syllables, "Syllabify", &local_err);
	}

	return NULL;

	word = NULL; /* compiler noise about unused parameters */
}

/************************************************************************************/
/*                                                                                  */
/* SSyllabificationRewrites class initialization                                    */
/*                                                                                  */
/************************************************************************************/

/* same as SSyllabificationClass */
static SSyllabificationRewritesClass SyllabificationRewritesClass =
{
	/* SObjectClass */
	{
		"SSyllabification:SSyllabificationRewrites",
		sizeof(SSyllabificationRewrites),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
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
