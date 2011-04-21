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
/* Example loading a g2p rewrites ebml.                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


#include <stdio.h>
#include "speect.h"
#include "g2p.h"


static const char *g2p_rewrites_plugin = "g2p_rewrites.spi";
static const char *test_word = "computer";


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
	SG2P *g2p = NULL;
	SPlugin *plugin = NULL;
	SList *wordInfo = NULL;


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
	 * load the utt-ebml plug-in
	 */
	plugin = s_pm_load_plugin(g2p_rewrites_plugin, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load plug-in at '%s'", g2p_rewrites_plugin))
	{
		printf("failed to load plug-in\n");
		goto quit;
	}
	else
	{
		printf("plug-in loaded\n");
	}

	/* load g2p */
	g2p = (SG2P*)SObjectLoad("lwazi_english_g2p.spct", "spct_g2p_rewrites", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load g2p"))
		goto quit;

	/* test g2p */
	wordInfo = S_G2P_CALL(g2p, apply)(g2p, test_word, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to method \"apply\" failed"))
		goto quit;

	if (wordInfo == NULL)
	{
		S_CTX_ERR(&error, S_FAILURE,
				  "main",
				  "G2P failed to return a phone list for word '%s'",
				  test_word);
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
	}


quit:
	if (wordInfo != NULL)
		S_DELETE(wordInfo, "main", &error);

	if (g2p != NULL)
		S_DELETE(g2p, "main", &error);

	/* unload plug-ins by deleting them */
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


