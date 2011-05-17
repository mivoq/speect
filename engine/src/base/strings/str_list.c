/************************************************************************************/
/* Copyright (c) 2011 The Department of Arts and Culture,                           */
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
/* DATE    : 12 May 2011                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* String lists implementation                                                      */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/errdbg/errdbg_macros.h"
#include "base/containers/list/list.h"
#include "base/strings/strings.h"
#include "base/strings/sprint.h"
#include "base/strings/str_list.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_bool s_str_list_compare_fp(const void *le1, const void *le2, s_erc *error);

static void s_str_list_free_fp(void *le, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * Create a new string list.
 */
S_API s_str_list *s_str_list_new(s_erc *error)
{
	s_str_list *self;


	S_CLR_ERR(error);
	self = s_list_new(&s_str_list_compare_fp, &s_str_list_free_fp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_new",
				  "Call to \"s_list_new\" failed"))
		return NULL;

	return self;
}


/**
 * Create a new string list from sub-strings of @c string divided
 * by @c split_string.
 */
S_API s_str_list *s_str_list_split(const char *string, const char *split_string,
								   s_erc *error)
{
	s_str_list *self = NULL;
	char *str_copy = NULL;
	char *nstr_copy;
	char *s;


	S_CLR_ERR(error);
	if ((string == NULL) || (split_string == NULL))
		goto error_return;

	self = s_str_list_new(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_split",
				  "Call to \"s_str_list_new\" failed"))
		goto error_return;

	str_copy = s_strdup(string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_split",
				  "Call to \"s_strdup\" failed"))
		goto error_return;

	nstr_copy = str_copy;
	s = s_strtok_r(NULL, split_string, &nstr_copy, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_split",
				  "Call to \"s_strtok_r\" failed"))
		goto error_return;

	while (s != NULL)
	{
		s_str_list_append(self, s, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_split",
					  "Call to \"s_str_list_append\" failed"))
			goto error_return;

		s = s_strtok_r(NULL, split_string, &nstr_copy, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_split",
					  "Call to \"s_strtok_r\" failed"))
			goto error_return;
	}

	/* all OK, clean and return */
	S_FREE(str_copy);
	return self;

	/* errors occurred, clean and return */
error_return:
	{
		s_erc local_err = S_FAILURE;


		if (str_copy != NULL)
			S_FREE(str_copy);

		if (self != NULL)
			s_list_delete(self, &local_err);
	}

	return NULL;
}


/**
 * Delete a string list.
 */
S_API void s_str_list_delete(s_str_list *self, s_erc *error)
{
	S_CLR_ERR(error);

	s_list_delete(self, error);
	S_CHK_ERR(error, S_CONTERR,
			  "s_str_list_delete",
			  "Call to \"s_list_delete\" failed");
}


/**
 * Find the string in the list which matches the given string.
 */
S_API const s_str_list_element *s_str_list_find_index(const s_str_list *self, const s_str_list_element *f,
													  const char *string, int *index, s_erc *error)
{
	const s_str_list_element *el;


	S_CLR_ERR(error);

	el = s_list_find_index(self, f, string, index, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_delete",
				  "Call to \"s_list_find_index\" failed"))
		return NULL;

	return el;
}


/**
 * Return the first string element in the list.
 */
S_API const s_str_list_element *s_str_list_first(const s_str_list *self, s_erc *error)
{
	const s_str_list_element *el;


	S_CLR_ERR(error);

	el = s_list_first(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_first",
				  "Call to \"s_list_first\" failed"))
		return NULL;

	return el;
}


/**
 * Return the last string element in the list.
 */
S_API const s_str_list_element *s_str_list_last(const s_str_list *self, s_erc *error)
{
	const s_str_list_element *el;


	S_CLR_ERR(error);

	el = s_list_last(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_last",
				  "Call to \"s_list_last\" failed"))
		return NULL;

	return el;
}


/**
 * Find the nth string element in the list.
 */
S_API const s_str_list_element *s_str_list_nth(const s_str_list *self, uint32 n, s_erc *error)
{
	const s_str_list_element *el;


	S_CLR_ERR(error);

	el = s_list_nth(self, n, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_nth",
				  "Call to \"s_list_nth\" failed"))
		return NULL;

	return el;
}


/**
 * Find the nth string in the list.
 */
S_API const char *s_str_list_nth_string(const s_str_list *self, uint32 n, s_erc *error)
{
	const s_str_list_element *el;
	const char *string;


	S_CLR_ERR(error);

	el = s_list_nth(self, n, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_nth_string",
				  "Call to \"s_list_nth\" failed"))
		return NULL;

	string = s_str_list_element_get(el, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_nth_string",
				  "Call to \"s_str_list_element_get\" failed"))
		return NULL;

	return string;
}


/**
 * Get the string list element's string.
 */
S_API const char *s_str_list_element_get(const s_str_list_element *self, s_erc *error)
{
	const char *data;


	S_CLR_ERR(error);

	data = s_list_element_get(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_element_get",
				  "Call to \"s_list_element_get\" failed"))
		return NULL;

	return data;
}


/**
 * Replace the string list element's string. The replaced string is
 * freed.
 */
S_API void s_str_list_element_replace(s_str_list_element *self, const char *string,
									  s_erc *error)
{
	char *replaced;
	char *new_string;


	S_CLR_ERR(error);
	if ((self == NULL) || (string == NULL))
		return;

	new_string = s_strdup(string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_element_replace",
				  "Call to \"s_strdup\" failed"))
		return;

	replaced = s_list_element_replace(self, new_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_element_replace",
				  "Call to \"s_list_element_replace\" failed"))
	{
		S_FREE(new_string);
		return;
	}

	S_FREE(replaced);
}


/**
 * Unlink string list element from string list, and return the string
 * list element's string.
 */
S_API char *s_str_list_element_unlink(s_str_list_element *self, s_erc *error)
{
	char *unlinked;


	S_CLR_ERR(error);
	unlinked = s_list_element_unlink(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_element_unlink",
				  "Call to \"s_list_element_unlink\" failed"))
		return NULL;

	return unlinked;
}


/**
 * Remove the string list element from it's parent string list and
 * delete string list element's string.
 */
S_API s_str_list_element *s_str_list_element_delete(s_str_list_element *self,
													s_erc *error)
{
	s_str_list_element *el;


	S_CLR_ERR(error);

	el = s_list_element_delete(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_element_delete",
				  "Call to \"s_list_element_delete\" failed"))
		return NULL;

	return el;
}


/**
 * Return the next string list element in the string list, relative to
 * the given one.
 */
S_API const s_str_list_element *s_str_list_element_next(const s_str_list_element *self,
														s_erc *error)
{
	const s_str_list_element *el;


	S_CLR_ERR(error);

	el = s_list_element_next(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_element_next",
				  "Call to \"s_list_element_next\" failed"))
		return NULL;

	return el;
}


/**
 * Return the previous string list element in the string list,
 * relative to the given one.
 */
S_API const s_str_list_element *s_str_list_element_prev(const s_str_list_element *self,
														s_erc *error)
{
	const s_str_list_element *el;


	S_CLR_ERR(error);

	el = s_list_element_prev(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_element_prev",
				  "Call to \"s_list_element_prev\" failed"))
		return NULL;

	return el;
}


/**
 * Query if the string list has any elements.
 */
S_API s_bool s_str_list_isempty(const s_str_list *self, s_erc *error)
{
	s_bool awnser;


	S_CLR_ERR(error);

	awnser = s_list_isempty(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_isempty",
				  "Call to \"s_list_isempty\" failed"))
		return FALSE;

	return awnser;
}


/**
 * Get the number of elements in the string list.
 */
S_API uint32 s_str_list_size(const s_str_list *self, s_erc *error)
{
	uint32 size;


	S_CLR_ERR(error);

	size = s_list_size(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_size",
				  "Call to \"s_list_size\" failed"))
		return 0;

	return size;
}


/**
 * Push the given string onto end of given string list. Same as
 * #s_str_list_append.
 */
S_API void s_str_list_push(s_str_list *self, const char *string, s_erc *error)
{
	char *new_string;


	S_CLR_ERR(error);
	if ((self == NULL) || (string == NULL))
		return;

	new_string = s_strdup(string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_push",
				  "Call to \"s_strdup\" failed"))
		return;

	s_list_append(self, new_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_push",
				  "Call to \"s_list_append\" failed"))
	{
		S_FREE(new_string);
	}
}


/**
 * Pop last string of string list. Removes last string list element
 * from the string list and returns the string list element's string.
 */
S_API char *s_str_list_pop(s_str_list *self, s_erc *error)
{
	char *popped;

	S_CLR_ERR(error);

	popped = s_list_pop(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_pop",
				  "Call to \"s_list_pop\" failed"))
		return NULL;

	return popped;
}


/**
 * Reverse the order of the string list elements in the string list.
 */
S_API void s_str_list_reverse(s_str_list *self, s_erc *error)
{
	S_CLR_ERR(error);
	s_list_reverse(self, error);
	S_CHK_ERR(error, S_CONTERR,
			  "s_str_list_reverse",
			  "Call to \"s_list_reverse\" failed");
}


/**
 * Prepend a string to beginning of the string list.
 */
S_API void s_str_list_prepend(s_str_list *self, const char *string, s_erc *error)
{
	char *new_string;


	S_CLR_ERR(error);
	if ((self == NULL) || (string == NULL))
		return;

	new_string = s_strdup(string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_prepend",
				  "Call to \"s_strdup\" failed"))
		return;

	s_list_prepend(self, new_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_prepend",
				  "Call to \"s_list_prepend\" failed"))
	{
		S_FREE(new_string);
	}
}


/**
 * Append a string to end of the string list.
 */
S_API void s_str_list_append(s_str_list *self, const char *string, s_erc *error)
{
	char *new_string;


	S_CLR_ERR(error);
	if ((self == NULL) || (string == NULL))
		return;

	new_string = s_strdup(string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_append",
				  "Call to \"s_strdup\" failed"))
		return;

	s_list_append(self, new_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_append",
				  "Call to \"s_list_append\" failed"))
	{
		S_FREE(new_string);
	}
}


/**
 * Insert a string before given string list element. Return reference
 * to inserted string element.
 */
S_API const s_str_list_element *s_str_list_insert_before(s_str_list_element *self,
														 const char *string, s_erc *error)
{
	const s_str_list_element *el;
	char *new_string;


	S_CLR_ERR(error);
	if ((self == NULL) || (string == NULL))
		return NULL;

	new_string = s_strdup(string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_insert_before",
				  "Call to \"s_strdup\" failed"))
		return NULL;

	el = s_list_insert_before(self, new_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_insert_before",
				  "Call to \"s_list_insert_before\" failed"))
	{
		S_FREE(new_string);
		return NULL;
	}

	return el;
}


/**
 * Insert a string after given string list element. Return reference
 * to inserted string element.
 */
S_API const s_str_list_element *s_str_list_insert_after(s_str_list_element *self,
														const char *string, s_erc *error)
{
	const s_str_list_element *el;
	char *new_string;


	S_CLR_ERR(error);
	if ((self == NULL) || (string == NULL))
		return NULL;

	new_string = s_strdup(string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_insert_after",
				  "Call to \"s_strdup\" failed"))
		return NULL;

	el = s_list_insert_after(self, new_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_insert_after",
				  "Call to \"s_list_insert_after\" failed"))
	{
		S_FREE(new_string);
		return NULL;
	}

	return el;
}


/**
 * Return a newly allocated copy of the source string list, which must
 * later be freed by the caller.
 */
S_API s_str_list *s_str_list_dup(const s_str_list *src, s_erc *error)
{
	s_str_list *copy;

	S_CLR_ERR(error);
	copy = s_str_list_slice(src, 0, -1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_dup",
				  "Call to \"s_str_list_slice\" failed"))
		return NULL;

	return copy;
}


/**
 * Return a newly allocated slice of the given string list of the
 * source string list, which must later be freed by the caller.
 */
S_API s_str_list *s_str_list_slice(const s_str_list *src,
								   uint32 a, sint32 b, s_erc *error)
{
	s_str_list *copy;
	s_str_list_element *iterator;
	uint32 osize;
	uint32 c;


	S_CLR_ERR(error);

	if (src == NULL)
		return NULL;

	osize = s_str_list_size(src, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_slice",
				  "Call to \"s_str_list_size\" failed"))
		return NULL;

	if (a >= osize)
		return NULL;

	if (b == -1)
	{
		/* we want until the end */
		c = (uint32)osize;
	}
	else
	{
		c = (uint32)b;
	}


	copy = s_str_list_new(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_slice",
				  "Call to \"s_str_list_new\" failed"))
		return NULL;

	iterator = (s_str_list_element*)s_str_list_nth(src, a, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_slice",
				  "Call to \"s_str_list_nth\" failed"))
		goto error_return;

	while ((iterator != NULL) && (a++ < c))
	{
		const char *element;


		element = s_str_list_element_get(iterator, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_slice",
					  "Call to \"s_str_list_element_get\" failed"))
			goto error_return;

		s_str_list_append(copy, element, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_slice",
					  "Call to \"s_str_list_append\" failed"))
			goto error_return;

		iterator = (s_str_list_element*)s_str_list_element_next(iterator, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_slice",
					  "Call to \"s_str_list_element_next\" failed"))
			goto error_return;
	}

	/* OK, no error return */
	return copy;

	/* clean up after error condition */
error_return:
	{
		s_erc local_err = S_SUCCESS;


		s_str_list_delete(copy, &local_err);
		return NULL;
	}
}


/**
 * Compare the elements of two string lists.
 */
S_API sint32 s_str_list_cmp(const s_str_list *sl1, const s_str_list *sl2, s_erc *error)
{
	uint32 sl1_size;
	uint32 sl2_size;
	uint32 cnt;
	const s_str_list_element *se1;
	const s_str_list_element *se2;


	S_CLR_ERR(error);

	if ((sl1 == NULL) || (sl2 == NULL))
		return 0;

	sl1_size = s_str_list_size(sl1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_cmp",
				  "Call to \"s_str_list_size\" failed"))
		return 0;

	sl2_size = s_str_list_size(sl1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_cmp",
				  "Call to \"s_str_list_size\" failed"))
		return 0;

	se1 = s_str_list_first(sl1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_cmp",
				  "Call to \"s_str_list_first\" failed"))
		return 0;

	se2 = s_str_list_first(sl2, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_cmp",
				  "Call to \"s_str_list_first\" failed"))
		return 0;

	cnt = 0;

	while ((se1 != NULL) || (se2 != NULL))
	{
		const char *e1;
		const char *e2;
		int rv;


		e1 = s_str_list_element_get(se1, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_cmp",
					  "Call to \"s_str_list_element_get\" failed"))
			return 0;

		e2 = s_str_list_element_get(se2, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_cmp",
					  "Call to \"s_str_list_element_get\" failed"))
			return 0;

		rv = s_strcmp(e1, e2, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_cmp",
					  "Call to \"s_strcmp\" failed"))
			return 0;

		if (rv != 0)
			return (sint32)cnt;

		se1 = s_str_list_element_next(se1, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_cmp",
					  "Call to \"s_str_list_element_next\" failed"))
			return 0;

		se2 = s_str_list_element_next(se2, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_cmp",
					  "Call to \"s_str_list_element_next\" failed"))
			return 0;

		cnt++;
	}

	if ((sl1_size == sl2_size) && (cnt == sl1_size))
		return -1; /* lists are the same */

	return (sint32)cnt;
}


/**
 * Merge two string lists. Append all the string list elements of one
 * list onto another list.
 */
S_API void s_str_list_merge(s_str_list *self, const s_str_list *with,
							s_erc *error)
{
	const s_str_list_element *itr;


	S_CLR_ERR(error);

	if ((self == NULL) || (with == NULL))
		return;


	itr = s_str_list_first(with, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_merge",
				  "Call to \"s_str_list_first\" failed"))
		return;

	while (itr != NULL)
	{
		const char *element;


		element = s_str_list_element_get(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_merge",
					  "Call to \"s_str_list_element_get\" failed"))
			return;

		s_str_list_append(self, element, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_merge",
					  "Call to \"s_str_list_append\" failed"))
			return;

		itr = s_str_list_element_next(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_merge",
					  "Call to \"s_str_list_element_next\" failed"))
			return;
	}
}


S_API char *s_str_list_to_string(s_str_list *self, const char *sep,
								 s_erc *error)
{
	char *buf = NULL;
	const s_str_list_element *itr;


	S_CLR_ERR(error);
	if ((self == NULL) || (sep == NULL))
		return NULL;

	itr = s_str_list_first(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_to_string",
				  "Call to \"s_str_list_first\" failed"))
		goto error_return;

	while (itr != NULL)
	{
		const char *element;


		element = s_str_list_element_get(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_to_string",
					  "Call to \"s_str_list_element_get\" failed"))
			goto error_return;

		if (buf == NULL)
			s_asprintf(&buf, error, "%s", element);
		else
			s_asprintf(&buf, error, "%s%s%s", buf, sep, element);

		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_to_string",
					  "Call to \"s_asprintf\" failed"))
			goto error_return;

		itr = s_str_list_element_next(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_str_list_merge",
					  "Call to \"s_str_list_element_next\" failed"))
			goto error_return;
	}

	return buf;

error_return:
	if (buf != NULL)
		S_FREE(buf);

	return NULL;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/* required for s_list to compare two elements */
static s_bool s_str_list_compare_fp(const void *le1, const void *le2, s_erc *error)
{
	const char *str1 = le1;
	const char *str2 = le2;
	int rv;


	S_CLR_ERR(error);
	if ((str1 == NULL) || (str2 == NULL))
		return FALSE;

	rv = s_strcmp(str1, str2, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_str_list_compare_fp",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (rv == 0)
		return TRUE;

	return FALSE;
}


/* required for s_list to free an elements */
static void s_str_list_free_fp(void *le, s_erc *error)
{
	char *str = le;


	S_CLR_ERR(error);
	S_FREE(str);
}


