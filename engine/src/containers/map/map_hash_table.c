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
/* DATE    : 8 October 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Hash table implementation of SMap container.                                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/math.h"
#include "base/utils/alloc.h"
#include "base/strings/strings.h"
#include "containers/list/list_list.h"
#include "containers/map/map_hash_table_iterator.h"
#include "containers/map/map_hash_table.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

/* initial size of SMapHashTable when created with S_NEW */
#define S_MAP_HASH_TABLE_INIT_SIZE 128


/************************************************************************************/
/*                                                                                  */
/*  Static variables                                                                */
/*                                                                                  */
/************************************************************************************/

static SMapHashTableClass MapHashTableClass; /* MapHashTable class declaration. */


/************************************************************************************/
/*                                                                                  */
/*  Static function prototypes                                                      */
/*                                                                                  */
/************************************************************************************/

void s_hash_table_delete_kv_pair(void *key, void *data, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/*  Function implementations                                                        */
/*                                                                                  */
/************************************************************************************/


S_API void SMapHashTableResize(SMapHashTable *self, sint32 size, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapHashTableResize",
				  "Argument \"self\" is NULL");
		return;
	}

	if (size == -1)
	{
		s_hash_table_resize(self->table, -1, error);
	}
	else if (size > 0)
	{
		s_hash_table_resize(self->table,  (sint32)ceil(s_log2(size)), error);
	}
	else
	{
		return;
	}

	S_CHK_ERR(error, S_CONTERR,
			  "SMapHashTableResize",
			  "Call to \"s_hash_table_resize\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_map_hash_table_class_add(s_erc *error)
{
	S_CLR_ERR(error);

	s_class_add(S_OBJECTCLASS(&MapHashTableClass), error);

	S_CHK_ERR(error, S_CONTERR,
			  "_s_map_hash_table_class_add",
			  "Failed to add SMapHashTableClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

void s_hash_table_delete_kv_pair(void *key, void *data, s_erc *error)
{
	char  *skey;
	SObject *val;


	S_CLR_ERR(error);

	skey = (char *)key;
	val = (SObject*)data;
	S_FREE(skey);
	S_DELETE(val, "s_hash_table_delete_kv_pair", error);
}



/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitMapHashTable(void *obj, s_erc *error)
{
	SMapHashTable *self = obj;


	S_CLR_ERR(error);
	self->table = s_hash_table_new(s_hash_table_delete_kv_pair,
								   (size_t)ceil(s_log2(S_MAP_HASH_TABLE_INIT_SIZE)),
								   error);
	S_CHK_ERR(error, S_CONTERR,
			  "InitMapHashTable",
			  "Failed to create 's_hash_table' for SMapHashTable");
}


static void DestroyMapHashTable(void *obj, s_erc *error)
{
	SMapHashTable *self = obj;


	S_CLR_ERR(error);

	if (self->table == NULL)
		return;

	s_hash_table_delete(self->table, error);
	S_CHK_ERR(error, S_CONTERR,
			  "DestroyMapHashTable",
			  "Failed to delete hash table");
}


static void DisposeMapHashTable(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static const SObject *MapHashTableValGet(const SMap *self, const char *key,
										 s_erc *error)
{
	const SMapHashTable *hashTable = (const SMapHashTable *)self;
	const s_hash_element *hte;
	const SObject *val;


	S_CLR_ERR(error);

	hte = s_hash_table_find(hashTable->table, (void*)key,
							s_strzsize(key, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableValGet",
				  "Call to \"s_hash_table_find\" failed"))
		return NULL;

	if (hte == NULL)
		return NULL;

	val = s_hash_element_get_data(hte, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableValGet",
				  "Call to \"s_hash_element_get_data\" failed"))
		return NULL;

	return val;
}


static void MapHashTableValSet(SMap *self, const char *key, const SObject *val,
							   s_erc *error)
{
	SMapHashTable *hashTable = (SMapHashTable *)self;
	s_hash_element *hte;
	char *tmp_key;
	SObject *old_val;


	S_CLR_ERR(error);
	if (hashTable->table == NULL)
		return;

	hte = (s_hash_element *)s_hash_table_find(hashTable->table, (void*)key,
											  s_strzsize(key, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableValGet",
				  "Call to \"s_hash_table_find\" failed"))
		return;

	if (hte)
	{
		/* existing pair, change it */
		old_val = (SObject*)s_hash_element_get_data(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "MapHashTableValGet",
					  "Call to \"s_hash_element_get_data\" failed"))
			return;

		S_DELETE(old_val, "MapHashTableValGet", error);

		/* increment reference count */
		SObjectIncRef((SObject *)val);
		s_hash_element_set_data(hte, (void*)val, error);
		S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableValGet",
				  "Call to \"s_hash_element_set_data\" failed");
		return;
	}
	else
	{
		/* new key-value pair */
		tmp_key = s_strdup(key, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "MapHashTableValGet",
					  "Call to \"strdup\" failed"))
			return;

		/* increment reference count */
		SObjectIncRef((SObject *)val);

		s_hash_table_add(hashTable->table, (void*)tmp_key, s_strzsize(tmp_key, error),
						 (void*)val, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "MapHashTableValGet",
					  "Call to \"s_hash_table_add\" failed"))
		{
			S_FREE(tmp_key);
			return;
		}
	}
}


static void MapHashTableValDelete(SMap *self, const char *key, s_erc *error)
{
	SMapHashTable *hashTable = (SMapHashTable *)self;
	s_hash_element *hte;


	S_CLR_ERR(error);

	hte = (s_hash_element *)s_hash_table_find(hashTable->table, (void*)key,
											  s_strzsize(key, error),
											  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableValDelete",
				  "Call to \"s_hash_table_find\" failed"))
		return;

	if (hte == NULL)
		return;

	s_hash_element_delete(hte, error);
	S_CHK_ERR(error, S_CONTERR,
			  "MapHashTableValDelete",
			  "Call to \"s_hash_table_element_delete\" failed");
}


static SObject *MapHashTableValUnlink(SMap *self, const char *key, s_erc *error)
{
	SMapHashTable *hashTable = (SMapHashTable *)self;
	s_hash_element *hte;
	char *tmp_key;
	SObject *object;


	S_CLR_ERR(error);

	hte = (s_hash_element *)s_hash_table_find(hashTable->table, (void*)key,
											  s_strzsize(key, error),
											  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableValUnlink",
				  "Call to \"s_hash_table_find\" failed"))
		return NULL;

	if (hte == NULL)
		return NULL;

	tmp_key = (char*)s_hash_element_key(hte, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableValUnlink",
				  "Call to \"s_hash_element_key\" failed"))
		return NULL;

	if (tmp_key != NULL)
		S_FREE(tmp_key);

	object = (SObject *)s_hash_element_get_data(hte, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableValUnlink",
				  "Call to \"s_hash_element_get_data\" failed"))
		return NULL;

	/* remove reference to this container */
	SObjectDecRef(object);

	s_hash_element_unlink(hte, error);
	S_CHK_ERR(error, S_CONTERR,
			  "MapHashTableValUnlink",
			  "Call to \"s_hash_element_unlink\" failed");

	return object;
}


static s_bool MapHashTableValPresent(const SMap *self, const char *key, s_erc *error)
{
	const SMapHashTable *hashTable = (const SMapHashTable *)self;
	const s_hash_element *hte;


	S_CLR_ERR(error);
	hte = s_hash_table_find(hashTable->table, (void*)key,
							s_strzsize(key, error),
							error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableValPresent",
				  "Call to \"s_hash_table_find\" failed"))
		return FALSE;

	if (hte == NULL)
		return FALSE;

	return TRUE;
}


static SList *MapHashTableValKeys(const SMap *self, s_erc *error)
{
	const SMapHashTable *hashTable = (const SMapHashTable *)self;
	const s_hash_element *hte;
	SList *keys;
	const char *tmp;

	S_CLR_ERR(error);
	keys = S_LIST(S_NEW(SListList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableValKeys",
				  "Failed to create new SList container for keys"))
		return NULL;

	hte = s_hash_table_first(hashTable->table, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableValKeys",
				  "Call to \"s_hash_table_first\" failed"))
	{
		S_DELETE(keys, "MapHashTableValKeys", error);
		return NULL;
	}

	if (hte == NULL)
		return keys;

	do
	{
		tmp = s_hash_element_key(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "MapHashTableValKeys",
					  "Call to \"s_hash_element_key\" failed"))
		{
			S_DELETE(keys, "MapHashTableValKeys", error);
			return NULL;
		}

		SListPush(keys, SObjectSetString(tmp, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "MapHashTableValKeys",
					  "Call to \"MapHashTableValKeys\" failed"))
		{
			S_DELETE(keys, "MapHashTableValKeys", error);
			return NULL;
		}

		hte = s_hash_element_next(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "MapHashTableValKeys",
					  "Call to \"s_hash_element_next\" failed"))
		{
			S_DELETE(keys, "MapHashTableValKeys", error);
			return NULL;
		}
	} while (hte != NULL);

	return keys;
}


/* number of elements, not the table size. */
static size_t MapHashTableSize(const SMap *self, s_erc *error)
{
	const SMapHashTable *hashTable = (const SMapHashTable *)self;
	size_t size;


	S_CLR_ERR(error);

	size = s_hash_table_size(hashTable->table, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableSize",
				  "Call to s_hash_table_size failed"))
		return 0;

	return size;
}


static SMap *MapHashTableCopy(SMap *dst, const SMap *src, s_erc *error)
{
	const SMapHashTable *mapSrc = (const SMapHashTable*)src;
	SMapHashTable *mapDst;
	const s_hash_element *hte;
	s_bool made_new = FALSE;
	const char *key;
	SObject *val;



	S_CLR_ERR(error);

	if (mapSrc->table == NULL)
		return NULL;

	if (dst == NULL)
	{
		made_new = TRUE;
		mapDst = S_NEW(SMapHashTable, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "MapHashTableCopy",
					  "Failed to create new hash table map"))
			return NULL;

		dst = S_MAP(mapDst);
		SMapHashTableResize(mapDst, MapHashTableSize(src, error), error);
		if (S_CHK_ERR(error, S_FAILURE,
					  "MapHashTableCopy",
					  "Failed to resize new hash table map"))
		{
			S_DELETE(mapDst, "MapHashTableCopy", error);
			return NULL;
		}
	}

	hte = s_hash_table_first(mapSrc->table, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableCopy",
				  "Call to \"s_hash_table_first\" failed"))
	{
		if (made_new == TRUE)
			S_DELETE(dst, "MapHashTableCopy", error);
		return dst;
	}

	if (hte == NULL)
		return dst;

	do
	{
		key = (const char*)s_hash_element_key(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "MapHashTableCopy",
					  "Call to \"s_hash_element_key\" failed"))
		{
			if (made_new == TRUE)
				S_DELETE(dst, "MapHashTableCopy", error);
			return dst;
		}

		val = (SObject*)s_hash_element_get_data(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "MapHashTableCopy",
					  "Call to \"s_hash_element_data\" failed"))
		{
			if (made_new == TRUE)
				S_DELETE(dst, "MapHashTableCopy", error);
			return dst;
		}

		S_MAP_CALL(dst, val_set)(dst, key, val, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "MapHashTableCopy",
					  "Call to \"MapHashTableValSet\" failed"))
		{
			if (made_new == TRUE)
				S_DELETE(dst, "MapHashTableCopy", error);
			return dst;
		}

		hte = s_hash_element_next(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "MapHashTableCopy",
					  "Call to \"s_hash_element_next\" failed"))
		{
			if (made_new == TRUE)
				S_DELETE(dst, "MapHashTableCopy", error);
			return dst;
		}
	} while (hte != NULL);

	return dst;
}


static SIterator *MapHashTableIterator(const SContainer *self, s_erc *error)
{
	SMapHashTableIterator *itr;


	S_CLR_ERR(error);

	itr = S_NEW(SMapHashTableIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableIterator",
				  "Failed to create new iterator"))
		return NULL;

	SMapHashTableIteratorInit(&itr, S_MAPHASHTABLE(self), error);
	S_CHK_ERR(error, S_CONTERR,
			  "MapHashTableIterator",
			  "Failed to initialize iterator");

	return S_ITERATOR(itr);
}


/************************************************************************************/
/*                                                                                  */
/* SMapHashTable class initialization                                               */
/*                                                                                  */
/************************************************************************************/

static SMapHashTableClass MapHashTableClass =
{
	{
		/* SObjectClass */
		{
			"SContainer:SMap:SMapHashTable",
			sizeof(SMapHashTable),
			{ 0, 1},
			InitMapHashTable,    /* init    */
			DestroyMapHashTable, /* destroy */
			DisposeMapHashTable, /* dispose */
			NULL,                /* compare */
			NULL,                /* print   */
			NULL,                /* copy    */
		},
		/* SContainerClass */
		MapHashTableIterator,    /* get_iterator */
	},
	/* SMapClass */
	MapHashTableValGet,            /* val_get      */
	MapHashTableValSet,            /* val_set      */
	MapHashTableValDelete,         /* val_delete   */
	MapHashTableValUnlink,         /* val_unlink   */
	MapHashTableValPresent,        /* val_present  */
	MapHashTableValKeys,           /* val_keys     */
	MapHashTableSize,              /* size         */
	MapHashTableCopy               /* copy         */
};
