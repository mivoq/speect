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
/* DATE    : 28 January 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Initialization and quit of the Speect Engine loggers.                            */
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
#include "main/loggers.h"
#include "base/utils/alloc.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static const char *error_log_def = "spct_error.log";

static const char *debug_log_def = "spct_debug.log";


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_create_loggers(s_ini_parser *spct_ini,
							   s_logger **err_logger, s_logger **dbg_logger,
							   s_layout **layout, s_erc *error)
{
	s_erc local_err = S_SUCCESS;
	const char *ini_error_log;
	const char *ini_debug_log;
	s_bool have_debug_log;


	S_CLR_ERR(error);

	*layout = s_layout_std_new();

	if (*layout == NULL)
	{
		S_NEW_ERR(error, S_FAILURE);
		S_ERR_PRINT(S_FAILURE, "_s_create_loggers",
					"Call to \"s_layout_std_new\" failed");
		return;
	}

	/* get error_log from INI, revert to error_log_def if not found */
	ini_error_log = s_iniparser_get_string(spct_ini, "loggers:error_log",
										   error_log_def);
	*err_logger = s_logger_file_new(ini_error_log);

	if (*err_logger == NULL)
	{
		local_err = s_layout_destroy(*layout);
		S_FREE(*layout);
		S_NEW_ERR(error, S_FAILURE);
		S_ERR_PRINT(S_FAILURE, "_s_create_loggers",
					"Call to \"s_logger_file_new\" failed");
		return;
	}

	have_debug_log = s_iniparser_entry_present(spct_ini, "loggers:debug_log");

	if (have_debug_log)
	{
		ini_debug_log = s_iniparser_get_string(spct_ini, "loggers:debug_log",
											   debug_log_def);
		*dbg_logger = s_logger_file_new(ini_debug_log);

		if (*dbg_logger == NULL)
		{
			s_layout_destroy(*layout);
			S_FREE(*layout);
			s_logger_destroy(*err_logger);
			S_FREE(*err_logger);
			S_NEW_ERR(error, S_FAILURE);
			S_ERR_PRINT(S_FAILURE, "_s_create_loggers",
						"Call to \"s_logger_file_new\" failed");
			return;
		}
	}
	else
	{
		*dbg_logger = NULL;
	}
}


S_LOCAL void _s_destroy_loggers(s_logger *err_logger,
								s_logger *dbg_logger,
								s_layout *layout,
								s_erc *error)
{
	s_erc local_err = S_SUCCESS;


	S_CLR_ERR(error);

	local_err = s_logger_destroy(err_logger);
	S_FREE(err_logger);

	if (local_err != S_SUCCESS)
	{
		S_NEW_ERR(error, local_err);
		S_ERR_PRINT(S_FAILURE, "_s_destroy_loggers",
					"Call to \"s_logger_destroy\" for 'err_logger' failed");
	}

	if (dbg_logger != NULL)
	{
		local_err = s_logger_destroy(dbg_logger);
		S_FREE(dbg_logger);

		if (local_err != S_SUCCESS)
		{
			S_NEW_ERR(error, local_err);
			S_ERR_PRINT(S_FAILURE, "_s_destroy_loggers",
						"Call to \"s_logger_destroy\" for 'dbg_logger' failed");
		}
	}

	local_err = s_layout_destroy(layout);
	S_FREE(layout);

	if (local_err != S_SUCCESS)
	{
		S_NEW_ERR(error, local_err);
		S_ERR_PRINT(S_FAILURE, "_s_destroy_loggers",
					"Call to \"s_layout_destroy\" for 'layout' failed");
	}
}


