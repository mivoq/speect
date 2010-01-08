/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* DATE    : November 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SMap Python wrapper.                                 */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	/*
	 * SMap iterator functions.
	 */
	SObject *py_smap_iterator_new(const SObject *map)
	{
		s_erc rv = S_SUCCESS;
		SIterator *itr;
		SMap *self = S_MAP(map);


		itr = SMapIterator(self, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get map iterator");
			return NULL;
		}

		return S_OBJECT(itr);
	}


	const char *py_smap_iterator_get_item(const SObject *itr)
	{
		s_erc rv = S_SUCCESS;
		const char *key;
		SIterator *iterator = (SIterator*)itr;


		key = SMapIteratorKey(iterator, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get map iterator key");
			return NULL;
		}

		return key;
	}


	SObject *py_smap_new(void)
	{
		s_erc rv = S_SUCCESS;
		SMap *newMap;


		newMap = S_MAP(S_NEW("SMapList", &rv));
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to create new map");
			return NULL;
		}

		SMapListInit(&newMap, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to initialize new map");
			return NULL;
		}

		return S_OBJECT(newMap);
	}


	uint32 py_smap_len(const SObject *map)
	{
		s_erc rv = S_SUCCESS;
		uint32 len = 0;
		const SMap *self = S_MAP(map);


		len = (uint32)SMapSize(self, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to determine "
							"map length");
			return 0;
		}

		return len;
	}


	s_bool py_smap_key_present(const SObject *map, const char *key)
	{
		s_erc rv = S_SUCCESS;
		const SMap *self = S_MAP(map);
		s_bool key_present;


		key_present = SMapObjectPresent(self, key, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to determine "
							"if key is present in map");
			return FALSE;
		}

		return key_present;
	}


	const SObject *py_smap_get_item(const SObject *map, const char *key)
	{
		s_erc rv = S_SUCCESS;
		const SMap *self = S_MAP(map);
		const SObject *item;


		item = SMapGetObject(self, key, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get item "
							"from map");
			return NULL;
		}

		return item;
	}


	void py_smap_set_item(SObject *map, const char *key, const SObject *item)
	{
		s_erc rv = S_SUCCESS;
		SMap *self = S_MAP(map);


		SMapSetObject(self, key, item, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to set item "
							"in map");
	}


	void py_smap_del_item(SObject *map, const char *key)
	{
		s_erc rv = S_SUCCESS;
		SMap *self = S_MAP(map);


		SMapObjectDelete(self, key, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to delete item "
							"from map");
	}


	SObject *py_smap_get_keys(const SObject *map)
	{
		s_erc rv = S_SUCCESS;
		SMap *self = S_MAP(map);
		SList *keys;


		keys = SMapGetKeys(self, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get the key "
							"list from the map");
			return NULL;
		}

		return S_OBJECT(keys);
	}


	SObject *py_smap_unlink_item(const SObject *map, const char *key)
	{
		s_erc rv = S_SUCCESS;
		SMap *self = S_MAP(map);
		SObject *item;


		item = SMapObjectUnlink(self, key, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to unlink key "
							"from the map");
			return NULL;
		}

		return item;
	}


	SObject *py_smap_copy(const SObject *map)
	{
		s_erc rv = S_SUCCESS;
		const SMap *self = S_MAP(map);
		SMap *newMap = NULL;


		newMap = SMapCopy(NULL, self, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to copy map");
			return NULL;
		}

		return S_OBJECT(newMap);
	}

/*
 * Do not delete this delimiter, required for SWIG
 */
%}




