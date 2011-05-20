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
/* DATE    : 25 March 2008                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/*  Base system utilities include file.                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_BASE_UTILS_H__
#define _SPCT_BASE_UTILS_H__


/**
 * @file utils.h
 * Base system utilities include and initialization.
 */


/**
 * @ingroup SBase
 * @defgroup SBaseUtils Utilities
 * Miscellaneous base functions and utility definitions.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/*  Include files in base utilities                                                 */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/alloc.h"       /* Memory allocation and deallocation. */
#include "base/utils/types.h"       /* Defines basic data types.           */
#include "base/utils/vernum.h"      /* Definiton of version data types.    */
#include "base/utils/math.h"        /* Basic math functions and constants. */
#include "base/utils/byteswap.h"    /* Endian byteswapping functions.      */
#include "base/utils/time.h"        /* Time functions and definitions.     */
#include "base/utils/path.h"        /* File system path functions.         */


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/*  Function prototypes                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize all the modules in the base utilities.
 * @private
 *
 * @param error Error code.
 *
 * @note Dependant on the @ref SErrDbg module being intialized.
 */
S_LOCAL void _s_base_utils_init(s_erc *error);


/**
 * Quit and free up resources of all the modules in the base
 * utilities.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_base_utils_quit(s_erc *error);


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

#endif /* _SPCT_BASE_UTILS_H__ */

