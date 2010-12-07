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
/* SWIG common C convenience functions for SItem.                                   */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%nodefaultctor SItem;
%nodefaultdtor SItem;

typedef struct
{
} SItem;

%types(SItem = SObject);

%extend SItem
{
	void delete(s_erc *error)
	{
		S_FORCE_DELETE($self, "SItem:delete", error);
	}


	SItem *as_relation(const char *relname, s_erc *error)
	{
		SItem *itemAs;


		itemAs = SItemAs($self, relname, error);
		if (*error != S_SUCCESS)
			return NULL;

		return itemAs;
	}


	s_bool in_relation(const char *relname, s_erc *error)
	{
		s_bool item_in;


		item_in = SItemIn($self, relname, error);
		if (*error != S_SUCCESS)
			return FALSE;

		return item_in;
	}


	SItem *next(s_erc *error)
	{
		SItem *next;

		next = SItemNext($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return next;
	}


	SItem *prev(s_erc *error)
	{
		SItem *prev;


		prev = SItemPrev($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return prev;
	}


	SItem *append(const SItem *toShare=NULL, s_erc *error)
	{
		SItem *appended;


		appended = SItemAppend($self, toShare, error);
		if (*error != S_SUCCESS)
			return NULL;

		return appended;
	}


	SItem *prepend(const SItem *toShare=NULL, s_erc *error)
	{
		SItem *prepended;


		prepended = SItemPrepend($self, toShare, error);
		if (*error != S_SUCCESS)
			return NULL;

		return prepended;
	}


	SItem *parent(s_erc *error)
	{
		SItem *parent;


		parent = SItemParent($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return parent;
	}


	SItem *daughter(int nth=0, s_erc *error)
	{
		SItem *daughter;


		if (nth < -1)
		{
			SWIG_exception(SWIG_TypeError, "Input argument \"nth\" must be equal or bigger than -1");
			return NULL;
		}

		if (nth == 0)
		{
			daughter = SItemDaughter($self, error);
			if (*error != S_SUCCESS)
				return NULL;
		}
		else if (nth == -1)
		{
			daughter = SItemLastDaughter($self, error);
			if (*error != S_SUCCESS)
				return NULL;
		}
		else
		{
			daughter = SItemNthDaughter($self, nth, error);
			if (*error != S_SUCCESS)
				return NULL;
		}

		return daughter;

		/* for SWIG_exception */
	fail:
		return NULL;
	}


	SItem *add_daughter(const SItem *toShare=NULL, s_erc *error)
	{
		SItem *daughter;


		daughter = SItemAddDaughter($self, toShare, error);
		if (*error != S_SUCCESS)
			return NULL;

		return daughter;
	}


	const SRelation *relation(s_erc *error)
	{
		const SRelation *rel;

		rel = SItemRelation($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return rel;
	}


	const SUtterance *utterance(s_erc *error)
	{
		const SUtterance *utt;

		utt = SItemUtterance($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return utt;
	}


	const SVoice *voice(s_erc *error)
	{
		const SVoice *voice;

		voice = SItemVoice($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return voice;
	}


	SItem *path_to_item(const char *path, s_erc *error)
	{
		const SItem *item;


		item = s_path_to_item($self, path, error);
		if (*error != S_SUCCESS)
			return NULL;

		return S_ITEM(item);
	}


	const SObject *feature_get(const char *key, s_erc *error)
	{
		const SObject *feature;

		feature = SItemGetObject($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		return feature;
	}
}








