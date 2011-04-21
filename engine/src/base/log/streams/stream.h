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
/* DATE    : 29 September 2009                                                      */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Basic stream facilities.                                                         */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_STREAM_H__
#define _SPCT_STREAM_H__


/**
 * @file stream.h
 * Basic stream facilities.
 */


/**
 * @ingroup SLogging
 * @defgroup SStream Streams
 * Basic stream facilities.
 * The stream module provides a very basic mechanism to write
 * messages to different types of streams. It is used by all the error
 * and debug reporting facilites in @ref SErrDbg, and therefore aims
 * to be as robust as possible. If any error occurs during one of the
 * streaming functions, an error will be printed to @c stderr, and the
 * message to the stream will be written to @c stdout. It is
 * easily extend-able by implementing the #s_stream structure pointer
 * functions for a new stream type, with current
 * implementations @ref SFileStream and @ref SConsoleStream.
 *
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdarg.h>
#include "include/common.h"
#include "base/errdbg/errdbg_defs.h"



/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Logger object                                                                    */
/*                                                                                  */
/************************************************************************************/

/**
 * Opaque data type for stream private info. Defined by each stream as
 * required.
 */
typedef struct s_stream_private_info s_stream_private_info;


/* forward declaration */
typedef struct s_stream s_stream;


/**
 * The s_stream structure definition. The stream consists of private
 * data which can be anything required by the specific implementation,
 * as it is only defined as an opaque type. The different stream
 * implementations must implement the function pointers as described.
 * If any error occurs during one of the stream functions, the stream
 * must try and continue and print the error to @c stderr.
 */
struct s_stream
{
	/**
	 * Stream private data. Defined by each stream as
	 * required (opaque type).
	 */
	s_stream_private_info *data;


	/**
	 * Write a message to the stream with a vardiac list pointer. Must
	 * be implemented by each stream as it is called by
	 * #s_stream_vwrite and #s_stream_write. If the message cannot be
	 * written successfully then the calling function
	 * (#s_stream_vwrite or #s_stream_write) will attemp to write the
	 * message to @c stdout.
	 *
	 * @param self This stream object.
	 * @param fmt A format string specifying the string to write and
	 * the format of the arguments in the va_list.
	 * @param argp The va_list, see the standard function @c
	 * vprintf(). The value of @c argp is undefined after the call to
	 * this function.
	 *
	 * @return Error code.
	 */
	s_erc (*v_write)(const s_stream *self, const char *fmt, va_list argp);


	/**
	 * Destroy the stream object's associated resources. The stream's
	 * memory itself must @b not be free'd. Must be implemented by
	 * each stream as it is called by #s_stream_destroy.
	 *
	 * @param stream This stream object.
	 *
	 * @return Error code.
	 */
	s_erc (*destroy)(s_stream *self);
};


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/


/**
 * Write a message to the stream with a variable argument list.
 * Writes the given message with the given format.
 *
 * @param stream The stream to write to.
 * @param fmt A format string specifying the string to write and the
 * format of the variable length argument list. Same as the standard
 * @c printf() function.
 *
 * @return Error code.
 *
 * @note Thread-safety is dependant on the underlying implementation.
 */
S_API s_erc s_stream_write(const s_stream *stream, const char *fmt, ...);


/**
 * Write a message to the stream with a vardiac list pointer.
 * Equivalent to the #s_stream_write except that it is called
 * with a va_list instead of a variable number of arguments,
 * same as the standard function @c vprintf().
 *
 * @param stream The stream to write to.
 * @param fmt A format string specifying the string to write and the
 * format of the arguments in the va_list.
 * @param argp The va_list, see the standard function @c
 * vprintf(). The value of argp is undefined after the call to this
 * function.
 *
 * @return error Error code.
 *
 * @note Thread-safety is dependant on the underlying implementation.
 */
S_API s_erc s_stream_vwrite(const s_stream *stream, const char *fmt, va_list argp);


/**
 * Destroy the given stream's associated resources. The stream @b must
 * still be free'd with #S_FREE after this call.
 *
 * @param stream The stream object's resources to destroy.
 *
 * @return error Error code.
 *
 * @note Thread-safety is dependant on the underlying implementation.
 */
S_API s_erc s_stream_destroy(s_stream *stream);


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

#endif /* _SPCT_STREAM_H__ */

