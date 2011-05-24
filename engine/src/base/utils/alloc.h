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
/* Memory Allocation Abstraction.                                                   */
/* We define wrappers to the standard library function for in case we want to       */
/* change something or do some memory tests.                                        */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_ALLOC_H__
#define _SPCT_ALLOC_H__


/**
 * @file alloc.h
 * Memory allocation abstraction.
 */


/**
 * @ingroup SBaseUtils
 * @defgroup SMemHandling Memory Handling Abstraction
 * Defines macros for use in memory allocation and deallocation. These macros
 * wrap the standard @c malloc, @c calloc, @c realloc and @c free functions.
 * @{
 */


/**
 * @example mem_example.c
 * Some memory handling examples.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stddef.h>
#include "include/common.h"


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
 * Allocate memory to a pointer.
 * Allocates @a NMEMB number of @a TYPE data type and returns a
 * pointer to the allocated memory.
 * @hideinitializer
 *
 * @param TYPE The data type to allocate.
 * @param NMEMB The number of @a TYPE data type to allocate.
 *
 * @return Pointer to the allocated memory or #NULL if failed.
 *
 * @note If (sizeof(TYPE) * NMEMB) = 0 then returns #NULL.
 */
#define S_MALLOC(TYPE, NMEMB) (_s_malloc(sizeof(TYPE)*(NMEMB)))


/**
 * Allocate memory to a pointer.
 * Allocates @a SIZE bytes and returns a pointer to the allocated memory.
 * @hideinitializer
 *
 * @param SIZE The number bytes to allocate.
 *
 * @return Pointer to the allocated memory or #NULL if failed.
 *
 * @note If SIZE = 0 then returns #NULL.
 */
#define S_MALLOC_SIZE(SIZE) (_s_malloc(SIZE))


/**
 * Allocate memory to a pointer and set it to zero.
 * Allocates @a NMEMB number of @a TYPE data type and returns a
 * pointer to the allocated memory.
 * @hideinitializer
 *
 * @param TYPE The data type to allocate.
 * @param NMEMB The number of @a TYPE data type to allocate.
 *
 * @return Pointer to the allocated memory (set to zero) or #NULL if
 * failed.
 *
 * @note If (sizeof(TYPE) * NMEMB) = 0 then returns #NULL.
 */
#define S_CALLOC(TYPE,NMEMB) (_s_calloc(sizeof(TYPE)*(NMEMB)))


/**
 * Allocate memory to a pointer and set it to zero.
 * Allocates @a SIZE bytes and returns a pointer to the allocated
 * memory.
 * @hideinitializer
 *
 * @param SIZE The number bytes to allocate.
 *
 * @return Pointer to the allocated memory (set to zero) or #NULL if
 * failed.
 *
 * @note If SIZE = 0 then returns #NULL.
 */
#define S_CALLOC_SIZE(SIZE) (_s_calloc(SIZE))


/**
 * Change the size of the memory block.
 * Changes the size of the memory block pointed to by @a P to @a NMEMB
 * number of @a TYPE data type.
 * @hideinitializer
 *
 * @param P The pointer to the memory of which to change the size.
 * @param TYPE The data type to allocate.
 * @param NMEMB The number of @a TYPE data type to allocate.
 *
 * @return Pointer to the allocated memory or #NULL if failed.
 *
 * @note If (sizeof(TYPE) * NMEMB) = 0 then frees @a P and returns #NULL.
 * @note If memory allocation failed then @a P is freed.
 */
#define S_REALLOC(P,TYPE,NMEMB) (_s_realloc((void *)(P),sizeof(TYPE)*(NMEMB)))


/**
 * Change the size of the memory block.
 * Changes the size of the memory block pointed to by @a P to @a SIZE
 * bytes.
 * @hideinitializer
 *
 * @param P The pointer to the memory of which to change the size.
 * @param SIZE The number of bytes to allocate.
 *
 * @return Pointer to the allocated memory or #NULL if failed.
 *
 * @note If SIZE = 0 then frees @a P and returns #NULL.
 * @note If memory allocation failed then @a P is freed.
 */
#define S_REALLOC_SIZE(P,SIZE) (_s_realloc((void *)(P),SIZE))


/**
 * Frees the memory space pointed to by @a P.
 * @hideinitializer
 *
 * @param P The memory space to free.
 *
 * @note Sets @a P to #NULL.
 */
#define S_FREE(P)								\
	do {										\
		_s_free(P);								\
		P = NULL;								\
	} while (0)


/************************************************************************************/
/*                                                                                  */
/* Function prototypes (do not use these directly, use above defined macros)        */
/*                                                                                  */
/************************************************************************************/

/* wrapper to malloc */
S_API void *_s_malloc(size_t len);


/* wrapper to calloc */
S_API void *_s_calloc(size_t len);


/* wrapper to realloc */
S_API void *_s_realloc(void *p_old, size_t len);


/* wrapper to free */
S_API void _s_free(void *p);


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

#endif /* _SPCT_ALLOC_H__ */

