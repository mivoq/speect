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
/* Console streaming facilities.                                                    */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "base/utils/alloc.h"
#include "base/utils/types.h"
#include "base/errdbg/errdbg.h"
#include "base/log/streams/console.h"


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/*
 * The opaque s_stream data structure
 */
struct s_stream_private_info
{
	FILE   *file;                   /* Stream handle for stdout/stderr.   */
	S_DECLARE_MUTEX(logging_mutex); /* Locking mutex.                     */
};


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_erc v_write_console(const s_stream *self, const char *fmt, va_list argp);

static s_erc destroy_console(s_stream *self);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API s_stream *s_stream_console_new(s_bool log_to_stdout)
{
	s_stream *console_stream;
	struct s_stream_private_info *private_data;
	s_erc this_error;


	S_CLR_ERR(&this_error);

	private_data = S_MALLOC(struct s_stream_private_info, 1);

	if (private_data == NULL)
	{
		S_FTL_ERR_PRINT(S_MEMERROR, "s_stream_console_new",
						"Failed to allocate memory for stream private data");
		return NULL;
	}

	console_stream = S_MALLOC(s_stream, 1);

	if (console_stream == NULL)
	{
		S_FTL_ERR_PRINT(S_MEMERROR, "s_stream_console_new",
						"Failed to allocate memory for stream object");
		S_FREE(private_data);
		return NULL;
	}

	if (log_to_stdout == TRUE)
		private_data->file = stdout;
	else
		private_data->file = stderr;

	/* initialize mutex */
	s_mutex_init(&(private_data->logging_mutex));

	console_stream->data = private_data;

	/* initialize console writer function pointers */
	console_stream->v_write = &v_write_console;
	console_stream->destroy = &destroy_console;

	return console_stream;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_erc v_write_console(const s_stream *self, const char *fmt, va_list argp)
{
	s_erc this_error;


	S_CLR_ERR(&this_error);

	/* lock mutex */
	s_mutex_lock(&(self->data->logging_mutex));

	if (vfprintf(self->data->file, fmt, argp) < 0)
	{
		S_NEW_ERR(&this_error, S_FAILURE);
		S_ERR_PRINT(this_error, "v_write_console",
					"vfprintf returned < 0");
	}

	fflush(self->data->file);

	/* unlock mutex */
	s_mutex_unlock(&(self->data->logging_mutex));

	return this_error;
}


static s_erc destroy_console(s_stream *self)
{
	s_erc this_error;


	S_CLR_ERR(&this_error);

	/* lock mutex */
	s_mutex_lock(&(self->data->logging_mutex));

	self->v_write = NULL;
	self->destroy = NULL;

	/* unlock and destroy mutex */
	s_mutex_unlock(&(self->data->logging_mutex));
	s_mutex_destroy(&(self->data->logging_mutex));

	/* free the private data */
	S_FREE(self->data);

	return this_error;
}

