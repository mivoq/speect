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
/* DATE    : 4 December 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Dynamic Shared Object's with win32 dll library calls.                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/strings/strings.h"
#include "pluginmanager/platform/win32/win32_dll.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/* wrapper for dlopen */
S_API void *s_win32_dll_open(const char *filename, s_erc *error)
{
	HMODULE dso_handle;
	UINT em;
	DWORD last_error;
	char errstr[65535];
	char buf[S_MAX_PATH_LENGTH];
	char *p;


	S_CLR_ERR(error);
	em = SetErrorMode(SEM_FAILCRITICALERRORS);
	SetLastError(0);

	if (filename == NULL)
	{
		dso_handle = GetModuleHandle( NULL );

		if (dso_handle == NULL)
		{
			last_error = GetLastError();
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
						  last_error, 0,
						  errstr, sizeof(errstr), NULL);

			S_CTX_ERR(error, S_FAILURE,
					  "s_win32_dll_open",
					  "Failed to open dso \"NULL\". Reported error \"%s\"",
					  errstr);
			SetLastError(0);
			SetErrorMode(em);

			return NULL;
		}

		SetLastError(0);
		SetErrorMode(em);
		return dso_handle;
	}

	s_strcpy(buf, filename, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_win32_dll_open",
				  "Call to \"s_strcpy\" failed"))
	{
		SetLastError(0);
		SetErrorMode(em);
		return NULL;
	}

	for (p = buf; *p != '\0'; p++)
	{
		if (*p == '/')
		{
			*p = '\\';
		}
	}

	dso_handle = LoadLibraryEx((LPSTR)buf, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	if (dso_handle == NULL)
	{
		last_error = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					  last_error, 0,
					  errstr, sizeof(errstr), NULL);

		S_CTX_ERR(error, S_FAILURE,
				  "s_win32_dll_open",
				  "Failed to open dso at \'%s\'. Reported error \"%s\"",
				  filename, errstr);
	}

	SetLastError(0);
	SetErrorMode(em);

	return (void*)dso_handle;
}


/* wrapper for dlsym */
S_API void *s_win32_dll_sym(void *dso_handle, const char *symbol, s_erc *error)
{
	FARPROC sym_handle;
	UINT em;
	DWORD last_error;
	char errstr[65535];


	S_CLR_ERR(error);
	em = SetErrorMode(SEM_FAILCRITICALERRORS);
	SetLastError(0);

	sym_handle = GetProcAddress(dso_handle , symbol);

	if (sym_handle == NULL)
	{
		last_error = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					  last_error, 0,
					  errstr, sizeof(errstr), NULL);

		S_CTX_ERR(error, S_FAILURE,
				  "s_win32_dll_sym",
				  "Failed to load symbol \'%s\'. Reported error \"%s\"",
				  symbol, errstr);
		SetLastError(0);
		SetErrorMode(em);
		return NULL;
	}

	SetLastError(0);
	SetErrorMode(em);

	return (void*)sym_handle;
}


/* wrapper for dlclose */
S_API void s_win32_dll_close(void *dso_handle, s_erc *error)
{
	UINT em;
	DWORD last_error;
	char errstr[65535];
	BOOL rv;

	S_CLR_ERR(error);
	em = SetErrorMode(SEM_FAILCRITICALERRORS);
	SetLastError(0);


	rv = FreeLibrary((HMODULE)dso_handle);

	if (rv == 0)
	{
		last_error = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					  last_error, 0,
					  errstr, sizeof(errstr), NULL);


		S_CTX_ERR(error, S_FAILURE,
				  "s_win32_dll_close",
				  "Failed to close dso. Reported error \"%s\"",
				  errstr);

		SetLastError(0);
		SetErrorMode(em);
		return;
	}

	SetLastError(0);
	SetErrorMode(em);
}



