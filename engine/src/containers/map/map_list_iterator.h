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
/* DATE    : 8 October 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Iterator implementation of SMapList container.                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_MAP_LIST_ITERATOR_H__
#define _SPCT_MAP_LIST_ITERATOR_H__


/**
 * @file map_list_iterator.h
 * #SMapList iterator implementation.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
#include "base/errdbg/errdbg.h"
#include "base/containers/list/list.h"
#include "containers/iterator.h"
#include "containers/map/map_list.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* SMapListIterator definition                                                      */
/*                                                                                  */
/************************************************************************************/

/**
 * SMapListIterator definition.
 * @private
 *
 * The extra pointer, @c n_itr, is required so that we can unlink
 * elements from the container while also still being able to iterate
 * over the container.
 */
typedef struct SMapListIterator_s
{
	SIterator             obj;       /*!< Inherit from SIterator.               */
	const s_list_element *c_itr;     /*!< Pointer to current list element.      */
	const s_list_element *n_itr;     /*!< Pointer to next list element.         */
} SMapListIterator;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize an SMapListIterator with the given SMapList.
 * @private @memberof SIterator
 *
 * @param self The SMapListIterator to initialize.
 * @param map The list map of the given iterator.
 * @param error Error code.
 *
 * @note The iterator will be deleted and @c self set to @c NULL if:
 * <ul>
 *  <li> this function fails, or
 *  <li> the given map is empty.
 * </ul>
 */
S_LOCAL void SMapListIteratorInit(SMapListIterator **self, SMapList *map, s_erc *error);


/**
 * Add the SMapListIterator class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_map_list_iterator_class_add(s_erc *error);


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

#endif /* _SPCT_MAP_LIST_ITERATOR_H__ */
