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
/* DATE    : 29 September 2009                                                      */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Logging event definitions.                                                       */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include <stdio.h>
#include "base/utils/math.h"
#include "base/utils/alloc.h"
#include "base/strings/strings.h"
#include "base/errdbg/errdbg_defs.h"
#include "base/errdbg/errdbg_utils.h"
#include "base/errdbg/errdbg_macros.h"
#include "base/log/event/event.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static const char * const event_strings[] =
{
	/*   0 */ "RESERVED",
	/*   1 */ "FATAL",
	/*   2 */ "ERROR",
	/*   3 */ "WARNING",
	/*   4 */ "INFO",
	/*   5 */ "DEBUG",
	/*   6 */ "TRACE"
};


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API char *s_log_event_str(s_log_event level)
{
	const char *enum_string;
	char *estring;


	/*
	 * note that we are using the lib c strncpy and
	 * not speect s_strdup. This enables us to use this
	 * function without the error and debug module being
	 * intialized. This is OK as long as event_strings[]
	 * are ascii characters.
	 */
	if (S_NUM_IN_RANGE(level, S_RESERVED_EVENT + 1, S_TRACE_EVENT))
		enum_string = event_strings[level];
	else
		enum_string = "UNKNOWN EVENT";

	estring = s_strdup_clib(enum_string);
	if (estring == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "s_log_event_str",
					"Call to \"s_strdup_clib\" failed");
		return NULL;
	}

	return estring;
}
