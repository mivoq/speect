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
	uint32 py_slist_len(const SObject *object, s_erc *error)
	{
		uint32 len = 0;
		const SList *self = S_LIST(object);


		len = (uint32)SListSize(self, error);
		if (*error != S_SUCCESS)
			return 0;

		return len;
	}


	const SObject *py_slist_nth(const SObject *object, uint32 n, s_erc *error)
	{
		const SObject *nth;
		SList *self = S_LIST(object);


		nth = SListNth(self, n, error);
		if (*error != S_SUCCESS)
			return NULL;

		return nth;
	}


	SObject *py_slist_iterator_new(const SObject *listObject, s_erc *error)
	{
		SIterator *itr;
		SList *list = S_LIST(listObject);


		itr = SListIterator(list, error);
		if (*error != S_SUCCESS)
			return NULL;

		return S_OBJECT(itr);
	}


	const SObject *py_slist_iterator_get_item(const SObject *itr, s_erc *error)
	{
		const SObject *object;
		SIterator *iterator = (SIterator*)itr;


		object = SListIteratorValue(iterator, error);
		if (*error != S_SUCCESS)
			return NULL;

		return object;
	}


	s_bool py_slist_constains(SObject *object, const SObject *item, s_erc *error)
	{
		SList *self = S_LIST(object);
		s_bool is_present;


		is_present = SListValPresent(self, item, error);
		if (*error != S_SUCCESS)
			return FALSE;

		return is_present;
	}


	void py_slist_append(SObject *object, const SObject *item, s_erc *error)
	{
		SList *self = S_LIST(object);


		SListAppend(self, item, error);
	}


	void py_slist_prepend(SObject *object, const SObject *item, s_erc *error)
	{
		SList *self = S_LIST(object);


		SListPrepend(self, item, error);
	}


	void py_slist_merge(SObject *object, const SObject *list, s_erc *error)
	{
		SList *self = S_LIST(object);
		SList *with = S_LIST(list);


		SListMerge(self, with, error);
	}


	SObject *py_slist_pop_last_item(SObject *object, s_erc *error)
	{
		SList *self = S_LIST(object);
		SObject *pop;


		pop = SListPop(self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return pop;
	}


	void py_slist_reverse(SObject *object, s_erc *error)
	{
		SList *self = S_LIST(object);


		self = SListReverse(self, error);
	}


	void py_slist_set_item(SObject *object, const SObject *item, uint32 n, s_erc *error)
	{
		SList *self = S_LIST(object);
		SIterator *itr;
		uint32 i;
		s_bool found_point;
		SObject *toDelete;


		itr = SListIterator(self, error);
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
					  "py_slist_set_item",
					  "Failed to find item to replace");
			if (itr)
				S_DELETE(itr, "py_slist_set_item", error);
			return;
		}

		SListInsertBefore(self, itr, item, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(itr, "py_slist_set_item", error);
			return;
		}

		toDelete = SListIteratorUnlink(itr, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(itr, "py_slist_set_item", error);
			return;
		}

		S_DELETE(itr, "py_slist_set_item", error);
		S_DELETE(toDelete, "py_slist_set_item", error);
	}


	void py_slist_del_item(SObject *object, uint32 n, s_erc *error)
	{
		SList *self = S_LIST(object);
		SIterator *itr;
		uint32 i;
		s_bool found_point;
		SObject *toDelete;


		itr = SListIterator(self, error);
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
					  "py_slist_del_item",
					  "Failed to find item to delete");
			if (itr)
				S_DELETE(itr, "py_slist_del_item", error);
			return;
		}

		toDelete = SListIteratorUnlink(itr, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(itr, "py_slist_del_item", error);
			return;
		}

		S_DELETE(itr, "py_slist_del_item", error);
		S_DELETE(toDelete, "py_slist_del_item", error);
	}


	SObject *py_slist_new(s_erc *error)
	{
		SList *newList;


		newList = S_LIST(S_NEW("SListList", error));
		if (*error != S_SUCCESS)
			return NULL;

		SListListInit(&newList, error);
		if (*error != S_SUCCESS)
			return NULL;

		return S_OBJECT(newList);
	}


	/* insert the given item at the given index */
	void py_slist_insert_item(SObject *object, const SObject *item, uint32 n, s_erc *error)
	{
		SList *self = S_LIST(object);
		SIterator *itr;
		uint32 i;
		s_bool found_point;


		itr = SListIterator(self, error);
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
					  "py_slist_insert_item",
					  "Failed to find index to list");
			if (itr)
				S_DELETE(itr, "py_slist_insert_item", error);
			return;
		}

		SListInsertBefore(self, itr, item, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(itr, "py_slist_insert_item", error);
			return;
		}

		S_DELETE(itr, "py_slist_insert_item", error);
	}


	/* pop the item at the given index */
	SObject *py_slist_pop_item(SObject *object, uint32 n, s_erc *error)
	{
		SList *self = S_LIST(object);
		SIterator *itr;
		uint32 i;
		s_bool found_point;
		SObject *pop;


		itr = SListIterator(self, error);
		if (*error != S_SUCCESS)
			return NULL;

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
					  "py_slist_pop_item",
					  "Failed to find item to pop");
			if (itr)
				S_DELETE(itr, "py_slist_pop_item", error);
			return NULL;
		}

		pop = SListIteratorUnlink(itr, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(itr, "py_slist_pop_item", error);
			return NULL;
		}

		S_DELETE(itr, "py_slist_pop_item", error);
		return pop;
	}
/*
 * Do not delete this delimiter, required for SWIG
 */
%}


