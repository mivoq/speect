/************************************************************************************/
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
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
/* DATE    : June 2012                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Windows specific string functions.                                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_STRING_WIN32_STRINGS_H__
#define _SPCT_STRING_WIN32_STRINGS_H__


/**
 * @file win32_strings.h
 * Windows specific string functions.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "windows.h"
#include "include/common.h"
#include "base/errdbg/errdbg.h"


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
 * Copy a UTF-8 string from a UTF-16 string.
 *
 * @param from UTF-16 string to copy from.
 * @param n Number of characters of @c from to copy. If -1
 * then all characters.
 * @param error Error code.
 *
 * @return Pointer to resulting UTF-8 string.
 *
 * @note Caller is responsible for returned memory.
 */
S_API char *s_utf8_from_utf16(const wchar_t* from, int n, s_erc *error);


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

#endif /* _SPCT_STRING_WIN32_STRINGS_H__ */
