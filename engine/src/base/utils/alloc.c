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
/* DATE    : 25 March 2008                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Memory Allocation Abstraction.                                                   */
/* We define wrappers to the standard library function for in case we want to       */
/* change something or do some memory tests.                                        */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdlib.h>
#include "base/utils/alloc.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/* wrapper to malloc */
S_API void *_s_malloc(size_t len)
{
	void *p;


	if (len == 0)
		return NULL;

	p = malloc(len);

	return p;
}


/* wrapper to calloc */
S_API void *_s_calloc(size_t len)
{
	void *p;


	if (len == 0)
		return NULL;

	p = calloc(len, 1);

	return p;
}


/* wrapper to realloc */
S_API void *_s_realloc(void *p_old, size_t len)
{
	void *p_new = NULL;


	if (p_old == NULL)
		p_new = _s_malloc(len);
	else if (len != 0)
		p_new = realloc(p_old, len);

	if (p_new == NULL)
	{
		_s_free(p_old);
	}

	return p_new;
}


/* wrapper to free */
S_API void _s_free(void *p)
{
	if (p != NULL)
		free(p);
}

