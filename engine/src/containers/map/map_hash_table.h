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
/* Hash table implementation of SMap container.                                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_MAP_HASH_TABLE_H__
#define _SPCT_MAP_HASH_TABLE_H__


/**
 * @file map_hash_table.h
 * Hash table, map data container implementation.
 */


/**
 * @ingroup SMap
 * @defgroup SMapHashTable HashTable
 * Hash table, map data container implementation.
 * @note The SMapHashTable's iterator (#SMapIterator) can @b only
 * iterate in a forward direction.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/containers/hashtable/hash_table.h"
#include "containers/map/map.h"


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
 * Return the given parent/child class object of an #SMapHashTable type as an SListList object.
 * @param SELF The given object.
 * @return Given object as #SMapHashTable* type.
 * @note This casting is not safety checked.
 */
#define S_MAPHASHTABLE(SELF)    ((SMapHashTable *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* SMapHashTable definition                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * The SMapHashTable structure.
 * Inherits and implements #SMap as a hash table.
 * @extends SMap
 */
typedef struct
{
	/**
	 * @protected Inherit from #SMap.
	 */
	SMap          obj;

	/**
	 * @protected Hash table container for keys-values.
	 */
	s_hash_table *table;
} SMapHashTable;


/************************************************************************************/
/*                                                                                  */
/* SMapHashTableClass definition                                                    */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef for key-value hash table map container class struct. Same as #SMapClass as
 * we are not adding any new methods.
 */
typedef SMapClass SMapHashTableClass;


/************************************************************************************/
/*                                                                                  */
/*  Function prototypes                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize a SMapHashTable data container.
 * @public @memberof SMapHashTable
 *
 * @param self The SMapHashTable (as an #SMap) to initialize.
 * @param size Initial size of table. The initial size of the table must be
 * chosen with care as resizing a table is an expensive operation.
 * @param error Error code.
 *
 * @note The #SMapHashTable iterator does @b not implement
 * <ul>
 *  <li> #SIteratorClass::last (#SIteratorLast), and </li>
 *  <li> #SIteratorClass::prev (#SIteratorPrev). </li>
 * </ul>
 * @note If this function fails the hash table will be deleted and the @c
 * self pointer will be set to @c NULL.
 */
S_API void SMapHashTableInit(SMap **self, size_t size, s_erc *error);


/**
 * Add the SMapHashTable class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_map_hash_table_class_add(s_erc *error);


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

#endif /* _SPCT_MAP_HASH_TABLE_H__ */
