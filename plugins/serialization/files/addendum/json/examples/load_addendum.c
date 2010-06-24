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
/* Example loading a addendum and looking up a word, and adding a word.             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


#include <stdio.h>
#include "speect.h"
#include "addendum.h"


static const char *addendum_json_plugin_path = "addendum_json.spi";
static const char *test_word = "aby"; /* word lookup to test in addendum */

static const char * const test_word_phones[] =
{
	"ei",
	"b",
	"ii",
	NULL
};



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


static void print_word(const SList *wordInfo, s_erc *error)
{
	char buf[1024] = "\0";


	S_CLR_ERR(error);
	print_list(wordInfo, buf, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "print_word",
				  "Call to \"print_list\" failed"))
		return;

	printf("%s = %s\n", test_word, buf);
}


static void create_new_entry(SAddendum *addendum, s_erc *error)
{
	SMap *features = NULL;
	SList *phones = NULL;
	int i;
	SObject *stringObject = NULL;


	S_CLR_ERR(error);

	/* create a word features map */
	features = (SMap*)S_NEW("SMapList", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_new_entry",
				  "Failed to create new 'SMapList' object"))
		goto quit_error;

	/* now create a phones list */
	phones = (SList*)S_NEW("SListList", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_new_entry",
				  "Failed to create new 'SListList' object"))
		goto quit_error;

	SMapSetObject(features, "phones", S_OBJECT(phones), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_new_entry",
				  "Failed to initialize new 'SListList' object"))
	{
		S_DELETE(phones, "create_new_entry", error);
		goto quit_error;
	}

	/* add the phones to list */
	for (i = 0; test_word_phones[i] != NULL; i++)
	{
		/* create a string object */
		stringObject = SObjectSetString(test_word_phones[i], error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_new_entry",
					  "Call to \"SObjectSetString\" failed"))
			goto quit_error;

		/* add string object to list */
		SListAppend(phones, stringObject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_new_entry",
					  "Call to \"SListAppend\" failed"))
		{
			S_DELETE(stringObject, "create_new_entry", error);
			goto quit_error;
		}
	}

	/* add entry to addendum */
	S_ADDENDUM_CALL(addendum, add_word)(addendum, test_word, features, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_new_entry",
				  "Call to method \"add_word\" failed"))
		goto quit_error;

	return;

quit_error:
	if (features != NULL)
		S_DELETE(features, "create_new_entry", error);
}



int main()
{
	s_erc error = S_SUCCESS;
	SAddendum *addendum = NULL;
	SPlugin *addendumJSONPlugin = NULL;
	SList *wordInfo = NULL;
	s_bool syllabified;


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
	 * load the addendum-json plug-in
	 */
	addendumJSONPlugin = s_pm_load_plugin(addendum_json_plugin_path, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load plug-in at '%s'", addendum_json_plugin_path))
	{
		printf("failed to load plug-in\n");
		goto quit;
	}
	else
	{
		printf("plug-in loaded\n");
	}

	/* load addendum */
	addendum = (SAddendum*)SObjectLoad("addendum_example.txt", "spct_addendum", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load addendum"))
		goto quit;

	/* get a word from the addendum, NULL features (get first match) */
	wordInfo = S_ADDENDUM_CALL(addendum, get_word)(addendum, test_word, NULL, &syllabified,
											 &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to method \"get_word\" failed"))
		goto quit;

	if (wordInfo != NULL)
	{
		printf("found word\n");
		print_word(wordInfo, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"print_word\" failed"))
			goto quit;
		S_DELETE(wordInfo, "main", &error);
	}
	else
	{
		printf("word '%s' not found, adding it\n", test_word);
		create_new_entry(addendum, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"create_new_entry\" failed"))
			goto quit;

		/* now search for word again */
		printf("searching for added word '%s'\n", test_word);
		wordInfo = S_ADDENDUM_CALL(addendum, get_word)(addendum, test_word, NULL, &syllabified,
													   &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to method \"get_word\" failed"))
			goto quit;

		if (wordInfo == NULL)
		{
			printf("failed to find added word, error\n");
			goto quit;
		}
		else
		{
			printf("found added word\n");
			print_word(wordInfo, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
						  "main",
						  "Call to \"print_word\" failed"))
				goto quit;
			S_DELETE(wordInfo, "main", &error);
		}
	}

quit:
	if (wordInfo != NULL)
		S_DELETE(wordInfo, "main", &error);

	if (addendum != NULL)
		S_DELETE(addendum, "main", &error);

	/* unload plug-ins by deleting them */
	if (addendumJSONPlugin != NULL)
		S_DELETE(addendumJSONPlugin, "main", &error);

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


