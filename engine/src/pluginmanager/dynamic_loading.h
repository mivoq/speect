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
/* DATE    : 4 December 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Dynamic Shared Object open/symbols/close abstraction.                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_DSO_LOADING_H__
#define _SPCT_DSO_LOADING_H__


/**
 * @file dynamic_loading.h
 * DSO open/symbols/close abstraction.
 */


/**
 * @ingroup SDSO
 * @defgroup SDSOLoading DSO Loading Abstraction
 * Defines a set of functions to open/close and load symbols from <i>dymanic shared
 * objects</i>. Different dynamic loading/unloading implementations are supported by
 * implementing the following macros:
 *
 * <table>
 *  <tr>
 *   <td> @code void *_S_DSO_OPEN(const char *FILENAME, s_erc *ERROR) @endcode </td>
 *   <td> Open a dynamic shared object (see #s_dso_open). </td>
 *  </tr>
 *  <tr>
 *   <td> @code void *_S_DSO_SYM(void *DSO_HANDLE, const char *SYMBOL, s_erc *ERROR) @endcode </td>
 *   <td> Load a symbol from a dynamic shared object (see #s_dso_sym). </td>
 *  </tr>
 *  <tr>
 *   <td> @code void _S_DSO_CLOSE(void *DSO_HANDLE, s_erc *ERROR) @endcode </td>
 *   <td> Close a dynamic shared objectx (see #s_dso_close).</td>
 *  </tr>
 * </table>
 *
 * See the posix_dll.h and win32_dll.h for examples. The dl library loader loads
 * with flags <tt>RTLD_LAZY|RTLD_LOCAL</tt>, while the win32 library loader's
 * loading flag is <tt>LOAD_WITH_ALTERED_SEARCH_PATH</tt>.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Open a <i>dynamic shared object</i>.
 *
 * @param filename The full path and filename of the dynamic shared object.
 * @param error Error code.
 *
 * @return Handle to the dynamic shared object.
 */
S_API void *s_dso_open(const char *filename, s_erc *error);


/**
 * Get a symbol from a given <i>dynamic shared object</i>.
 *
 * @param dso_handle Handle to the dynamic shared object.
 * @param symbol The name of the symbol to get.
 * @param error Error code.
 *
 * @return  Handle to the symbol.
 */
S_API void *s_dso_sym(void *dso_handle, const char *symbol, s_erc *error);


/**
 * Close the given <i>dynamic shared object</i>.
 *
 * @param dso_handle Handle to the dynamic shared object to close.
 * @param error Error code.
 */
S_API void s_dso_close(void *dso_handle, s_erc *error);


/**
 * Initialize the dynamic loading module
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_dynamic_loading_init(s_erc *error);


/**
 * Quit the dynamic loading module.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_dynamic_loading_quit(s_erc *error);


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

#endif /* _SPCT_DSO_LOADING_H__ */

