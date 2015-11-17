/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                Simone Daminato                                                   */
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
/* A Tokenization utterance processor.                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "tokenizer_proc.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static STokenizationUttProcClass TokenizationUttProcClass; /* STokenizationUttProc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void s_get_tokenizer_symbols(const SUttProcessor *uttProc, STokenstream *ts,
									s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_tokenization_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&TokenizationUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_tokenization_utt_proc_class_reg",
			  "Failed to register STokenizationUttProcClass");
}


S_LOCAL void _s_tokenization_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&TokenizationUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_tokenization_utt_proc_class_free",
			  "Failed to free STokenizationUttProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void s_get_tokenizer_symbols(const SUttProcessor *uttProc, STokenstream *ts,
									s_erc *error)
{
	s_bool have_symbols;
	const SMap *tokSymbols;
	const char *symbols;


	S_CLR_ERR(error);
	have_symbols = SUttProcessorFeatureIsPresent(uttProc, "tokenizer symbols", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"SUttProcessorFeatureIsPresent\" failed"))
		return;

	if (!have_symbols)
		return;  /* no tokenizer symbols defined */

	tokSymbols = S_CAST(SUttProcessorGetFeature(uttProc, "tokenizer symbols", error),
						SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Failed to get 'tokenizer symbols' SMap feature"))
		return;

	/* check if we have 'whitespace' symbols */
	have_symbols = SMapObjectPresent(tokSymbols, "whitespace", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"SMapObjectPresent\" failed"))
		return;

	if (have_symbols)
	{
		/* get them and set it in the tokenizer */
		symbols = SObjectGetString(SMapGetObject(tokSymbols, "whitespace", error),
								   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_tokenizer_symbols",
					  "Failed to get 'whitespace' symbols"))
			return;

		STokenstreamSetWhitespaceChars(ts, symbols, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_tokenizer_symbols",
					  "Failed to set 'whitespace' symbols"))
			return;
	}

	/* check if we have 'single-char' symbols */
	have_symbols = SMapObjectPresent(tokSymbols, "single-char", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"SMapObjectPresent\" failed"))
		return;

	if (have_symbols)
	{
		/* get them and set it in the tokenizer */
		symbols = SObjectGetString(SMapGetObject(tokSymbols, "single-char", error),
								   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_tokenizer_symbols",
					  "Failed to get 'single-char' symbols"))
			return;

		STokenstreamSetSingleChars(ts, symbols, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_tokenizer_symbols",
					  "Failed to set 'single-char' symbols"))
			return;
	}

	/* check if we have 'pre-punctuation' symbols */
	have_symbols = SMapObjectPresent(tokSymbols, "pre-punctuation", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"SMapObjectPresent\" failed"))
		return;

	if (have_symbols)
	{
		/* get them and set it in the tokenizer */
		symbols = SObjectGetString(SMapGetObject(tokSymbols, "pre-punctuation", error),
								   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_tokenizer_symbols",
					  "Failed to get 'pre-punctuation' symbols"))
			return;

		STokenstreamSetPrePuncChars(ts, symbols, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_tokenizer_symbols",
					  "Failed to set 'pre-punctuation' symbols"))
			return;
	}

	/* check if we have 'post-punctuation' symbols */
	have_symbols = SMapObjectPresent(tokSymbols, "post-punctuation", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_tokenizer_symbols",
				  "Call to \"SMapObjectPresent\" failed"))
		return;

	if (have_symbols)
	{
		/* get them and set it in the tokenizer */
		symbols = SObjectGetString(SMapGetObject(tokSymbols, "post-punctuation", error),
								   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_tokenizer_symbols",
					  "Failed to get 'post-punctuation' symbols"))
			return;

		STokenstreamSetPostPuncChars(ts, symbols, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_get_tokenizer_symbols",
					  "Failed to set 'post-punctuation' symbols"))
			return;
	}
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


static void Run(const SUttProcessor *self, SUtterance *utt,
				s_erc *error)
{
	const char *text;
	s_bool have_input;
	STokenstream *ts = NULL;
	SRelation *tokenRelation = NULL;
	s_bool eof;
	const SToken *token;
	const char *token_string;
	SItem *tokenItem;


	S_CLR_ERR(error);

	have_input = SUtteranceFeatureIsPresent(utt, "input", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceFeatureIsPresent\" failed"))
		goto quit_error;


	if (!have_input)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'input' feature of utterance");
		goto quit_error;
	}

	text = SObjectGetString(SUtteranceGetFeature(utt, "input", error),
							error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Failed to get string from 'input' feature"))
		goto quit_error;

	/* create string tokenizer */
	ts = (STokenstream*)S_NEW(STokenstreamString, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Failed to create string tokenizer"))
		goto quit_error;

	STokenstreamStringInit((STokenstreamString**)&ts,
						 text,
						 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Failed to initialize string tokenizer"))
		goto quit_error;

	s_get_tokenizer_symbols(self, ts, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_get_tokenizer_symbols\" failed"))
		goto quit_error;

	/* create token relation */
	tokenRelation = SUtteranceNewRelation(utt, "Token", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceNewRelation\" failed"))
		goto quit_error;

	eof = STokenstreamQueryEOF(ts, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"STokenstreamQueryEOF\" failed"))
		goto quit_error;

	while (!eof)
	{
		token = STokenstreamPeekToken(ts, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"STokenstreamPeekToken\" failed"))
			goto quit;

		token_string = STokenGetString(token, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"STokenGetString\" failed"))
			goto quit;

		if (token_string == NULL)
		{
			token_string = "";
		}
		token = STokenstreamGetToken(ts, error);
		if (S_CHK_ERR(error, S_CONTERR,
					"Run",
					"Call to \"STokenstreamGetToken\" failed"))
			goto quit;

		/* create item, NULL shared content */
		tokenItem = SRelationAppend(tokenRelation, NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SRelationAppend\" failed"))
			goto quit;

		/* item's name is the token string */
		SItemSetName(tokenItem, token_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemSetName\" failed"))
			goto quit;

		/* get white-space */
		token_string = STokenGetWhitespace(token, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"STokenGetWhitespace\" failed"))
			goto quit;

		if (token_string != NULL)
		{
			SItemSetString(tokenItem, "whitespace", token_string, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemSetString\" failed"))
				goto quit;
		}

		/* get pre-punctuation */
		token_string = STokenGetPrePunc(token, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to  \"STokenGetPrePunc\" failed"))
			goto quit;

		if (token_string != NULL)
		{
			SItemSetString(tokenItem, "prepunc", token_string, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemSetString\" failed"))
				goto quit;
		}

		/* get post-punctuation */
		token_string = STokenGetPostPunc(token, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"STokenGetPostPunc\" failed"))
			goto quit;

		if (token_string != NULL)
		{
			SItemSetString(tokenItem, "postpunc", token_string, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemSetString\" failed"))
				goto quit;
		}

		eof = STokenstreamQueryEOF(ts, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"STokenstreamQueryEOF\" failed"))
			goto quit_error;
	}

	/* here all is OK */
	goto quit;

	/* error clean-up code */
quit_error:
	SUtteranceDelRelation(utt, "Token", error);
	S_CHK_ERR(error, S_CONTERR,
			  "Run",
			  "Call to \"SUtteranceDelRelation\" failed");

	/* normal clean-up code */
quit:
	if (ts != NULL)
		S_DELETE(ts, "Run", error);
}


/************************************************************************************/
/*                                                                                  */
/* STokenizationUttProc class initialization                                        */
/*                                                                                  */
/************************************************************************************/

static STokenizationUttProcClass TokenizationUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:STokenizationUttProc",
		sizeof(STokenizationUttProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SUttProcessorClass */
	NULL,                /* initialize */
	Run                  /* run        */
};
