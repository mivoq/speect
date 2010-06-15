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
/* DATE    : 8 October 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* List implementation of SMap container.                                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "base/strings/strings.h"
#include "base/objsystem/class.h"
#include "containers/list/list_list.h"
#include "containers/map/map_list_iterator.h"
#include "containers/map/map_list.h"


/************************************************************************************/
/*                                                                                  */
/*  Data types                                                                      */
/*                                                                                  */
/************************************************************************************/

/**
 * Definition of key-value pair.
 */
typedef struct s_kvp_s
{
	const char    *key;    /*!< key   */
	const SObject *val;    /*!< value */
} s_kvp;


/************************************************************************************/
/*                                                                                  */
/*  Static variables                                                                */
/*                                                                                  */
/************************************************************************************/

static SMapListClass MapListClass;  /* MapList class declaration. */


/************************************************************************************/
/*                                                                                  */
/*  Static function prototypes                                                      */
/*                                                                                  */
/************************************************************************************/

/*
 * Helper function for SMapList.
 * Compare a key (char) and a key-value pair.
 */
static s_bool s_map_list_compare_keys(const void *le1, const void *le2, s_erc *error);


/*
 * Delete function for SMapList.
 * Used to delete a SMapList key-value pair.
 */
static void s_map_list_del_function(void *le, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_map_list_class_add(s_erc *error)
{
	S_CLR_ERR(error);

	s_class_add(&MapListClass, error);

	S_CHK_ERR(error, S_CONTERR,
		  "_s_map_list_class_add",
		  "Failed to add SMapListClass");
}


/************************************************************************************/
/*                                                                                  */
/*  Static function implementations                                                 */
/*                                                                                  */
/************************************************************************************/

/*
 * Helper function for SMapList.
 * Compare a key (char) and a key-value pair.
 */
static s_bool s_map_list_compare_keys(const void *le1, const void *le2, s_erc *error)
{
	const char *name;
	const s_kvp *tmp;
	int rv;


	S_CLR_ERR(error);
	name = (const char *)le1;
	tmp = (const s_kvp *)le2;

	rv = s_strcmp(name, tmp->key, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_map_list_compare_keys",
		      "Call to s_strcmp failed") || (rv != 0))
		return FALSE;

	return TRUE;
}


/*
 * Delete function for SMapList.
 * Used to delete a SMapList key-value pair.
 */
static void s_map_list_del_function(void *le, s_erc *error)
{
	char *key;
	SObject *val;
	s_kvp *tmp;

	S_CLR_ERR(error);
	tmp = (s_kvp*)le;
	key = (char*)tmp->key;
	val = (SObject*)tmp->val;
	S_FREE(key);
	S_FREE(tmp);
	S_DELETE(val, "s_map_list_del_function", error);
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitMapList(void *obj, s_erc *error)
{
	SMapList *self = obj;


	S_CLR_ERR(error);
	self->list = s_list_new(&s_map_list_compare_keys,
							&s_map_list_del_function, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "InitMapList",
				  "Failed to create 's_list' object"))
		self->list = NULL;
}


static void DestroyMapList(void *obj, s_erc *error)
{
	SMapList *self = obj;


	S_CLR_ERR(error);

	if (self->list == NULL)
		return;

	s_list_delete(self->list, error);
	S_CHK_ERR(error, S_CONTERR,
			  "DestroyMapList",
			  "Failed to delete list");
}


static void DisposeMapList(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static const SObject *MapListValGet(const SMap *self, const char *key, s_erc *error)
{
	const SMapList *mapList = (const SMapList*)self;
	s_list_element *e;
	const s_kvp *tmp;


	S_CLR_ERR(error);
	if (mapList->list == NULL)
		return NULL;

	e = s_list_find(mapList->list, (void*)key, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListValGet",
		      "Call to s_list_find failed"))
		return NULL;

	if (e == NULL)
		return NULL;

	tmp = s_list_element_get(e, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListValGet",
		      "Call to s_list_element_get failed"))
		return NULL;

	return tmp->val;
}


static void MapListValSet(SMap *self, const char *key, const SObject *val, s_erc *error)
{
	SMapList *mapList = (SMapList*)self;
	s_list_element *e;
	s_kvp *tmp;
	char *tmp_key;
	SObject *old_val;


	S_CLR_ERR(error);
	if (mapList->list == NULL)
		return;

	e = s_list_find(mapList->list, (void*)key, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListValSet",
		      "Call to s_list_find failed"))
		return;

	if (e == NULL)
	{
		/* new key-value pair */
		tmp = S_MALLOC(s_kvp, 1);
		if (tmp == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
				  "MapListValSet",
				  "Failed to allocate memory to key-value pair");
			return;
		}

		tmp->key = s_strdup(key, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "MapListValSet",
			      "Failed to duplicate key"))
		{
			S_FREE(tmp);
			return;
		}

		/* increment reference count */
		SObjectIncRef((SObject*)val);
		tmp->val = val;

		s_list_append(mapList->list, (void*)tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "MapListValSet",
			      "Failed to add key-value pair to list"))
		{
			tmp_key = (char*)tmp->key;
			S_FREE(tmp_key);
			S_FREE(tmp);

			/* remove reference */
			SObjectDecRef((SObject*)val);
			return;
		}
	}
	else
	{
		/* existing pair, change it */
		tmp = (s_kvp*)s_list_element_get(e, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "MapListValSet",
			      "Call to s_list_element_get failed"))
			return;


		old_val = (SObject *)tmp->val;

		/* increment reference count */
		SObjectIncRef((SObject*)val);
		tmp->val = val;

		S_DELETE(old_val, "MapListValSet", error);
	}
}


static void MapListValDelete(SMap *self, const char *key, s_erc *error)
{
	SMapList *mapList = (SMapList*)self;
	s_list_element *e = NULL;


	S_CLR_ERR(error);
	if (mapList->list == NULL)
		return;

	e = s_list_find(mapList->list, (void*)key, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListValDelete",
		      "Call to s_list_find failed"))
		return;

	if (e == NULL)
		return;

	s_list_element_delete(e, error);
	S_CHK_ERR(error, S_CONTERR,
		  "MapListValDelete",
		  "Call to s_list_element_delete failed");
}


static SObject *MapListValUnlink(SMap *self, const char *key, s_erc *error)
{
	SMapList *mapList = (SMapList*)self;
	s_list_element *e;
	s_kvp *tmp;
	char *ckey;
	SObject *val;


	S_CLR_ERR(error);
	if (mapList->list == NULL)
		return NULL;

	e = s_list_find(mapList->list, (void*)key, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListValUnlink",
		      "Call to s_list_find failed"))
		return NULL;

	if (e == NULL)
		return NULL;

	tmp = s_list_element_unlink(e, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListValUnlink",
		      "Call to s_list_element_unlink failed"))
		return NULL;

	ckey = (char*)tmp->key;
	val = (SObject*)tmp->val;
	S_FREE(ckey);
	S_FREE(tmp);

	/* remove reference */
	SObjectDecRef(val);

	return val;
}


static s_bool MapListValPresent(const SMap *self, const char *key, s_erc *error)
{
	const SMapList *mapList = (const SMapList*)self;
	s_list_element *e = NULL;


	S_CLR_ERR(error);
	if (mapList->list == NULL)
		return FALSE;

	e = s_list_find(mapList->list, (void*)key, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListValUnlink",
		      "Call to s_list_find failed"))
		return FALSE;

	if (e == NULL)
		return FALSE;

	return TRUE;
}


static SList *MapListValKeys(const SMap *self, s_erc *error)
{
	const SMapList *mapList = (const SMapList*)self;
	s_list_element *itr = NULL;
	SList *keys;
	s_kvp *tmp;

	S_CLR_ERR(error);
	keys = S_LIST(S_NEW("SListList", error));
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListValKeys",
		      "Failed to create new SList container for keys"))
		return NULL;

	for (itr = s_list_first(mapList->list, error);
	     itr != NULL;
	     itr = s_list_element_next(itr, error))
	{
		if (S_CHK_ERR(error, S_FAILURE,
			      "MapListValKeys",
			      "Failed to get iterator loop elements"))
		{
			S_DELETE(keys, "MapListValKeys", error);
			return NULL;
		}

		tmp = (s_kvp*)s_list_element_get(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "MapListValKeys",
			      "Call to \"s_list_element_get\" failed"))
		{
			S_DELETE(keys, "MapListValKeys", error);
			return NULL;
		}

		SListPush(keys, SObjectSetString(tmp->key, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "MapListValKeys",
			      "Call to \"SListPush\" failed"))
		{
			S_DELETE(keys, "MapListValKeys", error);
			return NULL;
		}
	}

	return keys;
}


static size_t MapListSize(const SMap *self, s_erc *error)
{
	const SMapList *mapList = (const SMapList*)self;
	size_t size;


	S_CLR_ERR(error);
	if (mapList->list == NULL)
		return 0;

	size = s_list_size(mapList->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListSize",
		      "Call to s_list_size failed"))
		return 0;

	return size;
}


static SMap *MapListCopy(SMap *dst, const SMap *src, s_erc *error)
{
	const SMapList *mapSrc = (const SMapList*)src;
	SMapList *mapDst;
	s_kvp *tmp;
	s_list_element *itr = NULL;
	s_bool made_new = FALSE;


	S_CLR_ERR(error);
	if (mapSrc->list == NULL)
		return NULL;

	if (dst == NULL)
	{
		made_new = TRUE;
		mapDst = (SMapList*)S_NEW("SMapList", error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "MapListCopy",
			      "Failed to create new list map"))
			return NULL;

		dst = S_MAP(mapDst);
	}


	for (itr = s_list_first(mapSrc->list, error);
	     itr != NULL;
	     itr = s_list_element_next(itr, error))
	{
		if (S_CHK_ERR(error, S_FAILURE,
			      "MapListCopy",
			      "Failed to get iterator loop elements"))
		{
			if (made_new == TRUE)
				S_DELETE(dst, "MapListCopy", error);
			return NULL;
		}

		tmp = (s_kvp*)s_list_element_get(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "MapListCopy",
			      "Call to \"s_list_element_get\" failed"))
		{
			if (made_new == TRUE)
				S_DELETE(dst, "MapListCopy", error);
			return NULL;
		}

		S_MAP_CALL(dst, val_set)(dst, tmp->key, tmp->val, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "MapListCopy",
			      "Failed to set value in container"))
		{
			if (made_new == TRUE)
				S_DELETE(dst, "MapListCopy", error);
			return NULL;
		}
	}

	return dst;
}


static SIterator *MapListIterator(const SMap *self, s_erc *error)
{
	SMapList *mapList = (SMapList*)self;
	SIterator *itr;


	S_CLR_ERR(error);

	itr = S_ITERATOR(S_NEW("SMapListIterator", error));
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListIterator",
		      "Failed to create new iterator"))
		return NULL;

	SMapListIteratorInit(&itr, mapList, error);
	S_CHK_ERR(error, S_CONTERR,
		      "MapHashTableIterator",
		      "Failed to initialize iterator");

	return itr;
}


static const char *MapListKey(const SIterator *iterator, s_erc *error)
{
	const SMapListIterator *self;
	s_kvp *tmp;


	S_CLR_ERR(error);

	/* must cast this one */
	self = S_CAST(iterator, SMapListIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListKey",
		      "Failed to cast SIterator to SMapListIterator"))
		return NULL;


	tmp = (s_kvp*)s_list_element_get((s_list_element*)self->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListKey",
		      "Call to s_list_element_get failed"))
		return NULL;

	if (tmp == NULL)
		return NULL;

	return tmp->key;
}


static const SObject *MapListVal(const SIterator *iterator, s_erc *error)
{
	const SMapListIterator *self;
	s_kvp *tmp;


	S_CLR_ERR(error);

	/* must cast this one */
	self = S_CAST(iterator, SMapListIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListVal",
		      "Failed to cast SIterator to SMapListIterator"))
		return NULL;


	tmp = (s_kvp*)s_list_element_get((s_list_element*)self->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListVal",
		      "Call to s_list_element_get failed"))
		return NULL;

	if (tmp == NULL)
		return NULL;

	return tmp->val;
}


static SObject *MapListUnlink(SIterator *iterator, s_erc *error)
{
	SMapListIterator *self;
	s_kvp *tmp;
	char *key;
	SObject *val;

	S_CLR_ERR(error);

	/* must cast this one */
	self = S_CAST(iterator, SMapListIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListUnlink",
		      "Failed to cast SIterator to SMapListIterator"))
		return NULL;

	if (self->c_itr == NULL)
		return NULL;

	tmp = s_list_element_unlink((s_list_element*)self->c_itr, error);
	self->c_itr = NULL;

	if (S_CHK_ERR(error, S_CONTERR,
		      "MapListUnlink",
		      "Call to s_list_element_unlink failed"))
		return NULL;

	val = (SObject*)tmp->val;

	/* remove reference to this container */
	SObjectDecRef(val);

	key = (char*)tmp->key;
	S_FREE(key);
	S_FREE(tmp);

	return val;
}


/************************************************************************************/
/*                                                                                  */
/* SMapList class initialization                                                    */
/*                                                                                  */
/************************************************************************************/

static SMapListClass MapListClass =
{
	{
		/* SObjectClass */
		{
			"SContainer:SMap:SMapList",
			sizeof(SMapList),
			{ 0, 1},
			InitMapList,    /* init    */
			DestroyMapList, /* destroy */
			DisposeMapList, /* dispose */
			NULL,           /* compare */
			NULL,           /* print   */
			NULL,           /* copy    */
		},
		/* SContainerClass */
		/* No methods */
	},
	/* SMapClass */
	MapListValGet,            /* val_get      */
	MapListValSet,            /* val_set      */
	MapListValDelete,         /* val_delete   */
	MapListValUnlink,         /* val_unlink   */
	MapListValPresent,        /* val_present  */
	MapListValKeys,           /* val_keys     */
	MapListSize,              /* size         */
	MapListCopy,              /* copy         */
	MapListIterator,          /* iterator     */
	MapListKey,               /* key          */
	MapListVal,               /* value        */
	MapListUnlink             /* unlink       */
};


