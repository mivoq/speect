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
/* DATE    : 27 October 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Relation is a named list of Items. An Utterance can hold an arbitrary number   */
/* of Relations.                                                                    */
/*                                                                                  */
/* Loosely based on cst_relation of Flite, http://cmuflite.org (1.3),               */
/* and Relation of FreeTTS, http://sourceforge.net/projects/freetts (1.2).          */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned projects.                                                              */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*  Flite license, cst_relation                                                     */
/*                                                                                  */
/*                       Language Technologies Institute                            */
/*                          Carnegie Mellon University                              */
/*                             Copyright (c) 1999                                   */
/*                             All Rights Reserved.                                 */
/*                                                                                  */
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
/*                                                                                  */
/*       CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK               */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE            */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author:  Alan W Black                                                      */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* FreeTTS license, Relation                                                        */
/*                                                                                  */
/*      Portions Copyright 2001-2004 Sun Microsystems, Inc.                         */
/*      Portions Copyright 1999-2001 Language Technologies Institute,               */
/*      Carnegie Mellon University.                                                 */
/*      All Rights Reserved.  Use is subject to license terms.                      */
/*                                                                                  */
/*      Permission is hereby granted, free of charge, to use and distribute         */
/*      this software and its documentation without restriction, including          */
/*      without limitation the rights to use, copy, modify, merge, publish,         */
/*      distribute, sublicense, and/or sell copies of this work, and to             */
/*      permit persons to whom this work is furnished to do so, subject to          */
/*      the following conditions:                                                   */
/*                                                                                  */
/*       1. The code must retain the above copyright notice, this list of           */
/*          conditions and the following disclaimer.                                */
/*       2. Any modifications must be clearly marked as such.                       */
/*       3. Original authors' names are not deleted.                                */
/*       4. The authors' names are not used to endorse or promote products          */
/*          derived from this software without specific prior written               */
/*          permission.                                                             */
/*                                                                                  */
/*      SUN MICROSYSTEMS, INC., CARNEGIE MELLON UNIVERSITY AND THE                  */
/*      CONTRIBUTORS TO THIS WORK DISCLAIM ALL WARRANTIES WITH REGARD TO THIS       */
/*      SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND           */
/*      FITNESS, IN NO EVENT SHALL SUN MICROSYSTEMS, INC., CARNEGIE MELLON          */
/*      UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR      */
/*      CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF      */
/*      USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR       */
/*      OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR      */
/*      PERFORMANCE OF THIS SOFTWARE.                                               */
/*                                                                                  */
/*      Authors: Willie Walker, Paul Lamere, Philip Kwok                            */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "base/strings/strings.h"
#include "containers/containers.h"
#include "base/objsystem/objsystem.h"
#include "hrg/item.h"
#include "hrg/relation.h"
#include "hrg/utterance.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SRelationClass RelationClass; /* SRelation class declaration. */

static const char * const _s_relation_noname = "NoName";


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SRelationInit(SRelation **self, const char *name, s_erc *error)
{
	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SRelationInit",
			  "Argument \"self\" is NULL");
		return;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SRelationInit",
			  "Argument \"self\" is NULL");
		S_FORCE_DELETE(*self, "SRelationInit", error);
		*self = NULL;
		return;
	}

	(*self)->name = s_strdup(name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SRelationInit",
				  "Call to \"s_strdup\" failed"))
	{
		S_FORCE_DELETE(*self, "SRelationInit", error);
		*self = NULL;
		return;
	}
}


S_API const char *SRelationName(const SRelation *self, s_erc *error)
{
	const char *name;

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SRelationName",
			  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_RELATION_METH_VALID(self, name))
	{
		S_CTX_ERR(error, S_METHINVLD,
			  "SRelationName",
			  "Relation method \"name\" not implemented");
		return NULL;
	}

	name = S_RELATION_CALL(self, name)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "SRelationName",
		      "Call to class method \"name\" failed"))
		return NULL;

	return name;
}


S_API const SUtterance *SRelationUtterance(const SRelation *self, s_erc *error)
{
	const SUtterance *utt;

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SRelationUtterance",
			  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_RELATION_METH_VALID(self, utt))
	{
		S_CTX_ERR(error, S_METHINVLD,
			  "SRelationUtterance",
			  "Relation method \"utt\" not implemented");
		return NULL;
	}

	utt = S_RELATION_CALL(self, utt)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "SRelationUtterance",
		      "Call to class method \"utt\" failed"))
		return NULL;

	return utt;
}


S_API const SItem *SRelationHead(const SRelation *self, s_erc *error)
{
	const SItem *item;

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SRelationHead",
			  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_RELATION_METH_VALID(self, head))
	{
		S_CTX_ERR(error, S_METHINVLD,
			  "SRelationHead",
			  "Relation method \"head\" not implemented");
		return NULL;
	}

	item = S_RELATION_CALL(self, head)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "SRelationHead",
		      "Call to class method \"head\" failed"))
		return NULL;

	return item;
}


S_API const SItem *SRelationTail(const SRelation *self, s_erc *error)
{
	const SItem *item;

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SRelationTail",
			  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_RELATION_METH_VALID(self, tail))
	{
		S_CTX_ERR(error, S_METHINVLD,
			  "SRelationTail",
			  "Relation method \"tail\" not implemented");
		return NULL;
	}

	item = S_RELATION_CALL(self, tail)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "SRelationTail",
		      "Call to class method \"tail\" failed"))
		return NULL;

	return item;
}


S_API SItem *SRelationAppend(SRelation *self, const SItem *toShare, s_erc *error)
{
	SItem *pItem;

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SRelationAppend",
			  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_RELATION_METH_VALID(self, append))
	{
		S_CTX_ERR(error, S_METHINVLD,
			  "SRelationAppend",
			  "Relation method \"append\" not implemented");
		return NULL;
	}

	s_mutex_lock(&(self->rel_mutex));
	pItem = S_RELATION_CALL(self, append)(self, toShare, error);
	s_mutex_unlock(&(self->rel_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
		      "SRelationAppend",
		      "Call to class method \"append\" failed"))
		return NULL;

	return pItem;
}


S_API SItem *SRelationPrepend(SRelation *self, const SItem *toShare, s_erc *error)
{
	SItem *pItem;

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SRelationPrepend",
			  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_RELATION_METH_VALID(self, prepend))
	{
		S_CTX_ERR(error, S_METHINVLD,
			  "SRelationPrepend",
			  "Relation method \"prepend\" not implemented");
		return NULL;
	}

	s_mutex_lock(&(self->rel_mutex));
	pItem = S_RELATION_CALL(self, prepend)(self, toShare, error);
	s_mutex_unlock(&(self->rel_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
		      "SRelationPrepend",
		      "Call to class method \"prepend\" failed"))
		return NULL;

	return pItem;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_relation_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&RelationClass), error);
	S_CHK_ERR(error, S_CONTERR,
		  "_s_relation_class_add",
		  "Failed to add SRelationClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitRelation(void *obj, s_erc *error)
{
	SRelation *self = obj;


	S_CLR_ERR(error);
	self->name = NULL;
	self->head = NULL;
	self->tail = NULL;
	self->utterance = NULL;

	/* so that it will only be deleted with S_FORCE_DELETE */
	SObjectIncRef(obj);

	s_mutex_init(&(self->rel_mutex));
}


static void DestroyRelation(void *obj, s_erc *error)
{
	SRelation *self = obj;
	SItem *item;
	SItem *nextItem;

	S_CLR_ERR(error);

	s_mutex_lock(&(self->rel_mutex));

	item = self->head;
	while (item != NULL)
	{
		nextItem = item->next;

		/* this deletes daughters as well */
		S_FORCE_DELETE(item, "DestroyRelation", error);

		item = nextItem;
	}

	if (self->name)
		S_FREE(self->name);

	s_mutex_unlock(&(self->rel_mutex));
	s_mutex_destroy(&(self->rel_mutex));
}


static const char *RelationName(const SRelation *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->name? self->name : _s_relation_noname;
}


static const SUtterance *RelationUtterance(const SRelation *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->utterance;
}


static const SItem *RelationHead(const SRelation *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->head;
}


static const SItem *RelationTail(const SRelation *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->tail;
}


static SItem *RelationAppend(SRelation *self, const SItem *toShare, s_erc *error)
{
	SItem *newItem;


	S_CLR_ERR(error);
	newItem = (SItem*)S_NEW("SItem", error);
	if (S_CHK_ERR(error, S_FAILURE,
		      "RelationAppend",
		      "Failed to create new item"))
		return NULL;

	_SItemInit_no_lock(&newItem, self, toShare, error);
	if (S_CHK_ERR(error, S_FAILURE,
		      "RelationAppend",
		      "Failed to initialize new item"))
		return NULL;

	if (self->head == NULL)
		self->head = newItem;

	newItem->prev = self->tail;

	if (self->tail != NULL)
		self->tail->next = newItem;

	self->tail = newItem;

	return newItem;
}


static SItem *RelationPrepend(SRelation *self, const SItem *toShare, s_erc *error)
{
	SItem *newItem;


	S_CLR_ERR(error);
	newItem = (SItem*)S_NEW("SItem", error);
	if (S_CHK_ERR(error, S_FAILURE,
		      "RelationAppend",
		      "Failed to create new item"))
		return NULL;

	_SItemInit_no_lock(&newItem, self, toShare, error);
	if (S_CHK_ERR(error, S_FAILURE,
		      "RelationAppend",
		      "Failed to initialize new item"))
		return NULL;

	if (self->tail == NULL)
		self->tail = newItem;

	newItem->next = self->head;

	if (self->head != NULL)
		self->head->prev = newItem;

	self->head = newItem;

	return newItem;
}


/************************************************************************************/
/*                                                                                  */
/* SRelation class initialization                                                   */
/*                                                                                  */
/************************************************************************************/

static SRelationClass RelationClass =
{
	/* SObjectClass */
	{
		"SRelation",
		sizeof(SRelation),
		{ 0, 1},
		InitRelation,      /* init    */
		DestroyRelation,   /* destroy */
		NULL,              /* dispose */
		NULL,              /* compare */
		NULL,              /* print   */
		NULL,              /* copy    */
	},
	/* SRelationClass */
	RelationName,              /* name    */
	RelationUtterance,         /* utt     */
	RelationHead,              /* head    */
	RelationTail,              /* tail    */
	RelationAppend,            /* append  */
	RelationPrepend            /* prepend */
};

