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
/* DATE    : 5 June 2009                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Time related functions and definitions.                                          */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <time.h>
#include "base/strings/strings.h"
#include "base/utils/alloc.h"
#include "base/utils/time.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static uint initialized_count = 0;

S_DECLARE_MUTEX_STATIC(time_mutex);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_time_init(s_erc *error)
{
	S_CLR_ERR(error);

	if (initialized_count++ > 0)
		return;

	/* init mutex */
	s_mutex_init(&time_mutex);
}


S_LOCAL void _s_time_quit(s_erc *error)
{
	S_CLR_ERR(error);

	if ((initialized_count == 0)
		|| (--initialized_count > 0))
		return;

	/* destroy mutex (for s_strtime) */
	s_mutex_destroy(&time_mutex);
}


S_API char *s_strtime(s_erc *error)
{
	time_t ltm;
	const struct tm *ltm_t;
	char datestring[256];
	char *rv;


	S_CLR_ERR(error);

	if (initialized_count == 0)
		S_ERR_PRINT(S_FAILURE, "s_strtime",
					"Time module has not been initialized.\n"
					"This will lead to undefined behaviour. Call s_time_init(...)\n");

	ltm = time(NULL); /* mt-safe */

	if (ltm == -1)
	{
		/* this is some or other system error */
		S_NEW_ERR(error, S_FAILURE);
		S_ERR_PRINT(S_FAILURE, "s_strtime",
					"Call to c-library function \"time\" failed");
		return NULL;
	}

	/* lock mutex */
	s_mutex_lock(&time_mutex);
	ltm_t = localtime(&ltm);

	if (ltm_t == NULL)
	{
		/* this is some or other system error */
		S_NEW_ERR(error, S_FAILURE);
		S_ERR_PRINT(S_FAILURE, "s_strtime",
					"Call to c-library function \"localtime\" failed");
		/* unlock mutex */
		s_mutex_unlock(&time_mutex);
		return NULL;
	}

	strftime(datestring, sizeof(datestring), "%c", ltm_t);

	rv = s_strdup_clib(datestring);
	if (rv == NULL)
	{
		S_NEW_ERR(error, S_FAILURE);
		S_ERR_PRINT(S_FAILURE, "s_strtime",
					"Call to \"s_strdup_clib\" failed");
		s_mutex_unlock(&time_mutex);
		return NULL;
	}

	/* unlock mutex */
	s_mutex_unlock(&time_mutex);
	return rv;
}
