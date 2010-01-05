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
/* DATE    : 15 October 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Reading/Writing and loading/saving of serialized objects.                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_SERIALIZE__
#define _SPCT_SERIALIZE__


/**
 * @file serialize.h
 * Reading/Writing and loading/saving of serialized objects.
 */


/**
 * @ingroup SDatasource
 * @defgroup SSerializeObjects Serialize Objects
 * Reading/Writing and loading/saving of serialized objects.
 * @todo explain
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"
#include "base/objsystem/objsystem.h"
#include "serialization/serialized_object.h"
#include "serialization/serialized_file.h"


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
 * Register the given #SSerializedObjectClass with the Speect Engine.
 * @public @memberof SSerializedObjectClass
 *
 * @param serializedObjectClass The given serialized object class to
 * register.
 * @param error Error code.
 */
S_API void SSerializedObjectRegister(const SSerializedObjectClass* serializedObjectClass,
									 s_erc *error);


/**
 * Free the given #SSerializedObjectClass from the Speect Engine.
 * @public @memberof SSerializedObjectClass
 *
 * @param serializedObjectClass The given serialized object class to
 * free.
 * @param error Error code.
 */
S_API void SSerializedObjectFree(const SSerializedObjectClass* serializedObjectClass,
								 s_erc *error);


/**
 * Get the given #SSerializedObjectClass from the Speect Engine.
 * @public @memberof SSerializedObjectClass
 *
 * @param object The object for which a serialized formatter is
 * requested.
 * @param format The format of the requested serialized formatter.
 * @param error Error code.
 *
 * @return A serialized object formatter.
 */
S_API const SSerializedObject *SSerializedObjectGet(const SObject* object,
													const char *format,
													s_erc *error);


/**
 * Query if object instances of the given type are readable.
 * Checks that there are #SSerializedObjectClass classes registered to read
 * object instances of the given type and in the given format.
 * @public @memberof SObject
 *
 * @param object_type The object type to query for.
 * @param format Format in which the object is wished to be read.
 * @param error Error code.
 *
 * @return @c TRUE or @c FALSE.
 */
S_API s_bool SObjectTypeIsReadable(const char *object_type,
								   const char *format,
								   s_erc *error);


/**
 * Query if the given object is writable in the given format.
 * Checks there are #SSerializedObjectClass classes registered to write
 * the object in the given format.
 * @public @memberof SObject
 *
 * @param object The object for which the query is done.
 * @param format Format in which the object is wished to be written.
 * @param error Error code.
 *
 * @return @c TRUE or @c FALSE.
 */
S_API s_bool SObjectIsWritable(const SObject *object, const char *format,
							   s_erc *error);


/**
 * Register the given #SSerializedFileClass with the Speect Engine.
 * @public @memberof SSerializedFileClass
 *
 * @param serializedFileClass The given serialized file class to
 * register.
 * @param error Error code.
 */
S_API void SSerializedFileRegister(const SSerializedFileClass *serializedFileClass,
								   s_erc *error);


/**
 * Free the given #SSerializedFileClass from the Speect Engine.
 * @public @memberof SSerializedFileClass
 *
 * @param serializedFileClass The given serialized file class to free.
 * @param error Error code.
 */
S_API void SSerializedFileFree(const SSerializedFileClass *serializedFileClass,
							   s_erc *error);


/**
 * Save the given object in the given format to the given file name.
 * @public @memberof SObject
 *
 * @param object The object to save.
 * @param path The full path and file name of the object file.
 * @param format The format in which the object should be saved.
 * @param error Error code.
 */
S_API void SObjectSave(const SObject *object, const char *path, const char *format,
					   s_erc *error);


/**
 * Load data of an object in the given format from the given file name.
 * @public @memberof SObject
 *
 * @param path The full path and file name of the object file.
 * @param format The format of the object file.
 * @param error Error code.
 *
 * @return The loaded object.
 */
S_API SObject *SObjectLoad(const char *path, const char *format,
						   s_erc *error);


/**
 * Initialize the serialize module resources.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_serialize_init(s_erc *error);


/**
 * Quit and free the serialize module resources.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_serialize_quit(s_erc *error);


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

#endif /* _SPCT_SERIALIZE__ */
