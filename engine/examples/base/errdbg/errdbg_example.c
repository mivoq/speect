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
/* DATE    : 6 June 2009                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Error handling and debugging example.                                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "speect.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

void deep_nested(int i, s_erc *error)
{
	/*
	 * a deeply nested function may not
	 * have enough context to set an appropriate
	 * error message, so it can just set an error.
	 */
	if (i != 151623)
	{
		S_NEW_ERR(error, S_FAILURE);
		return;
	}

	S_CLR_ERR(error);
}


int *make_numbers(int d, s_erc *error)
{
	int *n;

	n = S_MALLOC(n, 1);
	if (n == NULL)
	{
		/*
		 * fatal errors abort if the build switch
		 * -DERROR_ABORT_FATAL=1 is used during cmake configuration,
		 * see the README
		 */
		S_FTL_ERR(error, S_MEMERROR,
			     "make_numbers",
			     "Failed to allocate memory");
		return NULL;
	}

	/* set a debug message with a debug level */
	S_DEBUG(S_DBG_INFO, "d is %d", d);

	if (d == 0)
		*n = 12;
	else
		*n = 151623;

	/*
	 * call 'deep_nested' with
	 * the error variable
	 */
	deep_nested(*n, error);

	/*
	 * check the error status received from
	 * function 'deep_nested', and set an
	 * error with some context information
	 * if 'deep_nested' did not return S_SUCCESS
	 *
	 * In this example we use S_CONTERR, which will
	 * just pass on the error received from 'deep_nested'
	 */
	if (S_CHK_ERR(error, S_CONTERR,
			 "make_numbers",
			 "Number is not valid"))
		return n;

	/*
	 * no error occured, clear the error
	 * just to be safe
	 */
	S_CLR_ERR(error);

	return n;
}


int main(void)
{
	s_erc error = S_SUCCESS; /* start of with a clean slate */
	int i;
	int *tmp;

	/*
	 * initialize the error and debugging system
	 * with a log file and debugging level.
	 * If Speect was not cmake configured with the
	 * -DERROR_TO_FILE=1 switch (see README) then
	 * a log file will not be created, regardless of
	 * the log_file argument.
	 */
	s_errdbg_init("mylog.txt", "mylog.txt", S_DEBUG_, &error);

	/*
	 * Check error if any from s_errdbg_init, and set
	 * a new error with a new context.
	 */
	if (S_CHK_ERR(&error, S_FAILURE,
			 "main",
			 "Failed to initialize errdbg module"))
		return -1;

	for (i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			/* clear the error variable */
			S_CLR_ERR(&error);

			/* change the debugging level */
			s_errdbg_set_dbglevel(S_DEBUG_EXTENDED, &error);

			/* check error and continue it */
			S_CHK_ERR(&error, S_CONTERR,
				     "main",
				     "Failed to set debug level, trying to continue");

			tmp = make_numbers(i, &error);

			/* check error from make_numbers and set a new error and context */
			if (S_CHK_ERR(&error, S_FAILURE,
					 "main",
					 "Number is not valid"))
			{
				/* debug with S_DEBUG_EXTENDED level */
				S_DEBUG(S_DEBUG_EXTENDED, "Found error with i = %d", 1);

				if (tmp != NULL)
					S_FREE(tmp);
			}

			if (tmp != NULL)
				S_FREE(tmp);

			continue;
		}

		if (i == 1)
			/* set an new error with a context */
			S_CTX_ERR(&error, S_FAILURE,
				     "main",
				     "i = %d, is invalid", i);
		else
		{
			/* clear the error variable */
			S_CLR_ERR(&error);

			tmp = make_numbers(i, &error);

			/* check error from make_numbers and set a new error and context */
			if (S_CHK_ERR(&error, S_FAILURE,
					 "main",
					 "Number is not valid"))
			{
				/*
				 * debug with S_DEBUG_VERBOSE level,
				 * this debug will not do anything as it's level
				 * is higher than the debug level set at i = 0
				 */
				S_DEBUG(S_DEBUG_VERBOSE, "Found error with i = %d", 1);

				if (tmp != NULL)
					S_FREE(tmp);
			}

			if (tmp != NULL)
				S_FREE(tmp);
		}
	}

	/* clear the error variable */
	S_CLR_ERR(&error);

	/*
	 * free all resources associated with the
	 * error and debugging system, will also
	 * close the log file if any.
	 */
	s_errdbg_quit(&error);

	/* check for errors and continue the error with a new context */
	if (S_CHK_ERR(&error, S_CONTERR,
			 "main",
			 "Failed to quit errdbg module"))
		return -1;

	return 0;
}
