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
 * Hash table, map data container implementation. When a new
 * SMapHashTable is created, @c S_NEW("SMapHashTable"), it's size is
 * initialized to 128. Resizing the hash table (#SMapHashTableResize)
 * @e before any objects are added is a comparatively cheap operation
 * compared to growing the table.
 *
 * @note The SMapHashTable's iterator (#SMapIterator) can @b only
 * iterate in a forward direction and does @b not implement
 * <ul>
 *  <li> #SIteratorClass::last (#SIteratorLast), and </li>
 *  <li> #SIteratorClass::prev (#SIteratorPrev). </li>
 * </ul>
 *
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
 * Resize a SMapHashTable's hash table.
 * Resize the hash table to a specific size. When a new SMapHashTable
 * is created with @c S_NEW("SMapHashTable"), the hash table's size is
 * set to contain 128 elements. This function is useful if a rough
 * estimate of the number of objects that will be contained
 * in the map is known beforehand, as it can save some
 * memory, and growing a table is an relatively expensive
 * operation. Resizing a SMapHashTable @e before any objects are added
 * is comparatively cheap.
 *
 *
 * The hash table can be resized to any size as long as it can still
 * contain all the existing objects in it. There are a few outcomes
 * based on the given size and the elements contained in the hash table:
 *
 * <ul>
 * <li> <tt> size = -1 </tt>, the hash table is resized to the
 * minimum possible size (power of 2) that will still contain all
 * elements.</li>
 * <li> <tt> size = 0 </tt>, nothing is done and the function
 * returns.</li>
 * <li> <tt> size <= number_of_objects </tt>, nothing is done and
 * the function returns.</li>
 * <li> In all other cases the hash table is resized to closest power
 * of 2 bigger than <tt>size</tt>.
 </li></ul></p>
 *
 * @public @memberof SMapHashTable
 *
 * @param self The SMapHashTable to resize.
 * @param size The new size of table.
 * @param error Error code.
 */
S_API void SMapHashTableResize(SMapHashTable *self, sint32 size, s_erc *error);


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
