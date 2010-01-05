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
/* DATE    : 5 December 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Definition of data structures and functions for external plug-ins.               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_H__
#define _SPCT_PLUGIN_H__


/**
 * @file plugin.h
 * Data structures and functions for external plug-ins.
 */


/**
 * @ingroup SPlugins
 * @defgroup SPluginDef Plug-in Definitions
 * Definitions of plug-in functions and symbols. A plug-in @b must define at least one
 * externally visible symbol, namely @c s_plugin_init. This symbol must be a function
 * of the #s_plugin_init_fp type. The @ref SPluginManager will look up this symbol
 * and call this function after the plug-in has loaded. From this function the PluginManager
 * will get the #s_plugin_params defined by the plug-in. When the plug-in
 * is unloaded, the PluginManager will call the #s_plugin_exit_fp to free up any resources
 * used by the plug-in.
 * @todo plug-ins can load others
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/vernum.h"
#include "base/objsystem/objsystem.h"
#include "base/errdbg/errdbg.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

/* forward declaration */
/**
 * The plug-in parameters structure
 */
typedef struct s_plugin_params s_plugin_params;


/**
 * Plug-in start-up function. This is the first function called when a
 * plug-in is loaded. It must @b always be called @c s_plugin_init, as
 * this is the symbol name that is looked for. The function is passed the
 * Speect Engine version and must return a #s_plugin_params structure that
 * must be filled in by the plug-in itself.
 * @param version The Speect Engine version.
 * @param error Error code.
 * @return Pointer to the parameters that the plug-in must set, may be
 * statically allocated.
 * @note This function need not be re-entrant, and therefore not thread-safe.
 */
typedef const s_plugin_params *(*s_plugin_init_fp)(const s_lib_version version,
												   s_erc *error);


/**
 * Plug-in exit function. This function is called just before the <i>dynamic
 * shared object</i> that this plug-in belongs to is unloaded. Any acquired
 * resources must be released in this function.
 * @param error Error code.
 */
typedef void (*s_plugin_exit_fp)(s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The plug-in parameters structure. This structure is returned from a
 * plug-in on initialization.
 */
struct s_plugin_params
{
	/**
	 * The name of the plug-in.
	 */
	const char *name;

	/**
	 * A short description of the plug-in.
	 */
	const char *description;

	/**
	 * Plug-in version.
	 */
	const s_version version;

	/**
	 * Speect @c ABI version (which plug-in was compiled with)
	 */
	const s_version s_abi;

	/**
	 * Plug-in exit function pointer.
	 */
	const s_plugin_exit_fp at_exit;
};


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

#endif /* _SPCT_PLUGIN_DEF_H__ */

