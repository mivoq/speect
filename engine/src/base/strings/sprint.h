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
/* DATE    : 19 May 2008                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* UTF8 string printing functions.                                                  */
/*                                                                                  */
/* Adapted from Allegro http://alleg.sourceforge.net/  (4.9.7)                      */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_STRING_UTF8_SPRINT_H__
#define _SPCT_STRING_UTF8_SPRINT_H__


/************************************************************************************/
/*                                                                                  */
/*  Copyright (c) 2008 the Allegro 5 Development Team                               */
/*                                                                                  */
/*  This software is provided 'as-is', without any express or implied warranty.     */
/*  In no event will the authors be held liable for any damages arising from the    */
/*  use of this software.                                                           */
/*                                                                                  */
/*  Permission is granted to anyone to use this software for any purpose,           */
/*  including commercial applications, and to alter it and redistribute it          */
/*  freely, subject to the following restrictions:                                  */
/*                                                                                  */
/*  1. The origin of this software must not be misrepresented; you must not         */
/*     claim that you wrote the original software. If you use this software         */
/*     in a product, an acknowledgment in the product documentation would be        */
/*     appreciated but is not required.                                             */
/*                                                                                  */
/*  2. Altered source versions must be plainly marked as such, and must not be      */
/*     misrepresented as being the original software.                               */
/*                                                                                  */
/*  3. This notice may not be removed or altered from any source distribution.      */
/*                                                                                  */
/************************************************************************************/


/**
 * @file sprint.h
 * UTF8 string printing functions.
 */


/**
 * @ingroup SBaseStrings
 * @defgroup SUtf8Sprint String Printing Functions
 * Provides utf-8 aware string print functions.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdarg.h>
#include "include/common.h"
#include "base/strings/char.h"
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
 * Print a variable argument list to a string buffer. This function
 * can handle the size of the buffer.
 *
 * @param buf The buffer to print to.
 * @param count Not more than @a count characters will be printed to
 * the buffer.
 * @param format The format of the output.
 * @param args The variable argument list.
 * @param error Error code.
 *
 * @return The number of characters printed.
 */
S_API int s_vszprintf(char *buf, size_t count, const char *format,
					  va_list args, s_erc *error);


/**
 * @hideinitializer
 * Print a variable argument list to a string buffer. This function
 * does not handle the size of the buffer.
 *
 * @param buf The buffer to print to.
 * @param format The format of the output.
 * @param args The variable argument list.
 * @param error Error code.
 *
 * @return The number of characters printed.
 *
 * @sa s_vszprintf
 */
#define s_vsprintf(buf, format, args, error)		\
	s_vszprintf(buf, INT_MAX, format, args, error)


/**
 * Print a formatted string to a buffer. Allocates memory to
 * buffer as required. Uses #s_vsprintf internally.
 *
 * @param ret The buffer to print to.
 * @param format The format of the output.
 * @param args The variable argument list.
 * @param error Error code.
 *
 * @return The number of characters printed.
 *
 * @note This function is slower than #s_vsprintf.
 */
S_API int s_vasprintf(char **ret, const char *format, va_list args, s_erc *error);


/**
 * Print a formatted string to a buffer. This function does not handle
 * the size of the buffer. Uses #s_vsprintf internally.
 *
 * @param buf The buffer to print to.
 * @param error Error code.
 * @param format The format of the output.
 *
 * @return The number of characters printed.
 */
S_API int s_sprintf(char *buf, s_erc *error, const char *format, ...);


/**
 * Print a formatted string to a buffer. This function can handle
 * the size of the buffer. Uses #s_vszprintf internally.
 *
 * @param buf The buffer to print to.
 * @param format The format of the output.
 * @param error Error code.
 * @param count Not more than @a count characters will be printed to
 * the buffer.
 *
 * @return The number of characters printed.
 */
S_API int s_szprintf(char *buf, size_t count, s_erc *error, const char *format, ...);


/**
 * Print a formatted string to a buffer. Allocates memory to
 * buffer as required. Uses #s_vasprintf internally.
 *
 * @param ret The buffer to print to.
 * @param error Error code.
 * @param fmt The formatted output.
 *
 * @return The number of characters printed.
 *
 * @note This function is slower than #s_sprintf.
 */
S_API int s_asprintf(char **ret, s_erc *error, const char *fmt, ...);


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

#endif /* _SPCT_STRING_UTF8_SPRINT_H__ */

