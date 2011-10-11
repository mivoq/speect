/************************************************************************************/
/* Copyright (c) 2011 The Department of Arts and Culture,                           */
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
/* DATE    : 19 April 2011                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Python s_logger for Speect                                                       */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "py_logger.h"


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/*
 * The opaque s_logger data structure for file and console loggers
 */
typedef struct _s_logger_private_info
{
	PyObject *logFunction;        /* Python "logger.log" function */
} _s_logger_private_info;


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/* convert Speect event levels to Python severity levels. */
static int _s_spct_event_2_python(s_log_event level)
{
	if (level == S_FATAL_EVENT)
		return 50;
	if (level == S_ERROR_EVENT)
		return 40;
	if (level == S_WARN_EVENT)
		return 30;
	if (level == S_INFO_EVENT)
		return 20;
	if (level == S_DEBUG_EVENT)
		return 10;
	if (level == S_TRACE_EVENT)
		return 10;

	return 10;
}


static s_erc _v_write_swig(const s_logger *logger, s_log_event level, const char *error_msg,
						   const char *func_name, const char *file_name, int line_num,
						   const char *user_msg, va_list argp)
{
	s_erc this_error;
	_s_logger_private_info *private_data;
	PyObject *returnValue;
	PyObject *loggingInfo;
	char user_buf[65532];
	const char *unknown_err = "Unknown error";
	PyObject *args;
	PyObject *kw_args;


	S_CLR_ERR(&this_error);
	private_data = (_s_logger_private_info*)logger->data;

	/* create user_buf from user_msg and argp */
	if (user_msg != NULL)
	{
		if (vsprintf(user_buf, user_msg, argp) < 0)
		{
			S_ERR_PRINT(S_FAILURE, "_v_write_swig",
						"Call to \"vsprintf\" failed");
			return S_FAILURE;
		}
	}
	else
	{
		strcpy(user_buf, unknown_err);
	}

	/* create a dictionary with all available logging info */
	loggingInfo = Py_BuildValue("{sssssssiss}",
								"se_event", s_log_event_str(level),
								"se_function", func_name? func_name:"unknown",
								"se_file", file_name? file_name:"unknown",
								"se_line", line_num,
								"se_user_msg", user_buf);
	if (loggingInfo == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "_v_write_swig",
					"Call to \"Py_BuildValue\" failed");
		return this_error;
	}

	/* create a tuple with the arguments for the log function call */
	args = Py_BuildValue("(is)", _s_spct_event_2_python(level), error_msg);
	if (args == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "_v_write_swig",
					"Call to \"Py_BuildValue\" failed");
		Py_DECREF(loggingInfo);
		return this_error;
	}

	/* create a tuple with the keywords for the log function call */
	kw_args = Py_BuildValue("{sO}", "extra", loggingInfo);
	if (kw_args == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "_v_write_swig",
					"Call to \"Py_BuildValue\" failed");
		Py_DECREF(args);
		Py_DECREF(loggingInfo);
		return this_error;
	}

	/* call log function */
	returnValue = PyObject_Call(private_data->logFunction, args, kw_args);
	if (returnValue == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "s_logger_swig_new",
					"Call to \"PyObject_Call\" failed");
	}
	Py_DECREF(returnValue); /* not used */
	Py_DECREF(kw_args);
	Py_DECREF(args);
	Py_DECREF(loggingInfo);

	return this_error;
}


static s_erc _destroy_swig(s_logger *self)
{
	s_erc this_error;
	_s_logger_private_info *private_data;


	S_CLR_ERR(&this_error);

	private_data = (_s_logger_private_info*)self->data;
	Py_DECREF(private_data->logFunction);
	S_FREE(private_data);

	self->v_write = NULL;
	self->destroy = NULL;

	return this_error;
}


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void s_set_py_logger(PyObject *logger, s_erc *error)
{
	PyObject *logFunction;  /* logger.log */
	s_logger *slogger;
	_s_logger_private_info *private_data;


	S_CLR_ERR(error);
	Py_INCREF(logger);

	/* get log function */
	logFunction = PyObject_GetAttrString(logger, "log");
	if (logFunction == NULL)
	{
		char *py_error = s_get_python_error_str();


		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "s_set_py_logger",
					  "Call to \"PyObject_GetAttrString\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "s_set_py_logger",
					  "Call to \"PyObject_GetAttrString\" failed");
		}

		Py_DECREF(logger);
		return;
	}

	Py_DECREF(logger); /* done with logger */

	private_data = S_MALLOC(_s_logger_private_info, 1);
	if (private_data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_set_py_logger",
				  "Failed to allocate memory for logger private data");
		Py_DECREF(logFunction);
		return;
	}

	private_data->logFunction = logFunction;

	/* create s_logger */
	slogger = S_MALLOC(s_logger, 1);
	if (slogger == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_set_py_logger",
				  "Failed to allocate memory for \'s_logger\' object");
		S_FREE(private_data);
		Py_DECREF(logFunction);
		return;
	}

	slogger->data = (s_logger_private_info*)private_data;


	/* initialize file writer function pointers */
	slogger->v_write = &_v_write_swig;
	slogger->destroy = &_destroy_swig;

	/*
	 * all Speect Engine logging must come through, logger from
	 * Python decides what gets logged.
	 */
	s_set_errdbg_level(S_DBG_ALL, NULL);

	s_set_errdbg_logger(slogger);
}

