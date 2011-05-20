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
/* Windows type definitions.                                                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_WIN32_TYPES_H__
#define _SPCT_WIN32_TYPES_H__


/**
 * @file win32_types.h
 * Windows type definitions.
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

typedef unsigned __int8   s_bool;

typedef int               sint;

typedef unsigned int      uint;

typedef __int8            sint8;

typedef unsigned __int8   uint8;

typedef __int16	          sint16;

typedef unsigned __int16  uint16;

typedef __int32	          sint32;

typedef unsigned __int32  uint32;

typedef long int          slong;

typedef unsigned long int ulong;

typedef char              schar;

typedef unsigned char     uchar;

typedef uint8             s_byte;


/***********************************************************************************/
/*                                                                                 */
/* End external c declaration                                                      */
/*                                                                                 */
/***********************************************************************************/
S_END_C_DECLS


#endif /* _SPCT_WIN32_TYPES_H__ */

