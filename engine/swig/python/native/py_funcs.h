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
/* Miscellaneous Python object functions.                                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PY_FUNCS_H__
#define _SPCT_PY_FUNCS_H__


/**
 * @file py_funcs.h
 * Miscellaneous Python object functions.
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
/*  Function prototypes                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize the SWIG object types, must be called before any other
 * functions in this module (called by #s_python_native_objects_init).
 *
 * @param error Error code.
 */
S_LOCAL void _s_py_funcs_initialize(s_erc *error);


/**
 * Convert a Speect SObject to a Python PyObject type.
 *
 * @param object The object to convert.
 * @param own Ownership flag. If #TRUE then Python takes ownership of
 * the object, otherwise, if #FALSE Speect holds ownership and must
 * delete it later.
 * @param error Error code.
 *
 * @return A Python PyObject object wrapper for the Speect SObject
 * object (new reference).
 */
S_API PyObject *s_sobject_2_pyobject(const SObject *object, s_bool own,
									 s_erc *error);


/**
 * Convert a Python PyObject to a Speect SObject type.
 *
 * @param pobject The object to convert.
 * @param error Error code.
 *
 * @return A Speect SObject object wrapper for the Python PyObject
 * object.
 *
 * @note The caller is responsible for the memory of the returned
 * Speect SObject.
 * @note This function does @b not take hold the Python object
 * reference.
 */
S_API SObject *s_pyobject_2_sobject(PyObject *pobject, s_erc *error);


/**
 * Get a @e C string of the given Python string/unicode object.
 *
 * @param pobject Given Python string/unicode object.
 * @param error Error code.
 *
 * @return A null terminated @e C string (utf8 encoding).
 *
 * @note The caller is responsible for the memory of the
 * returned @e C string.
 * @note This function does @b not take hold the Python object
 * reference.
 */
S_API char *s_get_pyobject_str(PyObject *pobject, s_erc *error);


/**
 * Create a new Python unicode object from a @e C string.
 *
 * @param cstr A null terminated @e C string (utf8 encoding).
 * @param error Error code.
 *
 * @return New reference Python unicode object (dependent on
 * Python version).
 * @note This function always returns a unicode object.
  */
S_API PyObject *s_set_pyobject_str(const char *cstr, s_erc *error);


/**
 * Check if Python set an error (@c PyErr_Occurred()) and return the
 * error string as a null terminated @e C string (utf8 encoding). If
 * no error occured @c NULL is returned.
 *
 * @return A null terminated @e C string (utf8 encoding) or @c NULL.
 *
 * @note The caller is responsible for the memory of the
 * returned @e C string.
 */
S_API char *s_get_python_error_str(void);


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

#endif /* _SPCT_PY_FUNCS_H__ */
