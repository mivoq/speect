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
/* DATE    : 5 June 2009                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Error and debugging module init/quit and utils.                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_ERRDBG_IMPL_H__
#define _SPCT_ERRDBG_IMPL_H__


/**
 * @file errdbg_impl.h
 * Error and debugging module init/quit and utils.
 */

/**
 * @ingroup SErrDbg
 * @defgroup SErrDbgModule Utils
 * Error and debugging utilities.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
#include "base/log/log.h"


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
 * @private
 * Initialize the error and debugging system. The relevant resources
 * are allocated.
 *
 * @param err_logger The logger stream where errors and warnings are
 * logged.
 * @param dbg_logger The logger stream where debug messages are
 * logged. This can be the same logger as @a err_logger. If @c NULL
 * then no debug messages are logged.
 * @param layout The layout formatter.
 * @param level The initial debugging level. The debuggging level can
 * be changed with #s_errdbg_level.
 * @param error Error code.
 *
 * @note The Error and debugging module takes ownership of the layout
 * and the loggers. These resources are destroyed when the module is
 * quit with #_s_errdbg_quit.
 * @note Not thread safe.
 * @note The error and debug loggers can be the same logger.
 */
S_LOCAL void _s_errdbg_init(s_logger *err_logger, s_logger *dbg_logger,
							s_layout *layout, s_dbg_lvl level, s_erc *error);


/**
 * @private
 * Quit the error and debugging system. The relevant resources are
 * freed and the error and debugging streams are closed.
 *
 * @param error Error code.
 *
 * @note Not thread safe.
 */
S_LOCAL void _s_errdbg_quit(s_erc *error);


/**
 * Change the debugging level.
 *
 * @param level The new debugging level.
 * @param error Error code.
 *
 * @see #s_dbg_lvl
 */
S_API void s_errdbg_level(s_dbg_lvl level, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Private function prototypes (used by macros, see errdbg_macros.h)                */
/*                                                                                  */
/************************************************************************************/

/**
 * A dummy function, does nothing. Replaces #_s_err, #_s_fatal_err,
 * and #_s_check_err when error handling is turned off.
 * @private
 *
 * @note Even though it replaces above mentioned, it never gets called
 * because the macros become <c> if (0 && _s_err_dummy) </c>
 * which @e always evaluates to @c FALSE @e before calling @c _s_err_dummy
 */
S_API int _s_err_dummy(s_erc *error_code, s_erc this_error,
					   const char *function_name, const char *fmt, ...);


/**
 * A dummy warning function, does nothing. Replaces #_s_warn when
 * error handling is turned off.
 * @private
 *
 * @note Even though it replaces above mentioned, it never gets called
 * because the macros become <c> if (0 && _s_warn_dummy) </c>
 * which @e always evaluates to @c FALSE @e before calling @c _s_warn_dummy
 */
S_API int _s_warn_dummy(s_erc this_error, const char *function_name,
						const char *fmt, ...);


/**
 * @private
 */
S_API void (*_s_err(const char *file_name, int line))(s_erc *error_code,
													  s_erc this_error,
													  const char *function_name,
													  const char *fmt, ...);


/**
 * @private
 */
S_API void (*_s_fatal_err(const char *file_name, int line))(s_erc *error_code,
															s_erc this_error,
															const char *function_name,
															const char *fmt, ...);


/**
 * @private
 */
S_API s_bool (*_s_check_err(const char *file_name, int line))(s_erc *error_code,
															  s_erc this_error,
															  const char *function_name,
															  const char *fmt, ...);


/**
 * @private
 */
S_API void (*_s_warn(const char *file_name, int line))(s_erc this_error,
													   const char *function_name,
													   const char *fmt, ...);


/**
 * @private
 */
S_API void _s_dbg(s_dbg_lvl level, const char *fmt, ...);


/**
 * @}
 * end documentation
 */

#endif /* _SPCT_ERRDBG_IMPL_H__ */

