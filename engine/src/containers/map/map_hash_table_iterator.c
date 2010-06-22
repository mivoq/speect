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
/* Iterator implementation of SMapHashTable container.                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/objsystem/class.h"
#include "containers/map/map_hash_table_iterator.h"


/************************************************************************************/
/*                                                                                  */
/* SMapHashTableIteratorClass definition                                            */
/*                                                                                  */
/************************************************************************************/

/**
 * The SMapHashTableIterator class structure.  Same as SIteratorClass as
 * we are not adding any new methods.
 */
typedef SIteratorClass SMapHashTableIteratorClass;


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given object as an #SMapHashTable object.
 * @param SELF The given object.
 * @return Given object as #SMapHashTable* type.
 * @note This casting is not safety checked.
 */
#define S_MAPHASHTABLE(SELF)    ((SMapHashTable *)(SELF))


/**
 * @hideinitializer
 * @private
 * Return the given object as an iterator object.
 * @param SELF The given object.
 * @return Given object as #SMapHashTableIterator* type.
 * @note This casting is not safety checked.
 */
#define S_MAPHASHTABLE_ITER(SELF)    ((SMapHashTableIterator *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* MapHashTableIterator class declaration. */
static SMapHashTableIteratorClass MapHashTableIteratorClass;


/************************************************************************************/
/*                                                                                  */
/*  Function implementations                                                        */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void SMapHashTableIteratorInit(SMapHashTableIterator **self, SMapHashTable *map,
									   s_erc *error)
{
	size_t map_size;


	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapHashTableIteratorInit",
				  "Argument \"self\" is NULL");
		return;
	}

	if (map == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapHashTableIteratorInit",
				  "Argument \"map\" is NULL");
		S_DELETE(*self, "SMapHashTableIteratorInit", error);
		*self = NULL;
		return;
	}

	map_size = SMapSize(S_MAP(map), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapHashTableIteratorInit",
				  "Call to \"SMapSize\" failed"))
		goto clean_up;

	if (map_size == 0)
		goto clean_up;

	/* get first element of hash table and set as current element */
	(*self)->c_itr = s_hash_table_first(map->table, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapHashTableIteratorInit",
				  "Failed to find first element of hash table"))
		goto clean_up;

	/* get next element of hash table */
	(*self)->n_itr = s_hash_element_next((s_hash_element*)(*self)->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapHashTableIteratorInit",
				  "Failed to find next element of hash table"))
		goto clean_up;

	/* all OK */
	return;

	/* clean up code */
clean_up:
	S_DELETE(*self, "SMapListIteratorInit", error);
	*self = NULL;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_map_hash_table_iterator_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&MapHashTableIteratorClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_map_hash_table_iterator_class_add",
			  "Failed to add SMapHashTableIteratorClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitMapHashTableIterator(void *obj, s_erc *error)
{
	SMapHashTableIterator *mapItr = obj;


	S_CLR_ERR(error);
	mapItr->c_itr = NULL;
	mapItr->n_itr = NULL;
}


static void DisposeMapHashTableIterator(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static SIterator *Next(SIterator *self, s_erc *error)
{
	SMapHashTableIterator *mapItr = S_MAPHASHTABLE_ITER(self);


	S_CLR_ERR(error);
	if (mapItr->n_itr == NULL)
		return NULL;

	mapItr->c_itr = mapItr->n_itr;
	mapItr->n_itr = s_hash_element_next((s_hash_element*)mapItr->n_itr, error);
	S_CHK_ERR(error, S_CONTERR,
		      "Next",
		      "Failed to find next element of hash table");

	return self;
}


static const char *Key(SIterator *self, s_erc *error)
{
	SMapHashTableIterator *mapItr = S_MAPHASHTABLE_ITER(self);
	const char *key;


	S_CLR_ERR(error);
	key = (const char *)s_hash_element_key((s_hash_element*)mapItr->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Key",
				  "Failed to get key from hash table element"))
		return NULL;

	return key;
}


static const SObject *Object(SIterator *self, s_erc *error)
{
	SMapHashTableIterator *mapItr = S_MAPHASHTABLE_ITER(self);
	const SObject *obj;


	S_CLR_ERR(error);

	obj = (const SObject*)s_hash_element_get_data((s_hash_element*)mapItr->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Object",
				  "Failed to get value from hash table element"))
		return NULL;

	return obj;
}


static SObject *Unlink(SIterator *self, s_erc *error)
{
	SMapHashTableIterator *mapItr = S_MAPHASHTABLE_ITER(self);
	s_hash_element *hte;
	char *key;
	SObject *val;
	s_erc local_err = S_SUCCESS;


	S_CLR_ERR(error);

	if (mapItr->c_itr == NULL)
		return NULL;

	hte = (s_hash_element*)mapItr->c_itr;
	mapItr->c_itr = NULL;

	key = (char*)s_hash_element_key(hte, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Unlink",
				  "Call to \"s_hash_element_key\" failed"))
		local_err = *error;

	if (key != NULL)
		S_FREE(key);

	val = (SObject*)s_hash_element_get_data(hte, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Unlink",
				  "Call to \"s_hash_element_get_data\" failed"))
		local_err = *error;

	/* remove reference to this container */
	if (val != NULL)
		SObjectDecRef(val);

	s_hash_element_unlink(hte, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Unlink",
				  "Call to \"s_hash_element_unlink\" failed"))
		local_err = *error;

	if ((local_err != S_SUCCESS) && (*error == S_SUCCESS))
		*error = local_err;

	return val;
}


/************************************************************************************/
/*                                                                                  */
/* SMapHashTableIterator class initialization                                       */
/*                                                                                  */
/************************************************************************************/

static SMapHashTableIteratorClass MapHashTableIteratorClass =
{
	/* SObjectClass */
	{
		"SIterator:SMapHashTableIterator",
		sizeof(SMapHashTableIterator),
		{ 0, 1},
		InitMapHashTableIterator,    /* init    */
		NULL,                        /* destroy */
		DisposeMapHashTableIterator, /* dispose */
		NULL,                        /* compare */
		NULL,                        /* print   */
		NULL,                        /* copy    */
	},
	/* SIteratorClass */
	Next,                         /* next    */
	Key,                          /* key     */
	Object,                       /* object  */
	Unlink                        /* unlink  */
};


