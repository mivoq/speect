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
/* DATE    : June 2010                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SListPy test.                                                                    */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "Python.h"
#include "speect.h"
#include "py_native.h"




static SList *create_and_populate_py_list(s_erc *error)
{
	SListPy *pyList;
	PyObject *list;
	PyObject *tmp;


	S_CLR_ERR(error);
	list = PyList_New(3);
	if (list == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"PyList_New\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"PyList_New\" failed");
		}

		goto error;
	}


	/* create some pure Python objects and put
	 * them into the list
	 */
	tmp = PyLong_FromLong(12);
	if (tmp == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"PyLong_FromLong\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"PyLong_FromLong\" failed");
		}

		goto error;
	}

	if (PyList_SetItem(list, 0, tmp) != 0)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"PyList_SetItem\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"PyList_SetItem\" failed");
		}

		goto error;
	}

	tmp = PyFloat_FromDouble(-3287.987);
	if (tmp == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"PyFloat_FromDouble\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"PyFloat_FromDouble\" failed");
		}

		goto error;
	}

	if (PyList_SetItem(list, 1, tmp) != 0)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"PyList_SetItem\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"PyList_SetItem\" failed");
		}

		goto error;
	}


#if PY_VERSION_HEX >= 0x03000000
	tmp = PyUnicode_FromString("hello world");
#define SPCT_TEST_STR_FUNC "PyUnicode_FromString"
#else /* ! PY_VERSION_HEX >= 0x03000000 */
	tmp = PyString_FromString("hello world");
#define SPCT_TEST_STR_FUNC "PyString_FromString"
#endif /* PY_VERSION_HEX >= 0x03000000 */
	if (tmp == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"%s\" failed. Reported error: %s",
					  SPCT_TEST_STR_FUNC, py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"%s\" failed",
					  SPCT_TEST_STR_FUNC);
		}

		goto error;
	}

	if (PyList_SetItem(list, 2, tmp) != 0)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"PyList_SetItem\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_and_populate_py_list",
					  "Call to \"PyList_SetItem\" failed");
		}

		goto error;
	}

	/* create SListPy wrapper */
	pyList = (SListPy*)S_NEW("SListPy", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_and_populate_py_list",
				  "Failed to create new 'SListPy' object"))
		goto error;

	/* initialize with Python list */
	SListPyInit(&pyList, list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_and_populate_py_list",
				  "Call to \"SListPyInit\" failed"))
		goto error;

	/* all ok */
	return S_LIST(pyList);

error:
	Py_XDECREF(list);

	return NULL;
}


int main()
{
	s_erc error = S_SUCCESS;
	SList *list = NULL;
	SIterator *itr;
	SObject *a = NULL;
	SObject *b = NULL;
	SObject *c = NULL;
	PyObject *speectModule = NULL; /* for SWIG functions to work */


	/* Initialize the Python interpreter.  Required. */
    Py_Initialize();

	/* import speect python module */
	speectModule = PyImport_ImportModule("speect");
	if (speectModule == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(&error, S_FAILURE,
					  "main",
					  "Call to \"PyImport_ImportModule\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(&error, S_FAILURE,
					  "main",
					  "Call to \"PyImport_ImportModule\" failed");
		}

		goto quit;
	}

	/*
	 * initialize speect python native module
	 */
	error = s_python_native_objects_init();
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect Python native module\n");
		goto quit;
	}

	/* Create and populate Python list */
	list = create_and_populate_py_list(&error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"create_and_populate_py_list\" failed"))
		goto quit;

	/*
	 * get iterator to list, should be NULL as there are no objects
	 * in the list
	 */
	itr = S_ITERATOR_GET(list, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to get iterator to list"))
		goto quit;

	/* Create some objects and put the into the list */
	a = SObjectSetInt(10, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to set int object"))
		goto quit;

	b = SObjectSetFloat(3.14, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to set float object"))
		goto quit;

	c = SObjectSetString("python list test", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to set string object"))
		goto quit;

	SListPush(list, a, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to add object to list"))
		goto quit;
	else
		a = NULL; /* object belongs to list now, we don't want to
				   * delete it directly.
				   */

	SListPush(list, b, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to add object to list"))
		goto quit;
	else
		b = NULL; /* object belongs to list now, we don't want to
				   * delete it directly.
				   */

	SListPush(list, c, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to add object to list"))
		goto quit;
	else
		c = NULL; /* object belongs to list now, we don't want to
				   * delete it directly.
				   */

	/*
	 * get iterator to list, should not be NULL as there are now
	 * objects in the list
	 */
	itr = S_ITERATOR_GET(list, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to get iterator to list"))
		goto quit;

	/* iterate through list objects and print them to stdout */
	for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
	{
		char *buf;
		const SObject *tmp;


		tmp = SIteratorObject(itr, &error);
		if (S_CHK_ERR(&error,  S_CONTERR,
					  "main",
					  "Failed to get list iterator object"))
			goto quit;

		buf = SObjectPrint(tmp, &error);
		if (S_CHK_ERR(&error,  S_CONTERR,
					  "main",
					  "Failed to print  object"))
			goto quit;

		printf("list object = %s\n", buf);
		S_FREE(buf);
	}

quit:
	if (list != NULL)
		S_DELETE(list, "main", &error);

	if (itr != NULL)
		S_DELETE(itr, "main", &error);

	if (a != NULL)
		S_DELETE(a, "main", &error);

	if (b != NULL)
		S_DELETE(b, "main", &error);

	if (c != NULL)
		S_DELETE(c, "main", &error);

	Py_XDECREF(speectModule);
	Py_Finalize();

	return 0;
}


