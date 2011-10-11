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
/* DATE    : 22 October 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A class for containing the shared contents of items, that is the individual      */
/* linguistic features and references to relations.                                 */
/*                                                                                  */
/* Loosely based on cst_item_contents of Flite, http://cmuflite.org (1.3),          */
/* and ItemContents of FreeTTS, http://sourceforge.net/projects/freetts (1.2)       */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned projects.                                                              */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*  Flite license, cst_item_contents                                                */
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
/* FreeTTS license, ItemContents                                                    */
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
#include "base/objsystem/objsystem.h"
#include "hrg/item_content.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Call the given function method of the given #SItmContent.
 * @param SELF The given #SItmContent*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage:
 @verbatim
 S_ITMCONTENT_CALL(self, func)(param1, param2, ..., paramN);
 @endverbatim
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_ITMCONTENT_CALL(SELF, FUNC)				\
	((SItmContentClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SItmContent
 * can be called.
 * @param SELF The given #SItmContent*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_ITMCONTENT_METH_VALID(SELF, FUNC)			\
	S_ITMCONTENT_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SItmContentClass ItmContentClass; /*  SItmContent class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SItmContentAdd(SItmContent *self, const char *relation,
						  const SItem *item, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItmContentAdd",
				  "Argument \"self\" is NULL");
		return;
	}

	if (relation == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItmContentAdd",
				  "Argument \"relation\" is NULL");
		return;
	}

	if (item == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItmContentAdd",
				  "Argument \"item\" is NULL");
		return;
	}

	if (!S_ITMCONTENT_METH_VALID(self, add))
	{
		S_WARNING(S_METHINVLD,
				  "SItmContentAdd",
				  "SItmContent method \"add\" not implemented");
		return;
	}

	s_mutex_lock(&(self->content_mutex));
	S_ITMCONTENT_CALL(self, add)(self, relation, item, error);
	s_mutex_unlock(&(self->content_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "SItmContentAdd",
			  "Failed to add relation/item pair");
}


S_API s_bool SItmContentInRelation(const SItmContent *self, const char *relation,
								   s_erc *error)
{
	s_bool s_in_relation;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItmContentInRelation",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (relation == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItmContentInRelation",
				  "Argument \"relation\" is NULL");
		return FALSE;
	}

	if (!S_ITMCONTENT_METH_VALID(self, in_relation))
	{
		S_WARNING(S_METHINVLD,
				  "SItmContentInRelation",
				  "SItmContent method \"in_relation\" not implemented");
		return FALSE;
	}

	s_mutex_lock((s_mutex*)&(self->content_mutex));
	s_in_relation = S_ITMCONTENT_CALL(self, in_relation)(self, relation, error);
	s_mutex_unlock((s_mutex*)&(self->content_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItmContentInRelation",
				  "Call to class method \"in_relation\" failed"))
		return FALSE;

	return s_in_relation;
}


S_API void SItmContentRemove(SItmContent *self, const char *relation, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItmContentRemove",
				  "Argument \"self\" is NULL");
		return;
	}

	if (relation == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItmContentRemove",
				  "Argument \"relation\" is NULL");
		return;
	}

	if (!S_ITMCONTENT_METH_VALID(self, remove))
	{
		S_WARNING(S_METHINVLD,
				  "SItmContentRemove",
				  "SItmContent method \"remove\" not implemented");
		return;
	}

	s_mutex_lock(&(self->content_mutex));
	S_ITMCONTENT_CALL(self, remove)(self, relation, error);
	s_mutex_unlock(&(self->content_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "SItmContentRemove",
			  "Call to class method \"remove\" failed");
}


S_API const SItem *SItmContentGet(const SItmContent *self, const char *relation,
								  s_erc *error)
{
	const SItem *item;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItmContentGet",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (relation == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItmContentGet",
				  "Argument \"relation\" is NULL");
		return NULL;
	}

	if (!S_ITMCONTENT_METH_VALID(self, get))
	{
		S_WARNING(S_METHINVLD,
				  "SItmContentGet",
				  "SItmContent method \"get\" not implemented");
		return NULL;
	}

	s_mutex_lock((s_mutex*)&(self->content_mutex));
	item = S_ITMCONTENT_CALL(self, get)(self, relation, error);
	s_mutex_unlock((s_mutex*)&(self->content_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItmContentGet",
				  "Call to class method \"get\" failed"))
		return NULL;

	return item;
}


S_API const SMap *SItmContentFeatures(const SItmContent *self, s_erc *error)
{
	const SMap *features;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItmContentFeatures",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITMCONTENT_METH_VALID(self, features))
	{
		S_WARNING(S_METHINVLD,
				  "SItmContentFeatures",
				  "SItmContent method \"features\" not implemented");
		return NULL;
	}

	s_mutex_lock((s_mutex*)&(self->content_mutex));
	features = S_ITMCONTENT_CALL(self, features)(self, error);
	s_mutex_unlock((s_mutex*)&(self->content_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItmContentFeatures",
				  "Call to class method \"features\" failed"))
		return NULL;

	return features;
}


S_API size_t SItmContentNumRelations(const SItmContent *self, s_erc *error)
{
	size_t size;

	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItmContentNumRelations",
				  "Argument \"self\" is NULL");
		return 0;
	}

	if (!S_ITMCONTENT_METH_VALID(self, num_relations))
	{
		S_WARNING(S_METHINVLD,
				  "SItmContentNumRelations",
				  "SItmContent method \"num_relations\" not implemented");
		return 0;
	}

	s_mutex_lock((s_mutex*)&(self->content_mutex));
	size = S_ITMCONTENT_CALL(self, num_relations)(self, error);
	s_mutex_unlock((s_mutex*)&(self->content_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItmContentNumRelations",
				  "Call to class method \"num_relations\" failed"))
		return 0;

	return size;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_itm_content_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&ItmContentClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_itm_content_class_add",
			  "Failed to add SItmContentClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitItmContent(void *obj, s_erc *error)
{
	SItmContent *self = obj;


	S_CLR_ERR(error);

	self->features = S_MAP(S_NEW(SMapList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "InitItmContent",
				  "Failed to create features map"))
		return;

	self->relations = S_MAP(S_NEW(SMapList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "InitItmContent",
				  "Failed to create relations map"))
		return;

	/* so that it will only be deleted with S_FORCE_DELETE */
	SObjectIncRef(obj);

	s_mutex_init(&(self->content_mutex));
}


static void DestroyItmContent(void *obj, s_erc *error)
{
	SItmContent *self = obj;


	S_CLR_ERR(error);

	s_mutex_lock(&(self->content_mutex));

	if (self->relations != NULL)
		S_DELETE(self->relations, "DestroyItmContent", error);

	if (self->features != NULL)
		S_DELETE(self->features, "DestroyItmContent", error);

	s_mutex_unlock(&(self->content_mutex));
	s_mutex_destroy(&(self->content_mutex));
}


static void Add(SItmContent *self, const char *relation,
				const SItem *item, s_erc *error)
{
	S_CLR_ERR(error);
	SMapSetObject(self->relations, relation, S_OBJECT(item), error);
	S_CHK_ERR(error, S_CONTERR,
			  "Add",
			  "Failed to add item from relation \"%s\" to relations map", relation);
}


static s_bool InRelation(const SItmContent *self, const char *relation,
						 s_erc *error)
{
	s_bool in_relation;


	S_CLR_ERR(error);
	in_relation = SMapObjectPresent(self->relations, relation, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "InRelation",
				  "Failed to check if relation \"%s\" is in relations map", relation))
		return FALSE;

	return in_relation;
}


static void Remove(SItmContent *self, const char *relation, s_erc *error)
{
	S_CLR_ERR(error);

	SMapObjectDelete(self->relations, relation, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Remove",
			  "Failed to delete relation \"%s\" in relations map", relation);
}


static const SItem *Get(const SItmContent *self, const char *relation,
						s_erc *error)
{
	const SObject *tmp;
	const SItem *item;


	S_CLR_ERR(error);
	tmp = SMapGetObjectDef(self->relations, relation, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Get",
				  "Failed to get relation object \"%s\" in relations map", relation))
		return NULL;

	if (tmp == NULL)
		return NULL;

	item = S_CAST(tmp, SItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Get",
				  "Failed to cast object to item"))
		return NULL;

	return item;
}


static const SMap *Features(const SItmContent *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->features;
}


static size_t NumRelations(const SItmContent *self, s_erc *error)
{
	size_t size;


	S_CLR_ERR(error);
	size = SMapSize(self->relations, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "NumRelations",
				  "Failed to get the relations map size"))
		return 0;

	return size;
}


/************************************************************************************/
/*                                                                                  */
/* SItmContent class initialization                                                 */
/*                                                                                  */
/************************************************************************************/

static SItmContentClass ItmContentClass =
{
	/* SObjectClass */
	{
		"SItmContent",
		sizeof(SItmContent),
		{ 0, 1},
		InitItmContent,         /* init    */
		DestroyItmContent,      /* destroy */
		NULL,                   /* dispose */
		NULL,                   /* compare */
		NULL,                   /* print   */
		NULL,                   /* copy    */
	},
	/* SItmContentClass */
	Add,                        /* add           */
	Get,                        /* get           */
	Remove,                     /* remove        */
	InRelation,                 /* in_relation   */
	Features,                   /* features      */
	NumRelations                /* num_relations */
};

