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
/* DATE    : 25 March 2008                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Hash functions.                                                                  */
/*                                                                                  */
/* Derived from the work by Bob Jenkins.                                            */
/* http://www.burtleburtle.net/bob/, May 2006, Public Domain.                       */
/* Changed the function names and data types.                                       */
/*                                                                                  */
/************************************************************************************/


#ifndef _SPCT_HASH_FUNCTIONS_H__
#define _SPCT_HASH_FUNCTIONS_H__


/**
 * @file hash_functions.h
 * Hash functions.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

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
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

#ifdef SPCT_BIG_ENDIAN
#  define S_HASH(key, length, initval) (s_hashbig(key, length, initval))
#  define S_HASH_LITTLE_ENDIAN 0
#  define S_HASH_BIG_ENDIAN 1
#else /* !SPCT_BIG_ENDIAN */
#  define S_HASH(key, length, initval) (s_hashlittle(key,length, initval))
#  define S_HASH_LITTLE_ENDIAN 1
#  define S_HASH_BIG_ENDIAN 0
#endif /* SPCT_BIG_ENDIAN */


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

#define S_HASH_SIZE(n) ((uint32)1<<(n))

#define S_HASH_MASK(n) (SPCT_HASH_SIZE(n)-1)

#define S_ROT(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

#define S_MIX(a,b,c) \
{ \
  a -= c;  a ^= S_ROT(c, 4);  c += b; \
  b -= a;  b ^= S_ROT(a, 6);  a += c; \
  c -= b;  c ^= S_ROT(b, 8);  b += a; \
  a -= c;  a ^= S_ROT(c,16);  c += b; \
  b -= a;  b ^= S_ROT(a,19);  a += c; \
  c -= b;  c ^= S_ROT(b, 4);  b += a; \
}

#define S_FINAL(a,b,c) \
{ \
  c ^= b; c -= S_ROT(b,14); \
  a ^= c; a -= S_ROT(c,11); \
  b ^= a; b -= S_ROT(a,25); \
  c ^= b; c -= S_ROT(b,16); \
  a ^= c; a -= S_ROT(c,4);  \
  b ^= a; b -= S_ROT(a,14); \
  c ^= b; c -= S_ROT(b,24); \
}


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/*
 * k - the key, an array of uint32 values
 * length - the length of the key, in uint32
 * initval - the previous hash, or an arbitrary value
 */
S_LOCAL uint32 s_hashword(const uint32 *k, size_t length, uint32 initval);


/*
 * k - the key, an array of uint32 values
 * length - the length of the key, in uint32
 * pc - IN: seed OUT: primary hash value
 * pb - IN: more seed OUT: secondary hash value
 */
S_LOCAL void s_hashword2(const uint32 *k, size_t length, uint32 *pc, uint32 *pb);


/*
 * k - the key (the unaligned variable-length array of bytes)
 * length - the length of the key, counting by bytes
 * initval - can be any 4-byte value
 */
S_LOCAL uint32 s_hashlittle(const void *key, size_t length, uint32 initval);


/*
 * k - the key to hash
 * length - the length of the key
 * pc - IN: primary initval, OUT: primary hash value
 * pb - IN: secondary initval, OUT: secondary hash value
 */
S_LOCAL void s_hashlittle2(const uint32 *key, size_t length, uint32 *pc, uint32 *pb);


/*
 * k - the key (the unaligned variable-length array of bytes)
 * length - the length of the key, counting by bytes
 * initval - can be any 4-byte value
 */
S_LOCAL uint32 s_hashbig(const void *key, size_t length, uint32 initval);


/*
 * k - the key to hash
 * length - the length of the key
 * pc - IN: primary initval, OUT: primary hash value
 * pb - IN: secondary initval, OUT: secondary hash value
 */
S_LOCAL void s_hashbig2(const uint32 *key, size_t length, uint32 *pc, uint32 *pb);


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


#endif /* _SPCT_HASH_FUNCTIONS_H__ */

