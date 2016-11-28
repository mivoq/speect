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
/* WIN32 environment variable functions.                                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <windows.h>
#include "base/strings/strings.h"
#include "base/utils/platform/win32/win32_envvar.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL char *s_win32_envvar_get(const char *name, s_erc *error)
{
	char *ev_sys;
	HKEY hKey;
	unsigned char key_value[512] = "";
	DWORD key_value_size = sizeof(key_value);
	DWORD key_value_type = 0;


	S_CLR_ERR(error);

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					 "SOFTWARE\\Speect",
					 0,
					 KEY_QUERY_VALUE,
					 &hKey) != ERROR_SUCCESS)
	{
		S_NEW_ERR(error, S_FAILURE);
		S_ERR_PRINT(S_FAILURE, "s_win32_envvar_get",
				"Call to \"RegOpenKeyEx\" failed");
		return NULL;
	}

	if (RegQueryValueEx(hKey,
						name,
						NULL,
						&key_value_type,
						key_value,
						&key_value_size) != ERROR_SUCCESS)
	{
		S_NEW_ERR(error, S_FAILURE);
		S_ERR_PRINT(S_FAILURE, "s_win32_envvar_get",
					"Call to \"RegQueryValueEx\" failed");
		return NULL;
	}

	/* Make sure it is NULL terminted */
	key_value[sizeof(key_value)-1] = 0;
	RegCloseKey(hKey);

	if (key_value == NULL)
		return NULL;

	ev_sys = s_strdup_clib((char*)key_value);
	if (ev_sys == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "s_win32_envvar_get",
					"Call to \"s_strdup\" failed");
		return NULL;
	}

	return ev_sys;
}


S_LOCAL char *s_win32_getenv_plugin_path(s_erc *error)
{
	char *tmp;


	S_CLR_ERR(error);

	/* Speect plug-in path is defined in registry as
	 * HKEY_LOCAL_MACHINE\\SOFTWARE\\Speect\\plugin_path
	 */
	tmp = s_win32_envvar_get("plugin_path", error);
	if ((error != NULL) & (*error != S_SUCCESS))
	{
		S_ERR_PRINT(S_FAILURE, "s_win32_getenv_plugin_path",
					"Call to \"s_win32_envvar_get\" failed");
		return NULL;
	}

	return tmp;
}
