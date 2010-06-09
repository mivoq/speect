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
/* WIN32 memory mapped file.                                                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <windows.h>           /* Windows file memory mapping library */
#include "base/utils/alloc.h"
#include "datasources/platform/win32/win32_mmapfile.h"


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

struct s_mmapfile_win32_file_handle
{
	HANDLE file;
	HANDLE memory_map;
};


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API s_mmap_file_handle *s_win32_mmapfile_open(const char *path, size_t *map_size,
												uint8 **mem, s_erc *error)
{
	s_mmap_file_handle *handle;
	HANDLE file;
	HANDLE memory_map;
	DWORD file_size;
	UINT em;
	DWORD last_error;
	char errstr[65535];


	S_CLR_ERR(error);
	em = SetErrorMode(SEM_FAILCRITICALERRORS);
	SetLastError(0);

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_win32_mmapfile_open",
				  "Argument \"path\" is NULL");
		SetLastError(0);
		SetErrorMode(em);
		return NULL;
	}

	if (map_size == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_win32_mmapfile_open",
				  "Argument \"map_size\" is NULL");
		SetLastError(0);
		SetErrorMode(em);
		return NULL;
	}

	if (*mem == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_win32_mmapfile_open",
				  "Argument \"mem\" is NULL");
		SetLastError(0);
		SetErrorMode(em);
		return NULL;
	}

	file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL,
					  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		last_error = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					  last_error, 0,
					  errstr, sizeof(errstr), NULL);
		S_CTX_ERR(error, S_FAILURE,
				  "s_win32_mmapfile_open",
				  "Call to \"CreateFile\" failed. Reported error \"%s\"",
				  errstr);
		SetLastError(0);
		SetErrorMode(em);
		return NULL;
	}

	file_size = GetFileSize(file, NULL);

	if (file_size == INVALID_FILE_SIZE)
	{
		last_error = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					  last_error, 0,
					  errstr, sizeof(errstr), NULL);
		S_CTX_ERR(error, S_FAILURE,
				  "s_win32_mmapfile_open",
				  "Call to \"GetFileSize\" failed. Reported error \"%s\"",
				  errstr);
		CloseHandle(file);
		SetLastError(0);
		SetErrorMode(em);
		return NULL;
	}

	*map_size = (size_t)file_size;

	memory_map = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);

	if (memory_map == NULL)
	{
		last_error = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					  last_error, 0,
					  errstr, sizeof(errstr), NULL);
		S_CTX_ERR(error, S_FAILURE,
				  "s_win32_mmapfile_open",
				  "Call to \"CreateFileMapping\" failed. Reported error \"%s\"",
				  errstr);
		CloseHandle(file);
		SetLastError(0);
		SetErrorMode(em);
		return NULL;
	}

	*mem = MapViewOfFile(memory_map, FILE_MAP_READ, 0, 0, 0);

	if (*mem == NULL)
	{
		last_error = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					  last_error, 0,
					  errstr, sizeof(errstr), NULL);
		S_CTX_ERR(error, S_FAILURE,
				  "s_win32_mmapfile_open",
				  "Call to \"MapViewOfFile\" failed. Reported error \"%s\"",
				  errstr);
		CloseHandle(memory_map);
		CloseHandle(file);
		SetLastError(0);
		SetErrorMode(em);
		return NULL;
	}

	handle = S_MALLOC(s_mmap_file_handle, 1);
	if (handle == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_win32_mmapfile_open",
				  "Failed to allocate memory for \'s_mmap_file_handle\'");
		CloseHandle(memory_map);
		CloseHandle(file);
		SetLastError(0);
		SetErrorMode(em);
		return NULL;
	}

	handle->file = file;
	handle->memory_map = memory_map;
	SetLastError(0);
	SetErrorMode(em);
	return handle;
}


S_API void s_win32_mmapfile_close(s_mmap_file_handle *handle, uint8 *mem, s_erc *error)
{
	UINT em;
	DWORD last_error;
	char errstr[65535];


	S_CLR_ERR(error);
	em = SetErrorMode(SEM_FAILCRITICALERRORS);
	SetLastError(0);

	if (handle == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_win32_mmapfile_close",
				  "Argument \"handle\" is NULL");
		SetLastError(0);
		SetErrorMode(em);
		return;
	}

	if (mem == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_win32_mmapfile_close",
				  "Argument \"mem\" is NULL");
		SetLastError(0);
		SetErrorMode(em);
		return;
	}

	if (UnmapViewOfFile(mem) == 0)
	{
		last_error = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					  last_error, 0,
					  errstr, sizeof(errstr), NULL);
		S_CTX_ERR(error, S_FAILURE,
				  "s_win32_mmapfile_close",
				  "Call to \"UnmapViewOfFile\" failed. Reported error \"%s\"",
				  errstr);
	}

	if (CloseHandle(handle->memory_map) == 0)
	{
		last_error = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					  last_error, 0,
					  errstr, sizeof(errstr), NULL);
		S_CTX_ERR(error, S_FAILURE,
				  "s_win32_mmapfile_close",
				  "Call to \"CloseHandle\" for the memory map failed. Reported error \"%s\"",
				  errstr);
	}

	if (CloseHandle(handle->file) == 0)
	{
		last_error = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					  last_error, 0,
					  errstr, sizeof(errstr), NULL);
		S_CTX_ERR(error, S_FAILURE,
				  "s_win32_mmapfile_close",
				  "Call to \"CloseHandle\" for the file failed. Reported error \"%s\"",
				  errstr);
	}

	S_FREE(handle);
	SetLastError(0);
	SetErrorMode(em);
}
