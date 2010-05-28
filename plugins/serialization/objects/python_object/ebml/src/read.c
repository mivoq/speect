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
/* Serialized object reader functions for Python object data types                  */
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

S_LOCAL void read_python_object(SEbmlRead *reader, SObject *object,
								PyObject *loads_function, s_erc *error)
{
	SPyObject *self = (SPyObject*)object;
	uint32 id;
	s_bool container_exhausted;


	S_CLR_ERR(error);

	/* read S_EBML_ID_OBJECT_DATA container, id is checked in the
	 * 'read_object' method of the SEbmlRead class
	 */
	id = S_EBMLREAD_CALL(reader, container)(reader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_python_object",
				  "Call to SEbmlRead \"container\" failed"))
		return;

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(reader, container_at_end)(reader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_python_object",
					  "Call to SEbmlRead \"container_at_end\" failed"))
			return;

		if (container_exhausted)
			break; /* we are finished reading the python object */

		/* peek id for python object elements */
		id = S_EBMLREAD_CALL(reader, peek_id)(reader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_python_object",
					  "Call to SEbmlRead \"peek_id\" failed"))
			return;

		switch(id)
		{
		case S_PYTHON_OBJECT_DATA_EBML:
		{
			char *buf;
			PyObject *pickled_string;
			size_t slen;


			/* read the pickled string */
			buf = S_EBMLREAD_CALL(reader, read_utf8)(reader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_python_object",
						  "Call to SEbmlRead \"read_utf8\" failed"))
				return;

			/* NULL valued Python object */
			if (buf == NULL)
			{
				self->pobject = NULL;
				break;
			}

			/* create a Python object from the pickled string */
			slen = s_strsize(buf, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_python_object",
						  "Call to \"s_strsize\" failed"))
			{
				S_FREE(buf);
				return;
			}

			pickled_string = PyString_FromStringAndSize(buf, slen);
			S_FREE(buf);

			if (pickled_string == NULL)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "read_python_object",
						  "Call to \"PyString_FromStringAndSize\" failed");
				return;
			}

			self->pobject = PyObject_CallFunctionObjArgs(loads_function, pickled_string, NULL);
			Py_XDECREF(pickled_string);

			if (self->pobject == NULL)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "read_python_object",
						  "Call to \"PyObject_CallFunctionObjArgs\" failed");
				return;
			}

			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_python_object",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(reader, element_skip)(reader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_python_object",
						  "Call to SEbmlRead \"element_skip\" failed"))
				return;
		}
	}
}


