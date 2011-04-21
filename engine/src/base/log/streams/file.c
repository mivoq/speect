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
/* File streaming facilities.                                                       */
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
#include "base/strings/strings.h"
#include "base/errdbg/errdbg_macros.h"
#include "base/errdbg/errdbg_utils.h"
#include "base/log/streams/file.h"


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
	FILE *file;                     /* File handle                        */
	char *name;                     /* File name (full path name)         */
	S_DECLARE_MUTEX(logging_mutex); /* Locking mutex.                     */
};


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_erc v_write_file(const s_stream *self, const char *fmt, va_list argp);

static s_erc destroy_file(s_stream *self);

static s_erc startlog_info(s_stream *self);

static s_erc endlog_info(s_stream *self);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API s_stream *s_stream_file_new(const char *path)
{
	FILE *fhandle;
	s_stream *file_stream;
	struct s_stream_private_info *private_data;
	s_erc this_error;


	S_CLR_ERR(&this_error);

	if (path == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_stream_file_new",
					"Argument \"path\" is NULL");
		return NULL;
	}

	fhandle = fopen(path, "w");

	if (fhandle == NULL)
	{
		char *err_str = s_error_str(S_FAILURE);

#ifdef SPCT_USE_THREADS
		fprintf(stderr, "[ERROR (%s) %lu] Failed to open log file '%s'"
				" (in function 's_stream_file_new', %s, %d)\n",
				err_str, s_thread_id(), path, __FILE__, __LINE__);
#else /* !SPCT_USE_THREADS */
		fprintf(stderr, "[ERROR (%s)] Failed to open log file '%s'"
				" (in function 's_stream_file_new', %s, %d)\n",
				err_str, path, __FILE__, __LINE__);
#endif /* SPCT_USE_THREADS */
		S_FREE(err_str);
		return NULL;
	}

	clearerr(fhandle);

	private_data = S_MALLOC(struct s_stream_private_info, 1);

	if (private_data == NULL)
	{
		S_FTL_ERR_PRINT(S_MEMERROR, "s_stream_file_new",
						"Failed to allocate memory for stream private data");
		fclose(fhandle);
		return NULL;
	}

	private_data->file = fhandle;
	private_data->name = s_strdup_clib(path);

	if (private_data->name == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "s_stream_file_new",
					"Call to \"s_strdup_clib\" failed");
		S_FREE(private_data);
		fclose(fhandle);
		return NULL;
	}

	file_stream = S_MALLOC(s_stream, 1);

	if (file_stream == NULL)
	{
		S_FTL_ERR_PRINT(S_MEMERROR, "s_stream_file_new",
						"Failed to allocate memory for stream object");
		S_FREE(private_data->name);
		S_FREE(private_data);
		fclose(fhandle);
		return NULL;
	}

	/* init mutex */
	s_mutex_init(&(private_data->logging_mutex));

	file_stream->data = private_data;

	/* initialize file writer function pointers */
	file_stream->v_write = &v_write_file;
	file_stream->destroy = &destroy_file;

	if (startlog_info(file_stream) != S_SUCCESS)
		S_ERR_PRINT(S_FAILURE, "s_stream_file_new",
					"Call to \"startlog_info\" failed");

	return file_stream;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_erc v_write_file(const s_stream *self, const char *fmt, va_list argp)
{
	s_erc this_error;


	S_CLR_ERR(&this_error);

	/* lock mutex */
	s_mutex_lock(&(self->data->logging_mutex));

	if (vfprintf(self->data->file, fmt, argp) < 0)
	{
		char *err_str = s_error_str(this_error);


		S_NEW_ERR(&this_error, S_FAILURE);
#ifdef SPCT_USE_THREADS
		fprintf(stderr, "[ERROR (%s) %lu] vfprintf failed to write to file stream '%s'"
				" (in function 'v_write_file', %s, %d)\n",
				err_str, s_thread_id(), self->data->name, __FILE__, __LINE__);
#else /* !SPCT_USE_THREADS */
		fprintf(stderr, "[ERROR (%s)] vfprintf failed to write to file stream '%s'"
				" (in function 'v_write_file', %s, %d)\n",
				err_str, self->data->name, __FILE__, __LINE__);
#endif /* SPCT_USE_THREADS */
		S_ERR_PRINT(this_error, "v_write_file",
					"vfprintf returned < 0");
		S_FREE(err_str);
	}

	fflush(self->data->file);

	/* unlock mutex */
	s_mutex_unlock(&(self->data->logging_mutex));

	return this_error;
}


static s_erc destroy_file(s_stream *self)
{
	s_erc this_error;


	S_CLR_ERR(&this_error);

	if (endlog_info(self) != S_SUCCESS)
		S_ERR_PRINT(S_FAILURE, "destroy_file",
					"Call to \"endlog_info\" failed");

	/* lock mutex */
	s_mutex_lock(&(self->data->logging_mutex));


	fclose(self->data->file);
	S_FREE(self->data->name);

	self->v_write = NULL;
	self->destroy = NULL;

	/* unlock and destroy mutex */
	s_mutex_unlock(&(self->data->logging_mutex));
	s_mutex_destroy(&(self->data->logging_mutex));

	/* free the private data */
	S_FREE(self->data);

	return this_error;
}


static s_erc startlog_info(s_stream *self)
{
	s_erc this_error;
	s_erc store_error;


	S_CLR_ERR(&this_error);
	S_CLR_ERR(&store_error);

	/* Speect Engine version info */
	this_error = s_stream_write(self, "Speect (%s), version %d.%d.%d\n",
								S_RELEASE, S_MAJOR_VERSION,
								S_MINOR_VERSION, S_PATCHLEVEL);

	/* ok if this error is cleared futher on, not big issue */
	if (this_error != S_SUCCESS)
	{
		S_ERR_PRINT(S_FAILURE, "startlog_info",
					"Call to \"s_stream_write\" failed");
		store_error = this_error;
	}

	/* build info */
	this_error = s_stream_write(self, "Build Date: %s %s\n",
								S_COMPILE_DATE,
								S_COMPILE_TIME);

	/* ok if this error is cleared futher on, not big issue */
	if (this_error != S_SUCCESS)
	{
		S_ERR_PRINT(S_FAILURE, "startlog_info",
					"Call to \"s_stream_write\" failed");
		store_error = this_error;
	}

	/* start */
	this_error = s_stream_write(self,
								"------------------- start log --------------------\n\n");

	/* ok if this error is cleared futher on, not big issue */
	if (this_error != S_SUCCESS)
	{
		S_ERR_PRINT(S_FAILURE, "startlog_info",
					"Call to \"s_stream_write\" failed");
		store_error = this_error;
	}

	if ((store_error != S_SUCCESS) && (this_error == S_SUCCESS))
		this_error = store_error;

	return this_error;
}


static s_erc endlog_info(s_stream *self)
{
	s_erc this_error;


	S_CLR_ERR(&this_error);

	/* Speect Engine version info */
	this_error = s_stream_write(self,
								"\n--------------------- end log --------------------\n");

	/* ok if this error is cleared futher on, not big issue */
	if (this_error != S_SUCCESS)
		S_ERR_PRINT(S_FAILURE, "endlog_info",
					"Call to \"s_stream_write\" failed");

	return this_error;
}

