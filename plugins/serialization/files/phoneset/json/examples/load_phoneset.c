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
/* Example loading a phoneset and looking up a phone feature.                       */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


#include <stdio.h>
#include "speect.h"
#include "phoneset.h"


static const char *phoneset_json_plugin_path = "phoneset_json.spi";

static const char *test_phone = "E_ri";
static const char *test_phone_feature_true = "vowel";
static const char *test_phone_feature_false = "place_post-alveolar";



int main()
{
	s_erc error = S_SUCCESS;
	SPhoneset *phoneset = NULL;
	SPlugin *phonesetJSONPlugin = NULL;
	s_bool is_true;


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
	 * load the phoneset-json plug-in
	 */
	phonesetJSONPlugin = s_pm_load_plugin(phoneset_json_plugin_path, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load plug-in at '%s'", phoneset_json_plugin_path))
	{
		printf("failed to load plug-in\n");
		goto quit;
	}
	else
	{
		printf("plug-in loaded\n");
	}

	/* load phoneset */
	phoneset = (SPhoneset*)SObjectLoad("phoneset_example.txt", "spct_phoneset", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load phoneset"))
		goto quit;

	is_true = S_PHONESET_CALL(phoneset, has_phone)(phoneset, test_phone, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to method \"has_phone\" failed"))
		goto quit;

	printf("tested if phoneset has phone '%s', answer = '%s' (should be TRUE)\n",
		   test_phone, is_true? "TRUE" : "FALSE");

	is_true = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, test_phone,
														   test_phone_feature_true,
														   &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to method \"phone_has_feature\" failed"))
		goto quit;

	printf("tested if phoneset phone '%s' has feature '%s', answer = '%s' (should be TRUE)\n",
		   test_phone, test_phone_feature_true, is_true? "TRUE" : "FALSE");

	is_true = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset, test_phone,
														   test_phone_feature_false,
														   &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to method \"phone_has_feature\" failed"))
		goto quit;

	printf("tested if phoneset phone '%s' has feature '%s', answer = '%s' (should be FALSE)\n",
		   test_phone, test_phone_feature_false, is_true? "TRUE" : "FALSE");


quit:
	if (phoneset != NULL)
		S_DELETE(phoneset, "main", &error);

	/* unload plug-ins by deleting them */
	if (phonesetJSONPlugin != NULL)
		S_DELETE(phonesetJSONPlugin, "main", &error);

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


