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
/* DATE    : 12 October 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* List implementation of SList container.                                          */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LIST_LIST_H__
#define _SPCT_LIST_LIST_H__


/**
 * @file list_list.h
 * Doubly linked list, list data container implementation.
 */


/**
 * @ingroup SList
 * @defgroup SListList Doubly Linked List
 * Doubly linked list, list data container implementation.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/containers/list/list.h"
#include "containers/list/list.h"


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
 * Return the given parent/child class object of an #SListList type as
 * an SListList object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SListList* type.
 *
 * @note This casting is not safety checked.
 */
#define S_LISTLIST(SELF)    ((SListList *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* SListList definition                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * The SListList structure.
 * Inherits and implements #SList as a doubly linked list.
 * @extends SList
 */
typedef struct
{
	/**
	 * @protected Inherit from #SList.
	 */
	SList     obj;

	/**
	 * @protected Doubly linked list container for values.
	 */
	s_list   *list;
} SListList;



/************************************************************************************/
/*                                                                                  */
/* SListListClass definition                                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef for value-list list container class struct. Same as #SListClass as
 * we are not adding any new methods.
 */
typedef SListClass SListListClass;


/************************************************************************************/
/*                                                                                  */
/*  Function prototypes                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * Add the #SListList class to the object system.
 * @private @memberof SListList
 * @param error Error code.
 */
S_LOCAL void _s_list_list_class_add(s_erc *error);


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

#endif /* _SPCT_LIST_LIST_H__ */
