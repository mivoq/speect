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
/* SSerializedObjectClass implementation for Python object data                     */
/* types in EBML format.                                                            */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_SERIALIZED_EBML_PYTHON_OBJECT__
#define _SPCT_PLUGIN_SERIALIZED_EBML_PYTHON_OBJECT__


/**
 * @file serialized_python_object.h
 * SSerializedObjectClass implementation for Python object data
 * types in EBML format.
 */


/**
 * @ingroup SSerializedObject
 * @defgroup SEbmlPyObject Serialized EBML Python Objects
 * SSerializedObjectClass implementation for Python object data
 * types in EBML format.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "Python.h"
#include "speect.h"
#include "ebml.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_serialized_ebml_python_object_reg(s_erc *error);


/**
 * Free the plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_serialized_ebml_python_object_free(s_erc *error);


/**
 * Read an #SObject of type #SPyObject from an EBML format file.
 *
 * @param reader The ebml reader object.
 * @param object An already created #SPyObject object where
 * the data can be read into.
 * @param loads_function The Python pickle "loads" function.
 * @param error Error code.
 */
S_LOCAL void read_python_object(SEbmlRead *reader, SObject *object,
								PyObject *loads_function, s_erc *error);


/**
 * Write an #SObject of type #SPyObject to an EBML format file.
 *
 * @param writer The ebml writer object.
 * @param object The #SPyObject float object to write.
 * @param dumps_function The Python pickle "dumps" function.
 * @param error Error code.
 */
S_LOCAL void write_python_object(SEbmlWrite *writer, const SObject *object,
								 PyObject *dumps_function, s_erc *error);


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

#endif /* _SPCT_PLUGIN_SERIALIZED_EBML_PYTHON_OBJECT__ */

