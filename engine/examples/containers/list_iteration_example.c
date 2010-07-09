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
/* DATE    : 26 November 2009                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SList iteration expressions example.                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


#include <stdio.h>
#include "speect.h"


int main()
{
	s_erc error = S_SUCCESS;
	SList *list = NULL;
	SIterator *itr = NULL;
	SObject *a = NULL;
	SObject *b = NULL;
	SObject *c = NULL;


	/*
	 * initialize speect
	 */
	error = speect_init();
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}

	/* Create a new list */
	list = (SList*)S_NEW("SListList", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new list"))
		goto quit;

	/*
	 * get iterator to list, should be NULL as there are no objects
	 * in the list
	 */
	itr = S_ITERATOR_GET(list, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to get iterator to list"))
		goto quit;

	/* Create some objects and put the into the list */
	a = SObjectSetInt(10, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to set int object"))
		goto quit;

	b = SObjectSetFloat(3.14, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to set float object"))
		goto quit;

	c = SObjectSetString("hello world", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to set string object"))
		goto quit;

	SListPush(list, a, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to add object to list"))
		goto quit;
	else
		a = NULL; /* object belongs to list now, we don't want to
				   * delete it directly.
				   */

	SListPush(list, b, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to add object to list"))
		goto quit;
	else
		b = NULL; /* object belongs to list now, we don't want to
				   * delete it directly.
				   */

	SListPush(list, c, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to add object to list"))
		goto quit;
	else
		c = NULL; /* object belongs to list now, we don't want to
				   * delete it directly.
				   */

	/*
	 * get iterator to list, should not be NULL as there are now
	 * objects in the list
	 */
	itr = S_ITERATOR_GET(list, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to get iterator to list"))
		goto quit;

	/* iterate through list objects and print them to stdout */
	for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
	{
		char *buf;
		const SObject *tmp;


		tmp = SIteratorObject(itr, &error);
		if (S_CHK_ERR(&error,  S_CONTERR,
					  "main",
					  "Failed to get list iterator object"))
			goto quit;

		buf = SObjectPrint(tmp, &error);
		if (S_CHK_ERR(&error,  S_CONTERR,
					  "main",
					  "Failed to print  object"))
			goto quit;

		printf("list object = %s\n", buf);
		S_FREE(buf);
	}

quit:
	if (list != NULL)
		S_DELETE(list, "main", &error);

	if (itr != NULL)
		S_DELETE(itr, "main", &error);

	if (a != NULL)
		S_DELETE(a, "main", &error);

	if (b != NULL)
		S_DELETE(b, "main", &error);

	if (c != NULL)
		S_DELETE(c, "main", &error);

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


