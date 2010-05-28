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
/* Serialized object writer functions for Python object data types                  */
/* in ebml format.                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "pobject.h"
#include "ebml.h"
#include "element_id.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void write_python_object(SEbmlWrite *writer, const SObject *object,
								 PyObject *dumps_function, s_erc *error)
{
	const SPyObject *self = (SPyObject*)object;
	PyObject *pickled_string;
	char *buf;


	S_CLR_ERR(error);


	/* start S_EBML_ID_OBJECT_DATA container */
	S_EBMLWRITE_CALL(writer, start_container)(writer, S_EBML_ID_OBJECT_DATA,
											  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_python_object",
				  "Call to SEbmlWrite method \"start_container\" failed"))
		return;

	/* if the python object is NULL we stop, and return */
	if (self->pobject == NULL)
	{
		/* stop S_EBML_ID_OBJECT_DATA container */
		S_EBMLWRITE_CALL(writer, stop_container)(writer, error);
		S_CHK_ERR(error, S_CONTERR,
				  "write_python_object",
				  "Call to SEbmlWrite method \"stop_container\" failed");
		return;
	}

	/* pickle the object */
	pickled_string = PyObject_CallFunctionObjArgs(dumps_function, self->pobject, NULL);

	if (pickled_string == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "write_python_object",
				  "Call to \"PyObject_CallFunctionObjArgs\" failed");
		S_EBMLWRITE_CALL(writer, stop_container)(writer, NULL);
		return;
	}

	/* get c char buffer of string */
	buf = PyString_AsString(pickled_string);
	if (buf == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "write_python_object",
				  "Call to \"PyString_AsString\" failed");
		Py_XDECREF(pickled_string);
		S_EBMLWRITE_CALL(writer, stop_container)(writer, NULL);
		return;
	}

	/* write pickled string */
	S_EBMLWRITE_CALL(writer, write_utf8)(writer, S_PYTHON_OBJECT_DATA_EBML,
										 buf, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_python_object",
				  "Call to SEbmlWrite method \"write_utf8\" failed"))
	{
		Py_XDECREF(pickled_string);
		S_EBMLWRITE_CALL(writer, stop_container)(writer, NULL);
		return;
	}

	/* release */
	Py_XDECREF(pickled_string);

	/* finally close S_EBML_ID_OBJECT_DATA container and return */
	S_EBMLWRITE_CALL(writer, stop_container)(writer, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_python_object",
			  "Call to SEbmlWrite method \"stop_container\" failed");
}
