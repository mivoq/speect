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
/* Logging facilities for SWIG Python.                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%inline
%{
/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "base/utils/alloc.h"
#include "base/errdbg/errdbg_utils.h"
#include "base/errdbg/errdbg_macros.h"
#include "base/log/logger.h"


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
	PyObject *loggingModule;      /* Python "logging" module      */
	int       loggingLevel;       /* Python highest log level     */
} _s_logger_private_info;


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_erc _v_write_swig(const s_logger *logger, s_log_event level, const char *error_msg,
						   const char *func_name, const char *file_name, int line_num,
						   const char *user_msg, va_list argp)
{
	s_erc this_error;
	/* char *msg; */
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
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_ERR_PRINT(S_FAILURE, "_v_write_swig",
						"Call to \"Py_BuildValue\" failed");
			S_ERR_PRINT(S_FAILURE, "_v_write_swig", py_error);
			S_FREE(py_error);
		}
		else
		{
			S_ERR_PRINT(S_FAILURE, "_v_write_swig",
						"Call to \"Py_BuildValue\" failed");
		}

		return this_error;
	}

	/* create a tuple with the arguments for the log function call */
	args = Py_BuildValue("(is)", private_data->loggingLevel, error_msg);
	if (args == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_ERR_PRINT(S_FAILURE, "_v_write_swig",
						"Call to \"Py_BuildValue\" failed");
			S_ERR_PRINT(S_FAILURE, "_v_write_swig", py_error);
			S_FREE(py_error);
		}
		else
		{
			S_ERR_PRINT(S_FAILURE, "_v_write_swig",
						"Call to \"Py_BuildValue\" failed");
		}

		Py_DECREF(loggingInfo);
		return this_error;
	}

	/* create a tuple with the keywords for the log function call */
	kw_args = Py_BuildValue("{sO}", "extra", loggingInfo);
	if (kw_args == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_ERR_PRINT(S_FAILURE, "_v_write_swig",
						"Call to \"Py_BuildValue\" failed");
			S_ERR_PRINT(S_FAILURE, "_v_write_swig", py_error);
			S_FREE(py_error);
		}
		else
		{
			S_ERR_PRINT(S_FAILURE, "_v_write_swig",
						"Call to \"Py_BuildValue\" failed");
		}

		Py_DECREF(args);
		Py_DECREF(loggingInfo);
		return this_error;
	}

	/* call log function */
	returnValue = PyObject_Call(private_data->logFunction, args, kw_args);
	if (returnValue == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_ERR_PRINT(S_FAILURE, "s_logger_swig_new",
						"Call to \"PyObject_Call\" failed");
			S_ERR_PRINT(S_FAILURE, "s_logger_swig_new", py_error);
			S_FREE(py_error);
		}
		else
		{
			S_ERR_PRINT(S_FAILURE, "s_logger_swig_new",
						"Call to \"PyObject_Call\" failed");
		}
	}
	Py_DECREF(returnValue); /* not used */
	Py_DECREF(kw_args);
	Py_DECREF(args);
	Py_DECREF(loggingInfo);


	/* S_FREE(msg); */
	return this_error;
}


static s_erc _destroy_swig(s_logger *self)
{
	s_erc this_error;
	PyObject *loggingShutdownFuntion;
	_s_logger_private_info *private_data;


	S_CLR_ERR(&this_error);

	private_data = (_s_logger_private_info*)self->data;

	/* get shutdown function */
	loggingShutdownFuntion = PyObject_GetAttrString(private_data->loggingModule, "shutdown");
	if (loggingShutdownFuntion == NULL)
	{
		char *py_error = s_get_python_error_str();
		S_NEW_ERR(&this_error,S_FAILURE);

		if (py_error)
		{
			S_ERR_PRINT(this_error, "s_logger_swig_new",
						"Call to \"PyObject_GetAttrString\" failed");
			S_ERR_PRINT(this_error, "s_logger_swig_new", py_error);
			S_FREE(py_error);
		}
		else
		{
			S_ERR_PRINT(this_error, "s_logger_swig_new",
						"Call to \"PyObject_GetAttrString\" failed");
		}
	}
	else
	{
		/* call shutdown function */
		PyObject *returnValue;

		/* set the logging config file */
		returnValue = PyObject_CallFunctionObjArgs(loggingShutdownFuntion, NULL);
		if (returnValue == NULL)
		{
			char *py_error = s_get_python_error_str();
			S_NEW_ERR(&this_error,S_FAILURE);

			if (py_error)
			{
				S_ERR_PRINT(this_error, "s_logger_swig_new",
							"Call to \"PyObject_CallFunctionObjArgs\" failed");
				S_ERR_PRINT(this_error, "s_logger_swig_new", py_error);
				S_FREE(py_error);
			}
			else
			{
				S_ERR_PRINT(this_error, "s_logger_swig_new",
							"Call to \"PyObject_CallFunctionObjArgs\" failed");
			}
		}
		else
		{
			Py_DECREF(returnValue); /* not used */
		}
	}

	Py_DECREF(private_data->logFunction);
	Py_DECREF(private_data->loggingModule);
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

s_logger *_s_logger_swig_new(void)
{
	s_logger               *logger;
	_s_logger_private_info *private_data;
	PyObject               *loggingModule;
	PyObject               *loggingConfigModule;
	PyObject               *fileConfigFunction;
	PyObject               *returnValue;
	PyObject               *getLoggerFunction;
	PyObject               *loggerFunction;
	PyObject               *logFunction;
	PyObject               *loggingLevel;
	int                     loggingLevelInt;
	const char             *config_file;


	loggingModule = PyImport_ImportModule("logging");
	if (loggingModule == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyImport_ImportModule\" failed");
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new", py_error);
			S_FREE(py_error);
		}
		else
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyImport_ImportModule\" failed");
		}

		return NULL;
	}

	loggingConfigModule = PyImport_ImportModule("logging.config");
	if (loggingConfigModule == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyImport_ImportModule\" failed");
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new", py_error);
			S_FREE(py_error);
		}
		else
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyImport_ImportModule\" failed");
		}

		Py_DECREF(loggingModule);
		return NULL;
	}

	/* get highest logging level, so that all messages sent to the
	 * logger will be logged. Speect decides what is sent to the
	 * logger.
	 */
	loggingLevel = PyObject_GetAttrString(loggingModule, "CRITICAL");
	if (loggingLevel == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyObject_GetAttrString\" failed");
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new", py_error);
			S_FREE(py_error);
		}
		else
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyObject_GetAttrString\" failed");
		}

		Py_DECREF(loggingModule);
		Py_DECREF(loggingConfigModule);
		return NULL;
	}

	loggingLevelInt = (int)PyInt_AS_LONG(loggingLevel);
	Py_DECREF(loggingLevel); /* not needed anymore */


	/* get fileConfig function */
	fileConfigFunction = PyObject_GetAttrString(loggingConfigModule, "fileConfig");
	if (fileConfigFunction == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyObject_GetAttrString\" failed");
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new", py_error);
			S_FREE(py_error);
		}
		else
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyObject_GetAttrString\" failed");
		}

		Py_DECREF(loggingModule);
		Py_DECREF(loggingConfigModule);
		return NULL;
	}

	config_file = _get_spct_python_log_config_file();

	/* set the logging config file */
	returnValue = PyObject_CallFunction(fileConfigFunction, "s", config_file);
	if (returnValue == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyObject_CallFunction\" failed");
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new", py_error);
			S_FREE(py_error);
		}
		else
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyObject_CallFunction\" failed");
		}

		Py_DECREF(loggingModule);
		Py_DECREF(fileConfigFunction);
		Py_DECREF(loggingConfigModule);
		return NULL;
	}

	Py_DECREF(returnValue); /* not used */
	Py_DECREF(fileConfigFunction); /* not needed any more */
	Py_DECREF(loggingConfigModule); /* not needed any more */

	/* get getLogger function */
	getLoggerFunction = PyObject_GetAttrString(loggingModule, "getLogger");
	if (getLoggerFunction == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyObject_GetAttrString\" failed");
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new", py_error);
			S_FREE(py_error);
		}
		else
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyObject_GetAttrString\" failed");
		}

		Py_DECREF(loggingModule);
		return NULL;
	}

	/* create the logger */
	loggerFunction = PyObject_CallFunction(getLoggerFunction, "s", "Speect Engine");
	if (loggerFunction == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyObject_CallFunction\" failed");
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new", py_error);
			S_FREE(py_error);
		}
		else
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyObject_CallFunction\" failed");
		}

		Py_DECREF(getLoggerFunction);
		Py_DECREF(loggingModule);
		return NULL;
	}

	Py_DECREF(getLoggerFunction);  /* not needed anymore */

	/* get log function */
	logFunction = PyObject_GetAttrString(loggerFunction, "log");
	if (logFunction == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyObject_GetAttrString\" failed");
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new", py_error);
			S_FREE(py_error);
		}
		else
		{
			S_ERR_PRINT(S_FAILURE, "_s_logger_swig_new",
						"Call to \"PyObject_GetAttrString\" failed");
		}

		Py_DECREF(loggerFunction);
		Py_DECREF(loggingModule);
		return NULL;
	}

	Py_DECREF(loggerFunction); /* not needed anymore */

	private_data = S_MALLOC(_s_logger_private_info, 1);
	if (private_data == NULL)
	{
		S_FTL_ERR_PRINT(S_MEMERROR, "_s_logger_swig_new",
						"Failed to allocate memory for logger private data");
		Py_DECREF(logFunction);
		Py_DECREF(loggingModule);
		return NULL;
	}

	private_data->logFunction = logFunction;
	private_data->loggingModule = loggingModule;
	private_data->loggingLevel = loggingLevelInt;

	/* create logger */
	logger = S_MALLOC(s_logger, 1);
	if (logger == NULL)
	{
		S_FTL_ERR_PRINT(S_MEMERROR, "_s_logger_swig_new",
						"Failed to allocate memory for logger object");
		S_FREE(private_data);
		Py_DECREF(logFunction);
		Py_DECREF(loggingModule);
		return NULL;
	}

	logger->data = (s_logger_private_info*)private_data;

	/* initialize file writer function pointers */
	logger->v_write = &_v_write_swig;
	logger->destroy = &_destroy_swig;


	return logger;
}
%}

