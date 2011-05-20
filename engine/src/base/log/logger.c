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
/* Basic logging facilities.                                                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "base/utils/alloc.h"
#include "base/errdbg/errdbg_utils.h"
#include "base/errdbg/errdbg_macros.h"
#include "base/log/layout/layouts.h"
#include "base/log/streams/streams.h"
#include "base/log/logger.h"


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/*
 * The opaque s_logger data structure for file and console loggers
 */
struct s_logger_private_info
{
	s_stream *stream;                     /* Logging stream                     */
	s_layout *layout;                     /* Layout formatter                   */
};


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_erc v_write_file(const s_logger *logger, s_log_event level, const char *error_msg,
						  const char *func_name, const char *file_name, int line_num,
						  const char *user_msg, va_list argp);

static s_erc v_write_console(const s_logger *logger, s_log_event level, const char *error_msg,
							 const char *func_name, const char *file_name, int line_num,
							 const char *user_msg, va_list argp);

static s_erc destroy_file(s_logger *self);

static s_erc destroy_console(s_logger *self);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API s_logger *s_logger_file_new(const char *path)
{
	s_erc this_error;
	s_logger *logger;
	s_logger_private_info *private_data;


	S_CLR_ERR(&this_error);

	if (path == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_logger_file_new",
					"Argument \"path\" is NULL");
		return NULL;
	}

	private_data = S_MALLOC(s_logger_private_info, 1);
	if (private_data == NULL)
	{
		S_FTL_ERR_PRINT(S_MEMERROR, "s_logger_file_new",
						"Failed to allocate memory for logger private data");
		return NULL;
	}

	/* create file stream */
	private_data->stream = s_stream_file_new(path);
	if (private_data->stream == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "s_logger_file_new",
					"Call to \"s_stream_file_new\" failed");
		S_FREE(private_data);
		return NULL;
	}

	/* create layout */
	private_data->layout = s_layout_std_new();
	if (private_data->layout == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "s_logger_file_new",
					"Call to \"s_layout_std_new\" failed");
		s_stream_destroy(private_data->stream);
		S_FREE(private_data);
		return NULL;
	}

	logger = S_MALLOC(s_logger, 1);
	if (logger == NULL)
	{
		S_FTL_ERR_PRINT(S_MEMERROR, "s_logger_file_new",
						"Failed to allocate memory for logger object");
		s_stream_destroy(private_data->stream);
		s_layout_destroy(private_data->layout);
		S_FREE(private_data);
		return NULL;
	}

	logger->data = private_data;

	/* initialize file writer function pointers */
	logger->v_write = &v_write_file;
	logger->destroy = &destroy_file;


	return logger;
}


S_API s_logger *s_logger_console_new(s_bool log_to_stdout)
{
	s_erc this_error;
	s_logger *logger;
	s_logger_private_info *private_data;


	S_CLR_ERR(&this_error);

	private_data = S_MALLOC(s_logger_private_info, 1);
	if (private_data == NULL)
	{
		S_FTL_ERR_PRINT(S_MEMERROR, "s_logger_file_new",
						"Failed to allocate memory for logger private data");
		return NULL;
	}

	/* create file stream */
	private_data->stream = s_stream_console_new(log_to_stdout);
	if (private_data->stream == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "s_logger_console_new",
					"Call to \"s_stream_console_new\" failed");
		S_FREE(private_data);
		return NULL;
	}

	/* create layout */
	private_data->layout = s_layout_std_new();
	if (private_data->layout == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "s_logger_console_new",
					"Call to \"s_layout_std_new\" failed");
		s_stream_destroy(private_data->stream);
		S_FREE(private_data);
		return NULL;
	}

	logger = S_MALLOC(s_logger, 1);
	if (logger == NULL)
	{
		S_FTL_ERR_PRINT(S_MEMERROR, "s_logger_console_new",
						"Failed to allocate memory for logger object");
		s_stream_destroy(private_data->stream);
		s_layout_destroy(private_data->layout);
		S_FREE(private_data);
		return NULL;
	}

	logger->data = private_data;

	/* initialize file writer function pointers */
	logger->v_write = &v_write_console;
	logger->destroy = &destroy_console;


	return logger;
}


S_API s_erc s_logger_write(const s_logger *logger, s_log_event level, const char *error_msg,
						   const char *func_name, const char *file_name, int line_num,
						   const char *user_msg, ...)
{
	va_list marker;
	s_erc local_err;


	S_CLR_ERR(&local_err);
	if (logger == NULL)
	{
		S_NEW_ERR(&local_err, S_ARGERROR);
		S_ERR_PRINT(local_err, "s_logger_write",
					"Argument \"logger\" is NULL");
		return local_err;
	}

	if (logger->v_write == NULL)
	{
		S_NEW_ERR(&local_err, S_METHINVLD);
		S_ERR_PRINT(local_err, "s_logger_write",
					"Object method \"v_write\" is NULL");
		return local_err;
	}

	if (user_msg == NULL)
	{
		S_NEW_ERR(&local_err, S_ARGERROR);
		S_ERR_PRINT(local_err, "s_logger_write",
					"Argument \"user_msg\" is NULL");
		return local_err;
	}

	va_start(marker, user_msg);
	local_err = logger->v_write(logger, level, error_msg,
								func_name, file_name, line_num,
								user_msg, marker);
	va_end(marker);

	if (local_err != S_SUCCESS)
	{
		S_ERR_PRINT(local_err, "s_logger_write",
					"Call to object method \"v_write\" failed");
		/* try and print it to stdout */
		va_start(marker, user_msg);
		vfprintf(stdout, user_msg, marker);
		va_end(marker);
	}

	return local_err;
}


S_API s_erc s_logger_vwrite(const s_logger *logger, s_log_event level, const char *error_msg,
						   const char *func_name, const char *file_name, int line_num,
						   const char *user_msg, va_list argp)
{
	s_erc local_err;


	S_CLR_ERR(&local_err);
	if (logger == NULL)
	{
		S_NEW_ERR(&local_err, S_ARGERROR);
		S_ERR_PRINT(local_err, "s_logger_vwrite",
					"Argument \"logger\" is NULL");
		return local_err;
	}

	if (logger->v_write == NULL)
	{
		S_NEW_ERR(&local_err, S_METHINVLD);
		S_ERR_PRINT(local_err, "s_logger_vwrite",
					"Object method \"v_write\" is NULL");
		return local_err;
	}

	if (user_msg == NULL)
	{
		S_NEW_ERR(&local_err, S_ARGERROR);
		S_ERR_PRINT(local_err, "s_logger_vwrite",
					"Argument \"user_msg\" is NULL");
		return local_err;
	}

	local_err = logger->v_write(logger, level, error_msg,
								func_name, file_name, line_num,
								user_msg, argp);
	if (local_err != S_SUCCESS)
	{
		S_ERR_PRINT(local_err, "s_logger_vwrite",
					"Call to object method \"v_write\" failed");
		/* try and print it to stdout */
		vfprintf(stdout, user_msg, argp);
	}

	return local_err;
}


S_API s_erc s_logger_destroy(s_logger *logger)
{
	s_erc this_error;


	S_CLR_ERR(&this_error);

	if (logger == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_logger_destroy",
					"Argument \"logger\" is NULL");
		return S_ARGERROR;
	}

	if (logger->destroy == NULL)
	{
		S_ERR_PRINT(S_METHINVLD, "s_logger_destroy",
					"Object method \"destroy\" is NULL");
		return S_METHINVLD;
	}

	this_error = logger->destroy(logger);
	if (this_error != S_SUCCESS)
		S_ERR_PRINT(this_error, "s_logger_destroy",
					"Call to object method \"destroy\" failed");

	return this_error;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_erc v_write_file(const s_logger *logger, s_log_event level, const char *error_msg,
						  const char *func_name, const char *file_name, int line_num,
						  const char *user_msg, va_list argp)
{
	s_erc this_error;
	char *msg;
	s_logger_private_info *private_data;


	S_CLR_ERR(&this_error);

	private_data = (s_logger_private_info*)logger->data;

	msg = s_layout_vformat(private_data->layout, level, error_msg,
						   func_name, file_name, line_num, user_msg, argp);
	if (msg == NULL)
	{
		/*
		 * if layout made an error it should still output message
		 * to stdout.
		 */
		S_NEW_ERR(&this_error, S_METHFAIL);
		S_ERR_PRINT(this_error, "v_write_file",
					"Call to \"s_layout_vformat\" failed");
		return this_error;
	}

	/* send message to stream */
	this_error = s_stream_write(private_data->stream, msg);
	if (this_error != S_SUCCESS)
	{
		/*
		 * if stream made an error it should still output message
		 * to stdout.
		 */
		S_ERR_PRINT(this_error, "v_write_file",
					"Call to \"s_stream_write\" failed");
	}

	S_FREE(msg);
	return this_error;
}


static s_erc v_write_console(const s_logger *logger, s_log_event level, const char *error_msg,
							 const char *func_name, const char *file_name, int line_num,
							 const char *user_msg, va_list argp)
{
	s_erc this_error;
	char *msg;
	s_logger_private_info *private_data;


	S_CLR_ERR(&this_error);

	private_data = (s_logger_private_info*)logger->data;

	msg = s_layout_vformat(private_data->layout, level, error_msg,
						   func_name, file_name, line_num, user_msg, argp);
	if (msg == NULL)
	{
		/*
		 * if layout made an error it should still output message
		 * to stdout.
		 */
		S_NEW_ERR(&this_error, S_METHFAIL);
		S_ERR_PRINT(this_error, "v_write_console",
					"Call to \"s_layout_vformat\" failed");
		return this_error;
	}

	/* send message to stream */
	this_error = s_stream_write(private_data->stream, msg);
	if (this_error != S_SUCCESS)
	{
		/*
		 * if stream made an error it should still output message
		 * to stdout.
		 */
		S_ERR_PRINT(this_error, "v_write_console",
					"Call to \"s_stream_write\" failed");
	}

	S_FREE(msg);
	return this_error;
}


static s_erc destroy_file(s_logger *self)
{
	s_erc this_error;


	S_CLR_ERR(&this_error);

	this_error = s_stream_destroy(self->data->stream);
	if (this_error != S_SUCCESS)
		S_ERR_PRINT(S_FAILURE, "destroy_file",
					"Call to \"s_stream_destroy\" failed");

	this_error = s_layout_destroy(self->data->layout);
	if (this_error != S_SUCCESS)
		S_ERR_PRINT(S_FAILURE, "destroy_file",
					"Call to \"s_layout_destroy\" failed");

	S_FREE(self->data->stream);
	S_FREE(self->data->layout);
	S_FREE(self->data);

	self->v_write = NULL;
	self->destroy = NULL;

	return this_error;
}


static s_erc destroy_console(s_logger *self)
{
	s_erc this_error;


	S_CLR_ERR(&this_error);

	this_error = s_stream_destroy(self->data->stream);
	if (this_error != S_SUCCESS)
		S_ERR_PRINT(S_FAILURE, "destroy_console",
					"Call to \"s_stream_destroy\" failed");

	this_error = s_layout_destroy(self->data->layout);
	if (this_error != S_SUCCESS)
		S_ERR_PRINT(S_FAILURE, "destroy_console",
					"Call to \"s_layout_destroy\" failed");

	S_FREE(self->data->stream);
	S_FREE(self->data->layout);
	S_FREE(self->data);

	self->v_write = NULL;
	self->destroy = NULL;

	return this_error;
}

