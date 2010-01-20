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
/* DATE    : November 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for primitives SInt, SFloat, SString and                 */
/* SVoid Python wrappers.                                                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	SObject *py_sint_set(sint32 val, s_erc *error)
	{
		SObject *newInt;


		newInt = SObjectSetInt(val, error);
		if (*error != S_SUCCESS)
			return NULL;

		return newInt;
	}


	sint32 py_sint_get(SObject *self, s_erc *error)
	{
		sint32 val;


		val = SObjectGetInt(self, error);
		if (*error != S_SUCCESS)
			return 0;

		return val;
	}


	SObject *py_sfloat_set(float val, s_erc *error)
	{
		SObject *newFloat;


		newFloat = SObjectSetFloat(val, error);
		if (*error != S_SUCCESS)
			return NULL;

		return newFloat;
	}


	float py_sfloat_get(SObject *self, s_erc *error)
	{
		float val;


		val = SObjectGetFloat(self, error);
		if (*error != S_SUCCESS)
			return 0;

		return val;
	}


	SObject *py_sstring_set(const char *val, s_erc *error)
	{
		SObject *newString;


		newString = SObjectSetString(val, error);
		if (*error != S_SUCCESS)
			return NULL;

		return newString;
	}


	const char *py_sstring_get(SObject *self, s_erc *error)
	{
		const char *val;


		val = SObjectGetString(self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return val;
	}


	SObject *py_svoid_set(PyObject *val, s_erc *error)
	{
		SObject *newVoid;


		newVoid = SObjectSetVoid((void*)val, "PythonObject", NULL, error);
		if (*error != S_SUCCESS)
			return NULL;

		return newVoid;
	}


	PyObject *py_svoid_get(SObject *self, s_erc *error)
	{
		PyObject *val;


		val = (PyObject*)SObjectGetVoid(self, "PythonObject", error);
		if (*error != S_SUCCESS)
			return NULL;

		return val;
	}

/*
 * Do not delete this delimiter, required for SWIG
 */
%}


