/************************************************************************************/
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
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
/* DATE    : June 2012                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* POSIX environment variable functions.                                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdlib.h>
#include "base/strings/strings.h"
#include "base/utils/platform/posix/posix_envvar.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL char *s_posix_envvar_get(const char *name, s_erc *error)
{
	char *tmp = NULL;
	char *ev_sys;


	S_CLR_ERR(error);
	tmp = getenv(name);

	/* to give us same behaviour as win32 implementation */
	if (tmp == NULL)
		return NULL;

	ev_sys = s_strdup_clib(tmp);
	if (ev_sys == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "s_posix_envvar_get",
					"Call to \"s_strdup_clib\" failed");
		return NULL;
	}

	return ev_sys;
}


S_LOCAL char *s_posix_getenv_plugin_path(s_erc *error)
{
	char *tmp;


	S_CLR_ERR(error);

	/* Speect plug-in path is defined in environment as
	 * SPCT_PLUGIN_PATH
	 */
	tmp = s_posix_envvar_get("SPCT_PLUGIN_PATH", error);
	if ((error != NULL) & (*error != S_SUCCESS))
	{
		S_ERR_PRINT(S_FAILURE, "s_posix_getenv_plugin_path",
					"Call to \"s_posix_envvar_get\" failed");
		return NULL;
	}

	return tmp;
}

