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
/* DATE    : 12 October 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A list class, an abstract data structure that implements an ordered collection   */
/* of values, where the same value may occur more than once.                        */
/*                                                                                  */
/* Loosely based on EST_TList of Edinburgh Speech Tools,                            */
/* http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96)                          */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* Edinburgh Speech Tools license,  EST_TList                                       */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                           Copyright (c) 1995,1996                                */
/*                             All Rights Reserved.                                 */
/*       Permission is hereby granted, free of charge, to use and distribute        */
/*       this software and its documentation without restriction, including         */
/*       without limitation the rights to use, copy, modify, merge, publish,        */
/*       distribute, sublicense, and/or sell copies of this work, and to            */
/*       permit persons to whom this work is furnished to do so, subject to         */
/*       the following conditions:                                                  */
/*        1. The code must retain the above copyright notice, this list of          */
/*           conditions and the following disclaimer.                               */
/*        2. Any modifications must be clearly marked as such.                      */
/*        3. Original authors' names are not deleted.                               */
/*        4. The authors' names are not used to endorse or promote products         */
/*           derived from this software without specific prior written              */
/*           permission.                                                            */
/*       THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK              */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE           */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author :  Paul Taylor                                                      */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/objsystem/class.h"
#include "containers/list/list.h"


/************************************************************************************/
/*                                                                                  */
/*  Static variables                                                                */
/*                                                                                  */
/************************************************************************************/

static SListClass ListClass; /* List class declaration. */


/************************************************************************************/
/*                                                                                  */
/*  Function implementations                                                        */
/*                                                                                  */
/************************************************************************************/

S_API s_bool SListIsEmpty(const SList *self, s_erc *error)
{
	s_bool is_empty;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListIsEmpty",
				  "Argument \"self\" is NULL");
		return TRUE;
	}

	if (!S_LIST_METH_VALID(self, is_empty))
	{
		S_WARNING(S_METHINVLD,
				  "SListIsEmpty",
				  "List method \"is_empty\" not implemented");
		return TRUE;
	}

	S_LOCK_CONTAINER;
	is_empty = S_LIST_CALL(self, is_empty)(self, error);
	S_UNLOCK_CONTAINER;

	if (S_CHK_ERR(error, S_CONTERR,
				  "SListIsEmpty",
				  "Call to class method \"is_empty\" failed"))
		return TRUE;

	return is_empty;
}


S_API size_t SListSize(const SList *self, s_erc *error)
{
	size_t lsize;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListSize",
				  "Argument \"self\" is NULL");
		return 0;
	}

	if (!S_LIST_METH_VALID(self, size))
	{
		S_WARNING(S_METHINVLD,
				  "SListSize",
				  "List method \"size\" not implemented");
		return 0;
	}

	S_LOCK_CONTAINER;
	lsize = S_LIST_CALL(self, size)(self, error);
	S_UNLOCK_CONTAINER;

	if (S_CHK_ERR(error, S_CONTERR,
				  "SListSize",
				  "Call to class method \"size\" failed"))
		return 0;

	return lsize;
}


S_API void SListAppend(SList *self, const SObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListAppend",
				  "Argument \"self\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListAppend",
				  "Argument \"object\" is NULL");
		return;
	}

	if (!S_LIST_METH_VALID(self, append))
	{
		S_WARNING(S_METHINVLD,
				  "SListAppend",
				  "List method \"append\" not implemented");
		return;
	}

	S_LOCK_CONTAINER;
	S_LIST_CALL(self, append)(self, object, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SListAppend",
			  "Call to class method \"append\" failed");
}


S_API void SListPrepend(SList *self, const SObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListPrepend",
				  "Argument \"self\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListPrepend",
				  "Argument \"object\" is NULL");
		return;
	}

	if (!S_LIST_METH_VALID(self, prepend))
	{
		S_WARNING(S_METHINVLD,
				  "SListPrepend",
				  "List method \"prepend\" not implemented");
		return;
	}

	S_LOCK_CONTAINER;
	S_LIST_CALL(self, prepend)(self, object, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SListPrepend",
			  "Call to class method \"prepend\" failed");
}


S_API void SListInsertBefore(SList *self, SIterator *itr, const SObject *object,
							 s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListInsertBefore",
				  "Argument \"self\" is NULL");
		return;
	}

	if (itr == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListInsertBefore",
				  "Argument \"itr\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListInsertBefore",
				  "Argument \"object\" is NULL");
		return;
	}

	if (!S_LIST_METH_VALID(self, insert_before))
	{
		S_WARNING(S_METHINVLD,
				  "SListInsertBefore",
				  "List method \"insert_before\" not implemented");
		return;
	}

	S_LOCK_CONTAINER;
	S_LIST_CALL(self, insert_before)(self, itr, object, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SListInsertBefore",
			  "Call to class method \"insert_before\" failed");
}


S_API void SListInsertAfter(SList *self, SIterator *itr, const SObject *object,
							s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListInsertAfter",
				  "Argument \"self\" is NULL");
		return;
	}

	if (itr == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListInsertAfter",
				  "Argument \"itr\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListInsertAfter",
				  "Argument \"object\" is NULL");
		return;
	}

	if (!S_LIST_METH_VALID(self, insert_after))
	{
		S_WARNING(S_METHINVLD,
				  "SListInsertAfter",
				  "List method \"insert_after\" not implemented");
		return;
	}

	S_LOCK_CONTAINER;
	S_LIST_CALL(self, insert_after)(self, itr, object, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SListInsertAfter",
			  "Call to class method \"insert_after\" failed");
}


S_API void SListMerge(SList *self, const SList *with, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListMerge",
				  "Argument \"self\" is NULL");
		return;
	}

	if (with == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "",
				  "Argument \"with\" is NULL");
		return;
	}

	/*
	 * Is the merge method valid for SList self? We don't
	 * care about SList with, as long as we can get hold if it's
	 * values.
	 */
	if (!S_LIST_METH_VALID(self, merge))
	{
		S_WARNING(S_METHINVLD,
				  "SListMerge",
				  "List method \"merge\" not implemented");
		return;
	}

	/* lock both containers */
	s_mutex_lock(&(S_CONTAINER(self)->container_mutex));
	s_mutex_lock(&(S_CONTAINER(with)->container_mutex));

	S_LIST_CALL(self, merge)(self, with, error);

	/* unlock both containers */
	s_mutex_unlock(&(S_CONTAINER(self)->container_mutex));
	s_mutex_unlock(&(S_CONTAINER(with)->container_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "SListMerge",
			  "Call to class method \"merge\" failed");
}


S_API SList *SListCopy(SList *dst, const SList *src, s_erc *error)
{
	SList *tDst;


	S_CLR_ERR(error);

	if (src == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListCopy",
				  "Argument \"src\" is NULL");
		return dst;
	}

	if (!S_LIST_METH_VALID(src, copy))
	{
		S_WARNING(S_METHINVLD,
				  "SListCopy",
				  "List method \"copy\" not implemented");
		return dst;
	}

	/* lock both containers */
	if (dst != NULL)
		s_mutex_lock(&(S_CONTAINER(dst)->container_mutex));
	s_mutex_lock(&(S_CONTAINER(src)->container_mutex));

	tDst = S_LIST_CALL(src, copy)(dst, src, error);

	/* unlock both containers */
	if (dst != NULL)
		s_mutex_unlock(&(S_CONTAINER(dst)->container_mutex));
	s_mutex_unlock(&(S_CONTAINER(src)->container_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
				  "SListCopy",
				  "Call to class method \"copy\" failed"))
		return dst;

	return tDst;
}


S_API void SListPush(SList *self, const SObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListPush",
				  "Argument \"self\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListPush",
				  "Argument \"object\" is NULL");
		return;
	}

	if (!S_LIST_METH_VALID(self, push))
	{
		S_WARNING(S_METHINVLD,
				  "SListInsertPush",
				  "List method \"push\" not implemented");
		return;
	}

	S_LOCK_CONTAINER;
	S_LIST_CALL(self, push)(self, object, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SListPush",
			  "Call to class method \"push\" failed");
}


S_API SObject *SListPop(SList *self, s_erc *error)
{
	SObject *tmp;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListPop",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_LIST_METH_VALID(self, pop))
	{
		S_WARNING(S_METHINVLD,
				  "SListInsertPop",
				  "List method \"pop\" not implemented");
		return NULL;
	}

	S_LOCK_CONTAINER;
	tmp = S_LIST_CALL(self, pop)(self, error);
	S_UNLOCK_CONTAINER;

	if (S_CHK_ERR(error, S_CONTERR,
				  "SListPop",
				  "Call to class method \"pop\" failed"))
		return NULL;

	return tmp;
}


S_API SList *SListReverse(SList *self, s_erc *error)
{
	SList *tmp;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListReverse",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_LIST_METH_VALID(self, reverse))
	{
		S_WARNING(S_METHINVLD,
				  "SListReverse",
				  "List method \"reverse\" not implemented");
		return self;
	}

	S_LOCK_CONTAINER;
	tmp = S_LIST_CALL(self, reverse)(self, error);
	S_UNLOCK_CONTAINER;

	if (S_CHK_ERR(error, S_CONTERR,
				  "SListReverse",
				  "Call to class method \"reverse\" failed"))
		return self;

	return tmp;
}


S_API const SObject *SListNth(SList *self, uint32 n, s_erc *error)
{
	const SObject *tmp;
	size_t size;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListNth",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_LIST_METH_VALID(self, nth))
	{
		S_WARNING(S_METHINVLD,
				  "SListNth",
				  "List method \"nth\" not implemented");
		return NULL;
	}

	S_LOCK_CONTAINER;
	size = SListSize(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SListNth",
				  "Failed to determine list size"))
	{
		S_UNLOCK_CONTAINER;
		return NULL;
	}

	if (n >= size)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SListNth",
				  "Index %d is out of bounds, SList size is %d (indexed from 0)",
				  n, size);
		S_UNLOCK_CONTAINER;
		return NULL;
	}

	tmp = S_LIST_CALL(self, nth)(self, n, error);
	S_UNLOCK_CONTAINER;

	if (S_CHK_ERR(error, S_CONTERR,
				  "SListNth",
				  "Call to class method \"nth\" failed"))
		return NULL;

	return tmp;
}


S_API s_bool SListValPresent(const SList *self, const SObject *object, s_erc *error)
{
	s_bool is_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListValPresent",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListValPresent",
				  "Argument \"object\" is NULL");
		return FALSE;
	}

	if (!S_LIST_METH_VALID(self, val_present))
	{
		S_WARNING(S_METHINVLD,
				  "SListValPresent",
				  "List method \"val_present\" not implemented");
		return FALSE;
	}

	S_LOCK_CONTAINER;
	is_present = S_LIST_CALL(self, val_present)(self, object, error);
	S_UNLOCK_CONTAINER;

	if (S_CHK_ERR(error, S_CONTERR,
				  "SListValPresent",
				  "Call to class method \"val_present\" failed"))
		return FALSE;

	return is_present;
}


S_API SIterator *SListIterator(const SList *self, s_erc *error)
{
	SIterator *itr;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListIterator",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_LIST_METH_VALID(self, iterator))
	{
		S_WARNING(S_METHINVLD,
				  "SListIterator",
				  "List method \"iterator\" not implemented");
		return NULL;
	}

	S_LOCK_CONTAINER;
	itr = S_LIST_CALL(self, iterator)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SListIterator",
				  "Call to class method \"iterator\" failed"))
	{
		S_UNLOCK_CONTAINER;
		return NULL;
	}

	itr = SIteratorFirst(itr);
	S_UNLOCK_CONTAINER;

	return itr;
}


S_API const SObject *SListIteratorValue(const SIterator *iterator, s_erc *error)
{
	const SObject *val;


	S_CLR_ERR(error);

	if (iterator == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListIteratorValue",
				  "Argument \"iterator\" is NULL");
		return NULL;
	}

	if (!S_LIST_METH_VALID(iterator->myContainer, value))
	{
		S_WARNING(S_METHINVLD,
				  "SListIteratorValue",
				  "List method \"value\" not implemented");
		return NULL;
	}

	val = S_LIST_CALL(iterator->myContainer, value)(iterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SListIteratorValue",
				  "Call to class method \"value\" failed"))
		return NULL;

	return val;
}


S_API SObject *SListIteratorUnlink(SIterator *iterator, s_erc *error)
{
	SObject *tmp;


	S_CLR_ERR(error);

	if (iterator == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListIteratorUnlink",
				  "Argument \"iterator\" is NULL");
		return NULL;
	}

	if (!S_LIST_METH_VALID(iterator->myContainer, unlink))
	{
		S_WARNING(S_METHINVLD,
				  "SListIteratorUnlink",
				  "List method \"unlink\" not implemented");
		return NULL;
	}

	tmp = S_LIST_CALL(iterator->myContainer, unlink)(iterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SListIteratorUnlink",
				  "Call to class method \"unlink\" failed"))
		return NULL;

	return tmp;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_list_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&ListClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_list_class_add",
			  "Failed to add SListClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void DisposeList(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SList class initialization                                                       */
/*                                                                                  */
/************************************************************************************/

static SListClass ListClass =
{
	{
		/* SObjectClass */
		{
			"SContainer:SList",
			sizeof(SList),
			{ 0, 1},
			NULL,              /* init    */
			NULL,              /* destroy */
			DisposeList,       /* dispose */
			NULL,              /* compare */
			NULL,              /* print   */
			NULL,              /* copy    */
		},
		/* SContainerClass */
		/* No methods */
	},
	/* SListClass */
	NULL,              /* is_empty      */
	NULL,              /* size          */
	NULL,              /* append        */
	NULL,              /* prepend       */
	NULL,              /* insert_before */
	NULL,              /* insert_after  */
	NULL,              /* merge         */
	NULL,              /* copy          */
	NULL,              /* push          */
	NULL,              /* pop           */
	NULL,              /* reverse       */
	NULL,              /* nth           */
	NULL,              /* val_present   */
	NULL,              /* iterator      */
	NULL,              /* value         */
	NULL               /* unlink        */
};
