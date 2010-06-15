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


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/strings/utf8.h"
#include "containers/containers.h"
#include "serialization/serialize.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SMap *objectTable = NULL;

static SMap *fileTable = NULL;

static s_bool initialized = FALSE;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void add_object_class(const SSerializedObjectClass* serializedObjectClass,
							 s_erc *error);

static void free_object_class(const SSerializedObjectClass* serializedObjectClass,
							  s_erc *error);

static void add_file_class(const SSerializedFileClass* serializedFileClass,
						   s_erc *error);

static void free_file_class(const SSerializedFileClass* serializedFileClass,
							s_erc *error);

static const SSerializedFile *get_file_object(const char *format, s_erc *error);


static const SSerializedObject *get_object_formatter(const SObject* object,
													 const char *format,
													 s_erc *error);

static s_bool is_object_present(const char *object_class_name, const char *format,
								s_erc *error);

/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SSerializedObjectRegister(const SSerializedObjectClass* serializedObjectClass,
									 s_erc *error)
{
	S_CLR_ERR(error);

	if (serializedObjectClass == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedObjectRegister",
				  "Argument \"serializedObjectClass\" is NULL");
		return;
	}

	if (serializedObjectClass->format == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SSerializedObjectRegister",
				  "Format of given SSerializedObjectClass is NULL");
		return;
	}

	if (serializedObjectClass->object_types == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SSerializedObjectRegister",
				  "Object types of given SSerializedObjectClass is NULL");
		return;
	}

	if (serializedObjectClass->read == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SSerializedObjectRegister",
				  "Class method \"read\" of given SSerializedObjectClass is NULL");
		return;
	}

	if (serializedObjectClass->write == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SSerializedObjectRegister",
				  "Class method \"write\" of given SSerializedObjectClass is NULL");
		return;
	}

	add_object_class(serializedObjectClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SSerializedObjectRegister",
			  "Call to \"add_object_class\" failed");
}


S_API void SSerializedObjectFree(const SSerializedObjectClass* serializedObjectClass,
								 s_erc *error)
{
	S_CLR_ERR(error);

	if (serializedObjectClass == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedObjectFree",
				  "Argument \"serializedObjectClass\" is NULL");
		return;
	}

	if (serializedObjectClass->format == NULL)
	{
		/* it could not have been added */
		return;
	}

	if (serializedObjectClass->object_types == NULL)
	{
		/* it could not have been added */
		return;
	}

	free_object_class(serializedObjectClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SSerializedObjectFree",
			  "Call to \"free_object_class\" failed");
}


S_API const SSerializedObject *SSerializedObjectGet(const SObject* object,
													const char *format,
													s_erc *error)
{
	const SSerializedObject *formatter;


	S_CLR_ERR(error);

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedObjectGet",
				  "Argument \"object\" is NULL");
		return NULL;
	}

	if (format == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedObjectGet",
				  "Argument \"format\" is NULL");
		return NULL;
	}

	formatter = get_object_formatter(object, format, error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "SSerializedObjectGet",
				  "Call to \"get_object_formatter\" failed"))
		return NULL;

	return formatter;
}


S_API s_bool SObjectTypeIsReadable(const char *object_type,
								   const char *format,
								   s_erc *error)
{
	s_bool readable;


	S_CLR_ERR(error);

	if (object_type == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectTypeIsReadable",
				  "Argument \"object_type\" is NULL");
		return FALSE;
	}

	if (format == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectTypeIsReadable",
				  "Argument \"format\" is NULL");
		return FALSE;
	}

	readable = is_object_present(object_type, format, error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "SObjectTypeIsReadable",
				  "Call to \"is_object_present\" failed"))
		return FALSE;

	return readable;
}


S_API s_bool SObjectIsWritable(const SObject *object,
							   const char *format,
							   s_erc *error)
{
	s_bool writeable;
	const char *object_type;


	S_CLR_ERR(error);

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectIsWritable",
				  "Argument \"object\" is NULL");
		return FALSE;
	}

	if (format == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectIsWritable",
				  "Argument \"format\" is NULL");
		return FALSE;
	}

	object_type = SObjectType(object, error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "SObjectIsWritable",
				  "Call to \"SObjectType\" failed"))
		return FALSE;

	writeable = is_object_present(object_type, format, error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "SObjectIsWritable",
				  "Call to \"is_object_present\" failed"))
		return FALSE;

	return writeable;
}


S_API void SSerializedFileRegister(const SSerializedFileClass *serializedFileClass,
								   s_erc *error)
{
	S_CLR_ERR(error);

	if (serializedFileClass == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedFileRegister",
				  "Argument \"serializedFileClass\" is NULL");
		return;
	}

	if (serializedFileClass->format == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SSerializedFileRegister",
				  "Format of given SSerializedFileClass is NULL");
		return;
	}

	if (serializedFileClass->load == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SSerializedFileRegister",
				  "Class method \"load\" of given SSerializedFileClass is NULL");
		return;
	}

	if (serializedFileClass->save == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SSerializedFileRegister",
				  "Class method \"save\" of given SSerializedFileClass is NULL");
		return;
	}

	add_file_class(serializedFileClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SSerializedFileRegister",
			  "Call to \"add_file_class\" failed");
}


S_API void SSerializedFileFree(const SSerializedFileClass* serializedFileClass,
							   s_erc *error)
{
	S_CLR_ERR(error);

	if (serializedFileClass == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedFileFree",
				  "Argument \"serializedFileClass\" is NULL");
		return;
	}

	if (serializedFileClass->format == NULL)
	{
		/* it could not have been added */
		return;
	}

	free_file_class(serializedFileClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SSerializedFileFree",
			  "Call to \"free_file_class\" failed");
}


S_API void SObjectSave(const SObject *object, const char *path,
					   const char *format, s_erc *error)
{
	const SSerializedFile *serializedFile;


	S_CLR_ERR(error);

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectSave",
				  "Argument \"object\" is NULL");
		return;
	}

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectSave",
				  "Argument \"path\" is NULL");
		return;
	}

	if (format == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectSave",
				  "Argument \"format\" is NULL");
		return;
	}

	/* get the serializedFileClass object */
	serializedFile = get_file_object(format, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectSave",
				  "Call to \"get_file_object\" failed"))
		return;

	SSerializedFileSave(serializedFile, object, path, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SObjectSave",
			  "Call to \"SSerializedFileSave\" failed");
}


S_API SObject *SObjectLoad(const char *path, const char *format, s_erc *error)
{
	const SSerializedFile *serializedFile;
	SObject *loadedObject;


	S_CLR_ERR(error);

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectLoad",
				  "Argument \"path\" is NULL");
		return NULL;
	}

	if (format == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectLoad",
				  "Argument \"format\" is NULL");
		return NULL;
	}

	/* get the serializedFileClass object */
	serializedFile = get_file_object(format, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectLoad",
				  "Call to \"get_file_object\" failed"))
		return NULL;

	loadedObject = SSerializedFileLoad(serializedFile, path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectLoad",
				  "Call to \"SSerializedFileLoad\" failed"))
		return NULL;

	return loadedObject;
}


/************************************************************************************/
/*                                                                                  */
/* Module init and quit                                                             */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_serialize_init(s_erc *error)
{
	S_CLR_ERR(error);

	if (initialized == TRUE)
		return;

	initialized = TRUE;

	objectTable = S_MAP(S_NEW("SMapList", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialize_init",
				  "Failed to create object table map"))
	{
		initialized = FALSE;
		return;
	}

	fileTable = S_MAP(S_NEW("SMapList", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialize_init",
				  "Failed to create file table map"))
	{
		S_DELETE(objectTable, "_s_serialize_init", error);
		initialized = FALSE;
		return;
	}
}


S_LOCAL void _s_serialize_quit(s_erc *error)
{
	S_CLR_ERR(error);

	if (initialized == FALSE)
		return;

	initialized = FALSE;

	if (objectTable != NULL)
		S_DELETE(objectTable, "_s_serialize_quit", error);

	if (fileTable != NULL)
		S_DELETE(fileTable, "_s_serialize_quit", error);
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void add_object_class(const SSerializedObjectClass* serializedObjectClass,
							 s_erc *error)
{
	const SMap *objectType;
	const char * const * itr;
	SObject *serializedObject;
	const char *class_name;


	S_CLR_ERR(error);

	for (itr = serializedObjectClass->object_types; *itr != NULL; itr++)
	{
		objectType = S_MAP(SMapGetObjectDef(objectTable, *itr, NULL, error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_object_class",
					  "Call to \"SMapGetObjectDef\" failed"))
			return;

		class_name = s_class_name((SObjectClass*)serializedObjectClass, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_object_class",
					  "Call to \"s_class_name\" failed"))
			return;

		if (objectType == NULL)
		{
			/* create and initialize a new map */

			objectType = S_MAP(S_NEW("SMapList", error));
			if (S_CHK_ERR(error, S_CONTERR,
						  "add_object_class",
						  "Failed to create a new map object"))
				return;

			/* add it to the objectTable map */
			SMapSetObject(objectTable, *itr, S_OBJECT(objectType), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "add_object_class",
						  "Failed to add new map object"))
			{
				S_DELETE(objectType, "add_object_class", error);
				return;
			}
		}

		/* create a new object of the SSerializedObjectClass type */
		serializedObject = S_NEW(class_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_object_class",
					  "Failed to create new \"%s\" type object",
					  class_name))
			return;

		/* add it to the objectType map with given format */
		SMapSetObject(S_MAP(objectType), serializedObjectClass->format, serializedObject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_object_class",
					  "Failed to add new format object to map"))
		{
			S_DELETE(serializedObject, "add_object_class", error);
			return;
		}
	}
}


static void free_object_class(const SSerializedObjectClass* serializedObjectClass,
							  s_erc *error)
{
	const SMap *objectType;
	const char * const * itr;
	s_bool object_present;


	S_CLR_ERR(error);

	for (itr = serializedObjectClass->object_types; *itr != NULL; itr++)
	{
		objectType = S_MAP(SMapGetObjectDef(objectTable, *itr, NULL, error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "free_object_class",
					  "Call to \"SMapGetObjectDef\" failed"))
			return;

		if (objectType == NULL)
		{
			/* not registered */
			continue;
		}

		/* get the object of the SSerializedObjectClass format */
		object_present = SMapObjectPresent(objectType, serializedObjectClass->format, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "free_object_class",
					  "Call to \"SMapObjectPresent\" failed"))
			return;

		if (!object_present)
		{
			/* not registered */
			continue;
		}

		/* delete it */
		SMapObjectDelete(S_MAP(objectType), serializedObjectClass->format, error);
		S_CHK_ERR(error, S_CONTERR,
				  "free_object_class",
				  "Call to \"SMapObjectDelete\" failed");
	}
}


static const SSerializedObject *get_object_formatter(const SObject* object,
													 const char *format,
													 s_erc *error)
{
	const SSerializedObject *objectFormatter;
	const char *object_type;
	const SMap *objectType;
	s_bool object_present;


	S_CLR_ERR(error);

	object_type = SObjectType(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_object_formatter",
				  "Call to \"SObjectType\" failed"))
		return NULL;

	object_present = SMapObjectPresent(objectTable, object_type, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_object_formatter",
				  "Call to \"SMapObjectPresent\" failed"))
		return NULL;

	if (!object_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "get_object_formatter",
				  "No formatters registered for \'%s\' object type",
				  object_type);
		return NULL;
	}

	objectType = (const SMap*)SMapGetObject(objectTable, object_type, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_object_formatter",
				  "Call to \"SMapGetObject\" failed"))
		return NULL;

	object_present = SMapObjectPresent(objectType, format, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_object_formatter",
				  "Call to \"SMapObjectPresent\" failed"))
		return NULL;

	if (!object_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "get_object_formatter",
				  "Format \'%s\' not available for \'%s\' object type",
				  format, object_type);
		return NULL;
	}

	objectFormatter = (const SSerializedObject*)SMapGetObject(objectType, format, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_object_formatter",
				  "Call to \"SMapGetObject\" failed"))
		return NULL;

	return objectFormatter;
}


static s_bool is_object_present(const char *object_class_name, const char *format,
								s_erc *error)
{
	const SMap *objectType;
	s_bool object_present;


	S_CLR_ERR(error);

	object_present = SMapObjectPresent(objectTable, object_class_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "is_object_present",
				  "Call to \"SMapObjectPresent\" failed"))
		return FALSE;

	if (!object_present)
		return FALSE;

	objectType = (const SMap*)SMapGetObject(objectTable, object_class_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "is_object_present",
				  "Call to \"SMapGetObject\" failed"))
		return FALSE;

	object_present = SMapObjectPresent(objectType, format, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "is_object_present",
				  "Call to \"SMapObjectPresent\" failed"))
		return FALSE;

	return object_present;
}


static void add_file_class(const SSerializedFileClass* serializedFileClass,
						   s_erc *error)
{
	SObject *serializedFile;
	const char *class_name;


	S_CLR_ERR(error);

	class_name = s_class_name((SObjectClass*)serializedFileClass, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_file_class",
				  "Call to \"s_class_name\" failed"))
		return;

	/* create a new object of the SSerializedFileClass type */
	serializedFile = S_NEW(class_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_file_class",
				  "Failed to create new \"%s\" type file",
				  class_name))
		return;

	/* add it to the fileTable map with given format */
	SMapSetObject(fileTable, serializedFileClass->format, serializedFile, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_file_class",
				  "Failed to add new format to map"))
	{
		S_DELETE(serializedFile, "add_file_class", error);
		return;
	}
}


static void free_file_class(const SSerializedFileClass* serializedFileClass,
							s_erc *error)
{
	s_bool object_present;


	S_CLR_ERR(error);

	/* get the object of the SSerializedFileClass format */
	object_present = SMapObjectPresent(fileTable, serializedFileClass->format, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "free_file_class",
				  "Call to \"SMapObjectPresent\" failed"))
		return;

	if (!object_present)
	{
		/* not registered */
		return;
	}

	/* delete it */
	SMapObjectDelete(fileTable, serializedFileClass->format, error);
	S_CHK_ERR(error, S_CONTERR,
			  "free_file_class",
			  "Call to \"SMapObjectDelete\" failed");
}


static const SSerializedFile *get_file_object(const char *format, s_erc *error)
{
	s_bool object_present;
	const SSerializedFile *fileObject;


	S_CLR_ERR(error);

	/* get the serializedFileClass object */
	object_present = SMapObjectPresent(fileTable, format, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_file_object",
				  "Call to \"SMapObjectPresent\" failed"))
		return NULL;

	if (!object_present)
	{
		/* no handler registered for given format */
		S_CTX_ERR(error, S_FAILURE,
				  "get_file_object",
				  "No handler registered for given format \'%s\'", format);
		return NULL;
	}

	fileObject = (const SSerializedFile*)SMapGetObject(fileTable, format, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_file_object",
				  "Call to \"SMapGetObject\" failed"))
		return NULL;

	return fileObject;
}

