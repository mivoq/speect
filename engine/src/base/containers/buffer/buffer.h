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
/* DATE    : 27 April 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Buffer with exponential growth. The buffer ensures that it is always null padded.*/
/*                                                                                  */
/* Adapted from Yet Another JSON Library (YAJL) (0.4.0)                             */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_BUFFER_H__
#define _SPCT_BUFFER_H__


/************************************************************************************/
/* Yet Another JSON Library License                                                 */
/*                                                                                  */
/* Copyright 2007, Lloyd Hilaiel.                                                   */
/*                                                                                  */
/* Redistribution and use in source and binary forms, with or without               */
/* modification, are permitted provided that the following conditions are           */
/* met:                                                                             */
/*                                                                                  */
/*  1. Redistributions of source code must retain the above copyright               */
/*     notice, this list of conditions and the following disclaimer.                */
/*                                                                                  */
/*  2. Redistributions in binary form must reproduce the above copyright            */
/*     notice, this list of conditions and the following disclaimer in              */
/*     the documentation and/or other materials provided with the                   */
/*     distribution.                                                                */
/*                                                                                  */
/*  3. Neither the name of Lloyd Hilaiel nor the names of its                       */
/*     contributors may be used to endorse or promote products derived              */
/*     from this software without specific prior written permission.                */
/*                                                                                  */
/* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR             */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED                   */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           */
/* DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,               */
/* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES               */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR               */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)               */
/* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,              */
/* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING            */
/* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE               */
/* POSSIBILITY OF SUCH DAMAGE.                                                      */
/*                                                                                  */
/************************************************************************************/


/**
 * @file buffer.h
 * Buffer container.
 */


/**
 * @ingroup SBaseContainers
 * @defgroup SBuf Buffer
 * Buffer with exponential growth that ensures that it is always null padded.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

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
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * Opaque definition of the buffer.
 * s_buffer is a buffer with exponential growth.  The buffer ensures that
 * it is always null padded.
 */
typedef struct s_buffer s_buffer;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Create a new buffer.
 *
 * @param error Error code.
 *
 * @return Pointer to a new buffer data structure.
 */
S_API s_buffer *s_buffer_new(s_erc *error);


/**
 * Delete a buffer and it's contents.
 *
 * @param buf The buffer to delete.
 *
 * @param error Error code.
 */
S_API void s_buffer_delete(s_buffer *buf, s_erc *error);


/**
 * Append a number of bytes to the given buffer.
 *
 * @param buf The buffer to append to.
 * @param data The data to append to the buffer.
 * @param size The size of the data in bytes.
 * @param error Error code.
 */
S_API void s_buffer_append(s_buffer *buf, const void *data,
						   size_t size, s_erc *error);


/**
 * Clear the data in the buffer.
 *
 * @param buf The buffer to clear.
 * @param error Error code.
 */
S_API void s_buffer_clear(s_buffer *buf, s_erc *error);


/**
 * Get a pointer to the beginning of the data in the buffer.
 *
 * @param buf The buffer to get the data of.
 * @param error Error code.
 *
 * @return Pointer to the data in the buffer.
 */
S_API const uchar *s_buffer_data(s_buffer *buf, s_erc *error);


/**
 * Get the size of the data in the buffer.
 *
 * @param buf The buffer to get the size of.
 * @param error Error code.
 *
 * @return Size of data in buffer (bytes).
 */
S_API size_t s_buffer_size(s_buffer *buf, s_erc *error);


/**
 * Truncate the buffer.
 *
 * @param buf The buffer to truncate.
 * @param len The number of bytes to truncate.
 * @param error Error code.
 */
S_API void s_buffer_truncate(s_buffer *buf, size_t len, s_erc *error);


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

#endif /* _SPCT_BUFFER_H__ */

