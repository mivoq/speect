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
/* DATE    : 23 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* POSIX memory mapped file.                                                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "base/utils/alloc.h"
#include "base/strings/utf8.h"
#include "datasources/platform/posix/mmapfile_posix.h"


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

struct s_mmapfile_posix_file_handle
{
	int file;
	size_t map_size;
};


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API s_mmap_file_handle *s_posix_mmapfile_open(const char *path, size_t *map_size,
												uint8 **mem, s_erc *error)
{
	s_mmap_file_handle *handle;
	int fd;
	size_t msize;
	long pg_size;
	struct stat buf;


	S_CLR_ERR(error);

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_posix_mmapfile_open",
				  "Argument \"path\" is NULL");
		return NULL;
	}

	if (map_size == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_posix_mmapfile_open",
				  "Argument \"map_size\" is NULL");
		return NULL;
	}

	if (*mem == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_posix_mmapfile_open",
				  "Argument \"mem\" is NULL");
		return NULL;
	}

	if ((fd = open(path, O_RDONLY)) == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_posix_mmapfile_open",
				  "Call to \"open\" failed. Reported error \"%s\".",
				  s_strerror(errno));
		return NULL;
	}

	if ((fstat(fd, &buf)) == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_posix_mmapfile_open",
				  "Call to \"fstat\" failed. Reported error \"%s\".",
				  s_strerror(errno));
		close(fd);
		return NULL;
	}

	pg_size = sysconf(_SC_PAGESIZE);
	if (pg_size == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_posix_mmapfile_open",
				  "Call to \"sysconf(_SC_PAGESIZE)\" failed. Reported error \"%s\".",
				  s_strerror(errno));
		close(fd);
		return NULL;
	}

	msize = (buf.st_size + pg_size - 1) / pg_size * pg_size;
	*mem = mmap(0, msize, PROT_READ, MAP_SHARED, fd, 0);

	if (*mem == MAP_FAILED)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_posix_mmapfile_open",
				  "Call to \"sysconf(_SC_PAGESIZE)\" failed. Reported error \"%s\".",
				  s_strerror(errno));
		close(fd);
		return NULL;
	}

	handle = S_MALLOC(s_mmap_file_handle, 1);
	if (handle == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_posix_mmapfile_open",
				  "Failed to allocate memory for \'s_mmap_file_handle\'");
		munmap(*mem, msize);
		close(fd);
		return NULL;
	}

	handle->file = fd;
	handle->map_size = msize;
	*map_size = msize;

	return handle;
}


S_API void s_posix_mmapfile_close(s_mmap_file_handle *handle, uint8 *mem, s_erc *error)
{
	S_CLR_ERR(error);

	if (handle == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_posix_mmapfile_close",
				  "Argument \"handle\" is NULL");
		return;
	}

	if (mem == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_posix_mmapfile_close",
				  "Argument \"mem\" is NULL");
		return;
	}

	if (munmap(mem, handle->map_size) == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_posix_mmapfile_close",
				  "Call to \"munmap\" failed. Reported error \"%s\".",
				  s_strerror(errno));
	}

	if (close(handle->file) == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_posix_mmapfile_close",
				  "Call to \"close\" failed. Reported error \"%s\".",
				  s_strerror(errno));
	}

	S_FREE(handle);
}
