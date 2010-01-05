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
/* C convenience functions for SUtttProcessorCB Python wrapper.                     */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	/*
	 * Function that executes the Python callback
	 * by calling PyObject_CallObject
	 */
	static void execute_python_callback(SObject *utt, void *sfunction, s_erc *error)
	{
		PyObject *pySObject;
		PyObject *func;
		PyObject *arglist;
		PyObject *result;


		/* get Python function */
		func = (PyObject*)sfunction;

		/* Create a PyObject from the SObject, flag = 0 (Python does
		 * not own the SObject/PyObject
		 */
		pySObject = SWIG_NewPointerObj((void*)utt, SWIGTYPE_p_SObject, 0);

		/* create argument list */
		arglist = Py_BuildValue("(O)",pySObject);

		/* call Python and execute the function */
		result = PyObject_CallObject(func, arglist);

		/* we don't need the argument list anymore */
		Py_CLEAR(arglist);

		/* this should be None */
		Py_CLEAR(result);
	}


	SObject *py_suttproc_cb_new(PyObject *callback_func)
	{
		s_erc rv = S_SUCCESS;
		SUttProcessorCB *uttProcPy;


		/* create new Python utterance processor */
		uttProcPy = (SUttProcessorCB*)S_NEW("SUttProcessorCB", &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError,
							"Failed to create new SUttProcessorCB object");
			return NULL;
		}

		/* set the object's callback and execute functions */
		if (!S_UTTPROCESSOR_CB_METH_VALID(uttProcPy, set_callback))
		{
			PyErr_SetString(PyExc_RuntimeError,
							"SUttProcessorCB method \"set_callback\" not implemented");
			S_DELETE(uttProcPy, "py_suttproc_cb_new", &rv);
			return NULL;
		}

		S_UTTPROCESSOR_CB_CALL(uttProcPy, set_callback)(uttProcPy,
														&execute_python_callback,
														(void*)callback_func,
														&rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError,
							"Failed to set SUttProcessorCB callback function");
			S_DELETE(uttProcPy, "py_suttproc_cb_new", &rv);
			return NULL;
		}

		/* increment the reference to the callback function */
		Py_XINCREF(callback_func);

		return S_OBJECT(uttProcPy);
	}


	void py_suttproc_cb_decrement_func(SObject *uttProcPy)
	{
		PyObject *callback_func;
		SUttProcessorCB *self = S_UTTPROCESSOR_CB(uttProcPy);


		callback_func = (PyObject*)self->sfunction;
		self->sfunction = NULL;

		/* decrement the reference count, we don't need
		 * this function anymore
		 */
		Py_CLEAR(callback_func);
	}
/*
 * Do not delete this delimiter, required for SWIG
 */
%}
