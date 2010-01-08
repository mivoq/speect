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
/* C convenience functions for SList Python wrapper.                                */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	uint32 py_slist_len(const SObject *object)
	{
		s_erc rv = S_SUCCESS;
		uint32 len = 0;
		const SList *self = S_LIST(object);


		len = (uint32)SListSize(self, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to determine "
							"list length");
			return 0;
		}

		return len;
	}


	const SObject *py_slist_nth(const SObject *object, uint32 n)
	{
		s_erc rv = S_SUCCESS;
		const SObject *nth;
		SList *self = S_LIST(object);


		nth = SListNth(self, n, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get item "
							"from list");
			return NULL;
		}

		return nth;
	}


	void py_slist_set_item(SObject *object,
						   const SObject *item, uint32 n)
	{
		s_erc rv = S_SUCCESS;
		SIterator *itr;
		SList *self = S_LIST(object);
		uint32 i;
		s_bool found_point;
		SObject *toDelete;


		itr = SListIterator(self, &rv);
		if (rv != S_SUCCESS)
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
			PyErr_SetString(PyExc_RuntimeError, "Failed to find item to replace");
			if (itr)
				S_DELETE(itr, "py_slist_set_item", &rv);
			return;
		}

		SListInsertBefore(self, itr, item, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to insert item");
			S_DELETE(itr, "py_slist_set_item", &rv);
			return;
		}

		toDelete = SListIteratorUnlink(itr, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to unlink item");
			S_DELETE(itr, "py_slist_set_item", &rv);
			return;
		}

		S_DELETE(itr, "py_slist_set_item", &rv);
		S_DELETE(toDelete, "py_slist_set_item", &rv);
	}


	void py_slist_del_item(SObject *object, uint32 n)
	{
		s_erc rv = S_SUCCESS;
		SIterator *itr;
		SList *self = S_LIST(object);
		uint32 i;
		s_bool found_point;
		SObject *toDelete;


		itr = SListIterator(self, &rv);
		if (rv != S_SUCCESS)
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
			PyErr_SetString(PyExc_RuntimeError, "Failed to find item to delete");
			if (itr)
				S_DELETE(itr, "py_slist_set_item", &rv);
			return;
		}

		toDelete = SListIteratorUnlink(itr, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to unlink item");
			S_DELETE(itr, "py_slist_set_item", &rv);
			return;
		}

		S_DELETE(itr, "py_slist_set_item", &rv);
		S_DELETE(toDelete, "py_slist_set_item", &rv);
	}


	SObject *py_slist_new(void)
	{
		s_erc rv = S_SUCCESS;
		SList *newList;


		newList = S_LIST(S_NEW("SListList", &rv));
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to create new list");
			return NULL;
		}

		SListListInit(&newList, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to initialize new list");
			return NULL;
		}

		return S_OBJECT(newList);
	}


	SObject *py_slist_iterator_new(const SObject *listObject)
	{
		s_erc rv = S_SUCCESS;
		SIterator *itr;
		SList *list = S_LIST(listObject);


		itr = SListIterator(list, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get list iterator");
			return NULL;
		}

		return S_OBJECT(itr);
	}


	const SObject *py_slist_iterator_get_item(const SObject *itr)
	{
		s_erc rv = S_SUCCESS;
		const SObject *object;
		SIterator *iterator = (SIterator*)itr;


		object = SListIteratorValue(iterator, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Iterator error");
			return NULL;
		}

		return object;
	}


	s_bool py_slist_constains(SObject *object, const SObject *item)
	{
		s_erc rv = S_SUCCESS;
		SList *self = S_LIST(object);
		s_bool is_present;


		is_present = SListValPresent(self, item, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to determine if item is present");
			return FALSE;
		}

		return is_present;
	}


	void py_slist_append(SObject *object, const SObject *item)
	{
		s_erc rv = S_SUCCESS;
		SList *self = S_LIST(object);


		SListAppend(self, item, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to append item");
	}


	void py_slist_prepend(SObject *object, const SObject *item)
	{
		s_erc rv = S_SUCCESS;
		SList *self = S_LIST(object);


		SListPrepend(self, item, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to prepend item");
	}


	void py_slist_merge(SObject *object, const SObject *list)
	{
		s_erc rv = S_SUCCESS;
		SList *self = S_LIST(object);
		SList *with = S_LIST(list);


		SListMerge(self, with, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to merge lists");
	}


	void py_slist_insert_item(SObject *object,
							  const SObject *item, uint32 n)
	{
		s_erc rv = S_SUCCESS;
		SIterator *itr;
		SList *self = S_LIST(object);
		uint32 i;
		s_bool found_point;


		itr = SListIterator(self, &rv);
		if (rv != S_SUCCESS)
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
			PyErr_SetString(PyExc_RuntimeError, "Failed to find item to replace");
			if (itr)
				S_DELETE(itr, "py_slist_set_item", &rv);
			return;
		}

		SListInsertBefore(self, itr, item, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to insert item");
			S_DELETE(itr, "py_slist_set_item", &rv);
			return;
		}

		S_DELETE(itr, "py_slist_set_item", &rv);
	}


	SObject *py_slist_pop_item(SObject *object, uint32 n)
	{
		s_erc rv = S_SUCCESS;
		SIterator *itr;
		SList *self = S_LIST(object);
		uint32 i;
		s_bool found_point;
		SObject *pop;


		itr = SListIterator(self, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get list iterator");
			return NULL;
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
			PyErr_SetString(PyExc_RuntimeError, "Failed to find item to replace");
			if (itr)
				S_DELETE(itr, "py_slist_set_item", &rv);
			return NULL;
		}

		pop = SListIteratorUnlink(itr, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Iterator error");
			S_DELETE(itr, "py_slist_set_item", &rv);
			return NULL;
		}

		S_DELETE(itr, "py_slist_set_item", &rv);

		return pop;
	}


	SObject *py_slist_pop_last_item(SObject *object)
	{
		s_erc rv = S_SUCCESS;
		SList *self = S_LIST(object);
		SObject *pop;


		pop = SListPop(self, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to pop last item");
			return NULL;
		}

		return pop;
	}


	void py_slist_reverse(SObject *object)
	{
		s_erc rv = S_SUCCESS;
		SList *self = S_LIST(object);


		self = SListReverse(self, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to reverse list");
	}

/*
 * Do not delete this delimiter, required for SWIG
 */
%}



