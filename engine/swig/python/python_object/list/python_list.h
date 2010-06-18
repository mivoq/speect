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
/* SList wrappers for Python Lists.                                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LIST_PY_H__
#define _SPCT_LIST_PY_H__


/**
 * @file python_list.h
 * SList wrappers for Python Lists.
 */


/**
 * @ingroup SPyObject
 * @defgroup SListPy Python List
 * SList wrappers for Python Lists.
 * @{
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
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given parent/child class object of an #SListPy type as
 * an SListPy object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SListPy* type.
 *
 * @note This casting is not safety checked.
 */
#define S_LISTPY(SELF)    ((SListPy *)(SELF))


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
/* SListPyClass definition                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef for Python lists wrappers class struct. Same as #SListClass as
 * we are not adding any new methods.
 */
typedef SListClass SListPyClass;


/************************************************************************************/
/*                                                                                  */
/*  Function prototypes                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize an #SListPy data container.
 * @public @memberof SListPy
 *
 * @param self The SListPy (as an #SList) to initialize.
 * @param error Error code.
 *
 * @return Pointer to newly initialized Python list container.
 *
 * @note If this function fails the list will be deleted and the @c
 * self pointer will be set to @c NULL.
 */
S_API void SListPyInit(SList **self, s_erc *error);


/**
 * Register the #SListPy class to the object system.
 * @private @memberof SListPy
 *
 * @param error Error code.
 */
S_LOCAL void _s_list_py_class_reg(s_erc *error);


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

#endif /* _SPCT_LIST_PY_H__ */
