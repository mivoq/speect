/************************************************************************************/
/* Copyright (c) 2010 The Department of Arts and Culture,                           */
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
/* DATE    : June 2010                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SMap wrappers for Python dictionaries.                                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PY_MAP_H__
#define _SPCT_PY_MAP_H__


/**
 * @file py_map.h
 * SMap wrappers for Python dictionaries.
 */


/**
 * @ingroup SPyObject
 * @defgroup SMapPy Python Dictionary
 * SMap wrapper for Python dictionaries.
 * SMapPy must be initialized with the Python dictionary object with
 * #SMapPyInit.
 * @note SMapPy does not implement the following SMap methods:
 * <ul>
 * <li> #SMapCopy </li>
 * </ul>
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "py_object.h"
#include "py_native.h"


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
 * @hideinitializer
 * Return the given parent/child class object of an #SMapPy
 * type as an SMapPy object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SMapPy* type.
 *
 * @note This casting is not safety checked.
 */
#define S_MAPPY(SELF)    ((SMapPy *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* SMapPy definition (in py_native_defs.h)                                          */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* SMapHashTableClass definition                                                    */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef for Python dictionaries wrappers class struct. Same as #SMapClass as
 * we are not adding any new methods.
 */
typedef SMapClass SMapPyClass;


/************************************************************************************/
/*                                                                                  */
/*  Function prototypes                                                             */
/*                                                                                  */
/************************************************************************************/


/**
 * Initialize an #SMapPy data container.
 * @public @memberof SMapPy
 *
 * @param self The SMapPy to initialize.
 * @param object The Python dictionary to which this SMap is a wrapper.
 * @param error Error code.
 *
 * @return Pointer to newly initialized Python dictionary container.
 *
 * @note If this function fails the list will be deleted and the @c
 * self pointer will be set to @c NULL.
 */
S_API void SMapPyInit(SMapPy **self, PyObject *object, s_erc *error);


/**
 * Register the #SMapPy class to the object system.
 * @private @memberof SListPy
 *
 * @param error Error code.
 */
S_LOCAL void _s_map_py_class_reg(s_erc *error);


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

#endif /* _SPCT_PY_MAP_H__ */
