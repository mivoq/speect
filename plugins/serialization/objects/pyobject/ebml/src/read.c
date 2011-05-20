/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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

#include "ebml.h"
#include "element_id.h"
#include "py_native.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

/* read any type of PyObject and "unpickle" */
static PyObject *read_pickled_pyobject(SEbmlRead *reader, PyObject *loads_function,
									   s_erc *error);


/* these functions set the Python object in the Speect object wrappers */

static void finalize_smappy(SObject *object, PyObject *pobject, s_erc *error);

static void finalize_slistpy(SObject *object, PyObject *pobject, s_erc *error);

static void finalize_spyobject(SObject *object, PyObject *pobject, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void read_pyobject(SEbmlRead *reader, SObject *object,
						   PyObject *loads_function, s_erc *error)
{
	const char *class;
	uint32 id;
	s_bool container_exhausted;


	S_CLR_ERR(error);

	/* get object class */
	class = SObjectType(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_pyobject",
				  "Call to \"SObjectType\" failed"))
		return;

	/* read S_EBML_ID_OBJECT_DATA container, id is checked in the
	 * 'read_object' method of the SEbmlRead class
	 */
	id = S_EBMLREAD_CALL(reader, container)(reader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_pyobject",
				  "Call to SEbmlRead \"container\" failed"))
		return;

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(reader, container_at_end)(reader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_pyobject",
					  "Call to SEbmlRead \"container_at_end\" failed"))
			return;

		if (container_exhausted)
			break; /* we are finished reading the python object */

		/* peek id for python object elements */
		id = S_EBMLREAD_CALL(reader, peek_id)(reader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_pyobject",
					  "Call to SEbmlRead \"peek_id\" failed"))
			return;

		switch(id)
		{
		case S_PYTHON_OBJECT_DATA_EBML:
		{
			PyObject *pobject;
			int s_comp;

			/* read pickled Python object */
			pobject = read_pickled_pyobject(reader, loads_function, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_pyobject",
						  "Call to \"read_pickled_pyobject\" failed"))
				return;

			/* SPyObject */
			s_comp = s_strcmp(class, "SPyObject", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_pyobject",
						  "Call to \"s_strcmp\" failed"))
			{
				Py_XDECREF(pobject);
				return;
			}

			if (s_comp == 0)
			{
				finalize_spyobject(object, pobject, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_pyobject",
							  "Call to \"finalize_spyobject\" failed"))
				{
					Py_XDECREF(pobject);
					return;
				}

				break;
			}


			/* SMapPy */
			s_comp = s_strcmp(class, "SMapPy", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_pyobject",
						  "Call to \"s_strcmp\" failed"))
			{
				Py_XDECREF(pobject);
				return;
			}

			if (s_comp == 0)
			{
				finalize_smappy(object, pobject, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_pyobject",
							  "Call to \"finalize_smappy\" failed"))
				{
					Py_XDECREF(pobject);
					return;
				}

				break;
			}

			/* SListPy */
			s_comp = s_strcmp(class, "SListPy", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_pyobject",
						  "Call to \"s_strcmp\" failed"))
			{
				Py_XDECREF(pobject);
				return;
			}

			if (s_comp == 0)
			{
				finalize_slistpy(object, pobject, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_pyobject",
							  "Call to \"finalize_slistpy\" failed"))
				{
					Py_XDECREF(pobject);
					return;
				}

				break;
			}


			S_CTX_ERR(error, S_FAILURE,
					  "read_pyobject",
					  "Failed to read Python object of type '%s'", class);
			Py_XDECREF(pobject);
			return;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_pyobject",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(reader, element_skip)(reader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_pyobject",
						  "Call to SEbmlRead \"element_skip\" failed"))
				return;
		}
	}
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static PyObject *read_pickled_pyobject(SEbmlRead *reader, PyObject *loads_function,
									   s_erc *error)
{
	PyObject *object;
	PyObject *pickled_string;
	char *buf;
	uint32 id;


	S_CLR_ERR(error);

	/* read the pickled string, id has previously been checked */
	buf = S_EBMLREAD_CALL(reader, read_utf8)(reader, &id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_pickled_pyobject",
				  "Call to SEbmlRead \"read_utf8\" failed"))
		return NULL;

	/* NULL valued Python object */
	if (buf == NULL)
		return NULL;

	/* create a Python object from the pickled string */
	pickled_string = s_set_pyobject_str(buf, error);
	S_FREE(buf);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_pickled_pyobject",
				  "Call to \"s_set_pyobject_str\" failed"))
		return NULL;

	/* call cPickle loads function */
	object = PyObject_CallFunctionObjArgs(loads_function, pickled_string, NULL);
	Py_XDECREF(pickled_string);
	if (object == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_pickled_pyobject",
					  "Call to \"cPickle.loads\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_pickled_pyobject",
					  "Call to \"cPickle.loads\" failed");
		}

		return NULL;
	}

	return object;
}


static void finalize_spyobject(SObject *object, PyObject *pobject, s_erc *error)
{
	SPyObject *self = (SPyObject*)object;


	S_CLR_ERR(error);

	/* we received a new reference, so don't use SPyObjectSet */
	self->pobject = pobject;
}


static void finalize_smappy(SObject *object, PyObject *pobject, s_erc *error)
{
	SMapPy *self = (SMapPy*)object;


	S_CLR_ERR(error);

	/* we received a new reference, so don't use SPyObjectSet */
	self->pyObject->pobject = pobject;
}


static void finalize_slistpy(SObject *object, PyObject *pobject, s_erc *error)
{
	SListPy *self = (SListPy*)object;


	S_CLR_ERR(error);

	/* we received a new reference, so don't use SPyObjectSet */
	self->pyObject->pobject = pobject;
}
