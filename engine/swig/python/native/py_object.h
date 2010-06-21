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
/* DATE    : April 2010                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Python object interface for Speect.                                            */
/* Note that this object is part of the SWIG/Python interface and not               */
/* the Speect Engine itself.                                                        */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PY_OBJECT_H__
#define _SPCT_PY_OBJECT_H__


/**
 * @file py_object.h
 * A Python object interface for Speect.
 */


/**
 * @defgroup SPyObject Speect PyObject
 * A Python object interface for Speect.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

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
 * Return the given #SPyObject child/parent class object as a
 * #SPyObject object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SPyObject* type.
 * @note This casting is not safety checked.
 */
#define S_PYOBJECT(SELF)    ((SPyObject *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* SPyObject definition (in py_native_defs.h)                                       */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* SPyObjectClass definition                                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * The Speect Python object class structure, does not add any methods
 * to #SObjectClass, and therefore is exactly the same.
 */
typedef SObjectClass SPyObjectClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Set a Python object in the given SPyObject. The given SPyObject's
 * Python object's reference will be decremented (@c Py_XDECREF) if it
 * is not @c NULL. The new Python object's reference will be
 * incremented (@c Py_XINCREF).
 * @public @memberof SPyObject
 *
 * @param self The given SPyObject in which to set the Python object.
 * @param object The Python object.
 * @param error Error code.
 */
S_API void SPyObjectSet(SPyObject *self, PyObject *object, s_erc *error);


/**
 * Get the given SPyObject's Python object. If it is not set yet (it's
 * @c NULL) a @c Py_None will be returned.
 * @public @memberof SPyObject
 *
 * @param self The given SPyObject from which to get the Python object.
 * @param error Error code.
 *
 * @return <tt> New reference </tt>
 */
S_API PyObject *SPyObjectGet(const SPyObject *self, s_erc *error);


/**
 * Register the SPyObject with the object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_pyobject_class_reg(s_erc *error);


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

#endif /* _SPCT_PY_OBJECT_H__ */
