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
/* Basic stream facilities.                                                         */
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
#include "base/log/streams/stream.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API s_erc s_stream_write(const s_stream *stream, const char *fmt, ...)
{
	va_list marker;
	s_erc this_error;


	S_CLR_ERR(&this_error);

	if (stream == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_stream_write",
					"Argument \"stream\" is NULL");
		return S_ARGERROR;
	}

	if (fmt == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_stream_write",
					"Argument \"fmt\" is NULL");
		return S_ARGERROR;
	}

	if (stream->v_write == NULL)
	{
		S_ERR_PRINT(S_METHINVLD, "s_stream_write",
					"Object method \"v_write\" is NULL");
		return S_METHINVLD;
	}

	va_start(marker, fmt);
	this_error = stream->v_write(stream, fmt, marker);
	va_end(marker);

	if (this_error != S_SUCCESS)
	{
		S_ERR_PRINT(this_error, "s_stream_write",
					"Call to object method \"v_write\" failed");
		/* try and print it to stdout */
		va_start(marker, fmt);
		vfprintf(stdout, fmt, marker);
		va_end(marker);
	}

	return this_error;
}


S_API s_erc s_stream_vwrite(const s_stream *stream, const char *fmt, va_list argp)
{
	s_erc this_error;


	S_CLR_ERR(&this_error);

	if (stream == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_stream_vwrite",
					"Argument \"stream\" is NULL");
		return S_ARGERROR;
	}

	if (fmt == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_stream_vwrite",
					"Argument \"fmt\" is NULL");
		return S_ARGERROR;
	}

	if (stream->v_write == NULL)
	{
		S_ERR_PRINT(S_METHINVLD, "s_stream_vwrite",
					"Object method \"v_write\" is NULL");
		return S_METHINVLD;
	}

	this_error = stream->v_write(stream, fmt, argp);

	if (this_error != S_SUCCESS)
	{
		S_ERR_PRINT(this_error, "s_stream_vwrite",
					"Call to object method \"v_write\" failed");

		/* try and print it to stdout */
		vfprintf(stdout, fmt, argp);
	}

	return this_error;
}


S_API s_erc s_stream_destroy(s_stream *stream)
{
	s_erc this_error;


	S_CLR_ERR(&this_error);

	if (stream == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_stream_destroy",
					"Argument \"stream\" is NULL");
		return S_ARGERROR;
	}

	if (stream->destroy == NULL)
	{
		S_ERR_PRINT(S_METHINVLD, "s_stream_destroy",
					"Object method \"destroy\" is NULL");
		return S_METHINVLD;
	}

	this_error = stream->destroy(stream);

	if (this_error != S_SUCCESS)
		S_ERR_PRINT(this_error, "s_stream_destroy",
					"Call to object method \"destroy\" failed");

	return this_error;
}


