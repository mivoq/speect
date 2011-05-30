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
/* Message layout (format) functions.                                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include "base/utils/alloc.h"
#include "base/errdbg/errdbg_macros.h"
#include "base/errdbg/errdbg_utils.h"
#include "base/log/layout/layout.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

#define _S_FAILED_PRINT_INFO_STDOUT										\
	do {																\
		char *event_str = s_log_event_str(level);						\
		fprintf(stdout, "Event level : %s\n", event_str);				\
		fprintf(stdout, "Error message : \"%s\"\n", error_msg ? error_msg : "none"); \
		fprintf(stdout, "Function name : \"%s\"\n", func ? func : "none"); \
		fprintf(stdout, "File name : \"%s\"\n", file ? file : "none");	\
		fprintf(stdout, "Line number : %d\n", line);					\
		fprintf(stdout, "User message : \"");							\
		S_FREE(event_str);												\
	} while (0)


#define _S_FAILED_PRINT_MSG_VSTDOUT				\
	do {										\
		va_start(marker, user_msg);				\
		vfprintf(stdout, user_msg, marker);		\
		va_end(marker);							\
		fprintf(stdout, "\n");					\
	} while (0)


#define _S_FAILED_PRINT_MSG_STDOUT				\
	do {										\
		vfprintf(stdout, user_msg, argp);		\
		fprintf(stdout, "\n");					\
	} while (0)


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API char *s_layout_format(const s_layout *layout, s_log_event level,
							const char *error_msg, const char *func,
							const char *file, int line, const char *user_msg, ...)
{
	va_list marker;
	char *formated_mesg;


	if (layout == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_layout_format",
					"Argument \"layout\" is NULL, printing logging event to stdout");
		_S_FAILED_PRINT_INFO_STDOUT;
		_S_FAILED_PRINT_MSG_VSTDOUT;
		return NULL;
	}

	if (layout->format == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_layout_format",
					"Object method \"format\" is NULL, printing logging event to stdout");
		_S_FAILED_PRINT_INFO_STDOUT;
		_S_FAILED_PRINT_MSG_VSTDOUT;
		return NULL;
	}

	va_start(marker, user_msg);
	formated_mesg = layout->format(level, error_msg, func,
								   file, line, user_msg, marker);
	va_end(marker);

	if (formated_mesg == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "s_layout_format",
					"Call to object method \"format\" failed,"
					" printing logging event to stdout");
		_S_FAILED_PRINT_INFO_STDOUT;
		_S_FAILED_PRINT_MSG_VSTDOUT;
	}

	return formated_mesg;
}


S_API char *s_layout_vformat(const s_layout *layout, s_log_event level,
							 const char *error_msg, const char *func,
							 const char *file, int line,
							 const char *user_msg, va_list argp)
{
	char *formated_mesg;


	if (layout == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_layout_vformat",
					"Argument \"layout\" is NULL, printing logging event to stdout");
		_S_FAILED_PRINT_INFO_STDOUT;
		_S_FAILED_PRINT_MSG_STDOUT;
		return NULL;
	}

	if (layout->format == NULL)
	{
		S_ERR_PRINT(S_METHINVLD, "s_layout_vformat",
					"Object method \"format\" is NULL, printing logging event to stdout");
		_S_FAILED_PRINT_INFO_STDOUT;
		_S_FAILED_PRINT_MSG_STDOUT;
		return NULL;
	}

	formated_mesg = layout->format(level, error_msg, func,
								   file, line, user_msg, argp);

	if (formated_mesg == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "s_layout_vformat",
					"Call to object method \"format\" failed,"
					" printing logging event to stdout");
		_S_FAILED_PRINT_INFO_STDOUT;
		_S_FAILED_PRINT_MSG_STDOUT;
	}

	return formated_mesg;
}


S_API s_erc s_layout_destroy(s_layout *layout)
{
	s_erc this_error;


	S_CLR_ERR(&this_error);

	if (layout == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_layout_destroy",
					"Argument \"layout\" is NULL");
		return S_ARGERROR;
	}

	if (layout->destroy == NULL)
	{
		S_ERR_PRINT(S_METHINVLD, "s_layout_destroy",
					"Object method \"destroy\" is NULL");
		return S_METHINVLD;
	}

	this_error = layout->destroy(layout);

	if (this_error != S_SUCCESS)
	{
		S_ERR_PRINT(this_error, "s_layout_destroy",
					"Call to object method \"destroy\" failed");
	}

	return this_error;
}

