/************************************************************************************/
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
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
/* DATE    : June 2012                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Environment variable routines.                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_ENVVAR_H__
#define _SPCT_ENVVAR_H__


/**
 * @file envvar.h
 * Environment variable routines.
 */


/**
 * @ingroup SBaseUtils
 * @defgroup SEnvvar Environment Variables
 * Environment variable routines.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"
#include "base/utils/platform/envvar_impl.h"


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
 * Initialize the environment variable module. This initialization is
 * required because the standard environment variable functions return
 * values that are statically allocated, and are therefore neither
 * re-entrant or thread safe.
 * @private
 *
 * @param error Error code.
 *
 * @note Dependent on the @ref SErrDbg module being initialized.
 */
S_LOCAL void _s_envvar_init(s_erc *error);


/**
 * Quit the environment variable module. Releases the mutex resources
 * for this module.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_envvar_quit(s_erc *error);


/**
 * The function returns a pointer to named value in the environment,
 * or NULL if there is no match.
 *
 * @param name The name of the environment variable to get.
 * @param error Error code.
 *
 * @return Pointer to the named value in the environment, or NULL if
 * there is no match.
 * @note Caller is responsible for returned memory.
 */
S_API char *s_getenv(const char *name, s_erc *error);


/**
 * Get the environment defined Speect plug-in path.
 *
 * @param error Error code.
 *
 * @return Pointer to the plug-in path, or NULL if it is not defined.
 * @note Caller is responsible for returned memory.
 */
S_API char *s_getenv_plugin_path(s_erc *error);


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

#endif /* _SPCT_ENVVAR_H__ */

