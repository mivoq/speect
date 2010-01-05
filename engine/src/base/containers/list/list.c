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
/* DATE    : 25 March 2008                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Doubly linked list data structure.                                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/containers/list/list.h"
#include "base/utils/alloc.h"


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/*
 * Definition of a doubly linked list.
 */
struct s_list
{
	uint32              size;           /* The size of the list.                                  */
	s_list_element     *first;          /* Pointer to the first element in the list.              */
	s_list_element     *last;           /* Pointer to the last element in the list.               */
	s_list_compare_fp   compare_func;   /* Function pointer of list element comparison function.  */
	s_list_free_fp      free_func;      /* Function pointer of list element free function.        */
};


/*
 * Definition of a list element.
 */
struct s_list_element
{
	void              *data;  /* The data of the list element.      */
	s_list_element    *next;  /* The next element in the list.      */
	s_list_element    *prev;  /* The previous element in the list.  */
	s_list            *list;  /* List that this element belongs to. */
};


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/* List accessing functions */

/*
 * Return the first element in the list.
 */
S_API s_list_element *s_list_first(s_list *self, s_erc *error)
{
	S_CLR_ERR(error);

	return self ? self->first : NULL;
}


/*
 * Return the last element in the list.
 */
S_API s_list_element *s_list_last(s_list *self, s_erc *error)
{
	S_CLR_ERR(error);

	return self ? self->last : NULL;
}


/* List element functions */

/*
 * Get the list element data.
 */
S_API const void *s_list_element_get(s_list_element *self, s_erc *error)
{
	S_CLR_ERR(error);

	return self ? self->data : NULL;
}


/*
 * Replace list element data, does not delete old data.
 */
S_API void *s_list_element_replace(s_list_element *self, void *data, s_erc *error)
{
	void *old_data;


	S_CLR_ERR(error);

	if (self == NULL)
		return NULL;

	old_data = self->data;
	self->data = data;

	return old_data;
}


/*
 * Unlink list element from list. Does not delete data of element.
 */
S_API void *s_list_element_unlink(s_list_element *self, s_erc *error)
{
	void *tmp;


	S_CLR_ERR(error);

	if ((self == NULL) || (self->list == NULL))
		return NULL;

	if (self->prev == NULL)
		self->list->first = self->next;
	else
		self->prev->next = self->next;

	if (self->next == NULL)
		self->list->last = self->prev;
	else
		self->next->prev = self->prev;

	self->list->size--;
	tmp = self->data;
	S_FREE(self);

	return tmp;
}


/*
 * Remove list element from list and delete list element memory.
 */
S_API s_list_element *s_list_element_delete(s_list_element *self, s_erc *error)
{
	s_list_element *tmp;

	S_CLR_ERR(error);

	if ((self == NULL) || (self->list == NULL) || (self->list->free_func == NULL))
		return NULL;

	if (self->prev == NULL)
		self->list->first = self->next;
	else
		self->prev->next = self->next;

	if (self->next == NULL)
		self->list->last = self->prev;
	else
		self->next->prev = self->prev;

	self->list->size--;
	tmp = self->prev;

	(self->list->free_func)(self->data, error);
	S_CHK_ERR(error, S_FAILURE,
			  "s_list_element_delete",
			  "Call to list free function failed");

	S_FREE(self);

	return tmp;
}


/*
 * Return the next element in the list.
 */
S_API s_list_element *s_list_element_next(s_list_element *self, s_erc *error)
{
	S_CLR_ERR(error);

	if ((self == NULL) || (self->next == NULL))
		return NULL;

	self = self->next;
	return self;
}


/*
 * Return the prev element in the list.
 */
S_API s_list_element *s_list_element_prev(s_list_element *self, s_erc *error)
{
	S_CLR_ERR(error);

	if ((self == NULL) || (self->prev == NULL))
		return NULL;

	self = self->prev;
	return self;
}


/* List query functions */

/*
 * Query if the list has any elements.
 */
S_API s_bool s_list_isempty(s_list *self, s_erc *error)
{
	S_CLR_ERR(error);

	return self ? ((self->size == 0) ? TRUE : FALSE) : TRUE;
}


/*
 * Get the number of elements in the list.
 */
S_API uint32 s_list_size(s_list *self, s_erc *error)
{
	S_CLR_ERR(error);

	return self ? (self->size) : 0;
}


/*
 * Find the element in the list which matches the comparison with the
 * comparison function #s_list_compare_fp. If the comparison function is
 * null then nothing is done.
 */
S_API s_list_element *s_list_find_index(s_list *self, s_list_element *f,
										const void *data, int *index, s_erc *error)
{
	s_list_element *iterator;
	s_list_element *from;
	int cnt;
	s_bool compared;

	S_CLR_ERR(error);

	if ((self == NULL) || (data == NULL) || (self->compare_func == NULL))
		return NULL;

	if (f == NULL)
		from = s_list_first(self, error);
	else
		from = f;

	for (iterator = from, cnt = 0; iterator != NULL;
	     iterator = s_list_element_next(iterator, error), cnt++)
	{
		compared = (self->compare_func)(data, s_list_element_get(iterator, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_list_find_index",
					  "Call to list compare function failed"))
			return NULL;

		if (compared == TRUE)
		{
			if (index != NULL)
				(*index) = cnt;
			return iterator;
		}
	}

	return NULL;
}


/*
 * Find the nth element in the list.
 */
S_API s_list_element *s_list_nth(s_list *self, uint32 n, s_erc *error)
{
	s_list_element *iterator = NULL;
	uint32 cnt;

	S_CLR_ERR(error);

	if ((self == NULL) || (n > self->size))
		return NULL;

	for (iterator = s_list_first(self, error), cnt = 0; cnt < n; cnt++,
		     iterator = s_list_element_next(iterator, error))
		continue;

	return iterator;
}


/* List queue functions */

/*
 * Push data onto end of list. Same as #s_list_append.
 */
S_API void s_list_push(s_list *self, void *data, s_erc *error)
{
	S_CLR_ERR(error);

	s_list_append(self, data, error);
}


/*
 * Pop last element of list. Removes last element from
 */
S_API void *s_list_pop(s_list *self, s_erc *error)
{
	s_list_element *last = NULL;

	S_CLR_ERR(error);

	if (self == NULL)
		return NULL;

	last = s_list_last(self, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_list_pop",
				  "Failed to get last item in list"))
		return NULL;

	if (last != NULL)
		return s_list_element_unlink(last, error);

	return NULL;
}


/*
 * Reverse the elements in the list.
 */
S_API void s_list_reverse(s_list *self, s_erc *error)
{
	s_list_element *iterator;
	s_list_element *tmp;
	s_list_element *orig_first;


	S_CLR_ERR(error);

	if (self == NULL)
		return;

	orig_first = s_list_first(self, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_list_reverse",
				  "Failed to get first item in list"))
		return;

	iterator = s_list_last(self, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_list_reverse",
				  "Failed to get last item in list"))
		return;

	while (iterator != NULL)
	{
		tmp = iterator->next;
		iterator->next = iterator->prev;
		iterator->prev = tmp;

		iterator = iterator->next;
	}

	self->first = self->last;
	self->last = orig_first;
}


/* List insertion functions */

/*
 * Prepend data to beginning of the list.
 */
S_API void s_list_prepend(s_list *self, void *data, s_erc *error)
{
	s_list_element *new_element;


	S_CLR_ERR(error);

	if ((self == NULL) || (data == NULL))
		return;

	if (self->first != NULL) /* the list was not empty */
		s_list_insert_before(self->first, data, error);
	else
	{
		new_element = S_MALLOC(s_list_element, 1);

		if (new_element == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "s_list_prepend",
					  "Failed to allocate memory for new list element");
			return;
		}

		new_element->data = data;
		new_element->prev = NULL;
		new_element->next = NULL;
		new_element->list = self;
		self->first = new_element;
		self->last = new_element;
		self->size++;
	}
}


/*
 * Append data to end of the list.
 */
S_API void s_list_append(s_list *self, void *data, s_erc *error)
{
	S_CLR_ERR(error);

	if ((self == NULL) || (data == NULL))
		return;

	if (self->last == NULL)
		s_list_prepend(self, data, error);
	else
		s_list_insert_after(self->last, data, error);
}


/*
 * Insert data before given list element. Return reference to inserted data element.
 */
S_API s_list_element *s_list_insert_before(s_list_element *self, void *data, s_erc *error)
{
	s_list_element *new_element ;


	S_CLR_ERR(error);

	if ((self == NULL) || (data == NULL) || (self->list == NULL))
		return NULL;

	new_element = S_MALLOC(s_list_element, 1);

	if (new_element == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_list_insert_before",
				  "Failed to allocate memory for new list element");
		return NULL;
	}

	new_element->data = data;
	new_element->prev = self->prev;
	new_element->next = self;
	new_element->list = self->list;

	if (self->prev == NULL)
		self->list->first = new_element;
	else
		self->prev->next = new_element;

	self->prev = new_element;
	self->list->size++;

	return new_element;
}


/*
 * Insert data after given list element. Return reference to inserted data element.
 */
S_API s_list_element *s_list_insert_after(s_list_element *self, void *data, s_erc *error)
{
	s_list_element *new_element;


	S_CLR_ERR(error);

	if ((self == NULL) || (data == NULL) || (self->list == NULL))
		return NULL;

	new_element = S_MALLOC(s_list_element, 1);

	if (new_element == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_list_insert_after",
				  "Failed to allocate memory for new list element");
		return NULL;
	}

	new_element->data = data;
	new_element->prev = self;
	new_element->next = self->next;
	new_element->list = self->list;

	if (self->next == NULL)
		self->list->last = new_element;
	else
		self->next->prev = new_element;

	self->next = new_element;
	self->list->size++;

	return new_element;
}


/* List creation, deletion */

/*
 * Create a new list.
 */
S_API s_list *s_list_new(s_list_compare_fp compare_func, s_list_free_fp free_func, s_erc *error)
{
	s_list *self;


	S_CLR_ERR(error);

	self = S_MALLOC(s_list, 1);

	if (self == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_list_new",
				  "Failed to allocate memory for new list");
		return NULL;
	}

	self->size = 0;
	self->first = NULL;
	self->last = NULL;
	self->compare_func = compare_func;
	self->free_func = free_func;

	return self;
}


/*
 * Delete a list.
 */
S_API void s_list_delete(s_list *self, s_erc *error)
{
	s_list_element *iterator;


	S_CLR_ERR(error);

	if (self == NULL)
		return;

	if (self->free_func != NULL)
	{
		do
		{
			iterator = s_list_element_delete(self->last, error);
		} while (iterator);
	}
	else
	{
		void *data;

		do
		{
			data = s_list_element_unlink(self->last, error);
		} while (data);
	}

	if (self != NULL)
		S_FREE(self);
}

