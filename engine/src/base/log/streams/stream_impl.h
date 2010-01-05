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
/* Basic logging stream facilities.                                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LOG_STREAM_H__
#define _SPCT_LOG_STREAM_H__


/**
 * @file stream_impl.h
 * Basic stream logging facilities.
 */


/**
 * @ingroup SLogging
 * @defgroup SLogger Streams
 * Basic loggin stream facilities.
 * The logging stream system provides a very basic mechanism to write
 * messages to different types of streams. It is used by all the error
 * and debug reporting facilites in @ref SErrDbg, and therefore aims
 * to be as robust as possible. If any error occurs during one of the
 * logger functions, an error will be printed to @c stderr. If a
 * logger is unsuccessful at writing the log message to the log stream
 * then it will log the message to @c stdout. It is easily extendable
 * by implementing the #s_logger structure pointer functions for a
 * new logger, with current implementations @ref SFileLogger
 * and @ref SConsoleLogger.
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
 * Opaque data type for logger private info. Defined by each logger as
 * required.
 */
typedef struct s_logger_private_info s_logger_private_info;


/* forward declaration */
typedef struct s_logger s_logger;


/**
 * The s_logger structure definition. The logger consists of private
 * data which can be anything required by the specific implementation,
 * as it is only defined as an opaque type. The different logger
 * implementations must implement the function pointers as described.
 * If any error occurs during one of the logger functions, the logger
 * must try and continue and print the error to @c stderr.
 */
struct s_logger
{
	/**
	 * Logger private data. Defined by each logger as
	 * required (opaque type).
	 */
	s_logger_private_info *data;


	/**
	 * Write a message to the logger stream with a vardiac list
	 * pointer. Must be implemented by each logger as it is called by
	 * #s_logger_vwrite and #s_logger_write. If the message cannot be
	 * written successfully then the calling function
	 * (#s_logger_vwrite or #s_logger_write) will attemp to write the
	 * message to @c stdout.
	 *
	 * @param self This logger object.
	 * @param fmt A format string specifying the string to write and
	 * the format of the arguments in the va_list.
	 * @param argp The va_list, see the standard function @c
	 * vprintf(). The value of @c argp is undefined after the call to
	 * this function.
	 *
	 * @return Error code.
	 */
	s_erc (*v_write)(const s_logger *self, const char *fmt, va_list argp);


	/**
	 * Destroy the logger object's associated resources. The logger's
	 * memory itself must @b not be free'd. Must be implemented by
	 * each logger as it is called by #s_logger_destroy.
	 *
	 * @param logger This logger object.
	 *
	 * @return Error code.
	 */
	s_erc (*destroy)(s_logger *self);
};


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/


/**
 * Write a message to the logger with a variable argument list.
 * Writes the given message with the given format.
 *
 * @param logger The logger to write to.
 * @param fmt A format string specifying the string to write and the
 * format of the variable length argument list. Same as the the
 * standard @c printf() function.
 *
 * @return Error code.
 *
 * @note Thread-safety is dependant on the underlying implementation.
 */
S_API s_erc s_logger_write(const s_logger *logger, const char *fmt, ...);


/**
 * Write a message to the logger with a vardiac list pointer.
 * Equivalent to the #s_logger_write except that it is called
 * with a va_list instead of a variable number of arguments,
 * same as the standard function @c vprintf().
 *
 * @param logger The logger to write to.
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
S_API s_erc s_logger_vwrite(const s_logger *logger, const char *fmt, va_list argp);


/**
 * Destroy the given logger's associated resources. The logger @b must
 * still be free'd with #S_FREE after this call.
 *
 * @param logger The logger object's resources to destroy.
 *
 * @return error Error code.
 *
 * @note Thread-safety is dependant on the underlying implementation.
 */
S_API s_erc s_logger_destroy(s_logger *logger);


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

#endif /* _SPCT_LOG_STREAM_H__ */

