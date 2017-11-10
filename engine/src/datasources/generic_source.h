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
/* AUTHOR  : Cristian Andrighetto                                                   */
/*           based on Aby Louw work.                                                */
/* DATE    : 13 December 2016                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A generic data source class.                                                     */
/*                                                                                  */
/* Adapted from Agar toolkit http://libagar.org/ (1.3.2)                            */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_GENERIC_SOURCE_H__
#define _SPCT_GENERIC_SOURCE_H__


/************************************************************************************/
/* libagar license                                                                  */
/*                                                                                  */
/* Copyright (c) 2002-2007 Hypertriton, Inc. <http://hypertriton.com/>              */
/* All rights reserved.                                                             */
/*                                                                                  */
/* Redistribution and use in source and binary forms, with or without               */
/* modification, are permitted provided that the following conditions               */
/* are met:                                                                         */
/* 1. Redistributions of source code must retain the above copyright                */
/*    notice, this list of conditions and the following disclaimer.                 */
/* 2. Redistributions in binary form must reproduce the above copyright             */
/*    notice, this list of conditions and the following disclaimer in the           */
/*    documentation and/or other materials provided with the distribution.          */
/*                                                                                  */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"      */
/* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE        */
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE       */
/* ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR       */
/* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL           */
/* DAMAGES (INCLUDING BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR        */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER       */
/* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,    */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE        */
/* USE OF THIS SOFTWARE EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.          */
/*                                                                                  */
/************************************************************************************/


/**
 * @file generic_source.h
 * Definition of the generic data source class.
 */


/**
 * @ingroup SDatasource
 * @defgroup SGenericSource Generic Data Source
 * Any sources used as a data source.
 * @{
 */


/**
 * @example datasource_example.c
 * An example of writing to a file using a data source.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "datasources/data_source.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* SGenericsource definition                                                        */
/*                                                                                  */
/************************************************************************************/

/**
* Structure for define i/o functions.
*/
typedef struct
{
	/**
	* @protected The pointer to read functions.
	*/
	ssize_t (*read)(void *ptr, char* buf, size_t size);
	/**
	* @protected The pointer to write function.
	*/
	ssize_t (*write)(void *ptr, const char* buf, size_t size);
	/**
	* @protected The pointer to seek function.
	*/
	int (*seek)(void * ptr, ptrdiff_t *offset, int whence);
	/**
	* @protected The pointer to close function.
	*/
	int (*close)(void *ptr);
}SIOFunctions;

/**
 * The generic source structure. Provides an interface to a generic based
 * data source.
 * @extends SDatasource
 */
typedef struct
{
	/**
	 * @protected Inherit from #SDatasource.
	 */
	SDatasource     obj;

	/**
	 * @protected The pointer to memory
	 */
	void* ptr;

	/**
	 * @protected Structure that contains pointers to io functions
	 */
	SIOFunctions* io_functs;
}SGenericsource;


/************************************************************************************/
/*                                                                                  */
/* SGenericsourceClass definition                                                   */
/*                                                                                  */
/************************************************************************************/

/**
 * Generic source class structure. Same as #SDatasourceClass but we add
 * a structure that contains pointers to io functions (write, read, seek)
 * defined by user.
 */
typedef SDatasourceClass SGenericsourceClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Access to a memory area to be used as a data source.
 * Access to a memory area defined by pointer ptr, where mode is character string as
 * used by the standard function @c fopen().

 * @public @memberof SGenericsource
 *
 * @param ptr The pointer to the memory area.
 * @param mode The mode string as used by the standard
 * function @c fopen().
 * @param io_functs Struct that contains pointers to functions used as callback in the io operations.
 * @param error Error code.
 *
 * @return Pointer to the newly created data source.
 */
S_API SDatasource *SGenericsourceOpen(void *ptr, const char *mode, SIOFunctions* io_functs, s_erc *error);


/**
 * Add the SGenericsource class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_generic_source_class_add(s_erc *error);


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

#endif /* _SPCT_GENERIC_SOURCE_H__ */
