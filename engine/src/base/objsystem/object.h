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
/* DATE    : 5 July 2008                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Implementation of the generic object system.                                     */
/*                                                                                  */
/* Adapted from Agar toolkit http://libagar.org/ (1.3.2)                            */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_OBJECT_H__
#define _SPCT_OBJECT_H__


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
 * @file object.h
 * Object functions.
 */


/**
 * @ingroup SObjSystem
 * @defgroup SBaseObject Object Functions
 * Provides object type function.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"
#include "base/utils/vernum.h"
#include "base/utils/types.h"
#include "base/objsystem/object_def.h"


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
 * Allocates a new object instance of the given class and initializes
 * its members.
 * @private @memberof SObject
 *
 * @param cls The class from which to create a new object.
 * @param error Error code.
 *
 * @return Pointer to the newly created object.
 *
 * @todo check that everything is free'd on error.
 *
 * @sa #SObjectNewFromName
 */
S_API SObject *SObjectNew(SObjectClass *cls, s_erc *error);


/**
 * Return the size of the given object in bytes.
 * @public @memberof SObject
 *
 * @param self The object.
 * @param error Error code.
 *
 * @return The size of the object in bytes.
 */
S_API size_t SObjectSize(const SObject *self, s_erc *error);


/**
 * Test if an object is derived from the given class.
 * @public @memberof SObject
 *
 * @param self The object to test.
 * @param type The class to test against.
 * @param error Error code.
 *
 * @return #TRUE or #FALSE.
 */
S_API s_bool SObjectIsType(const SObject *self, const char *type, s_erc *error);


/**
 * Get the given object's class type name.
 * @public @memberof SObject
 *
 * @param self The given object.
 * @param error Error code.
 *
 * @return Pointer to the given object's class type name.
 */
S_API const char *SObjectType(const SObject *self, s_erc *error);


/**
 * Get the given object's class inheritance.
 * @public @memberof SObject
 *
 * @param self The given object.
 * @param error Error Code.
 *
 * @return A ":" separated string of the given classes inheritance
 * hierarchy, excluding #SObject.
 */
S_API const char *SObjectInheritance(const SObject *self, s_erc *error);


/**
 * Dispose of an object.
 * @private @memberof SObject
 *
 * @param self Object to dispose.
 * @param error Error code.
 *
 * @note Do @b not use this function directly, use either
 * #S_DELETE or #S_FORCE_DELETE.
 */
S_API void SObjectDispose(SObject *self, s_erc *error);


/**
 * Compare two objects.
 * @public @memberof SObject
 *
 * @param oa First object of comparison.
 * @param ob Second object of comparison.
 * @param error Error code.
 *
 * @return #TRUE if the objects are the same, else #FALSE.
 *
 * @note The reference count of two objects needn't be equal for
 * objects to be equal.
 */
S_API s_bool SObjectCompare(const SObject *oa, const SObject *ob, s_erc *error);


/**
 * Print the contents of the given SObject to a character buffer. The
 * caller is responsible for the memory of the buffer.
 * @public @memberof SObject
 *
 * @param self The SObject to print.
 * @param error Error code.
 *
 * @return Pointer to a character buffer containing the contents of
 * the given SObject.
 */
S_API char *SObjectPrint(const SObject *self, s_erc *error);


/**
 * Copy an SObject.
 * @public @memberof SObject
 *
 * @param self The SObject to copy.
 * @param error Error code.
 *
 * @return Pointer to a copy of the given SObject.
 *
 * @note This is a deep copy.
 * @note Caller is responsible for returned object's memory.
 */
S_API SObject *SObjectCopy(const SObject *self, s_erc *error);


/**
 * Read an SObject from a data source reader.
 * @public @memberof SObject
 *
 * @param self The SObject to be read, the caller must create this
 * SObject from the correct class beforehand.
 * @param formater The serialized object formater.
 * @param reader The serialized data reader.
 * @param error Error code.
 */
S_API void SObjectRead(SObject *self, const SSerializedObject *formater,
					   SDataReader *reader, s_erc *error);


/**
 * Write an SObject to a data source writer.
 * @public @memberof SObject
 *
 * @param self The SObject to be written.
 * @param formater The serialized object formater.
 * @param writer The serialized data writer.
 * @param error Error code.
 */
S_API void SObjectWrite(const SObject *self, const SSerializedObject *formater,
						SDataWriter *writer, s_erc *error);


/**
 * Increment the object's reference count.
 * @public @memberof SObject
 *
 * @param self The SObject object.
 *
 * @warning Use with care
 */
S_API void SObjectIncRef(SObject *self);


/**
 * Decrement the object's reference count.
 * @public @memberof SObject
 *
 * @param self The SObject object.
 *
 * @warning Use with care
 */
S_API void SObjectDecRef(SObject *self);


/**
 * Test to see if given object can be cast to given
 * object type.
 * @private @memberof SObject
 *
 * @param self The object to cast.
 * @param cast_to The object type to cast to.
 * @param error Error code.
 *
 * @return A void pointer to the given object if it can be cast to the
 * given object type, otherwise @a NULL.
 *
 * @note Use macros #S_CAST, #S_CAST_SAFE and #S_CAST_UNSAFE.
 */
S_API void *s_safe_cast(SObject *self, const char *cast_to, s_erc *error);


/**
 * Add the SObject class to the object system.
 * @private @memberof SObject
 *
 * @param error Error code.
 */
S_LOCAL void _s_object_class_add(s_erc *error);


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

#endif /* _SPCT_OBJECT_H__ */
