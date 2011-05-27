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

#ifndef _SPCT_DSO_H__
#define _SPCT_DSO_H__


/**
 * @file dso.h
 * Dynamic Shared Object support.
 */


/**
 * @ingroup SPlugins
 * @defgroup SDSO Dynamic Shared Objects
 * Provides support for the use of <i>dynamic shared objects</i> (dso).
 * With SDso one can open/close a dso and get symbols from a dso. SDso will close
 * the associated dso when it is deleted with #S_DELETE. Implementation specific dynamic
 * linking is abstracted with @ref SDSOLoading.
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
 * Return the given #SDso child class object as a dso object.
 * @param SELF The given object.
 * @return Given object as #SDso* type.
 * @note This casting is not safety checked.
 */
#define S_DSO(SELF)    ((SDso *)(SELF))


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* SDso definition                                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * The SDso structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject  obj;

	/**
	 * @protected Handle to the OS dso object.
	 */
	void    *_handle;

	/**
	 * @protected Locking mutex
	 */
	S_DECLARE_MUTEX(dso_mutex);
} SDso;


/************************************************************************************/
/*                                                                                  */
/* SDsoClass definition                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * The SDsoClass structure.
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
	 * the given #SDso object.
	 * @param self The given SDso object.
	 * @param path The full path and name of the dso.
	 * @param error Error code.
	 */
	void  (*load)       (SDso *self, const char *path, s_erc *error);


	/**
	 * @protected GetSymbol function pointer.
	 * Get a handle to the named symbol from the given dynamic shared
	 * object.
	 * @param self The dso from which to get the symbol.
	 * @param name The name of the symbol to get.
	 * @param error Error code.
	 * @return Handle to the symbol.
	 */
	void *(*get_symbol) (const SDso *self, const char *name, s_erc *error);
} SDsoClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Load a <i>dynamic shared object</i> from the given path into the
 * given #SDso object.
 * @public @memberof SDso
 * @param self The given SDso object.
 * @param path The full path and name of the dso.
 * @param error Error code.
 */
S_API void SDsoLoad(SDso *self, const char *path, s_erc *error);


/**
 * Get a handle to the named symbol from the given dynamic shared
 * object.
 * @public @memberof SDso
 * @param self The dso from which to get the symbol.
 * @param name The name of the symbol to get.
 * @param error Error code.
 * @return Handle to the symbol.
 */
S_API void *SDsoGetSymbol(const SDso *self, const char *name, s_erc *error);


/**
 * Add the SDso class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_dso_class_add(s_erc *error);


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

#endif /* _SPCT_DSO_H__ */

