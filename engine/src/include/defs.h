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
/* Some usefull macros and defines.                                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_DEFS_H__
#define _SPCT_DEFS_H__


/**
 * @file defs.h
 * Definitions of macros that are common to the whole Speect engine.
 */


/**
 * @ingroup SDefines
 * @defgroup SMacros System Macros
 * Definitions of macros that are common to the whole Speect engine.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/spctconfig.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * @def S_BEGIN_C_DECLS
 * Start block for enclosing C code for inclusion in C++ programs.
 * This allows C++ programs to include the C header files of the Speect
 * engine. @code extern "C" { @endcode
 * @hideinitializer
 */


/**
 * @def S_END_C_DECLS
 * End block for enclosing C code for inclusion in C++ programs.
 * This allows C++ programs to include the C header files of the Speect
 * engine. @code } @endcode
 * @hideinitializer
 */


#undef  S_BEGIN_C_DECLS
#undef  S_END_C_DECLS
#ifdef __cplusplus
#  define S_BEGIN_C_DECLS extern "C" {
#  define S_END_C_DECLS }
#else /* !__cplusplus */
#  define S_BEGIN_C_DECLS
#  define S_END_C_DECLS
#endif	/* __cplusplus */


/**
 * @def NULL
 * Defined NULL pointer if it has not previously been defined.
 * Platform specific value.
 * @hideinitializer
 */
#if !defined(__MACH__)
#  ifndef NULL
#    ifdef __cplusplus
#      define NULL 0
#    else /* !__cplusplus */
#      define NULL ((void *)0)
#    endif /* __cplusplus */
#  endif /* NULL */
#endif /* ! Mac OS X - breaks precompiled headers */


/**
 * @def S_MAX_PATH_LENGTH
 * Defined maximum length (size in bytes) of a string
 * buffer for file paths.
 */
#define S_MAX_PATH_LENGTH 1024


/**
 * @def S_PATH_SEP
 * Defined path separator of the file system.
 * Platform specific value.
 */
#ifdef SPCT_WIN32
#  define S_PATH_SEP '\\'
#else /* !SPCT_WIN32 */
#  define S_PATH_SEP '/'
#endif /* SPCT_WIN32 */


/* make sure these are defined if building Speect */
/**
 * @def TRUE
 * Definition of the TRUE value. TRUE = 1.
 * @hideinitializer
 */

/**
 * @def FALSE
 * Definition of the FALSE value. FALSE = 0.
 * @hideinitializer
 */
#ifdef SPCT_SRC /* defined by compiler when building Speect */
#  undef TRUE
#  undef FALSE
#  define TRUE 1
#  define FALSE 0
#else /* ! SPCT_SRC */

/* when only using Speect */
#  ifndef TRUE
#    define TRUE 1
#  elif TRUE != 1
#    error "TRUE != 1, Speect requires TRUE == 1"
#  endif
#  ifndef FALSE
#    define FALSE 0
#  elif FALSE != 0
#    error "FALSE != 0, Speect requires FALSE == 0"
#  endif
#endif /* SPCT_SRC */


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @def S_CONCAT(A,B)
 * @hideinitializer
 * Concatenate two strings.
 *
 * @param A String A
 * @param B String B to concatenate to string A.
 *
 * @return Concatenated strings A and B.
 */
#define S_MAKECONCAT(A,B) A ## B
#define S_CONCAT(A,B) S_MAKECONCAT(A,B)


/**
 * @def S_TOSTRING(S)
 * @hideinitializer
 * Return a string of the given argument.
 *
 * @param S Argument to turn into a string.
 *
 * @return @c "S"
 */
#define S_TOSTRING(S) # S


/**
 * @def S_VOIDPTR(PTR)
 * @hideinitializer
 *
 * Cast a pointer to a a pointer of type void.
 *
 * @param PTR Pointer to cast to void pointer.
 *
 * @return <tt> (void*)PTR </tt>
 */
#define S_VOIDPTR(PTR) ((void*)PTR)


/**
 * @}
 * end documentation
 */

#endif /* _SPCT_DEFS_H__ */
