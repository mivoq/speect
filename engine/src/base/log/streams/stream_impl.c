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
#include "base/log/streams/stream_impl.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API s_erc s_logger_write(const s_logger *logger, const char *fmt, ...)
{
	va_list marker;
	s_erc this_error;


	S_CLR_ERR(&this_error);

	if (logger == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_logger_write",
					"Argument \"logger\" is NULL");
		return S_ARGERROR;
	}

	if (fmt == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_logger_write",
					"Argument \"fmt\" is NULL");
		return S_ARGERROR;
	}

	if (logger->v_write == NULL)
	{
		S_ERR_PRINT(S_METHINVLD, "s_logger_write",
					"Object method \"v_write\" is NULL");
		return S_METHINVLD;
	}

	va_start(marker, fmt);
	this_error = logger->v_write(logger, fmt, marker);
	va_end(marker);

	if (this_error != S_SUCCESS)
	{
		S_ERR_PRINT(this_error, "s_logger_write",
					"Call to object method \"v_write\" failed");
		/* try and print it to stdout */
		va_start(marker, fmt);
		vfprintf(stdout, fmt, marker);
		va_end(marker);
	}

	return this_error;
}


S_API s_erc s_logger_vwrite(const s_logger *logger, const char *fmt, va_list argp)
{
	s_erc this_error;


	S_CLR_ERR(&this_error);

	if (logger == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_logger_vwrite",
					"Argument \"logger\" is NULL");
		return S_ARGERROR;
	}

	if (fmt == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_logger_vwrite",
					"Argument \"fmt\" is NULL");
		return S_ARGERROR;
	}

	if (logger->v_write == NULL)
	{
		S_ERR_PRINT(S_METHINVLD, "s_logger_vwrite",
					"Object method \"v_write\" is NULL");
		return S_METHINVLD;
	}

	this_error = logger->v_write(logger, fmt, argp);

	if (this_error != S_SUCCESS)
	{
		S_ERR_PRINT(this_error, "s_logger_vwrite",
					"Call to object method \"v_write\" failed");

		/* try and print it to stdout */
		vfprintf(stdout, fmt, argp);
	}

	return this_error;
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
