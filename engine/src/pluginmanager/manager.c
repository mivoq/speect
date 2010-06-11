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
#include "containers/containers.h"
#include "serialization/json/json_parse_config.h"
#include "pluginmanager/manager.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

/* Minimum major version of Speect Engine ABI from plug-ins that will
 * work with this version of Speect Engine */
#define SPCT_MAJOR_VERSION_MIN 0

/* Minimum minor version of Speect Engine ABI from plug-ins that will
 * work with this version of Speect Engine */
#define SPCT_MINOR_VERSION_MIN 8


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SMap *pluginCache = NULL;

static s_bool initialized = FALSE;

S_DECLARE_MUTEX_STATIC(pm_mutex);

static char *plugin_path = NULL;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void load_plugin(SPlugin *self, const char *path, s_erc *error);

static void cache_add_plugin(SPlugin *plugin, const char *path, s_erc *error);

static void cache_remove_plugin(SPlugin *plugin, s_erc *error);

static char *fix_path(const char *path, s_erc *error);

static s_bool version_ok(const s_version abi_version);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API SPlugin *s_pm_load_plugin(const char *path, s_erc *error)
{
	SPlugin *loaded;
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
	new_path = fix_path(path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_load_plugin",
				  "Call to \"fix_path\" failed"))
		return NULL;

	/* we know it is a plugin, therefore the unsafe cast */
	loaded = S_PLUGIN(SMapGetObjectDef(pluginCache, new_path, NULL, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_load_plugin",
				  "Call to \"SMapGetObjectDef\" failed"))
	{
		S_FREE(new_path);
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
		s_mutex_unlock(&pm_mutex);
		return loaded;
	}

	/*
	 * it has not been cached, create a new plugin
	 * object. Cache and then load, this overcomes
	 * recursion problem.
	 */
	loaded = (SPlugin*)S_NEW("SPlugin", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_load_plugin",
				  "Failed to create new plug-in object"))
	{
		S_FREE(new_path);
		s_mutex_unlock(&pm_mutex);
		return NULL;
	}

	cache_add_plugin(loaded, new_path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_load_plugin",
				  "Failed to cache plug-in at \'%s\'", new_path))
	{
		S_FORCE_DELETE(loaded, "s_pm_load_plugin", error);
		S_FREE(new_path);
		s_mutex_unlock(&pm_mutex);
		return NULL;
	}

	/*
	 * unlock manager mutex, now other threads
	 * can load plugins
	 */
	s_mutex_unlock(&pm_mutex);

	load_plugin(loaded, new_path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_load_plugin",
				  "Failed to load plug-in at \'%s\'", new_path))
	{
		cache_remove_plugin(loaded, NULL);
		S_FORCE_DELETE(loaded, "s_pm_load_plugin", error);
		S_FREE(new_path);
		return NULL;
	}

	S_FREE(new_path);

	/*
	 * it's loaded and added to cache,
	 * increase reference count and return it.
	 */
	SObjectIncRef(S_OBJECT(loaded)); /* ref = 2, caller and cache */
	return loaded;
}


S_API s_bool s_pm_plugin_loaded(const char *path, s_erc *error)
{
	s_bool cached_plugin;
	char *new_path;


	S_CLR_ERR(error);

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_pm_plugin_loaded",
				  "Argument \"path\" is NULL");
		return FALSE;
	}

	/*
	 * if path does not contain a path separator we concatenate the
	 * default path.
	 */
	new_path = fix_path(path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_plugin_loaded",
				  "Call to \"fix_path\" failed"))
		return FALSE;

	/* check if the plugin is in the cache */
	cached_plugin = SMapObjectPresent(pluginCache, new_path,
									  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pm_plugin_loaded",
				  "Call to \"SMapObjectPresent\" failed for plug-in path \'%s\'",
				  new_path))
	{
		S_FREE(new_path);
		return FALSE;
	}

	S_FREE(new_path);
	return cached_plugin;
}


S_LOCAL void _s_pm_unload_plugin(SPlugin *plugin, s_erc *error)
{
	S_CLR_ERR(error);

	if (S_OBJECT_REF(plugin) != 1)
	{
		/* nothing to do */
		return;
	}

	if (plugin->in_pluginmanager == FALSE)
	{
		/*
		 * plugin does not belong to plugin manager,
		 * nothing to do.
		 */
		return;
	}

	/*
	 * The plugin must be unloaded from the
	 * pluginmanager. Unlink from cache and
	 * force delete.
	 */

	/*
	 * we dont need this return value, as we
	 * already have hold of plugin.
	 */
	cache_remove_plugin(plugin, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_pm_unload_plugin",
			  "Failed to remove plug-in,  at \'%s\', from cached",
			  plugin->path);

	/*
	 * Make sure that it will be deleted.
	 */
	S_OBJECT(plugin)->ref = 0;
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

	pluginCache = S_MAP(S_NEW("SMapHashTable", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_pm_init",
				  "Failed to create new SMapHashTable for plug-in cache"))
	{
		s_mutex_destroy(&pm_mutex);
		initialized = FALSE;
		return;
	}

	/* 128 plugins for now */
	SMapHashTableInit(&pluginCache, 128, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_pm_init",
				  "Failed to initialize SMapHashTable for plug-in cache"))
	{
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
	S_DELETE(pluginCache, "_s_pm_init", error);

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

static void load_plugin(SPlugin *self, const char *path, s_erc *error)
{
	SDso *pluginDso;
	s_plugin_init_fp plugin_initialize;


	S_CLR_ERR(error);

	/* path gets freed when SPlugin is deleted */
	self->path = s_strdup(path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadPlugin",
				  "Call to \"s_strdup\" failed"))
		return;

	pluginDso = (SDso*)S_NEW("SDso", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadPlugin",
				  "Failed to create new dynamic shared object for plug-in"))
		return;


	SDsoLoad(pluginDso, path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadPlugin",
				  "Failed to load plug-in dynamic shared object at \"%s\"",
				  path))
	{
		S_DELETE(pluginDso, "PluginLoad", error);
		return;
	}

	/*
	 * get the plugin initialization function
	 * done as described in dlsym man page.
	 */
	*(void**)(&plugin_initialize) = SDsoGetSymbol(pluginDso, "s_plugin_init", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadPlugin",
				  "Failed to get \'s_plugin_init\' symbol from plug-in at \"%s\"",
				  path))
	{
		S_DELETE(pluginDso, "PluginLoad", error);
		return;
	}

	if (plugin_initialize == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "LoadPlugin",
				  "Plug-in symbol \'s_plugin_init\' is NULL for plug-in at \"%s\"",
				  path);
		S_DELETE(pluginDso, "PluginLoad", error);
		return;
	}

	self->plugin_info = (plugin_initialize)(s_speect_version(), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadPlugin",
				  "Call to \"s_plugin_init\" failed"))
	{
		S_DELETE(pluginDso, "PluginLoad", error);
		return;
	}

	if (self->plugin_info == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "LoadPlugin",
				  "Plug-in at \"%s\" failed to return s_plugin_params structure information",
				  path);
		S_DELETE(pluginDso, "PluginLoad", error);
		return;
	}

	if (!version_ok(self->plugin_info->s_abi))
	{
		s_lib_version s_version = s_speect_version();


		S_CTX_ERR(error, S_FAILURE,
				  "LoadPlugin",
				  "Plug-in at \"%s\" is not compatible with this ABI version (%d.%d.%d) of the Speect Engine",
				  path, s_version.major, s_version.minor, s_version.patch);
		S_DELETE(pluginDso, "PluginLoad", error);
		return;
	}

	self->library = pluginDso;
	self->in_pluginmanager = TRUE;
}


/* path might not yet be set in plugin when we want to add it. */
static void cache_add_plugin(SPlugin *plugin, const char *path, s_erc *error)
{
	SMapSetObject(pluginCache, path, S_OBJECT(plugin), error);
	S_CHK_ERR(error, S_CONTERR,
			  "cache_add_plugin",
			  "Call to \"SMapSetObject\" failed, could not"
			  " add plugin at \'%s\' to cache", path);
}


static void cache_remove_plugin(SPlugin *plugin, s_erc *error)
{
	SMapObjectUnlink(pluginCache, plugin->path,
					 error);
	S_CHK_ERR(error, S_CONTERR,
			  "cache_remove_plugin",
			  "Call to \"SMapObjectUnlink\" failed, could not"
			  " remove plug-in at \'%s\' from cache",
			  plugin->path? plugin->path : "NULL");
}


static char *fix_path(const char *path, s_erc *error)
{
	char *new_path;
	const char *ps;


	S_CLR_ERR(error);

	/* look for path seperator */
	ps = s_strchr(path, S_PATH_SEP, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "fix_path",
				  "Call to \"s_strchr\" failed"))
		return NULL;

	if (ps != NULL) /* we have path separators, just copy and return */
	{
		new_path = s_strdup(path, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "fix_path",
					  "Call to \"s_strdup\" failed"))
			return NULL;

		return new_path;
	}

	/* no path separators, concatenate with default path */
	s_asprintf(&new_path, error, "%s%c%s", plugin_path, S_PATH_SEP, path);
	if (S_CHK_ERR(error, S_CONTERR,
				  "fix_path",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return new_path;
}


/* check the Speect ABI version of plug-in */
static s_bool version_ok(const s_version abi_version)
{
	if ((abi_version.major >= SPCT_MAJOR_VERSION_MIN)
		&& (abi_version.minor >= SPCT_MINOR_VERSION_MIN))
		return TRUE;

	return FALSE;
}
