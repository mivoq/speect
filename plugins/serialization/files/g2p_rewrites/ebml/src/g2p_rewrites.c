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
/* A g2p rewrites class implementation.                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "g2p_rewrites_rule.h"
#include "g2p_rewrites.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SG2PRewritesClass G2PRewritesClass; /* SG2PRewrites class declaration. */


/************************************************************************************/
/*                                                                                  */
/*  Static function prototypes                                                      */
/*                                                                                  */
/************************************************************************************/

static char *s_add_gzeros(const SG2PRewrites *self, const char *word, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_g2p_rewrites_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&G2PRewritesClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_g2p_rewrites_class_reg",
			  "Failed to register SG2PRewritesClass");
}


S_LOCAL void _s_g2p_rewrites_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&G2PRewritesClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_g2p_rewrites_class_free",
			  "Failed to free SG2PRewritesClass");
}

/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static char *s_add_gzeros(const SG2PRewrites *self, const char *word, s_erc *error)
{
	char *regex_string = NULL;
	char *new_word = NULL;
	char *string_before = NULL;
	char *string_after = NULL;
	char *temp_word = NULL;
	size_t string_size;
	size_t new_string_size;
	s_bool matches;
	s_gzero *zeros = self->zeros;


	S_CLR_ERR(error);
	new_word = s_strdup(word, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_add_gzeros",
				  "Call to \"s_strdup\" failed"))
		goto quit_error;

	while (zeros->symbol != NULL)
	{
		string_size = s_strzsize(zeros->symbol, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_add_gzeros",
					  "Call to \"s_strzsize\" failed"))
			goto quit_error;

		regex_string = S_MALLOC(char, string_size + 4);
		if (regex_string == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "s_add_gzeros",
					  "Failed to allocate memory for 'char' object");
			goto quit_error;
		}

		s_sprintf(regex_string, error, ".*%s.*", zeros->symbol);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_add_gzeros",
					  "Call to \"s_sprintf\" failed"))
			goto quit_error;

		matches = s_smatches(new_word, regex_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_add_gzeros",
					  "Call to \"s_smatches\" failed"))
			goto quit_error;

		new_string_size = 0;

		while (matches)
		{
			string_before = s_sbefore(new_word, zeros->symbol, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_add_gzeros",
						  "Call to \"s_sbefore\" failed"))
				goto quit_error;

			string_size = s_strsize(string_before, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_add_gzeros",
						  "Call to \"s_strzsize\" failed"))
				goto quit_error;

			new_string_size += string_size;

			string_after = s_safter(new_word, zeros->symbol, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_add_gzeros",
						  "Call to \"s_safter\" failed"))
				goto quit_error;

			string_size = s_strsize(string_after, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_add_gzeros",
						  "Call to \"s_strzsize\" failed"))
				goto quit_error;

			new_string_size += string_size;

			string_size = s_strsize(zeros->replacement, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_add_gzeros",
						  "Call to \"s_strzsize\" failed"))
				goto quit_error;

			new_string_size += string_size;

			temp_word = S_MALLOC(char, new_string_size + 1);
			if (temp_word == NULL)
			{
				S_FTL_ERR(error, S_MEMERROR,
						  "s_add_gzeros",
						  "Failed to allocate memory for 'char' object");
				goto quit_error;
			}

			s_sprintf(temp_word, error, "%s%s%s", string_before, zeros->replacement, string_after);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_add_gzeros",
						  "Call to \"s_sprintf\" failed"))
				goto quit_error;

			S_FREE(new_word);
			S_FREE(string_before);
			S_FREE(string_after);

			new_word = temp_word;
			temp_word = NULL;

			matches = s_smatches(new_word, regex_string, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_add_gzeros",
						  "Call to \"s_smatches\" failed"))
				goto quit_error;
		}

		S_FREE(regex_string);
		zeros++;
	}

	/* all OK */
	return new_word;

	/* error cleanup */
quit_error:
	if (new_word != NULL)
		S_FREE(new_word);

	if (regex_string != NULL)
		S_FREE(regex_string);

	if (string_before != NULL)
		S_FREE(string_before);

	if (string_after != NULL)
		S_FREE(string_after);

	if (temp_word != NULL)
		S_FREE(temp_word);

	return NULL;
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SG2PRewrites *self = obj;


	S_CLR_ERR(error);
	self->rules = NULL;
	self->zeros = NULL;
}


static void Destroy(void *obj, s_erc *error)
{
	SG2PRewrites *self = obj;
	s_gzero      *zeros;

	S_CLR_ERR(error);
	if (self->rules != NULL)
		S_DELETE(self->rules, "Destroy", error);

	if (self->zeros != NULL)
	{
		zeros = self->zeros;
		while (zeros->symbol != NULL)
		{
			S_FREE(zeros->symbol);
			S_FREE(zeros->replacement);
			zeros++;
		}

		S_FREE(self->zeros);
	}
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static const char *GetName(const SG2P *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;

	return (const char*)self->info->name;
}


static const char *GetDescription(const SG2P *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;

	return (const char*)self->info->description;
}


static const char *GetLanguage(const SG2P *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;

	return (const char*)self->info->language;
}


static const char *GetLangCode(const SG2P *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;

	return (const char*)self->info->lang_code;
}


static const s_version *GetVersion(const SG2P *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;

	return (const s_version*)&(self->info->version);
}

static const SObject *GetFeature(const SG2P *self, const char *key,
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


static SList *Apply(const SG2P *self, const char *word, s_erc *error)
{
	SG2PRewrites *g2p = S_G2PREWRITES(self);
	char *word_copy = NULL;
	char *word_copy_orig = NULL;
	char *word_left_context = NULL;
	size_t word_size;
	char alpha[4] = "\0";
	uint32 character;
	const SList *rules;
	SIterator *itr;
	const char *matched_phone;
	s_bool rule_matches;
	SList *phoneList;


	S_CLR_ERR(error);

	if (g2p->rules == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Apply",
				  "G2P Rewrites has no rules");
		return NULL;
	}

	/* create a list for the phones */
	phoneList = (SList*)S_NEW("SListList", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Apply",
				  "Failed to create new 'SList' object"))
		goto quit_error;

	SListListInit(&phoneList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Apply",
				  "Failed to initialize new 'SList' object"))
		goto quit_error;

	word_size = s_strzsize(word, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Apply",
				  "Call to \"s_strsize\" failed"))
		goto quit_error;

	/* add "#" at beginning and end of word */
	word_copy = S_MALLOC(char, word_size + 2);
	if (word_copy == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "Apply",
				  "Failed to allocate memory for 'char' object");
		goto quit_error;
	}

	s_sprintf(word_copy, error, "#%s#", word);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Apply",
				  "Call to \"s_sprintf\" failed"))
		goto quit_error;

	/*
	 * if we have gzeros, then fix the word.
	 */
	if (g2p->zeros != NULL)
	{
		char *tmp_word;


		tmp_word = s_add_gzeros(g2p, word_copy, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Apply",
					  "Call to \"s_add_gzeros\" failed"))
			goto quit_error;

		S_FREE(word_copy);
		word_copy = tmp_word;

		/* get zeros fixed word size */
		word_size = s_strzsize(word_copy, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Apply",
					  "Call to \"s_strsize\" failed"))
			goto quit_error;
	}

	word_copy_orig = word_copy;

	/* make word left context holder */
	word_left_context = S_CALLOC(char, word_size);
	if (word_left_context == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "Apply",
				  "Failed to allocate memory for 'char' object");
		goto quit_error;
	}

	/* populate word left context with # of word */
	s_setat(word_left_context, 0, s_getx(&word_copy, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Apply",
				  "Call to \"s_setat/s_getx\" failed"))
		goto quit_error;

	character = s_getx(&word_copy, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Apply",
				  "Call to \"s_getx\" failed"))
		goto quit_error;

	word_size = s_strsize(word_copy, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Apply",
				  "Call to \"s_strsize\" failed"))
		goto quit_error;

	while ((character != 0) && (word_size > 0)) /* skip last '#' */
	{
		/* move character to alpha */
		s_setc(alpha, character, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Apply",
					  "Call to \"s_setc\" failed"))
			goto quit_error;

		/* get list of rules that match alpha */
		rules = (SList*)SMapGetObjectDef(g2p->rules, alpha, NULL, error);
		if (rules == NULL)
		{
			/* issue warning */
			S_WARNING(S_FAILURE,
					  "Apply",
					  "Failed to find a g2p rule for character '%s' of word '%s'",
					  alpha, word);
		}
		else
		{
			/* iterate through rules searching for a match */
			itr = SListIterator(rules, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Apply",
						  "Call to \"SListIterator\" failed"))
				goto quit_error;

			while (itr != NULL)
			{
				const SG2PRewritesRule *thisRule;


				thisRule = (const SG2PRewritesRule*)SListIteratorValue(itr, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Apply",
							  "Call to \"SListIteratorValue\" failed"))
					goto quit_error;

				rule_matches = S_G2PREWRITES_RULE_CALL(thisRule, rule_matches)(thisRule,
																			   &matched_phone,
																			   word_left_context,
																			   word_copy,
																			   error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Apply",
							  "Call to method \"rule_matches\" failed"))
					goto quit_error;

				if (rule_matches)
				{
					S_DELETE(itr, "Apply", error);
					break;
				}

				itr = SIteratorNext(itr);
			}

			if (rule_matches)
			{
				/* add phone to list, matched_phone can be NULL */
				if (matched_phone != NULL)
				{
					SListAppend(phoneList, SObjectSetString(matched_phone, error), error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "Apply",
								  "Failed to add matched phone"))
						goto quit_error;
				}
			}
			else
			{
				/* issue warning that we did not find a match */
				S_WARNING(S_FAILURE,
						  "Apply",
						  "Failed to find a g2p rule match for character '%s' of word '%s'",
						  alpha, word);
			}
		}

		/* move alpha to left context (at index 0) */
		s_insert(word_left_context, 0, s_getc(alpha, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Apply",
					  "Call to \"s_insert/s_getc\" failed"))
			goto quit_error;

		/* get next character */
		character = s_getx(&word_copy, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Apply",
					  "Call to \"s_getx\" failed"))
			goto quit_error;

		word_size = s_strsize(word_copy, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Apply",
					  "Call to \"s_strsize\" failed"))
			goto quit_error;
	}

	/* got here so OK */
	goto quit;


	/* error clean up code */
quit_error:
	if (itr != NULL)
		S_DELETE(itr, "Apply", error);

	/* sets phoneList = NULL */
	if (phoneList != NULL)
		S_DELETE(phoneList, "Apply", error);

	/* normal cleanup code */
quit:
	if (word_copy_orig != NULL)
		S_FREE(word_copy_orig);

	if (word_left_context != NULL)
		S_FREE(word_left_context);

	return phoneList;
}


/************************************************************************************/
/*                                                                                  */
/* SG2P class initialization                                                        */
/*                                                                                  */
/************************************************************************************/

/* same as SG2PClass */
static SG2PRewritesClass G2PRewritesClass =
{
	/* SObjectClass */
	{
		"SG2P:SG2PRewrites",
		sizeof(SG2PRewrites),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SG2PClass */
	GetName,             /* get_name        */
	GetDescription,      /* get_description */
	GetLanguage,         /* get_language    */
	GetLangCode,         /* get_lang_code   */
	GetVersion,          /* get_version     */
	GetFeature,          /* get_feature     */
	Apply                /* apply           */
};
