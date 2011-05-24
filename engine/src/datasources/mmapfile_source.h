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
/* DATE    : 23 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A memory mapped file data source class.                                          */
/*                                                                                  */
/* Adapted from Agar toolkit http://libagar.org/ (1.3.2)                            */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_MMAP_FILE_SOURCE_H__
#define _SPCT_MMAP_FILE_SOURCE_H__


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
 * @file mmapfile_source.h
 * Definition of the memory mapped file data source class.
 */


/**
 * @ingroup SDatasource
 * @defgroup SMMapFileSource Memory Mapped File Data Source
 * A memory mapped file to be used as a data source.
 * @todo size of mapped file (need boundry error checking)
 * @todo should access be direct or memcpy
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
#include "datasources/data_source.h"
#include "datasources/mmapfile.h"

/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* SMMapFilesource definition                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The mmap file source structure. Provides an interface to a memory mapped
 * file based data source.
 * @extends SDatasource
 */
typedef struct
{
	/**
	 * @protected Inherit from #SDatasource.
	 */
	SDatasource         obj;

	/**
	 * @protected File path.
	 */
	char               *path;

	/**
	 * @protected Memory that this file maps to.
	 */
	uint8              *mem;

	/**
	 * @protected Size of the memory page.
	 */
	size_t              map_size;

	/**
	 * @protected  Current position.
	 */
	long                offset;

	/**
	 * @protected File handle to memory mapped file.
	 */
	s_mmap_file_handle *handle;
} SMMapFilesource;


/************************************************************************************/
/*                                                                                  */
/* SMMapFilesourceClass definition                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * MMapFilesource class structure. Same as #SDatasourceClass as
 * we are not adding any new methods.
 */
typedef SDatasourceClass SMMapFilesourceClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Opens a file to be memory mapped and used as a data source.
 * Open the file at path, where mode is character string as used by
 * the standard function @c fopen().
 * @public @memberof SMMapFilesource
 *
 * @param path The path and name of the file to open.
 * @param error Error code.
 *
 * @return Pointer to the newly created data source.
 *
 * @note The #SMMapFilesource data source is read only and does @b not
 * implement #SDatasourceClass methods @c write (#SDatasourceWrite) and
 * @c write_at (#SDatasourceWriteAt).
 */
S_API SDatasource *SMMapFilesourceOpenFile(const char *path, s_erc *error);


/**
 * Add the SMMapFilesource class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_mmap_file_source_class_add(s_erc *error);


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

#endif /* _SPCT_MMAP_FILE_SOURCE_H__ */

