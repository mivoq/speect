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


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "serialization/serialized_file.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given #SSerializedFile child class object as a #SSerializedFile object.
 * @param SELF The given object.
 * @return Given object as #SSerializedFile* type.
 * @note This casting is not safety checked.
 */
#define S_SERIALIZED_FILE(SELF)    ((SSerializedFile *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SSerializedFile.
 * @param SELF The given #SSerializedFile*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage:
 @verbatim
 S_SERIALIZED_FILE_CALL(self, func)(param1, param2, ..., paramN);
 @endverbatim
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_SERIALIZED_FILE_CALL(SELF, FUNC)				\
	((SSerializedFileClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SSerializedFile
 * can be called.
 * @param SELF The given #SSerializedFile*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_SERIALIZED_FILE_METH_VALID(SELF, FUNC)		\
	S_SERIALIZED_FILE_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSerializedFileClass SerializedFileClass; /*  Serialized file class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SObject *SSerializedFileLoad(const SSerializedFile *self, const char *path,
									 s_erc *error)
{
	SObject *loaded;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedFileLoad",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedFileLoad",
				  "Argument \"path\" is NULL");
		return NULL;
	}

	if (!S_SERIALIZED_FILE_METH_VALID(self, load))
	{
		S_WARNING(S_METHINVLD,
				  "SSerializedFileLoad",
				  "Serialized file method \"load\" not implemented");
		return NULL;
	}

	loaded = S_SERIALIZED_FILE_CALL(self, load)(path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SSerializedFileLoad",
				  "Class method \"load\" failed"))
		return NULL;

	return loaded;
}


S_LOCAL void SSerializedFileSave(const SSerializedFile *self, const SObject *object,
								 const char *path, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedFileSave",
				  "Argument \"self\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedFileSave",
				  "Argument \"object\" is NULL");
		return;
	}

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedFileSave",
				  "Argument \"path\" is NULL");
		return;
	}

	if (!S_SERIALIZED_FILE_METH_VALID(self, save))
	{
		S_WARNING(S_METHINVLD,
				  "SSerializedFileSave",
				  "Serialized file method \"save\" not implemented");
		return;
	}

	S_SERIALIZED_FILE_CALL(self, save)(object, path, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SSerializedFileSave",
			  "Class file method \"save\" failed");
}

S_LOCAL void SSerializedFileSaveToDatasource(const SSerializedFile *self, const SObject *object,
	SDatasource *ds, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			"SSerializedFileSaveToDatasource",
			"Argument \"self\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			"SSerializedFileSaveToDatasource",
			"Argument \"object\" is NULL");
		return;
	}

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			"SSerializedFileSaveToDatasource",
			"Argument \"path\" is NULL");
		return;
	}

	if (!S_SERIALIZED_FILE_METH_VALID(self, save_to_datasource))
	{
		S_WARNING(S_METHINVLD,
			"SSerializedFileSaveToDatasource",
			"Serialized file method \"save_to_datasource\" not implemented");
		return;
	}

	S_SERIALIZED_FILE_CALL(self, save_to_datasource)(object, ds, error);
	S_CHK_ERR(error, S_CONTERR,
		"SSerializedFileSaveToDatasource",
		"Class file method \"save_to_datasource\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_serialized_file_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&SerializedFileClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_serialized_file_class_add",
			  "Failed to add SSerializedFileClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void DisposeSerializedFile(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SDatasource class initialization                                                 */
/*                                                                                  */
/************************************************************************************/

static SSerializedFileClass SerializedFileClass =
{
	/* SObjectClass */
	{
		"SSerializedFile",
		sizeof(SSerializedFile),
		{ 0, 1},
		NULL,                  /* init    */
		NULL,                  /* destroy */
		DisposeSerializedFile, /* dispose */
		NULL,                  /* compare */
		NULL,                  /* print   */
		NULL,                  /* copy    */
	},
	/* SSerializedFileClass */
	NULL,                      /* format  */
	NULL,                      /* load    */
	NULL                       /* save    */
};


