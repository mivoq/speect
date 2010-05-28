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
 * SObject functions for five primitive data types: ints, floats,
 * strings, void structures and void*. Internally four new classes and
 * object types are created and inherit from #SObjectClass and
 * #SObject. The only class that is externally accessible is SVoid.
 *
 * The classes and objects are:
 * <ul>
 *  <li> @c SInt and SIntClass, </li>
 *  <li> @c SFloat and SFloatClass, </li>
 *  <li> @c SString and SStringClass, and </li>
 *  <li> @c SVoidStruct and SVoidStructClass. </li>
 *  <li> @c SVoid and SVoidClass. </li>
 * </ul>
 *
 * Although these classes exists and can be used in function
 * calls, it is easier to use them as #SObject objects.
 *
 * SVoidStruct and it's associated functions (#SObjectSetVoidStruct,
 * #SObjectResetVoidStruct, #SObjectGetVoidStruct) are used to
 * encapsulate data structures so that they can be simply used as #SObject
 * types.
 *
 * SVoid is used to create new object types that inherit from SVoid (and
 * in turn #SObject), and implement the member functions of
 * #SVoidClass, which enables the use/implementation of more
 * complicated void types (or even external object types that Speect
 * does not need to know of).
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
typedef void (*free_void_callback)(void *ptr, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* SVoid definition                                                                 */
/*                                                                                  */
/************************************************************************************/

/**
 * The SVoid object structure.
 * SVoid inherits from #SObject and is a container to hold any type of
 * data.
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject             obj;
} SVoid;


/************************************************************************************/
/*                                                                                  */
/* SVoidClass definition                                                            */
/*                                                                                  */
/************************************************************************************/

/**
 * Definition of the void class. This class adds no class methods to the
 * #SObjectClass and is therefore exactly the same.
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SObjectClass.
	 */
	SObjectClass  _inherit;

	/* Class methods */
	/**
	 * @protected Create a new SVoid object from a void pointer.
	 * The void data type must be known, and a new instance of the
	 * void object type must be given.
	 *
	 * @param self A new instance of a void object type that is
	 * inherited from the #SVoid object and implements the functions
	 * of the #SVoidClass.
	 * @param ptr Pointer to the void data.
	 * @param error Error code.
	 *
	 * @return Pointer to the given SVoid object (@c self) with the data set.
	 */
	void (*set)(SVoid *self, void *ptr, s_erc *error);

	/**
	 * @protected Change the pointer of an SVoid that has been
	 * previously created by #SObjectSetVoid.
	 *
	 * @param self The @c SVoid type SObject.
	 * @param ptr Pointer to the @a new void object.
	 * @param error Error code.
	 */
	void (*reset)(SVoid *self, void *ptr, s_erc *error);

	/**
	 * @protected Get the void pointer of the SVoid object.
	 *
	 * @param self The @c SVoid object.
	 * @param error Error code.
	 *
	 * @return Void pointer.
	 */
	const void *(*get)(const SVoid *self, s_erc *error);
} SVoidClass;


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
S_API SObject *SObjectSetInt(const sint32 i, s_erc *error);


/**
 * Changed the data value (singed integer) of an SObject that has been
 * previously created by #SObjectSetInt.
 * @public @memberof SObject
 *
 * @param self The @c SInt type SObject.
 * @param i The @a new signed integer value.
 * @param error Error code.
 */
S_API void SObjectResetInt(SObject *self, const sint32 i, s_erc *error);


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
S_API SObject *SObjectSetFloat(const float f, s_erc *error);


/**
 * Changed the data value (float) of an SObject that has been
 * previously created by #SObjectSetFloat.
 * @public @memberof SObject
 *
 * @param self The @c SFloat type SObject.
 * @param f The @a new float value.
 * @param error Error code.
 */
S_API void SObjectResetFloat(SObject *self, const float f, s_erc *error);


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
 * @return Pointer to the newly created SObject (of type @c SVoidStruct).
 *
 * @note The @c type_name is copied and not referenced.
 */
S_API SObject *SObjectSetVoidStruct(void *ptr,
									const char *type_name,
									free_void_callback free_func,
									s_erc *error);


/**
 * Changed the pointer of an SObject that has been
 * previously created by #SObjectSetVoidStruct.
 * @public @memberof SObject
 *
 * @param self The @c SVoidStruct type SObject.
 * @param ptr Pointer to the @a new object.
 * @param type_name An identifier for this void object type. Used for
 * safety checks.
 * @param free_func A callback function used to free the
 * object.
 * @param error Error code.
 *
 * @note The @c type_name is copied and not referenced.
 */
S_API void SObjectResetVoidStruct(SObject *self, void *ptr,
								  const char *type_name,
								  free_void_callback free_func,
								  s_erc *error);


/**
 * Get the void pointer of the SObject object.
 * @public @memberof SObject
 *
 * @param self The SObject (of type @c SVoidStruct).
 * @param type_name An identifier for this void object type. Used for
 * safety checks.
 * @param error Error code.
 *
 * @return Void pointer.
 */
S_API const void *SObjectGetVoidStruct(const SObject *self,
									   const char *type_name,
									   s_erc *error);


/**
 * @}
 */


/**
 * @name Void*
 * @{
 */


/**
 * Create a new #SObject object from a void pointer.
 * The void data type must be known, and a new instance of the
 * void object type must be given.
 * @public @memberof SObject
 *
 * @param type The type from which to create a new instance of a void
 * object that is inherited from the #SVoid object and implements the
 * functions of the #SVoidClass.
 * @param ptr Pointer to the plain void data/object.
 * @param error Error code.
 *
 * @return Pointer to @c voidObject.
 */
S_API SObject *SObjectSetVoid(const char *type, void *ptr,
							  s_erc *error);


/**
 * Change the pointer of an SVoid that has been
 * previously created by #SObjectSetVoid.
 * @public @memberof SObject
 *
 *
 * @param self The @c SVoid type #SObject.
 * @param ptr Pointer to the @a new void object.
 * @param error Error code.
 */
S_API void SObjectResetVoid(SObject *self, void *ptr, s_erc *error);


/**
 * Get the void pointer of the SObject object.
 * @public @memberof SObject
 *	 *
 * @param self The @c SVoid type #SObject.
 * @param error Error code.
 *
 * @return Void pointer.
 */
S_API const void *SObjectGetVoid(const SObject *self, s_erc *error);


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
