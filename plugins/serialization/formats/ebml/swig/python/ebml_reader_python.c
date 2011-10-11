/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
/* AUTHOR  : Richard Carlson, Aby Louw                                              */
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SEbmlRead Python wrapper.                            */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Extend the SEbmlRead class                                                       */
/*                                                                                  */
/************************************************************************************/

%extend SEbmlRead
{
	PyObject *read_uint(s_erc *error)
	{
		PyObject *tuple;
		PyObject *object;
		uint32 val;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_uint)($self, &id, error);
		if (*error != S_SUCCESS)
			return NULL;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_uint",
					  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = PyLong_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);

		object = PyLong_FromLong((ulong) val);
		PyTuple_SET_ITEM(tuple, 1, object);

		return tuple;
	}


	PyObject *read_sint(s_erc *error)
	{
		PyObject *tuple;
		PyObject *object;
		sint32 val;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_sint)($self, &id, error);
		if (*error != S_SUCCESS)
			return NULL;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_sint",
					  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = PyLong_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);

		object = PyLong_FromLong((long) val);
		PyTuple_SET_ITEM(tuple, 1, object);

		return tuple;
	}


	PyObject *read_double(s_erc *error)
	{
		PyObject *tuple;
		PyObject *object;
		double val;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_double)($self, &id, error);
		if (*error != S_SUCCESS)
			return NULL;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_double",
					  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = PyLong_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);

		object = PyFloat_FromDouble(val);
		PyTuple_SET_ITEM(tuple, 1, object);

		return tuple;
	}


	PyObject *read_str(s_erc *error)
	{
		PyObject *tuple;
		PyObject *object;
		char *val;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_utf8)($self, &id, error);
		if (*error != S_SUCCESS)
			return NULL;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_str",
					  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = PyLong_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);

		object = s_set_pyobject_str(val, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_str",
					  "Call to \"s_set_pyobject_str\" failed"))
		{
			S_FREE(val);
			Py_XDECREF(tuple);
			return NULL;
		}

		S_FREE(val);
		PyTuple_SET_ITEM(tuple, 1, object);

		return tuple;
	}


	PyObject *read_object(s_erc *error)
	{
		SObject *val;
		PyObject *tuple;
		PyObject *object;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_object)($self, &id, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_object",
					  "Call to SEbmlRead method \"read_object\" failed"))
			return NULL;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_str",
					  "Call to \"PyTuple_New\" failed");
			S_DELETE(val, "read_object", error);
			return NULL;
		}

		/* Python owns object, TRUE */
		object = s_sobject_2_pyobject(val, TRUE, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_object",
					  "Call to SEbmlRead method \"read_object\" failed"))
		{
			S_DELETE(val, "read_object", error);
			Py_DECREF(tuple);
			return NULL;
		}

		PyTuple_SET_ITEM(tuple, 1, object);

		object = PyLong_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);
		return tuple;
	}
};


