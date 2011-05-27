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
/* Dynamic Shared Object support.                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "pluginmanager/dso.h"
#include "pluginmanager/dynamic_loading.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Call the given function method of the given #SDso.
 * @param SELF The given #SDso*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage:
 @verbatim
 S_DSO_CALL(self, func)(param1, param2, ..., paramN);
 @endverbatim
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_DSO_CALL(SELF, FUNC)					\
	((SDsoClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SDso
 * can be called.
 * @param SELF The given #SDso*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_DSO_METH_VALID(SELF, FUNC)			\
	S_DSO_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

/*
 * This is for testing memory leaks in plug-ins. Uncomment  to
 * check a newly created plug-in for memory leaks. When set, the
 * plug-in manager will not unload plug-ins, thereby allowing valgrind
 * to find the leaks.
 * Note that this will cause leaks because of unloaded plug-ins, but
 * it should be easily distinguishable from other memory leaks.
 */
/* #define SPCT_VALGRIND_PLUGINS */


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SDsoClass DsoClass; /* Dso class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SDsoLoad(SDso *self, const char *path, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SDsoLoad",
				  "Argument \"self\" is NULL");
		return;
	}

	if (!S_DSO_METH_VALID(self, load))
	{
		S_WARNING(S_METHINVLD,
				  "SDsoLoad",
				  "Dso method \"load\" not implemented");
		return;
	}

	s_mutex_lock(&(self->dso_mutex));
	S_DSO_CALL(self, load)(self, path, error);
	s_mutex_unlock(&(self->dso_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "SDsoLoad",
			  "Call to class method \"load\" failed");
}


S_API void *SDsoGetSymbol(const SDso *self, const char *name, s_erc *error)
{
	void *sym_handle;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SDsoGetSymbol",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SDsoGetSymbol",
				  "Argument \"name\" is NULL");
		return NULL;
	}

	if (!S_DSO_METH_VALID(self, get_symbol))
	{
		S_WARNING(S_METHINVLD,
				  "SDsoLoad",
				  "Dso method \"get_symbol\" not implemented");
		return NULL;
	}

	s_mutex_lock((s_mutex*)&(self->dso_mutex));
	sym_handle = S_DSO_CALL(self, get_symbol)(self, name, error);
	s_mutex_unlock((s_mutex*)&(self->dso_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
				  "SDsoLoad",
				  "Call to class method \"get_symbol\" failed"))
		return NULL;

	return sym_handle;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_dso_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&DsoClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_dso_class_add",
			  "Failed to add SDsoClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitDso(void *obj, s_erc *error)
{
	SDso *self = obj;

	S_CLR_ERR(error);
	self->_handle = NULL;
	s_mutex_init(&(self->dso_mutex));
}


static void DestroyDso(void *obj, s_erc *error)
{
	SDso *self = obj;


	S_CLR_ERR(error);
	s_mutex_lock(&(self->dso_mutex));

	if (self->_handle != NULL)
	{
#ifndef SPCT_VALGRIND_PLUGINS  /* for memory leaks testing, see top */
		s_dso_close(self->_handle, error);
#endif
		S_CHK_ERR(error, S_CONTERR,
				  "DestroyDso",
				  "Call to \"s_dso_close\" failed");
	}

	s_mutex_unlock(&(self->dso_mutex));
	s_mutex_destroy(&(self->dso_mutex));
}


static void DisposeDso(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void LoadDso(SDso *self, const char *path, s_erc *error)
{
	void *dso_handle;


	S_CLR_ERR(error);

	dso_handle = s_dso_open(path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadDso",
				  "Call to \"s_dso_open\" failed"))
		return;

	self->_handle = dso_handle;
}


static void *GetSymbol(const SDso *self, const char* name, s_erc *error)
{
	void *sym_handle;


	S_CLR_ERR(error);

	if (self->_handle == NULL)
		return NULL;

	sym_handle = s_dso_sym(self->_handle, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetSymbol",
				  "Call to \"s_dso_sym\" failed"))
		return NULL;

	return sym_handle;
}


/************************************************************************************/
/*                                                                                  */
/* SDso class initialization                                                        */
/*                                                                                  */
/************************************************************************************/

static SDsoClass DsoClass =
{
	/* SObjectClass */
	{
		"SDso",
		sizeof(SDso),
		{ 0, 1},
		InitDso,       /* init    */
		DestroyDso,    /* destroy */
		DisposeDso,    /* dispose */
		NULL,          /* compare */
		NULL,          /* print   */
		NULL,          /* copy    */
	},
	/* SDsoClass */
	LoadDso,          /* load       */
	GetSymbol         /* get_symbol */
};
