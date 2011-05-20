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
/* Serialized object writer functions for Python object data types                  */
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

/* "pickle" any type of PyObject and write */
static void write_pickled_pyobject(SEbmlWrite *writer, PyObject *object,
								   PyObject *dumps_function, s_erc *error);


/* these functions get the Python object from the Speect object wrappers */

static void write_spyobject(SEbmlWrite *writer, const SObject *object,
							PyObject *dumps_function, s_erc *error);

static void write_smappy(SEbmlWrite *writer, const SObject *object,
						 PyObject *dumps_function, s_erc *error);

static void write_slistpy(SEbmlWrite *writer, const SObject *object,
						  PyObject *dumps_function, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void write_pyobject(SEbmlWrite *writer, const SObject *object,
							PyObject *dumps_function, s_erc *error)
{
	const char *class;
	int s_comp;


	S_CLR_ERR(error);

	/* get object class */
	class = SObjectType(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_pyobject",
				  "Call to \"SObjectType\" failed"))
		return;

	/* start S_EBML_ID_OBJECT_DATA container */
	S_EBMLWRITE_CALL(writer, start_container)(writer, S_EBML_ID_OBJECT_DATA,
											  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_pyobject",
				  "Call to SEbmlWrite method \"start_container\" failed"))
		return;

	/* SPyObject */
	s_comp = s_strcmp(class, "SPyObject", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_pyobject",
				  "Call to \"s_strcmp\" failed"))
		return;

	if (s_comp == 0)
	{
		write_spyobject(writer, object, dumps_function, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_pyobject",
					  "Call to \"write_spyobject\" failed"))

			return;

		/* all OK, close container */
		goto finalize;
	}

	/* SMapPy */
	s_comp = s_strcmp(class, "SMapPy", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_pyobject",
				  "Call to \"s_strcmp\" failed"))
		return;

	if (s_comp == 0)
	{
		write_smappy(writer, object, dumps_function, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_pyobject",
					  "Call to \"write_smappy\" failed"))
			return;

		/* all OK, close container */
		goto finalize;
	}

	/* SListPy */
	s_comp = s_strcmp(class, "SListPy", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_pyobject",
				  "Call to \"s_strcmp\" failed"))
		return;

	if (s_comp == 0)
	{
		write_slistpy(writer, object, dumps_function, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_pyobject",
					  "Call to \"write_slistpy\" failed"))
			return;

		/* all OK, close container */
		goto finalize;
	}


	/* if we get here then we don't know how to write the class */
	S_CTX_ERR(error, S_FAILURE,
			  "write_pyobject",
			  "Failed to write Python object of type '%s'", class);
	return;

	/* here all is OK, we close the S_EBML_ID_OBJECT_DATA container
	 * and return
	 */
finalize:
	S_EBMLWRITE_CALL(writer, stop_container)(writer, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_pyobject",
			  "Call to SEbmlWrite method \"stop_container\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void write_pickled_pyobject(SEbmlWrite *writer, PyObject *object,
								   PyObject *dumps_function, s_erc *error)
{
	PyObject *pickled_string;
	char *buf;


	S_CLR_ERR(error);

	/* if the python object is NULL we return */
	if (object == NULL)
		return;

	/* pickle the object */
	pickled_string = PyObject_CallFunctionObjArgs(dumps_function, object, NULL);
	if (pickled_string == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "write_pickled_pyobject",
					  "Call to \"cPickle.dumps\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "write_pickled_pyobject",
					  "Call to \"cPickle.dumps\" failed");
		}

		return;
	}

	/* get c char buffer of string */
	buf = s_get_pyobject_str(pickled_string, error);
	Py_XDECREF(pickled_string);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_pickled_pyobject",
				  "Call to \"s_get_pyobject_str\" failed"))
		return;

	/* write pickled string */
	S_EBMLWRITE_CALL(writer, write_utf8)(writer, S_PYTHON_OBJECT_DATA_EBML,
										 buf, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_pickled_pyobject",
			  "Call to SEbmlWrite method \"write_utf8\" failed");

	S_FREE(buf);
}


static void write_spyobject(SEbmlWrite *writer, const SObject *object,
						   PyObject *dumps_function, s_erc *error)
{
	SPyObject *self = (SPyObject*)object;


	S_CLR_ERR(error);
	write_pickled_pyobject(writer, self->pobject, dumps_function, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_spyobject",
			  "Call to \"write_pickled_pyobject\" failed");
}


static void write_smappy(SEbmlWrite *writer, const SObject *object,
						 PyObject *dumps_function, s_erc *error)
{
	SMapPy *self = (SMapPy*)object;


	S_CLR_ERR(error);
	write_pickled_pyobject(writer, self->pyObject->pobject, dumps_function, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_spyobject",
			  "Call to \"write_pickled_pyobject\" failed");
}


static void write_slistpy(SEbmlWrite *writer, const SObject *object,
						  PyObject *dumps_function, s_erc *error)
{
	SListPy *self = (SListPy*)object;


	S_CLR_ERR(error);
	write_pickled_pyobject(writer, self->pyObject->pobject, dumps_function, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_spyobject",
			  "Call to \"write_pickled_pyobject\" failed");
}
