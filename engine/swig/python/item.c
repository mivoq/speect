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

%define item_DOCSTRING
"""
A class for containing the items (arbitrary objects). Items are nodes in a
relation which is again linked to an utterance. Items contain a key-value
(string/object) set of features. An Item can have a list of daughter items as
well. Items can be shared between relations.
"""
%enddef

%feature("autodoc", item_DOCSTRING) SItem;


%define item_as_relation_DOCSTRING
"""
as_relation(relname)

Find the item in the given relation that has the same shared
content as this item.

:param relname: The relation name.
:return: This item as it is in the named relation, or ``None`` if this item does not share it's contents with any item in the named relation.
:rtype: SItem
"""
%enddef

%feature("autodoc", item_as_relation_DOCSTRING) SItem::as_relation;


%define item_in_relation_DOCSTRING
"""
in_relation(relname)

Query if this item is in the named relation.

:param relname: The relation name.
:return: ``True`` or ``False``.
:rtype: bool
"""
%enddef

%feature("autodoc", item_in_relation_DOCSTRING) SItem::in_relation;


%define item_eq_DOCSTRING
"""
__eq__(item)

Query if the given item equals this one. Equal meaning they
share the same contents.

:param item: The item to test against.
:type item: SItem
:return: ``True`` or ``False``.
:rtype: bool
"""
%enddef

%feature("autodoc", item_eq_DOCSTRING) SItem::__eq__;


%define item_next_DOCSTRING
"""
next()

Get the item next to this one in the current relation.

:return: The next item, or ``None`` if this is the last item in the relation.
:rtype: SItem
"""
%enddef

%feature("autodoc", item_next_DOCSTRING) SItem::next;


%define item_prev_DOCSTRING
"""
prev()

Get the item previous to this one in the current relation.

:return: The previous item, or ``None`` if this is the first item in the relation.
:rtype: SItem
"""
%enddef

%feature("autodoc", item_prev_DOCSTRING) SItem::prev;


%define item_append_DOCSTRING
"""
append([toShare = None])

Create a new item and append it after this one.

:param toShare: The item with which the newly created item will share it's content. If ``None`` then a new content will be created for the appended item.
:type toShare: SItem
:return: Newly created and appended item object.
:rtype: SItem
:raises: RuntimeError if Speect was unable to create the item.
"""
%enddef

%feature("autodoc", item_append_DOCSTRING) SItem::append;


%define item_prepend_DOCSTRING
"""
prepend([toShare = None])

Create a new item and prepend it after this one.

:param toShare: The item with which the newly created item will share it's content. If ``None`` then a new content will be created for the prepended item.
:type toShare: SItem
:return: Newly created and prepended item object.
:rtype: SItem
:raises: RuntimeError if Speect was unable to create the item.
"""
%enddef

%feature("autodoc", item_prepend_DOCSTRING) SItem::prepend;


%define item_parent_DOCSTRING
"""
parent()

Get this item's parent item.

:return: The parent item of this item, or ``None`` if no parent item.
:rtype: SItem
"""
%enddef

%feature("autodoc", item_parent_DOCSTRING) SItem::parent;


%define item_daughter_DOCSTRING
"""
daughter([nth = 0])


Get this item's daughter item.

:param nth: 0 for first daughter, -1 for last daughter and *nth* > 0 for *nth* daughter.
:type nth: int
:return: The *nth* daughter item of this item, or ``None`` if no daughter item.
:rtype: SItem
:raises: TypeError if *nth* < -1
"""
%enddef

%feature("autodoc", item_daughter_DOCSTRING) SItem::daughter;


%define item_add_daughter_DOCSTRING
"""
add_daughter([toShare = None])

Create a new item and add it as a daughter of this item.

:param toShare: The item with which the newly created item will share it's content. If ``None`` then a new content will be created for the daughter item.
:type toShare: SItem
:return: Newly created daughter item object.
:rtype: SItem
:raises: RuntimeError if Speect was unable to create the item.
"""
%enddef

%feature("autodoc", item_add_daughter_DOCSTRING) SItem::add_daughter;


%define item_relation_DOCSTRING
"""
relation()

Get this item's relation.

:return: This item's relation.
:rtype: SRelation
"""
%enddef

%feature("autodoc", item_relation_DOCSTRING) SItem::relation;


%define item_utterance_DOCSTRING
"""
utterance()

Get this item's utterance.

:return: This item's utterance, or ``None`` if no utterance has been set.
:rtype: SUtterance
"""
%enddef

%feature("autodoc", item_utterance_DOCSTRING) SItem::utterance;


%define item_voice_DOCSTRING
"""
voice()

Get this item's voice.

:return: This item's voice, or ``None`` if no voice has been set.
:rtype: SVoice
"""
%enddef

%feature("autodoc", item_voice_DOCSTRING) SItem::voice;


%define item_path_to_item_DOCSTRING
"""
path_to_item(path)

Get the item, from the given path, relative to this item.

:param path: The path to the desired item, relative to this item.
:type path: string
:return: The item from the path, relative to this item.
:rtype: SItem
"""
%enddef

%feature("autodoc", item_path_to_item_DOCSTRING) SItem::path_to_item;


%define item_path_to_feature_DOCSTRING
"""
path_to_feature(path)

Get the item feature, from the given path, relative to this item.

:param path: The path to the desired item's feature, relative to this item.
:type path: string
:return: The feature object from the path, relative to this item.
"""
%enddef

%feature("autodoc", item_path_to_feature_DOCSTRING) SItem::path_to_feature;


%define item_path_to_derived_feature_DOCSTRING
"""
path_to_derived_feature(path)

Execute the given *feature processor*, on the item from the
given path, relative to the given item. The name of the feature
processor is the last element of the path.

:param path: The path to the desired item, relative to this item, with the last element being the name of the feature processor.
:type path: string
:return: The extracted feature object from the path, relative to this item.
"""
%enddef

%feature("autodoc", item_path_to_derived_feature_DOCSTRING) SItem::path_to_derived_feature;


%define item_len_DOCSTRING
"""
__len__()

Get the number of features in this item.

:return: The number of features in this item.
:rtype: int
"""
%enddef

%feature("autodoc", item_len_DOCSTRING) SItem::__len__;


%define item_contains_DOCSTRING
"""
__contains__(feat_name)

Query if named feature is present in this item.

:param feat_name: The feature key which to test for.
:type feat_name: string
:return: ``True`` or ``False``.
:rtype: bool
"""
%enddef

%feature("autodoc", item_contains_DOCSTRING) SItem::__contains__;


%define item_getitem_DOCSTRING
"""
__getitem__(key)

Get the key-value feature pair.

:param key: The desired feature's key.
:type key: string
:return: The feature's value object, or ``None`` if no such key-value pair.
:rtype: SObject
"""
%enddef

%feature("autodoc", item_getitem_DOCSTRING) SItem::__getitem__;


%define item_setitem_DOCSTRING
"""
__setitem__(key, value)

Set the key-value feature pair.

:param key: The feature's key.
:type key: string
:param value: The feature's value object.
:type value: SObject
:note: An existing key-value pair in the item's features with the given key will be deleted.
"""
%enddef

%feature("autodoc", item_setitem_DOCSTRING) SItem::__setitem__;


%define item_delitem_DOCSTRING
"""
__delitem__(key)

Delete the key-value feature pair.

:param key: The feature's key.
:type key: string
"""
%enddef

%feature("autodoc", item_delitem_DOCSTRING) SItem::__delitem__;


%define item_iter_DOCSTRING
"""
__iter__()

The Python iterator protocol for iteration over item features.
"""
%enddef

%feature("autodoc", item_iter_DOCSTRING) SItem::__iter__;


%define item_feature_get_DOCSTRING
"""
feature_get(key)

Get the raw ``SObject`` feature of the item that is associated with the given key.
This function can be used in place of the normal::

    item['feature_name']

The reason for this is that the above will try to convert the returned
feature into a Python object if possible. Sometimes we want the feature
as a Speect object. So instead we can use this function::

    item.feature_get('feature_name')

which will return the feature object as it is in Speect, i.e. an ``SObject``.

:param key: The key of the key-value pair to get.
:type key: string
:return: The feature associated with the given key or ``None`` if no such key-value pair.
:rtype: SObject
"""
%enddef

%feature("autodoc", item_feature_get_DOCSTRING) SItem::feature_get;



typedef struct
{
} SItem;

%nodefaultctor SItem;

%types(SItem = SObject);

%extend SItem
{
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


	s_bool __eq__(const SItem *other, s_erc *error)
	{
		s_bool is_equal;


		is_equal = SItemEqual($self, other, error);
		if (*error != S_SUCCESS)
			return FALSE;

		return is_equal;
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
			PyErr_SetString(PyExc_TypeError, "Input argument \"nth\" must be equal or bigger than -1");
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


	PyObject *path_to_feature(const char *path, s_erc *error)
	{
		const SObject *sfeature;
		PyObject *pfeature;


		sfeature = s_path_to_feature($self, path, error);
		if (*error != S_SUCCESS)
			return NULL;

		pfeature = sobject_2_pyobject(sfeature, error, FALSE);
		if (*error != S_SUCCESS)
			return NULL;

		return pfeature;
	}


	PyObject *path_to_derived_feature(const char *path, s_erc *error)
	{
		SObject *sfeature;
		PyObject *pfeature;


		sfeature = s_path_to_featproc($self, path, error);
		if (*error != S_SUCCESS)
			return NULL;

		pfeature = sobject_2_pyobject(sfeature, error, TRUE);
		if (*error != S_SUCCESS)
		{
			S_DELETE(sfeature, "SItem::path_to_derived_feature()", error);
			return NULL;
		}

		return pfeature;
	}


	int __len__(s_erc *error)
	{
		size_t num_feats;


		num_feats = SMapSize($self->content->features, error);
		if (*error != S_SUCCESS)
			return 0;

		return (int)num_feats;
	}


	s_bool __contains__(const char *feat_name, s_erc *error)
	{
		s_bool is_present;


		is_present = SItemFeatureIsPresent($self, feat_name, error);
		if (*error != S_SUCCESS)
			return FALSE;

		return is_present;
	}


	const SObject *feature_get(const char *key, s_erc *error)
	{
		const SObject *feature;

		feature = SItemGetObject($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		return feature;
	}


	PyObject *__getitem__(const char *key, s_erc *error)
	{
		const SObject *feature;
		PyObject *object;


		feature = SItemGetObject($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		object = sobject_2_pyobject(feature, error, FALSE);
		if (*error != S_SUCCESS)
			return NULL;

		return object;
	}


	void __setitem__(const char *key, PyObject *val, s_erc *error)
	{
		SObject *newObject;


		newObject = pyobject_2_sobject(val, error);
		if (*error != S_SUCCESS)
			return;

		if (newObject == NULL)
			return;

		SItemSetObject($self, key, newObject, error);
		if (*error != S_SUCCESS)
			S_DELETE(newObject, "SItem::__setitem__", error);
	}


	void __delitem__(const char *key, s_erc *error)
	{
		s_bool is_present;


		is_present = SItemFeatureIsPresent($self, key, error);
		if (*error != S_SUCCESS)
			return;

		if (!is_present)
			return;

		SItemDelFeature($self, key, error);
	}


	PMapIterator *__iter__()
	{
		PMapIterator *pitr;
		SIterator *itr;
		s_erc error;


		S_CLR_ERR(&error);
		itr = SMapIterator($self->content->features, &error);
		if (error != S_SUCCESS)
			return NULL;

		itr = SIteratorFirst(itr);
		pitr = make_PMapIterator(itr, &error);
		if (error != S_SUCCESS)
			return NULL;

		return pitr;
	}

%pythoncode
%{
def __str__(self):
    """
    Get a string representation of the item.

    :return: A string representation of the item.
    :rtype: string
    """
    return self.to_string()


def to_string(self, prefix="", label="Item"):
    # helper function
    label_spacing = ''
    for s in range(0, len(label)):
        label_spacing += ' '

    num_features = len(self)
    if num_features > 0:
        stri = "%s%s: [ " %(prefix,label)

        first = True
        count = num_features

        for ik in self:
            if not first:
                stri += '\n%s%s    ' %(prefix, label_spacing)
            first = False
            count -= 1
            if count != 0:
                stri += '%15.15s => %s,' %(ik, repr(self[ik]))
            else:
                stri += '%15.15s => %s' %(ik, repr(self[ik]))

        if  num_features > 0:
            stri += '    ]\n'

    else:
        stri = ""

    i = self.daughter()
    n = 0
    daughter_prefix = '%s    ' %prefix
    while i:
        stri += '%s' %i.to_string(prefix=daughter_prefix,
                                 label='Daughter')
        n += 1
        i = self.daughter(n)

    return stri
%}
}








