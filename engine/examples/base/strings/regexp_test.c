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
/* DATE    : December 2012                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Regular expressions testing.                                                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "speect.h"


/************************************************************************************/
/*                                                                                  */
/*  Static function implementations                                                 */
/*                                                                                  */
/************************************************************************************/

static void usage(int rv)
{
    printf("usage: regexp_test -r REGEXP -t TEXT\n"
           "  Compiles regular expression REGEXP and tries to match it on TEXT.\n"
		   "  None of the arguments are optional.\n"
           "  --help      Output usage string\n");
	exit(rv);
}


/************************************************************************************/
/*                                                                                  */
/*  Main function                                                                   */
/*                                                                                  */
/************************************************************************************/

int main(int argc, char **argv)
{
	s_erc error = S_SUCCESS;
	s_regexsub *rsub;
	s_regex *rx;
	int rv;
	int i;
	int scomp;
	const char *text = NULL;
	const char *regexp = NULL;


	/*
	 * initialize speect
	 */
	error = speect_init(NULL);
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}

	/* parse options */
    for (i=1; i<argc; i++)
    {
		scomp = s_strcmp(argv[i],"-h", &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_strcmp\" failed"))
			return 1;

		if (scomp == 0)
			usage(0);

		scomp = s_strcmp(argv[i],"--help", &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_strcmp\" failed"))
			return 1;

		if (scomp == 0)
			usage(0);

		scomp = s_strcmp(argv[i],"-t", &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_strcmp\" failed"))
			return 1;

		if ((scomp == 0) && (i + 1 < argc))
		{
			text = argv[i+1];
			i++;
		}

		scomp = s_strcmp(argv[i],"-r", &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_strcmp\" failed"))
			return 1;

		if ((scomp == 0) && (i + 1 < argc))
		{
			regexp = argv[i+1];
			i++;
		}
	}

	if ((regexp == NULL) || (text == NULL))
	{
		S_CTX_ERR(&error, S_ARGERROR,
				  "main",
				  "Arguments are not optional, see usage");
		usage(1);
	}

	/*
	 * compile regular expression, "." metacharacter does not match newline.
	 */
	rx = s_regex_comp(regexp, S_DOT_EXCLD_NEWLINE, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to compile regular expression"))
		goto quit;

	/*
	 * try to match the compiled regular expression with
	 * the given string.
	 */
	rv = s_regex_match(rx, text, &rsub, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_regex_match\" failed"))
	{
		S_FREE(rx);
		goto quit;
	}

	if (rv > 0)
	{
		printf("string matched\n");
	}
	else if (rv < 0)
	{
		printf("ran out of space\n");
	}
	else
	{
		printf("no match\n");
	}


	/*
	 * get sub-string group matches
	 */
	if (rv > 0)
	{
		uint8 num_matches;
		uint i;


		/*
		 * get number of groups
		 */
		num_matches = s_regexsub_num_groups(rsub, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to get number of sub-match groups"))
		{
			S_FREE(rx);
			S_FREE(rsub);
			goto quit;
		}

		for (i = 0; i < num_matches; i++)
		{
			char *sub_string;


			/*
			 * get specific group and print. Group 0 is always
			 * the whole match.
			 */
			sub_string = s_regexsub_group(rsub, i, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
						  "main",
						  "Failed to get sub-match group %d", i))
			{
				S_FREE(rx);
				S_FREE(rsub);
				goto quit;
			}

			if (sub_string != NULL)
			{
				printf("group [%d] = \"%s\"\n", i, sub_string);
				S_FREE(sub_string);
			}
			else
				printf("group [%d] = NULL\n", i);
		}
	}

	/*
	 * free memory of regexp and sub-matches
	 */
	S_FREE(rx);
	S_FREE(rsub);


quit:
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


