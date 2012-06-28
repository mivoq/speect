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
/* DATE    : 8 December 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Implementation of the plugin manager.                                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include "base/strings/strings.h"
#include "base/strings/sprint.h"
#include "base/utils/alloc.h"
#include "base/utils/vernum.h"
#include "containers/containers.h"
#include "serialization/json/json_parse_config.h"
#include "pluginmanager/manager.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

/* 128 libraries for now, used for cache initialization size (can grow
 * but expensive) */
#define SPCT_MAX_NUM_LIBRARIES 128


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SMap *libraryCache = NULL;

static s_bool initialized = FALSE;

S_DECLARE_MUTEX_STATIC(pm_mutex);

static char *plugin_path = NULL;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void cache_add_library(SLibrary *library, const char *path, s_erc *error);

static void cache_remove_library(SLibrary *library, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API SPlugin *s_pm_load_plugin(const char *path, s_erc *error)
{
	SPlugin *plugin;
	SLibrary *loaded;
	char *new_path;


	S_CLR_ERR(error);

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_pm_load_plugin",
				  "Argument \"path\" is NULL");
		return NULL;
	}

	s_mutex_lock(&pm_mutex);

	/*
	 * if path does not contain a path separator we concatenate the
	 * default path.
	 */
	new_path = s_pm_get_plugin_path(path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_load_plugin",
				  "Call to \"s_pm_get_plugin_path\" failed"))
		return NULL;

	/* Create a new plug-in object */
	plugin = S_NEW(SPlugin, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_load_plugin",
				  "Failed to create new plug-in object"))
	{
		S_FREE(new_path);
		s_mutex_unlock(&pm_mutex);
		return NULL;
	}

	/*
	 * get the library that is associated with this path from the
	 * cache, we know it is a library, therefore the unsafe cast
	 */
	loaded = S_LIBRARY(SMapGetObjectDef(libraryCache, new_path, NULL, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_load_plugin",
				  "Call to \"SMapGetObjectDef\" failed"))
	{
		S_FREE(new_path);
		S_DELETE(plugin, "s_pm_load_plugin", error);
		s_mutex_unlock(&pm_mutex);
		return NULL;
	}

	if (loaded != NULL)
	{
		/*
		 * it's loaded, increase reference count
		 * and return it.
		 */
		SObjectIncRef(S_OBJECT(loaded));
		S_FREE(new_path);
		plugin->library = loaded;

		s_mutex_unlock(&pm_mutex);

		/* wait until plug-in library has loaded before
		 * returning and using it */
		while (!SPluginIsReady(plugin))
		{
			/* NOP */
		}

		return plugin;
	}

	/*
	 * it has not been cached, create a new library
	 * object. Cache and then load, this overcomes
	 * recursion problem.
	 */
	loaded = S_NEW(SLibrary, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_load_plugin",
				  "Failed to create new library object"))
	{
		S_FREE(new_path);
		S_DELETE(plugin, "s_pm_load_plugin", error);
		s_mutex_unlock(&pm_mutex);
		return NULL;
	}

	cache_add_library(loaded, new_path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_load_plugin",
				  "Failed to cache library at \'%s\'", new_path))
	{
		S_DELETE(loaded, "s_pm_load_plugin", error);
		S_FREE(new_path);
		S_DELETE(plugin, "s_pm_load_plugin", error);
		s_mutex_unlock(&pm_mutex);
		return NULL;
	}

	/*
	 * unlock manager mutex, now other threads
	 * can load plugins
	 */
	s_mutex_unlock(&pm_mutex);

	/* load library */
	SLibraryLoad(loaded, new_path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_load_plugin",
				  "Failed to load libraery at \'%s\'", new_path))
	{
		cache_remove_library(loaded, NULL);
		S_DELETE(loaded, "s_pm_load_plugin", error);
		S_FREE(new_path);
		S_DELETE(plugin, "s_pm_load_plugin", error);;
		return NULL;
	}

	S_FREE(new_path);

	/* set the plug-in manager flag so that we know that this library
	 * was loaded by the plug-in manager */
	loaded->in_pluginmanager = TRUE;

	/*
	 * it's loaded and added to cache,
	 * increase reference count and return it.
	 */
	SObjectIncRef(S_OBJECT(loaded));  /* ref = 2, caller and cache */

	plugin->library = loaded;

	/* other threads can start using the plug-in */
	SPluginSetReady(plugin);

	return plugin;
}


S_API char *s_pm_get_plugin_path(const char *path, s_erc *error)
{
	char *new_path;
	const char *ps;


	S_CLR_ERR(error);

	/* look for path seperator */
	ps = s_strchr(path, S_PATH_SEP, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_get_plugin_path",
				  "Call to \"s_strchr\" failed"))
		return NULL;

	if (ps != NULL) /* we have path separators, just copy and return */
	{
		new_path = s_strdup(path, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pm_get_plugin_path",
					  "Call to \"s_strdup\" failed"))
			return NULL;

		return new_path;
	}

	/* no path separators, concatenate with default path, if any */
	if (plugin_path != NULL)
	{
		s_asprintf(&new_path, error, "%s%c%s", plugin_path, S_PATH_SEP, path);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pm_get_plugin_path",
					  "Call to \"s_asprintf\" failed"))
			return NULL;
	}
	else /* we don't have a plugin_path */
	{
		new_path = s_strdup(path, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pm_get_plugin_path",
					  "Call to \"s_strdup\" failed"))
			return NULL;
	}

	return new_path;
}


S_LOCAL void _s_pm_unload_library(SLibrary *library, s_erc *error)
{
	S_CLR_ERR(error);

	if (S_OBJECT_REF(library) != 1)
	{
		/* nothing to do */
		return;
	}

	if (library->in_pluginmanager == FALSE)
	{
		/*
		 * library does not belong to plug-in manager,
		 * nothing to do.
		 */
		return;
	}

	/*
	 * The library must be unloaded from the
	 * plug-in manager. Unlink from cache and
	 * force delete.
	 */

	/*
	 * we dont need this return value, as we
	 * already have hold of library.
	 */
	cache_remove_library(library, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_pm_unload_library",
			  "Failed to remove library,  at \'%s\', from cache",
			  library->path);

	/*
	 * Make sure that it will be deleted.
	 */
	S_OBJECT(library)->ref = 0;
}


S_LOCAL void _s_pm_set_plugin_path(const char *path, s_erc *error)
{
	S_CLR_ERR(error);

	s_mutex_lock(&pm_mutex);
	if (plugin_path != NULL)
		S_FREE(plugin_path);

	plugin_path = s_strdup(path, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_pm_set_plugin_path",
			  "Call to \"s_strdup\" failed");

	/* check that last char is not path separator */
	if (plugin_path[s_strsize(plugin_path, error) - 1] == S_PATH_SEP)
		plugin_path[s_strsize(plugin_path, error) - 1] = '\0';

	s_mutex_unlock(&pm_mutex);
}


S_LOCAL const char *_s_pm_get_plugin_path(s_erc *error)
{
	S_CLR_ERR(error);

	return plugin_path;
}


S_LOCAL void _s_pm_init(s_erc *error)
{
	S_CLR_ERR(error);

	if (initialized == TRUE)
		return;

	initialized = TRUE;
	s_mutex_init(&pm_mutex);

	libraryCache = S_MAP(S_NEW(SMapHashTable, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_pm_init",
				  "Failed to create new SMapHashTable for library cache"))
	{
		s_mutex_destroy(&pm_mutex);
		initialized = FALSE;
		return;
	}

	/* 128 plugins for now */
	SMapHashTableResize(S_MAPHASHTABLE(libraryCache), SPCT_MAX_NUM_LIBRARIES, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_pm_init",
				  "Failed to resize SMapHashTable for library cache"))
	{
		S_DELETE(libraryCache, "_s_pm_init", error);
		s_mutex_destroy(&pm_mutex);
		initialized = FALSE;
		return;
	}
}


S_LOCAL void _s_pm_quit(s_erc *error)
{
	S_CLR_ERR(error);

	s_mutex_lock(&pm_mutex);

	if (!initialized)
		return;

	initialized = FALSE;
	S_DELETE(libraryCache, "_s_pm_init", error);

	if (plugin_path != NULL)
		S_FREE(plugin_path);

	s_mutex_unlock(&pm_mutex);
	s_mutex_destroy(&pm_mutex);
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/


/* path might not yet be set in plugin when we want to add it. */
static void cache_add_library(SLibrary *library, const char *path, s_erc *error)
{
	SMapSetObject(libraryCache, path, S_OBJECT(library), error);
	S_CHK_ERR(error, S_CONTERR,
			  "cache_add_library",
			  "Call to \"SMapSetObject\" failed, could not"
			  " add library at \'%s\' to cache", path);
}


static void cache_remove_library(SLibrary *library, s_erc *error)
{
	SMapObjectUnlink(libraryCache, library->path,
					 error);
	S_CHK_ERR(error, S_CONTERR,
			  "cache_remove_library",
			  "Call to \"SMapObjectUnlink\" failed, could not"
			  " remove library at \'%s\' from cache",
			  library->path? library->path : "NULL");
}



