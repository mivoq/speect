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
/* DATE    : 8 October 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Iterator implementation of SMapHashTable container.                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_MAP_HASH_TABLE_ITERATOR_H__
#define _SPCT_MAP_HASH_TABLE_ITERATOR_H__


/**
 * @file map_hash_table_iterator.h
 * #SMapHashTable iterator implementation.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
#include "containers/iterator.h"
#include "base/containers/hashtable/hash_table.h"
#include "containers/map/map_hash_table.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* SMapHashTableIterator definition                                                 */
/*                                                                                  */
/************************************************************************************/

/**
 * SMapHashTableIterator definition.
 * @private
 *
 * The extra pointer, @c n_itr, is required so that we can unlink
 * elements from the container while also still being able to iterate
 * over the container.
 */
typedef struct
{
	SIterator             obj;     /*!< Inherit from SIterator.                       */
	const s_hash_element *c_itr;   /*!< Pointer to current element in the hash table. */
	const s_hash_element *n_itr;   /*!< Pointer to next element in the hash table.    */
} SMapHashTableIterator;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize an SMapHashTableIterator with the given SMapHashTable.
 * @private @memberof SIterator
 *
 * @param self The SMapHashTableIterator to initialize.
 * @param map The hash table map of the given iterator.
 * @param error Error code.
 *
 * @note The iterator will be deleted and @c self set to @c NULL if:
 * <ul>
 *  <li> this function fails, or
 *  <li> the given map is empty.
 * </ul>
 */
S_LOCAL void SMapHashTableIteratorInit(SMapHashTableIterator **self, SMapHashTable *map,
									   s_erc *error);


/**
 * Add the SMapHashTableIterator class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_map_hash_table_iterator_class_add(s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


#endif /* _SPCT_MAP_HASH_TABLE_ITERATOR_H__ */
