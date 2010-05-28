/************************************************************************************/
/* Copyright (c) 2010 The Department of Arts and Culture,                           */
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
/* DATE    : April 2010                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SSerializedObjectClass implementation for Python object data                     */
/* types in EBML format.                                                            */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "serialized_python_object.h"


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

typedef SSerializedObject SEbmlPyObject;

typedef SSerializedObjectClass SEbmlPyObjectClass;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SEbmlPyObjectClass EbmlPyObjectClass; /* SEbmlPyObject class declaration. */

static const char *object_types[] =
{
	"SPyObject",
	NULL
};

static PyObject *cPickleModule = NULL;

static PyObject *cPickle_loads = NULL;

static PyObject *cPickle_dumps = NULL;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_serialized_ebml_python_object_reg(s_erc *error)
{
	S_CLR_ERR(error);

	cPickleModule = PyImport_ImportModule("cPickle");
	if (cPickleModule == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "_s_serialized_ebml_python_object_reg",
				  "Call to \"PyImport_ImportModule\" failed");
		return;
	}

	cPickle_loads = PyObject_GetAttrString(cPickleModule, "loads");
	if (cPickle_loads == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "_s_serialized_ebml_python_object_reg",
				  "Call to \"PyObject_GetAttrString\" failed");
		Py_XDECREF(cPickleModule);
		return;
	}

	cPickle_dumps = PyObject_GetAttrString(cPickleModule, "dumps");
	if (cPickle_dumps == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "_s_serialized_ebml_python_object_reg",
				  "Call to \"PyObject_GetAttrString\" failed");
		Py_XDECREF(cPickle_loads);
		Py_XDECREF(cPickleModule);
		return;
	}

	s_class_reg(&EbmlPyObjectClass, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_ebml_python_object_reg",
				  "Failed to register SEbmlPyObjectClass"))
	{
		Py_XDECREF(cPickle_loads);
		Py_XDECREF(cPickle_dumps);
		Py_XDECREF(cPickleModule);
		return;
	}

	SSerializedObjectRegister(&EbmlPyObjectClass, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_ebml_python_object_reg",
				  "Failed to add serialized object class SEbmlPyObjectClass"))
	{
		s_class_free(&EbmlPyObjectClass, NULL);
		Py_XDECREF(cPickle_loads);
		Py_XDECREF(cPickle_dumps);
		Py_XDECREF(cPickleModule);
	}
}


S_LOCAL void _s_serialized_ebml_python_object_free(s_erc *error)
{
	s_erc local_err;


	S_CLR_ERR(&local_err);
	S_CLR_ERR(error);

	Py_XDECREF(cPickle_loads);
	Py_XDECREF(cPickle_dumps);
	Py_XDECREF(cPickleModule);

	SSerializedObjectFree(&EbmlPyObjectClass, &local_err);
	S_CHK_ERR(&local_err, S_CONTERR,
			  "_s_serialized_ebml_python_object_free",
			  "Failed to remove serialized object class SEbmlPyObjectClass");

	s_class_free(&EbmlPyObjectClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_serialized_ebml_python_object_free",
			  "Failed to free SEbmlPyObjectClass");

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

	read_python_object(ebmlReader, object, cPickle_loads, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Read",
				  "Call to \"read_python_object\" failed"))
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

	write_python_object(ebmlWriter, object, cPickle_dumps, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Write",
			  "Call to \"write_python_object\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* SEbmlPyObject class initialization                                               */
/*                                                                                  */
/************************************************************************************/

static SEbmlPyObjectClass EbmlPyObjectClass =
{
	/* SObjectClass */
	{
		"SSerializedObject:SEbmlPyObject",
		sizeof(SEbmlPyObject),
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


