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
/* Memory mapped file open/close abstraction.                                       */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_MMAP_FILE_H__
#define _SPCT_MMAP_FILE_H__


/**
 * @file mmapfile.h
 * Memory mapped file open/close abstraction.
 */


/**
 * @ingroup SMMapFileSource
 * @defgroup SMMapFile Memory Mapped File Abstraction
 * Defines a set of macros to open and close memory mapped files.
 * Different memory mapped files open/close implementations are supported by
 * implementing the following macros:
 *
 * <table>
 *  <tr>
 *   <td> @code s_mmap_file_handle *_S_MMAP_OPEN(const char *FILENAME, size_t *MAP_SIZE, uint8 **MEM, s_erc *ERROR) @endcode </td>
 *   <td> Open a memory mapped file (see #s_mmapfile_open). </td>
 *  </tr>
 *  <tr>
 *   <td> @code void _S_MMAP_CLOSE(s_mmap_file_handle *HANDLE, uint8 *MEM, s_erc *ERROR) @endcode </td>
 *   <td> Close a memory mapped file (see #s_mmapfile_close).</td>
 *  </tr>
 * </table>
 *
 * See the src/datasources/platform/posix/mmapfile_posix.h and
 * src/datasources/platform/win32/mmapfile_win32.h for examples. The
 * memory mapped files are <i> read only </i>.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

/*
 * Include the memory mapped file open/close implementation.
 * mmmapfile_impl.h is automatically created by the build system
 * from system tests.
 */
#include "datasources/platform/mmapfile_impl.h"


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
 * @def s_mmapfile_open
 * @hideinitializer
 * Open a <i>memory mapped file</i>.
@verbatim
mmap_file_handle *s_mmapfile_open(const char *FILENAME, size_t *MAP_SIZE, uint8 **MEM, s_erc *ERROR)
@endverbatim
 * @param FILENAME The full path and filename of the file to be memory
 * mapped.
 * @param MAP_SIZE Pointer to a holder for the map size.
 * @param MEM Pointer to a pointer of the memory of the mapped file.
 * @param ERROR Error code.
 * @return Handle to the memory mapped file.
 */
#define s_mmapfile_open(FILENAME, MAP_SIZE, MEM, ERROR) \
	_S_MMAP_OPEN(FILENAME, MAP_SIZE, MEM, ERROR)


/**
 * @def s_mmapfile_close
 * @hideinitializer
 * Close the given <i>memory mapped file</i>.
@verbatim
void _S_MMAP_CLOSE(s_mmap_file_handle *HANDLE, uint8 *MEM, s_erc *ERROR)
@endverbatim
 * @param HANDLE Handle to the memory mapped file to close.
 * @param MEM Pointer to the memory of the memory mapped file.
 * @param ERROR Error code.
 */
#define s_mmapfile_close(HANDLE, MEM, ERROR)	\
	_S_MMAP_CLOSE(HANDLE, MEM, ERROR)


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

#endif /* _SPCT_MMAP_FILE_H__ */

