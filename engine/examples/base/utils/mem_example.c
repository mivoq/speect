/************************************************************************************/
/* Copyright (c) 2008-2011 The Department of Arts and Culture,                      */
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
/* DATE    : 17 November 2009                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Memory allocation/free example.                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#include <stdlib.h> /* for abort() */
#include "speect.h"

/* Memory Handling Examples */

int main(void)
{
	int *counter;
	char **cpoint1;
	char **cpoint2;

	/*
	 * note that TYPE can be either a reference to a
	 * specific variable or to a data type. Internally
	 * the macro uses the sizeof() operator.
	 */
	counter = S_MALLOC(counter, 10);
	counter[4] = 0;
	S_FREE(counter);

	counter = S_MALLOC(int, 10);
	counter[1] = 0;
	S_FREE(counter);

	cpoint1 = S_CALLOC(char*, 2);
	cpoint1 = S_REALLOC(cpoint1, char*, 5);
	S_FREE(cpoint1);

	cpoint2 = S_CALLOC(char*, 12);
	S_FREE(cpoint2);

	/*
	 * SPCT_FREE sets the pointed to variable to NULL
	 * therefore this test should not abort.
	 */
	if (cpoint1 != cpoint2)
		abort();

	return 0;
}


