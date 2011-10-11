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
/* DATE    : 5 June 2009                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Error handling and debugging utilities.                                          */
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
#include "base/utils/smath.h"
#include "base/utils/alloc.h"
#include "base/strings/strings.h"
#include "base/errdbg/errdbg_defs.h"
#include "base/errdbg/errdbg_utils.h"
#include "base/errdbg/errdbg_macros.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static const char * const erc_strings[] =
{
	/*   0 */ "No error",
	/*  -1 */ "Failure",
	/*  -2 */ "Memory allocation failed",
	/*  -3 */ "Function argument(s) invalid",
	/*  -4 */ "Class/object method does not exist",
	/*  -5 */ "Method/function call failed",
	/*  -6 */ "End of file/stream",
	/*  -7 */ "Warning, possible error",
	/*  -8 */ "Error context continued"    /* must be last one */
};


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API char *s_error_str(s_erc error_code)
{
	const char *enum_string;
	char *estring;


	/*
	 * note that we are using the lib c strncpy and
	 * not speect s_strdup. This enables us to use this
	 * function without the error and debug module being
	 * intialized. This is OK as long as erc_strings[]
	 * are ascii characters.
	 */
	if (S_NUM_IN_RANGE(error_code, S_CONTERR, S_SUCCESS))
		enum_string = erc_strings[S_ABS(error_code)];
	else
		enum_string = "Undefined error";

	estring = s_strdup_clib(enum_string);
	if (estring == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "s_error_str",
					"Call to \"s_strdup_clib\" failed");
		return NULL;
	}

	return estring;
}


