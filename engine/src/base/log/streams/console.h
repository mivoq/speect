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
/* DATE    : 29 September 2009                                                      */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Console streaming facilities.                                                    */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_STREAM_CONSOLE_H__
#define _SPCT_STREAM_CONSOLE_H__


/**
 * @file console.h
 * A console log stream.
 */


/**
 * @ingroup SStream
 * @defgroup SConsoleStream Console Stream
 * Implements a console stream for either @c stdout or @c stderr.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
#include "base/log/streams/stream.h"


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
 * Create a new console stream. The console stream implementes the
 * function pointers of #s_stream (#s_stream::v_write and
 * #s_stream::destroy).
 *
 * @param log_to_stdout If #TRUE then logging will be to @c stdout,
 * otherwise streaming will be to @c stderr.
 *
 * @return Pointer to newly created console stream that can be used
 * with the functions defined in @ref SStream, or @c NULL on error.
 *
 * @note Only thread safe if compiled with threading library, and
 * whether the standard vfprintf() function is thread-safe,
 * see @ref SThreads.

 * @todo check MT safety
 */
S_API s_stream *s_stream_console_new(s_bool log_to_stdout);


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

#endif /* _SPCT_STREAM_CONSOLE_H__ */

