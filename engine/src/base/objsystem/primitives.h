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
/* DATE    : 1 November 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SObject functions for four primitive data types: ints, floats, strings and void* */
/*                                                                                  */
/* Loosely based on cst_val of Flite, http://cmuflite.org (1.3)                     */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PRIMITIVES_H__
#define _SPCT_PRIMITIVES_H__


/************************************************************************************/
/*  Flite license, cst_val                                                          */
/*                                                                                  */
/*                       Language Technologies Institute                            */
/*                          Carnegie Mellon University                              */
/*                             Copyright (c) 1999                                   */
/*                             All Rights Reserved.                                 */
/*                                                                                  */
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
/*                                                                                  */
/*       CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK               */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE            */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author:  Alan W Black                                                      */
/*                                                                                  */
/************************************************************************************/


/**
 * @file primitives.h
 * SObjects for four primitive data types: ints, floats, strings and void*.
 */


/**
 * @ingroup SObjSystem
 * @defgroup SPrimitiveObjects Primitive Data Types Objects
 * SObject functions for four primitive data types: ints, floats,
 * strings, void*. Internally four new classes and* object types are
 * created and inherit from #SObjectClass and #SObject.
 *
 * The classes and objects are:
 * <ul>
 *  <li> @c SInt and SIntClass, </li>
 *  <li> @c SFloat and SFloatClass, </li>
 *  <li> @c SString and SStringClass, and </li>
 *  <li> @c SVoid and SVoidClass. </li>
 * </ul>
 *
 * Although these classes exists and can be used in function
 * calls, it is easier to use them as #SObject objects.
 *
 * SVoid and it's associated functions (#SObjectSetVoid,
 * #SObjectResetVoid, #SObjectGetVoid) are used to
 * encapsulate data structures so that they can be simply used as #SObject
 * types. This enables the use/implementation of more complicated
 * types (or even external object types that Speect does not need to
 * know of).
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
#include "base/errdbg/errdbg.h"
#include "base/objsystem/object_def.h"


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

/*
 * Type definiton of a callback function to free a unknown data type.
 */
typedef void (*s_sobject_free_callback_fp)(void *ptr, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * @name Integer
 * @{
 */


/**
 * Create a new SObject from a signed integer.
 * @public @memberof SObject
 *
 * @param i The signed integer value.
 * @param error Error code.
 *
 * @return Pointer to the newly created SObject (of type @c SInt).
 */
S_API SObject *SObjectSetInt(sint32 i, s_erc *error);


/**
 * Changed the data value (singed integer) of an SObject that has been
 * previously created by #SObjectSetInt.
 * @public @memberof SObject
 *
 * @param self The @c SInt type SObject.
 * @param i The @a new signed integer value.
 * @param error Error code.
 */
S_API void SObjectResetInt(SObject *self, sint32 i, s_erc *error);


/**
 * Get the signed integer of the SObject object.
 * @public @memberof SObject
 *
 * @param self The SObject (of type @c SInt).
 * @param error Error code.
 *
 * @return The signed integer value.
 */
S_API sint32 SObjectGetInt(const SObject *self, s_erc *error);


/**
 * @}
 */


/**
 * @name Float
 * @{
 */


/**
 * Create a new SObject from a float.
 * @public @memberof SObject
 *
 * @param f The float value.
 * @param error Error code.
 *
 * @return Pointer to the newly created SObject (of type @c SFloat).
 */
S_API SObject *SObjectSetFloat(float f, s_erc *error);


/**
 * Changed the data value (float) of an SObject that has been
 * previously created by #SObjectSetFloat.
 * @public @memberof SObject
 *
 * @param self The @c SFloat type SObject.
 * @param f The @a new float value.
 * @param error Error code.
 */
S_API void SObjectResetFloat(SObject *self, float f, s_erc *error);


/**
 * Get the float of the SObject object.
 * @public @memberof SObject
 *
 * @param self The SObject (of type @c SFloat).
 * @param error Error code.
 *
 * @return The float value.
 */
S_API float SObjectGetFloat(const SObject *self, s_erc *error);


/**
 * @}
 */


/**
 * @name String
 * @{
 */


/**
 * Create a new SObject from a character string.
 * @public @memberof SObject
 *
 * @param s Pointer to the string.
 * @param error Error code.
 *
 * @return Pointer to the newly created SObject (of type @c SString).
 *
 * @note The string is copied and not referenced.
 */
S_API SObject *SObjectSetString(const char *s, s_erc *error);


/**
 * Changed the data value (string) of an SObject that has been
 * previously created by #SObjectSetString.
 * @public @memberof SObject
 *
 * @param self The @c SString type SObject.
 * @param s Pointer to the @a new string.
 * @param error Error code.
 */
S_API void SObjectResetString(SObject *self, const char *s, s_erc *error);


/**
 * Get the string of the SObject object.
 * @public @memberof SObject
 *
 * @param self The SObject (of type @c SString).
 * @param error Error code.
 *
 * @return Pointer to the string.
 */
S_API const char *SObjectGetString(const SObject *self, s_erc *error);


/**
 * @}
 */


/**
 * @name VoidStruct
 * @{
 */


/**
 * Create a new SObject from a void pointer.
 * @public @memberof SObject
 *
 * @param ptr Pointer to the object.
 * @param type_name An identifier for this void object type. Used for
 * safety checks.
 * @param free_func A callback function used to free the object.
 * @param error Error code.
 *
 * @return Pointer to the newly created SObject.
 *
 * @note The @c type_name is copied and not referenced.
 */
S_API SObject *SObjectSetVoid(void *ptr, const char *type_name,
							  s_sobject_free_callback_fp free_func,
							  s_erc *error);


/**
 * Change the pointer of an SObject that has been
 * previously created by #SObjectSetVoid.
 * @public @memberof SObject
 *
 * @param self The SObject.
 * @param ptr Pointer to the @a new object.
 * @param type_name An identifier for this void object type. Used for
 * safety checks.
 * @param free_func A callback function used to free the
 * object.
 * @param error Error code.
 *
 * @note The @c type_name is copied and not referenced.
 */
S_API void SObjectResetVoid(SObject *self, void *ptr,
							const char *type_name,
							s_sobject_free_callback_fp free_func,
							s_erc *error);


/**
 * Get the void pointer of the SObject object.
 * @public @memberof SObject
 *
 * @param self The SObject.
 * @param type_name An identifier for this void object type. Used for
 * safety checks.
 * @param error Error code.
 *
 * @return Void pointer.
 */
S_API const void *SObjectGetVoid(const SObject *self,
								 const char *type_name,
								 s_erc *error);


/**
 * @}
 */


/**
 * Add the SInt, SFloat, SString and SVoid classes to the object system.
 * @private @memberof SObject
 *
 * @param error Error code.
 */
S_LOCAL void _s_primitive_class_add(s_erc *error);


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

#endif /* _SPCT_PRIMITIVES_H__ */
