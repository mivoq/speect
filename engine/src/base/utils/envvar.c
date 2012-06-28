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
/* Environment variable routines.                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/envvar.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static uint initialized_count = 0;

S_DECLARE_MUTEX_STATIC(envvar_mutex);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_envvar_init(s_erc *error)
{
	S_CLR_ERR(error);

	if (initialized_count++ > 0)
		return;

	/* init mutex */
	s_mutex_init(&envvar_mutex);
}


S_LOCAL void _s_envvar_quit(s_erc *error)
{
	S_CLR_ERR(error);

	if ((initialized_count == 0)
		|| (--initialized_count > 0))
		return;

	/* destroy mutex (for s_getenv) */
	s_mutex_destroy(&envvar_mutex);
}


S_API char *s_getenv(const char *name, s_erc *error)
{
	char *ev_sys;


	S_CLR_ERR(error);
	if (initialized_count == 0)
	{
		S_NEW_ERR(error, S_FAILURE);
		S_ERR_PRINT(S_FAILURE,
					"s_getenv",
					"EnvVar module has not been initialized.\n"
					"This will lead to undefined behaviour. Call s_envvar_init(...)\n");
		return NULL;
	}

	if (name == NULL)
	{
		S_NEW_ERR(error, S_FAILURE);
		S_ERR_PRINT(S_ARGERROR, "s_getenv",
					"Argument \"name\" is NULL");
		return NULL;
	}

	/* lock mutex */
	s_mutex_lock(&envvar_mutex);

	/* wrapper to the platform system environment variable implementation */
	ev_sys = _S_ENVVAR_GET(name, error);
	if ((error != NULL) & (*error != S_SUCCESS))
	{
		S_ERR_PRINT(S_FAILURE, "s_getenv",
					"Call to \"_S_ENVVAR_GET\" failed");
		return NULL;
	}

	/* unlock mutex */
	s_mutex_unlock(&envvar_mutex);
	return ev_sys;
}


S_API char *s_getenv_plugin_path(s_erc *error)
{
	char *tmp;


	S_CLR_ERR(error);
	if (initialized_count == 0)
	{
		S_NEW_ERR(error, S_FAILURE);
		S_ERR_PRINT(S_FAILURE,
					"s_getenv_plugin_path",
					"EnvVar module has not been initialized.\n"
					"This will lead to undefined behaviour. Call s_envvar_init(...)\n");
		return NULL;
	}

	/* lock mutex */
	s_mutex_lock(&envvar_mutex);

	/* wrapper to the platform Speect plug-in path */
	tmp = _S_GETENV_PLUGIN_PATH(error);
	if ((error != NULL) & (*error != S_SUCCESS))
	{
		S_ERR_PRINT(S_FAILURE, "s_getenv_plugin_path",
					"Call to \"_S_GETENV_PLUGIN_PATH\" failed");
		return NULL;
	}

	/* unlock mutex */
	s_mutex_unlock(&envvar_mutex);
	return tmp;
}

