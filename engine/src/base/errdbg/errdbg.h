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
/* Error handling and debugging facilities.                                         */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_ERRDBG_H__
#define _SPCT_ERRDBG_H__


/**
 * @file errdbg.h
 * Error handling and debugging facilities.
 */

/**
 * @ingroup SBase
 * @defgroup SErrDbg Error handling and Debugging
 * Error handling and debugging module, utilities, macros and defintions.
 *
 * The error handling mechanism works on the basis that @a all functions are passed
 * a pointer to an error code variable as a function parameter. If an error occured
 * during function execution, then the function can set an appropriate error code in this
 * pointer (there are exceptions to this rule, i.e. @ref SThreads and @ref SLogging).
 *
 * All calling functions can then check the error pointer and take appropriate action based
 * on the error code. Each calling function can then add context to the error and in turn
 * set an error code for it's calling function again, thereby giving a detailed trace from
 * the origin of the error to the source of the error. All errors are logged by the @ref SLogging
 * system.
 *
 * There are two types of errors, @a fatal errors and @a non-fatal errors. Fatal errors
 * are usually programming errors, memory errors and so forth, where there are no alternative
 * options if such an error occured. Non-fatal errors are errors that occured from which
 * the system can recover or reset to a previous state and continue.
 *
 * The error handling system as described enables one to have a detailed "backtrace" that has
 * been logged to some or other place, and therefore makes development and bug tracking a much easier
 * task.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"         /* Top level error and debugging include.   */
#include "base/errdbg/errdbg_defs.h"    /* Error and debugging defintions.          */
#include "base/errdbg/errdbg_macros.h"  /* Error and debugging macros.              */
#include "base/errdbg/errdbg_impl.h"    /* Error and debugging implementation.      */
#include "base/errdbg/errdbg_utils.h"   /* Error and debugging utilities.           */


#endif /* _SPCT_ERRDBG_H__ */
