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


/************************************************************************************/
/*                                                                                  */
/* Inline helper functions                                                          */
/*                                                                                  */
/************************************************************************************/

%inline
%{
	/*
	 * Function that executes the Python callback
	 * by calling PyObject_CallObject
	 */
	static void execute_python_callback(SUtterance *utt, void *sfunction, s_erc *error)
	{
		PyObject *pyUtt;
		PyObject *func;
		PyObject *arglist;
		PyObject *result;


		S_CLR_ERR(error);

		if (utt == NULL)
		{
			S_CTX_ERR(error, S_ARGERROR,
					  "execute_python_callback",
					  "Argument \"utt\" is NULL");
			return;
		}

		if (sfunction == NULL)
		{
			S_CTX_ERR(error, S_ARGERROR,
					  "execute_python_callback",
					  "Argument \"sfunction\" is NULL");
			return;
		}

		/* get Python function */
		func = (PyObject*)sfunction;

		/* Create Python utterance from the utt, FALSE as Python does
		 * not own the utterance
		 */
		pyUtt = s_sobject_2_pyobject(S_OBJECT(utt), FALSE, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "execute_python_callback",
					  "Call to \"s_sobject_2_pyobject\" failed"))
			return;

		/* create argument list */
		arglist = Py_BuildValue("(O)", pyUtt);
		if (arglist == NULL)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "execute_python_callback",
						  "Call to \"Py_BuildValue\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "execute_python_callback",
						  "Call to \"Py_BuildValue\" failed");
			}
		}

		/* call Python and execute the function */
		result = PyObject_CallObject(func, arglist);

		/* we don't need the argument list anymore */
		Py_DECREF(arglist);

		if (result == NULL) /* callback function failed */
		{
			char *py_error;


			py_error = s_get_python_error_str();
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

			/* cleanup */
			Py_DECREF(pyUtt);

			return;
		}

		/* cleanup */
		Py_DECREF(pyUtt);

		/* this should be None */
		Py_DECREF(result);
	}


	static void suttproc_cb_decrement_func(SUttProcessorCB *uttProcPy, s_erc *error)
	{
		PyObject *callback_func;
		SUttProcessorCB *self = S_UTTPROCESSOR_CB(uttProcPy);


		callback_func = (PyObject*)self->sfunction;
		self->sfunction = NULL;

		/* decrement the reference count, we don't need
		 * this function anymore
		 */
		Py_DECREF(callback_func);
	}


	SUttProcessor *_s_uttproc_cb_new(PyObject *callback_func, s_erc *error)
	{
		SUttProcessorCB *uttProcPy;


		/* test whether the callback function is actually callable */
		if (!PyCallable_Check(callback_func))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "_s_uttproc_cb_new",
					  "Given callback function failed \"PyCallable_Check\"");
			return NULL;
		}

		/* create new Python utterance processor */
		uttProcPy = (SUttProcessorCB*)S_NEW("SUttProcessorCB", error);
		if (*error != S_SUCCESS)
			return NULL;

		/* set the object's callback and execute functions */
		if (!S_UTTPROCESSOR_CB_METH_VALID(uttProcPy, set_callback))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "_s_uttproc_cb_new",
					  "SUttProcessorCB method \"set_callback\" not implemented");
			S_DELETE(uttProcPy, "py_suttproc_cb_new", error);
			return NULL;
		}

		S_UTTPROCESSOR_CB_CALL(uttProcPy, set_callback)(uttProcPy,
														&execute_python_callback,
														&suttproc_cb_decrement_func,
														(void*)callback_func,
														error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(uttProcPy, "suttproc_cb_new", error);
			return NULL;
		}

		/* increment the reference to the callback function */
		Py_XINCREF(callback_func);

		return S_UTTPROCESSOR(uttProcPy);
	}
%}


/************************************************************************************/
/*                                                                                  */
/* Extend the SUttProcessor class                                                   */
/*                                                                                  */
/************************************************************************************/

%pythoncode
%{
import speect

def callback(callback_function):
    """
    callback(callback_function)

    Create a new utterance processor that has a Python function as a callback. When
    this newly created feature processor's ``run`` method is called, the Python
    function will be called with the arguments as supplied to the utterance processor.
    The Python callback must take one argument, an utterance (:class:`speect.SUtterance`),
    and must not return anything. The utterance can be modified in place.

    :param callback_function: A Python function that will be used as a callback
                              function when this utterance processor's ``run`` method
                              is called.
    :type callback_function: A callable function
    """


    return _s_uttproc_cb_new(callback_function)


# add the functions to the Speect SUttProcessor class
setattr(speect.SUttProcessor, "callback", staticmethod(callback))
%}

