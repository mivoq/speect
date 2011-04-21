/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* DATE    : 18 November 2009                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Simple SDatasource writing example.                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "speect.h"


int main()
{
	s_erc error = S_SUCCESS;
	SDatasource *ds;
	const char *text = "hello world";

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
	 * open file data source
	 */
	ds = SFilesourceOpenFile("hello_word.txt", "w", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to open data source"))
		goto quit;

	/*
	 * Write to data source
	 */
	SDatasourceWrite(ds, text, sizeof(char), s_strsize(text, &error), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to write to data source"))
	{
		S_DELETE(ds, "main", &error);
		goto quit;
	}

	/*
	 * Delete data source, will close file as well
	 */
	S_DELETE(ds, "main", &error);

quit:
	/*
	 * quit speect
	 */
	error = speect_quit();
	if (error != S_SUCCESS)
	{
		printf("Call to 'speect_quit' failedn");
		return 1;
	}

	return 0;
}
