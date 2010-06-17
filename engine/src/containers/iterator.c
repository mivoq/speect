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
/* An abstract iterator for data containers.                                        */
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


S_API const char *SIteratorKey(SIterator *self, s_erc *error)
{
	const char *key;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SIteratorKey",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITERATOR_METH_VALID(self, key))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SIteratorKey",
				  "Iterator method \"key\" not implemented");
		return NULL;
	}

	key = S_ITERATOR_CALL(self, key)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SIteratorKey",
				  "Call to class method \"key\" failed"))
		return NULL;

	return key;
}


S_API const SObject *SIteratorObject(SIterator *self, s_erc *error)
{
	const SObject *obj;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SIteratorObject",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITERATOR_METH_VALID(self, object))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SIteratorObject",
				  "Iterator method \"object\" not implemented");
		return NULL;
	}

	obj = S_ITERATOR_CALL(self, object)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SIteratorObject",
				  "Call to class method \"object\" failed"))
		return NULL;

	return obj;
}


S_API SObject *SIteratorUnlink(SIterator *self, s_erc *error)
{
	SObject *obj;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SIteratorUnlink",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITERATOR_METH_VALID(self, unlink))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SIteratorUnlink",
				  "Iterator method \"unlink\" not implemented");
		return NULL;
	}

	obj = S_ITERATOR_CALL(self, unlink)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SIteratorUnlink",
				  "Call to class method \"unlink\" failed"))
		return NULL;

	return obj;
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
		NULL,              /* init    */
		NULL,              /* destroy */
		DisposeIterator,   /* dispose */
		NULL,              /* compare */
		NULL,              /* print   */
		NULL,              /* copy    */
	},
	/* SIteratorClass */
	NULL,                  /* next    */
	NULL,                  /* key     */
	NULL,                  /* object  */
	NULL                   /* unlink  */
};

