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
/* DATE    : 4 December 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Dynamic Shared Object's with dl (POSIX) library calls.                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "pluginmanager/platform/posix/posix_dll.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/* wrapper for dlopen */
S_API void *s_dl_open(const char *filename, s_erc *error)
{
	void *dso_handle;
	char *errstr;


	S_CLR_ERR(error);
	dlerror();    /* Clear any existing error */

	dso_handle = dlopen(filename, RTLD_LAZY|RTLD_LOCAL);

	if (dso_handle == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
			  "s_dl_open",
			  "Failed to open dso at \'%s\'. Reported error \"%s\"",
			  filename? filename : "NULL", dlerror());
		return NULL;
	}

	errstr = dlerror();

	if (errstr != NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
			  "s_dl_open",
			  "DSO \'%s\' loaded but dynamic linking reported error \"%s\"",
			  filename? filename : "NULL", errstr);
	}

	return dso_handle;
}


/* wrapper for dlsym */
S_API void *s_dl_sym(void *dso_handle, const char *symbol, s_erc *error)
{
	void *sym_handle;
	char *errstr;


	S_CLR_ERR(error);
	dlerror();    /* Clear any existing error */

	sym_handle = dlsym(dso_handle, symbol);

	errstr = dlerror();

	if (errstr != NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
			  "s_dl_sym",
			  "symbol \'%s\' loaded but dynamic linking reported error \"%s\"",
			  symbol, errstr);
	}

	return sym_handle;
}


/* wrapper for dlclose */
S_API void  s_dl_close(void *dso_handle, s_erc *error)
{
	int rv;
	char *errstr;


	S_CLR_ERR(error);
	dlerror();    /* Clear any existing error */

	rv = dlclose(dso_handle);

	if (rv != 0)
	{
		S_CTX_ERR(error, S_FAILURE,
			  "s_dl_close",
			  "Failed to close dso. Reported error \"%s\"",
			  dlerror());
		return;
	}

	errstr = dlerror();

	if (errstr != NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
			  "s_dl_close",
			  "DSO closed but dynamic linking reported error \"%s\"",
			  errstr);
	}
}
