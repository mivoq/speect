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
/* C convenience functions for SObject Python wrapper.                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%apply float *OUTPUT {float *output}



/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	/*
	 * This is just a dummy function which will cause SWIG to wrap
	 * it and do the type checking for us. If the type is not SObject
	 * then SWIG will raise a TypeError before it actually gets here.
	 */
	s_bool py_is_sobject(SObject *self)
	{
		return TRUE;
	}


	void py_sobject_delete(SObject *self, s_erc *error)
	{
		S_DELETE(self, "py_sobject_delete", error);
	}


	void py_sobject_force_delete(SObject *self, s_erc *error)
	{
		S_FORCE_DELETE(self, "py_sobject_force_delete", error);
	}


	PyObject *py_sobject_print(const SObject *self, s_erc *error)
	{
		char *buf;
		size_t buff_size;
		PyObject *pyString;


		buf = SObjectPrint(self, error);
		if (*error != S_SUCCESS)
			return NULL;

		buff_size = s_strsize(buf, error);
		if (*error != S_SUCCESS)
		{
			S_FREE(buf);
			return NULL;
		}

		pyString = PyString_FromStringAndSize(buf, buff_size);
		S_FREE(buf);

		return pyString;
	}


	s_bool py_sobject_is_type(SObject *self, const char *type, s_erc *error)
	{
		s_bool is_instance;


		is_instance = SObjectIsType(self, type, error);
		if (*error != S_SUCCESS)
			return FALSE;

		return is_instance;
	}


	const char *py_sobject_get_type(SObject *self, s_erc *error)
	{
		const char *type;


		type = SObjectType(self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return type;
	}


	s_bool py_sobject_compare(SObject *self, SObject *other, s_erc *error)
	{
		s_bool compare;


		compare = SObjectCompare(self, other, error);
		if (*error != S_SUCCESS)
			return FALSE;

		return compare;
	}


	SObject *py_sobject_copy(SObject *self, s_erc *error)
	{
		SObject *copy;


		copy = SObjectCopy(self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return copy;
	}


	void py_sobject_dec_ref(SObject *self)
	{
		SObjectDecRef(self);
	}


	void py_sobject_inc_ref(SObject *self)
	{
		SObjectIncRef(self);
	}
/*
 * Do not delete this delimiter, required for SWIG
 */
%}

