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

S_LOCAL void SMapHashTableIteratorInit(SIterator **self, SMapHashTable *map,
									   s_erc *error)
{
	SMapHashTableIterator *mapHashTableItr;


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

	mapHashTableItr = S_CAST(*self, SMapHashTableIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapHashTableInit",
				  "Argument \"self\" is not of SMapHashTableIterator type"))
	{
		S_DELETE(*self, "SMapHashTableIteratorInit", error);
		*self = NULL;
		return;
	}

	(*self)->myContainer = S_CONTAINER(map);
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


static SIterator *First(SIterator *self, s_erc *error)
{
	SMapHashTableIterator *mapItr = S_MAPHASHTABLE_ITER(self);
	s_hash_element *hte;
	uint32 table_size;


	S_CLR_ERR(error);

	hte = s_hash_table_first(S_MAPHASHTABLE(self->myContainer)->table, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "First",
				  "Failed to find first element of hash table"))
		return self;

	table_size = s_hash_table_size(S_MAPHASHTABLE(self->myContainer)->table, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "First",
				  "Failed to get hash table size"))
		return self;

	if (hte == NULL)
	{
		if (table_size != 0)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "First",
					  "Failed to move to first element of hash table");
			return self;
		}
		else
		{
			return NULL;
		}
	}

	mapItr->c_itr = hte;
	mapItr->n_itr = s_hash_element_next((s_hash_element*)mapItr->c_itr, error);
	S_CHK_ERR(error, S_CONTERR,
		      "First",
		      "Failed to find next element of hash table");

	return self;
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
	First,                           /* first   */
	NULL,                            /* last    */
	Next,                            /* next    */
	NULL                             /* prev    */
};


