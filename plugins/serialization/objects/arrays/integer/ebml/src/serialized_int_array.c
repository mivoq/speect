/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SSerializedObjectClass implementation for integer array data                     */
/* types in EBML format.                                                            */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "serialized_int_array.h"


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

typedef SSerializedObject SEbmlArrayInt;

typedef SSerializedObjectClass SEbmlArrayIntClass;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SEbmlArrayIntClass EbmlArrayIntClass; /* SEbmlArrayInt class declaration. */

static const char *object_types[] =
{
	"SArrayInt",
	NULL
};


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_serialized_ebml_array_int_reg(s_erc *error)
{
	S_CLR_ERR(error);

	s_class_reg(&EbmlArrayIntClass, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_ebml_array_int_reg",
				  "Failed to register SEbmlArrayIntClass"))
		return;

	SSerializedObjectRegister(&EbmlArrayIntClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_serialized_ebml_array_int_reg",
			  "Failed to add serialized object class SEbmlArrayIntClass");
}


S_LOCAL void _s_serialized_ebml_array_int_free(s_erc *error)
{
	s_erc local_err;


	S_CLR_ERR(&local_err);
	S_CLR_ERR(error);

	SSerializedObjectFree(&EbmlArrayIntClass, &local_err);
	S_CHK_ERR(&local_err, S_CONTERR,
			  "_s_serialized_ebml_array_int_free",
			  "Failed to remove serialized object class SEbmlArrayIntClass");

	s_class_free(&EbmlArrayIntClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_serialized_ebml_array_int_free",
			  "Failed to free SEbmlArrayIntClass");

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


	S_CLR_ERR(error);

	ebmlReader = S_CAST(reader, SEbmlRead, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Read",
				  "Call to \"S_CAST (SEbmlRead)\" failed for SDataReader object"))
		return;

	read_int_array(ebmlReader, object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Read",
				  "Call to \"read_int_array\" failed"))
		return;
}


static void Write(SDataWriter *writer, const SObject *object, s_erc *error)
{
	SEbmlWrite *ebmlWriter;


	S_CLR_ERR(error);

	ebmlWriter = S_CAST(writer, SEbmlWrite, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Write",
				  "Call to \"S_CAST (SEbmlWrite)\" failed for SDataWriter object"))
		return;

	write_int_array(ebmlWriter, object, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Write",
			  "Call to \"write_int_array\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* SEbmlArrayInt class initialization                                               */
/*                                                                                  */
/************************************************************************************/

static SEbmlArrayIntClass EbmlArrayIntClass =
{
	/* SObjectClass */
	{
		"SSerializedObject:SEbmlArrayInt",
		sizeof(SEbmlArrayInt),
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


