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
/* An abstract class for a serialized file data source.                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_SERIALIZED_FILE__
#define _SPCT_SERIALIZED_FILE__


/**
 * @file serialized_file.h
 * Abstract data source class for objects serialized to/from file.
 */


/**
 * @ingroup SSerializeObjects
 * @defgroup SSerializedFile Serialized File
 * An abstract data source class for objects serialized to/from file.
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
/* SSerializedFile definition                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The abstract <i>serialized file</i> structure. Provides an abstract interface to
 * a <i>serialized file</i> reader/writer. All classes that want to read/write an #SObject
 * type from/to file must inherit from this object's class.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from SObject.
	 */
	SObject        obj;
} SSerializedFile;


/************************************************************************************/
/*                                                                                  */
/* SSerializedFileClass definition                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * The abstract <i>serialized file</i> class structure.
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
	 * @protected The format of this file serialization class.
	 */
	const char * const format;

	/* Class methods */
	/**
	 * @protected Load function pointer.
	 * Load data of an object from the given file name.
	 *
	 * @param path The full path and file name of the object file.
	 * @param error Error code.
	 *
	 * @return The loaded object.
	 */
	SObject *(*load) (const char *path, s_erc *error);

	/**
	 * @protected Save function pointer.
	 * Save the given object to the given file name.
	 *
	 * @param object The object to save.
	 * @param path The full path and file name of the object file.
	 * @param error Error code.
	 */
	void     (*save) (const SObject *object, const char *path, s_erc *error);
} SSerializedFileClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Load an object from a serialized file data source. This is just a
 * wrapper function for the class method and used internally by #SObjectLoad
 *
 * @private
 * @param self The SSerializedFile handler.
 * @param path The full path and file name of the object file.
 * @param error Error code.
 *
 * @return The loaded object.
 */
S_LOCAL SObject *SSerializedFileLoad(const SSerializedFile *self, const char *path,
									 s_erc *error);


/**
 * Save an object to a serialized file data source. This is just a
 * wrapper function for the class method and used internally by #SObjectSave
 *
 * @private
 * @param self The SSerializedFile handler.
 * @param object The object to save.
 * @param path The full path and file name of the object file.
 * @param error Error code.
 */
S_LOCAL void SSerializedFileSave(const SSerializedFile *self, const SObject *object,
								 const char *path, s_erc *error);


/**
 * Add the SSerializedFile class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_serialized_file_class_add(s_erc *error);


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

#endif /* _SPCT_SERIALIZED_FILE__ */

