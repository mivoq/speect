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

static const char *log_def = "spct.log";


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_create_logger(s_ini_parser *spct_ini, s_logger **logger, s_erc *error)
{
	const char *ini_log;

	S_CLR_ERR(error);


	/* get error_log from INI, revert to error_log_def if not found */
	ini_log = s_iniparser_get_string(spct_ini, "loggers:error_log",
									 log_def);
	*logger = s_logger_file_new(ini_log);
	if (*logger == NULL)
	{
		S_NEW_ERR(error, S_FAILURE);
		S_ERR_PRINT(S_FAILURE, "_s_create_loggers",
					"Call to \"s_logger_file_new\" failed");
		return;
	}
}


S_LOCAL void _s_destroy_loggers(s_logger *logger, s_erc *error)
{
	s_erc local_err = S_SUCCESS;


	S_CLR_ERR(error);

	local_err = s_logger_destroy(logger);
	S_FREE(logger);

	if (local_err != S_SUCCESS)
	{
		S_NEW_ERR(error, local_err);
		S_ERR_PRINT(S_FAILURE, "_s_destroy_loggers",
					"Call to \"s_logger_destroy\" for 'logger' failed");
	}
}


