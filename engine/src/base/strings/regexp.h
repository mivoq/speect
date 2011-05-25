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
/* Utf8 aware regular expressions.                                                  */
/*                                                                                  */
/* Adapted from Unix port of the Plan 9 regular expression library.                 */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_REGEXP_H__
#define _SPCT_REGEXP_H__


/************************************************************************************/
/*                                                                                  */
/* Unix port of the Plan 9 regular expression library license                       */
/*                                                                                  */
/*                                                                                  */
/* The authors of this software is Rob Pike.                                        */
/*		Copyright (c) 2002 by Lucent Technologies.                                  */
/* Permission to use, copy, modify, and distribute this software for any            */
/* purpose without fee is hereby granted, provided that this entire notice          */
/* is included in all copies of any software which is or includes a copy            */
/* or modification of this software and in all copies of the supporting             */
/* documentation for such software.                                                 */
/* THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED          */
/* WARRANTY.  IN PARTICULAR, NEITHER THE AUTHORS NOR LUCENT TECHNOLOGIES MAKE ANY   */
/* REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY            */
/* OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.                      */
/*                                                                                  */
/************************************************************************************/


/**
 * @file regexp.h
 * UTF8 regular expression functions.
 */


/**
 * @ingroup SBaseStrings
 * @defgroup SUtf8Regex Regular Expressions
 * Provides utf-8 aware regular expressions.
 * The Speect regular expression syntax is a bit different from the
 * POSIX or Perl syntax's. The supported meta-character (operators) are
 * mostly the same, that is: <tt> . | ( ) [ ] ? + * ^ $ </tt>
 *
 *
 * Escaping (literal character inclusion) is supported using the
 * <tt>\\</tt>-character. POSIX character classes are not supported.
 * Unlike the POSIX or Perl variants, the Speect regular expression
 * engine always match the whole string, not any part of it. That is,
 * regular expression "a" matches the string "a", but not the string
 * "blah", whereas a POSIX or Perl regular expression would match both
 * strings. To get the latter behaviour, simply add ".*" before and
 * after the string, i.e. ".*a.*".
 *
 *
 * The regular expressions are matched using a purely NFA
 * (nondeterministic finite automaton) based approach. No backtracking
 * algorithm is provided.
 *
 *
 * @section SUtf8RegexSyntax Syntax Reference
 *
 * <table>
 *  <tr>
 *   <td> @b Metacharacter </td>
 *   <td> @b Description </td>
 *  </tr>
 *  <tr>
 *   <td> @c . </td>
 *   <td> Matches any single character. </td>
 *  </tr>
 *  <tr>
 *   <td> @c | </td>
 *   <td> The alternation operator matches either the expression
 * before or the expression after the operator. For example, abc|def
 * matches "abc" or "def". </td>
 *  </tr>
 *  <tr>
 *   <td> <tt>( )</tt> </td>
 *   <td> Defines a marked group. </td>
 *  </tr>
 *  <tr>
 *   <td> <tt>[ ]</tt> </td>
 *   <td> A bracket expression. Matches a single character that is
 * contained within the brackets. For example, [abc] matches "a", "b",
 * or "c". [a-z] specifies a range which matches any lowercase letter
 * from "a" to "z". These forms can be mixed: [abcx-z0-9] matches "a",
 * "b", "c", "x", "y", "z" or single digits "0" to "9".</td>
 *  </tr>
 *  <tr>
 *   <td> <tt>[^ ]</tt> </td>
 *   <td> Matches a single character that is not contained within the
 * brackets. For example, [^abc] matches any character other than "a",
 * "b", or "c". [^a-z] matches any single character that is not a
 * lowercase letter from "a" to "z". </td>
 *  </tr>
 *  <tr>
 *   <td> @c ? </td>
 *   <td> Matches the preceding element zero or one time. </td>
 *  </tr>
 *  <tr>
 *   <td> @c + </td>
 *   <td> Matches the preceding element one or more times. </td>
 *  </tr>
 *  <tr>
 *   <td> @c * </td>
 *   <td> Matches the preceding element zero or more times. </td>
 *  </tr>
 *  <tr>
 *   <td> @c ^ </td>
 *   <td> Matches the starting position within the string. </td>
 *  </tr>
 *  <tr>
 *   <td> @c $ </td>
 *   <td> Matches the ending position of the string. </td>
 *  </tr>
 * </table>
 *
 * @{
 */


/**
 * @example regexp_example.c
 * Regular expressions examples.
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
/* Typedefs and Data types                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * Type definition of the (opaque) compiled regular expression.
 */
typedef struct s_regex s_regex;


/**
 * Type definition of the (opaque) sub-expression elements of a
 * matched regular expression.
 */
typedef struct s_regexsub s_regexsub;


/**
 * Regular expression flags.
 */
typedef enum
{
	S_DOT_INCLD_NEWLINE = 0,  /*!< "." Metacharacter includes newlines.  */
	S_DOT_EXCLD_NEWLINE = 1   /*!< "." Metacharacter excludes newlines.  */
} s_regex_flags;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Compile a UTF-8 regular expression and return a pointer to the generated
 * description.
 *
 * @param string String containing the regular expression to compile.
 * @param flags Regular expression flags.
 * @param error Error code.
 *
 * @return Compiled regular expression.
 */
S_API s_regex *s_regex_comp(const char *string, s_regex_flags flags, s_erc *error);



/**
 * Matches a null-terminated UTF-8 string against the given compiled regular
 * expression in @c rxcomp. If it matches, and the sub-expression
 * elements structure @c rsub is not #NULL, @c rsub will be
 * filled with character pointers to the groups of strings that
 * correspond to the parenthesized sub-expressions of the expression.
 *
 * @param rxcomp The compiled regular expression.
 * @param string String to run regular expression on.
 * @param rsub Sub-expression elements, or #NULL.
 * @param error Error code.
 *
 * @retval 0 if no match
 * @retval > 0 if a match
 * @retval < 0 if we ran out of space
 */
S_API int s_regex_match(s_regex *rxcomp, const char *string,
						s_regexsub **rsub, s_erc *error);


/**
 * Query the number of groups that correspond to the parenthesized
 * sub-expressions of the given matched regular expression.
 *
 * @param rsub Sub-expression elements of a matched regular expression.
 * @param error Error code.
 *
 * @return The number of sub-string matches.
 */
S_API uint8 s_regexsub_num_groups(s_regexsub *rsub, s_erc *error);


/**
 * Extract the given numbered group of the given matched regular
 * expression sub-expression elements.
 *
 * @param rsub Sub-expression elements of a matched regular expression.
 * @param n The group number to extract.
 * @param error Error code.
 *
 * @return Character string of the given numbered group, can be #NULL.
 *
 * @note Caller is responsible for memory of returned string.
 * @note Group @c 0 is always the whole match.
 */
S_API char *s_regexsub_group(s_regexsub *rsub, uint8 n, s_erc *error);


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

#endif /* _SPCT_REGEXP_H__ */
