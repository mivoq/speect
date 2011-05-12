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
/* Error and debugging macros.                                                      */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_ERRDBG_MACROS_H__
#define _SPCT_ERRDBG_MACROS_H__


/**
 * @file errdbg_macros.h
 * Error and debugging macros.
 */


/**
 * @ingroup SErrDbg
 * @defgroup SErrDbgMacros Macros
 * Error and debugging macros definitions. Detailed description of macros:
 */


/**
 * @example errdbg_example.c
 * Error and debugging examples.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdlib.h>  /* for abort */
#include <stdio.h>
#include "include/common.h"
#include "base/utils/alloc.h"

/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @ingroup SErrDbgMacros
 * @hideinitializer
 * @def S_NEW_ERR
 * Set a new error with no context. Set @a ERROR to the error code
 * that occured, @a ERROR_CODE. This macro is useful in deeply nested
 * functions where there is not enough context to add, i.e. it is only
 * known that an error of @a ERROR_CODE type occured, but not why it
 * occurred.
 *
 * @param ERROR Pointer to error code variable to set.
 * @param ERROR_CODE The new error code.
 */
#ifdef SPCT_DOXYGEN_ONLY
#  define S_NEW_ERR(ERROR, ERROR_CODE)
#else /* !SPCT_DOXYGEN_ONLY */
#  ifdef SPCT_ERROR_HANDLING
#    define S_NEW_ERR(ERROR, ERROR_CODE)				\
	do {												\
		if ((ERROR) != NULL)							\
			(*(ERROR)) = ERROR_CODE;					\
	} while(0)
#  else /* ! SPCT_ERROR_HANDLING */
#    define S_NEW_ERR(ERROR, ERROR_CODE) ((void) 0)
#  endif /* SPCT_ERROR_HANDLING */
#endif /* SPCT_DOXYGEN_ONLY */


/**
 * @ingroup SErrDbgMacros
 * @hideinitializer
 * @def S_CLR_ERR
 * Clear the given error. It is good practice to clear the error code
 * (*@a ERROR = #S_SUCCESS) at the beginning of all functions.
 *
 * @param ERROR Pointer to error code to clear.
 */
#ifdef SPCT_DOXYGEN_ONLY
#  define S_CLR_ERR(ERROR)
#else /* !SPCT_DOXYGEN_ONLY */
#  ifdef SPCT_ERROR_HANDLING
#    define S_CLR_ERR(ERROR)			\
	do {								\
		if ((ERROR) != NULL)			\
			(*(ERROR)) = S_SUCCESS;		\
	} while(0)
#  else /* ! SPCT_ERROR_HANDLING */
#    define S_CLR_ERR(ERROR) ((void) 0)
#  endif /* SPCT_ERROR_HANDLING */
#endif /* SPCT_DOXYGEN_ONLY */


/**
 * @ingroup SErrDbgMacros
 * @hideinitializer
 * @def S_CTX_ERR
 * Set a new error with a context. Sets @a ERROR to the error code
 * that occured, @a NEW_ERROR, as well as giving the error logger the
 * function name and a variable length argument string of the context
 * wherein this error occurred.
 *
 * @param ERROR Pointer to error code variable to set.
 * @param NEW_ERROR The new error code.
 * @param FUNCTION_NAME The name of the function where the error
 *        occurred (optional, can be @c NULL).
 * @param MSG A format string specifying the error message and the
 *        format of the variable length argument list. Same as the
 *        standard @c printf() function.
 */
#ifdef SPCT_DOXYGEN_ONLY
#  define S_CTX_ERR(ERROR, NEW_ERROR, FUNCTION_NAME, MSG, ...)
#else /* !SPCT_DOXYGEN_ONLY */
#  ifdef SPCT_ERROR_HANDLING
#    define S_CTX_ERR (*_s_err(__FILE__, __LINE__))
#  else /* ! SPCT_ERROR_HANDLING */
#    define S_CTX_ERR 1 ? 0 : _s_err_dummy
#  endif /* SPCT_ERROR_HANDLING */
#endif /* SPCT_DOXYGEN_ONLY */


/**
 * @ingroup SErrDbgMacros
 * @hideinitializer
 * @def S_FTL_ERR
 * Set a new @b fatal context error. Sets @a ERROR to the fatal error
 * code that occured, @a NEW_ERROR, as well as giving the error logger
 * the function name and a variable length argument string of the
 * context wherein this error occurred. The same as #S_CTX_ERR except
 * that if the build option @b SPCT_ERROR_ABORT_FATAL is set then the
 * program is aborted.
 *
 * @param ERROR Pointer to error code variable to set.
 * @param NEW_ERROR The new error code.
 * @param FUNCTION_NAME The name of the function where the error
 *        occurred (optional, can be @c NULL).
 * @param MSG A format string specifying the error message and the
 *        format of the variable length argument list. Same as the
 *        standard @c printf() function.
 */
#ifdef SPCT_DOXYGEN_ONLY
#  define S_FTL_ERR(ERROR, NEW_ERROR, FUNCTION_NAME, MSG, ...)
#else /* !SPCT_DOXYGEN_ONLY */
#  ifdef SPCT_ERROR_ABORT_FATAL
#    define S_FTL_ERR (*_s_fatal_err(__FILE__, __LINE__))
#  else /* !S_ERROR_ABORT_FATAL */
#    define S_FTL_ERR (*_s_err(__FILE__, __LINE__))
#  endif /* S_ERROR_ABORT_FATAL */
#endif /* SPCT_DOXYGEN_ONLY */


/**
 * @ingroup SErrDbgMacros
 * @hideinitializer
 * @def S_CHK_ERR
 * Check if an error has been set. Test if an error has occured (if @a ERROR is
 * set, i.e. not equal to #S_SUCCESS), and if so set a new context
 * error (same as #S_CTX_ERR), and return #TRUE. If no error was
 * set, (*@a ERROR = #S_SUCCESS) then nothing is done and #FALSE is
 * returned. This macro can be used in @c if statements directly after
 * calling a function with an error code parameter, testing if an
 * error occured, setting a new error context and executing the @c if
 * statement. If no error occured then no new context is set and the
 * @c if statement is not executed.
 *
 * @param ERROR Pointer to error code variable to set.
 * @param NEW_ERROR The new error code.
 * @param FUNCTION_NAME The name of the function where the error
 *        occurred (optional, can be @c NULL).
 * @param MSG A format string specifying the error message and the
 *        format of the variable length argument list. Same as the
 *        standard @c printf() function.
 */
#ifdef SPCT_DOXYGEN_ONLY
#  define S_CHK_ERR(ERROR, NEW_ERROR, FUNCTION_NAME, MSG, ...)
#else /* !SPCT_DOXYGEN_ONLY */
#  ifdef SPCT_ERROR_HANDLING
#    define S_CHK_ERR (*_s_check_err(__FILE__, __LINE__))
#  else /* ! SPCT_ERROR_HANDLING */
#    define S_CHK_ERR 1 ? 0 : _s_err_dummy
#  endif /* SPCT_ERROR_HANDLING */
#endif /* SPCT_DOXYGEN_ONLY */


/**
 * @ingroup SErrDbgMacros
 * @hideinitializer
 * @def S_WARNING
 * Macro that sets a warning message.  Set a warning with the given
 * context. The warning is logged with the given context and message.
 *
 * @param ERROR Pointer to error code variable to set.
 * @param FUNCTION_NAME The name of the function where the error
 *        occurred (optional, can be @c NULL).
 * @param MSG A format string specifying the error message and the
 *        format of the variable length argument list. Same as the
 *        standard @c printf() function.
 */
#ifdef SPCT_DOXYGEN_ONLY
#  define S_WARNING(ERROR, FUNCTION_NAME, MSG, ...)
#else /* !SPCT_DOXYGEN_ONLY */
#  ifdef SPCT_ERROR_HANDLING
#    define S_WARNING (*_s_warn(__FILE__, __LINE__))
#  else /* ! SPCT_ERROR_HANDLING */
#    define S_WARNING 1 ? 0 : _s_warn_dummy
#  endif /* SPCT_ERROR_HANDLING */
#endif /* SPCT_DOXYGEN_ONLY */


/**
 * @ingroup SErrDbgMacros
 * @hideinitializer
 * @def S_DEBUG
 * Macro that sets a debug message. The debug message is logged only
 * if the the given debug level is equal or lower that  the set level
 * of the @ref SErrDbg module. This level can be changed with the
 * #s_errdbg_level function.
 *
 * @param LEVEL The debug level (#s_dbg_lvl) of this message.
 * @param MSG A format string specifying the error message and the
 *        format of the variable length argument list. Same as the
 *        standard @c printf() function.
 */
/* SPCT_DEBUGMODE is a compiler switch, -DSPCT_DEBUGMODE=1 */
#ifdef SPCT_DOXYGEN_ONLY
#  define S_DEBUG(LEVEL, MSG, ...)
#else
#  ifdef SPCT_DEBUGMODE
#    define S_DEBUG _s_dbg
#  else /* !SPCT_DEBUGMODE */
#    define S_DEBUG 1 ? 0 : _s_dbg
#  endif /* SPCT_DEBUGMODE */
#endif /* SPCT_DOXYGEN_ONLY */


/************************************************************************************/
/*                                                                                  */
/* Private macros (for internal use only)                                           */
/*                                                                                  */
/************************************************************************************/

/*
 * Used internally by Error and debug module, Logging module
 * for errors that occur during their initialization.
 *
 * -------------------------------------
 *
 * S_ERR_PRINT(ERR, FUNC, MSG)
 *
 * Print an error to stderr.
 * ERR The error, of type s_erc.
 * FUNC String function name in which the error occured.
 * MSG String error message.
 *
 * -------------------------------------
 *
 * S_FTL_ERR_PRINT(ERR, FUNC, MSG)
 *
 * Print a fatal error to stderr and abort.
 * ERR The error, of type s_erc.
 * FUNC String function name in which the error occured.
 * MSG String error message.
 *
 * -------------------------------------
 *
 */

#ifdef SPCT_USE_THREADS
#  define S_ERR_PRINT(ERR, FUNC, MSG)					\
	do {								\
	char *err_str = s_error_str(ERR);				\
	fprintf(stderr, "[ERROR (%s) %lu] %s (in function '%s', %s, %d)\n", \
		err_str, s_thread_id(), MSG, FUNC, __FILE__, __LINE__);	\
	S_FREE(err_str);						\
	} while (0)
#else /* !SPCT_USE_THREADS */
#  define S_ERR_PRINT(ERR, FUNC, MSG)				\
	do {								\
	char *err_str = s_error_str(ERR);				\
	fprintf(stderr, "[ERROR (%s)] %s (in function '%s', %s, %d)\n",	\
		err_str, MSG, FUNC, __FILE__, __LINE__);		\
	S_FREE(err_str);						\
	} while (0)
#endif /* SPCT_USE_THREADS */


#ifdef SPCT_ERROR_ABORT_FATAL
#  ifdef SPCT_USE_THREADS
#    define S_FTL_ERR_PRINT(ERR, FUNC, MSG)				\
	do {								\
		char *err_str = s_error_str(ERR);			\
		fprintf(stderr, "[FATAL (%s) %lu] %s (in function '%s', %s, %d)\n", \
			err_str, s_thread_id(), MSG, FUNC, __FILE__, __LINE__); \
		S_FREE(err_str);					\
		abort();						\
	} while (0)
#  else /* !SPCT_USE_THREADS */
#    define S_FTL_ERR_PRINT(ERR, FUNC, MSG)				\
	do {								\
		char *err_str = s_error_str(ERR);			\
		fprintf(stderr, "[FATAL ERROR (%s)] %s (in function '%s', %s, %d)\n", \
			err_str, MSG, FUNC, __FILE__, __LINE__);	\
		S_FREE(err_str);					\
		abort();						\
	} while (0)
#  endif /* SPCT_USE_THREADS */
#else /* !SPCT_ERROR_ABORT_FATAL */
#  ifdef  SPCT_USE_THREADS
#    define S_FTL_ERR_PRINT(ERR, FUNC, MSG) S_ERR_PRINT(ERR, FUNC, MSG)
#  else /* !SPCT_USE_THREADS */
#    define S_FTL_ERR_PRINT(ERR, FUNC, MSG) S_ERR_PRINT(ERR, FUNC, MSG)
#  endif /* SPCT_USE_THREADS */
#endif /* SPCT_ERROR_ABORT_FATAL */


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


#endif /* _SPCT_ERRDBG_MACROS_H__ */
