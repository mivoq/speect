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


int main()
{
	s_erc error;
	STokenizer *ts = NULL;
	s_bool eof;
	const SToken *token;
	const char *tmp;
	const char *string_to_tokenize = "\"my (ma) se $10.00 en!, ek abý sal? [en;]   jy.\"";


	S_CLR_ERR(&error);

	/*
	 * initialize speect
	 */
	error = speect_init(NULL);
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}

	/* create string tokenizer */
	ts = (STokenizer*)S_NEW(STokenizerString, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new string tokenizer"))
		goto quit;

	/* initialize string tokenizer */
	STokenizerStringInit((STokenizerString**)&ts, string_to_tokenize, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to initialize string tokenizer"))
		goto quit;

	eof = STokenizerQueryEOF(ts, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to query end of file"))
		goto quit;

	while (!eof)
	{
		token = STokenizerPeekToken(ts, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to peek token"))
			goto quit;

		tmp = STokenGetString(token, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to get token string"))
			goto quit;

		if (tmp == NULL)
			break;
		else
		{
			token = STokenizerGetToken(ts, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
						  "main",
						  "Failed to get token"))
				goto quit;
		}

		printf("\nTOKEN:\n");

		/* white space */
		tmp = STokenGetWhitespace(token, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to get token white-space"))
			goto quit;

		if (tmp != NULL)
			printf("token whitespace = \"%s\".\n", tmp);
		else
			printf("token whitespace = NULL.\n");

		/* pre-punctuation */
		tmp = STokenGetPrePunc(token, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to get token pre-punctuation"))
			goto quit;

		if (tmp != NULL)
			printf("token pre-punctuation = \"%s\".\n", tmp);
		else
			printf("token pre-punctuation = NULL.\n");

		/* token string */
		tmp = STokenGetString(token, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to get token string"))
			goto quit;

		if (tmp != NULL)
			printf("token string = \"%s\".\n", tmp);
		else
			printf("token string = NULL.\n");

		/* post-punctuation */
		tmp = STokenGetPostPunc(token, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to get token post-punctuation"))
			goto quit;

		if (tmp != NULL)
			printf("token post-punctuation = \"%s\".\n", tmp);
		else
			printf("token post-punctuation = NULL.\n");

		eof = STokenizerQueryEOF(ts, &error);
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
