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
/* Example loading a lexicon and looking up a word.                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


#include <stdio.h>
#include "speect.h"
#include "lexicon.h"


static const char *lexicon_json_plugin_path = "lexicon_json.spi";
static const char *test_word = "counterrevolutionary"; /* word lookup to test in lexicon */



static void print_list(const SList *list, char *buf, s_erc *error)
{
	SIterator *itr;
	size_t list_size;


	S_CLR_ERR(error);

	list_size = SListSize(list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "print_list",
				  "Call to \"list_size\" failed"))
		return;

	s_strcat(buf, "[", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "print_list",
				  "Call to \"s_strcat\" failed"))
		return;

	itr = S_ITERATOR_GET(list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "print_list",
				  "Call to \"S_ITERATOR_GET\" failed"))
		return;

	for (/* NOP */; itr != NULL; itr = SIteratorNext(itr), --list_size)
	{
		s_bool type_ok;
		const SObject *tmp;


		tmp = SIteratorObject(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "print_list",
					  "Call to \"SIteratorObject\" failed"))
		{
			S_DELETE(itr, "print_list", error);
			return;
		}

		type_ok = SObjectIsType(tmp, "SList", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "print_word",
					  "Call to \"SObjectIsType\" failed"))
		{
			S_DELETE(itr, "print_list", error);
			return;
		}

		if (type_ok)
		{
			print_list(S_LIST(tmp), buf, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "print_list",
						  "Call to \"print_list\" failed"))
			{
				S_DELETE(itr, "print_list", error);
				return;
			}
		}
		else
		{
			type_ok = SObjectIsType(tmp, "SString", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "print_list",
						  "Call to \"SObjectIsType\" failed"))
			{
				S_DELETE(itr, "print_list", error);
				return;
			}

			if (type_ok)
			{
				const char *string = SObjectGetString(S_OBJECT(tmp), error);

				if (S_CHK_ERR(error, S_CONTERR,
							  "print_list",
							  "Call to \"SObjectGetString\" failed"))
				{
					S_DELETE(itr, "print_list", error);
					return;
				}
				s_strcat(buf, string, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "print_list",
							  "Call to \"s_strcat\" failed"))
				{
					S_DELETE(itr, "print_list", error);
					return;
				}

				if ((list_size-1) != 0)
				{
					s_strcat(buf, " ", error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "print_list",
								  "Call to \"s_strcat\" failed"))
					{
						S_DELETE(itr, "print_list", error);
						return;
					}
				}
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "print_list",
						  "Unknown type in list");
				{
					S_DELETE(itr, "print_list", error);
					return;
				}
			}
		}
	}

	s_strcat(buf, "]", error);
	S_CHK_ERR(error, S_CONTERR,
			  "print_list",
			  "Call to \"s_strcat\" failed");
}


int main()
{
	s_erc error = S_SUCCESS;
	SLexicon *lex = NULL;
	SPlugin *lexiconJSONPlugin = NULL;
	SList *wordInfo;
	s_bool syllabified;


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

	/*
	 * load the lexicon-json plug-in
	 */
	lexiconJSONPlugin = s_pm_load_plugin(lexicon_json_plugin_path, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load plug-in at '%s'", lexicon_json_plugin_path))
	{
		printf("failed to load plug-in\n");
		goto quit;
	}
	else
	{
		printf("plug-in loaded\n");
	}

	/* load lexicon */
	lex = (SLexicon*)SObjectLoad("lexicon_example.txt", "spct_lexicon", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load lexicon"))
		goto quit;

	/* get a word from the lexicon, NULL features (get first match) */
	wordInfo = S_LEXICON_CALL(lex, get_word)(lex, test_word, NULL, &syllabified,
											 &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to method \"get_word\" failed"))
		goto quit;

	if (wordInfo == NULL)
	{
		printf("Word '%s' not in lexicon\n", test_word);
		goto quit;
	}
	else
	{
		char buf[1024] = "\0";


		print_list(wordInfo, buf, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"print_list\" failed"))
			goto quit;

		printf("%s = %s\n", test_word, buf);
		S_DELETE(wordInfo, "main", &error);
	}

quit:
	if (lex != NULL)
		S_DELETE(lex, "main", &error);

	/* unload plug-ins by deleting them */
	if (lexiconJSONPlugin != NULL)
		S_DELETE(lexiconJSONPlugin, "main", &error);

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


