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
/* A syllabification rewrites rule class implementation.                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "syllab_rewrites_rule.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SSyllabificationRewritesRule class declaration. */
static SSyllabificationRewritesRuleClass SyllabificationRewritesRuleClass;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_bool _item_match(const char *PATT, const char *THING, const SMap *sets,
						  s_erc *error);

static s_bool _context_match(const s_str_list *LC, const s_str_list *rLC,
							 const SMap *sets, s_erc *error);

static s_str_list *_compare_A(const s_str_list *ruleA, const s_str_list *RC,
							  const SMap *sets, uint32 *counter, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API s_list *SSyllabificationRewritesRuleMatches(const SSyllabificationRewritesRule *self,
												  const s_str_list *LC, const s_str_list *RC,
												  s_str_list **A, const SMap *sets, s_erc *error)
{
	s_list *rc_after;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSyllabificationRewritesRuleMatches",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (LC == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSyllabificationRewritesRuleMatches",
				  "Argument \"LC\" is NULL");
		return NULL;
	}

	if (RC == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSyllabificationRewritesRuleMatches",
				  "Argument \"RC\" is NULL");
		return NULL;
	}

	if (A == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSyllabificationRewritesRuleMatches",
				  "Argument \"A\" is NULL");
		return NULL;
	}

	if (!S_SYLLABIFICATIONREWRITES_RULE_METH_VALID(self, rule_matches))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SSyllabificationRewritesRuleMatches",
				  "Rewrites rule method \"rule_matches\" not implemented");
		return NULL;
	}

	rc_after = S_SYLLABIFICATIONREWRITES_RULE_CALL(self, rule_matches)(self,
																	   LC,
																	   RC,
																	   A,
																	   sets,
																	   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SSyllabificationRewritesRuleMatches",
				  "Call to class method \"rule_matches\" failed"))
		return NULL;

	return rc_after;
}


const s_list *SSyllabificationRewritesRuleGetReplacement(const SSyllabificationRewritesRule *self,
														 s_erc *error)
{
	const s_list *replacement;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSyllabificationRewritesRuleGetReplacement",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_SYLLABIFICATIONREWRITES_RULE_METH_VALID(self, get_replacement))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SSyllabificationRewritesRuleGetReplacement",
				  "Rewrites rule method \"get_replacement\" not implemented");
		return NULL;
	}

	replacement = S_SYLLABIFICATIONREWRITES_RULE_CALL(self, get_replacement)(self,
																			 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SSyllabificationRewritesRuleGetReplacement",
				  "Call to class method \"get_replacement\" failed"))
		return NULL;

	return replacement;
}


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_syllabification_rewrites_rule_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&SyllabificationRewritesRuleClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syllabification_rewrites_rule_class_reg",
			  "Failed to register SSyllabificationRewritesRuleClass");
}


S_LOCAL void _s_syllabification_rewrites_rule_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&SyllabificationRewritesRuleClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syllabification_rewrites_rule_class_free",
			  "Failed to free SSyllabificationRewritesRuleClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SSyllabificationRewritesRule *self = obj;


	S_CLR_ERR(error);
	self->LC = NULL;
	self->A = NULL;
	self->RC = NULL;
	self->B = NULL;
}



static void Destroy(void *obj, s_erc *error)
{
	SSyllabificationRewritesRule *self = obj;
	s_erc local_error = S_SUCCESS;


	S_CLR_ERR(error);
	if (self->LC != NULL)
	{
		s_str_list_delete(self->LC, &local_error);
		S_CHK_ERR(&local_error, S_CONTERR,
				  "Destroy",
				  "Call to \"s_list_delete\" failed");
	}

	if (self->A != NULL)
	{
		s_str_list_delete(self->A, &local_error);
		S_CHK_ERR(&local_error, S_CONTERR,
				  "Destroy",
				  "Call to \"s_list_delete\" failed");
	}


	if (self->RC != NULL)
	{
		S_CLR_ERR(&local_error);
		s_str_list_delete(self->RC, &local_error);
		S_CHK_ERR(&local_error, S_CONTERR,
				  "Destroy",
				  "Call to \"s_list_delete\" failed");
	}

	if (self->B != NULL)
	{
		S_CLR_ERR(&local_error);
		s_str_list_delete(self->B, &local_error);
		S_CHK_ERR(&local_error, S_CONTERR,
				  "Destroy",
				  "Call to \"s_list_delete\" failed");
	}
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static const s_list *GetReplacement(const SSyllabificationRewritesRule *self,
									s_erc *error)
{
	S_CLR_ERR(error);
	return self->B;
}


static s_list *RuleMatches(const SSyllabificationRewritesRule *self,
						   const s_str_list *LC, const s_str_list *RC,
						   s_str_list **A, const SMap *sets, s_erc *error)
{
	uint32 size_RC;
	uint32 size_self_A;
	uint32 counter;
	s_str_list *reversedLC = NULL;
	s_str_list *newRC = NULL;
	s_str_list *a_match = NULL;
	int r;
	int s;


	S_CLR_ERR(error);
	if (LC == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "RuleMatches",
				  "Argument \"LC\" is NULL");
		return NULL;
	}

	if (RC == NULL)
		return NULL; /* no context to match */

	size_self_A = s_str_list_size(self->A, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "RuleMatches",
				  "Call to \"s_str_list_size\" failed"))
		return NULL;

	size_RC = s_str_list_size(RC, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "RuleMatches",
				  "Call to \"s_str_list_size\" failed"))
		return NULL;

	/* given right context (actually A + RC) must be at least as long
	 * as rule's A
	 */
	if (size_RC < size_self_A)
		return NULL;

	/* check if [ A ] matches given RC */
	(*A) = NULL;
	a_match = _compare_A(self->A, RC, sets, &counter, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "RuleMatches",
				  "Call to \"s_str_list_cmp\" failed"))
		goto error_return;

	if (a_match == NULL)
		return NULL; /* no match */

	/* RC is ok, now check LC. LC may have some limited regex stuff  */
	reversedLC = s_str_list_dup(LC, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "RuleMatches",
				  "Call to \"s_str_list_dup\" failed"))
		return NULL;

	s_str_list_reverse(reversedLC, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "RuleMatches",
				  "Call to \"s_list_reverse\" failed"))
		goto error_return;

	r = _context_match(self->LC, reversedLC, sets, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "RuleMatches",
				  "Call to \"_context_match\" failed"))
		goto error_return;

	s_str_list_delete(reversedLC, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "RuleMatches",
				  "Call to \"s_str_list_delete\" failed"))
		goto error_return;

	reversedLC = NULL;

	newRC = s_str_list_slice(RC, counter, -1, error); /* RC[counter:] */
	if (S_CHK_ERR(error, S_CONTERR,
				  "RuleMatches",
				  "Call to \"s_str_list_slice\" failed"))
		goto error_return;

	s = _context_match(self->RC, newRC, sets, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "RuleMatches",
				  "Call to \"_context_match\" failed"))
		goto error_return;

	if (r && s)
	{
		(*A) = a_match;
		return newRC;
	}

	/* all OK, no errors, clean up, no match */
	s_str_list_delete(newRC, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "RuleMatches",
				  "Call to \"s_str_list_delete\" failed"))
		goto error_return;

	s_str_list_delete(a_match, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "RuleMatches",
				  "Call to \"s_str_list_delete\" failed"))
		goto error_return;

	newRC = NULL;

	return NULL;

error_return:
	{
		s_erc local_err = S_SUCCESS;

		if (reversedLC != NULL)
			s_str_list_delete(reversedLC, &local_err);

		if (newRC != NULL)
			s_str_list_delete(newRC, &local_err);

		if (a_match != NULL)
			s_str_list_delete(a_match, &local_err);
	}

	return NULL;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/


static s_str_list *_compare_A(const s_str_list *ruleA, const s_str_list *RC,
							  const SMap *sets, uint32 *counter, s_erc *error)
{
	s_str_list *a_match = NULL;
	const s_str_list_element *itr_A;
	const s_str_list_element *itr_RC;


	S_CLR_ERR(error);
	a_match = s_str_list_new(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_compare_A",
				  "Call to \"s_str_list_new\" failed"))
		goto error_return;

	itr_A = s_str_list_first(ruleA, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_compare_A",
				  "Call to \"s_str_list_first\" failed"))
		goto error_return;

	itr_RC = s_str_list_first(RC, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_compare_A",
				  "Call to \"s_str_list_first\" failed"))
		goto error_return;

	*counter = 0;
	while ((itr_A != NULL) && (itr_RC != NULL))
	{
		const char *element_A;
		const char *element_RC;
		s_bool match;


		element_A = s_str_list_element_get(itr_A, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_compare_A",
					  "Call to \"s_str_list_element_get\" failed"))
			goto error_return;

		element_RC = s_str_list_element_get(itr_RC, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_compare_A",
					  "Call to \"s_str_list_element_get\" failed"))
			goto error_return;


		match = _item_match(element_A, element_RC, sets, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_compare_A",
					  "Call to \"_item_match\" failed"))
			goto error_return;

		if (!match)
		{
			s_str_list_delete(a_match, error);
			S_CHK_ERR(error, S_CONTERR,
					  "_compare_A",
					  "Call to \"s_str_list_delete\" failed");
			return NULL;
		}
		else
		{
			s_str_list_append(a_match, element_RC, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_compare_A",
						  "Call to \"s_str_list_append\" failed"))
				goto error_return;
		}

		itr_A = s_str_list_element_next(itr_A, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_compare_A",
					  "Call to \"s_str_list_element_next\" failed"))
			goto error_return;

		itr_RC = s_str_list_element_next(itr_RC, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_compare_A",
					  "Call to \"s_str_list_element_next\" failed"))
			goto error_return;

		(*counter)++;
	}

	/* all OK */
	return a_match;

	/* error, cleanup */
error_return:
	{
		s_erc local_err = S_SUCCESS;

		if (a_match != NULL)
			s_str_list_delete(a_match, &local_err);
	}

	return NULL;
}


static s_bool _item_match(const char *PATT, const char *THING, const SMap *sets,
						  s_erc *error)
{
	int rv;
	s_bool is_present;
	const SObject *tmp;
	const SList *setList;
	SObject *thingObject;

	S_CLR_ERR(error);
	rv = s_strcmp(PATT, THING, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_item_match",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (rv == 0)
		return TRUE;

	if (sets == NULL)
		return FALSE;

	tmp = SMapGetObjectDef(sets, PATT, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_item_match",
				  "Call to \"SMapGetObjectDef\" failed"))
		return FALSE;

	if (tmp == NULL)
		return FALSE;

	setList = S_CAST(tmp, SList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_item_match",
				  "Call to \"S_CAST(SList)\" failed"))
		return FALSE;

	thingObject = SObjectSetString(THING, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_item_match",
				  "Call to \"SObjectSetString\" failed"))
		return FALSE;

	is_present = SListValPresent(setList, thingObject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_item_match",
				  "Call to \"SListValPresent\" failed"))
	{
		S_DELETE(thingObject, "_item_match", error);
		return FALSE;
	}

	S_DELETE(thingObject, "_item_match", error);
	return is_present;
}


static s_bool _context_match(const s_str_list *pattern, const s_str_list *string,
							 const SMap *sets, s_erc *error)
{
	uint32 plength;
	uint32 slength;
	const char *element;
	int rv;
	int r;
	int s;
	int t;
	int u;
	s_str_list *new_pattern = NULL;
	s_str_list *tmp = NULL;
	s_str_list *new_string = NULL;
	const char *pattern0;
	const char *string0;


	S_CLR_ERR(error);
	plength = s_str_list_size(pattern, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_context_match",
				  "Call to \"s_str_list_size\" failed"))
		return FALSE;

	if (plength == 0) /* rule context is none, so match */
		return TRUE;

	slength = s_str_list_size(string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_context_match",
				  "Call to \"s_str_list_size\" failed"))
		return FALSE;

	if (slength == 0)
	{
		/* rule context is not none and itape context is
		 * none
		 */
		return FALSE;
	}

	if (plength > 1)
	{
		element = s_str_list_nth_string(pattern, 1, error); /* pattern[1] */
		if (S_CHK_ERR(error, S_CONTERR,
					  "_context_match",
					  "Call to \"s_str_list_nth_string\" failed"))
			return FALSE;

		rv = s_strcmp(element, "*", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_context_match",
					  "Call to \"s_strcmp\" failed"))
			return FALSE;

		if (rv == 0) /* pattern[1] == "*" */
		{
			new_pattern = s_str_list_slice(pattern, 2, -1, error); /* pattern[2:] */
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_slice\" failed"))
				return FALSE;

			r = _context_match(new_pattern, string, sets, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Recursive call to \"_context_match\" failed"))
				goto error_return;

			tmp = s_str_list_dup(new_pattern, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_dup\" failed"))
				goto error_return;

			s_str_list_delete(new_pattern, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_delete\" failed"))
				goto error_return;

			new_pattern = NULL;

			pattern0 = s_str_list_nth_string(pattern, 0, error); /* pattern[0] */
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_nth_string\" failed"))
				goto error_return;

			s_str_list_prepend(tmp, s_strdup(pattern0, error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_strdup/s_str_list_prepend\" failed"))
				goto error_return;

			s = _context_match(tmp, string, sets, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Recursive call to \"_context_match\" failed"))
				goto error_return;

			s_str_list_delete(tmp, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_delete\" failed"))
				goto error_return;

			tmp = NULL;

			string0 = s_str_list_nth_string(string, 0, error); /* string[0] */
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_nth_string\" failed"))
				goto error_return;

			t = _item_match(pattern0, string0, sets, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"_item_match\" failed"))
				goto error_return;

			new_string = s_str_list_slice(string, 1, -1, error); /* string[1:] */
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_slice\" failed"))
				return FALSE;

			u = _context_match(pattern, new_string, sets, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Recursive call to \"_context_match\" failed"))
				goto error_return;

			s_str_list_delete(new_string, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_delete\" failed"))
				goto error_return;

			new_string = NULL;

			return (r || s || (t && u));
		}

		rv = s_strcmp(element, "+", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_context_match",
					  "Call to \"s_strcmp\" failed"))
			return FALSE;

		if (rv == 0) /* pattern[1] == "+" */
		{
			pattern0 = s_str_list_nth_string(pattern, 0, error); /* pattern[0] */
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_nth_string\" failed"))
				goto error_return;

			string0 = s_str_list_nth_string(string, 0, error); /* string[0] */
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_nth_string\" failed"))
				goto error_return;

			r = _item_match(pattern0, string0, sets, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"_item_match\" failed"))
				goto error_return;

			tmp = s_str_list_slice(pattern, 2, -1, error); /* pattern[2:] */
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_slice\" failed"))
				return FALSE;

			s_str_list_prepend(tmp, s_strdup("*", error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_strdup/s_str_list_prepend\" failed"))
				goto error_return;

			s_str_list_prepend(tmp, s_strdup(pattern0, error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_strdup/s_str_list_prepend\" failed"))
				goto error_return;

			new_string = s_str_list_slice(string, 1, -1, error); /* string[1:] */
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_slice\" failed"))
				return FALSE;

			s = _context_match(tmp, string, sets, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Recursive call to \"_context_match\" failed"))
				goto error_return;

			s_str_list_delete(tmp, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_delete\" failed"))
				goto error_return;

			tmp = NULL;

			s_str_list_delete(new_string, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_context_match",
						  "Call to \"s_str_list_delete\" failed"))
				goto error_return;

			new_string = NULL;

			return (r && s);
		}
	}

	pattern0 = s_str_list_nth_string(pattern, 0, error); /* pattern[0] */
	if (S_CHK_ERR(error, S_CONTERR,
				  "_context_match",
				  "Call to \"s_str_list_nth_string\" failed"))
		goto error_return;

	string0 = s_str_list_nth_string(string, 0, error); /* string[0] */
	if (S_CHK_ERR(error, S_CONTERR,
				  "_context_match",
				  "Call to \"s_str_list_nth_string\" failed"))
		goto error_return;

	r =  _item_match(pattern0, string0, sets, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_context_match",
				  "Call to \"_item_match\" failed"))
		goto error_return;

	if (r == TRUE)
	{
		new_string = s_str_list_slice(string, 1, -1, error); /* string[1:] */
		if (S_CHK_ERR(error, S_CONTERR,
					  "_context_match",
					  "Call to \"s_str_list_slice\" failed"))
			return FALSE;


		new_pattern = s_str_list_slice(pattern, 1, -1, error); /* pattern[1:] */
		if (S_CHK_ERR(error, S_CONTERR,
					  "_context_match",
					  "Call to \"s_str_list_slice\" failed"))
			return FALSE;

		s = _context_match(new_pattern, new_string, sets, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_context_match",
					  "Recursive call to \"_context_match\" failed"))
			goto error_return;

		s_str_list_delete(new_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_context_match",
					  "Call to \"s_str_list_delete\" failed"))
			goto error_return;

		new_string = NULL;

		s_str_list_delete(new_pattern, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_context_match",
					  "Call to \"s_str_list_delete\" failed"))
			goto error_return;

		new_pattern = NULL;

		return s;
	}

	/* all OK, no error and no match */
	return FALSE;


error_return:
	{
		s_erc local_err = S_SUCCESS;


		if (new_pattern != NULL)
			s_str_list_delete(new_pattern, &local_err);

		if (tmp != NULL)
			s_str_list_delete(tmp, &local_err);

		if (new_string != NULL)
			s_str_list_delete(new_string, &local_err);
	}

	return FALSE;
}


/************************************************************************************/
/*                                                                                  */
/* SSyllabificationRewritesRule class initialization                                */
/*                                                                                  */
/************************************************************************************/

static SSyllabificationRewritesRuleClass SyllabificationRewritesRuleClass =
{
	/* SObjectClass */
	{
		"SSyllabificationRewritesRule",
		sizeof(SSyllabificationRewritesRule),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SSyllabificationRewritesRuleClass */
	GetReplacement,      /* get_replacement */
	RuleMatches          /* rule_matches    */
};
