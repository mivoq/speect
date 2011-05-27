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


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "serialization/serialized_object.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given #SSerializedObject child class object as a #SSerializedObject object.
 * @param SELF The given object.
 * @return Given object as #SSerializedObject* type.
 * @note This casting is not safety checked.
 */
#define S_SERIALIZED_OBJECT(SELF)    ((SSerializedObject *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SSerializedObject.
 * @param SELF The given #SSerializedObject*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage:
 @verbatim
 S_SERIALIZED_OBJECT_CALL(self, func)(param1, param2, ..., paramN);
 @endverbatim
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_SERIALIZED_OBJECT_CALL(SELF, FUNC)				\
	((SSerializedObjectClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SSerializedObject
 * can be called.
 * @param SELF The given #SSerializedObject*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_SERIALIZED_OBJECT_METH_VALID(SELF, FUNC)		\
	S_SERIALIZED_OBJECT_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSerializedObjectClass SerializedObjectClass; /*  Serialized object class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SSerializedObjectRead(const SSerializedObject *self, SDataReader *reader,
								 SObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedObjectRead",
				  "Argument \"self\" is NULL");
		return;
	}

	if (reader == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedObjectRead",
				  "Argument \"reader\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedObjectRead",
				  "Argument \"object\" is NULL");
		return;
	}

	if (!S_SERIALIZED_OBJECT_METH_VALID(self, read))
	{
		S_WARNING(S_METHINVLD,
				  "SSerializedObjectRead",
				  "Serialized object method \"read\" not implemented");
		return;
	}

	S_SERIALIZED_OBJECT_CALL(self, read)(reader, object, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SSerializedObjectRead",
			  "Class method \"read\" failed");
}


S_API void SSerializedObjectWrite(const SSerializedObject *self, SDataWriter *writer,
								  const SObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedObjectWrite",
				  "Argument \"self\" is NULL");
		return;
	}

	if (writer == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedObjectWrite",
				  "Argument \"writer\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SSerializedObjectWrite",
				  "Argument \"object\" is NULL");
		return;
	}

	if (!S_SERIALIZED_OBJECT_METH_VALID(self, write))
	{
		S_WARNING(S_METHINVLD,
				  "SSerializedObjectWrite",
				  "Serialized object method \"write\" not implemented");
		return;
	}

	S_SERIALIZED_OBJECT_CALL(self, write)(writer, object, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SSerializedObjectWrite",
			  "Class object method \"write\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_serialized_object_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&SerializedObjectClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_serialized_object_class_add",
			  "Failed to add SSerializedObjectClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void DisposeSerializedObject(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SDatasource class initialization                                                 */
/*                                                                                  */
/************************************************************************************/

static SSerializedObjectClass SerializedObjectClass =
{
	/* SObjectClass */
	{
		"SSerializedObject",
		sizeof(SSerializedObject),
		{ 0, 1},
		NULL,                    /* init    */
		NULL,                    /* destroy */
		DisposeSerializedObject, /* dispose */
		NULL,                    /* compare */
		NULL,                    /* print   */
		NULL,                    /* copy    */
	},
	/* SSerializedObjectClass */
	NULL,                      /* format       */
	NULL,                      /* object_types */
	NULL,                      /* read         */
	NULL                       /* write        */
};


