/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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
/* DATE    : May 2010                                                               */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Filesystem path routines.                                                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PATH_H__
#define _SPCT_PATH_H__


/**
 * @file path.h
 * Filesystem path routines.
 */


/**
 * @ingroup SBaseUtils
 * @defgroup SPath Filesystem Path Routines
 * Filesystem path routines.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/platform/path_impl.h"


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
 * Query if a file exists. Also if the file can be manipulated in a
 * certain mode (i.e. reading, writing, ...)
 *
 * @param path The path and name of the file to query.
 * @param mode The mode string as used by the standard function @c fopen().
 * @param error Error code.
 *
 * @return #TRUE if the file exists and can be manipulated with @c
 * mode, or #FALSE.
 */
S_API s_bool s_file_exists(const char *path, const char *mode, s_erc *error);


/**
 * Combine a base path and a file name. If the file name is absolute
 * then just a copy of it is returned. Otherwise, build a
 * path to it by considering it is relative to base_path. URL are
 * supported.
 *
 * @param base_path The base path relative to the given file name.
 * @param filename The file name, can be relative or absolute.
 * @param error Error code.
 *
 * @return The combined base and file name.
 *
 * @note Caller is responsible for memory of returned string.
 */
S_API char *s_path_combine(const char *base_path, const char *filename,
						   s_erc *error);


/**
 * Get the base path of the given absolute file name.
 *
 * @param absolute_filename The absolute file name.
 * @param error Error code.
 *
 * @return The base path of the given absolute file name.
 *
 * @note Caller is responsible for memory of returned string.
 */
S_API char *s_get_base_path(const char *absolute_filename, s_erc *error);


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

#endif /* _SPCT_PATH_H__ */

