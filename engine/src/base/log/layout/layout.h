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
/* Message layout (format) functions.                                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LAYOUT_H__
#define _SPCT_LAYOUT_H__


/**
 * @file layout.h
 * Message layout (format) implementation.
 */


/**
 * @ingroup SLogging
 * @defgroup SLayout Message Layout
 * Message layout (format) functions.
 * The layout formatters provides a simple mechanism to format logging
 * messages before they are output to different streams. This enables
 * one to have different log formats, i.e. xml, html, syslog, and so
 * forth, with the same top level interface. Of course the layout
 * implementations need to be robust enough to still output the
 * message in some or other way even if the formatting failed for some
 * reason. It is easily extendable by implementing the #s_layout
 * structure pointer functions for a new layout, with a current
 * implementation @ref SLayoutStd.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdarg.h>
#include "include/common.h"
#include "base/log/event/event.h"
#include "base/errdbg/errdbg_defs.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Typedef and data types                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * Opaque data type for layout private info. Defined by each layout as
 * required.
 */
typedef struct s_layout_private_info s_layout_private_info;


/* forward declaration */
typedef struct s_layout s_layout;


/**
 * The s_layout structure definition. The layout consists of private
 * data which can be anything required by the specific implementation,
 * as it is only defined as an opaque type. The different layout
 * implementations must implement the function pointers as described.
 */
struct s_layout
{
	/**
	 * Layout private data. Defined by each layout as
	 * required (opaque type).
	 */
	s_layout_private_info *data;


	/**
	 * Format the given logging event information to a character
	 * string buffer, which can be ouput by different logging streams
	 * (@ref SLogger). Must be implemented by each layout as it is
	 * called by #s_layout_format. If the message cannot be formatted
	 * successfully, this function @b must return @c NULL, then the
	 * #s_layout_format will attemp to write the message to @c stdout
	 * in a predefined format.
	 *
	 * @param level The event level.
	 * @param error_msg As defined by #s_erc.
	 * @param func Calling function name.
	 * @param file Calling function file name.
	 * @param line Calling line in function.
	 * @param user_msg A format string specifying the string to write
	 * and the format of the arguments in the va_list.
	 * @param argp The va_list, see the standard function @c
	 * vprintf(). The value of argp is undefined after the call to
	 * this function.
	 *
	 * @return Pointer to formated string, or @c NULL on error.
	 */
	char *(*format)(s_log_event level, const char *error_msg,
					const char *func, const char *file, int line,
					const char *user_msg, va_list argp);

	/**
	 * Destroy the layout object's associated resources. The layout's
	 * memory itself must @b not be free'd. Must be implemented by
	 * each layout as it is called by #s_layout_destroy.
	 *
	 * @param layout This layout object.
	 *
	 * @return Error code.
	 */
	s_erc (*destroy)(s_layout *self);
};


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Format the given logging event information to a character string
 * buffer, with the given layout formatter. If the underlying
 * implementation of the layout formatter fails, then the logging
 * event info will be output to @c stdout in a simple format.
 *
 * @param layout The given layout formatter.
 * @param level The event level.
 * @param error_msg As defined by #s_erc.
 * @param func Calling function name.
 * @param file Calling function file name.
 * @param line Calling line in function.
 * @param user_msg A format string specifying the string to write and
 * the format of the variable length argument list. Same as the the
 * standard @c printf() function.
 *
 * @return Pointer to formated string, or @c NULL on error.
 *
 * @note Thread-safety is dependant on the underlying implementation.
 */
S_API char *s_layout_format(const s_layout *layout, s_log_event level,
							const char *error_msg, const char *func,
							const char *file, int line, const char *user_msg, ...);


/**
 * Format the given logging event information, with a vardiac list
 * pointer, to a character string buffer, with the given layout
 * formatter. If the underlying implementation of the layout formatter
 * fails, then the logging event info will be output to @c stdout in a
 * simple format.
 *
 * @param layout The given layout formatter.
 * @param level The event level.
 * @param error_msg As defined by #s_erc.
 * @param func Calling function name.
 * @param file Calling function file name.
 * @param line Calling line in function.
 * @param user_msg A format string specifying the string to write and
 * the format of the arguments in the va_list.
 * @param argp The va_list, see the standard function @c
 * vprintf(). The value  of argp is undefined after the call to this
 * function.
 *
 * @return Pointer to formated string, or @c NULL on error.
 *
 * @note Thread-safety is dependant on the underlying implementation.
 */
S_API char *s_layout_vformat(const s_layout *layout, s_log_event level,
							 const char *error_msg, const char *func, const char *file,
							 int line, const char *user_msg, va_list argp);


/**
 * Destroy the given layout's associated resources. The layout @b must
 * still be free'd with #S_FREE after this call.
 *
 * @param layout The layout object's resources to destroy.
 *
 * @return error Error code.
 *
 * @note Thread-safety is dependant on the underlying implementation.
 */
S_API s_erc s_layout_destroy(s_layout *layout);


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

#endif /* _SPCT_LAYOUT_H__ */

