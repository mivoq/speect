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
/* SSerializedObjectClass implementation for primitives data types in EBML format.  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "serialized_primitives.h"


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

typedef SSerializedObject SEbmlPrimitives;

typedef SSerializedObjectClass SEbmlPrimitivesClass;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SEbmlPrimitivesClass EbmlPrimitivesClass; /* SEbmlPrimitives class declaration. */

static const char *object_types[] =
{
	"SInt",
	"SFloat",
	"SString",
	NULL
};


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_serialized_ebml_primitives_reg(s_erc *error)
{
	S_CLR_ERR(error);

	s_class_reg(S_OBJECTCLASS(&EbmlPrimitivesClass), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_ebml_primitives_reg",
				  "Failed to register SEbmlPrimitivesClass"))
		return;

	SSerializedObjectRegister(&EbmlPrimitivesClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_serialized_ebml_primitives_reg",
			  "Failed to add serialized object class SEbmlPrimitivesClass");
}


S_LOCAL void _s_serialized_ebml_primitives_free(s_erc *error)
{
	s_erc local_err;


	S_CLR_ERR(&local_err);
	S_CLR_ERR(error);

	SSerializedObjectFree(&EbmlPrimitivesClass, &local_err);
	S_CHK_ERR(&local_err, S_CONTERR,
			  "_s_serialized_ebml_primitives_free",
			  "Failed to remove serialized object class SEbmlPrimitivesClass");

	s_class_free(S_OBJECTCLASS(&EbmlPrimitivesClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_serialized_ebml_primitives_free",
			  "Failed to free SEbmlPrimitivesClass");

	if ((local_err != S_SUCCESS)
		&& (error != NULL)
		&& (*error == S_SUCCESS))
		*error = local_err;
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void Read(SDataReader *reader, SObject *object, s_erc *error)
{
	SEbmlRead *ebmlReader;
	const char *object_type;
	int rv;


	S_CLR_ERR(error);

	ebmlReader = S_CAST(reader, SEbmlRead, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Read",
				  "Call to \"S_CAST (SEbmlRead)\" failed for SDataReader object"))
		return;

	object_type = SObjectType(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Read",
				  "Call to \"SObjectType\" failed"))
		return;


	/* SInt */
	rv = s_strcmp(object_type, "SInt", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Read",
				  "Call to \"s_strcmp\" failed"))
		return;

	if (rv == 0)
	{
		read_int(ebmlReader, object, error);
		S_CHK_ERR(error, S_CONTERR,
				  "Read",
				  "Call to \"read_int\" failed");
		return;
	}

	/* SFloat */
	rv = s_strcmp(object_type, "SFloat", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Read",
				  "Call to \"s_strcmp\" failed"))
		return;

	if (rv == 0)
	{
		read_float(ebmlReader, object, error);
		S_CHK_ERR(error, S_CONTERR,
				  "Read",
				  "Call to \"read_float\" failed");
		return;
	}


	/* SString */
	rv = s_strcmp(object_type, "SString", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Read",
				  "Call to \"s_strcmp\" failed"))
		return;

	if (rv == 0)
	{
		read_string(ebmlReader, object, error);
		S_CHK_ERR(error, S_CONTERR,
				  "Read",
				  "Call to \"read_string\" failed");
		return;
	}

	/* should not get here */
	S_CTX_ERR(error, S_FAILURE,
			  "Read",
			  "SEbmlPrimitivesClass cannot read object of type '%s'",
			  object_type);
}


static void Write(SDataWriter *writer, const SObject *object, s_erc *error)
{
	SEbmlWrite *ebmlWriter;
	const char *object_type;
	int rv;


	S_CLR_ERR(error);

	ebmlWriter = S_CAST(writer, SEbmlWrite, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Write",
				  "Call to \"S_CAST (SEbmlWrite)\" failed for SDataWriter object"))
		return;

	object_type = SObjectType(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Write",
				  "Call to \"SObjectType\" failed"))
		return;


	/* SInt */
	rv = s_strcmp(object_type, "SInt", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Write",
				  "Call to \"s_strcmp\" failed"))
		return;

	if (rv == 0)
	{
		write_int(ebmlWriter, object, error);
		S_CHK_ERR(error, S_CONTERR,
				  "Write",
				  "Call to \"write_int\" failed");
		return;
	}

	/* SFloat */
	rv = s_strcmp(object_type, "SFloat", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Write",
				  "Call to \"s_strcmp\" failed"))
		return;

	if (rv == 0)
	{
		write_float(ebmlWriter, object, error);
		S_CHK_ERR(error, S_CONTERR,
				  "Write",
				  "Call to \"write_float\" failed");
		return;
	}


	/* SString */
	rv = s_strcmp(object_type, "SString", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Write",
				  "Call to \"s_strcmp\" failed"))
		return;

	if (rv == 0)
	{
		write_string(ebmlWriter, object, error);
		S_CHK_ERR(error, S_CONTERR,
				  "Write",
				  "Call to \"write_string\" failed");
		return;
	}

	/* should not get here */
	S_CTX_ERR(error, S_FAILURE,
			  "Write",
			  "SEbmlPrimitivesClass cannot write object of type '%s'",
			  object_type);
}


/************************************************************************************/
/*                                                                                  */
/* SDatasource class initialization                                                 */
/*                                                                                  */
/************************************************************************************/

static SEbmlPrimitivesClass EbmlPrimitivesClass =
{
	/* SObjectClass */
	{
		"SSerializedObject:SEbmlPrimitives",
		sizeof(SEbmlPrimitives),
		{ 0, 1},
		NULL,                    /* init    */
		NULL,                    /* destroy */
		Dispose,                 /* dispose */
		NULL,                    /* compare */
		NULL,                    /* print   */
		NULL,                    /* copy    */
	},
	/* SSerializedObjectClass */
	"spct_ebml",               /* format       */
	object_types,              /* object_types */
	Read,                      /* read         */
	Write                      /* write        */
};


