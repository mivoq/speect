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
/* DATE    : 19 May 2008                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* UTF8 character functions.                                                        */
/*                                                                                  */
/* Adapted from Allegro http://alleg.sourceforge.net/  (4.9.7)                      */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_STRING_UTF8_CHAR_H__
#define _SPCT_STRING_UTF8_CHAR_H__


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
 * @file char.h
 * UTF8 character functions.
 */


/**
 * @ingroup SBaseStrings
 * @defgroup SUtf8Chars Character Functions
 * Provides UTF-8 aware character functions.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
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
 * @name Get
 * @{
 */


/**
 * Read the first character from a UTF-8 string.
 *
 * @param s String to read character from.
 * @param error Error code.
 *
 * @return UTF8 character represented as a 4-byte unsigned integer.
 */
S_API uint32 s_getc(const char *s, s_erc *error);


/**
 * Read the first character from a UTF-8 string, advancing the pointer
 * position.
 *
 * @param s String to read character from.
 * @param error Error code.
 *
 * @return UTF8 character represented as a 4-byte unsigned integer.
 *
 * @note The original pointer is lost if it is not saved somewhere else.
 */
S_API uint32 s_getx(char **s, s_erc *error);


/**
 * Return the character from the specified index within the
 * UTF-8 string. Indexing in strings start at 0.
 *
 * @param s The string.
 * @param index The index of the character to return.
 * @param error Error code.
 *
 * @return The UTF-8 character at @a index in @a s.
 */
S_API uint32 s_getat(const char *s, uint index, s_erc *error);


/**
 * @}
 */


/**
 * @name Set
 * @{
 */


/**
 * Set a character in a UTF-8 string and return the byte size
 * of the character.
 *
 * @param s The string.
 * @param c The UTF-8 character.
 * @param error Error code.
 *
 * @return The byte size of the character.
 */
S_API size_t s_setc(char *s, uint32 c, s_erc *error);


/**
 * Modify the character at the specified index within the UTF-8 string,
 * handling adjustments for variable width data. Returns how far the
 * rest of the string was moved in bytes. Indexing in strings start at
 * 0.
 *
 * @param s The string.
 * @param index The index of the character to modify.
 * @param c The new character.
 * @param error Error code.
 *
 * @return Number of bytes the string was moved.
 */
S_API size_t s_setat(char *s, uint index, uint32 c, s_erc *error);


/**
 * @}
 */


/**
 * @name Width
 * @{
 */


/**
 * Return the width (number of bytes) of the first UTF-8 character in
 * the given string.
 *
 * @param s The string.
 * @param error Error code.
 *
 * @return The byte width of the first character in the string.
 */
S_API size_t s_width(const char *s, s_erc *error);


/**
 * Return the width (number of bytes) of a UTF-8 character represented
 * by the given 4-byte unsigned integer.
 *
 * @param c The UTF-8 character.
 * @param error Error code.
 *
 * @return The byte width of the character.
 */
S_API size_t s_cwidth(uint32 c, s_erc *error);


/**
 * @}
 */


/**
 * @name Offset
 * @{
 */


/**
 * Return the offset in bytes from the start of the UTF-8 string to
 * the character at the specified index. If the index is negative,
 * counts backward from the end of the string (-1 returns an offset to
 * the last character).
 *
 * @param s The string.
 * @param index The index of the character.
 * @param error Error code.
 *
 * @return The byte offset of the character at @a index in @a s.
 */
S_API size_t s_offset(const char *s, int index, s_erc *error);


/**
 * @}
 */


/**
 * @name Insert/Remove
 * @{
 */


/**
 * Insert a character at the specified index within a UTF-8 string,
 * sliding following data along to make room. Returns how far the data
 * was moved in bytes.
 *
 * @param s The string. Indexing in strings start at 0.
 * @param index The index where to insert the new character.
 * @param c The new character.
 * @param error Error code.
 *
 * @return The amount of bytes the string was moved.
 *
 * @note The given string @c s must be big enough to take inserted
 * character, no checking is done.
 */
S_API size_t s_insert(char *s, uint index, uint32 c, s_erc *error);


/**
 * Remove the character at the specified index within the UTF-8
 * string, sliding following data back to make room. Returns how far
 * the data was moved in bytes.
 *
 * @param s The string. Indexing in strings start at 0.
 * @param index The index of the character to remove.
 * @param error Error code.
 *
 * @return The amount of bytes the string was moved.
 */
S_API size_t s_remove(char *s, uint index, s_erc *error);


/**
 * @}
 */


/**
 * @name Case conversion
 * @{
 */


/**
 * Convert the given UTF-8 character to lower case.
 *
 * @param c The character to convert.
 * @param error Error code.
 *
 * @return The lower case of @a c.
 */
S_API uint32 s_tolower(uint32 c, s_erc *error);


/**
 * Convert the given UTF-8 character to upper case.
 *
 * @param c The character to convert.
 * @param error Error code.
 *
 * @return The upper case of @a c.
 */
S_API uint32 s_toupper(uint32 c, s_erc *error);


/**
 * @}
 */


/**
 * @name Character type
 * @{
 */


/**
 * Check whether the given UTF-8 character is a white-space. White-space is
 * space (' '), form-feed ('\\f'), newline ('\\n'), carriage return
 * ('\\r'), horizontal tab ('\\t'), and vertical tab ('\\v').
 *
 * @param c The UTF-8 character to test.
 * @param error Error code.
 *
 * @return Non-zero if @a c is a white-space otherwise @a 0.
 */
S_API int s_isspace(uint32 c, s_erc *error);


/**
 * Check whether the given UTF-8 character is a digit.
 *
 * @param c The UTF-8 character to test.
 * @param error Error code.
 *
 * @return Non-zero if @a c is a digit otherwise @a 0.
 */
S_API int s_isdigit(uint32 c, s_erc *error);


/**
 * @}
 */


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

#endif /* _SPCT_STRING_UTF8_CHAR_H__ */

