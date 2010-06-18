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
/* Iterator implementation for SListPy container.                                   */
/* Uses the Python iteration protocol.                                              */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LIST_PY_ITERATOR_H__
#define _SPCT_LIST_PY_ITERATOR_H__


/**
 * @file list_list_iterator.h
 * #SListPy iterator implementation.
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
/* SListPyIterator definition                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * SListPyIterator definition.
 * @private
 */
typedef struct
{
	SIterator   obj;       /*!< Inherit from SIterator.               */
	PyObject   *iterator;  /*!< Python iterator object.               */
} SListPyIterator;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize an SListPyIterator with the given SListPy.
 * @private @memberof SIterator
 *
 * @param self The SListPyIterator (as an #SIterator) to initialize.
 * @param list The Python list of the given iterator.
 * @param error Error code.
 *
 * @note If this function fails the iterator will be deleted and the @c
 * self pointer will be set to @c NULL.
 */
S_LOCAL void SListPyIteratorInit(SIterator **self, SListPy *list, s_erc *error);


/**
 * Register the SListPyIterator class with the object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_list_py_iterator_class_reg(s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


#endif /* _SPCT_LIST_PY_ITERATOR_H__ */
