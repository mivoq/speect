/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* Simple string tokenization example.                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "speect.h"
#include "tokenizer_string.h"


static const char *plugin_path = "tokenizer.spi";


int main()
{
	s_erc error;
	SPlugin *plugin = NULL;
	STokenizer *ts = NULL;
	s_bool eof;
	SToken *token;
	const char *tmp;
	const char *string_to_tokenize = "\"my (ma) se $10.00 en!, ek abý sal? [en;]   jy.\"";


	S_CLR_ERR(&error);

	/*
	 * initialize speect
	 */
	error = speect_init();
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}

	/*
	 * load the tokenizer plug-in
	 */
	plugin = s_pm_load_plugin(plugin_path, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load plug-in at '%s'", plugin_path))
	{
		printf("failed to load plug-in\n");
		goto quit;
	}
	else
	{
		printf("plug-in loaded\n");
	}

	/* create string tokenizer */
	ts = (STokenizer*)S_NEW("STokenizerString", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new string tokenizer"))
		goto quit;

	/* initialize string tokenizer */
	S_TOKENIZER_STRING_CALL(S_TOKENIZER_STRING(ts), init)((STokenizerString**)&ts,
														  string_to_tokenize, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to initialize string tokenizer"))
		goto quit;

	eof = S_TOKENIZER_CALL(ts, query_eof)(ts, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to query end of file"))
		goto quit;

	while (!eof)
	{
		token = S_TOKENIZER_CALL(ts, peek_token)(ts, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to peek token"))
			goto quit;

		tmp = S_TOKEN_CALL(token, get_string)(token, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to get token string"))
			goto quit;

		if (tmp == NULL)
			break;
		else
		{
			token = S_TOKENIZER_CALL(ts, get_token)(ts, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
						  "main",
						  "Failed to get token"))
				goto quit;
		}

		printf("\nTOKEN:\n");

		/* white space */
		tmp = S_TOKEN_CALL(token, get_whitespace)(token, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to get token white-space"))
			goto quit;

		if (tmp != NULL)
			printf("token whitespace = \"%s\".\n", tmp);
		else
			printf("token whitespace = NULL.\n");

		/* pre-punctuation */
		tmp = S_TOKEN_CALL(token, get_pre_punc)(token, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to get token pre-punctuation"))
			goto quit;

		if (tmp != NULL)
			printf("token pre-punctuation = \"%s\".\n", tmp);
		else
			printf("token pre-punctuation = NULL.\n");

		/* token string */
		tmp = S_TOKEN_CALL(token, get_string)(token, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to get token string"))
			goto quit;

		if (tmp != NULL)
			printf("token string = \"%s\".\n", tmp);
		else
			printf("token string = NULL.\n");

		/* post-punctuation */
		tmp = S_TOKEN_CALL(token, get_post_punc)(token, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to get token post-punctuation"))
			goto quit;

		if (tmp != NULL)
			printf("token post-punctuation = \"%s\".\n", tmp);
		else
			printf("token post-punctuation = NULL.\n");

		eof = S_TOKENIZER_CALL(ts, query_eof)(ts, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to query end of file"))
			goto quit;
	}


	/*
	 * quit
	 */
quit:
	if (ts != NULL)
		S_DELETE(ts, "main", &error);

	/* must be after tokenizer */
	if (plugin != NULL)
		S_DELETE(plugin, "main", &error);

	/*
	 * quit speect
	 */
	error = speect_quit();
	if (error != S_SUCCESS)
	{
		printf("Call to 'speect_quit' failed\n");
		return 1;
	}


	return 0;
}
