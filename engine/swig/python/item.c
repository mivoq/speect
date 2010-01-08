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
/* C convenience functions for SItem Python wrapper.                                */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	SObject *py_sitem_new(const SObject *rel, const SObject *shared)
	{
		s_erc rv = S_SUCCESS;
		SItem *newItem;


		newItem = (SItem*)S_NEW("SItem", &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to create new SItem");
			return NULL;
		}

		SItemInit(&newItem, S_RELATION(rel), S_ITEM(shared), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to initialize new SItem");
			return NULL;
		}

		return S_OBJECT(newItem);
	}


	SObject *py_sitem_as(const SObject *self, const char *relname)
	{
		s_erc rv = S_SUCCESS;
		SItem *itemAs;


		itemAs = SItemAs(S_ITEM(self), relname, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_Format(PyExc_RuntimeError, "Failed to get item as in relation '%s'", relname);
			return NULL;
		}

		return S_OBJECT(itemAs);
	}


	s_bool py_sitem_in(const SObject *self, const char *relname)
	{
		s_erc rv = S_SUCCESS;
		s_bool item_in;


		item_in = SItemIn(S_ITEM(self), relname, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_Format(PyExc_RuntimeError, "Failed to query if item is in relation '%s'", relname);
			return FALSE;
		}

		return item_in;
	}


	SObject *py_sitem_next(const SObject *self, s_erc * tmp)
	{
		s_erc rv = S_SUCCESS;
		SItem *next;

		next = SItemNext(S_ITEM(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get next item");
			return NULL;
		}

		return S_OBJECT(next);
	}


	SObject *py_sitem_prev(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		SItem *prev;


		prev = SItemPrev(S_ITEM(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get previous item");
			return NULL;
		}

		return S_OBJECT(prev);
	}


	SObject *py_sitem_append(SObject *self, const SObject *toShare)
	{
		s_erc rv = S_SUCCESS;
		SItem *appended;


		appended = SItemAppend(S_ITEM(self), S_ITEM(toShare), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to append item");
			return NULL;
		}

		return S_OBJECT(appended);
	}


	SObject *py_sitem_prepend(SObject *self, const SObject *toShare)
	{
		s_erc rv = S_SUCCESS;
		SItem *prepended;


		prepended = SItemPrepend(S_ITEM(self), S_ITEM(toShare), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to prepend item");
			return NULL;
		}

		return S_OBJECT(prepended);
	}


	SObject *py_sitem_parent(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		SItem *parent;


		parent = SItemParent(S_ITEM(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get parent item");
			return NULL;
		}

		return S_OBJECT(parent);
	}


	SObject *py_sitem_daughter(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		SItem *daughter;


		daughter = SItemDaughter(S_ITEM(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get daughter item");
			return NULL;
		}

		return S_OBJECT(daughter);
	}


	SObject *py_sitem_nth_daughter(const SObject *self, uint32 n)
	{
		s_erc rv = S_SUCCESS;
		SItem *daughter;


		daughter = SItemNthDaughter(S_ITEM(self), n, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get nth daughter item");
			return NULL;
		}

		return S_OBJECT(daughter);
	}


	SObject *py_sitem_last_daughter(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		SItem *daughter;


		daughter = SItemLastDaughter(S_ITEM(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get last daughter item");
			return NULL;
		}

		return S_OBJECT(daughter);
	}


	SObject *py_sitem_add_daughter(const SObject *self, const SObject *toShare)
	{
		s_erc rv = S_SUCCESS;
		SItem *daughter;


		daughter = SItemAddDaughter(S_ITEM(self), S_ITEM(toShare), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to add daughter item");
			return NULL;
		}

		return S_OBJECT(daughter);
	}


	s_bool py_sitem_feat_present(const SObject *self, const char *feat_name)
	{
		s_erc rv = S_SUCCESS;
		s_bool is_present;


		is_present = SItemFeatureIsPresent(S_ITEM(self), feat_name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to query feature presence");
			return FALSE;
		}

		return is_present;
	}


	void py_sitem_del_feature(SObject *self, const char *feat_name)
	{
		s_erc rv = S_SUCCESS;


		SItemDelFeature(S_ITEM(self), feat_name, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to remove feature");
	}


	const SObject *py_sitem_get_feature(const SObject *self, const char *featname)
	{
		s_erc rv = S_SUCCESS;
		const SObject *feature;


		feature = SItemGetObject(S_ITEM(self), featname, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_Format(PyExc_RuntimeError, "Failed to get item feature '%s'", featname);
			return NULL;
		}

		return S_OBJECT(feature);
	}


	void py_sitem_set_feature(SObject *self, const char *featname, const SObject *feat)
	{
		s_erc rv = S_SUCCESS;


		SItemSetObject(S_ITEM(self), featname, feat, &rv);
		if (rv != S_SUCCESS)
			PyErr_Format(PyExc_RuntimeError, "Failed to set item feature '%s'", featname);
	}


	const SObject *py_sitem_relation(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		const SRelation *rel;

		rel = SItemRelation(S_ITEM(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get item's relation");
			return NULL;
		}

		return S_OBJECT(rel);
	}


	const SObject *py_sitem_utterance(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		const SUtterance *utt;

		utt = SItemUtterance(S_ITEM(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get item's utterance");
			return NULL;
		}

		return S_OBJECT(utt);
	}


	const SObject *py_sitem_voice(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		const SVoice *voice;

		voice = SItemVoice(S_ITEM(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get item's voice");
			return NULL;
		}

		return S_OBJECT(voice);
	}


	const SObject *py_sitem_features(SObject *self)
	{
		SItem *item = S_ITEM(self);


		if (item == NULL)
			return NULL;

		return (const SObject*)item->content->features;
	}


	SObject *py_sitem_path_to_item(const SObject *self, const char *path)
	{
		s_erc rv = S_SUCCESS;
		const SItem *item;


		item = s_path_to_item(S_ITEM(self), path, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get item from path");
			return NULL;
		}

		return S_OBJECT(item);
	}


	const SObject *py_sitem_path_to_feature(const SObject *self, const char *path)
	{
		s_erc rv = S_SUCCESS;
		const SObject *feature;


		feature = s_path_to_feature(S_ITEM(self), path, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get item feature from path");
			return NULL;
		}

		return feature;
	}


	SObject *py_sitem_path_to_extracted_feature(const SObject *self, const char *path)
	{
		s_erc rv = S_SUCCESS;
		SObject *feature;


		feature = s_path_to_featproc(S_ITEM(self), path, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get extract item feature from path");
			return NULL;
		}

		return feature;
	}

/*
 * Do not delete this delimiter, required for SWIG
 */
%}





