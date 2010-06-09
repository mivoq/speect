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
/* C convenience functions for SIterator Python wrapper.                            */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%extend PMapIterator
{
	PMapIterator *__iter__()
	{
		return $self;
	}

	PyObject *next()
	{
		const char *key;
		size_t slen;
		s_erc error;
		PyObject *pobject;


		S_CLR_ERR(&error);
		if ($self->itr == NULL)
		{
			PyErr_SetNone(PyExc_StopIteration);
			return NULL;
		}

		key = SMapIteratorKey($self->itr, &error);
		if (error != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Call to \"SMapIteratorKey\" failed");
			return NULL;
		}

		slen = s_strsize(key, &error);
		if (error != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Call to \"s_strsize\" failed");
			return NULL;
		}

		$self->itr = SIteratorNext($self->itr);
		pobject = PyString_FromStringAndSize(key, slen);
		return pobject;
	}
};


%extend PListIterator
{
	PListIterator *__iter__()
	{
		return $self;
	}

	PyObject *next()
	{
		const SObject *iObject;
		s_erc error;
		PyObject *pobject;


		S_CLR_ERR(&error);
		if ($self->itr == NULL)
		{
			PyErr_SetNone(PyExc_StopIteration);
			return NULL;
		}

		iObject = SListIteratorValue($self->itr, &error);
		if (error != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Call to \"SListIteratorValue\" failed");
			return NULL;
		}

		$self->itr = SIteratorNext($self->itr);
		pobject = sobject_2_pyobject(iObject, &error, FALSE);
		if (error != S_SUCCESS)
			return NULL;

		return pobject;
	}
};

