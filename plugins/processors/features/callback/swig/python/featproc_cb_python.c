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
/* C convenience functions for SFeatProcessorCB Python wrapper.                     */
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
	static SObject *execute_python_callback(const SItem *item, void *sfunction, s_erc *error)
	{
		PyObject *pyItem;
		PyObject *func;
		PyObject *arglist;
		PyObject *result;
		SObject *retval = NULL;


		S_CLR_ERR(error);

		if (item == NULL)
		{
			S_CTX_ERR(error, S_ARGERROR,
					  "execute_python_callback",
					  "Argument \"item\" is NULL");
			return NULL;
		}

		if (sfunction == NULL)
		{
			S_CTX_ERR(error, S_ARGERROR,
					  "execute_python_callback",
					  "Argument \"sfunction\" is NULL");
			return NULL;
		}

		/* get Python function */
		func = (PyObject*)sfunction;

		/* Create Python item from the SItem, FALSE as Python does
		 * not own the item
		 */
		pyItem = s_sobject_2_pyobject(S_OBJECT(item), FALSE, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "execute_python_callback",
					  "Call to \"s_sobject_2_pyobject\" failed"))
			return NULL;

		/* create argument list */
		arglist = Py_BuildValue("(O)", pyItem);
		if (arglist == NULL) /* callback function failed */
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

			return NULL;
		}

		/* cleanup */
		Py_DECREF(pyItem);

		/* convert result to Speect object */
		retval = s_pyobject_2_sobject(result, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "execute_python_callback",
					  "Call to \"fpc_pyobject_2_sobject\" failed"))
			retval = NULL;

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
		Py_DECREF(callback_func);
	}


	SFeatProcessor *_s_featproc_cb_new(PyObject *callback_func, s_erc *error)
	{
		SFeatProcessorCB *featProcPy;


		/* test whether the callback function is actually callable */
		if (!PyCallable_Check(callback_func))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "_s_featproc_cb_new",
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
					  "_s_featproc_cb_new",
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
			S_DELETE(featProcPy, "_s_featproc_cb_new", error);
			return NULL;
		}

		/* increment the reference to the callback function */
		Py_XINCREF(callback_func);

		return S_FEATPROCESSOR(featProcPy);
	}
%}


/************************************************************************************/
/*                                                                                  */
/* Extend the SFeatProcessor class                                                  */
/*                                                                                  */
/************************************************************************************/

%pythoncode
%{
import speect

def callback(callback_function):
    """
    callback(callback_function)

    Create a new feature processor that has a Python function as a callback. When
    this newly created feature processor's ``run`` method is called, the Python
    function will be called with the arguments as supplied to the feature processor.
    The Python callback must take one argument, an item (:class:`speect.SItem`),
    and return a single object, which will be considered the extracted feature. The
    input item argument must be considered as constant (in the C sense) and not
    modified in any way.

    :param callback_function: A Python function that will be used as a callback
                              function when this feature processor's ``run`` method
                              is called.
    :type callback_function: A callable function
    :return: Extracted feature.
    """

    return _s_featproc_cb_new(callback_function)


# add the functions to the Speect SFeatProcessor class
setattr(speect.SFeatProcessor, "callback", staticmethod(callback))
%}






