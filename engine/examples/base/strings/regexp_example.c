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
/* DATE    : 17 November 2009                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Regular expressions example.                                                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "speect.h"


int main()
{
	s_erc error = S_SUCCESS;
	const char *string = "A soufflé is a light, fluffy, baked cake.";

	/* Groups             g(1) g(2)  g(3)  */
	const char *regex = "(.*) (.*é) (.*)";
	s_regexsub *rsub;
	s_regex *rx;
	int rv;


	/*
	 * initialize speect
	 */
	error = speect_init(NULL);
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}

	/*
	 * compile regular expression, "." metacharacter does not match newline.
	 */
	rx = s_regex_comp(regex, S_DOT_EXCLD_NEWLINE, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to compile regular expression"))
		goto quit;

	/*
	 * try to match the compiled regular expression with
	 * the string "A soufflé is a light, fluffy, baked cake."
	 * The second group, "(.*é)" must match the word "soufflé".
	 */
	rv = s_regex_match(rx, string, &rsub, &error);
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
	 * Group 0 is always the whole match, i.e. "A soufflé is a light, fluffy, baked cake."
	 * Group 1 should be "A"
	 * Group 2 should be "soufflé"
	 * Group 3 should be "is a light, fluffy, baked cake."
	 *
	 * Note that the spaces around "soufflé" are not grouped.
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


