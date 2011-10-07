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
/* DATE    : 5 June 2009                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Time related functions and definitions.                                          */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_STIME_H__
#define _SPCT_STIME_H__


/**
 * @file stime.h
 * Time related functions and definitions.
 */


/**
 * @ingroup SBaseUtils
 * @defgroup STime Time Functions
 * Time related functions and definitions.
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
 * Initialize the time module. This initialization is required because
 * the standard time functions return values that are statically
 * allocated, and are therefore neither re-entrant or thread safe.
 * @private
 *
 * @param error Error code.
 *
 * @note Dependent on the @ref SErrDbg module being initialized.
 */
S_LOCAL void _s_time_init(s_erc *error);


/**
 * Quit the time module. Releases the mutex resources for this module.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_time_quit(s_erc *error);


/**
 * Get the current time as a string.
 *
 * @param error Error code.
 *
 * @return Pointer to a string buffer of the current time. Returns a full
 * representation of the data and time, for example:
 @verbatim
 Tue 03 Nov 2009 13:12:54 SAST
 @endverbatim
 *
 * @note Caller is responsible for memory of returned string buffer.
 */
S_API char *s_strtime(s_erc *error);


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

#endif /* _SPCT_STIME_H__ */

