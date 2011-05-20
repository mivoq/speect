/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
/* Example loading an utterance.                                                    */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


#include <stdio.h>
#include "speect.h"


static const char *utt_plugin_path = "utt_ebml.spi";


int main()
{
	s_erc error = S_SUCCESS;
	SUtterance *utt = NULL;
	SPlugin *uttEBMLplugin = NULL;


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
	uttEBMLplugin = s_pm_load_plugin(utt_plugin_path, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load plug-in at '%s'",utt_plugin_path))
	{
		printf("failed to load plug-in\n");
		goto quit;
	}
	else
	{
		printf("plug-in loaded\n");
	}

	/* load utterance */
	utt = (SUtterance*)SObjectLoad("test.utt", "spct_utt", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load utterance"))
		goto quit;

	/* and save it again for binary comparison */
	SObjectSave(S_OBJECT(utt), "re_test.utt", "spct_utt", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to save utterance"))
		goto quit;


quit:
	if (utt != NULL)
		S_DELETE(utt, "main", &error);

	/* unload plug-ins by deleting them */
	if (uttEBMLplugin != NULL)
		S_DELETE(uttEBMLplugin, "main", &error);

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


