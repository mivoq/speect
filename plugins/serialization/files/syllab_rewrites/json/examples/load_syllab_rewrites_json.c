/************************************************************************************/
/* Copyright (c) 2011 The Department of Arts and Culture,                           */
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
/* DATE    : May 2011                                                               */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Example loading a JSON format syllabification rewrites rule-set and              */
/* running it on a input phone list.                                                */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


#include <stdio.h>
#include "speect.h"
#include "syllabification.h"


static const char *syllabification_rewrites_json_plugin_path = "syllab_rewrites_json.spi";


int main()
{
	s_erc error = S_SUCCESS;
	SSyllabification *syllab = NULL;
	SPlugin *syllabRewritesJSONPlugin = NULL;
	SList *syls = NULL;
	SList *phones = NULL;
	char *tmp = NULL;
	SIterator *itr1 = NULL;
	SIterator *itr2 = NULL;


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
	 * load the syllabification rewrites json plug-in
	 */
	syllabRewritesJSONPlugin = s_pm_load_plugin(syllabification_rewrites_json_plugin_path,
												&error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load plug-in at '%s'",
				  syllabification_rewrites_json_plugin_path))
	{
		printf("failed to load plug-in\n");
		goto quit;
	}
	else
	{
		printf("plug-in loaded\n");
	}

	/* load syllabification */
	syllab = (SSyllabification*)SObjectLoad("syllabification.json",
											"spct_syllabification_rewrites_json",
											&error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load syllabification"))
		goto quit;

	/* setup a phone list */
	phones = (SList*)S_NEW("SListList", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create a new 'SList' object"))
		goto quit;

	/* add the phones "s i l a b i f i k ei sh _ n" to the list */
	SListAppend(phones, S_OBJECT(SObjectSetString("s",&error)), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSetString/SListAppend failed"))
		goto quit;

	SListAppend(phones, S_OBJECT(SObjectSetString("i",&error)), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSetString/SListAppend failed"))
		goto quit;

	SListAppend(phones, S_OBJECT(SObjectSetString("l",&error)), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSetString/SListAppend failed"))
		goto quit;

	SListAppend(phones, S_OBJECT(SObjectSetString("a",&error)), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSetString/SListAppend failed"))
		goto quit;

	SListAppend(phones, S_OBJECT(SObjectSetString("b",&error)), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSetString/SListAppend failed"))
		goto quit;

	SListAppend(phones, S_OBJECT(SObjectSetString("i",&error)), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSetString/SListAppend failed"))
		goto quit;

	SListAppend(phones, S_OBJECT(SObjectSetString("f",&error)), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSetString/SListAppend failed"))
		goto quit;

	SListAppend(phones, S_OBJECT(SObjectSetString("i",&error)), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSetString/SListAppend failed"))
		goto quit;

	SListAppend(phones, S_OBJECT(SObjectSetString("k",&error)), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSetString/SListAppend failed"))
		goto quit;

	SListAppend(phones, S_OBJECT(SObjectSetString("ei",&error)), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSetString/SListAppend failed"))
		goto quit;

	SListAppend(phones, S_OBJECT(SObjectSetString("sh",&error)), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSetString/SListAppend failed"))
		goto quit;

	SListAppend(phones, S_OBJECT(SObjectSetString("_",&error)), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSetString/SListAppend failed"))
		goto quit;

	SListAppend(phones, S_OBJECT(SObjectSetString("n",&error)), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSetString/SListAppend failed"))
		goto quit;


	/* call syllabify method on the syllabification rule-set */
	syls = S_SYLLABIFICATION_CALL(syllab, syllabify)(syllab, NULL, phones, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"syllabify\" method failed"))
		goto quit;

	/* get an iterator to the syllables list */
	itr1 = S_ITERATOR_GET(syls, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"S_ITERATOR_GET\" failed"))
		goto quit;

	for (/* NOP */; itr1 != NULL; itr1 = SIteratorNext(itr1))
	{
		const SList *syllable;


		syllable = (SList*)SIteratorObject(itr1, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"SIteratorObject\" failed"))
			goto quit;

		/* get an iterator to the phones in the syllable */
		itr2 = S_ITERATOR_GET(syllable, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"S_ITERATOR_GET\" failed"))
			goto quit;

		for (/* NOP */; itr2 != NULL; itr2 = SIteratorNext(itr2))
		{
			const SObject *phone;


			phone = SIteratorObject(itr2, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
						  "main",
						  "Call to \"SIteratorObject\" failed"))
				goto quit;

			tmp = SObjectPrint(phone, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
						  "main",
						  "Call to \"SObjectPrint\" failed"))
				goto quit;

			printf("%s ", tmp);
			S_FREE(tmp);
		}

		printf("\n");
	}


quit:
	if (tmp != NULL)
		S_FREE(tmp);

	if (itr1 != NULL)
		S_DELETE(itr1, "main", &error);

	if (itr2 != NULL)
		S_DELETE(itr2, "main", &error);

	if (phones != NULL)
		S_DELETE(phones, "main", &error);

	if (syls != NULL)
		S_DELETE(syls, "main", &error);

	if (syllab != NULL)
		S_DELETE(syllab, "main", &error);

	/* unload plug-ins by deleting them */
	if (syllabRewritesJSONPlugin != NULL)
		S_DELETE(syllabRewritesJSONPlugin, "main", &error);

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


