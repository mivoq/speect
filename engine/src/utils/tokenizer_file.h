/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A file tokenizer class implementation.                                           */
/* Loosely based on EST_Token of Edinburgh Speech Tools,                            */
/* http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96)                          */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_TOKENIZER_FILE_H__
#define _SPCT_TOKENIZER_FILE_H__


/************************************************************************************/
/* Edinburgh Speech Tools license,  EST_Token                                       */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                           Copyright (c) 1996                                     */
/*                             All Rights Reserved.                                 */
/*       Permission is hereby granted, free of charge, to use and distribute        */
/*       this software and its documentation without restriction, including         */
/*       without limitation the rights to use, copy, modify, merge, publish,        */
/*       distribute, sublicense, and/or sell copies of this work, and to            */
/*       permit persons to whom this work is furnished to do so, subject to         */
/*       the following conditions:                                                  */
/*        1. The code must retain the above copyright notice, this list of          */
/*           conditions and the following disclaimer.                               */
/*        2. Any modifications must be clearly marked as such.                      */
/*        3. Original authors' names are not deleted.                               */
/*        4. The authors' names are not used to endorse or promote products         */
/*           derived from this software without specific prior written              */
/*           permission.                                                            */
/*       THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK              */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE           */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author :  Alan W Black                                                     */
/*                                                                                  */
/************************************************************************************/

/**
 * @file tokenizer_file.h
 * A file tokenizer class implementation.
 */


/**
 * @ingroup STokenizer
 * @defgroup STokenizerFile File Tokenizer
 * A file tokenizer class implementation.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"
#include "datasources/file_source.h"
#include "utils/tokenizer.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given #STokenizerFile child/parent class object as a
 * #STokenizerFile object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #STokenizerFile* type.
 * @note This casting is not safety checked.
 */
#define S_TOKENIZER_FILE(SELF)    ((STokenizerFile *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* STokenizerFile definition                                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * The file tokenizer structure.
 * @extends STokenizer
 */
typedef struct
{
	/**
	 * @protected Inherit from #STokenizer.
	 */
	STokenizer     obj;

	/**
	 * @protected The file data source.
	 */
	SDatasource   *ds;
} STokenizerFile;


/************************************************************************************/
/*                                                                                  */
/* STokenizerFileClass definition                                                   */
/*                                                                                  */
/************************************************************************************/

/**
 * The file tokenizer class structure. Same as #STokenizerClass as
 * we are not adding any new methods.
 */
typedef STokenizerClass STokenizerFileClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize a file tokenizer with a file path.
 *
 * @public @memberof STokenizerFile
 *
 * @param self The file tokenizer to initialize.
 * @param path The full path and file name of the file to
 * tokenize.
 * @param error Error code.
 *
 * @note If this function fails the file tokenizer will be deleted
 * and the @c self pointer will be set to @c NULL.
 */
S_API void STokenizerFileInit(STokenizerFile **self, const char *path, s_erc *error);


/**
 * Add the STokenizerFile class to the object system.
 * @private @memberof STokenizerFile
 * @param error Error code.
 */
S_LOCAL void _s_tokenizer_file_class_add(s_erc *error);


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

#endif /* _SPCT_TOKENIZER_FILE_H__ */
