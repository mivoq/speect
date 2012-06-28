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
/* Symbol visibility. From http://gcc.gnu.org/wiki/Visibility                       */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_VISIBILITY_H__
#define _SPCT_VISIBILITY_H__


/**
 * @file visibility.h
 * Symbol visibility definitions.
 */


/**
 * @ingroup SDefines
 * @defgroup SVisibility Symbol Visibility
 * Macros for controlling library symbol visibility and linkage. These
 * macros have platform specific values. #S_API and #S_LOCAL
 * can be reused by plug-in implementations for better portability. The
 * complexity is mostly due to Windows DLL exports and imports.
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
 * @def S_API
 * @hideinitializer
 * Declares a symbol to be exported for shared library usage.
 */


/**
 * @def S_LOCAL
 * @hideinitializer
 * Declares a symbol hidden from other libraries.
 */


/**
 * @def S_PLUGIN_API
 * @hideinitializer
 * Declares a symbol to be exported for plug-in usage.
 */


/* Generic helper definitions for shared library support */
#if defined _WIN32 || defined __CYGWIN__
#  define SPCT_HELPER_DLL_IMPORT __declspec(dllimport)
#  define SPCT_HELPER_DLL_EXPORT __declspec(dllexport)
#  define SPCT_HELPER_DLL_LOCAL
#else /* ! defined _WIN32 || defined __CYGWIN__ || defined SPCT_WIN32 */
#  if __GNUC__ >= 4
#    define SPCT_HELPER_DLL_IMPORT __attribute__ ((visibility("default")))
#    define SPCT_HELPER_DLL_EXPORT __attribute__ ((visibility("default")))
#    define SPCT_HELPER_DLL_LOCAL  __attribute__ ((visibility("hidden")))
#  else /* ! __GNUC__ >= 4 */
#    define SPCT_HELPER_DLL_IMPORT
#    define SPCT_HELPER_DLL_EXPORT
#    define SPCT_HELPER_DLL_LOCAL
#  endif /* __GNUC__ >= 4 */
#endif /* defined _WIN32 || defined __CYGWIN__ || defined SPCT_WIN32 */


/*
 * SPCT_LIBRARIES_EXPORTS
 * ----------------------
 * CMake will define SPCT_LIBRARIES_EXPORTS when it configures to build a
 * shared library. If you are going to use another build system
 * you need to define SPCT_LIBRARIES_EXPORTS when building a DLL.
 *
 * Now we use the generic helper definitions above to define
 * S_API and S_LOCAL. S_API is used for the public API symbols.
 * It's either DLL imports or DLL exports (or does nothing for static build)
 * S_LOCAL is used for non-api symbols.
 *
 * SPCT_SRC
 * ------------------
 * CMake defines SPCT_SRC when building Speect (instead of just using it).
 * If you are going to use another build system you need to define
 * SPCT_SRC when building Speect.
 *
 */


#ifdef SPCT_LIBRARIES_EXPORTS
#  ifdef SPCT_SRC
#    define S_API SPCT_HELPER_DLL_EXPORT
#  else /* !SPCT_SRC */
#    define S_API SPCT_HELPER_DLL_IMPORT
#  endif /* SPCT_SRC */
#  define S_LOCAL SPCT_HELPER_DLL_LOCAL
#else /* !SPCT_LIBRARIES_EXPORTS (static library) */
#  define S_API
#  define S_LOCAL
#endif /* SPCT_LIBRARIES_EXPORTS */


/*
 * Plug-in s_plugin_init functions need this modifier so that the
 * function is visible to the outside.
 */
#define S_PLUGIN_API SPCT_HELPER_DLL_EXPORT


/**
 * @}
 * end documentation
 */

#endif /* _SPCT_VISIBILITY_H__ */
