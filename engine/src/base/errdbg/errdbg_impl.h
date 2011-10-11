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
 * @param logger The logger stream where errors, warnings and debug
 * messages are logged.
 * @param level The initial debugging level. The debugging level can
 * be changed with #s_set_errdbg_level.
 * @param error Error code.
 *
 * @note The Error and debugging module takes ownership of the
 * logger. These resources are destroyed when the module is
 * quit with @c _s_errdbg_quit.
 * @note Not thread safe.
 */
S_LOCAL void _s_errdbg_init(s_logger *logger, s_dbg_lvl level, s_erc *error);


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
 * Set the error and debugging system logger to the given one. The
 * current logger (if any) will be freed. This is useful when the
 * logger needs to be changed after Speect has started up.
 *
 * @param logger The new logger for the error and debugging system
 */
S_API void s_set_errdbg_logger(s_logger *logger);


/**
 * Query whether the <i>Error and Debugging System</i> is activated or
 * not. The system can be activated/deactivate with the compile time
 * definition of @c SPCT_ERROR_HANDLING. If deactivated then no
 * logging or checking of error codes will occur.
 *
 * @return #TRUE if activated, else #FALSE.
 */
S_API s_bool s_errdbg_on(void);


/**
 * Change the debugging level.
 *
 * @param level The new debugging level.
 * @param error Error code.
 *
 * @see #s_dbg_lvl
 */
S_API void s_set_errdbg_level(s_dbg_lvl level, s_erc *error);


/**
 * Get the debugging level.
 *
 * @param error Error code.
 *
 * @return The debugging level.
 *
 * @see #s_dbg_lvl
 */
S_API s_dbg_lvl s_get_errdbg_level(s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Private function prototypes (used by macros, see errdbg_macros.h)                */
/*                                                                                  */
/************************************************************************************/

/**
 * A dummy function, does nothing. Replaces @c _s_check_err
 * in @c S_CHK_ERR when error handling is turned off.
 * @private
 *
 * @note We need this function to gobble up the macros parameters (no
 * vardiac macros like C99).
 */
S_API int _s_err_dummy(s_erc *error_code, s_erc this_error,
					   const char *function_name, const char *fmt, ...);


/**
 * A dummy function, does nothing. Replaces @c _s_err in @c S_CTX_ERR
 * when error handling is turned off. Also replaces @c _s_err
 * in @c S_FTL_ERR when no error checking is done and aborting on
 * error (@c SPCT_ERROR_ABORT_FATAL) is not set.
 * @private
 *
 * A dummy function, does nothing. Replaces @c _s_err in @c S_CTX_ERR
 * when error handling is turned off.
 * @private
 *
 * @note We need this function to gobble up the macros parameters (no
 * vardiac macros like C99).
 */
S_API void _s_err_dummy_void(s_erc *error_code, s_erc this_error,
							 const char *function_name, const char *fmt, ...);


/**
 * A dummy warning function, does nothing. Replaces @c _s_warn
 * in @c S_WARNING when error handling is turned off.
 * @private
 *
 * @note We need this function to gobble up the macros parameters (no
 * vardiac macros like C99).
 */
S_API void _s_warn_dummy(s_erc this_error, const char *function_name,
						 const char *fmt, ...);


/**
 * A dummy debug function, does nothing. Replaces @c _s_dbg
 * in @c S_DEBUG when debug mode is turned off.
 * @private
 *
 * @note We need this function to gobble up the macros parameters (no
 * vardiac macros like C99).
 */
S_API void _s_debug_dummy(s_dbg_lvl level, const char *fmt, ...);


/**
 * A function to abort when error checking is off and aborting on
 * error (@c SPCT_ERROR_ABORT_FATAL) is set. Used in @c S_FTL_ERR
 * @private
 *
 * @note We need this function to gobble up the macros parameters (no
 * vardiac macros like C99).
 */
S_API void _s_fatal_err_no_checking(s_erc *error_code, s_erc this_error,
									const char *function_name, const char *fmt, ...);


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

