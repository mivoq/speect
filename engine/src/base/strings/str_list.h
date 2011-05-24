/************************************************************************************/
/* Copyright (c) 2011 The Department of Arts and Culture,                           */
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
/* DATE    : 12 May 2011                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* String lists                                                                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_STRING_UTF8_STRING_LIST_H__
#define _SPCT_STRING_UTF8_STRING_LIST_H__


/**
 * @file str_list.h
 * UTF8 string lists.
 */


/**
 * @ingroup SBaseStrings
 * @defgroup SUtf8StringLists String List
 * Provides s utf-8 aware string list.
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
#include "base/containers/list/list.h"


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
 * Opaque definition of a string list, same as an #s_list.
 */
typedef s_list s_str_list;


/**
 * Opaque definition of a string list element, same as an
 * #s_list_element. The list element represents one element in the
 * string list.
 */
typedef s_list_element s_str_list_element;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Create a new string list.
 *
 * @param error Error code.
 *
 * @return Pointer to the newly created string list.
 */
S_API s_str_list *s_str_list_new(s_erc *error);


/**
 * Create a new string list from sub-strings of @c string divided
 * by @c split_string.
 *
 * @param string The string to split into sub-strings.
 * @param split_string The string to split @c string with.
 * @param error Error code.
 *
 * @return Pointer to the newly created string list.
 */
S_API s_str_list *s_str_list_split(const char *string, const char *split_string,
								   s_erc *error);


/**
 * Delete a string list.
 * Deletes the list and frees up all the strings contained in the
 * list.
 *
 * @param self The string list to delete.
 * @param error Error code.
 */
S_API void s_str_list_delete(s_str_list *self, s_erc *error);


/**
 * @private
 * Find the string in the list which matches the given string.
 *
 * @param self The string list.
 * @param f Start searching from this string element (including) onwards. If
 * @c NULL then start from first string element.
 * @param string The string to match.
 * @param index A variable to hold the index of the string element found
 * which matches the data. -1 if not found. Set to #NULL if not required.
 * @param error Error code.
 *
 * @return Pointer to the string list element which matches the data,
 * else @c NULL.
 *
 * @note String lists are indexed starting from 0.
 * @note The index variable will be relative if f is @b not #NULL.
 */
S_API const s_str_list_element *s_str_list_find_index(const s_str_list *self,
													  const s_str_list_element *f,
													  const char *string,
													  int *index,
													  s_erc *error);


/**
 * Return the first string element in the list.
 *
 * @param self The string list.
 * @param error Error code.
 *
 * @return Pointer to the first string element in the list.
 */
S_API const s_str_list_element *s_str_list_first(const s_str_list *self,
												 s_erc *error);


/**
 * Return the last string element in the list.
 *
 * @param self The string list.
 * @param error Error code.
 *
 * @return Pointer to the last string element in the list.
 */
S_API const s_str_list_element *s_str_list_last(const s_str_list *self,
												s_erc *error);


/**
 * @hideinitializer
 * Find the index into the string list of the given string list
 * element.
 *
 * @param self The string list.
 * @param string The string to match.
 * @param index A variable to hold the index of the string element
 * found which matches the data. -1 if not found.
 * @param error Error code.
 *
 * @return Pointer to the string list element which matches the data,
 * else #NULL.
 *
 * @note String lists are indexed starting from 0.
 */
#define s_str_list_index(self, string, index, error)	\
	s_str_list_find_index(self, NULL, string, index, error)


/**
 * Find the nth string element in the list.
 *
 * @param self The string list.
 * @param n Index of string element to find.
 * @param error Error code.
 *
 * @return Pointer to the nth string list element or #NULL if index
 * is out of bounds.
 *
 * @note The string list elements are indexed from 0.
 */
S_API const s_str_list_element *s_str_list_nth(const s_str_list *self,
											   uint32 n,
											   s_erc *error);


/**
 * Find the nth string in the list.
 *
 * @param self The string list.
 * @param n Index of string to find.
 * @param error Error code.
 *
 * @return Pointer to the nth string or #NULL if index
 * is out of bounds.
 *
 * @note The string list elements are indexed from 0.
 */
S_API const char *s_str_list_nth_string(const s_str_list *self,
										uint32 n,
										s_erc *error);


/**
 * Get the string list element's string.
 *
 * @param self The string list element.
 * @param error Error code.
 *
 * @return Pointer to the string list element's string.
 */
S_API const char *s_str_list_element_get(const s_str_list_element *self,
										 s_erc *error);


/**
 * Replace the string list element's string. The replaced string is
 * freed.
 *
 * @param self The string list element who's string is to be replaced.
 * @param string The new string list element string.
 * @param error Error code.
 *
 * @note Internally a duplicate of @c string is made and ownership
 * taken thereof.
 */
S_API void s_str_list_element_replace(s_str_list_element *self,
									  const char *string, s_erc *error);


/**
 * Unlink string list element from string list, and return the string
 * list element's string.
 *
 * @param self The string list element to be unlinked.
 * @param error Error code.
 *
 * @return Pointer to the unlinked string list element's @a string.
 *
 * @note Caller is responsible for the returned string's memory.
 */
S_API char *s_str_list_element_unlink(s_str_list_element *self, s_erc *error);


/**
 * Remove the string list element from it's parent string list and
 * delete string list element's string.
 *
 * @param self The string list element to be deleted.
 * @param error Error code.
 *
 * @return Pointer to the previous string list element or #NULL if none.
 *
 */
S_API s_str_list_element *s_str_list_element_delete(s_str_list_element *self,
													s_erc *error);


/**
 * Return the next string list element in the string list, relative to
 * the given one.
 *
 * @param self Pointer to current position in string list.
 * @param error Error code.
 *
 * @return Pointer to the next string list element in the string list.
 */
S_API const s_str_list_element *s_str_list_element_next(const s_str_list_element *self,
														s_erc *error);


/**
 * Return the previous string list element in the string list,
 * relative to the given one.
 *
 * @param self Pointer to current position in string list.
 * @param error Error code.
 *
 * @return Pointer to the previous string list element in the string
 * list.
 */
S_API const s_str_list_element *s_str_list_element_prev(const s_str_list_element *self,
														s_erc *error);


/**
 * @hideinitializer
 * Find the string list element which matches the given string.
 *
 * @param self The string list.
 * @param string The string to match.
 * @param error Error code.
 *
 * @return Pointer to the string list element which matches the given
 * string, else #NULL if no match.
 *
 * @note Returns the first match.
 */
#define s_str_list_find(self, string, error)			\
	s_str_list_find_index(self, NULL, string, NULL, error)


/**
 * Query if the string list has any elements.
 *
 * @param self The string list to query.
 * @param error Error code.
 *
 * @return #TRUE or #FALSE.
 */
S_API s_bool s_str_list_isempty(const s_str_list *self, s_erc *error);


/**
 * Get the number of elements in the string list.
 *
 * @param self The string list to query.
 * @param error Error code.
 *
 * @return The number of elements in the string list.
 */
S_API uint32 s_str_list_size(const s_str_list *self, s_erc *error);


/**
 * Push the given string onto end of given string list. Same as
 * #s_str_list_append.
 *
 * @param self The string list.
 * @param error Error code.
 * @param data The string to append.
 *
 * @note Internally a duplicate of @c string is made and ownership
 * taken thereof.
 */
S_API void s_str_list_push(s_str_list *self, const char *string,
						   s_erc *error);


/**
 * Pop last string of string list. Removes last string list element
 * from the string list and returns the string list element's string.
 *
 * @param self The string list.
 * @param error Error code.
 *
 * @return Pointer to popped string.
 *
 * @note The caller is responsible for the memory of the returned
 * string.
 */
S_API char *s_str_list_pop(s_str_list *self, s_erc *error);


/**
 * Reverse the order of the string list elements in the string list.
 *
 * @param self The string list to reverse.
 * @param error Error code.
 *
 * @note The string list element's strings are not reversed.
 */
S_API void s_str_list_reverse(s_str_list *self, s_erc *error);


/**
 * Prepend a string to beginning of the string list.
 *
 * @param self The string list.
 * @param error Error code.
 * @param string The string to prepend.
 *
 * @note Internally a duplicate of @c string is made and ownership
 * taken thereof.
 */
S_API void s_str_list_prepend(s_str_list *self, const char *string,
							  s_erc *error);


/**
 * Append a string to end of the string list.
 *
 * @param self The string list.
 * @param error Error code.
 * @param string The string to append.
 *
 * @note Internally a duplicate of @c string is made and ownership
 * taken thereof.
 */
S_API void s_str_list_append(s_str_list *self, const char *string,
							 s_erc *error);


/**
 * Insert a string before given string list element. Return reference
 * to inserted string element.
 *
 * @param self The string list element before which the string must be
 * inserted.
 * @param string The string to be inserted.
 * @param error Error code.
 *
 * @return Pointer to the inserted string list element.
 *
 * @note Internally a duplicate of @c string is made and ownership
 * taken thereof.
 */
S_API const s_str_list_element *s_str_list_insert_before(s_str_list_element *self,
														 const char *string, s_erc *error);


/**
 * Insert a string after given string list element. Return reference
 * to inserted string element.
 *
 * @param self The string list element after which the string must be
 * inserted.
 * @param string The string to be inserted.
 * @param error Error code.
 *
 * @return Pointer to the inserted string list element.
 *
 * @note Internally a duplicate of @c string is made and ownership
 * taken thereof.
 */
S_API const s_str_list_element *s_str_list_insert_after(s_str_list_element *self,
														const char *string, s_erc *error);


/**
 * Return a newly allocated copy of the source string list, which must
 * later be freed by the caller.
 *
 * @param src The source string list.
 * @param error Error code.
 *
 * @return A copy of @a src.
 *
 * @note This is a <em> deep copy </em>.
 */
S_API s_str_list *s_str_list_dup(const s_str_list *src, s_erc *error);


/**
 * Return a newly allocated slice of the given string list of the
 * source string list, which must later be freed by the caller.
 *
 * @param src The source string list.
 * @param a Start position of the slice in the source string list. If
 * a is greater than or equal to the list size then #NULL is returned.
 * @param b End position of the slice in the source string list. If b = -1, or
 * b is greater than the number of elements in the source string list then
 * the slice is to the end of the source string list
 * @param error Error code.
 *
 * @return A slice of @a src.
 *
 * @note This is a <em> deep copy </em>.
 */
S_API s_str_list *s_str_list_slice(const s_str_list *src,
								   uint32 a, sint32 b, s_erc *error);


/**
 * Compare the elements of two string lists.
 *
 * @param sl1 First string list.
 * @param sl2 Second string list.
 * @param error Error code.
 *
 * @return -1 if lists compare exactly, else the number of elements
 * that compare.
 */
S_API sint32 s_str_list_cmp(const s_str_list *sl1, const s_str_list *sl2,
							s_erc *error);


/**
 * Merge two string lists. Append all the string list elements of one
 * list onto another list.
 *
 * @param self The string list that @c with is merged with.
 * @param with The string list to merge with @c self.
 * @param error Error code.
 */
S_API void s_str_list_merge(s_str_list *self, const s_str_list *with,
							s_erc *error);


/**
 * Convert the string list to a string with the given separator
 * between elements of the string list.
 *
 * @param self The string list.
 * @param sep The separator to put between the string elements.
 * @param error Error code.
 *
 * @return The string list as a string.
 *
 * @note The caller is responsible for the memory of the returned
 * string.
 */
S_API char *s_str_list_to_string(s_str_list *self, const char *sep,
								 s_erc *error);


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

#endif /* _SPCT_STRING_UTF8_STRING_LIST_H__ */

