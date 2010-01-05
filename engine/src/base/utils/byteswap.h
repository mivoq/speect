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
/* DATE    : 25 March 2008                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Byteswapping functions.                                                          */
/*                                                                                  */
/* Adapted from Agar toolkit http://libagar.org/ (1.3.2)                            */
/* Note that this is a derived work.                                                */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_BYTE_SWAP_H__
#define _SPCT_BYTE_SWAP_H__


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
 * @file byteswap.h
 * Byteswapping functions.
 */


/**
 * @ingroup SBaseUtils
 * @defgroup byteswap Byteswapping Functions
 * Declarations of byteswapping functions.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
#include "base/errdbg/errdbg_defs.h"


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

S_API uint16 s_swap16(uint16 x);

S_API uint32 s_swap32(uint32 x);

S_API float s_swapflt(float v);

S_API double s_swapdbl(double v);

#ifdef SPCT_LONG_DOUBLE
S_API long double s_swapldbl(long double v);
#endif /* SPCT_LONG_DOUBLE */

S_API s_byte *s_swap_bytes(s_byte *self, size_t length, s_erc *error);

S_API uint16 *s_swap_bytes16(uint16 *self, size_t length, s_erc *error);

S_API uint32 *s_swap_bytes32(uint32 *self, uint32 length, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/


/**
 * @def s_swap_le16
 * Swap a 16 bit little endian integer.
 * @hideinitializer
 *
 * @param SELF The 16 bit little endian integer to swap.
 *
 * @return A byte-swapped 16 bit integer.
 */


/**
 * @def s_swap_le_bytes16
 * Swap a 16 bit little endian integer array.
 * @hideinitializer
 *
 * @param SELF The 16 bit little endian integer array to swap.
 * @param LENGTH The length of the array.
 * @param E #s_erc Error code.
 *
 * @return A byte-swapped 16 bit integer array.
 */


/**
 * @def s_swap_le32
 * Swap a 32 bit little endian integer.
 * @hideinitializer
 *
 * @param SELF The 32 bit little endian integer to swap.
 *
 * @return A byte-swapped 32 bit integer.
 */


/**
 * @def s_swap_le_bytes32
 * Swap a 32 bit little endian integer array.
 * @hideinitializer
 *
 * @param SELF The 32 bit little endian integer array to swap.
 * @param LENGTH The length of the array.
 * @param E #s_erc Error code.
 *
 * @return A byte-swapped 32 bit integer array.
 */


/**
 * @def s_swap_le_bytes
 * Swap a #s_byte array little endian data representation.
 * @hideinitializer
 *
 * @param SELF The #s_byte array little endian data to swap.
 * @param LENGTH The length of the #s_byte array.
 * @param E #s_erc Error code.
 *
 * @return A byte-swapped #s_byte array.
 */


/**
 * @def s_swap_be16
 * Swap a 16 bit big endian integer.
 * @hideinitializer
 *
 * @param SELF The 16 bit big endian integer to swap.
 *
 * @return A byte-swapped 16 bit integer.
 */


/**
 * @def s_swap_be_bytes16
 * Swap a 16 bit big endian integer array.
 * @hideinitializer
 *
 * @param SELF The 16 bit big endian integer array to swap.
 * @param LENGTH The length of the array.
 * @param E #s_erc Error code.
 *
 * @return A byte-swapped 16 bit integer array.
 */


/**
 * @def s_swap_be32
 * Swap a 32 bit big endian integer.
 * @hideinitializer
 *
 * @param SELF The 32 bit big endian integer to swap.
 *
 * @return A byte-swapped 32 bit integer.
 */


/**
 * @def s_swap_be_bytes32
 * Swap a 32 bit big endian integer array.
 * @hideinitializer
 *
 * @param SELF The 32 bit big endian integer array to swap.
 * @param LENGTH The length of the array.
 * @param E #s_erc Error code.
 *
 * @return A byte-swapped 32 bit integer array.
 */


/**
 * @def s_swap_be_bytes
 * Swap a #s_byte array big endian data representation.
 * @hideinitializer
 *
 * @param SELF The #s_byte array big endian data to swap.
 * @param LENGTH The length of the #s_byte array.
 * @param E #s_erc Error code.
 *
 * @return A byte-swapped #s_byte array.
 */


/**
 * @def s_swap_le_flt
 * Swap a little endian float data representation.
 * @hideinitializer
 *
 * @param SELF The little endian float data to swap.
 *
 * @return A byte-swapped float.
 */


/**
 * @def s_swap_be_flt
 * Swap a big endian float data representation.
 * @hideinitializer
 *
 * @param SELF The big endian float data to swap.
 *
 * @return A byte-swapped float.
 */


/**
 * @def s_swap_le_dbl
 * Swap a little endian double data representation.
 * @hideinitializer
 *
 * @param SELF The little endian double data to swap.
 *
 * @return A byte-swapped double.
 */


/**
 * @def s_swap_be_dbl
 * Swap a big endian double data representation.
 * @hideinitializer
 *
 * @param SELF The big endian double data to swap.
 *
 * @return A byte-swapped double.
 */

#ifdef SPCT_LONG_DOUBLE
/**
 * @def s_swap_le_ldbl
 * Swap a little endian long double data representation.
 * @hideinitializer
 *
 * @param SELF The little endian long double data to swap.
 *
 * @return A byte-swapped long double.
 */


/**
 * @def s_swap_be_ldbl
 * Swap a big endian long double data representation.
 * @hideinitializer
 *
 * @param SELF The big endian long double data to swap.
 *
 * @return A byte-swapped long double.
 */
#endif /* SPCT_LONG_DOUBLE */


#ifdef SPCT_BIG_ENDIAN
#  define s_swap_le16(SELF)	s_swap16(SELF)
#  define s_swap_le_bytes16(SELF, LENGTH, E) s_swap_bytes16((SELF), (LENGTH), (E))
#  define s_swap_le32(SELF)	s_swap32(SELF)
#  define s_swap_le_bytes32(SELF, LENGTH, E) s_swap_bytes32((SELF), (LENGTH), (E))
#  define s_swap_le_bytes(SELF, LENGTH, E) s_swap_bytes((SELF), (LENGTH), (E))
#  define s_swap_be16(SELF)	(SELF)
#  define s_swap_be_bytes16(SELF, LENGTH, E) (SELF)
#  define s_swap_be32(SELF)	(SELF)
#  define s_swap_be_bytes32(SELF, LENGTH, E) (SELF)
#  define s_swap_be_bytes(SELF) (SELF)
#  define s_swap_le_flt(SELF)   s_swapflt(SELF)
#  define s_swap_be_flt(SELF)   (SELF)
#  define s_swap_le_dbl(SELF)   s_swapdbl(SELF)
#  define s_swap_be_dbl(SELF)   (SELF)
#  ifdef SPCT_LONG_DOUBLE
#    define s_swap_le_ldbl(SELF)s_swapldbl(SELF)
#    define s_swap_be_ldbl(SELF)(SELF)
#  endif /* SPCT_LONG_DOUBLE */
#else /* !SPCT_BIG_ENDIAN */
#  define s_swap_le16(SELF)	(SELF)
#  define s_swap_le_bytes16(SELF, LENGTH, E) (SELF)
#  define s_swap_le32(SELF)	(SELF)
#  define s_swap_le_bytes32(SELF, LENGTH, E) (SELF)
#  define s_swap_le_bytes(SELF, LENGTH, E) (SELF)
#  define s_swap_be16(SELF)	s_swap16(SELF)
#  define s_swap_be_bytes16(SELF, LENGTH, E) s_swap_bytes16((SELF), (LENGTH), (E))
#  define s_swap_be32(SELF)	s_swap32(SELF)
#  define s_swap_be_bytes32(SELF, LENGTH, E) s_swap_bytes32((SELF), (LENGTH), (E))
#  define s_swap_be_bytes(SELF, LENGTH, E) s_swap_bytes((SELF), (LENGTH), (E))
#  define s_swap_le_flt(SELF)   (SELF)
#  define s_swap_be_flt(SELF)   s_swapflt(SELF)
#  define s_swap_le_dbl(SELF)   (SELF)
#  define s_swap_be_dbl(SELF)   s_swapdbl(SELF)
#  ifdef SPCT_LONG_DOUBLE
#    define s_swap_le_ldbl(SELF)(SELF)
#    define s_swap_be_ldbl(SELF)s_swapldbl(SELF)
#  endif /* SPCT_LONG_DOUBLE */
#endif /* SPCT_BIG_ENDIAN */


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

#endif /* _SPCT_BYTE_SWAP_H__ */

