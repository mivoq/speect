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
/* Type definitions.                                                                */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_TYPES_H__
#define _SPCT_TYPES_H__


/**
 * @file types.h
 * Type definitions.
 */


/**
 * @ingroup SBaseUtils
 * @defgroup STypedfs Type Definitions Abstraction
 * Definition of types used in the Speect engine.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

/*
 * Include the platform specific type definitions.
 * types_impl.h is automatically created by the build system
 * from user options
 */
#include "base/utils/platform/types_impl.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Typedefs and data types                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * @typedef s_bool
 * Type definition of a boolean type (#TRUE or #FALSE).
 */


/**
 * @typedef sint
 * Type definition of a signed integer.
 */


/**
 * @typedef uint
 * Type definition of an unsigned integer.
 */


/**
 * @typedef sint8
 * Type definition of a signed 1 byte integer.
 */


/**
 * @typedef uint8
 * Type definition of an unsigned 1 byte integer.
 */


/**
 * @typedef sint16
 * Type definition of a signed 2 byte integer.
 */


/**
 * @typedef uint16
 * Type definition of an unsigned 2 byte integer.
 */


/**
 * @typedef sint32
 * Type definition of a signed 4 byte integer.
 */


/**
 * @typedef uint32
 * Type definition of an unsigned 4 byte integer.
 */


/**
 * @typedef slong
 * Type definition of a signed long integer.
 */


/**
 * @typedef ulong
 * Type definition of an unsigned long integer.
 */


/**
 * @typedef schar
 * Type definition of a signed char.
 */


/**
 * @typedef uchar
 * Type definition of an unsigned char.
 */


/**
 * @typedef s_byte
 * Type definition of a 1 byte data type.
 */


/************************************************************************************/
/*                                                                                  */
/* Sanity macros                                                                    */
/*                                                                                  */
/************************************************************************************/

/* Make sure the types really have the right sizes as we expect */
#define S_TYPE_ASSERT(NAME, X)               \
       typedef int S_dummy_ ## NAME[(X) * 2 - 1]

S_TYPE_ASSERT(uint8, sizeof(uint8) == 1);
S_TYPE_ASSERT(sint8, sizeof(sint8) == 1);
S_TYPE_ASSERT(uint16, sizeof(uint16) == 2);
S_TYPE_ASSERT(sint16, sizeof(sint16) == 2);
S_TYPE_ASSERT(uint32, sizeof(uint32) == 4);
S_TYPE_ASSERT(sint32, sizeof(sint32) == 4);
S_TYPE_ASSERT(float, sizeof(float) == 4);
S_TYPE_ASSERT(double, sizeof(double) == 8);


/* Check to make sure enums are the size of ints, for structure packing. */
typedef enum
{
	DUMMY_ENUM_VALUE
} S_DUMMY_ENUM;


S_TYPE_ASSERT(enum, sizeof(S_DUMMY_ENUM) == sizeof(int));


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

#endif /* _SPCT_TYPES_H__ */

