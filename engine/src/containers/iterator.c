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
/* DATE    : 5 October 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* An iterator for abstract data containers.                                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "containers/iterator.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SIteratorClass IteratorClass; /* Iterator class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API SIterator *SIteratorFirst(SIterator *self)
{
	SIterator *firstI;
	s_erc local_err;


	S_CLR_ERR(&local_err);

	if (self == NULL)
	{
		S_CTX_ERR(&local_err, S_ARGERROR,
				  "SIteratorFirst",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITERATOR_METH_VALID(self, first))
	{
		S_CTX_ERR(&local_err, S_METHINVLD,
				  "SIteratorFirst",
				  "Iterator method \"first\" not implemented");
		S_DELETE(self, "SIteratorFirst", &local_err);
		return NULL;
	}

	firstI = S_ITERATOR_CALL(self, first)(self, &local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "SIteratorFirst",
				  "Call to class method \"first\" failed"))
	{
		S_DELETE(self, "SIteratorFirst", &local_err);
		return NULL;
	}

	if (firstI == NULL)
		S_DELETE(self, "SIteratorFirst", &local_err);

	return firstI;
}


S_API SIterator *SIteratorLast(SIterator *self)
{
	SIterator *lastI;
	s_erc local_err;


	S_CLR_ERR(&local_err);

	if (self == NULL)
	{
		S_CTX_ERR(&local_err, S_ARGERROR,
				  "SIteratorLast",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITERATOR_METH_VALID(self, last))
	{
		S_CTX_ERR(&local_err, S_METHINVLD,
				  "SIteratorLast",
				  "Iterator method \"last\" not implemented");
		S_DELETE(self, "SIteratorLast", &local_err);
		return NULL;
	}

	lastI = S_ITERATOR_CALL(self, last)(self, &local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "SIteratorLast",
				  "Call to class method \"last\" failed"))
	{
		S_DELETE(self, "SIteratorLast", &local_err);
		return NULL;
	}

	if (lastI == NULL)
		S_DELETE(self, "SIteratorLast", &local_err);

	return lastI;
}


S_API SIterator *SIteratorNext(SIterator *self)
{
	SIterator *nextI;
	s_erc local_err;


	S_CLR_ERR(&local_err);

	if (self == NULL)
	{
		S_CTX_ERR(&local_err, S_ARGERROR,
				  "SIteratorNext",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITERATOR_METH_VALID(self, next))
	{
		S_CTX_ERR(&local_err, S_METHINVLD,
				  "SIteratorNext",
				  "Iterator method \"next\" not implemented");
		S_DELETE(self, "SIteratorNext", &local_err);
		return NULL;
	}

	nextI = S_ITERATOR_CALL(self, next)(self, &local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "SIteratorNext",
				  "Call to class method \"next\" failed"))
	{
		S_DELETE(self, "SIteratorNext", &local_err);
		return NULL;
	}

	if (nextI == NULL)
		S_DELETE(self, "SIteratorNext", &local_err);

	return nextI;
}


S_API SIterator *SIteratorPrev(SIterator *self)
{
	SIterator *prevI;
	s_erc local_err;


	S_CLR_ERR(&local_err);

	if (self == NULL)
	{
		S_CTX_ERR(&local_err, S_ARGERROR,
				  "SIteratorPrev",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITERATOR_METH_VALID(self, next))
	{
		S_CTX_ERR(&local_err, S_METHINVLD,
				  "SIteratorPrev",
				  "Iterator method \"prev\" not implemented");
		S_DELETE(self, "SIteratorPrev", &local_err);
		return NULL;
	}

	prevI = S_ITERATOR_CALL(self, prev)(self, &local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "SIteratorPrev",
				  "Call to class method \"prev\" failed"))
	{
		S_DELETE(self, "SIteratorPrev", &local_err);
		return NULL;
	}

	if (prevI == NULL)
		S_DELETE(self, "SIteratorPrev", &local_err);

	return prevI;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_iterator_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&IteratorClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_iterator_class_add",
			  "Failed to add SIteratorClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitIterator(void *obj, s_erc *error)
{
	SIterator *self = obj;


	S_CLR_ERR(error);
	self->myContainer = NULL;
}


static void DisposeIterator(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* Class initialization                                                             */
/*                                                                                  */
/************************************************************************************/

static SIteratorClass IteratorClass =
{
	/* SObjectClass */
	{
		"SIterator",
		sizeof(SIterator),
		{ 0, 1},
		InitIterator,      /* init    */
		NULL,              /* destroy */
		DisposeIterator,   /* dispose */
		NULL,              /* compare */
		NULL,              /* print   */
		NULL,              /* copy    */
	},
	/* SIteratorClass */
	NULL,                  /* first   */
	NULL,                  /* last    */
	NULL,                  /* next    */
	NULL                   /* prev    */
};

