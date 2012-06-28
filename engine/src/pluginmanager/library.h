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

#ifndef _SPCT_LIBRARY_H__
#define _SPCT_LIBRARY_H__


/**
 * @file library.h
 * Library implementation.
 */


/**
 * @ingroup SPlugins
 * @defgroup SLibrary Library Implementation
 * A library encapsulates the @ref SDso and plug-in information (@ref s_plugin_params),
 * and is used inside the plug-in object (@ref SPlugin). This allows
 * us to easily share plug-ins without needing to reload them.
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
 * Return the given #SLibrary child class object as a library object.
 * @param SELF The given object.
 * @return Given object as #SLibrary* type.
 * @note This casting is not safety checked.
 */
#define S_LIBRARY(SELF)    ((SLibrary *)(SELF))


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* SLibrary definition                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * The SLibrary structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject                obj;

	/**
	 * @protected Dynamic shared object associated with this library.
	 */
	SDso                  *dso;

	/**
	 * @protected Plug-in information of the library.
	 */
	const s_plugin_params *plugin_info;

	/**
	 * @protected Dynamic shared object path
	 */
	char                  *path;

	/**
	 * @private Library was loaded into plug-in manager.
	 */
	s_bool                 in_pluginmanager;

	/**
	 * @private Is the library loaded and ready to be used?
	 */
	s_bool                 ready;

	/**
	 * @protected Locking mutex
	 */
	S_DECLARE_MUTEX(library_mutex);
} SLibrary;


/************************************************************************************/
/*                                                                                  */
/* SLibraryClass definition                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * The SLibraryClass structure.
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
	 * @protected Load function pointer.
	 * Load a <i> dynamic shared object </i> from the given path into
	 * the given #SLibrary object, initialize the plug-in, and run the
	 * plug-in register function.
	 *
	 * @param self The given SLibrary object.
	 * @param path The full path and name of the dso.
	 * @param error Error code.
	 */
	void  (* const load)       (SLibrary *self, const char *path, s_erc *error);
} SLibraryClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Load a <i>dynamic shared object</i> from the given path into the
 * given #SLibrary object, initialize the plug-in, and run the plug-in
 * register function.
 * @public @memberof SLibrary
 *
 * @param self The given SLibrary object.
 * @param path The full path and name of the dso of the plug-in library.
 * @param error Error code.
 */
S_API void SLibraryLoad(SLibrary *self, const char *path, s_erc *error);


/**
 * Add the SLibrary class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_library_class_add(s_erc *error);


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

#endif /* _SPCT_LIBRARY_H__ */

