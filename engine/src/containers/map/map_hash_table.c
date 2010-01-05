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

S_API void SMapHashTableInit(SMap **self, size_t size, s_erc *error)
{
	SMapHashTable *mapHashTable;


	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapHashTableInit",
				  "Argument \"self\" is NULL");
		return;
	}

	mapHashTable = S_CAST(*self, SMapHashTable, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapHashTableInit",
				  "Argument \"self\" is not of SMapHashTable type"))
	{
		S_DELETE(*self, "SMapHashTableInit", error);
		*self = NULL;
		return;
	}


	mapHashTable->table = s_hash_table_new(s_hash_table_delete_kv_pair,
										   ceil(s_log2(size)), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapHashTableInit",
				  "Failed to create hash table for SMapHashTable"))
	{
		S_DELETE(*self, "SMapHashTableInit", error);
		*self = NULL;
	}
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_map_hash_table_class_add(s_erc *error)
{
	S_CLR_ERR(error);

	s_class_add(&MapHashTableClass, error);

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
	self->table = NULL;
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
	s_hash_element *hte;
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

	val = (const SObject *)s_hash_element_get_data(hte, error);
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

	hte = s_hash_table_find(hashTable->table, (void*)key,
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

	hte = s_hash_table_find(hashTable->table, (void*)key,
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

	hte = s_hash_table_find(hashTable->table, (void*)key,
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

	object = s_hash_element_get_data(hte, error);
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
	s_hash_element *hte;


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
	s_hash_element *hte;
	SList *keys;
	const char *tmp;

	S_CLR_ERR(error);
	keys = S_LIST(S_NEW("SListList", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableValKeys",
				  "Failed to create new SList container for keys"))
		return NULL;

	SListListInit(&keys, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableValKeys",
				  "Call to \"SListListInit\" failed"))
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
		tmp = (const char*)s_hash_element_key(hte, error);
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
	s_hash_element *hte;
	s_bool made_new = FALSE;
	const char *key;
	SObject *val;



	S_CLR_ERR(error);

	if (mapSrc->table == NULL)
		return NULL;

	if (dst == NULL)
	{
		made_new = TRUE;
		mapDst = (SMapHashTable*)S_NEW("SMapHashTable", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "MapHashTableCopy",
					  "Failed to create new hash table map"))
			return NULL;

		dst = S_MAP(mapDst);
		SMapHashTableInit(&dst, MapHashTableSize(src, error), error);
		if (S_CHK_ERR(error, S_FAILURE,
					  "MapHashTableCopy",
					  "Failed to initialize new hash table map"))
			return NULL;
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


static SIterator *MapHashTableIterator(const SMap *self, s_erc *error)
{
	SMapHashTable *hashTable = (SMapHashTable*)self;
	SIterator *itr;


	S_CLR_ERR(error);

	itr = S_ITERATOR(S_NEW("SMapHashTableIterator", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableIterator",
				  "Failed to create new iterator"))
		return NULL;

	SMapHashTableIteratorInit(&itr, hashTable, error);
	S_CHK_ERR(error, S_CONTERR,
			  "MapHashTableIterator",
			  "Failed to initialize iterator");

	return itr;
}


static const char *MapHashTableKey(const SIterator *iterator, s_erc *error)
{
	const SMapHashTableIterator *self;
	const char *key;


	S_CLR_ERR(error);

	/* must cast this one */
	self = S_CAST(iterator, SMapHashTableIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableKey",
				  "Failed to cast SIterator to SMapHashTableIterator"))
		return NULL;

	key = (const char *)s_hash_element_key((s_hash_element*)self->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableKey",
				  "Failed to get key from hash table element"))
		return NULL;

	return key;
}


static const SObject *MapHashTableVal(const SIterator *iterator, s_erc *error)
{
	const SMapHashTableIterator *self;
	const SObject *val;


	S_CLR_ERR(error);

	/* must cast this one */
	self = S_CAST(iterator, SMapHashTableIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableVal",
				  "Failed to cast SIterator to SMapHashTableIterator"))
		return NULL;

	val = (const SObject*)s_hash_element_get_data((s_hash_element*)self->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableVal",
				  "Failed to get value from hash table element"))
		return NULL;

	return val;
}


static SObject *MapHashTableUnlink(SIterator *iterator, s_erc *error)
{
	SMapHashTableIterator *self;
	s_hash_element *hte;
	char *key;
	SObject *val;
	s_erc local_err = S_SUCCESS;


	S_CLR_ERR(error);

	/* must cast this one */
	self = S_CAST(iterator, SMapHashTableIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableUnlink",
				  "Failed to cast SIterator to SMapHashTableIterator"))
		return NULL;

	hte = (s_hash_element*)self->c_itr;

	key = (char*)s_hash_element_key(hte, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableUnlink",
				  "Call to \"s_hash_element_key\" failed"))
		local_err = *error;

	if (key != NULL)
		S_FREE(key);

	val = (SObject*)s_hash_element_get_data(hte, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableUnlink",
				  "Call to \"s_hash_element_get_data\" failed"))
		local_err = *error;

	/* remove reference to this container */
	if (val != NULL)
		SObjectDecRef(val);

	s_hash_element_unlink(hte, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapHashTableUnlink",
				  "Call to \"s_hash_element_unlink\" failed"))
		local_err = *error;

	if ((local_err != S_SUCCESS) && (*error == S_SUCCESS))
		*error = local_err;

	return val;
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
		/* No methods */
	},
	/* SMapClass */
	MapHashTableValGet,            /* val_get      */
	MapHashTableValSet,            /* val_set      */
	MapHashTableValDelete,         /* val_delete   */
	MapHashTableValUnlink,         /* val_unlink   */
	MapHashTableValPresent,        /* val_present  */
	MapHashTableValKeys,           /* val_keys     */
	MapHashTableSize,              /* size         */
	MapHashTableCopy,              /* copy         */
	MapHashTableIterator,          /* iterator     */
	MapHashTableKey,               /* key          */
	MapHashTableVal,               /* value        */
	MapHashTableUnlink             /* unlink       */
};
