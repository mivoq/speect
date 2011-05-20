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
/* DATE    : 9 November 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Initialization file parser.                                                      */
/* Based on iniParser version 3.0b (beta), http://ndevilla.free.fr/iniparser/       */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_INIPARSER_H__
#define _SPCT_INIPARSER_H__


/************************************************************************************/
/* iniParser license.                                                               */
/*                                                                                  */
/* Copyright (c) 2000-2007 by Nicolas Devillard.                                    */
/* MIT License                                                                      */
/*                                                                                  */
/* Permission is hereby granted, free of charge, to any person obtaining a          */
/* copy of this software and associated documentation files (the "Software"),       */
/* to deal in the Software without restriction, including without limitation        */
/* the rights to use, copy, modify, merge, publish, distribute, sublicense,         */
/* and/or sell copies of the Software, and to permit persons to whom the            */
/* Software is furnished to do so, subject to the following conditions:             */
/*                                                                                  */
/* The above copyright notice and this permission notice shall be included in       */
/* all copies or substantial portions of the Software.                              */
/*                                                                                  */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING          */
/* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER              */
/* DEALINGS IN THE SOFTWARE.                                                        */
/*                                                                                  */
/************************************************************************************/


/**
 * @file iniparser.h
 * Initialization files parser.
 */


/**
 * @ingroup SBase
 * @defgroup SIniParser Initialization File Parser
 * Implementation of a very simple initialization (INI) file
 * parser.
 *
 * An @c INI file is an ASCII file describing simple parameters
 * (character strings, integers, floating-point values or booleans) in
 * an explicit format, easy to use and modify for users.
 *
 * Parameters are defined as key-value combinations, for example:
 * @verbatim
 key = value
 age = 10; This is a comment
 # this is also a comment
 name = "Speect" # as is this a comment
 ;and another comment
 @endverbatim
 *
 * The key is any string (possibly containing white-spaces). The value
 * is any character on the right side of the equal sign. Values can be
 * given enclosed with quotes. If no quotes are present, the value is
 * understood as containing all characters between the first and the
 * last non white-space characters.
 *
 * A semicolon or hash denote a comment. Comments starts from the
 * first character after the semicolon/hash up to the end of the line.
 *
 * Multi-line @b values can be provided by ending the line with a backslash
 * (@c \).
 * @verbatim
 Multiple = Line 1 \
 Line 2 \
 Line 3 \
 Line 4 ; comment
 @endverbatim
 * with the value in the above example equal to <i>Line 1  Line 2
 * Line 3  Line 4</i>.
 *
 * An @c INI file is segmented into @a sections, declared by the
 * following syntax:
 * @verbatim
 [Section Name]
 @endverbatim
 *
 * Keys declared in sections are accessed by <i>section
 * name:key</i>. For example, in
 * @verbatim
 library = "speect"

 [plug-ins]
 tokenization = "libtoken.so"
 normalization = "libnorm.so"

 [features]
 volume = 100
 @endverbatim
 * the keys are <i>plug-ins:tokenization</i>,
 * <i>plug-ins:normalization</i>, and <i>features:volume</i>. A key
 * with an undefined section is accessed with <i>:key</i>, so in the
 * above example it would be <i>:library</i>.
 *
 * It must be noted that the implementation is not very robust with
 * regards to syntax errors, but the INI files are only used for
 * the start up of the Speect Engine.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stddef.h>
#include "include/common.h"
#include "base/utils/types.h"

/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Typedef                                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef of the opaque ini parser structure.
 */
typedef struct s_ini_parser s_ini_parser;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes (do not use these directly, use above defined macros)        */
/*                                                                                  */
/************************************************************************************/

/**
 * Parse an @c INI file and return an allocated parser object.
 * This is the parser for ini files. This function is called, providing
 * the name of the file to be read. It returns an opaque @c INI parser
 * structure that must be freed using #s_iniparser_free().
 *
 * @param path The full path and file name of the @c INI file
 *
 * @return An opaque @c INI parser structure, or @c NULL on error.
 *
 * @note Not thread safe.
 */
S_API s_ini_parser *s_iniparser_load(const char *path);


/**
 * Free the resources allocated to the given @c INI parser.
 *
 * @param inip The opaque @c INI parser structure to free.
 *
 * @note Not thread safe.
 */
S_API void s_iniparser_free(s_ini_parser *inip);


/**
 * Get the string associated with the given key.  This function
 * queries the given @c INI structure for a key. A key as read from an
 * @c INI file is given as "section:key". If the key cannot be found,
 * the pointer passed as @a def is returned.
 *
 * @param inip The INI parser structure.
 * @param key The key to search for.
 * @param def Default value to return if key not found.
 *
 * @return Pointer to string value of given key or @c def if not
 * found.
 *
 * @note Not thread safe.
 */
S_API const char *s_iniparser_get_string(const s_ini_parser *inip, const char *key,
										 const char *def);


/**
 * Get the string associated to a key, convert to an int. This function
 * queries the given @c INI structure for a key. A key as read from an
 * @c INI file is given as "section:key". If the key cannot be found,
 * the pointer passed as @a def is returned.
 *
 * Supported values for integers include the usual @c C notation
 * so decimal, octal (starting with 0) and hexadecimal (starting with 0x)
 * are supported. Examples:
 * @verbatim
 "42"      ->  42
 "042"     ->  34 (octal -> decimal)
 "0x42"    ->  66 (hexa  -> decimal)
 @endverbatim
 *
 * @param inip The INI parser structure.
 * @param key The key to search for.
 * @param def Default value to return if key not found.
 *
 * @return Integer associated with given key or @c def if not found.
 *
 * @warning  The conversion may overflow in various ways. Conversion is
 * totally outsourced to @c strtol(), see the associated man page for
 * overflow handling.
 *
 * @note Not thread safe.
 */
S_API int s_iniparser_get_int(const s_ini_parser *inip, const char *key,
							  int def);


/**
 * Get the string associated to a key, convert to a double. This function
 * queries the given @c INI structure for a key. A key as read from an
 * @c INI file is given as "section:key". If the key cannot be found,
 * the pointer passed as @a def is returned.
 *
 * The conversion process from string to double is done with the
 * standard @c C function @c atof().
 *
 * @param inip The INI parser structure.
 * @param key The key to search for.
 * @param def Default value to return if key not found.
 *
 * @return Double associated with given key or @c def if not found.
 *
 * @note Not thread safe.
 */
S_API double s_iniparser_get_double(const s_ini_parser *inip, const char *key,
									double def);


/**
 * Get the string associated to a key, convert to a boolean. This function
 * queries the given @c INI structure for a key. A key as read from an
 * @c INI file is given as "section:key". If the key cannot be found,
 * the pointer passed as @a def is returned.
 *
 * A true boolean is found if one of the following is matched:
 * <ul>
 *  <li> A string starting with 'y' </li>
 *  <li> A string starting with 'Y' </li>
 *  <li> A string starting with 't' </li>
 *  <li> A string starting with 'T' </li>
 *  <li> A string starting with '1' </li>
 * </ul>
 *
 * A false boolean is found if one of the following is matched:
 * <ul>
 * <li> A string starting with 'n' </li>
 * <li> A string starting with 'N' </li>
 * <li> A string starting with 'f' </li>
 * <li> A string starting with 'F' </li>
 * <li> A string starting with '0' </li>
 * </ul>
 *
 * @param inip The INI parser structure.
 * @param key The key to search for.
 * @param def Default value to return if key not found.
 *
 * @return Boolean associated with given key or @c def if not found.
 *
 * @note Not thread safe.
 */
S_API s_bool s_iniparser_get_boolean(const s_ini_parser *inip, const char *key,
									 s_bool def);


/**
 * Query the @c INI structure for the presence of the given entry.
 *
 * @param inip inip The INI parser structure.
 * @param entry The entry to search for.
 *
 * @return #TRUE if entry is present in the parser, else #FALSE.
 *
 * @note Not thread safe.
 */
S_API s_bool s_iniparser_entry_present(const s_ini_parser *inip, const char *entry);


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

#endif /* _SPCT_INIPARSER_H__ */

