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
/* DATE    : 5 December 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Definition of the plug-in object.                                                */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_OBJECT_H__
#define _SPCT_PLUGIN_OBJECT_H__


/**
 * @file plugin_object.h
 * Definition of the plug-in object.
 */


/**
 * @ingroup SPlugins
 * @defgroup SPluginObject Plug-in Objects
 * Plug-in objects encapsulate the loading/unloading and resource acquisition
 * of <i>dynamic shared objects</i>.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/objsystem/objsystem.h"
#include "base/errdbg/errdbg.h"
#include "pluginmanager/dso.h"
#include "pluginmanager/plugin.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @name Utility Macros
 * @{
 */


/**
 * @hideinitializer
 * Return the given #SPlugin child class object as a plug-in object.
 *
 * @param SELF The given object.
 * @return Given object as #SPlugin* type.
 *
 * @note This casting is not safety checked.
 */
#define S_PLUGIN(SELF)    ((SPlugin *)(SELF))


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* SPlugin definition                                                               */
/*                                                                                  */
/************************************************************************************/

/**
 * The SPlugin structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject                obj;

	/**
	 * @protected Dynamic shared object associated with this plug-in.
	 */
	const SDso            *library;

	/**
	 * @protected Plug-in information.
	 */
	const s_plugin_params *plugin_info;

	/**
	 * @protected Plug-in path.
	 */
	const char            *path;

	/**
	 * @private Plug-in was loaded into plug-in manager.
	 */
	s_bool                 in_pluginmanager;

	/**
	 * @private Is the plug-in loaded and ready to be used?
	 */
	s_bool                 ready;

	/**
	 * @protected Main locking mutex
	 */
	S_DECLARE_MUTEX(plugin_mutex);
} SPlugin;


/************************************************************************************/
/*                                                                                  */
/* SPluginClass definition                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * The SPluginClass structure.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SObjectClass.
	 */
	SObjectClass _inherit;

	/* Class methods */
	/**
	 * @private IsReady function pointer.
	 * Query whether the plug-in is ready for use. A plug-in can be in
	 * cache and returned to a caller without being completely loaded
	 * and ready for use. This is especially true for multi-threaded
	 * applications. Use this call to check whether the plug-in is
	 * ready to be used.
	 *
	 * @param self The plug-in to query.
	 *
	 * @return #TRUE if ready for use, otherwise #FALSE.
	 */
	s_bool (* const is_ready) (const SPlugin *self);

	/**
	 * @private SetReady function pointer.
	 * Set the ready flag of the plug-in. This must only be set after
	 * a plug-in has been fully loaded, so that threaded applications
	 * can now when a plug-in is ready for use.
	 *
	 * @param self The plug-in to set the ready flag for.
	 */
	void   (* const set_ready)(SPlugin *self);
} SPluginClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Query whether the plug-in is ready for use. A plug-in can be in
 * cache and returned to a caller without being completely loaded
 * and ready for use. This is especially true for multi-threaded
 * applications. Use this call to check whether the plug-in is
 * ready to be used. Used internally in the PluginManager.
 *
 * @private @memberof SPlugin
 *
 *
 * @param self The plug-in to query.
 *
 * @return #TRUE if ready for use, otherwise #FALSE.
 */
S_LOCAL s_bool SPluginIsReady(const SPlugin *self);


/**
 * Set the ready flag of the plug-in. This must only be set after
 * a plug-in has been fully loaded, so that threaded applications
 * can now when a plug-in is ready for use. Used internally in the
 * PluginManager.
 *
 * @private @memberof SPlugin
 *
 * @param self The plug-in to set the ready flag for.
 */
S_LOCAL void SPluginSetReady(SPlugin *self);


/**
 * Add the SPlugin class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_plugin_class_add(s_erc *error);


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

#endif /* _SPCT_PLUGIN_OBJECT_H__ */

