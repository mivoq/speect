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
/* Definition of the plugin manager.                                                */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_MANAGER_H__
#define _SPCT_PLUGIN_MANAGER_H__


/**
 * @file pluginmanager/manager.h
 * Definition of the PluginManager.
 */


/**
 * @ingroup SPlugins
 * @defgroup SPluginManager PluginManager
 * The @c PluginManager provides methods for loading plug-ins that
 * are then available to the Speect Engine and all other loaded plug-ins.
 * Loaded plug-ins are chached for reuse.
 * @todo explain NULL for errors (GLOBAL) see s_pm_load_plugin line 176
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "pluginmanager/plugin_object.h"
#include "base/errdbg/errdbg.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Load a plug-in from the given path. If the plug-in at the given
 * path has already been loaded, then the plug-in's reference counter
 * is increased and a pointer to the loaded plug-in is returned. This
 * reduces the need for multiple redundant calls. If the given path
 * does not include any path separators (just a file name) then the
 * path is concatenated with the default plug-in path.
 *
 * @param path The full path and name of the plug-in to load.
 * @param error Error code.
 *
 * @return Pointer to the loaded plug-in.
 *
 * @note The returned plug-in can be normally deleted with #S_DELETE.
 * @note Thread safe.
 */
S_API SPlugin *s_pm_load_plugin(const char *path, s_erc *error);


/**
 * Query if the plug-in at the given path is loaded in the
 * PluginManager cache. If the given path does not include any path
 * separators (just a file name) then the path is concatenated with
 * the default plug-in path.
 *
 * @param path he full path and name of the plug-in to query.
 * @param error Error code.
 *
 * @return #TRUE or #FALSE.
 *
 * @note Thread safe.
 */
S_API s_bool s_pm_plugin_loaded(const char *path, s_erc *error);


/**
 * Get the full plug-in path from the given path. If the given path
 * does not include any path separators (just a file name) then the
 * path is concatenated with the @a default plug-in path.
 *
 * @param path The plug-in file/path
 * @param error Error code.
 *
 * @return The full plug-in path
 *
 * @note The caller is responsible for the memory of the returned
 * string.
 */
S_API char *s_pm_get_plugin_path(const char *path, s_erc *error);


/**
 * Unload the given plug-in.
 * @private
 *
 * @param plugin The plug-in to unload.
 * @param error Error code.
 *
 * @note Used inernally in SPlugin::dispose.
 */
S_LOCAL void _s_pm_unload_plugin(SPlugin *plugin, s_erc *error);


/**
 * Set the default plug-in path of the PluginManager. The default path
 * is used as a @a prefix to the given plug-in paths in
 * #s_pm_load_plugin() if the paths are not absolute.
 * @private
 *
 * @param path The default plug-in path to set.
 * @param error Error code.
 */
S_LOCAL void _s_pm_set_plugin_path(const char *path, s_erc *error);


/**
 * Get the default plug-in path of the PluginManager. The default path
 * is used as a @a prefix to the given plug-in paths in
 * #s_pm_load_plugin() if the paths are not absolute.
 * @private
 *
 * @param error Error code.
 *
 * @return Pointer to the default plug-in path.
 */
S_LOCAL const char *_s_pm_get_plugin_path(s_erc *error);


/**
 * Initialize the PluginManager. Sets up the plug-in cache and
 * mutex.
 * @private
 *
 * @param error Error code.
 *
 * @note Not thread safe.
 */
S_LOCAL void _s_pm_init(s_erc *error);


/**
 * Quit the PluginManager. Clears the plug-in cache and
 * mutex.
 * @private
 *
 * @param error Error code.
 *
 * @note Not thread safe.
 */
S_LOCAL void _s_pm_quit(s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


/**
 * @}
 * end documentation
 */

#endif /* _SPCT_PLUGIN_MANAGER_H__ */

