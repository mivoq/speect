/************************************************************************************/
/* Copyright (c) 2008-2011 The Department of Arts and Culture,                      */
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
/* DATE    : 12 October 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* List implementation of SList container.                                          */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "base/objsystem/objsystem.h"
#include "containers/list/list_list_iterator.h"
#include "containers/list/list_list.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Call the given function method of the given #SListList,
 * see full description #S_LISTLIST_CALL for usage.
 * @param SELF The given #SListList*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage: @code S_LIST_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_LISTLIST_CALL(SELF, FUNC)					\
	((SListListClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SListList
 * can be called.
 * @param SELF The given #SListList*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_LISTLIST_METH_VALID(SELF, FUNC)			\
	S_LISTLIST_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/*  Static variables                                                                */
/*                                                                                  */
/************************************************************************************/

static SListListClass ListListClass; /* ListList class declaration. */


/************************************************************************************/
/*                                                                                  */
/*  Static function prototypes                                                      */
/*                                                                                  */
/************************************************************************************/

/*
 * Delete function for SListList.
 * Used to delete a SObject object list item.
 */
static void s_list_list_del_function(void *le, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_list_list_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&ListListClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_list_list_class_add",
			  "Failed to add SListListClass");
}


/************************************************************************************/
/*                                                                                  */
/*  Static function implementation                                                  */
/*                                                                                  */
/************************************************************************************/

/*
 * Delete function for SListList.
 * Used to delete a SObject object list item.
 */
static void s_list_list_del_function(void *le, s_erc *error)
{
	SObject *tmp = le;


	S_CLR_ERR(error);
	S_DELETE(tmp, "s_list_list_del_function", error);
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitListList(void *obj, s_erc *error)
{
	SListList *self = obj;


	S_CLR_ERR(error);

	self->list = s_list_new(NULL, &s_list_list_del_function,
							error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "InitListList",
				  "Failed to create new 's_list' object"))
		self->list = NULL;
}


static void DisposeListList(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void DestroyListList(void *obj, s_erc *error)
{
	SListList *self = obj;


	S_CLR_ERR(error);
	if (self->list == NULL)
		return;

	s_list_delete(self->list, error);
	S_CHK_ERR(error, S_CONTERR,
			  "DestroyListList",
			  "Failed to delete list");
}


static s_bool ListListIsEmpty(const SList *self, s_erc *error)
{
	s_bool is_empty;
	const SListList *lList = (const SListList*)self;


	S_CLR_ERR(error);
	if (lList->list == NULL)
		return TRUE;

	is_empty = s_list_isempty(lList->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListListIsEmpty",
				  "Call to s_list_isempty failed"))
		return TRUE;

	return is_empty;
}


static size_t ListListSize(const SList *self, s_erc *error)
{
	size_t size;
	const SListList *lList = (const SListList*)self;


	S_CLR_ERR(error);
	if (lList->list == NULL)
		return 0;

	size = s_list_size(lList->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListListSize",
				  "Call to s_list_size failed"))
		return 0;

	return size;
}


static void ListListAppend(SList *self, const SObject *object, s_erc *error)
{
	SObject *tmp;
	SListList *lList = (SListList*)self;


	S_CLR_ERR(error);
	if (lList->list == NULL)
		return;

	/* cast away const */
 	tmp = (SObject*)object;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* append */
	s_list_append(lList->list, (void*)tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListListAppend",
				  "Call to \"s_list_append\" failed"))
		SObjectDecRef(tmp);
}


static void ListListPrepend(SList *self, const SObject *object, s_erc *error)
{
 	SObject *tmp;
	SListList *lList = (SListList*)self;


	S_CLR_ERR(error);
	if (lList->list == NULL)
		return;

	/* cast away const */
 	tmp = (SObject*)object;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* prepend */
 	s_list_prepend(lList->list, (void*)tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListListPrepend",
				  "Call to \"s_list_prepend\" failed"))
		SObjectDecRef(tmp);
}


static void ListListInsertBefore(SList *self, SIterator *itr,
								 const SObject *object, s_erc *error)
{
	SListList *lList = (SListList*)self;
	SListListIterator *listItr;
	s_list_element *current_element;
	SObject *tmp;


	S_CLR_ERR(error);
	if (lList->list == NULL)
		return;

	listItr = (SListListIterator*)itr;
	current_element = (s_list_element*)listItr->c_itr;

	/* cast away const */
 	tmp = (SObject*)object;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* insert */
	s_list_insert_before(current_element, (void*)tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListListInsertBefore",
				  "Call to s_list_insert_before failed"))
	{
		/* remove reference */
		SObjectDecRef(tmp);
		return;
	}
}


static void ListListInsertAfter(SList *self, SIterator *itr,
								const SObject *object, s_erc *error)
{
	SListList *lList = (SListList*)self;
	SListListIterator *listItr;
	s_list_element *current_element;
	s_list_element *inserted_element;
	SObject *tmp;


	S_CLR_ERR(error);
	if (lList->list == NULL)
		return;

	listItr = (SListListIterator*)itr;
	current_element = (s_list_element*)listItr->c_itr;

	/* cast away const */
 	tmp = (SObject*)object;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* insert */
	inserted_element = (s_list_element*)s_list_insert_after(current_element, (void*)tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListListInsertAfter",
				  "Call to s_list_insert_after failed"))
	{
		/* remove reference */
		SObjectDecRef(tmp);
		return;
	}

	listItr->n_itr = inserted_element;
}


static void ListListMerge(SList *self, const SList *with, s_erc *error)
{
	SListList *lList = (SListList*)self;
	SIterator *itr;
	SObject *tmp;


	S_CLR_ERR(error);

	/* call method directly to avoid mutex locking issues */
	itr = S_CONTAINER_CALL(S_CONTAINER(with), get_iterator)(S_CONTAINER(with), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListListMerge",
				  "Failed to get an iterator to \'with\' list container"))
		return;

	while (itr)
	{
		/*
		 * get object of "with", cast away const as we
		 * want to increase the ref count
		 */
		tmp = (SObject*)SIteratorObject(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ListListMerge",
					  "Call to \"SIteratorObject\" for \'with\' list container failed"))
		{
			S_DELETE(itr, "ListListMerge", error);
			return;
		}

		/* increment reference count */
		SObjectIncRef(tmp);

		/* append */
		s_list_append(lList->list, (void*)tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ListListMerge",
					  "Call to \"s_list_append\" failed"))
		{
			SObjectDecRef(tmp);
			S_DELETE(itr, "ListListMerge", error);
			return;
		}

		itr = SIteratorNext(itr);
	}
}


static SList *ListListCopy(SList *dst, const SList *src, s_erc *error)
{
	const SListList *listSrc = (const SListList*)src;
	SListList *listDst;
	const s_list_element *itr = NULL;
	s_bool made_new = FALSE;
	const SObject *tmp;


	S_CLR_ERR(error);
	if (listSrc->list == NULL)
		return NULL;

	if (dst == NULL)
	{
		made_new = TRUE;
		listDst = (SListList*)S_NEW("SListList", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ListListCopy",
					  "Failed to create new 'SListList' object"))
			return NULL;

		dst = S_LIST(listDst);
	}


	for (itr = s_list_first(listSrc->list, error);
	     itr != NULL;
	     itr = s_list_element_next(itr, error))
	{
		if (S_CHK_ERR(error, S_FAILURE,
					  "ListListCopy",
					  "Failed to get iterator loop elements"))
		{
			if (made_new == TRUE)
				S_DELETE(dst, "ListListCopy", error);
			return NULL;
		}

		tmp = s_list_element_get(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ListListCopy",
					  "Call to \"s_list_element_get\" failed"))
		{
			if (made_new == TRUE)
				S_DELETE(dst, "ListListCopy", error);
			return NULL;
		}

		S_LIST_CALL(dst, append)(dst, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ListListCopy",
					  "Call to class method \"append\" failed"))
		{
			if (made_new == TRUE)
				S_DELETE(dst, "ListListCopy", error);
			return NULL;
		}
	}

	return dst;
}


static void ListListPush(SList *self, const SObject *object, s_erc *error)
{
	SListList *lList = (SListList*)self;
	SObject *tmp;


	S_CLR_ERR(error);
	if (lList->list == NULL)
		return;

	/* cast away const */
 	tmp = (SObject*)object;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* push */
	s_list_push(lList->list, (void*)tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListListPush",
				  "Call to s_list_push failed"))
		/* remove reference */
		SObjectDecRef(tmp);
}


static SObject *ListListPop(SList *self, s_erc *error)
{
	SListList *lList = (SListList*)self;
	SObject *tmp;


	S_CLR_ERR(error);
	if (lList->list == NULL)
		return NULL;

	tmp = (SObject*)s_list_pop(lList->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListListPop",
				  "Call to s_list_pop failed"))
		return NULL;

	/* remove reference */
	SObjectDecRef(tmp);

	return tmp;
}


static void ListListReverse(SList *self, s_erc *error)
{
	SListList *lList = (SListList*)self;


	S_CLR_ERR(error);
	if (lList->list == NULL)
		return;

	s_list_reverse(lList->list, error);
	S_CHK_ERR(error, S_CONTERR,
			  "ListListReverse",
			  "Call to s_list_reverse failed");
}


static const SObject *ListListNth(const SList *self, uint32 n, s_erc *error)
{
	SListList *lList = (SListList*)self;
	const s_list_element *nth;
	const SObject *object;


	S_CLR_ERR(error);
	if (lList->list == NULL)
		return NULL;

	nth = s_list_nth(lList->list, n, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListListNth",
				  "Call to s_list_nth failed"))
		return NULL;

	if (nth == NULL)
		return NULL;

	object = (const SObject*)s_list_element_get(nth, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListListNth",
				  "Call to s_list_element_get failed"))
		return NULL;

	return object;
}


static s_bool ListListValPresent(const SList *self, const SObject *val, s_erc *error)
{
	const SObject *object;
	SListList *lList = (SListList*)self;
	const s_list_element *itr;
	s_bool same;


	S_CLR_ERR(error);
	if (lList->list == NULL)
		return FALSE;

	itr = s_list_first(lList->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListListValPresent",
				  "Call to s_list_first failed"))
		return FALSE;

	while (itr != NULL)
	{
		object = s_list_element_get(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ListListValPresent",
					  "Call to s_list_element_get failed"))
			return FALSE;

		same = SObjectCompare(val, object, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ListListValPresent",
					  "Call to SObjectCompare failed"))
			return FALSE;

		if (same == TRUE)
			return TRUE;

		itr = s_list_element_next(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ListListValPresent",
					  "Call to s_list_element_next failed"))
			return FALSE;
	}

	return FALSE;
}


static SIterator *ListListIterator(const SContainer *self, s_erc *error)
{
	SListListIterator *itr;


	S_CLR_ERR(error);

	itr = (SListListIterator*)S_NEW("SListListIterator", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListListIterator",
				  "Failed to create new iterator"))
		return NULL;

	SListListIteratorInit(&itr, S_LISTLIST(self), error);
	S_CHK_ERR(error, S_CONTERR,
			  "ListListIterator",
			  "Failed to initialize iterator");

	return S_ITERATOR(itr);
}


/************************************************************************************/
/*                                                                                  */
/* SListList class initialization                                                   */
/*                                                                                  */
/************************************************************************************/

static SListListClass ListListClass =
{
	{
		/* SObjectClass */
		{
			"SContainer:SList:SListList",
			sizeof(SListList),
			{ 0, 1},
			InitListList,          /* init    */
			DestroyListList,       /* destroy */
			DisposeListList,       /* dispose */
			NULL,                  /* compare */
			NULL,                  /* print   */
			NULL,                  /* copy    */
		},
		/* SContainerClass */
		ListListIterator,          /* get_iterator */
	},
	/* SListClass */
	ListListIsEmpty,                /* is_empty      */
	ListListSize,                   /* size          */
	ListListAppend,                 /* append        */
	ListListPrepend,                /* prepend       */
	ListListInsertBefore,           /* insert_before */
	ListListInsertAfter,            /* insert_after  */
	ListListMerge,                  /* merge         */
	ListListCopy,                   /* copy          */
	ListListPush,                   /* push          */
	ListListPop,                    /* pop           */
	ListListReverse,                /* reverse       */
	ListListNth,                    /* nth           */
	ListListValPresent              /* val_present   */
};
