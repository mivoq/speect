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
/* C convenience functions for SFeatProcessorCB Python wrapper.                     */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%include "exception.i"


/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	/*
	 * FIXME: SVoid_PyObject_free and pyobject_2_sobject come from
	 * speect/engine/swig/python/primitives.c
	 * Don't know how to include them here.
	 */

	SObject *pyobject_2_sobject(PyObject *pobject, s_erc *error)
	{
		SObject *object;
		int res;
		void *argp;


		if (pobject == Py_None)
			return NULL;

		res = SWIG_ConvertPtr(pobject, &argp,SWIGTYPE_p_SObject, SWIG_POINTER_DISOWN|0);
		if (SWIG_IsOK(res))
		{
			/* it's a speect object */
			object = (SObject*)argp;
			return object;
		}

		/* it's a python object */
		if (PyObject_IsInstance(pobject, (PyObject*)&PyInt_Type))
		{
			sint32 tmp;


			tmp = (sint32)PyLong_AsLong(pobject);
			object = SObjectSetInt(tmp, error);
			if (*error != S_SUCCESS)
				return NULL;

			return object;
		}

		if (PyObject_IsInstance(pobject, (PyObject*)&PyFloat_Type))
		{
			float tmp;


			tmp = (float)PyFloat_AsDouble(pobject);
			object = SObjectSetFloat(tmp, error);
			if (*error != S_SUCCESS)
				return NULL;

			return object;
		}

		if (PyObject_IsInstance(pobject, (PyObject*)&PyString_Type))
		{
			const char *tmp;


			tmp = PyString_AsString(pobject);
			object = SObjectSetString(tmp, error);
			if (*error != S_SUCCESS)
				return NULL;

			return object;
		}

		if (PyObject_IsInstance(pobject, (PyObject*)&PyUnicode_Type))
		{
			PyObject *ustring;
			const char *tmp;


			ustring = PyUnicode_AsUTF8String(pobject);
			if (ustring == NULL)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "pyobject_2_sobject",
						  "Call to \"PyUnicode_AsUTF8String\" failed");
				return NULL;
			}

			tmp = PyString_AsString(ustring);
			object = SObjectSetString(tmp, error);
			if (*error != S_SUCCESS)
				return NULL;

			Py_XDECREF(ustring);
			return object;
		}

		/*
		 * Not a simple object, make a SPyObject SVoid type, note
		 * that the SPyObject plug-in must be loaded for this to work,
		 *
		 * >>> import speect.pyobject
		 *
		 */
		object = SObjectSetVoid("SPyObject", (void*)pobject, error);
		if (*error != S_SUCCESS)
			return NULL;

		return object;
	}


	static char *get_python_err(void)
	{
		PyObject *pyErr;
		PyObject *ptype;
		PyObject *pvalue;
		PyObject *ptraceback;
		PyObject *errorStr;
		const char *perror;
		char *rerror;
		s_erc error;


		S_CLR_ERR(&error);
		pyErr = PyErr_Occurred();
		if (pyErr == NULL)
		{
			rerror = s_strdup("No error", &error);
			return rerror;
		}

		PyErr_Fetch(&ptype, &pvalue, &ptraceback);
		if (ptype == NULL)
		{
			rerror = s_strdup("Unknown error", &error);
			return rerror;
		}

		PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);
		if (ptype == NULL)
		{
			rerror = s_strdup("Unknown error", &error);
			return rerror;
		}

		errorStr = PyObject_Str(ptype);
		if (errorStr == NULL)
		{
			rerror = s_strdup("Unknown error", &error);
			return rerror;
		}

		perror = PyString_AsString(errorStr);
		if (perror == NULL)
		{
			rerror = s_strdup("Unknown error", &error);
			return rerror;
		}

		rerror = s_strdup(perror, &error);
		Py_XDECREF(errorStr);
		return rerror;
	}


	/*
	 * Function that executes the Python callback
	 * by calling PyObject_CallObject
	 */
	static SObject *execute_python_callback(const SItem *item, void *sfunction, s_erc *error)
	{
		PyObject *pyItem;
		PyObject *func;
		PyObject *arglist;
		PyObject *result;
		SObject *retval = NULL;

		/* get Python function */
		func = (PyObject*)sfunction;

		/* Create a PyObject from the SObject, flag = 0 (Python does
		 * not own the SObject/PyObject
		 */
		pyItem = SWIG_NewPointerObj((void*)item, SWIGTYPE_p_SItem, 0);

		/* create argument list */
		arglist = Py_BuildValue("(O)", pyItem);
		if (arglist == NULL) /* callback function failed */
		{
			S_CTX_ERR(error, S_FAILURE,
					  "execute_python_callback",
					  "Call to \"Py_BuildValue\" failed");
			return NULL;
		}

		/* call Python and execute the function */
		result = PyObject_CallObject(func, arglist);

		/* we don't need the argument list anymore */
		Py_DECREF(arglist);

		if (result == NULL) /* callback function failed */
		{
			char *py_error;


			py_error = get_python_err();
			if (py_error != NULL)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "execute_python_callback",
						  "Python callback execution failed: \"%s\"", py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "execute_python_callback",
						  "Python callback execution failed: \"Unknown error!\"");
			}

			return NULL;
		}

		/* convert result to Speect object */
		retval = pyobject_2_sobject(result, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "execute_python_callback",
					  "Call to \"pyobject_2_sobject\" failed"))
			retval = NULL;

		/* we don't need the result object anymore */
		Py_CLEAR(result);

		return retval;
	}


	static void sfeatproc_cb_decrement_func(SFeatProcessorCB *featProcPy, s_erc *error)
	{
		PyObject *callback_func;
		SFeatProcessorCB *self = S_FEATPROCESSOR_CB(featProcPy);


		callback_func = (PyObject*)self->sfunction;
		self->sfunction = NULL;

		/* decrement the reference count, we don't need
		 * this function anymore
		 */
		Py_XDECREF(callback_func);
	}


	SFeatProcessor *sfeatproc_cb_new(PyObject *callback_func, s_erc *error)
	{
		SFeatProcessorCB *featProcPy;


		/* test whether the callback function is actually callable */
		if (!PyCallable_Check(callback_func))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "sfeatproc_cb_new",
					  "Given callback function failed \"PyCallable_Check\"");
			return NULL;
		}

		/* create new Python feature processor */
		featProcPy = (SFeatProcessorCB*)S_NEW("SFeatProcessorCB", error);
		if (*error != S_SUCCESS)
			return NULL;

		/* set the object's callback and execute functions */
		if (!S_FEATPROCESSOR_CB_METH_VALID(featProcPy, set_callback))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "sfeatproc_cb_new",
					  "SFeatProcessorCB method \"set_callback\" not implemented");
			S_DELETE(featProcPy, "sfeatproc_cb_new", error);
			return NULL;
		}

		S_FEATPROCESSOR_CB_CALL(featProcPy, set_callback)(featProcPy,
														  &execute_python_callback,
														  &sfeatproc_cb_decrement_func,
														  (void*)callback_func,
														  error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(featProcPy, "sfeatproc_cb_new", error);
			return NULL;
		}

		/* increment the reference to the callback function */
		Py_XINCREF(callback_func);

		return S_FEATPROCESSOR(featProcPy);
	}
/*
 * Do not delete this delimiter, required for SWIG
 */
%}
