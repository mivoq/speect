/************************************************************************************/
/* Copyright (c) 2010 The Department of Arts and Culture,                           */
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
/* DATE    : June 2010                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Hash table example.                                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include <string.h>
#include "speect.h"


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/* a simple key-value structure */
typedef struct
{
	const char *key;
	int         val;
} key_val;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* test data */
static const key_val test_data[] =
{
	{ "leonardo", 20 },
	{ "cleo", 12 },
	{ "karna", 2 },
	{ "marco", 56 },
	{ "rudi", 3 },
	{ "klara", 32 },
	{ "wilco", 78 },
	{ "draco", 1 },
	{ "leo", 43 },
	{ "qwerty", 19 },
	{ NULL, 0 }
};


/************************************************************************************/
/*                                                                                  */
/* Main function implementation                                                     */
/*                                                                                  */
/************************************************************************************/

int main()
{
	s_erc error = S_SUCCESS; /* start of with a clean slate */
	s_hash_table *ht_ptr = NULL;
	const key_val *itr;
	size_t size;
	const s_hash_element *el;
	int counter;
	char *stats;


	/* initialize speect */
	error = speect_init(NULL);
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}

	/* create new hash table, no free function
	 * initial size is 6 (2^3) and we have 10 data elements, so
	 * hash table will grow (note that this is relatively slow).
	 */
	ht_ptr = s_hash_table_new(NULL, 3, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_hash_table_new\" failed"))
		goto exit_error;

	/*
	 * add data, note that the key and value data elements can
	 * be of any type, but keys must be unique.
	 */
	for (itr = test_data; itr->key != NULL; itr++)
	{
		size = s_strsize(itr->key, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_strsize\" failed"))
			goto exit_error;

		s_hash_table_add(ht_ptr,
						 (void*)itr->key, size, (void*)&(itr->val),
						 &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_hash_table_add\" failed"))
			goto exit_error;
	}

	/*
	 * iterate through hash table elements
	 */
	el = s_hash_table_first(ht_ptr, &error);  /* get first element */
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_hash_table_first\" failed"))
		goto exit_error;

	counter = 0;
	printf("#\tkey\t\tvalue\n");
	while (el != NULL)
	{
		const char *key;
		const int *val;


		/* get hash element key */
		key = s_hash_element_key(el, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_hash_element_key\" failed"))
			goto exit_error;

		printf("%d\t%s\t\t", counter++, key);

		/* get hash element value */
		val = s_hash_element_get_data(el, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_hash_element_get_data\" failed"))
			goto exit_error;

		printf("%d\n", *val);

		el = s_hash_element_next(el, &error);  /* get next element */
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_hash_element_next\" failed"))
			goto exit_error;
	}
	printf("\n");


	/* get table size (number of elements in table) */
	size = s_hash_table_size(ht_ptr, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_hash_table_size\" failed"))
		goto exit_error;

	printf("the hash table contains %d elements\n\n", size);

	/* get some statistics and info on the table and it's elements */
	stats = s_hash_table_stats(ht_ptr, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_hash_table_stats\" failed"))
		goto exit_error;

	if (stats != NULL)
	{
		printf("%s\n", stats);
		S_FREE(stats);
	}

	/* resize hash table to 2^8 */
	s_hash_table_resize(ht_ptr, 8, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_hash_table_resize\" failed"))
		goto exit_error;

	/* get some statistics and info on the table and it's elements */
	stats = s_hash_table_stats(ht_ptr, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_hash_table_stats\" failed"))
		goto exit_error;

	if (stats != NULL)
	{
		printf("%s\n", stats);
		S_FREE(stats);
	}

	/* resize hash table to minimum allowed size */
	s_hash_table_resize(ht_ptr, -1, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_hash_table_resize\" failed"))
		goto exit_error;

	/* get some statistics and info on the table and it's elements */
	stats = s_hash_table_stats(ht_ptr, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_hash_table_stats\" failed"))
		goto exit_error;

	if (stats != NULL)
	{
		printf("%s\n", stats);
		S_FREE(stats);
	}

	/*
	 * iterate through hash table elements
	 */
	el = s_hash_table_first(ht_ptr, &error);  /* get first element */
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_hash_table_first\" failed"))
		goto exit_error;

	counter = 0;
	printf("#\tkey\t\tvalue\n");
	while (el != NULL)
	{
		const char *key;
		const int *val;


		/* get hash element key */
		key = s_hash_element_key(el, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_hash_element_key\" failed"))
			goto exit_error;

		printf("%d\t%s\t\t", counter++, key);

		/* get hash element value */
		val = s_hash_element_get_data(el, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_hash_element_get_data\" failed"))
			goto exit_error;

		printf("%d\n", *val);

		el = s_hash_element_next(el, &error);  /* get next element */
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_hash_element_next\" failed"))
			goto exit_error;
	}
	printf("\n");

	/* delete the hash table */
	s_hash_table_delete(ht_ptr, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_hash_table_delete\" failed"))
		goto exit_error;

	/* quit speect */
	error = speect_quit();
	if (error != S_SUCCESS)
	{
		printf("Call to 'speect_quit' failed\n");
		return 1;
	}

	return 0;

exit_error:
	if (ht_ptr != NULL)
	{
		s_erc local_err = S_SUCCESS;


		/* we use a local error here because we already have an set
		 * error by the time we reach this point, and
		 * s_hash_table_delete will clear it.
		 */
		s_hash_table_delete(ht_ptr, &local_err);
	}

	error = speect_quit();
	if (error != S_SUCCESS)
		printf("Call to 'speect_quit' failed\n");

	return 1;
}

