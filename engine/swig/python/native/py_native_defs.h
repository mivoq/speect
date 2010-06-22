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
/* Python Native type definitions.                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PYTHON_NATIVE_DEFS_H__
#define _SPCT_PYTHON_NATIVE_DEFS_H__


/**
 * @file py_native_defs.h
 * Python Native type definitions.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "Python.h"
#include "speect.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* SPyObject definition                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * The Speect Python obect structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject   obj;

	/**
	 * @protected The Python object.
	 */
	PyObject *pobject;
} SPyObject;


/************************************************************************************/
/*                                                                                  */
/* SListPy definition                                                               */
/*                                                                                  */
/************************************************************************************/

/**
 * The SListPy structure.
 * Inherits and implements #SList wrappers for Python lists.
 * @extends SList
 */
typedef struct
{
	/**
	 * @protected Inherit from #SList.
	 */
	SList     obj;

	/**
	 * @protected Python object interface.
	 */
	SPyObject *pyObject;
} SListPy;


/************************************************************************************/
/*                                                                                  */
/* SListPyIterator definition                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * SListPyIterator definition.
 * @private
 *
 * The extra pointer, @c n_itr, is required so that we can unlink
 * elements from the container while also still being able to iterate
 * over the container.
 */
typedef struct
{
	SIterator   obj;       /*!< Inherit from SIterator.               */
	PyObject   *iterator;  /*!< Python iterator object.				  */
	PyObject   *c_itr;     /*!< Pointer to current list element.      */
	PyObject   *n_itr;     /*!< Pointer to next list element.         */
	SObject    *sobject;   /*!< Pointer to Object method's tmp value. */
} SListPyIterator;


/************************************************************************************/
/*                                                                                  */
/* SMapPy definition                                                                */
/*                                                                                  */
/************************************************************************************/

/**
 * The SMapPy structure.
 * Inherits and implements #SMap wrappers for Python dictionaries.
 * @extends SMap
 */
typedef struct
{
	/**
	 * @protected Inherit from #SMap.
	 */
	SMap       obj;

	/**
	 * @protected Python object interface.
	 */
	SPyObject *pyObject;

	/**
	 * @protected Temporary object for @c const calls.
	 */
	SObject   *tmp;
} SMapPy;


/************************************************************************************/
/*                                                                                  */
/* SMapPyIterator definition                                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * SMapPyIterator definition.
 * @private
 *
 * The extra pointer, @c n_itr, is required so that we can unlink
 * elements from the container while also still being able to iterate
 * over the container.
 */
typedef struct
{
	SIterator   obj;       /*!< Inherit from SIterator.               */
	PyObject   *iterator;  /*!< Python iterator object.				  */
	PyObject   *c_itr;     /*!< Pointer to current map element.       */
	PyObject   *n_itr;     /*!< Pointer to next map element.          */
	SObject    *sobject;   /*!< Pointer to Object method's tmp value. */
	char       *key;       /*!< Pointer to Key method's tmp value.    */
} SMapPyIterator;


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

#endif /* _SPCT_PYTHON_NATIVE_DEFS_H__ */
