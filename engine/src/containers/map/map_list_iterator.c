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
/* Iterator implementation of SMapList container.                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/objsystem/class.h"
#include "containers/map/map_list_iterator.h"


/************************************************************************************/
/*                                                                                  */
/*  Data types (as defined in map_list.c)                                           */
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
/* SMapListIteratorClass definition                                                 */
/*                                                                                  */
/************************************************************************************/

/**
 * The SMapListIterator class structure.  Same as SIteratorClass as
 * we are not adding any new methods.
 */
typedef SIteratorClass SMapListIteratorClass;


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given object as an #SMapList object.
 * @param SELF The given object.
 * @return Given object as #SMapList* type.
 * @note This casting is not safety checked.
 */
#define S_MAPLIST(SELF)    ((SMapList *)(SELF))


/**
 * @hideinitializer
 * @private
 * Return the given object as an iterator object.
 * @param SELF The given object.
 * @return Given object as #SMapListIterator* type.
 * @note This casting is not safety checked.
 */
#define S_MAPLIST_ITER(SELF)    ((SMapListIterator *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* MapListIterator class declaration. */
static SMapListIteratorClass MapListIteratorClass;


/************************************************************************************/
/*                                                                                  */
/*  Function implementations                                                        */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void SMapListIteratorInit(SMapListIterator **self, SMapList *map, s_erc *error)
{
	size_t map_size;


	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapListIteratorInit",
				  "Argument \"self\" is NULL");
		return;
	}

	if (map == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapListIteratorInit",
				  "Argument \"map\" is NULL");
		S_DELETE(*self, "SMapListIteratorInit", error);
		*self = NULL;
		return;
	}

	map_size = SMapSize(S_MAP(map), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapListIteratorInit",
				  "Call to \"SMapSize\" failed"))
		goto clean_up;

	if (map_size == 0)
		goto clean_up;

	/* get first element of list and set as current element */
	(*self)->c_itr = s_list_first(map->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapListIteratorInit",
				  "Failed to find first element of list"))
		goto clean_up;

	/* get next element of list */
	(*self)->n_itr = s_list_element_next((s_list_element*)(*self)->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapListIteratorInit",
				  "Failed to get next element of list"))
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

S_LOCAL void _s_map_list_iterator_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&MapListIteratorClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_map_list_iterator_class_add",
			  "Failed to add SMapListIteratorClass");
}



/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitMapListIterator(void *obj, s_erc *error)
{
	SMapListIterator *mapItr = obj;

	S_CLR_ERR(error);
	mapItr->c_itr = NULL;
	mapItr->n_itr = NULL;
}


static void DisposeMapListIterator(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static SIterator *Next(SIterator *self, s_erc *error)
{
	SMapListIterator *mapItr = S_MAPLIST_ITER(self);


	S_CLR_ERR(error);
	if (mapItr->n_itr == NULL)
		return NULL;

	mapItr->c_itr = mapItr->n_itr;
	mapItr->n_itr = s_list_element_next((s_list_element*)mapItr->n_itr, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Next",
			  "Failed to move to next element of list");

	return self;
}


static const char *Key(SIterator *self, s_erc *error)
{
	const SMapListIterator *mapItr;
	const s_kvp *tmp;


	S_CLR_ERR(error);

	/* must cast this one to make sure */
	mapItr = S_CAST(self, SMapListIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Key",
				  "Failed to cast SIterator to SMapListIterator"))
		return NULL;

	tmp = s_list_element_get((s_list_element*)mapItr->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Key",
				  "Call to \"s_list_element_get\" failed"))
		return NULL;

	if (tmp == NULL)
		return NULL;

	return tmp->key;
}


static const SObject *Object(SIterator *self, s_erc *error)
{
	SMapListIterator *mapItr;
	const s_kvp *tmp;


	S_CLR_ERR(error);

	/* must cast this one to make sure */
	mapItr = S_CAST(self, SMapListIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Object",
				  "Failed to cast SIterator to SMapListIterator"))
		return NULL;

	tmp = s_list_element_get((s_list_element*)mapItr->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Object",
				  "Call to \"s_list_element_get\" failed"))
		return NULL;

	if (tmp == NULL)
		return NULL;

	return tmp->val;
}


static SObject *Unlink(SIterator *self, s_erc *error)
{
	SMapListIterator *mapItr;
	SObject *obj;
	s_kvp *tmp;
	char *key;


	S_CLR_ERR(error);

	/* must cast this one to make sure */
	mapItr = S_CAST(self, SMapListIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Unlink",
				  "Failed to cast SIterator to SMapListIterator"))
		return NULL;

	if (mapItr->c_itr == NULL)
		return NULL;

	tmp = s_list_element_unlink((s_list_element*)mapItr->c_itr, error);
	mapItr->c_itr = NULL;
	if (S_CHK_ERR(error, S_CONTERR,
				  "Unlink",
				  "Call to \"s_list_element_unlink\" failed"))
		return NULL;

	obj = S_OBJECT(tmp->val);

	/* remove the object's reference to the container */
	SObjectDecRef(obj);

	/* free the key */
	key = (char*)tmp->key;
	S_FREE(key);

	/* free key-value pair container */
	S_FREE(tmp);

	return obj;
}


/************************************************************************************/
/*                                                                                  */
/* SMapListIterator class initialization                                            */
/*                                                                                  */
/************************************************************************************/

static SMapListIteratorClass MapListIteratorClass =
{
	/* SObjectClass */
	{
		"SIterator:SMapListIterator",
		sizeof(SMapListIterator),
		{ 0, 1},
		InitMapListIterator,      /* init    */
		NULL,                     /* destroy */
		DisposeMapListIterator,   /* dispose */
		NULL,                     /* compare */
		NULL,                     /* print   */
		NULL,                     /* copy    */
	},
	/* SIteratorClass */
	Next,                         /* next    */
	Key,                          /* key     */
	Object,                       /* object  */
	Unlink                        /* unlink  */
};
