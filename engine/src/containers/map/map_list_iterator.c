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

S_LOCAL void SMapListIteratorInit(SIterator **self, SMapList *map, s_erc *error)
{
	SMapListIterator *mapListItr;


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

	mapListItr = S_CAST(*self, SMapListIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapListIteratorInit",
				  "Argument \"self\" is not of SMapListIterator type"))
	{
		S_DELETE(*self, "SMapListIteratorInit", error);
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
	mapItr->p_itr = NULL;
}


static void DisposeMapListIterator(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static SIterator *First(SIterator *self, s_erc *error)
{
	SMapListIterator *mapItr = S_MAPLIST_ITER(self);
	s_bool list_is_empty;


	S_CLR_ERR(error);
	mapItr->c_itr = s_list_first(S_MAPLIST(self->myContainer)->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "First",
				  "Failed to find first element of list"))
		return self;

	list_is_empty = s_list_isempty(S_MAPLIST(self->myContainer)->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "First",
				  "Call to s_list_isempty failed"))
		return self;

	if (mapItr->c_itr == NULL)
	{
		if (list_is_empty == FALSE)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "First",
					  "Failed to move to first element of list");
			return self;
		}
		else
		{
			return NULL;
		}
	}

	mapItr->p_itr = NULL;
	mapItr->n_itr = s_list_element_next((s_list_element*)mapItr->c_itr, error);
	S_CHK_ERR(error, S_CONTERR,
			  "First",
			  "Failed to move to next element of list");

	return self;
}


static SIterator *Last(SIterator *self, s_erc *error)
{
	SMapListIterator *mapItr = S_MAPLIST_ITER(self);
	s_bool list_is_empty;


	S_CLR_ERR(error);
	mapItr->c_itr = s_list_last(S_MAPLIST(self->myContainer)->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Last",
				  "Failed to find last element of list"))
		return self;

	list_is_empty = s_list_isempty(S_MAPLIST(self->myContainer)->list, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Last",
				  "Call to s_list_isempty failed"))
		return self;

	if (mapItr->c_itr == NULL)
	{
		if (list_is_empty == FALSE)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "Last",
					  "Failed to move to last element of list");
			return self;
		}
		else
		{
			return NULL;
		}
	}

	mapItr->n_itr = NULL;
	mapItr->p_itr = s_list_element_prev((s_list_element*)mapItr->c_itr, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Last",
			  "Failed to move to prev element of list");

	return self;
}


static SIterator *Next(SIterator *self, s_erc *error)
{
	SMapListIterator *mapItr = S_MAPLIST_ITER(self);


	S_CLR_ERR(error);

	if (mapItr->n_itr == NULL)
		return NULL;

	mapItr->p_itr = mapItr->c_itr;
	mapItr->c_itr = mapItr->n_itr;
	mapItr->n_itr = s_list_element_next((s_list_element*)mapItr->n_itr, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Next",
			  "Failed to move to next element of list");

	return self;
}


static SIterator *Prev(SIterator *self, s_erc *error)
{
	SMapListIterator *mapItr = S_MAPLIST_ITER(self);


	S_CLR_ERR(error);
	if (mapItr->p_itr == NULL)
		return NULL;

	mapItr->n_itr = mapItr->c_itr;
	mapItr->c_itr = mapItr->p_itr;
	mapItr->p_itr = s_list_element_prev((s_list_element*)mapItr->p_itr, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Prev",
			  "Failed to move to prev element of list");

	return self;
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
	First,                        /* first   */
	Last,                         /* last    */
	Next,                         /* next    */
	Prev                          /* prev    */
};

