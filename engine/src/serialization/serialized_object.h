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
/* DATE    : 15 October 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* An abstract class for a serialized object data source.                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_SERIALIZED_OBJECT__
#define _SPCT_SERIALIZED_OBJECT__


/**
 * @file serialized_object.h
 * Abstract data source class for serialized objects.
 */


/**
 * @ingroup SSerializeObjects
 * @defgroup SSerializedObject Serialized Object
 * An abstract data source class for serialized objects.
 * @todo explain
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "include/common.h"
#include "base/objsystem/objsystem.h"
#include "base/errdbg/errdbg.h"
#include "datasources/data_source.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* SSerializedObject definition                                                     */
/*                                                                                  */
/************************************************************************************/

/**
 * The abstract <i>serialized object</i> structure. Provides an abstract interface to
 * a <i>serialized object</i> reader/writer. All classes that want to read/write an #SObject
 * type from/to file must inherit from this object's class.
 * @extends SObject
 */
struct SSerializedObject  /* typedef is in src/base/objsystem/object_def.h */
{
	/**
	 * @protected Inherit from SObject.
	 */
	SObject        obj;
};


/************************************************************************************/
/*                                                                                  */
/* SSerializedObjectClass definition                                                */
/*                                                                                  */
/************************************************************************************/

/**
 * The abstract <i>serialized object</i> class structure.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from SObjectClass.
	 */
	SObjectClass  _inherit;

	/**
	 * @protected The format of this object serialization class.
	 */
	const char * const format;

	/**
	 * @protected The object types of this object serialization class.
	 * This is a @c NULL delimited array of object type names.
	 * @todo is delimited the right word?
	 */
	const char * const * const object_types;

	/* Class methods */
	/**
	 * @protected Read function pointer.
	 * Read the given object's data from a serialized data source.
	 *
	 * @param reader The serialized data reader.
	 * @param object The object's data to read.
	 * @param error Error code.
	 */
	void (* const read) (SDataReader *reader, SObject *object,
						 s_erc *error);

	/**
	 * @protected Write function pointer.
	 * Write the given object's data to a serialized data source.
	 *
	 * @param writer The serialized data writer.
	 * @param object The object's data to write.
	 * @param error Error code.
	 */
	void (* const write)(SDataWriter *writer, const SObject *object,
						 s_erc *error);
} SSerializedObjectClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Read an object's data from a serialized data source. This is just a
 * wrapper function for the class method.
 *
 * @public @memberof SSerializedObject
 * @param self The serialized object formater.
 * @param reader The serialized data reader.
 * @param object The object's data to read.
 * @param error Error code.
 */
S_API void SSerializedObjectRead(const SSerializedObject *self, SDataReader *reader,
								 SObject *object, s_erc *error);


/**
 * Write an object's data to a serialized data source. This is just a
 * wrapper function for the class method.
 *
 * @public @memberof SSerializedObject
 * @param self The serialized object formater.
 * @param writer The serialized data writer.
 * @param object The object's data to write.
 * @param error Error code.
 */
S_API void SSerializedObjectWrite(const SSerializedObject *self, SDataWriter *writer,
								  const SObject *object, s_erc *error);


/**
 * Add the SSerializedObject class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_serialized_object_class_add(s_erc *error);


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

#endif /* _SPCT_SERIALIZED_OBJECT__ */

