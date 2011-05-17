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
/* looking up the syllabification rule-set's description.                           */
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
	const char *description = NULL;
	SList *syls;
	SList *phones;
	char *tmp;



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

	phones = (SList*)S_NEW("SListList", &error);
	SListAppend(phones, S_OBJECT(SObjectSetString("a",&error)), &error);
	SListAppend(phones, S_OBJECT(SObjectSetString("nb",&error)), &error);
	SListAppend(phones, S_OBJECT(SObjectSetString("y",&error)), &error);



	syls = S_SYLLABIFICATION_CALL(syllab, syllabify)(syllab, NULL, phones, &error);
	tmp = SObjectPrint(S_OBJECT(syls), &error);

	printf("syls = %s\n", tmp);
	S_FREE(tmp);

	if (syls != NULL)
		S_DELETE(syls, "main", &error);

	S_DELETE(phones, "main", &error);




	description = S_SYLLABIFICATION_CALL(syllab, get_description)(syllab, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to method \"get_description\" failed"))
		goto quit;

	printf("description of syllabification rewrites rule-set is: '%s'\n", description);

quit:
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


