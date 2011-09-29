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
/* DATE    : 26 November 2009                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* HRG example. Creating an utterance and a relation, adding items.                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


#include <stdio.h>
#include "speect.h"


int main()
{
	s_erc error = S_SUCCESS;
	SUtterance *utt;
	SRelation *rel;
	SItem *item;
	SItem *item_daughter;


	/*
	 * initialize speect
	 */
	error = speect_init(NULL);
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}

	/* Create a new utterance */
	utt = S_NEW(SUtterance, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new utterance"))
		goto quit;

	/* initialize utterance */
	SUtteranceInit(&utt, NULL, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to initialize new utterance"))
		goto quit;

	/* Create a new relation */
	rel = S_NEW(SRelation, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new relation"))
		goto quit;

	/* initialize relation */
	SRelationInit(&rel, "Word", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to initialize new relation"))
		goto quit;

	/* create an item, NULL shared contents */
	item = SRelationAppend(rel, NULL, &error);

	/* delete the relation using S_FORCE_DELETE, as the relation is
	 * not connected to an utterance
	 */
	S_FORCE_DELETE(rel, "main", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to delete new relation"))
		goto quit;

	/* Create a new relation */
	rel = S_NEW(SRelation, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new relation"))
		goto quit;

	/* initialize relation */
	SRelationInit(&rel, "Word", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to initialize new relation"))
		goto quit;

	/* create an item, NULL shared contents */
	item = SRelationAppend(rel, NULL, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new appended item"))
		goto quit;

	/* connect relation to utterance */
	SUtteranceSetRelation(utt, rel, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to connect new relation to utterance"))
		goto quit;
	else
		rel = NULL; /* clear reference so that "goto quit" works */

	/* add daughter item */
	item_daughter = SItemAddDaughter(item, NULL, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new daughter item"))
		goto quit;

	/* Create a new relation with SUtteranceNewRelation */
	rel = SUtteranceNewRelation(utt, "Syllable", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new relation"))
		goto quit;

	/* create an item, NULL shared contents */
	item = SRelationAppend(rel, item_daughter, &error);
	rel = NULL; /* relation is connected to utterance */
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new appended item"))
		goto quit;

quit:
	/*
	 * utterance will delete the
	 * relation which in turn will
	 * delete it's items.
	 */
	if (utt != NULL)
		S_DELETE(utt, "main", &error);

	if (rel != NULL)
		S_FORCE_DELETE(rel, "main", &error);

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


