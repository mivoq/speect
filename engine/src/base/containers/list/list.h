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
/* DATE    : 25 March 2008                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Doubly linked list data structure.                                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_BASE_CONTAINERS_LIST_H__
#define _SPCT_BASE_CONTAINERS_LIST_H__


/**
 * @file src/base/containers/list/list.h
 * Doubly linked list.
 */


/**
 * @ingroup SBaseContainers
 * @defgroup SDllist Doubly Linked List
 * A doubly linked list definition and functions.
 * @{
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
 * The s_list element comparison function typedef.
 * A pointer to a function that compares 2 elements
 * of a list.
 *
 * @param s_list_compare_fp A pointer to the comparison function.
 * @param le1 List element 1.
 * @param le2 List element 2.
 * @param error Error code.
 *
 * @return #TRUE if elements are equal else #FALSE.
 */
typedef s_bool (*s_list_compare_fp)(const void *le1, const void *le2, s_erc *error);


/**
 * The s_list element free function typedef.
 * A pointer to a function that frees the dynamically allocated
 * memory of a list element.
 *
 * @param s_list_free_fp A pointer to the function that frees the memory.
 * @param le List element to free.
 * @param error Error code.
 */
typedef void (*s_list_free_fp)(void *le, s_erc *error);


/**
 * Opaque definition of a list element.
 * The list element represents one element in the doubly linked list.
 */
typedef struct s_list_element s_list_element;


/**
 * Opaque definition of a doubly linked list.
 * The doubly linked list is headed by a pair of pointers, one to the head of the
 * list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before or
 * after an existing element, at the head of the list, or at the end of
 * the list. A doubly linked list may be traversed in either direction.
 */
typedef struct s_list s_list;


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
 * Create a new list.
 * Creates a new list and sets the comparison and free functions. The
 * comparison function must be of the type #s_list_compare_fp while
 * the free function must be of the type #s_list_free_fp. If the
 * comparison function is @c NULL then #s_list_find will do nothing.
 * If the free function is @c NULL then the #s_list_element_delete and
 * #s_list_delete functions will do nothing.
 *
 * @param compare_func A comparison function. Used to find an element in the list.
 * @param free_func A memory free function. Used to free element data from the list.
 * @param error Error code.
 *
 * @return Pointer to a list data structure.
 */
S_API s_list *s_list_new(s_list_compare_fp compare_func, s_list_free_fp free_func,
						 s_erc *error);


/**
 * Delete a list.
 * Delete a list and all its elements. If the #s_list_free_fp is NULL
 * then the elements memory @b wont be freed.
 *
 * @param self The list to delete.
 * @param error Error code.
 */
S_API void s_list_delete(s_list *self, s_erc *error);


/**
 * @}
 */


/**
 * @private
 * Find the element in the list which matches the comparison with the
 * comparison function #s_list_compare_fp. If the comparison function
 * is null then nothing is done.
 *
 * @param self The list.
 * @param f Start searching from this element (including) onwards. If
 * @c NULL then start from first element.
 * @param data The data to match.
 * @param index A variable to hold the index of the element found
 * which matches the data. -1 if not found. Set to @c NULL if not required.
 * @param error Error code.
 *
 * @return Pointer to the list element which matches the data,
 * else @c NULL.
 *
 * @note Lists are indexed starting from 0.
 * @note The index variable will be relative if f is @b not @c NULL.
 */
S_API s_list_element *s_list_find_index(const s_list *self, const s_list_element *f,
										const void *data, int *index, s_erc *error);


/**
 * @name Accessing
 * @{
 */


/**
 * Return the first element in the list.
 *
 * @param self The list.
 * @param error Error code.
 *
 * @return Pointer to the first element in the list.
 */
S_API const s_list_element *s_list_first(const s_list *self, s_erc *error);


/**
 * Return the last element in the list.
 *
 * @param self The list.
 * @param error Error code.
 *
 * @return Pointer to the last element in the list.
 */
S_API const s_list_element *s_list_last(const s_list *self, s_erc *error);


/**
 * @hideinitializer
 * Find the index into the list of the given list element. If the list
 * comparison function #s_list_compare_fp is @c NULL then nothing is
 * done.
 *
 * @param self The list.
 * @param data The data to match.
 * @param index A variable to hold the index of the element found
 * which matches the data. -1 if not found.
 * @param error Error code.
 *
 * @return Pointer to the list element which matches the data,
 * else @c NULL.
 *
 * @note Lists are indexed starting from 0.
 */
#define s_list_index(self, data, index, error) \
	s_list_find_index(self, NULL, data, index, error)


/**
 * Find the nth element in the list.
 *
 * @param self The list.
 * @param n Index of element to find.
 * @param error Error code.
 *
 * @return Pointer to the nth list element or @c NULL if index
 * is out of bounds.
 *
 * @note The list elements are indexed from 0.
 */
S_API const s_list_element *s_list_nth(const s_list *self, uint32 n,
									   s_erc *error);


/**
 * @}
 */


/**
 * @name Element functions
 * @{
 */


/**
 * Get the list element data.
 *
 * @param self The list element.
 * @param error Error code.
 *
 * @return Pointer to the list element data.
 */
S_API const void *s_list_element_get(const s_list_element *self, s_erc *error);


/**
 * Replace the list element data, does not delete old data.
 *
 * @param self The list element who's data is to be replaced.
 * @param data The new list element data.
 * @param error Error code.
 *
 * @return Pointer to the replaced list element @a data.
 *
 * @note The list element takes ownership of the new data.
 */
S_API void *s_list_element_replace(s_list_element *self, void *data,
								   s_erc *error);


/**
 * Unlink list element from list. Does not delete data of element.
 *
 * @param self The list element to be unlinked.
 * @param error Error code.
 *
 * @return Pointer to the unlinked list element @a data.
 *
 * @note The caller is responsible for the returned data's memory.
 */
S_API void *s_list_element_unlink(s_list_element *self, s_erc *error);


/**
 * Remove the list element from it's parent list and delete the list
 * element data.
 *
 * @param self The list element to be deleted.
 * @param error Error code.
 *
 * @return Pointer to the previous list element or @c NULL if none.
 *
 * @note If #s_list_free_fp is not defined then nothing is done
 * and @c NULL returned.
 */
S_API s_list_element *s_list_element_delete(s_list_element *self,
											s_erc *error);


/**
 * Return the next element in the list relative to the given one.
 *
 * @param self Pointer to current position in list.
 * @param error Error code.
 *
 * @return Pointer to the next element in the list.
 */
S_API const s_list_element *s_list_element_next(const s_list_element *self,
												s_erc *error);


/**
 * Return the previous element in the list relative to the given one.
 *
 * @param self Pointer to current position in list.
 * @param error Error code.
 *
 * @return Pointer to the previous element in the list.
 */
S_API const s_list_element *s_list_element_prev(const s_list_element *self,
												s_erc *error);


/**
 * @hideinitializer
 * Find the list element which matches the given data with the
 * comparison function #s_list_compare_fp. If the comparison function
 * is @c NULL then nothing is done.
 *
 * @param self The list.
 * @param data The data to match.
 * @param error Error code.
 *
 * @return Pointer to the list element which matches the data, else @c
 * NULL.
 *
 * @note Returns the first match.
 */
#define s_list_find(self, data, error) \
	s_list_find_index(self, NULL, data, NULL, error)


/**
 * @}
 */


/**
 * @name Query
 * @{
 */


/**
 * Query if the list has any elements.
 *
 * @param self The list to query.
 * @param error Error code.
 *
 * @return #TRUE or #FALSE.
 */
S_API s_bool s_list_isempty(const s_list *self, s_erc *error);


/**
 * Get the number of elements in the list.
 *
 * @param self The list to query.
 * @param error Error code.
 *
 * @return The number of elements in the list.
 */
S_API uint32 s_list_size(const s_list *self, s_erc *error);


/**
 * @}
 */


/**
 * @name Queue
 * @{
 */


/**
 * Push data onto end of list. Same as #s_list_append.
 *
 * @param self The list.
 * @param error Error code.
 * @param data The data to append.
 *
 * @note The list takes ownership of the data.
 */
S_API void s_list_push(s_list *self, void *data, s_erc *error);


/**
 * Pop last element of list. Removes last element from
 * list and returns the element data.
 *
 * @param self The list.
 * @param error Error code.
 *
 * @return Pointer to popped element data.
 *
 * @note The caller is responsible for the memory of the returned
 * data.
 */
S_API void *s_list_pop(s_list *self, s_erc *error);


/**
 * Reverse the elements in the list.
 *
 * @param self The list to reverse.
 * @param error Error code.
 */
S_API void s_list_reverse(s_list *self, s_erc *error);


/**
 * @}
 */


/**
 * @name Insertion
 * @{
 */


/**
 * Prepend data to beginning of the list.
 *
 * @param self The list.
 * @param error Error code.
 * @param data The data to prepend.
 *
 * @note The list takes ownership of the data.
 */
S_API void s_list_prepend(s_list *self, void *data, s_erc *error);


/**
 * Append data to end of the list.
 *
 * @param self The list.
 * @param error Error code.
 * @param data The data to append.
 *
 * @note The list takes ownership of the data.
 */
S_API void s_list_append(s_list *self, void *data, s_erc *error);


/**
 * Insert data before given list element. Return reference to inserted
 * data element.
 *
 * @param self The element before which data must be inserted.
 * @param data The data to be inserted.
 * @param error Error code.
 *
 * @return Pointer to the inserted list element.
 *
 * @note The list takes ownership of the data.
 */
S_API const s_list_element *s_list_insert_before(s_list_element *self, void *data, s_erc *error);


/**
 * Insert data after given list element. Return reference to inserted
 * data element.
 *
 * @param self The element after which data must be inserted.
 * @param data The data to be inserted.
 * @param error Error code.
 *
 * @return Pointer to the inserted list element.
 *
 * @note The list takes ownership of the data.
 */
S_API const s_list_element *s_list_insert_after(s_list_element *self, void *data, s_erc *error);


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

#endif /* _SPCT_BASE_CONTAINERS_LIST_H__ */

