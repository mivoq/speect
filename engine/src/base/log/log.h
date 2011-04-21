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
/* Logging facilities.                                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LOG_H__
#define _SPCT_LOG_H__


/**
 * @file log.h
 * Logging facilities.
 */


/**
 * @ingroup SBase
 * @defgroup SLogging Logging
 * Loggin facilities.
 * The logging system provides a mechanism to write @a formatted
 * messages to different types of streams. It is used by all the error
 * and debug reporting facilites in @ref SErrDbg, and therefore aims
 * to be as robust as possible. If any error occurs during one of the
 * logging functions, an error will be printed to @c stderr, and the
 * logging system will attempt to write the message to @c stdout.
 *
 * The logging system consists of three components:
 * <ul>
 * <li> @ref SLogEvent
 *
 * Logging events describe the severity of the log message. The event
 * level can also have an influence on the format of the message
 * output.
 *
 * </li>
 * <li> @ref SLayout
 *
 * The layout is the format of the log message. The format is
 * influenced by the log event level and the information supplied to
 * the underlying layout formatter.
 *
 * </li>
 * <li> @ref SLogger
 *
 * The logger ouputs the log message to a stream.
 *
 * </li>
 * </ul>
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/log/event/event.h"     /* Logging events include.      */
#include "base/log/layout/layouts.h"  /* Logging layouts include.     */
#include "base/log/streams/streams.h" /* Logging streams include.     */
#include "base/log/logger.h"          /* Logging include.             */


#endif /* _SPCT_LOG_H__ */

