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
/* An abstract container class.                                                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "containers/container.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SContainerClass ContainerClass; /* Container class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API SIterator *SContainerGetIterator(const SContainer *self, s_erc *error)
{
	SIterator *itr;


	S_CLR_ERR(error);
	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SContainerGetIterator",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_CONTAINER_METH_VALID(self, get_iterator))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SContainerGetIterator",
				  "Container method \"get_iterator\" not implemented");
		return NULL;
	}

	S_LOCK_CONTAINER;
	itr = S_CONTAINER_CALL(self, get_iterator)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SContainerGetIterator",
				  "Call to class method \"get_iterator\" failed"))
	{
		S_UNLOCK_CONTAINER;
		return NULL;
	}

	S_UNLOCK_CONTAINER;

	return itr;
}


/* private */
/* helper function, see S_ITERATOR_GET macro in container.h */
S_API SIterator *_s_container_get_iterator_check(const void *self, s_erc *error)
{
	const SContainer *tmp;


	S_CLR_ERR(error);

	tmp = S_CAST(S_OBJECT(self), SContainer, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_container_get_iterator_check",
				  "Failed to cast given object to 'SContainer'"))
		return NULL;

	return SContainerGetIterator(S_CONTAINER(self), error);
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_container_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&ContainerClass, error);
	S_CHK_ERR(error, S_CONTERR,
		  "_s_container_class_add",
		  "Failed to add SContainerClass");
}

/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

/*
 * We only create/destroy the mutex if SPCT_USE_THREADS
 * is defined, else there is no need for this
 * constructor/destructor.
 */
#ifdef SPCT_USE_THREADS
static void InitContainer(void *obj, s_erc *error)
{
	SContainer *self = obj;


	S_CLR_ERR(error);
	s_mutex_init(&(self->container_mutex));
}


static void DestroyContainer(void *obj, s_erc *error)
{
	SContainer *self = obj;

	S_CLR_ERR(error);
	s_mutex_destroy(&(self->container_mutex));
}
#endif /* SPCT_USE_THREADS */


static void DisposeContainer(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SContainer class initialization                                                  */
/*                                                                                  */
/************************************************************************************/

static SContainerClass ContainerClass =
{
	/* SObjectClass */
	{
		"SContainer",
		sizeof(SContainer),
		{ 0, 1},
#ifdef SPCT_USE_THREADS
		InitContainer,     /* init    */
		DestroyContainer,  /* destroy */
#else /*!SPCT_USE_THREADS */
		NULL,              /* init    */
		NULL,              /* destroy */
#endif /* SPCT_USE_THREADS */
		DisposeContainer,  /* dispose */
		NULL,              /* compare */
		NULL,              /* print   */
		NULL,              /* copy    */
	},
	/* SContainerClass */
	NULL                   /* get_iterator */
};

