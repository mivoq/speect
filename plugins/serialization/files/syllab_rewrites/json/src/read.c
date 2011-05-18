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
/* Read syllabification rewrite rules from a JSON format file.                      */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "syllab_rewrites.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void set_syllabification_info(SSyllabification *syllabification,
									 const SMap *syllabificationDef, s_erc *error);

static s_str_list *reverse_fix_LC(const s_str_list *LC, s_erc *error);

static void set_rule_in_map(SMap *ruleMap, const SSyllabificationRewritesRule *rule,
							const char *entry, const SMap *sets, s_erc *error);

static void _set_rule(SMap *ruleMap, const SSyllabificationRewritesRule *rule,
					  const char *entry, s_erc *error);

static SMap *parse_rules(const SList *ruleList, const SMap *sets, s_erc *error);

static s_str_list *check_and_set_string_list(const SObject *poppedObject, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SSyllabificationRewrites *s_read_syllabification_rewrites_json(const char *path, s_erc *error)
{
	SSyllabificationRewrites *syllab = NULL;
	SMap *parsedFile = NULL;
	const SMap *tmpMap;
	const SList *tmpList;
	const SObject *tmp;
	s_bool is_present;
	SMap *rules;


	S_CLR_ERR(error);

	/* create syllab rewrites */
	syllab = (SSyllabificationRewrites*)S_NEW("SSyllabificationRewrites", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_syllabification_rewrites_json",
				  "Failed to create new syllabification rewrite rules object"))
		goto quit_error;

	/* read the JSON file into a SMAP */
	parsedFile = s_json_parse_config_file(path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_syllabification_rewrites_json",
				  "Call to \"s_json_parse_config_file\" failed"))
		goto quit_error;

	/* get "syllabification-definition" key */
	tmp = SMapGetObjectDef(parsedFile, "syllabification-definition", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_syllabification_rewrites_json",
				  "Call to \"SMapGetObjectDef\" failed"))
		goto quit_error;

	if (tmp == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_read_syllabification_rewrites_json",
				  "Syllabification rules file does not have a 'syllabification-definition' key");
		goto quit_error;
	}

	/* cast to make sure it's a map */
	tmpMap = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_syllabification_rewrites_json",
				  "Syllabification rules file key 'syllabification-definition' must be a map type"))
		goto quit_error;

	set_syllabification_info(S_SYLLABIFICATION(syllab), tmpMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_syllabification_rewrites_json",
				  "Call to \"set_syllabification_info\" failed"))
		goto quit_error;

	/* get syllabification features, if any */
	is_present = SMapObjectPresent(parsedFile, "features", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_syllabification_rewrites_json",
				  "Call to \"SMapObjectIsPresent\" failed"))
		goto quit_error;

	if (is_present)
	{
		SObject *featuresObject;
		SMap *featuresMap;


		featuresObject = SMapObjectUnlink(parsedFile, "features", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_syllabification_rewrites_json",
					  "Call to \"SMapObjectUnlink\" failed"))
			goto quit_error;

		/* cast to make sure it's a map */
		featuresMap = S_CAST(featuresObject, SMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_syllabification_rewrites_json",
					  "Syllabification rules file key 'features' must be a map type"))
		{
			S_DELETE(featuresObject, "s_read_syllabification_rewrites_json", error);
			goto quit_error;
		}

		/* give the features map to the syllabification rules */
		S_SYLLABIFICATION(syllab)->features = featuresMap;
	}

	/* get sets, if any */
	is_present = SMapObjectPresent(parsedFile, "sets", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_syllabification_rewrites_json",
				  "Call to \"SMapObjectIsPresent\" failed"))
		goto quit_error;

	if (is_present)
	{
		SObject *setsObject;
		SMap *setsMap;


		setsObject = SMapObjectUnlink(parsedFile, "sets", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_syllabification_rewrites_json",
					  "Call to \"SMapObjectUnlink\" failed"))
			goto quit_error;

		/* cast to make sure it's a map */
		setsMap = S_CAST(setsObject, SMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_syllabification_rewrites_json",
					  "Syllabification rules file key 'sets' must be a map type"))
		{
			S_DELETE(setsObject, "s_read_syllabification_rewrites_json", error);
			goto quit_error;
		}

		/* give the sets map to the syllabification rules */
		syllab->sets = setsMap;
	}

	/* get rules */
	tmp = SMapGetObjectDef(parsedFile, "rules", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_syllabification_rewrites_json",
				  "Call to \"SMapGetObjectDef\" failed"))
		goto quit_error;

	if (tmp == NULL)
	{
		/* must have rules */
		S_CTX_ERR(error, S_FAILURE,
				  "s_read_syllabification_rewrites_json",
				  "Syllabification rules file does not have a 'rules' key");
		goto quit_error;
	}

	/* cast to make sure it's a list */
	tmpList = S_CAST(tmp, SList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_syllabification_rewrites_json",
				  "Syllabification rules file key 'rules' must be a list type"))
		goto quit_error;

	/* parse rules */
	rules = parse_rules(tmpList, syllab->sets, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_syllabification_rewrites_json",
				  "Call to \"parse_rules\" failed"))
		goto quit_error;

	/* give rules to SSyllabificationRewrites */
	syllab->rules = rules;

	/* done */
	goto quit;


	/* errors start clean up code here */
quit_error:
	if (syllab != NULL)
		S_DELETE(syllab, "s_read_syllabification_rewrites_json", error);  /* sets syllab = NULL */

	/* normal exit start clean up code here */
quit:
	if (parsedFile != NULL)
		S_DELETE(parsedFile, "s_read_syllabification_rewrites_json", error);

	return syllab;
}

/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_str_list *reverse_fix_LC(const s_str_list *LC, s_erc *error)
{
	const s_str_list_element *itr;
	uint32 LC_size;
	s_str_list *new_LC = NULL;
	s_bool skip;
	uint32 cnt;


	S_CLR_ERR(error);
	if (LC == NULL)
		return NULL;

	LC_size = s_str_list_size(LC, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "reverse_fix_LC",
				  "Call to \"s_str_list_size\" failed"))
		goto error_return;

	if (LC_size == 1)
	{
		new_LC = s_str_list_dup(LC, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "reverse_fix_LC",
					  "Call to \"s_str_list_dup\" failed"))
			return NULL;

		return new_LC;
	}

	new_LC = s_str_list_new(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "reverse_fix_LC",
				  "Call to \"s_str_list_new\" failed"))
		goto error_return;


	itr = s_str_list_first(LC, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "reverse_fix_LC",
				  "Call to \"s_str_list_first\" failed"))
		return NULL;

	skip = FALSE;
	for (cnt = 0; cnt < (LC_size - 1); cnt++)
	{
		const char *element;
		int rv0;
		int rv1;


		if (skip)
		{
			skip = FALSE;
			continue;
		}

		element = s_str_list_nth_string(LC, cnt + 1, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "reverse_fix_LC",
					  "Call to \"s_str_list_nth_string\" failed"))
			goto error_return;

		rv0 = s_strcmp(element, "*", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "reverse_fix_LC",
					  "Call to \"s_strcmp\" failed"))
			goto error_return;

		rv1 = s_strcmp(element, "+", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "reverse_fix_LC",
					  "Call to \"s_strcmp\" failed"))
			goto error_return;

		if ((rv0 == 0) || (rv1 == 0))
		{
			s_str_list_append(new_LC, element, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "reverse_fix_LC",
						  "Call to \"s_str_list_append\" failed"))
				goto error_return;

			skip = TRUE;
		}

		element = s_str_list_nth_string(LC, cnt, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "reverse_fix_LC",
					  "Call to \"s_str_list_nth_string\" failed"))
			goto error_return;

		s_str_list_append(new_LC, element, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "reverse_fix_LC",
					  "Call to \"s_str_list_append\" failed"))
			goto error_return;
	}

	s_str_list_reverse(new_LC, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "reverse_fix_LC",
				  "Call to \"s_str_list_reverse\" failed"))
		goto error_return;

	/* all OK */
	return new_LC;

	/* error cleanup */
error_return:
	{
		s_erc local_err = S_SUCCESS;


		if (new_LC != NULL)
			s_str_list_delete(new_LC, &local_err);
	}

	return NULL;
}


static void _set_rule(SMap *ruleMap, const SSyllabificationRewritesRule *rule,
					  const char *entry, s_erc *error)
{
	SList *ruleList;


	S_CLR_ERR(error);
	ruleList = (SList*)SMapGetObjectDef(ruleMap, entry, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_set_rule",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (ruleList == NULL)
	{
		ruleList = (SList*)S_NEW("SListList", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_set_rule",
					  "Failed to create new 'SListList' object"))
			return;

		SMapSetObject(ruleMap, entry, S_OBJECT(ruleList), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_set_rule",
					  "Call to \"SMapSetObject\" failed"))
		{
			S_DELETE(ruleList, "_set_rule", error);
			return;
		}
	}

	SListAppend(ruleList, S_OBJECT(rule), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_set_rule",
			  "Call to \"SListAppend\" failed");
}


static void set_rule_in_map(SMap *ruleMap, const SSyllabificationRewritesRule *rule,
							const char *entry, const SMap *sets, s_erc *error)
{
	SList *setList;
	const SObject *tmp;


	S_CLR_ERR(error);
	tmp = SMapGetObjectDef(sets, entry, NULL, error);
	if (tmp != NULL)
	{
		SIterator *itr;


		setList = S_CAST(tmp, SList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_rule_in_map",
					  "Call to \"S_CAST(SList)\" failed"))
			return;

		itr = S_ITERATOR_GET(setList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_rule_in_map",
					  "Call to \"S_ITERATOR_GET\" failed"))
			return;

		for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
		{
			const SObject *setObject;


			setObject = SIteratorObject(itr, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "set_rule_in_map",
						  "Call to \"SIteratorObject\" failed"))
			{
				S_DELETE(itr, "set_rule_in_map", error);
				return;
			}

			_set_rule(ruleMap, rule, SObjectGetString(setObject, error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "set_rule_in_map",
						  "Call to \"SObjectGetString/_set_rule\" failed"))
			{
				S_DELETE(itr, "set_rule_in_map", error);
				return;
			}
		}
	}
	else
	{
		_set_rule(ruleMap, rule, entry, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_rule_in_map",
					  "Call to \"set_rule\" failed"))
			return;
	}
}


static s_str_list *check_and_set_string_list(const SObject *poppedObject, s_erc *error)
{
	const char *poppedString;
	int rv;
	s_str_list *list = NULL;


	S_CLR_ERR(error);
	poppedString = SObjectGetString(poppedObject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "check_and_set_string_list",
				  "Call to \"SObjectGetString\" failed"))
		return NULL;

	/* check if empty string */
	rv = s_strcmp(poppedString, "", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "check_and_set_string_list",
				  "Call to \"s_strcmp\" failed"))
		return NULL;

	if (rv != 0) /* not empty */
	{
		list = s_str_list_split(poppedString, " ", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "check_and_set_string_list",
					  "Call to \"s_str_list_split\" failed"))
			return NULL;
	}

	return list;
}


static SMap *parse_rules(const SList *ruleList, const SMap *sets, s_erc *error)
{
	SIterator *itr = NULL;
	SMap *ruleMap = NULL;
	uint32 rule_counter;
	SSyllabificationRewritesRule *rule = NULL;
	SObject *poppedObject = NULL;


	S_CLR_ERR(error);

	/* create ruleMap, default size (128) is fine */
	ruleMap = (SMap*)S_NEW("SMapHashTable", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "parse_rules",
				  "Failed to create new 'SMapHashTable' object"))
		goto error_return;

	itr = S_ITERATOR_GET(ruleList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "parse_rules",
				  "Call to \"S_ITERATOR_GET\" failed"))
		goto error_return;

	for (rule_counter = 0; itr != NULL; itr = SIteratorNext(itr), rule_counter++)
	{
		const SObject *tmp;
		SList *ruleDef;
		size_t lsize;
		SSyllabificationRewritesRule *rule;
		const char *map_char;
		s_str_list *tmp_list;


		tmp = SIteratorObject(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"SIteratorObject\" failed"))
			goto error_return;

		/* cast to make sure it's a list */
		ruleDef = S_CAST(tmp, SList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"S_CAST\" for rule number %d failed", rule_counter))
			goto error_return;

		lsize = SListSize(ruleDef, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"SListSize\" for rule number %d failed", rule_counter))
			goto error_return;

		if (lsize != 4)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "parse_rules",
					  "Rule number %d does not contain 4 entries", rule_counter);
			goto error_return;
		}

		/* create a new rule */
		rule = (SSyllabificationRewritesRule*)S_NEW("SSyllabificationRewritesRule", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Failed to create a new \"SSyllabificationRewritesRule\" object"))
			goto error_return;

		/* get replacement B */
		poppedObject = SListPop(ruleDef, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"SListPop\" failed"))
			goto error_return;

		rule->B = check_and_set_string_list(poppedObject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"check_and_set_string_list\" failed"))
			goto error_return;

		S_DELETE(poppedObject, "parse_rules", error);

		/* get right-context RC */
		poppedObject = SListPop(ruleDef, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"SListPop\" failed"))
			goto error_return;

		rule->RC = check_and_set_string_list(poppedObject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"check_and_set_string_list\" failed"))
			goto error_return;

		S_DELETE(poppedObject, "parse_rules", error);

		/* get this-context A */
		poppedObject = SListPop(ruleDef, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"SListPop\" failed"))
			goto error_return;

		rule->A = check_and_set_string_list(poppedObject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"check_and_set_string_list\" failed"))
			goto error_return;

		S_DELETE(poppedObject, "parse_rules", error);

		if (rule->A == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "parse_rules",
					  "Rule context is NULL for rule number %d", rule_counter);
			goto error_return;
		}

		/* first entry of this context */
		map_char = s_str_list_nth_string(rule->A, 0, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"s_str_list_nth_string\" failed"))
			goto error_return;

		/* get left-context LC */
		poppedObject = SListPop(ruleDef, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"SListPop\" failed"))
			goto error_return;

		tmp_list = check_and_set_string_list(poppedObject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"check_and_set_string_list\" failed"))
			goto error_return;

		S_DELETE(poppedObject, "parse_rules", error);

		rule->LC = reverse_fix_LC(tmp_list, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"reverse_fix_LC\" failed"))
			goto error_return;

		s_str_list_delete(tmp_list, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"s_str_list_delete\" failed"))
			goto error_return;

		tmp_list = NULL;

		/* set entry for rule map */
		set_rule_in_map(ruleMap, rule, map_char, sets, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "parse_rules",
					  "Call to \"set_rule_in_map\" failed"))
			goto error_return;

		rule = NULL;
	}

	/* all OK */
	return ruleMap;


	/* error cleanup */
error_return:
	{
		s_erc local_err = S_SUCCESS;


		if (ruleMap != NULL)
			S_DELETE(ruleMap, "parse_rules", &local_err);

		if (itr != NULL)
			S_DELETE(itr, "parse_rules", &local_err);

		if (rule != NULL)
			S_DELETE(rule, "parse_rules", &local_err);

		if (poppedObject != NULL)
			S_DELETE(poppedObject, "parse_rules", &local_err);
	}

	return NULL;
}


static void set_syllabification_info(SSyllabification *syllabification,
									 const SMap *syllabificationDef, s_erc *error)
{
	const SObject *tmpObject;
	const char *tmp_string;
	const SMap *versionMap;


	S_CLR_ERR(error);

	/* get syllabification name */
	tmp_string = SMapGetStringDef(syllabificationDef, "name", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_syllabification_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp_string == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_syllabification_info",
				  "'syllabification-definition' does not have a 'name' key");
		return;
	}

	syllabification->info->name = s_strdup(tmp_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_syllabification_info",
				  "Call to \"s_strdup\" failed"))
		return;

	/* get syllabification description */
	tmp_string = SMapGetStringDef(syllabificationDef, "description", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_syllabification_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp_string == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_syllabification_info",
				  "'syllabification-definition' does not have a 'description' key");
		return;
	}

	syllabification->info->description = s_strdup(tmp_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_syllabification_info",
				  "Call to \"s_strdup\" failed"))
		return;

	/* get syllabification language */
	tmp_string = SMapGetStringDef(syllabificationDef, "language", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_syllabification_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp_string == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_syllabification_info",
				  "'syllabification-definition' does not have a 'language' key");
		return;
	}

	syllabification->info->language = s_strdup(tmp_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_syllabification_info",
				  "Call to \"s_strdup\" failed"))
		return;

	/* get syllabification lang_code */
	tmp_string = SMapGetStringDef(syllabificationDef, "lang-code", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_syllabification_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp_string == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_syllabification_info",
				  "'syllabification-definition' does not have a 'lang_code' key");
		return;
	}

	syllabification->info->lang_code = s_strdup(tmp_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_syllabification_info",
				  "Call to \"s_strdup\" failed"))
		return;

	/* get syllabification version */
	tmpObject = SMapGetObjectDef(syllabificationDef, "version", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_syllabification_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmpObject == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_syllabification_info",
				  "'syllabification-definition' does not have a 'version' key");
		return;
	}

	versionMap = S_CAST(tmpObject, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_syllabification_info",
				  "'syllabification-definition' key 'version' must be a map type"))
		return;

	syllabification->info->version.major = SMapGetInt(versionMap, "major", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_syllabification_info",
				  "Call to \"SMapGetInt\" failed"))
		return;

	syllabification->info->version.minor = SMapGetInt(versionMap, "minor", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_syllabification_info",
				  "Call to \"SMapGetInt\" failed"))
		return;
}
