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
/* DATE    : March 2010                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SList Python wrapper.                                */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%extend SList
{
	PyObject *__getitem__(int key, s_erc *error)
	{
		const SObject *listObject;
		size_t num_objects;
		PyObject *pobject;


		S_CLR_ERR(error);
		num_objects = SListSize($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		if ((key >= (int)num_objects) || (key < 0))
		{
			PyErr_SetString(PyExc_KeyError, "Given key index out of bounds");
			return NULL;
		}

		listObject = SListNth($self, (uint32)key, error);
		if (*error != S_SUCCESS)
			return NULL;

		pobject = s_sobject_2_pyobject(listObject, FALSE, error);
		if (*error != S_SUCCESS)
			return NULL;

		return pobject;
	}


	int __len__(s_erc *error)
	{
		size_t num_objects;


		S_CLR_ERR(error);
		num_objects = SListSize($self, error);
		if (*error != S_SUCCESS)
			return 0;

		return (int)num_objects;
	}


	void __setitem__(int key, PyObject *val, s_erc *error)
	{
		SIterator *itr;
		uint32 i;
		s_bool found_point;
		SObject *toDelete;
		SObject *newObject;
		uint32 n = (uint32)key;
		size_t num_objects;


		S_CLR_ERR(error);
		num_objects = SListSize($self, error);
		if (*error != S_SUCCESS)
			return;

		if ((key >= (int)num_objects) || (key < 0))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "SList::__setitem__()",
					  "Given key index out of bounds");
			return;
		}

		itr = S_ITERATOR_GET(self, error);
		if (*error != S_SUCCESS)
			return;

		i = 0;
		found_point = FALSE;
		while (itr)
		{
			if (i == n)
			{
				found_point = TRUE;
				break;
			}

			i++;
			itr = SIteratorNext(itr);
		}

		if ((!found_point) || (!itr))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "SList::__setitem__()",
					  "Failed to find item to replace");
			if (itr)
				S_DELETE(itr, "SList::__setitem__()", error);
			return;
		}

		newObject = s_pyobject_2_sobject(val, error);
		if (*error != S_SUCCESS)
			return;

		SListInsertBefore(self, itr, newObject, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(newObject, "SList::__setitem__()", error);
			S_DELETE(itr, "SList::__setitem__()", error);
			return;
		}

		toDelete = SIteratorUnlink(itr, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(itr, "SList::__setitem__()", error);
			return;
		}

		S_DELETE(itr, "SList::__setitem__()", error);
		S_DELETE(toDelete, "SList::__setitem__()", error);
	}


	void __delitem__(int key, s_erc *error)
	{
		SIterator *itr;
		uint32 i;
		s_bool found_point;
		SObject *toDelete;
		uint32 n = (uint32)key;
		size_t num_objects;


		S_CLR_ERR(error);
		num_objects = SListSize($self, error);
		if (*error != S_SUCCESS)
			return;

		if ((key >= (int)num_objects) || (key < 0))
		{
			PyErr_SetString(PyExc_KeyError, "Given key index out of bounds");
			return;
		}

		itr = S_ITERATOR_GET($self, error);
		if (*error != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get list iterator");
			return;
		}

		i = 0;
		found_point = FALSE;
		while (itr)
		{
			if (i == n)
			{
				found_point = TRUE;
				break;
			}

			i++;
			itr = SIteratorNext(itr);
		}

		if ((!found_point) || (!itr))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "SList::__delitem__()",
					  "Failed to find item to delete");
			if (itr)
				S_DELETE(itr, "SList::__delitem__()", error);
			return;
		}

		toDelete = SIteratorUnlink(itr, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(itr, "SList::__delitem__()", error);
			return;
		}

		S_DELETE(itr, "SList::__delitem__()", error);
		S_DELETE(toDelete, "SList::__delitem__()", error);
	}


	PListIterator *__iter__()
	{
		PListIterator *pitr;
		SIterator *itr;
		s_erc error;


		S_CLR_ERR(&error);
		itr = S_ITERATOR_GET($self, &error);
		if (error != S_SUCCESS)
			return NULL;

		pitr = make_PListIterator(itr, &error);
		if (error != S_SUCCESS)
			return NULL;

		return pitr;
	}

%pythoncode
%{
def __str__(self):
    """
    Return a string representation of the values that are in the list.

    :return: A string representation of the values that are in the list.
    :rtype: string
    """
    string = "["
    length = len(self)
    if length == 0:
        string += "]"
        return string
    else:
        string += " "
        for i in self:
            string += repr(i)
            length -= 1
            if length != 0:
                string += ", "
        string += " ]";
        return string;
%}

};

