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
/* DATE    : 25 March 2008                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Speect configuration includes.                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_COMMON_H__
#define _SPCT_COMMON_H__


/**
 * @file include/common.h
 * A common header file included in all source code.
 */


/**
 * @ingroup Speect
 * @defgroup SDefines System Defines
 * Standard defines for the Speect Engine. Contains includes to other header files
 * that are part of the configuration. This includes the system configuration as
 * defined by the build system, system libraries, Speect Engine version information,
 * standard defintions and macros, exported symbol visibility macros, and the threading
 * implementation as defined during build time.
 */


/************************************************************************************/
/*                                                                                  */
/* Speect configuration includes                                                    */
/*                                                                                  */
/************************************************************************************/

#include "include/spctconfig.h"       /* Configuration as done by build system.     */
#include "include/syslibs.h"          /* System specific libraries.                 */
#include "include/version.h"          /* Speect version information.                */
#include "include/defs.h"             /* Some usefull macros and defines.           */
#include "include/visibility.h"       /* Symbols visibility.                        */
#include "../base/threads/threads.h"  /* Thread implementation.                     */


#endif /* _SPCT_COMMON_H__ */
