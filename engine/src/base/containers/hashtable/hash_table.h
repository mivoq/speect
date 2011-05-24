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
/* DATE    : 25 March 2008                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Hash table definition.                                                           */
/*                                                                                  */
/* Derived from the work by Bob Jenkins.                                            */
/* http://www.burtleburtle.net/bob/, May 2006, Public Domain.                       */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_HASH_TABLE_H__
#define _SPCT_HASH_TABLE_H__


/**
 * @file hash_table.h
 * Hash table container.
 */


/**
 * @ingroup SBaseContainers
 * @defgroup SHash Hash Table
 * A Hash Table definition and functions.
 * @{
 */


/**
 * @example hash_table_example.c
 * A simple hash table example.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
#include "base/errdbg/errdbg.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * @name Typedefs
 * @{
 */

/**
 * The s_hash_table element free function callback typedef.
 * A pointer to a function that frees the dynamically allocated
 * memory of a hash table element.
 *
 * @param key Hash table element key to free.
 * @param data Hash table element data to free.
 * @param error Error code.
 */
typedef void (*s_hash_table_free_fp)(void *key, void *data, s_erc *error);


/**
 * Opaque hash table element.
 * The hash table element represents one element in a hash table.
 */
typedef struct s_hash_element s_hash_element;


/**
 * Opaque hash table.
 * The hash table grows automatically, albeit the grow function is slow as elements
 * already in the hash table need to be rehashed. Therefore it is desirable to select
 * the hash table size as optimal as possible during creation with #s_hash_table_new.
 */
typedef struct s_hash_table s_hash_table;

/**
 * @}
 */

/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * @name Create/Delete
 * @{
 */


/**
 * Create a new hash table.
 * Creates a new hash table and initializes the size of the table to 2
 * <sup>size</sup>.
 *
 * @param free_func A memory free callback function. Used to free element data
 * from the hash table.
 * @param size The initial size of the table will be 2 <sup>size</sup>.
 * @param error Error code.
 *
 * @return The newly created hash table.
 */
S_API s_hash_table *s_hash_table_new(s_hash_table_free_fp free_func,
									 size_t size, s_erc *error);


/**
 * Delete a hash table.
 * Delete a hash table and all it's elements. If the
 * #s_hash_table_free_fp is #NULL then the elements memory @b wont
 * be freed.
 *
 * @param self The hash table.
 * @param error Error code.
 */
S_API void s_hash_table_delete(s_hash_table *self, s_erc *error);


/**
 * @}
 */


/**
 * @name Adding/Removing
 * @{
 */


/**
 * Add an element to a hash table. Hash table element keys must be
 * unique.
 *
 * @param self The hash table.
 * @param key The key of the element to add to the hash table.
 * @param key_length The length of the key.
 * @param data The data to associate with the key.
 * @param error Error code.
 *
 * @note The newly created hash table element takes hold of the memory
 * of the key.
 */
S_API void s_hash_table_add(s_hash_table *self, void *key, size_t key_length,
							void *data, s_erc *error);


/**
 * Unlink the given hash table element from the hash table.
 *
 * @param self The hash table element.
 * @param error Error code.
 *
 * @note The caller must take care of the memory of the key and data
 * of the given hash table element before unlinking, otherwise the
 * references to the key and data are lost as the element is freed
 * with this call.
 */
S_API void s_hash_element_unlink(s_hash_element *self, s_erc *error);


/**
 * Delete the given hash table element. If the #s_hash_table_free_fp
 * function pointer is #NULL then the memory of the key and data of
 * the element will not be freed.
 *
 * @param self The hash table element.
 * @param error Error code.
 */
S_API void s_hash_element_delete(s_hash_element *self, s_erc *error);


/**
 * @}
 */


/**
 * @name Size/Resize
 * @{
 */


/**
 * Get the number of elements in the hash table.
 *
 * @param self The hash table.
 * @param error Error code.
 *
 * @return Returns the number of elements of the hash table.
 */
S_API uint32 s_hash_table_size(const s_hash_table *self, s_erc *error);


/**
 * Resize the hash table to a specific size. The hash table can be
 * resized to any size as long as it can still contain all the
 * existing elements in it.  There are a few outcomes based on the
 * given size and the elements contained in the hash table:
 *
 * <ul>
 * <li> <tt> size = -1 </tt>, the hash table is resized to the
 * minimum possible size (power of 2) that will still contain all
 * elements.</li>
 * <li> <tt> size = 0 </tt>, nothing is done and the function
 * returns.</li>
 * <li> <tt> 2 <sup>size</sup> @b < number_of_elements </tt>, nothing is done and
 * the function returns.</li>
 * <li> <tt> 2 <sup>size</sup> = 2<sup>current_size</sup> </tt>, nothing is done and
 * the function returns.</li>
 * <li> In all other cases the hash table is resized to <tt> 2
 * <sup>size</sup> </tt>. </li></ul></p>
 *
 * @param self The hash table.
 * @param size Required size of the table (2 <sup>size</sup>).
 * @param error Error code.
 *
 * @note This function is relatively slow, as rehashing a table takes
 * time. However, if there are no elements in the list it is quite fast.
 */
S_API void s_hash_table_resize(s_hash_table *self, sint32 size,
							   s_erc *error);


/**
 * @}
 */


/**
 * @name Accessing
 * @{
 */


/**
 * Find a hash table element in the hash table.
 *
 * @param self The hash table.
 * @param key The key to look for.
 * @param key_length The length of the key in bytes.
 * @param error Error code.
 *
 * @return Pointer to the hash table element or #NULL if the given
 * key does not have an associated hash table element.
 */
S_API const s_hash_element *s_hash_table_find(const s_hash_table *self, const void *key,
											  size_t key_length, s_erc *error);


/**
 * Get the first hash table element of the given hash table.
 *
 * @param self The hash table.
 * @param error Error code.
 *
 * @return Pointer to the first hash table element, or #NULL if not
 * found or the hash table is empty.
 */
S_API const s_hash_element *s_hash_table_first(const s_hash_table *self, s_erc *error);


/**
 * Get the next hash table element relative to the given one.
 *
 * @param self The hash table element.
 * @param error Error code.
 *
 * @return Pointer to next hash table element relative to given one,
 * or #NULL if none.
 */
S_API const s_hash_element *s_hash_element_next(const s_hash_element *self, s_erc *error);


/**
 * @}
 */


/**
 * @name Element key/data
 * @{
 */


/**
 * Get the key of the given hash table element.
 *
 * @param self The hash table element.
 * @param error Error code.
 *
 * @return The key of the given hash table element.
 */
S_API const void *s_hash_element_key(const s_hash_element *self, s_erc *error);


/**
 * Get the key length (bytes) of the given hash table element.
 *
 * @param self The hash table element.
 * @param error Error code.
 *
 * @return The key length (bytes) of the given hash table element.
 */
S_API size_t s_hash_element_key_length(const s_hash_element *self, s_erc *error);


/**
 * Get the data of the given hash table element.
 *
 * @param self The hash table element.
 * @param error Error code.
 *
 * @return The data of the given hash table element.
 */
S_API const void *s_hash_element_get_data(const s_hash_element *self, s_erc *error);


/**
 * Set the data of the given hash table element.
 *
 * @param self The hash table element.
 * @param data The data to set the given hash element data to.
 * @param error Error code.
 *
 * @note If the hash element has some data it will be lost.
 */
S_API void s_hash_element_set_data(s_hash_element *self, void *data, s_erc *error);


/**
 * @}
 */


/**
 * @name Misc
 * @{
 */


/**
 * Print statistics about the hash table to a buffer that is returned.
 *
 * @param self The hash table.
 * @param error Error code.
 *
 * @return Character buffer with Hash Table stats in.
 *
 * @note Prints out information as follows:
 * @verbatim
 s_hash_table_stats:
 items 0: <number of buckets with 0 items> buckets
 items 1: <number of buckets with 1 item> buckets
 ...

 buckets: <number of buckets> items: <number of items> existing: <x>
 @endverbatim
 * ( @a x is the average length of the list when you look for an
 * item that exists.  When the item does not exists, the average
 * length is (number of items)/(number of buckets).)
 * @note The caller is responsible for the memory of the returned
 * buffer.
 *
 */
S_API char *s_hash_table_stats(const s_hash_table *self, s_erc *error);


/**
 * Get the position of the given hash table element in the hash table
 * array.
 *
 * @param self The hash table element.
 * @param error Error code.
 *
 * @return The position of the given hash table element in the hash
 * table array.
 */
S_API uint32 s_hash_element_pos(const s_hash_element *self, s_erc *error);


/**
 * @}
 */


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

#endif /* _SPCT_HASH_TABLE_H__ */

