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
/* Hash table implementation.                                                       */
/*                                                                                  */
/* Derived from the work by Bob Jenkins.                                            */
/* http://www.burtleburtle.net/bob/, May 2006, Public Domain.                       */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include "base/containers/hashtable/hash_table.h"
#include "base/containers/hashtable/hash_functions.h"
#include "base/utils/alloc.h"
#include "base/strings/sprint.h"
#include "base/strings/strings.h"
#include "base/utils/math.h"


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/*
 * The hash table structure.
 */
struct s_hash_table
{
	s_hash_element       **table;     /* hash table, array of size 2^logsize.       */
	uint32                 logsize;   /* log of size of table.                      */
	size_t                 mask;      /* (hashval & mask) is position in table.     */
	uint32                 count;     /* how many items in this hash table so far?  */
	uint32                 apos;      /* position in the array.                     */
    s_hash_table_free_fp   free_func; /* function pointer of element free function. */
};


/*
 * The hash table element structure.
 */
struct s_hash_element
{
	void           *key;        /* key that is hashed.               */
	size_t          keyl;       /* length of key.                    */
	void           *data;       /* data stored in this hash element. */
	uint32          hval;       /* hash value.                       */
	s_hash_element *next;       /* next hash element in list.        */
	s_hash_table   *table;      /* hash table that of this element.  */
};


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void _s_hash_table_resize(s_hash_table *self, uint32 size_hint, s_erc *error);

static s_bool _s_hash_table_n_bucket(s_hash_table *self, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/*
 * Create a new hash table.
 */
S_API s_hash_table *s_hash_table_new(s_hash_table_free_fp free_func,
									 size_t size, s_erc *error)
{
	ulong i;
	ulong len;
	s_hash_table *self;


	S_CLR_ERR(error);

	self = S_MALLOC(s_hash_table, 1);

	if (self == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_hash_table_new",
				  "Failed to allocate memory for new hash table");
		return NULL;
	}

	len = ((ulong)1<<size);
	self->table = S_MALLOC(s_hash_element*, len);

	if (self->table == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_hash_table_new",
				  "Failed to allocate memory for new hash table elements");
		S_FREE(self);
		return NULL;
	}

	for (i = 0; i < len; i++)
		self->table[i] = NULL;

	self->logsize = size;
	self->mask = len-1;
	self->count = 0;
	self->apos = 0;
	self->free_func = free_func;

	return self;
}


/*
 * Delete a hash table.
 */
S_API void s_hash_table_delete(s_hash_table *self, s_erc *error)
{
	s_hash_element *hte;
	s_hash_element *next;
	s_erc local_err = S_SUCCESS;


	S_CLR_ERR(error);

	if (self == NULL)
		return;

	hte = s_hash_table_first(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_hash_table_delete",
				  "Failed to find first element of hash table"))
		return;

	while (hte != NULL)
	{
		next = s_hash_element_next(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_hash_table_delete",
					  "Failed to find next element of hash table"))
		{
			next = NULL;
			local_err = *error;
		}

		s_hash_element_delete(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_hash_table_delete",
					  "Failed to delete element of hash table"))
			local_err = *error;

		hte = next;
	}

	S_FREE(self->table);
	S_FREE(self);

	if ((local_err != S_SUCCESS) && (*error == S_SUCCESS))
		*error = local_err;
}


/*
 * Resize a hash table.
 */
S_API void s_hash_table_resize(s_hash_table *self, sint32 size,
							   s_erc *error)
{
	S_CLR_ERR(error);

	if ((self == NULL)
		|| (size == 0)
		|| (size < -1)
		|| (size == ((sint32)self->logsize)))
		return;

	/* if there are no elements in the table we can do it quite fast,
	 * this is almost the same as s_hash_table_new.
	 */
	if (self->count == 0)
	{
		ulong len;
		ulong i;
		s_hash_element **newtable;


		len = ((ulong)1<<size);

		newtable = S_MALLOC(s_hash_element*, len);
		if (newtable == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "s_hash_table_resize",
					  "Failed to reallocate memory for hash table elements");
			return;
		}

		for (i = 0; i < len; i++)
			newtable[i] = NULL;

		S_FREE(self->table);
		self->table = newtable;

		self->logsize = size;
		self->mask = len-1;
		self->count = 0;
		self->apos = 0;

		return;
	}

	if (size == -1)
	{
		/*
		 * Resize hash table to minimum size that can accommodate
		 * elements already	in table.
		 */
		uint32 logsize = (uint32)ceil(s_log2(self->count));


		_s_hash_table_resize(self, logsize, error);
		S_CHK_ERR(error, S_CONTERR,
				  "s_hash_table_resize",
				  "Call to \"_s_hash_table_resize\" failed");
	}
	else if ((1<<size) < ((sint32)(self->count)))
	{
		/* don't do anything if new size cannot accommodate
		 * elements already	in table.
		 */
		return;
	}
	else
	{
		_s_hash_table_resize(self, (uint32)size, error);
		S_CHK_ERR(error, S_CONTERR,
				  "s_hash_table_resize",
				  "Call to \"_s_hash_table_resize\" failed");
	}
}


/*
 * Add an hash element to a table.
 */
S_API void s_hash_table_add(s_hash_table *self, void *key, size_t key_length,
							void *data, s_erc *error)
{
	register s_hash_element *hte;
	register s_hash_element **hp;
	register uint32 y;
	register uint32 x;


	S_CLR_ERR(error);

	if ((self == NULL) || (key == NULL) || (key_length == 0) || (data == NULL))
		return;

	x = S_HASH(key, key_length, 0);
	y = (x&(self->mask));

	/* make sure the key is not already there */
	for (hte = self->table[y]; hte; hte = hte->next)
	{
		if ((x == hte->hval) &&
		    (key_length == hte->keyl) &&
		    !memcmp(key, hte->key, key_length))
		{
			self->apos = y;

			S_CTX_ERR(error, S_FAILURE,
					  "s_hash_table_add",
					  "Hash table element keys must be unique");
			return;
		}
	}

	/* find space for a new item */
	hte = S_MALLOC(s_hash_element, 1);

	if (hte == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_hash_table_add",
				  "Failed to allocate memory for new hash table element");
		return;
	}

	/* make the hash table bigger if it is getting full */
	if (++self->count > (uint32)1<<(self->logsize))
	{
		_s_hash_table_resize(self, 0, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_hash_table_add",
					  "Failed to grow table while adding element"))
		{
			S_FREE(hte);
			return;
		}

		y = (x&(self->mask));
	}

	/* add the new key to the table */
	hte->key = key;
	hte->keyl = key_length;
	hte->data = data;
	hte->hval  = x;
	hte->table = self;
	hp = &self->table[y];
	hte->next = *hp;
	*hp = hte;
	self->apos = y;

	return;
}


S_API void s_hash_element_unlink(s_hash_element *self, s_erc *error)
{
	s_hash_element **ip;     /* a counter */


	S_CLR_ERR(error);

	if ((self == NULL)
		|| (self->table == NULL))
		return;

	/* set table position */
	self->table->apos = (self->hval&(self->table->mask));

	/*
	 * remove element from its list
	 * this is for the elements in a list as a consequence of
	 * collisions
	 */
	for (ip = &self->table->table[self->table->apos]; *ip != self; ip = &(*ip)->next)
		/* void */;

	/*
	 * got the element, no we move the pointer to the next element
	 */
	*ip = (*ip)->next;

	/* decrement count */
	--(self->table->count);

	S_FREE(self);
}


S_API void s_hash_element_delete(s_hash_element *self, s_erc *error)
{
	s_hash_element **ip;     /* a counter */


	S_CLR_ERR(error);

	if (self == NULL)
		return;

	if (self->table == NULL)
	{
		S_FREE(self);
		return;
	}

	/* set table position */
	self->table->apos = (self->hval&(self->table->mask));

	/*
	 * remove element from its list
	 * this is for the elements in a list as a consequence of
	 * collisions
	 */
	for (ip = &self->table->table[self->table->apos]; *ip != self; ip = &(*ip)->next)
		/* void */;

	/*
	 * got the element, no we move the pointer to the next element
	 */
	*ip = (*ip)->next;

	/* decrement count */
	--(self->table->count);

	if (self->table->free_func != NULL)
	{
		(self->table->free_func)(self->key, self->data, error);
		S_CHK_ERR(error, S_CONTERR,
				  "s_hash_element_delete",
				  "Call to hash table free function failed");
	}

	S_FREE(self);
}


/*
 * Find a hash element in the hash table.
 */
S_API s_hash_element *s_hash_table_find(s_hash_table *self, void *key,
										size_t keyl, s_erc *error)
{
	s_hash_element *hte;
	ulong y;
	ulong x;


	S_CLR_ERR(error);

	if ((self == NULL) || (key == NULL) || (keyl == 0))
		return NULL;

	x = S_HASH(key, keyl, 0);
	y = (x&(self->mask));

	for (hte = self->table[y]; hte; hte = hte->next)
	{
		if ((x == hte->hval) &&
		    (keyl == hte->keyl) &&
		    !memcmp(key, hte->key, keyl))
		{
			self->apos = y;
			return hte;
		}
	}

	return NULL;
}


S_API s_hash_element *s_hash_table_first(s_hash_table *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
		return NULL;

	self->apos = self->mask;
	_s_hash_table_n_bucket(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_hash_table_first",
				  "Failed to find element in bucket"))
		return NULL;

	return self->table[self->apos];
}


S_API s_hash_element *s_hash_element_next(s_hash_element *self, s_erc *error)
{
	s_bool no_wrap;


	S_CLR_ERR(error);

	if ((self == NULL) || (self->table == NULL))
		return NULL;

	/* set table position (self element) */
	self->table->apos = (self->hval&(self->table->mask));

	if (self->next != NULL)
		return self->next;

	no_wrap = _s_hash_table_n_bucket(self->table, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_hash_element_next",
				  "Cannot find first item in next nonempty bucket"))
		return NULL;

	if (!no_wrap)
		return NULL;

	return self->table->table[self->table->apos];
}


S_API void *s_hash_element_key(s_hash_element *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
		return NULL;

	return self->key;
}


S_API size_t s_hash_element_key_length(s_hash_element *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
		return 0;

	return self->keyl;
}


S_API void *s_hash_element_get_data(s_hash_element *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
		return NULL;

	return self->data;
}


S_API void s_hash_element_set_data(s_hash_element *self, void *data,
								   s_erc *error)
{
	S_CLR_ERR(error);

	if ((self == NULL) || (data == NULL))
		return;

	self->data = data;
}


S_API uint32 s_hash_element_pos(s_hash_element *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_hash_element_pos",
				  "Argument \"self\" is NULL");
		return 0;
	}

	if (self->table == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_hash_element_pos",
				  "Given hash element does not belong to a hash table");
		return 0;
	}

	return (self->hval&(self->table->mask));
}


/*
 * Print statistics about the hash table
 */
S_API char *s_hash_table_stats(s_hash_table *self, s_erc *error)
{
	uint32 i;
	uint32 j;
	double total = 0.0;
	s_hash_element *hte;
	s_hash_element *walk;
	s_hash_element *walk2;
	s_hash_element *stat;
	char *buf;
	char *tmp;


	S_CLR_ERR(error);

	stat = NULL;
	total = 0.0;

	/* in stat, keyl will store length of list, hval the number of buckets */
	for (i = 0; i <= self->mask; ++i)
	{
		for (hte = self->table[i], j = 0; hte; ++j, hte = hte->next)
			/* void */;

		for (walk = stat; walk && (walk->keyl != j); walk = walk->next)
			/* void */;

		if (walk != NULL)
		{
			++(walk->hval);
		}
		else
		{
			walk = S_MALLOC(s_hash_element, 1);

			if (walk == NULL)
			{
				S_FTL_ERR(error, S_MEMERROR,
						  "s_hash_table_stats",
						  "Failed to allocate memory for new hash table element");

				return NULL;
			}

			walk->keyl = j;
			walk->hval = 1;

			if ((stat == NULL) || (stat->keyl > j))
			{
				walk->next = stat;
				stat = walk;
			}
			else
			{
				for (walk2 = stat;
				     walk2->next && (walk2->next->keyl<j);
				     walk2 = walk2->next)
					/* void */;

				walk->next = walk2->next;
				walk2->next = walk;
			}
		}
	}

	/* figure out average list length for existing elements */
	for (walk = stat; walk; walk = walk->next)
	{
		total+=(double)walk->hval*(double)walk->keyl*(double)walk->keyl;
	}

	if (self->count != 0)
		total /= (double)self->count;
	else
		total = (double)0;


	/* print statistics */

	buf = s_strdup("s_hash_table_stats:\n", error);

	for (walk = stat; walk; walk = walk->next)
	{
		s_asprintf(&tmp, error, "items %lu: %lu buckets\n", walk->keyl, walk->hval);
		buf = S_REALLOC(buf, char, s_strsize(buf, error) + s_strsize(tmp, error) + 1);
		buf = s_strzcat(buf, tmp, s_strsize(buf, error) + s_strsize(tmp, error) + 1, error);
		S_FREE(tmp);
	}

	s_asprintf(&tmp, error, "\nbuckets: %lu\titems: %lu\texisting: %f\n\n",
			   ((ulong)1<<self->logsize), self->count, total);
	buf = S_REALLOC(buf, char, s_strsize(buf, error) + s_strsize(tmp, error) + 1);
	buf = s_strzcat(buf, tmp, s_strsize(buf, error) + s_strsize(tmp, error) + 1, error);
	S_FREE(tmp);

	/* clean up */
	while (stat != NULL)
	{
		walk = stat->next;
		S_FREE(stat);
		stat = walk;
	}

	return buf;
}


/*
 * Get the number of elements in the list.
 */
S_API uint32 s_hash_table_size(s_hash_table *self, s_erc *error)
{
	S_CLR_ERR(error);

	return self ? (self->count) : 0;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/*
 * private function, grow/shrink hash table, this is slow.
 * s_hash_table_shrink
 */
static void _s_hash_table_resize(s_hash_table *self, uint32 size_hint, s_erc *error)
{
	register uint32 newsize;
	register uint32 newmask;
	register uint32 i;
	register s_hash_element **oldtab;
	register s_hash_element **newtab;


	S_CLR_ERR(error);

	if (size_hint == 0)
		newsize = (uint32)1<<(self->logsize + 1);
	else
		newsize = (uint32)1<<size_hint;

	newmask = newsize-1;
	oldtab = self->table;
	newtab = S_MALLOC(s_hash_element *,newsize);

	if (newtab == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "_s_hash_table_grow",
				  "Failed to allocate memory for new hash table elements");
		return;
	}

	/* make sure newtab is cleared */
	for (i = 0; i < newsize; i++)
		newtab[i] = NULL;

	self->table = newtab;
	self->mask = newmask;

	/* Walk through old table putting entries in new table */
	for (i = 1<<(self->logsize); i--;)
	{
		register s_hash_element  *this_element;
		register s_hash_element  *that;
		register s_hash_element **newplace;

		for (this_element = oldtab[i]; this_element; /* NOP */)
		{
			that = this_element;
			this_element = this_element->next;
			newplace = &newtab[(that->hval & newmask)];
			that->next = *newplace;
			*newplace = that;
		}
	}

	if (size_hint == 0)
		self->logsize++;
	else
		self->logsize = size_hint;

	/* position the hash table on some existing item */
	s_hash_table_first(self, error);

	S_CHK_ERR(error, S_CONTERR,
			  "_s_hash_table_grow",
			  "Failed to find first element after grow");

	/* free the old array */
	S_FREE(oldtab);
}


/*
 * Private function - move to first item in the next nonempty bucket.
 * t    - the hash table
 * This is private to hash_table; do not use it externally.
 * Returns TRUE if element found without wrap around, else FALSE
 */

static s_bool _s_hash_table_n_bucket(s_hash_table *self, s_erc *error)
{
	uint32 oldapos;
	uint32 end;
	uint32 i;


	S_CLR_ERR(error);

	oldapos = self->apos;
	end = (uint32)1<<(self->logsize);

	/* see if the element can be found without wrapping around */
	for (i = oldapos+1; i < end; ++i)
	{
		if (self->table[i&self->mask] != NULL)
		{
			self->apos = i;
			return TRUE;
		}
	}

	/* must have to wrap around to find the last element */
	for (i = 0; i <= oldapos; ++i)
	{
		if (self->table[i] != NULL)
		{
			self->apos = i;
			return FALSE;
		}
	}

	return FALSE;
}

