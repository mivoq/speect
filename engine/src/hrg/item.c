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
/* DATE    : 24 October 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A class for containing the items (arbitrary objects). Items are nodes in a       */
/* relation which is again linked to an utterance. Items contain a key-value        */
/* (string/object) set of features. An Item can have a list of daughter items as    */
/* well. Items can be shared between relations.                                     */
/*                                                                                  */
/* Loosely based on cst_item of Flite, http://cmuflite.org (1.3), and               */
/* EST_Item of Edinburgh Speech Tools,                                              */
/* http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96).                         */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned projects.                                                              */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*  Flite license, cst_item                                                         */
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
/* Edinburgh Speech Tools license,  EST_Item                                        */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                              Copyright (c) 1998                                  */
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
/*       Author :  Alan W Black                                                     */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "base/objsystem/objsystem.h"
#include "hrg/utterance.h"
#include "hrg/relation.h"
#include "hrg/item.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/*
 * Lock the item, this is done by locking the relation, as
 * relation<->item are interdependent.
 */
#define S_LOCK_ITEM(SELF)						\
	s_mutex_lock(&((SELF)->relation->rel_mutex))


/*
 * Unlock the item.
 */
#define S_UNLOCK_ITEM(SELF)						\
	s_mutex_unlock(&((SELF)->relation->rel_mutex))


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SItemClass ItemClass;  /* SItem class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function definitions                                                      */
/*                                                                                  */
/************************************************************************************/

static void assign_id(SItem *self, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/


S_API void SItemInit(SItem **self, const SRelation *rel,
					 const SItem *toShare, s_erc *error)
{
	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemInit",
				  "Argument \"self\" is NULL");
		return;
	}

	if (rel == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemInit",
				  "Argument \"rel\" is NULL");
		S_FORCE_DELETE(*self, "SItemInit", error);
		*self = NULL;
		return;
	}

	(*self)->relation = (SRelation*)rel;

	if (toShare == NULL) /* new content    */
	{
		(*self)->content = (SItmContent*)S_NEW("SItmContent", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SItemInit",
					  "Failed to create new item contents"))
		{
			S_FORCE_DELETE(*self, "SItemInit", error);
			*self = NULL;
			return;
		}
	}
	else                /* shared content */
	{
		/*
		 * ABY: Check this, old Speect
		 * SPCT_ICRemoveItemRelation(current->contents,
		 * current->relation->name);
		 */
		(*self)->content = toShare->content;
	}

	S_LOCK_ITEM(*self);
	SItmContentAdd((*self)->content, (*self)->relation->name,
				   *self, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemInit",
				  "Failed to add new relation/item pair for relation \"%s\"",
				  (*self)->relation->name? (*self)->relation->name : "NULL"))
	{
		S_FORCE_DELETE(*self, "SItemInit", error);
		*self = NULL;
		return;
	}

	assign_id(*self, error);
	S_UNLOCK_ITEM(*self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemInit",
				  "Call to \"assign_id\" failed"))
	{
		S_FORCE_DELETE(*self, "SItemInit", error);
		*self = NULL;
	}
}


/* This is almost the same as SItemInit except that no mutex locking
 * is done. If we don't do this then a relation cannot create items.
 * This *must* only be called in SRelation and SItem functions
 */
S_LOCAL void _SItemInit_no_lock(SItem **self, const SRelation *rel,
								const SItem *toShare, s_erc *error)
{
	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "_SItemInit_no_lock",
				  "Argument \"self\" is NULL");
		return;
	}

	if (rel == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "_SItemInit_no_lock",
				  "Argument \"rel\" is NULL");
		S_FORCE_DELETE(*self, "_SItemInit_no_lock", error);
		*self = NULL;
		return;
	}

	(*self)->relation = (SRelation*)rel;

	if (toShare == NULL) /* new content    */
	{
		(*self)->content = (SItmContent*)S_NEW("SItmContent", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_SItemInit_no_lock",
					  "Failed to create new item contents"))
		{
			S_FORCE_DELETE(*self, "_SItemInit_no_lock", error);
			*self = NULL;
			return;
		}
	}
	else                /* shared content */
	{
		/*
		 * ABY: Check this, old Speect
		 * SPCT_ICRemoveItemRelation(current->contents,
		 * current->relation->name);
		 */
		(*self)->content = toShare->content;
	}

	/* don't lock */
	/*	S_LOCK_ITEM(*self); */
	SItmContentAdd((*self)->content, (*self)->relation->name,
				   *self, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "_SItemInit_no_lock",
				  "Failed to add new relation/item pair for relation \"%s\"",
				  (*self)->relation->name? (*self)->relation->name : "NULL"))
	{
		S_FORCE_DELETE(*self, "_SItemInit_no_lock", error);
		*self = NULL;
		return;
	}

	assign_id(*self, error);

	/* don't unlock */
	/* S_UNLOCK_ITEM(*self); */

	if (S_CHK_ERR(error, S_CONTERR,
				  "_SItemInit_no_lock",
				  "Call to \"assign_id\" failed"))
	{
		S_FORCE_DELETE(*self, "_SItemInit_no_lock", error);
		*self = NULL;
	}
}


S_API SItem *SItemAs(const SItem *self, const char *relation, s_erc *error)
{
	SItem *itemAs;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemAs",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (relation == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemAs",
				  "Argument \"relation\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, item_as))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemAs",
				  "Item method \"item_as\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	itemAs = S_ITEM_CALL(self, item_as)(self, relation, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemAs",
				  "Call to class method \"item_as\" failed"))
		return NULL;

	return itemAs;
}


S_API s_bool SItemIn(const SItem *self, const char *relation, s_erc *error)
{
	s_bool itemIn;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemIn",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (relation == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemIn",
				  "Argument \"relation\" is NULL");
		return FALSE;
	}

	if (!S_ITEM_METH_VALID(self, item_in))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemIn",
				  "Item method \"item_in\" not implemented");
		return FALSE;
	}

	S_LOCK_ITEM(self);
	itemIn = S_ITEM_CALL(self, item_in)(self, relation, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemIn",
				  "Call to class method \"item_in\" failed"))
		return FALSE;

	return itemIn;
}


S_API SItem *SItemNext(const SItem *self, s_erc *error)
{
	SItem *nextItem;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemNext",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, item_next))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemNext",
				  "Item method \"item_next\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	nextItem = S_ITEM_CALL(self, item_next)(self, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemNext",
				  "Call to class method \"item_next\" failed"))
		return NULL;

	return nextItem;
}


S_API SItem *SItemPrev(const SItem *self, s_erc *error)
{
	SItem *prevItem;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemPrev",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, item_prev))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemPrev",
				  "Item method \"item_prev\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	prevItem = S_ITEM_CALL(self, item_prev)(self, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemPrev",
				  "Call to class method \"item_prev\" failed"))
		return NULL;

	return prevItem;
}


S_API SItem *SItemAppend(SItem *self, const SItem *toShare, s_erc *error)
{
	SItem *appendedItem;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemAppend",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, item_append))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemAppend",
				  "Item method \"item_append\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	appendedItem = S_ITEM_CALL(self, item_append)(self, toShare, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemAppend",
				  "Call to class method \"item_append\" failed"))
		return NULL;

	return appendedItem;
}


S_API SItem *SItemPrepend(SItem *self, const SItem *toShare, s_erc *error)
{
	SItem *prependedItem;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemPrepend",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, item_prepend))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemPrepend",
				  "Item method \"item_prepend\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	prependedItem = S_ITEM_CALL(self, item_prepend)(self, toShare, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemPrepend",
				  "Call to class method \"item_prepend\" failed"))
		return NULL;

	return prependedItem;
}


S_API SItem *SItemParent(const SItem *self, s_erc *error)
{
	SItem *parentItem;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemParent",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, item_parent))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemParent",
				  "Item method \"item_parent\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	parentItem = S_ITEM_CALL(self, item_parent)(self, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemParent",
				  "Call to class method \"item_parent\" failed"))
		return NULL;

	return parentItem;
}


S_API SItem *SItemDaughter(const SItem *self, s_erc *error)
{
	SItem *daughterItem;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemDaughter",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, item_daughter))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemDaughter",
				  "Item method \"item_daughter\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	daughterItem = S_ITEM_CALL(self, item_daughter)(self, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemDaughter",
				  "Call to class method \"item_daughter\" failed"))
		return NULL;

	return daughterItem;
}


S_API SItem *SItemNthDaughter(const SItem *self, uint32 n, s_erc *error)
{
	SItem *nthDaughter;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemNthDaughter",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, nth_daughter))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemNthDaughter",
				  "Item method \"nth_daughter\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	nthDaughter = S_ITEM_CALL(self, nth_daughter)(self, n, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemNthDaughter",
				  "Call to class method \"nth_daughter\" failed"))
		return NULL;

	return nthDaughter;
}


S_API SItem *SItemLastDaughter(const SItem *self, s_erc *error)
{
	SItem *lastDaughter;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemLastDaughter",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, last_daughter))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemLastDaughter",
				  "Item method \"last_daughter\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	lastDaughter = S_ITEM_CALL(self, last_daughter)(self, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemLastDaughter",
				  "Call to class method \"last_daughter\" failed"))
		return NULL;

	return lastDaughter;
}


S_API SItem *SItemAddDaughter(SItem *self, const SItem *toShare, s_erc *error)
{
	SItem *newAddedDaughter;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemAddDaughter",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, add_daughter))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemAddDaughter",
				  "Item method \"add_daughter\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	newAddedDaughter = S_ITEM_CALL(self, add_daughter)(self, toShare, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemAddDaughter",
				  "Call to class method \"add_daughter\" failed"))
		return NULL;

	return newAddedDaughter;
}


S_API const char *SItemGetName(const SItem *self, s_erc *error)
{
	const char *name;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemGetName",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, get_string))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemGetName",
				  "Item method \"get_string\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	name = S_ITEM_CALL(self, get_string)(self, "name", error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemGetName",
				  "Call to class method \"get_string\" failed"))
		return NULL;

	return name;
}


S_API void SItemSetName(SItem *self, const char *name, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemSetName",
				  "Argument \"self\" is NULL");
		return;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemSetName",
				  "Argument \"name\" is NULL");
		return;
	}

	if (!S_ITEM_METH_VALID(self, set_string))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemSetName",
				  "Item method \"set_string\" not implemented");
		return;
	}

	S_LOCK_ITEM(self);
	S_ITEM_CALL(self, set_string)(self, "name", name, error);
	S_UNLOCK_ITEM(self);

	S_CHK_ERR(error, S_CONTERR,
			  "SItemSetName",
			  "Call to class method \"set_string\" failed");
}


S_API s_bool SItemFeatureIsPresent(const SItem *self, const char *name, s_erc *error)
{
	s_bool feat_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemFeatureIsPresent",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemFeatureIsPresent",
				  "Argument \"name\" is NULL");
		return FALSE;
	}

	if (!S_ITEM_METH_VALID(self, feat_present))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemFeatureIsPresent",
				  "Item method \"feat_present\" not implemented");
		return FALSE;
	}

	S_LOCK_ITEM(self);
	feat_present = S_ITEM_CALL(self, feat_present)(self, name, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemFeatureIsPresent",
				  "Call to class method \"feat_present\" failed"))
		return FALSE;

	return feat_present;
}


S_API void SItemDelFeature(SItem *self, const char *name, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemDelFeature",
				  "Argument \"self\" is NULL");
		return;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemDelFeature",
				  "Argument \"name\" is NULL");
		return;
	}

	if (!S_ITEM_METH_VALID(self, feat_delete))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemDelFeature",
				  "Item method \"feat_delete\" not implemented");
		return;
	}

	S_LOCK_ITEM(self);
	S_ITEM_CALL(self, feat_delete)(self, name, error);
	S_UNLOCK_ITEM(self);

	S_CHK_ERR(error, S_CONTERR,
			  "SItemDelFeature",
			  "Call to class method \"feat_delete\" failed");
}


S_API SList *SItemFeatKeys(const SItem *self, s_erc *error)
{
	SList *featKeys;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemFeatKeys",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, feat_keys))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemFeatKeys",
				  "Item method \"feat_keys\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	featKeys = S_ITEM_CALL(self, feat_keys)(self, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemFeatKeys",
				  "Call to class method \"feat_keys\" failed"))
		return NULL;

	return featKeys;
}


S_API const SObject *SItemGetObject(const SItem *self, const char *name, s_erc *error)
{
	const SObject *object;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemGetObject",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemGetObject",
				  "Argument \"name\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, get_object))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemGetObject",
				  "Item method \"get_object\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	object = S_ITEM_CALL(self, get_object)(self, name, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemGetObject",
				  "Call to class method \"get_object\" failed"))
		return NULL;

	return object;
}


S_API sint32 SItemGetInt(const SItem *self, const char *name, s_erc *error)
{
	sint32 int_val;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemGetInt",
				  "Argument \"self\" is NULL");
		return 0;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemGetInt",
				  "Argument \"name\" is NULL");
		return 0;
	}

	if (!S_ITEM_METH_VALID(self, get_int))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemGetInt",
				  "Item method \"get_int\" not implemented");
		return 0;
	}

	S_LOCK_ITEM(self);
	int_val = S_ITEM_CALL(self, get_int)(self, name, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemGetInt",
				  "Call to class method \"get_int\" failed"))
		return 0;

	return int_val;
}


S_API float SItemGetFloat(const SItem *self, const char *name, s_erc *error)
{
	float float_val;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemGetFloat",
				  "Argument \"self\" is NULL");
		return 0.0;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemGetFloat",
				  "Argument \"name\" is NULL");
		return 0.0;
	}

	if (!S_ITEM_METH_VALID(self, get_float))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemGetFloat",
				  "Item method \"get_float\" not implemented");
		return 0.0;
	}

	S_LOCK_ITEM(self);
	float_val = S_ITEM_CALL(self, get_float)(self, name, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemGetFloat",
				  "Call to class method \"get_float\" failed"))
		return 0.0;

	return float_val;
}


S_API const char *SItemGetString(const SItem *self, const char *name, s_erc *error)
{
	const char *string_val;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemGetString",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemGetString",
				  "Argument \"name\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, get_string))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemGetString",
				  "Item method \"get_string\"  not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	string_val = S_ITEM_CALL(self, get_string)(self, name, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemGetString",
				  "Call to class method \"get_string\" failed"))
		return NULL;

	return string_val;
}


S_API void SItemSetObject(SItem *self, const char *name,
						  const SObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemSetObject",
				  "Argument \"self\" is NULL");
		return;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemSetObject",
				  "Argument \"name\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemSetObject",
				  "Argument \"object\" is NULL");
		return;
	}

	if (!S_ITEM_METH_VALID(self, set_object))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemSetObject",
				  "Item method \"set_object\" not implemented");
		return;
	}

	S_LOCK_ITEM(self);
	S_ITEM_CALL(self, set_object)(self, name, object, error);
	S_UNLOCK_ITEM(self);

	S_CHK_ERR(error, S_CONTERR,
			  "SItemSetObject",
			  "Call to class method \"set_object\" failed");
}


S_API void SItemSetInt(SItem *self, const char *name, sint32 i, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemSetInt",
				  "Argument \"self\" is NULL");
		return;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemSetInt",
				  "Argument \"name\" is NULL");
		return;
	}

	if (!S_ITEM_METH_VALID(self, set_int))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemSetInt",
				  "Item method \"set_int\" not implemented");
		return;
	}

	S_LOCK_ITEM(self);
	S_ITEM_CALL(self, set_int)(self, name, i, error);
	S_UNLOCK_ITEM(self);

	S_CHK_ERR(error, S_CONTERR,
			  "SItemSetInt",
			  "Call to class method \"set_int\" failed");
}


S_API void SItemSetFloat(SItem *self, const char *name, float f, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemSetFloat",
				  "Argument \"self\" is NULL");
		return;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemSetFloat",
				  "Argument \"name\" is NULL");
		return;
	}

	if (!S_ITEM_METH_VALID(self, set_float))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemSetFloat",
				  "Item method \"set_float\" not implemented");
		return;
	}

	S_LOCK_ITEM(self);
	S_ITEM_CALL(self, set_float)(self, name, f, error);
	S_UNLOCK_ITEM(self);

	S_CHK_ERR(error, S_CONTERR,
			  "SItemSetFloat",
			  "Call to class method \"set_float\" failed");
}


S_API void SItemSetString(SItem *self, const char *name,
						  const char *string, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemSetString",
				  "Argument \"self\" is NULL");
		return;
	}

	if (string == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemSetString",
				  "Argument \"string\" is NULL");
		return;
	}

	if (!S_ITEM_METH_VALID(self, set_string))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemSetString",
				  "Item method \"set_string\" not implemented");
		return;
	}

	S_LOCK_ITEM(self);
	S_ITEM_CALL(self, set_string)(self, name, string, error);
	S_UNLOCK_ITEM(self);

	S_CHK_ERR(error, S_CONTERR,
			  "SItemSetString",
			  "Call to class method \"set_string\" failed");
}


S_API const SRelation *SItemRelation(const SItem *self, s_erc *error)
{
	const SRelation *rel;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemRelation",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, relation))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemRelation",
				  "Item method \"relation\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	rel = S_ITEM_CALL(self, relation)(self, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemRelation",
				  "Call to class method \"relation\" failed"))
		return NULL;

	return rel;
}


S_API const SUtterance *SItemUtterance(const SItem *self, s_erc *error)
{
	const SUtterance *utt;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemUtterance",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, utterance))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemUtterance",
				  "Item method \"utterance\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	utt = S_ITEM_CALL(self, utterance)(self, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemUtterance",
				  "Call to class method \"utterance\" failed"))
		return NULL;

	return utt;
}


S_API const SVoice *SItemVoice(const SItem *self, s_erc *error)
{
	const SVoice *voice;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SItemVoice",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_ITEM_METH_VALID(self, voice))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SItemVoice",
				  "Item method \"voice\" not implemented");
		return NULL;
	}

	S_LOCK_ITEM(self);
	voice = S_ITEM_CALL(self, voice)(self, error);
	S_UNLOCK_ITEM(self);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemVoice",
				  "Call to class method \"voice\" failed"))
		return NULL;

	return voice;
}


S_API s_bool SItemEqual(const SItem *a, const SItem *b, s_erc *error)
{
	s_bool is_equal;


	S_CLR_ERR(error);

	if ((a == NULL) || (b == NULL))
		return FALSE;

	/* only compare items from same utterance */
	if (a->relation->utterance != b->relation->utterance)
		return FALSE;

	S_LOCK_ITEM(a);
	is_equal = S_OBJECT_CALL(S_OBJECT(a), compare)(S_OBJECT(a), S_OBJECT(b), error);
	S_UNLOCK_ITEM(a);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SItemEqual",
				  "Call to SObjectCompare"))
		return FALSE;

	return is_equal;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_item_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&ItemClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_item_class_add",
			  "Failed to add SItemClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/*
 * All internal ids are found by getting the next id number from
 * the utterance and prefixing a "_" to show that this is internally
 * generated.
 */
static void assign_id(SItem *self, s_erc *error)
{
	sint32 id;
	const SUtterance *u;
	s_bool id_present;


	S_CLR_ERR(error);

	/* return if an id is already present. */
	id_present = SMapObjectPresent(SItmContentFeatures(self->content,
													   error), "_id", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "assign_id",
				  "Failed to see if id is present"))
		return;

	if (id_present == TRUE)
		return;

	/* to avoid mutex locking use call macro, also cast away const
	 * so that we can lock the utt's mutex
	 */
	u = S_ITEM_CALL(self, utterance)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "assign_id",
				  "Call to method \"utterance\" failed"))
		return;

	/* get id from utt */
	if (u != NULL)
	{
		id = SUtteranceGetNextId(u, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "assign_id",
					  "Call to \"SUtteranceGetNextId\" failed"))
			return;

		S_ITEM_CALL(self, set_int)(self, "_id", id, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "assign_id",
					  "Call to method \"set_int\" failed"))
			return;
	}
}


/*
 * Remove this item's reference from it's content, and delete content
 * if no one else is referencing it
 */
static void item_unref_content(SItem *item, s_erc *error)
{
	size_t size;
	s_erc local_err;


	S_CLR_ERR(&local_err);
	S_CLR_ERR(error);

	if ((item->relation != NULL) && (item->content != NULL))
	{
		SItmContentRemove(item->content,
						  item->relation->name,
						  error);
		S_CHK_ERR(error, S_CONTERR,
				  "item_unref_content",
				  "Call to \"SItmContentRemove\" failed");
		local_err = *error;
	}
	else
		return;

	size = SItmContentNumRelations(item->content, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "item_unref_content",
				  "Call to \"SItmContentNumRelations\" failed"))
		return;

	if ((local_err != S_SUCCESS) && (*error == S_SUCCESS))
		*error = local_err;

	if (size == 0)
		S_FORCE_DELETE(item->content, "item_unref_content", error);
}



/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitItem(void *obj, s_erc *error)
{
	SItem *self = obj;


	S_CLR_ERR(error);
	self->relation = NULL;
	self->content = NULL;
	self->next = NULL;
	self->prev = NULL;
	self->up = NULL;
	self->down = NULL;

	/* so that it will only be deleted with S_FORCE_DELETE */
	SObjectIncRef(obj);
}


static void DestroyItem(void *obj, s_erc *error)
{
	SItem *self = obj;
	SItem *ds;
	SItem *nds;
	s_erc local_err;


	S_CLR_ERR(error);

	if (self->next != NULL)
	{
		self->next->prev = self->prev;
		self->next->up = self->up;
	}

	if (self->prev != NULL)
		self->prev->next = self->next;

	if (self->up != NULL)
		self->up->down = self->next;

	if (self->relation != NULL)
	{
		if (self->relation->head == self)
			self->relation->head = self->next;
		if (self->relation->tail == self)
			self->relation->tail = self->prev;
	}

	/* Delete all the daughters of item */
	for (ds = self->down; ds; ds = nds)
	{
		nds = ds->next;
		S_FORCE_DELETE(ds, "DestroyItem", error);
	}

	/*
	 * Remove this item's reference from it's content, and delete content
	 * if no one else is referencing it
	 */
	item_unref_content(self, &local_err);
	S_CHK_ERR(&local_err, S_CONTERR,
			  "DestroyItem",
			  "Call to \"item_unref_content\" failed");

	if ((local_err != S_SUCCESS) && (*error == S_SUCCESS))
		*error = local_err;
}


static SItem *ItemAs(const SItem *self, const char *relation, s_erc *error)
{
	SItem *itemAs;


	S_CLR_ERR(error);
	itemAs = (SItem*)SItmContentGet(self->content, relation, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ItemAs",
				  "Call to SItmContentGetItemRelation failed"))
		return NULL;

	return itemAs;
}


static s_bool ItemIn(const SItem *self, const char *relation, s_erc *error)
{
	s_bool item_in;

	S_CLR_ERR(error);
	item_in = SItmContentInRelation(self->content, relation, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ItemIn",
				  "Call to SItmContentItemInRelation failed"))
		return FALSE;

	return item_in;
}


static SItem *ItemNext(const SItem *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
		return NULL;

	return self->next;
}


static SItem *ItemPrev(const SItem *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
		return NULL;

	return self->prev;
}


static SItem *ItemAppend(SItem *self, const SItem *toShare, s_erc *error)
{
	SItem *rni;


	S_CLR_ERR(error);

	if ((toShare != NULL) && (toShare->relation == self->relation))
	{
		S_CTX_ERR(error, S_FAILURE,
				  "ItemAppend",
				  "\"toShare\" item already in relation");
		return NULL;
	}

	rni = (SItem*)S_NEW("SItem", error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "ItemAppend",
				  "Failed to create new item"))
		return NULL;

	_SItemInit_no_lock(&rni, self->relation, toShare, error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "ItemAppend",
				  "Failed to initialize new item"))
		return NULL;

	rni->next = self->next;

	if (self->next != NULL)
		self->next->prev = rni;

	rni->prev = self;
	self->next = rni;

	if (self->relation->tail == self)
		self->relation->tail = rni;

	return rni;
}


static SItem *ItemPrepend(SItem *self, const SItem *toShare, s_erc *error)
{
	SItem *rni;


	S_CLR_ERR(error);

	if ((toShare != NULL) && (toShare->relation == self->relation))
	{
		S_CTX_ERR(error, S_FAILURE,
				  "ItemPrepend",
				  "\"toShare\" item already in relation");
		return NULL;
	}

	rni = (SItem*)S_NEW("SItem", error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "ItemPrepend",
				  "Failed to create new item"))
		return NULL;

	_SItemInit_no_lock(&rni, self->relation, toShare, error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "ItemPrepend",
				  "Failed to initialize new item"))
		return NULL;

	rni->prev = self->prev;

	if (self->prev != NULL)
		self->prev->next = rni;

	rni->next = self;
	self->prev = rni;

	if (self->up)  /* in a tree */
	{
		self->up->down = rni;
		rni->up = self->up;
		self->up = NULL;
	}

	if (self->relation->head == self)
		self->relation->head = rni;

	return rni;
}


static SItem *ItemParent(const SItem *self, s_erc *error)
{
	const SItem *n;


	S_CLR_ERR(error);

	for (n = self; ItemPrev(n, error) != NULL; n = ItemPrev(n, error))
		/* NOP */;

	if (S_CHK_ERR(error, S_CONTERR,
				  "ItemParent",
				  "Call to ItemPrev failed"))
		return NULL;

	if (n == NULL)
		return NULL;

	return n->up;
}


static SItem *ItemDaughter(const SItem *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
		return NULL;

	return self->down;
}


static SItem *ItemNthDaughter(const SItem *self, uint32 n, s_erc *error)
{
	SItem *itr;
	uint32 cnt;


	S_CLR_ERR(error);

	for (cnt = 0, itr = ItemDaughter(self, error);
	     (itr != NULL) && (cnt < n);
	     itr = ItemNext(itr, error), cnt++)
		/* void */;

	if (S_CHK_ERR(error, S_CONTERR,
				  "ItemNthDaughter",
				  "Error while searching for nth daughter"))
		return NULL;

	return itr;
}


static SItem *ItemLastDaughter(const SItem *self, s_erc *error)
{
	SItem *itr;


	S_CLR_ERR(error);

	for (itr = ItemDaughter(self, error);
	     ItemNext(itr, error) != NULL;
	     itr = ItemNext(itr, error))
		/* void */;

	if (S_CHK_ERR(error, S_CONTERR,
				  "ItemLastDaughter",
				  "Error while searching for last daughter"))
		return NULL;

	return itr;
}


static SItem *ItemAddDaughter(SItem *self, const SItem *toShare, s_erc *error)
{
	SItem *itr;
	SItem *rnd;


	S_CLR_ERR(error);

	itr = ItemLastDaughter(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ItemAddDaughter",
				  "Call to ItemLastDaughter failed"))
		return NULL;

	if (itr != NULL)
	{
		rnd = ItemAppend(itr, toShare, error);
		S_CHK_ERR(error, S_CONTERR,
				  "ItemAddDaughter",
				  "Call to ItemAppend failed");

	}
	else
	{       /* first new daughter */

		if ((toShare != NULL) && (toShare->relation == self->relation))
		{
			/* got to delete it first as nd can't be in a relation twice */
			S_CTX_ERR(error, S_FAILURE,
					  "ItemAddDaughter",
					  "daughter item already in relation");
			return NULL;
		}

		rnd = (SItem*)S_NEW("SItem", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ItemAddDaughter",
					  "Failed to create new daugther item"))
			return NULL;

		_SItemInit_no_lock(&rnd, self->relation, toShare, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ItemAddDaughter",
					  "Failed to initialize new daugther item"))
			return NULL;

		rnd->up = self;
		self->down = rnd;
	}

	return rnd;
}


static const char *ItemGetName(const SItem *self, s_erc *error)
{
	S_CLR_ERR(error);
	return SMapGetString(SItmContentFeatures(self->content,
											 error), "name", error);
}


static void ItemSetName(SItem *self, const char *name, s_erc *error)
{
	S_CLR_ERR(error);
	SMapSetString((SMap*)SItmContentFeatures(self->content,
											 error), "name", name, error);
}


static s_bool ItemFeatPresent(const SItem *self, const char *name, s_erc *error)
{
	S_CLR_ERR(error);
	return SMapObjectPresent(SItmContentFeatures(self->content, error),
							 name, error);
}


static void ItemDelFeature(SItem *self, const char *name, s_erc *error)
{
	S_CLR_ERR(error);
	SMapObjectDelete((SMap*)SItmContentFeatures(self->content, error),
					 name, error);
}


static SList *ItemFeatKeys(const SItem *self, s_erc *error)
{
	const SMap *feats;
	SList *featKeys;


	S_CLR_ERR(error);
	feats = SItmContentFeatures(self->content, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ItemFeatKeys",
				  "Call to \"SItmContentFeatures\" failed"))
		return NULL;

	featKeys = SMapGetKeys(feats, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ItemFeatKeys",
				  "Call to \"SMapGetKeys\" failed"))
		return NULL;

	return featKeys;
}


static const SObject *ItemGetObject(const SItem *self, const char *name, s_erc *error)
{
	S_CLR_ERR(error);
	return SMapGetObject(SItmContentFeatures(self->content, error),
						 name, error);
}


static sint32 ItemGetInt(const SItem *self, const char *name, s_erc *error)
{
	S_CLR_ERR(error);
	return SMapGetInt(SItmContentFeatures(self->content, error),
					  name, error);
}


static float ItemGetFloat(const SItem *self, const char *name, s_erc *error)
{
	S_CLR_ERR(error);
	return SMapGetFloat(SItmContentFeatures(self->content, error),
						name, error);
}

static const char *ItemGetString(const SItem *self, const char *name, s_erc *error)
{
	S_CLR_ERR(error);
	return SMapGetString(SItmContentFeatures(self->content, error),
						 name, error);
}


static void ItemSetObject(SItem *self, const char *name,
						  const SObject *object, s_erc *error)
{
	S_CLR_ERR(error);
	SMapSetObject((SMap*)SItmContentFeatures(self->content, error),
				  name, object, error);
}


static void ItemSetInt(SItem *self, const char *name,
					   sint32 i, s_erc *error)
{
	S_CLR_ERR(error);
	SMapSetInt((SMap*)SItmContentFeatures(self->content, error),
			   name, i, error);
}


static void ItemSetFloat(SItem *self, const char *name,
						 float f, s_erc *error)
{
	S_CLR_ERR(error);
	SMapSetFloat((SMap*)SItmContentFeatures(self->content, error),
				 name, f, error);
}


static void ItemSetString(SItem *self, const char *name,
						  const char *string, s_erc *error)
{
	S_CLR_ERR(error);
	SMapSetString((SMap*)SItmContentFeatures(self->content, error),
				  name, string, error);
}


static const SRelation *ItemRelation(const SItem *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->relation;
}


static const SUtterance *ItemUtterance(const SItem *self, s_erc *error)
{
	S_CLR_ERR(error);
	return (self->relation? self->relation->utterance: NULL);
}


static const SVoice *ItemVoice(const SItem *self, s_erc *error)
{
	const SUtterance *utt;


	S_CLR_ERR(error);
	utt = ItemUtterance(self, error);

	if (utt == NULL)
		return NULL;

	return utt->voice;
}

static s_bool ObjectCompare(const SObject *a, const SObject *b, s_erc *error)
{
	S_CLR_ERR(error);

	if ((S_ITEM(a) == S_ITEM(b)) ||
	    ((a != NULL) &&
	     (b != NULL) &&
	     (S_ITEM(a)->content == S_ITEM(b)->content)))
		return TRUE;

	return FALSE;
}


/************************************************************************************/
/*                                                                                  */
/* SItem class initialization                                                       */
/*                                                                                  */
/************************************************************************************/

static SItemClass ItemClass =
{
	/* SObjectClass */
	{
		"SItem",
		sizeof(SItem),
		{ 0, 1},
		InitItem,          /* init    */
		DestroyItem,       /* destroy */
		NULL,              /* dispose */
		ObjectCompare,     /* compare */
		NULL,              /* print   */
		NULL,              /* copy    */
	},
	/* SItemClass */
	ItemAs,                    /* item_as         */
	ItemIn,                    /* item_in         */
	ItemNext,                  /* item_next       */
	ItemPrev,                  /* item_prev       */
	ItemAppend,                /* item_append     */
	ItemPrepend,               /* item_prepend    */
	ItemParent,                /* item_parent     */
	ItemDaughter,              /* item_daughter   */
	ItemNthDaughter,           /* nth_daughter    */
	ItemLastDaughter,          /* last_daughter   */
	ItemAddDaughter,           /* add_daughter    */
	ItemGetName,               /* get_name        */
	ItemSetName,               /* set_name        */
	ItemFeatPresent,           /* feat_present    */
	ItemDelFeature,            /* feat_delete     */
	ItemFeatKeys,              /* feat_keys       */
	ItemGetObject,             /* get_object      */
	ItemGetInt,                /* get_int         */
	ItemGetFloat,              /* get_float       */
	ItemGetString,             /* get_string      */
	ItemSetObject,             /* set_object      */
	ItemSetInt,                /* set_int         */
	ItemSetFloat,              /* set_float       */
	ItemSetString,             /* set_string      */
	ItemRelation,              /* relation        */
	ItemUtterance,             /* utterance       */
	ItemVoice                  /* voice           */
};


