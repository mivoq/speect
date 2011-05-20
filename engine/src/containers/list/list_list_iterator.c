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
 * @private
 * Return the given object as an iterator object.
 * @param SELF The given object.
 * @return Given object as #SListListIterator* type.
 * @note This casting is not safety checked.
 */
#define S_LISTLIST_ITER(SELF)    ((SListListIterator *)(SELF))


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

S_LOCAL void SListListIteratorInit(SListListIterator **self, SListList *list, s_erc *error)
{
	s_bool list_is_empty;


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
		goto clean_up;
	}

	/* call list method directly, otherwise we get mutex locking
	   clashes */
	list_is_empty = S_LIST_CALL(S_LIST(list), is_empty)(S_LIST(list), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SListListIteratorInit",
				  "Call to \"SListIsEmpty\" failed"))
		goto clean_up;

	if (list_is_empty)
		goto clean_up;

	/* get first element of list and set as current element */
	(*self)->c_itr = s_list_first(list->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SListListIteratorInit",
				  "Failed to find first element of list"))
		goto clean_up;

	/* get next element of list */
	(*self)->n_itr = s_list_element_next((s_list_element*)(*self)->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SListListIteratorInit",
				  "Failed to get next element of list"))
		goto clean_up;

	/* all OK */
	return;

	/* clean up code */
clean_up:
	S_DELETE(*self, "SListListIteratorInit", error);
	*self = NULL;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_list_list_iterator_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&ListListIteratorClass), error);
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
}


static void DisposeListListIterator(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static SIterator *Next(SIterator *self, s_erc *error)
{
	SListListIterator *listItr = S_LISTLIST_ITER(self);


	S_CLR_ERR(error);
	if (listItr->n_itr == NULL)
		return NULL;

	listItr->c_itr = listItr->n_itr;
	listItr->n_itr = s_list_element_next((s_list_element*)listItr->n_itr, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Next",
			  "Failed to move to next element of list");

	return self;
}


static const SObject *Object(SIterator *self, s_erc *error)
{
	SListListIterator *listItr = S_LISTLIST_ITER(self);
	const SObject *tmp;


	S_CLR_ERR(error);
	tmp = s_list_element_get((s_list_element*)listItr->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Object",
				  "Call to \"s_list_element_get\" failed"))
		return NULL;

	return tmp;
}


static SObject *Unlink(SIterator *self, s_erc *error)
{
	SListListIterator *listItr = S_LISTLIST_ITER(self);
	SObject *tmp;


	S_CLR_ERR(error);

	if (listItr->c_itr == NULL)
		return NULL;

	tmp = s_list_element_unlink((s_list_element*)listItr->c_itr, error);
	listItr->c_itr = NULL;
	if (S_CHK_ERR(error, S_CONTERR,
				  "Unlink",
				  "Call to \"s_list_element_unlink\" failed"))
		return NULL;

	/* remove the object's reference to the container */
	SObjectDecRef(tmp);

	return tmp;
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
	Next,                         /* next    */
	NULL,                         /* key     */
	Object,                       /* object  */
	Unlink                        /* unlink  */
};

