/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* DATE    : 14 October 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Iterator implementation for SListList container.                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LIST_LIST_ITERATOR_H__
#define _SPCT_LIST_LIST_ITERATOR_H__


/**
 * @file list_list_iterator.h
 * #SListList iterator implementation.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
#include "containers/iterator.h"
#include "base/containers/list/list.h"
#include "containers/list/list_list.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* SListListIterator definition                                                     */
/*                                                                                  */
/************************************************************************************/

/**
 * SListListIterator definition.
 * @private
 *
 * The extra pointer, @c n_itr, is required so that we can unlink
 * elements from the container while also still being able to iterate
 * over the container.
 */
typedef struct
{
	SIterator             obj;       /*!< Inherit from SIterator.               */
	const s_list_element *c_itr;     /*!< Pointer to current list element.      */
	const s_list_element *n_itr;     /*!< Pointer to next list element.         */
} SListListIterator;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize an SListListIterator with the given SListList.
 * @private @memberof SIterator
 *
 * @param self The @c SListListIterator to initialize.
 * @param list The #SListList of the given iterator.
 * @param error Error code.
 *
 * @note The iterator will be deleted and @c self set to @c NULL if:
 * <ul>
 *  <li> this function fails, or
 *  <li> the given list is empty.
 * </ul>
 */
S_LOCAL void SListListIteratorInit(SListListIterator **self,
								   SListList *list, s_erc *error);


/**
 * Add the SListListIterator class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_list_list_iterator_class_add(s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


#endif /* _SPCT_LIST_LIST_ITERATOR_H__ */
