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
/* AUTHOR  : Richard Carlson, Aby Louw                                              */
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SEbmlWrite Python wrapper.                           */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Extend the SEbmlRead class                                                       */
/*                                                                                  */
/************************************************************************************/

%extend SEbmlWrite
{
	void write_object(PyObject *val, uint32 id, s_erc *error)
	{
		SObject *object;
		s_erc local_err = S_SUCCESS;


		S_CLR_ERR(error);
		object = s_pyobject_2_sobject(val, error);
		Py_DECREF(val);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_object",
					  "Call to \"s_pyobject_2_sobject\" failed"))
			return;

		S_EBMLWRITE_CALL($self, write_object)($self, id, object, &local_err);
		S_CHK_ERR(&local_err, S_CONTERR,
				  "write_object",
				  "Call to SEbmlWrite method \"write_object\" failed");

		/* if it's a primitive type we must delete it, as the Speect
		 * Python native library does not wrap the primitives, it
		 * creates new Speect objects.
		 */

		if ((PyObject_IsInstance(val, (PyObject*)&PyInt_Type))
			|| (PyObject_IsInstance(val, (PyObject*)&PyFloat_Type))
			|| (PyObject_IsInstance(val, (PyObject*)&PyString_Type))
			|| (PyObject_IsInstance(val, (PyObject*)&PyUnicode_Type)))
		{
			S_DELETE(object, "write_object", error);
		}

		if ((error != NULL)
			&& (local_err != S_SUCCESS)
			&& (*error == S_SUCCESS))
		{
			*error = local_err;
		}
	}
};
