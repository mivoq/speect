/************************************************************************************/
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
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
/* DATE    : June 2012                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Utterance break utterance processor.                                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "uttbreak_proc.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SUttBreakUttProcClass UttBreakUttProcClass; /* SUttBreakUttProc class declaration. */

/* default character symbols */
static const char * const s_default_whitespacesymbols      = " \t\n\r";
static const char * const s_default_singlecharsymbols      = "(){}[]";
static const char * const s_default_prepunctuationsymbols  = "\"'`({[";
static const char * const s_default_postpunctuationsymbols = "\"'`.,:;!?(){}[]";


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void s_get_tokenizer_symbols(SUttBreakUttProc *self, const SMap *features,
									s_erc *error);

/* return post punc */
static char *s_add_token_to_relation(SRelation *tokenRel, const SToken *token,
									 const char *token_string, s_erc *error);

s_bool s_test_whitespace_double_line_breaks(const char *token_whitespace, s_erc *error);

s_bool s_test_unambiguous_postpunc(const char *item_post_punc, s_erc *error);

s_bool s_test_period_capital(const char *token_whitespace, const char *token_string,
							 const char *item_post_punc, s_erc *error);

s_bool s_test_abbreviation(const char *item_string, s_erc *error);

s_bool s_test_period_abbreviation(const char *token_string, const char *item_string,
								  const char *item_post_punc, s_erc *error);

s_bool s_got_uttbreak(const SToken *token, const char *token_string,
					  const char *item_post_punc, const char *item_string, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_uttbreak_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&UttBreakUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_uttbreak_utt_proc_class_reg",
			  "Failed to register SUttBreakUttProcClass");
}


S_LOCAL void _s_uttbreak_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&UttBreakUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_uttbreak_utt_proc_class_free",
			  "Failed to free SUttBreakUttProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void s_get_tokenizer_symbols(SUttBreakUttProc *self, const SMap *features,
									s_erc *error)
{
	const SMap *tokenizerSymbols;
	const char *tmp;


	S_CLR_ERR(error);

	tokenizerSymbols = S_MAP(SMapGetObjectDef(features, "tokenizer symbols", NULL, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (tokenizerSymbols == NULL)
	{
		/* use defaults */
		self->white_space_chars = s_strdup(s_default_whitespacesymbols, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_tokenizer_symbols",
					  "Call to \"s_strdup\" failed"))
			goto quit_error;

		self->single_char_symbols = s_strdup(s_default_singlecharsymbols, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_tokenizer_symbols",
					  "Call to \"s_strdup\" failed"))
			goto quit_error;

		self->pre_punc_symbols =  s_strdup(s_default_prepunctuationsymbols, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_tokenizer_symbols",
					  "Call to \"s_strdup\" failed"))
			goto quit_error;

		self->post_punc_symbols =  s_strdup(s_default_postpunctuationsymbols, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_tokenizer_symbols",
					  "Call to \"s_strdup\" failed"))
			goto quit_error;

		return;
	}

	/* get symbols from map */
	tmp = SMapGetStringDef(tokenizerSymbols, "whitespace", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"SMapGetStringDef\" failed"))
		goto quit_error;

	if (tmp == NULL) /* use defaults */
		tmp = s_default_whitespacesymbols;

	self->white_space_chars = s_strdup(tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"s_strdup\" failed"))
		goto quit_error;

	tmp = SMapGetStringDef(tokenizerSymbols, "single-char", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"SMapGetStringDef\" failed"))
		goto quit_error;

	if (tmp == NULL) /* use defaults */
		tmp = s_default_singlecharsymbols;

	self->single_char_symbols = s_strdup(tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"s_strdup\" failed"))
		goto quit_error;


	tmp = SMapGetStringDef(tokenizerSymbols, "pre-punctuation", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"SMapGetStringDef\" failed"))
		goto quit_error;

	if (tmp == NULL) /* use defaults */
		tmp = s_default_prepunctuationsymbols;

	self->pre_punc_symbols = s_strdup(s_default_prepunctuationsymbols, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"s_strdup\" failed"))
		goto quit_error;

	tmp = SMapGetStringDef(tokenizerSymbols, "post-punctuation", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"SMapGetStringDef\" failed"))
		goto quit_error;

	if (tmp == NULL) /* use defaults */
		tmp = s_default_postpunctuationsymbols;

	self->post_punc_symbols = s_strdup(s_default_postpunctuationsymbols, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"s_strdup\" failed"))
		goto quit_error;

	/* all ok */
	return;

	/* error clean up */
quit_error:
	if (self->white_space_chars != NULL)
		S_FREE(self->white_space_chars);

	if (self->single_char_symbols != NULL)
		S_FREE(self->single_char_symbols);

	if (self->pre_punc_symbols != NULL)
		S_FREE(self->pre_punc_symbols);

	if (self->post_punc_symbols != NULL)
		S_FREE(self->post_punc_symbols);
}


static char *s_add_token_to_relation(SRelation *tokenRel, const SToken *token,
									 const char *token_string, s_erc *error)
{
	SItem *tokenItem;
	const char *tmp;
	char *post_punc;


	S_CLR_ERR(error);

	/* create item, NULL shared content */
	tokenItem = SRelationAppend(tokenRel, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_add_token_to_relation",
				  "Call to \"SRelationAppend\" failed"))
		return NULL;

	/* item's name is the token string */
	SItemSetName(tokenItem, token_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_add_token_to_relation",
				  "Call to \"SItemSetName\" failed"))
		return NULL;

	/* get white-space */
	tmp = STokenGetWhitespace(token, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_add_token_to_relation",
				  "Call to \"STokenGetWhitespace\" failed"))
		return NULL;

	if (tmp != NULL)
	{
		SItemSetString(tokenItem, "whitespace", tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_add_token_to_relation",
					  "Call to \"SItemSetString\" failed"))
			return NULL;
	}

	/* get pre-punctuation */
	tmp = STokenGetPrePunc(token, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_add_token_to_relation",
				  "Call to  \"STokenGetPrePunc\" failed"))
		return NULL;

	if (tmp != NULL)
	{
		SItemSetString(tokenItem, "prepunc", tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_add_token_to_relation",
					  "Call to \"SItemSetString\" failed"))
			return NULL;
	}

	/* get post-punctuation */
	tmp = STokenGetPostPunc(token, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_add_token_to_relation",
				  "Call to \"STokenGetPostPunc\" failed"))
		return NULL;

	if (tmp != NULL)
	{
		SItemSetString(tokenItem, "postpunc", tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_add_token_to_relation",
					  "Call to \"SItemSetString\" failed"))
			return NULL;
	}

	post_punc = s_strdup(tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_add_token_to_relation",
				  "Call to \"s_strdup\" failed"))
		return NULL;

	return post_punc; /* return the post punctuation */
}


/* test if the whitespace of the token contains at least 2
 * linebreaks. This means that this token is at the beginning of a new
 * paragraph
 */
s_bool s_test_whitespace_double_line_breaks(const char *token_whitespace, s_erc *error)
{
	const char *forward_linebreak;
	const char *reverse_linebreak;


	S_CLR_ERR(error);
	forward_linebreak = s_strchr(token_whitespace, '\n', error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_test_whitespace_double_line_breaks",
				  "Call to \"s_strchr\" failed"))
		return FALSE;

	if (forward_linebreak == NULL) /* no linebreaks */
		return FALSE;

	reverse_linebreak = s_strrchr(token_whitespace, '\n', error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_test_whitespace_double_line_breaks",
				  "Call to \"s_strrchr\" failed"))
		return FALSE;

	if (forward_linebreak != reverse_linebreak) /* at least 2
												   linebreaks */
		return TRUE;

	return FALSE;
}


/* test if we have unambiguous post punctuation in the current last
 * item. Unambiguous post punctuation is ? and !
 */
s_bool s_test_unambiguous_postpunc(const char *item_post_punc, s_erc *error)
{
	const char *tmp;


	S_CLR_ERR(error);
	tmp = s_strchr(item_post_punc, '?', error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_test_unambiguous_postpunc",
				  "Call to \"s_strchr\" failed"))
		return FALSE;

	if (tmp != NULL) /* got question mark, break */
		return TRUE;

	tmp = s_strchr(item_post_punc, '!', error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_test_unambiguous_postpunc",
				  "Call to \"s_strchr\" failed"))
		return FALSE;

	if (tmp != NULL) /* got exclamation mark, break */
		return TRUE;

	return FALSE;
}


/* test if the capital in the token and the post punc period in the
 * item are a break. break is is period and capital is seperated by
 * more than one space, for example "done.  And blah"
 */
s_bool s_test_period_capital(const char *token_whitespace, const char *token_string,
							 const char *item_post_punc, s_erc *error)
{
	const char *tmp;
	size_t len;


	S_CLR_ERR(error);
	tmp = s_strchr(item_post_punc, '.', error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_test_period_capital",
				  "Call to \"s_strchr\" failed"))
		return FALSE;

	if (tmp == NULL) /* post punc is not period, no break */
		return FALSE;

	len = s_strlen(token_whitespace, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_test_period_capital",
				  "Call to \"s_strlen\" failed"))
		return FALSE;

	if (len < 2) /* maybe token is a abbreviation, ambiguous */
		return FALSE;

	tmp = s_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ", token_string[0], error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_test_period_capital",
				  "Call to \"s_strchr\" failed"))
		return FALSE;

	if (tmp == NULL) /* token does not start with a capital letter */
		return FALSE;

	return TRUE;
}



/* Simple test for possible abbreviation. If item starts and ends in
 * capitals and is less than 4 chars long then it's an abbreviation.
 */
s_bool s_test_abbreviation(const char *item_string, s_erc *error)
{
	const char *tmp;
	size_t len;


	S_CLR_ERR(error);
	len = s_strlen(item_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_test_abbreviation",
				  "Call to \"s_strlen\" failed"))
		return FALSE;

	tmp = s_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ", item_string[len-1], error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_test_period_abbreviation",
				  "Call to \"s_strchr\" failed"))
		return FALSE;

	if (tmp == NULL) /* item is probably not abbreviation, doesn't end in a capital */
		return FALSE;

	tmp = s_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ", item_string[0], error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_test_abbreviation",
				  "Call to \"s_strchr\" failed"))
		return FALSE;

	/* item starts and ends with capital and is less than 4 chars
	   long, so is probably an abbreviation */
	if ((tmp != NULL) && (len < 4))
		return TRUE;

	return FALSE;
}


/* test there is period in item post punc, item is not abbreviation and
 * token starts with capital.
 */
s_bool s_test_period_abbreviation(const char *token_string, const char *item_string,
								  const char *item_post_punc, s_erc *error)
{
	const char *tmp;
	s_bool is_abbreviation;


	S_CLR_ERR(error);
	tmp = s_strchr(item_post_punc, '.', error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_test_period_abbreviation",
				  "Call to \"s_strchr\" failed"))
		return FALSE;

	if (tmp == NULL) /* post punc is not period, no break */
		return FALSE;

	tmp = s_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ", token_string[0], error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_test_period_abbreviation",
				  "Call to \"s_strchr\" failed"))
		return FALSE;

	if (tmp == NULL) /* token does not start with a capital letter */
		return FALSE;

	is_abbreviation = s_test_abbreviation(item_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_test_period_abbreviation",
				  "Call to \"s_test_abbreviation\" failed"))
		return FALSE;

	if (is_abbreviation) /* last word is abbreviation */
		return FALSE;

	return TRUE;
}


s_bool s_got_uttbreak(const SToken *token, const char *token_string,
					  const char *item_post_punc, const char *item_string, s_erc *error)
{
	s_bool test;
	const char *token_whitespace;


	S_CLR_ERR(error);
	token_whitespace = STokenGetWhitespace(token, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_got_uttbreak",
				  "Call to \"STokenGetWhitespace\" failed"))
		return FALSE;

	test = s_test_whitespace_double_line_breaks(token_whitespace, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_got_uttbreak",
				  "Call to \"s_test_whitespace_double_line_breaks\" failed"))
		return FALSE;

	if (test) /* got at least 2 linebreaks */
		return TRUE;

	test = s_test_unambiguous_postpunc(item_post_punc, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_got_uttbreak",
				  "Call to \"s_test_unambiguous_postpunc\" failed"))
		return FALSE;

	if (test) /* got at unambiguous post punctuation */
		return TRUE;

	test = s_test_period_capital(token_whitespace, token_string,
								 item_post_punc, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_got_uttbreak",
				  "Call to \"s_test_period_capital\" failed"))
		return FALSE;

	if (test) /* got period, at least 2 whitespaces and a capital letter */
		return TRUE;

	test = s_test_period_abbreviation(token_string, item_string,
									  item_post_punc, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_got_uttbreak",
				  "Call to \"s_test_period_abbreviation\" failed"))
		return FALSE;

	if (test) /* got period, item is not abbreviation and token starts with capital letter */
		return TRUE;

	return FALSE;
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SUttBreakUttProc *self = obj;


	S_CLR_ERR(error);
	self->white_space_chars = NULL;
	self->single_char_symbols = NULL;
	self->pre_punc_symbols = NULL;
	self->post_punc_symbols = NULL;
}


static void Destroy(void *obj, s_erc *error)
{
	SUttBreakUttProc *self = obj;


	S_CLR_ERR(error);
	if (self->white_space_chars != NULL)
		S_FREE(self->white_space_chars);

	if (self->single_char_symbols != NULL)
		S_FREE(self->single_char_symbols);

	if (self->pre_punc_symbols != NULL)
		S_FREE(self->pre_punc_symbols);

	if (self->post_punc_symbols != NULL)
		S_FREE(self->post_punc_symbols);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void Initialize(SUttProcessor *self, const SVoice *voice, s_erc *error)
{
	S_CLR_ERR(error);

	/* get tokenization symbols defined in voicefile, now as features
	 * of this utterance processor. If none, then defaults are used.
	 */
	s_get_tokenizer_symbols(S_UTTBREAK_UTTPROC(self), self->features, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Initialize",
			  "Call to \"s_get_tokenizer_symbols\" failed");

	S_UNUSED(voice);
}


static void SetTokenstreamSymbols(const SUttBreakUttProc *self, STokenstream *ts, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetTokenstreamSymbols",
				  "Argument \"self\" is NULL");
		return;
	}

	if (ts == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetTokenstreamSymbols",
				  "Argument \"ts\" is NULL");
		return;
	}

	/* set tokenization symbols of the token stream */
	STokenstreamSetWhitespaceChars(ts, self->white_space_chars, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SetTokenstreamSymbols",
				  "Call to \"STokenstreamSetWhitespaceChars\" failed"))
		return;

	STokenstreamSetSingleChars(ts, self->single_char_symbols, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SetTokenstreamSymbols",
				  "Call to \"STokenstreamSetSingleChars\" failed"))
		return;

	STokenstreamSetPrePuncChars(ts, self->pre_punc_symbols, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SetTokenstreamSymbols",
				  "Call to \"STokenstreamSetPrePuncChars\" failed"))
		return;

	STokenstreamSetPostPuncChars(ts, self->post_punc_symbols, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetTokenstreamSymbols",
			  "Call to \"STokenstreamSetPostPuncChars\" failed");
}


static SUtterance *GetNextUtt(const SUttBreakUttProc *self, const SVoice *voice,
							  STokenstream *ts, s_erc *error)
{
	SUtterance *utt = NULL;
	SRelation *tokenRel;
	s_bool eof;
	s_bool is_break = FALSE;
	const char *token_string;
	const SToken *token;
	char *item_post_punc = NULL;
	char *item_string = NULL;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetNextUtt",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (voice == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetNextUtt",
				  "Argument \"voice\" is NULL");
		return NULL;
	}

	if (ts == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetNextUtt",
				  "Argument \"ts\" is NULL");
		return NULL;
	}

	/* check that we are not at the end of the stream */
	eof = STokenstreamQueryEOF(ts, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetNextUtt",
				  "Call to \"STokenstreamQueryEOF\" failed"))
		return NULL;

	if (eof) /* nothing to do if at end of stream */
		return NULL;

	/* peek if first token is not a NULL string */
	token = STokenstreamPeekToken(ts, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetNextUtt",
				  "Call to \"STokenstreamPeekToken\" failed"))
		goto quit_error;

	token_string = STokenGetString(token, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetNextUtt",
				  "Call to \"STokenGetString\" failed"))
		goto quit_error;

	if (token_string == NULL)
	{
		/* it's a NULL string, nothing to do as there should?? not be
		 * any more tokens */
		return NULL;
	}

	/* ok, we can get this token */
	token = STokenstreamGetToken(ts, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetNextUtt",
				  "Call to \"STokenstreamGetToken\" failed"))
		return NULL;

	/* create new utterance */
	utt = S_NEW(SUtterance, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetNextUtt",
				  "Failed to create new utterance"))
		return NULL;

	/* initialize utterance */
	SUtteranceInit(&utt, voice, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetNextUtt",
				  "Failed to initialize new utterance"))
		return NULL;

	/* create token relation */
	tokenRel = SUtteranceNewRelation(utt, "Token", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetNextUtt",
				  "Call to \"SUtteranceNewRelation\" failed"))
		goto quit_error;

	/* add the first token */
	item_post_punc = s_add_token_to_relation(tokenRel, token, token_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetNextUtt",
				  "Call to \"s_add_token_to_relation\" failed"))
		goto quit_error;

	/* save this for futher processing */
	item_string = s_strdup(token_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetNextUtt",
				  "Call to \"s_strdup\" failed"))
		goto quit_error;

	while (!eof)
	{
		token = STokenstreamPeekToken(ts, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetNextUtt",
					  "Call to \"STokenstreamPeekToken\" failed"))
			goto quit_error;

		token_string = STokenGetString(token, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetNextUtt",
					  "Call to \"STokenGetString\" failed"))
			goto quit_error;

		if (token_string == NULL)
			break;

		is_break = s_got_uttbreak(token, token_string,
								  item_post_punc, item_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetNextUtt",
					  "Call to \"s_got_uttbreak\" failed"))
			goto quit_error;

		if (is_break) /* got utterance, break and return it */
			break;

		/* get token */
		token = STokenstreamGetToken(ts, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetNextUtt",
					  "Call to \"STokenstreamGetToken\" failed"))
			goto quit_error;

		S_FREE(item_post_punc);

		/* add the token to the utterance */
		item_post_punc = s_add_token_to_relation(tokenRel, token, token_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetNextUtt",
					  "Call to \"s_add_token_to_relation\" failed"))
			goto quit_error;

		S_FREE(item_string);
		item_string = s_strdup(token_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetNextUtt",
					  "Call to \"s_strdup\" failed"))
			goto quit_error;

		eof = STokenstreamQueryEOF(ts, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetNextUtt",
					  "Call to \"STokenstreamQueryEOF\" failed"))
			goto quit_error;
	}

	S_FREE(item_post_punc);
	S_FREE(item_string);

	return utt;

quit_error:
	if (item_string != NULL)
		S_FREE(item_string);

	if (item_post_punc != NULL)
		S_FREE(item_post_punc);

	if (utt != NULL)
		S_DELETE(utt, "GetNextUtt", error);

	return NULL;
}



/************************************************************************************/
/*                                                                                  */
/* SUttBreakUttProc class initialization                                            */
/*                                                                                  */
/************************************************************************************/

static SUttBreakUttProcClass UttBreakUttProcClass =
{
	{
		/* SObjectClass */
		{
			"SUttProcessor:SUttBreakUttProc",
			sizeof(SUttBreakUttProc),
			{ 0, 1},
			Init,            /* init    */
			Destroy,         /* destroy */
			Dispose,         /* dispose */
			NULL,            /* compare */
			NULL,            /* print   */
			NULL,            /* copy    */
		},
		/* SUttProcessorClass */
		Initialize,          /* initialize */
		NULL,                /* run        */
	},
	/* SUttBreakUttProcClass */
	SetTokenstreamSymbols,   /* set_tokenstream_symbols */
	GetNextUtt               /* get_next_utt            */
};
