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
/* DATE    : 29 September 2009                                                      */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Standard layout functions.                                                       */
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
#include <string.h>
#include <stdlib.h>
#include "base/utils/alloc.h"
#include "base/utils/time.h"
#include "base/strings/strings.h"
#include "base/strings/sprint.h"
#include "base/errdbg/errdbg_macros.h"
#include "base/log/layout/layout_impl.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/


/*
 * Error message example
 *
 * Mon Sep 28 13:36:55 2009 [ERROR (Memory error) 10] Failed to allocate object memory (in function 'func', /home/aby/test.c, 121)
 *
 */

/*
 * Debug message example
 *
 * Mon Sep 28 13:36:55 2009 [TRACE 10] Iterator 10, value = 20.321
 *
 */

#ifdef SPCT_USE_THREADS

static const char * const std_es_all = "%s [%s (%s) %lu] %s (in function '%s', %s, %d)\n";
static const char * const std_es_func = "%s [%s (%s) %lu] %s (in function '%s')\n";
static const char * const std_es_line = "%s [%s (%s) %lu] %s (%s, %d)\n";
static const char * const std_es_min = "%s [%s (%s) %lu] %s\n";
static const char * const std_ds = "%s [%s %lu] %s\n";

#else /* !SPCT_USE_THREADS */

static const char * const std_es_all  = "%s [%s (%s)] %s (in function '%s', %s, %d)\n";
static const char * const std_es_func = "%s [%s (%s)] %s (in function '%s')\n";
static const char * const std_es_line = "%s [%s (%s)] %s (%s, %d)\n";
static const char * const std_es_min = "%s [%s (%s)] %s\n";
static const char * const std_ds = "%s [%s] %s\n";

#endif /* SPCT_USE_THREADS */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static char *format_std(s_log_event level, const char *error_msg, const char *func,
						const char *file, int line, const char *user_msg, va_list argp);

static char *format_std_err(s_log_event level, const char *error_msg, const char *func,
							const char *file, int line, const char *user_buf);

static char *format_std_dbg(s_log_event level, const char *user_buf);

static s_erc destroy_std(s_layout *self);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API s_layout *s_layout_std_new(void)
{
	s_layout *std_layout;


	std_layout = S_MALLOC(s_layout, 1);

	if (std_layout == NULL)
	{
		S_FTL_ERR_PRINT(S_MEMERROR, "s_layout_std_new",
						"Failed to allocate memory for layout object");
		return NULL;
	}


	std_layout->data = NULL;

	/* initialize standard layout function pointers */
	std_layout->format = &format_std;
	std_layout->destroy = &destroy_std;

	return std_layout;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static char *format_std(s_log_event level, const char *error_msg,
						const char *func, const char *file, int line,
						const char *user_msg, va_list argp)
{
	char *out_buf;
	char user_buf[65532];
	const char *unknown_err = "Unknown error";


	/* create user_buf from user_msg and ... */
	if (user_msg != NULL)
	{
		if (vsprintf(user_buf, user_msg, argp) < 0)
		{
			S_ERR_PRINT(S_FAILURE, "format_std",
						"Call to \"vsprintf\" failed");
			return NULL;
		}
	}
	else
	{
		strcpy(user_buf, unknown_err);
	}

	if (level > S_WARN_EVENT)
	{
		out_buf = format_std_dbg(level, user_buf);
		if (out_buf == NULL)
		{
			S_ERR_PRINT(S_METHFAIL, "format_std",
						"Call to \"format_std_dbg\" failed");
		}
	}
	else
	{
		out_buf = format_std_err(level, error_msg, func, file, line, user_buf);
		if (out_buf == NULL)
		{
			S_ERR_PRINT(S_METHFAIL, "format_std",
						"Call to \"format_std_err\" failed");
		}

	}

	return out_buf;
}


/* for error logging events */
static char *format_std_err(s_log_event level, const char *error_msg, const char *func,
							const char *file, int line, const char *user_buf)
{
	s_erc local_err;
	char out_buf[65532];
	char *level_string;
	char *str_time;
	char *formatted;
	int rv;


	S_CLR_ERR(&local_err);

	str_time = s_strtime(&local_err);
	if (local_err != S_SUCCESS)
	{
		str_time = s_strdup_clib("00:00:00");
	}

	level_string = s_log_event_str(level);

	if ((func == NULL) && (file == NULL)) /* minimal */
	{
#ifdef SPCT_USE_THREADS
		rv = sprintf(out_buf, std_es_min,
					 (str_time? str_time:"00:00:00"),
					 (level_string? level_string:"UNKNOWN EVENT"),
					 error_msg, s_thread_id(), user_buf);
#else /* !SPCT_USE_THREADS */
		rv = sprintf(out_buf, std_es_min,
					 (str_time? str_time:"00:00:00"),
					 (level_string? level_string:"UNKNOWN EVENT"),
					  error_msg, user_buf);
#endif /* SPCT_USE_THREADS */
		if (rv < 0)
		{
			S_ERR_PRINT(S_FAILURE, "format_std_err",
						"Call to \"sprintf\" failed");
			S_FREE(str_time);
			S_FREE(level_string);
			return NULL;
		}
	}
	else if (func == NULL)  /* file != NULL */
	{
#ifdef SPCT_USE_THREADS
		rv = sprintf(out_buf, std_es_line,
					 (str_time? str_time:"00:00:00"),
					 (level_string? level_string:"UNKNOWN EVENT"),
					 error_msg, s_thread_id(), user_buf, file, line);
#else /* !SPCT_USE_THREADS */
		rv = sprintf(out_buf, std_es_line,
					 (str_time? str_time:"00:00:00"),
					 (level_string? level_string:"UNKNOWN EVENT"),
					 error_msg, user_buf, file, line);
#endif /* SPCT_USE_THREADS */
		if (rv < 0)
		{
			S_ERR_PRINT(S_FAILURE, "format_std_err",
						"Call to \"sprintf\" failed");
			S_FREE(str_time);
			S_FREE(level_string);
			return NULL;
		}
	}
	else if (file == NULL)  /* func != NULL */
	{
#ifdef SPCT_USE_THREADS
		rv = sprintf(out_buf, std_es_func,
					 (str_time? str_time:"00:00:00"),
					 (level_string? level_string:"UNKNOWN EVENT"),
					 error_msg, s_thread_id(), user_buf, func);
#else /* !SPCT_USE_THREADS */
		rv = sprintf(out_buf, std_es_func,
					 (str_time? str_time:"00:00:00"),
					 (level_string? level_string:"UNKNOWN EVENT"),
					 error_msg, user_buf, func);
#endif /* SPCT_USE_THREADS */
		if (rv < 0)
		{
			S_ERR_PRINT(S_FAILURE, "format_std_err",
						"Call to \"sprintf\" failed");
			S_FREE(str_time);
			S_FREE(level_string);
			return NULL;
		}
	}
	else /* all */
	{
#ifdef SPCT_USE_THREADS
		rv = sprintf(out_buf, std_es_all,
					 (str_time? str_time:"00:00:00"),
					 (level_string? level_string:"UNKNOWN EVENT"),
					 error_msg, s_thread_id(), user_buf, func, file, line);
#else /* !SPCT_USE_THREADS */
		rv = sprintf(out_buf, std_es_all,
					 (str_time? str_time:"00:00:00"),
					 (level_string? level_string:"UNKNOWN EVENT"),
					 error_msg, user_buf, func, file, line);
#endif /* SPCT_USE_THREADS */
		if (rv < 0)
		{
			S_ERR_PRINT(S_FAILURE, "format_std_err",
						"Call to \"sprintf\" failed");
			S_FREE(str_time);
			S_FREE(level_string);
			return NULL;
		}
	}

	S_FREE(str_time);
	S_FREE(level_string);
	formatted = s_strdup_clib(out_buf);

	return formatted;
}


/* for debug logging events */
static char *format_std_dbg(s_log_event level, const char *user_buf)
{
	s_erc local_err;
	char out_buf[65532];
	char *level_string;
	char *str_time;
	char *formatted;
	int rv;


	S_CLR_ERR(&local_err);

	str_time = s_strtime(&local_err);
	if (local_err != S_SUCCESS)
	{
		str_time = s_strdup_clib("00:00:00");
	}

	level_string = s_log_event_str(level);

#ifdef SPCT_USE_THREADS
	rv = sprintf(out_buf, std_ds,
				 (str_time? str_time:"00:00:00"),
				 (level_string? level_string:"UNKNOWN EVENT"),
				 s_thread_id(), user_buf);
#else /* !SPCT_USE_THREADS */
	rv = sprintf(out_buf, std_ds,
				 (str_time? str_time:"00:00:00"),
				 (level_string? level_string:"UNKNOWN EVENT"),
				 user_buf);
#endif /* SPCT_USE_THREADS */

	if (rv < 0)
	{
		S_ERR_PRINT(S_FAILURE, "format_std_dbg",
					"Call to \"sprintf\" failed");
		S_FREE(str_time);
		S_FREE(level_string);
		return NULL;
	}

	S_FREE(str_time);
	S_FREE(level_string);
	formatted = s_strdup_clib(out_buf);

	return formatted;
}


static s_erc destroy_std(s_layout *self)
{
	s_erc this_error;


	S_CLR_ERR(&this_error);

	self->format = NULL;
	self->destroy = NULL;

	return this_error;
}

