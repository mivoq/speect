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
/* Dynamic Shared Object open/symbols/close abstraction.                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "pluginmanager/dynamic_loading.h"

/*
 * Include the dso loading implementation.
 * dynamic_loading_impl.h is automatically created by the build system
 * from system tests.
 */
#include "pluginmanager/platform/dynamic_loading_impl.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* declare mutex */
S_DECLARE_MUTEX_STATIC(dll_mutex);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void *s_dso_open(const char *filename, s_erc *error)
{
	void *handle;


	S_CLR_ERR(error);
	s_mutex_lock(&dll_mutex);
	if (filename == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_dso_open",
				  "Argument \"filename\" is NULL");
		s_mutex_unlock(&dll_mutex);
		return NULL;
	}

	handle = _S_DSO_OPEN(filename, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_dso_open",
				  "Call to \"_S_DSO_OPEN\" failed"))
	{
		s_mutex_unlock(&dll_mutex);
		return NULL;
	}

	s_mutex_unlock(&dll_mutex);
	return handle;
}


S_API void *s_dso_sym(void *dso_handle, const char *symbol, s_erc *error)
{
	void *symbol_add;


	S_CLR_ERR(error);
	s_mutex_lock(&dll_mutex);
	if (dso_handle == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_dso_sym",
				  "Argument \"dso_handle\" is NULL");
		s_mutex_unlock(&dll_mutex);
		return NULL;
	}

	if (symbol == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_dso_sym",
				  "Argument \"symbol\" is NULL");
		s_mutex_unlock(&dll_mutex);
		return NULL;
	}

	symbol_add = _S_DSO_SYM(dso_handle, symbol, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_dso_open",
				  "Call to \"_S_DSO_SYM\" failed"))
	{
		s_mutex_unlock(&dll_mutex);
		return NULL;
	}

	s_mutex_unlock(&dll_mutex);
	return symbol_add;
}


S_API void s_dso_close(void *dso_handle, s_erc *error)
{
	S_CLR_ERR(error);
	s_mutex_lock(&dll_mutex);
	if (dso_handle == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_dso_close",
				  "Argument \"dso_handle\" is NULL");
		s_mutex_unlock(&dll_mutex);
		return;
	}

	_S_DSO_CLOSE(dso_handle, error);
	S_CHK_ERR(error, S_CONTERR,
			  "s_dso_close",
			  "Call to \"_S_DSO_CLOSE\" failed");
	s_mutex_unlock(&dll_mutex);
}


S_LOCAL void _s_dynamic_loading_init(s_erc *error)
{
	S_CLR_ERR(error);
	s_mutex_init(&dll_mutex);
}


S_LOCAL void _s_dynamic_loading_quit(s_erc *error)
{
	S_CLR_ERR(error);
	s_mutex_destroy(&dll_mutex);
}

