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
/* Library implementation. A library encapsulates the DSO and plug-in               */
/* information, and is used inside the plug-in object. This allows us               */
/* to easily share plug-ins without needing to reload them.                         */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "base/strings/strings.h"
#include "pluginmanager/manager.h"
#include "pluginmanager/library.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Call the given function method of the given #SLibrary.
 * @param SELF The given #SLibrary*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage:
 @verbatim
 S_LIBRARY_CALL(self, func)(param1, param2, ..., paramN);
 @endverbatim
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_LIBRARY_CALL(SELF, FUNC)					\
	((SLibraryClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SLibrary
 * can be called.
 * @param SELF The given #SLibrary*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_LIBRARY_METH_VALID(SELF, FUNC)			\
	S_LIBRARY_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SLibraryClass LibraryClass; /* Library class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SLibraryLoad(SLibrary *self, const char *path, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SLibraryLoad",
				  "Argument \"self\" is NULL");
		return;
	}

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SLibraryLoad",
				  "Argument \"path\" is NULL");
		return;
	}

	if (!S_LIBRARY_METH_VALID(self, load))
	{
		S_WARNING(S_METHINVLD,
				  "SLibraryLoad",
				  "Dso method \"load\" not implemented");
		return;
	}

	s_mutex_lock(&(self->library_mutex));
	S_LIBRARY_CALL(self, load)(self, path, error);
	s_mutex_unlock(&(self->library_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "SLibraryLoad",
			  "Call to class method \"load\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_library_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&LibraryClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_library_class_add",
			  "Failed to add SLibraryClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitLibrary(void *obj, s_erc *error)
{
	SLibrary *self = obj;

	S_CLR_ERR(error);
	self->dso = NULL;
	self->plugin_info = NULL;
	self->path = NULL;
	self->in_pluginmanager = FALSE;
	self->ready = FALSE;              /* set and queried by plug-in
										 object */
	s_mutex_init(&(self->library_mutex));
}


static void DestroyLibrary(void *obj, s_erc *error)
{
	SLibrary *self = obj;
	s_plugin_exit_fp exit_function;
	s_erc local_err = S_SUCCESS;


	S_CLR_ERR(error);
	s_mutex_lock(&(self->library_mutex));

	if (self->dso != NULL)
	{
		exit_function = self->plugin_info->at_exit;

		if (exit_function != NULL)
		{
			exit_function(&local_err);
			S_CHK_ERR(&local_err, S_CONTERR,
					  "DestroyLibrary",
					  "Plugin dso at_exit function reported error");
		}

		S_DELETE(self->dso, "DestroyLibrary", error);
	}

	if (self->path != NULL)
		S_FREE(self->path);

	self->plugin_info = NULL;
	self->in_pluginmanager = FALSE;
	self->ready = FALSE;

	if ((local_err != S_SUCCESS) && (*error == S_SUCCESS))
		*error = local_err;

	s_mutex_unlock(&(self->library_mutex));
	s_mutex_destroy(&(self->library_mutex));
}


static void DisposeLibrary(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
	_s_pm_unload_library(S_LIBRARY(obj), error);
}


/* name change because of clash in Windows */
static void LoadLib(SLibrary *self, const char *path, s_erc *error)
{
	SDso *libraryDso;
	s_plugin_init_fp plugin_initialize;


	S_CLR_ERR(error);

	/* path gets freed when SLibrary is deleted */
	self->path = s_strdup(path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadLib",
				  "Call to \"s_strdup\" failed"))
		return;

	libraryDso = S_NEW(SDso, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadLib",
				  "Failed to create new dynamic shared object for Library"))
		return;

	SDsoLoad(libraryDso, path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadLib",
				  "Failed to load plug-in dynamic shared object at \"%s\"",
				  path))
	{
		S_DELETE(libraryDso, "LoadLib", error);
		return;
	}

	/*
	 * get the plugin initialization function
	 * done as described in dlsym man page.
	 */
	plugin_initialize = (s_plugin_init_fp)SDsoGetSymbol(libraryDso, "s_plugin_init", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadLib",
				  "Failed to get \'s_plugin_init\' symbol from plug-in at \"%s\"",
				  path))
	{
		S_DELETE(libraryDso, "LoadLib", error);
		return;
	}

	if (plugin_initialize == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "LoadLib",
				  "Plug-in symbol \'s_plugin_init\' is NULL for plug-in at \"%s\"",
				  path);
		S_DELETE(libraryDso, "LoadLib", error);
		return;
	}

	/* run the plug-in initialization function */
	self->plugin_info = (plugin_initialize)(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadLib",
				  "Call to \"plugin_initialize\" failed"))
	{
		S_DELETE(libraryDso, "LoadLib", error);
		return;
	}

	/* check version compatibility */
	if (!s_version_ok(self->plugin_info->s_abi))
	{
		S_CTX_ERR(error, S_FAILURE,
				  "LoadLib",
				  "Plug-in at \"%s\" (compiled ABI version %d.%d) is not compatible with this ABI version (%d.%d.%s) of the Speect Engine",
				  path, self->plugin_info->s_abi.major, self->plugin_info->s_abi.minor,
				  S_MAJOR_VERSION, S_MINOR_VERSION, S_PATCHLEVEL);
		S_DELETE(libraryDso, "LoadLib", error);
		return;
	}

	/* call plug-in register function */
	if (self->plugin_info->reg_func != NULL)
	{
		self->plugin_info->reg_func(error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "LoadLib",
					  "Plug-in at \"%s\" failed to register",
					  path))
		{
			S_DELETE(libraryDso, "LoadLib", error);
			return;
		}
	}

	self->dso = libraryDso;
}



/************************************************************************************/
/*                                                                                  */
/* SLibrary class initialization                                                    */
/*                                                                                  */
/************************************************************************************/

static SLibraryClass LibraryClass =
{
	/* SObjectClass */
	{
		"SLibrary",
		sizeof(SLibrary),
		{ 0, 1},
		InitLibrary,       /* init    */
		DestroyLibrary,    /* destroy */
		DisposeLibrary,    /* dispose */
		NULL,              /* compare */
		NULL,              /* print   */
		NULL,              /* copy    */
	},
	/* SLibraryClass */
	LoadLib                /* load    */
};
