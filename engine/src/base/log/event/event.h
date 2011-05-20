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
/* Logging event definitions.                                                       */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LOG_EVENT_H__
#define _SPCT_LOG_EVENT_H__


/**
 * @file event.h
 * Logging event definitions.
 */


/**
 * @ingroup SLogging
 * @defgroup SLogEvent Events
 * Logging event definitions. The log events defines the type of information
 * that will be output to the log.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"


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
 * Logging event level defintions. Follows log4j Log levels (http://en.wikipedia.org/wiki/Log4j).
 */
typedef enum
{
	/*!< Reserved. */
	S_RESERVED_EVENT = 0,

	/*!< Severe errors that cause premature termination. [FATAL] in layout. */
	S_FATAL_EVENT    = 1,

	/*!< Other runtime errors or unexpected conditions. [ERROR] in layout. */
	S_ERROR_EVENT    = 2,

	/*!< Undesirable/unexpected runtime situations, but not necessarily "wrong". [WARN] in layout.*/
	S_WARN_EVENT     = 3,

	/*!< Interesting runtime events (startup/shutdown). [INFO] in layout. */
	S_INFO_EVENT     = 4,

	/*!< Detailed information on the flow through the system. [DEBUG] in layout. */
	S_DEBUG_EVENT    = 5,

	/*!< More detailed information. [TRACE] in layout. */
	S_TRACE_EVENT    = 6
} s_log_event;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Get a string representation of the given log event level.
 *
 * @param level The given log event level.
 *
 * @return Pointer to string representation of given log event level.
 * If the error code is unknown then @a "UNKNOWN EVENT" is returned.
 *
 * @note Caller is responsible for returned charachter buffer's memory.
 * @note Thread-safe.
 */
S_API char *s_log_event_str(s_log_event level);


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

#endif /* _SPCT_LOG_EVENT_H__ */

