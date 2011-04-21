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
/* Basic logging facilities.                                                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LOGGER_H__
#define _SPCT_LOGGER_H__


/**
 * @file logger.h
 * Basic logging facilities.
 */


/**
 * @ingroup SLogging
 * @defgroup SLogger Loggers
 * Basic logging facilities.
 * The logging system provides a very basic mechanism to write
 * messages to different types of streams. It is used by all the error
 * and debug reporting facilites in @ref SErrDbg, and therefore aims
 * to be as robust as possible. If any error occurs during one of the
 * logger functions, an error will be printed to @c stderr. If a
 * logger is unsuccessful at writing the log message to the log stream
 * then it will log the message to @c stdout.
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
	 * Format and write the given logging event information to the
	 * logger. Must be implemented by each logger as it is called by
	 * #s_logger_vwrite and #s_logger_write. If the message cannot be
	 * formatted or written successfully then the calling function
	 * (#s_logger_vwrite or #s_logger_write) will attemp to write the
	 * message to @c stdout.
	 *
	 * @param self This logger object.
	 * @param level The event level.
	 * @param error_msg As defined by #s_erc.
	 * @param func_name Calling function name.
	 * @param file_name Calling function file name.
	 * @param line_num Calling line number in function.
	 * @param user_msg A format string specifying the string to write
	 * and the format of the arguments in the va_list.
	 * @param argp The va_list, see the standard function @c
	 * vprintf(). The value of argp is undefined after the call to
	 * this function.
	 *
	 * @return Error code.
	 */
	s_erc (*v_write)(const s_logger *self, s_log_event level, const char *error_msg,
					 const char *func_name, const char *file_name, int line_num,
					 const char *user_msg, va_list argp);


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
 * @name Logger Creators
 * @{
 */


/**
 * Create a new file stream logger, see @ref SFileStream. The logging
 * message will be in the @ref SLayoutStd layout format.
 *
 * @param path The full path and filename of the file which to
 * log to. If the file already exists, then it will be overwritten.
 *
 * @return Pointer to newly created file stream logger, or @c NULL on error.
 *
 * @note Only thread safe if compiled with threading library, and
 * whether the standard vfprintf() function is thread-safe,
 * see @ref SThreads.
 *
 * @todo check MT safety
 */
S_API s_logger *s_logger_file_new(const char *path);


/**
 * Create a new console stream logger, see @ref SConsoleStream. The
 * logging message will be in the @ref SLayoutStd layout format.
 *
 * @param log_to_stdout If #TRUE then logging will be to @c stdout,
 * otherwise streaming will be to @c stderr.
 *
 * @return Pointer to newly created console stream logger, or @c NULL
 * on error.
 *
 * @note Only thread safe if compiled with threading library, and
 * whether the standard vfprintf() function is thread-safe,
 * see @ref SThreads.

 * @todo check MT safety
 */
S_API s_logger *s_logger_console_new(s_bool log_to_stdout);


/**
 * @}
 */


/**
 * @name Logging Functions
 * @{
 */


/**
 * Format and write the given logging event information to the
 * logger.
 *
 * @param logger The logger to write to.
 * @param level The event level.
 * @param error_msg As defined by #s_erc.
 * @param func_name Calling function name.
 * @param file_name Calling function file name.
 * @param line_num Calling line number in function.
 * @param user_msg A format string specifying the string to write and
 * the format of the variable length argument list. Same as the the
 * standard @c printf() function.
 *
 * @return Error code.
 */
S_API s_erc s_logger_write(const s_logger *logger, s_log_event level, const char *error_msg,
						   const char *func_name, const char *file_name, int line_num,
						   const char *user_msg, ...);


/**
 * Format and write the given logging event information to the
 * logger. Equivalent to the #s_logger_write except that it is called
 * with a va_list instead of a variable number of arguments,
 * same as the standard function @c vprintf().
 *
 * @param logger The logger to write to.
 * @param level The event level.
 * @param error_msg As defined by #s_erc.
 * @param func_name Calling function name.
 * @param file_name Calling function file name.
 * @param line_num Calling line number in function.
 * @param user_msg A format string specifying the string to write
 * and the format of the arguments in the va_list.
 * @param argp The va_list, see the standard function @c
 * vprintf(). The value of argp is undefined after the call to
 * this function.
 *
 * @return Error code.
 */
S_API s_erc s_logger_vwrite(const s_logger *logger, s_log_event level, const char *error_msg,
							const char *func_name, const char *file_name, int line_num,
							const char *user_msg, va_list argp);


/**
 * @}
 */


/**
 * @name Logger Destroyer
 * @{
 */


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


/**
 * @}
 */


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

#endif /* _SPCT_LOGGER_H__ */

