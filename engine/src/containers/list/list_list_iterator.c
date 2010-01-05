/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* DATE    : 14 October 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Iterator implementation for SListList container.                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/objsystem/class.h"
#include "containers/list/list_list_iterator.h"


/************************************************************************************/
/*                                                                                  */
/* SListListIteratorClass definition                                                */
/*                                                                                  */
/************************************************************************************/

/**
 * The SListListIterator class structure.  Same as SIteratorClass as
 * we are not adding any new methods.
 */
typedef SIteratorClass SListListIteratorClass;


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given object as an #SListList object.
 * @param SELF The given object.
 * @return Given object as #SListList* type.
 * @note This casting is not safety checked.
 */
#define S_LISTLIST(SELF)    ((SListList *)(SELF))


/**
 * @hideinitializer
 * @private
 * Return the given object as an iterator object.
 * @param SELF The given object.
 * @return Given object as #SListListIterator* type.
 * @note This casting is not safety checked.
 */
#define S_LISTLIST_ITER(SELF)    ((SListListIterator *)(SELF))


/**
 * @hideinitializer
 * @private
 * Call the given function method of the given #SListListIterator,
 * see full description #S_LISTLIST_ITER_CALL for usage.
 * @param SELF The given #SListListIterator*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage: @code S_LISTLIST_ITER_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_LISTLIST_ITER_CALL(SELF, FUNC)					\
	((SListListIteratorClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * @private
 * Test if the given function method of the given #SListListIterator
 * can be called.
 * @param SELF The given #SListListIterator*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_LISTLIST_ITER_METH_VALID(SELF, FUNC)		\
	S_LISTLIST_ITER_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SListListIteratorClass ListListIteratorClass; /* ListListIterator class declaration. */


/************************************************************************************/
/*                                                                                  */
/*  Function implementations                                                        */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void SListListIteratorInit(SIterator **self, SListList *list, s_erc *error)
{
	SListListIterator *listListItr;


	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListListIteratorInit",
				  "Argument \"self\" is NULL");
		return;
	}

	if (list == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListListIteratorInit",
				  "Argument \"list\" is NULL");
		S_DELETE(*self, "SListListIteratorInit", error);
		*self = NULL;
		return;
	}

	listListItr = S_CAST(*self, SListListIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SListListIteratorInit",
				  "Argument \"self\" is not of SListListIterator type"))
	{
		S_DELETE(*self, "SListListIteratorInit", error);
		*self = NULL;
		return;
	}

	(*self)->myContainer = S_CONTAINER(list);
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_list_list_iterator_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&ListListIteratorClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_list_list_iterator_class_add",
			  "Failed to add SListListIteratorClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitListListIterator(void *obj, s_erc *error)
{
	SListListIterator *listItr = obj;

	S_CLR_ERR(error);
	listItr->c_itr = NULL;
	listItr->n_itr = NULL;
	listItr->p_itr = NULL;
}


static void DisposeListListIterator(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static SIterator *First(SIterator *self, s_erc *error)
{
	SListListIterator *listItr = S_LISTLIST_ITER(self);
	s_bool list_is_empty;


	S_CLR_ERR(error);
	listItr->c_itr = s_list_first(S_LISTLIST(self->myContainer)->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "First",
				  "Failed to find first element of list"))
		return self;

	list_is_empty = s_list_isempty(S_LISTLIST(self->myContainer)->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "First",
				  "Call to \"s_list_isempty\" failed"))
		return self;

	if (listItr->c_itr == NULL)
	{
		if (list_is_empty == FALSE)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "First",
					  "Failed to move to first element of list");
			return self;
		}
		else
		{
			return NULL;
		}
	}

	listItr->p_itr = NULL;
	listItr->n_itr = s_list_element_next((s_list_element*)listItr->c_itr, error);
	S_CHK_ERR(error, S_CONTERR,
			  "First",
			  "Failed to get next element of list");

	return self;
}


static SIterator *Last(SIterator *self, s_erc *error)
{
	SListListIterator *listItr = S_LISTLIST_ITER(self);
	s_bool list_is_empty;


	S_CLR_ERR(error);
	listItr->c_itr = s_list_last(S_LISTLIST(self->myContainer)->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Last",
				  "Failed to find last element of list"))
		return self;

	list_is_empty = s_list_isempty(S_LISTLIST(self->myContainer)->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Last",
				  "Call to s_list_isempty failed"))
		return self;

	if (listItr->c_itr == NULL)
	{
		if (list_is_empty == FALSE)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "Last",
					  "Failed to move to last element of list");
			return self;
		}
		else if (listItr->c_itr == NULL)
		{
			return NULL;
		}
	}

	listItr->n_itr = NULL;
	listItr->p_itr = s_list_element_prev((s_list_element*)listItr->c_itr, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Last",
			  "Failed to get previous element of list");

	return self;
}


static SIterator *Next(SIterator *self, s_erc *error)
{
	SListListIterator *listItr = S_LISTLIST_ITER(self);


	S_CLR_ERR(error);
	if (listItr->n_itr == NULL)
		return NULL;

	listItr->p_itr = listItr->c_itr;
	listItr->c_itr = listItr->n_itr;
	listItr->n_itr = s_list_element_next((s_list_element*)listItr->n_itr, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Next",
			  "Failed to move to next element of list");

	return self;
}


static SIterator *Prev(SIterator *self, s_erc *error)
{
	SListListIterator *listItr = S_LISTLIST_ITER(self);


	S_CLR_ERR(error);
	if (listItr->p_itr == NULL)
		return NULL;

	listItr->n_itr = listItr->c_itr;
	listItr->c_itr = listItr->p_itr;
	listItr->p_itr = s_list_element_prev((s_list_element*)listItr->p_itr, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Prev",
			  "Failed to move to prev element of list");

	return self;
}


/************************************************************************************/
/*                                                                                  */
/* SListListIterator class initialization                                           */
/*                                                                                  */
/************************************************************************************/

static SListListIteratorClass ListListIteratorClass =
{
	/* SObjectClass */
	{
		"SIterator:SListListIterator",
		sizeof(SListListIterator),
		{ 0, 1},
		InitListListIterator,     /* init    */
		NULL,                     /* destroy */
		DisposeListListIterator,  /* dispose */
		NULL,                     /* compare */
		NULL,                     /* print   */
		NULL,                     /* copy    */
	},
	/* SIteratorClass */
	First,                        /* first   */
	Last,                         /* last    */
	Next,                         /* next    */
	Prev                          /* prev    */
};

