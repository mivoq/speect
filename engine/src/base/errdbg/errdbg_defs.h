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
/* Error code and debugging level definitions.                                      */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_ERRDBG_DEFS_H__
#define _SPCT_ERRDBG_DEFS_H__


/**
 * @file errdbg_defs.h
 * Error code and debugging level definitions.
 */


/**
 * @ingroup SErrDbg
 * @defgroup SErrDbgDefs Definitions
 * Error code and debugging level definitions.
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
/* Typedefs and data types                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * Definitions of error return codes.
 */
typedef enum
{
	S_SUCCESS =        0,     /*!< Success, no errors.                       */

	S_FAILURE =       -1,     /*!< An unspecified failure occurred.          */

	/* memory */
	S_MEMERROR =      -2,     /*!< A memory allocation error.                */

	/* function arguments */
	S_ARGERROR =      -3,     /*!< Error in function arguments.              */

	/* class/object method errors */
	S_METHINVLD =     -4,     /*!< Class/Object method does not exist.       */
	S_METHFAIL =      -5,     /*!< Method/function call failed.              */

	/* end of file/stream */
	S_IOEOF =         -6,     /*!< End of file/stream.                       */

	/* warnings */
	S_WARNERR =       -7,     /*!< Warning, possible error.                  */

	/* continue error context (must be last one in list) */
	S_CONTERR =       -8      /*!< Error context continued.                  */
} s_erc;



/**
 * Debug level definitions. If the debug level is set to @c S_DBG_TRACE then
 * all debug messages with a debug level equal and lower will be output
 * to the @ref SLogging system.
 * @sa s_log_event.
 */
typedef enum
{
	S_DBG_NONE    = 0,  /*!< No debugging output.    */
	S_DBG_INFO    = 1,  /*!< Basic debug level.      */
	S_DBG_TRACE   = 2,  /*!< Verbose debug level.    */
	S_DBG_ALL     = 3   /*!< All debugging logged.   */
} s_dbg_lvl;


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

#endif /* _SPCT_ERRDBG_DEFS_H__ */
