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
/************************************************************************************/

%define map_DOCSTRING
"""
A map class, an abstract data type composed of a collection of unique keys (strings) and a
collection of values (``SObject``), where each key is associated with one value.
"""
%enddef

%feature("autodoc", map_DOCSTRING) SMap;


%define map_getitem_DOCSTRING
"""
__getitem__(key)

Get the value in the map that is associated with the given key.

:param key: The key of the desired value.
:type key: string
:return: The value associated with the given key or ``None`` if no such key-value pair.
"""
%enddef

%feature("autodoc", map_getitem_DOCSTRING) SMap::__getitem__;


%define map_setitem_DOCSTRING
"""
__setitem__(key, value)

Set the given key-value pair in the map.

:param key: The key of the value.
:type key: string
:param value: The value of the associated key.
:note: If a key-value pair with the given key already exists, it will be deleted.
"""
%enddef

%feature("autodoc", map_setitem_DOCSTRING) SMap::__setitem__;


%define map_delitem_DOCSTRING
"""
__delitem__(key)

Delete the key-value pair of the given in the map.

:param key: The key of the key-value pair to delete.
:type key: string
"""
%enddef

%feature("autodoc", map_delitem_DOCSTRING) SMap::__delitem__;


%define map_len_DOCSTRING
"""
__len__()

Return the number of key-value pairs that are in the map.

:return: The number of key-value pairs that are in the map.
:rtype: int
"""
%enddef

%feature("autodoc", map_len_DOCSTRING) SMap::__len__;


%define map_iter_DOCSTRING
"""
__iter__()

The Python iterator protocol for iteration over keys in a map.
"""
%enddef

%feature("autodoc", utterance_iter_DOCSTRING) SMap::__iter__;


typedef struct
{
} SMap;


%types(SMap = SObject);

%nodefaultctor SMap;


%extend SMap
{
	PyObject *__getitem__(const char *key, s_erc *error)
	{
		const SObject *mapObject;
		PyObject *object;


		mapObject = SMapGetObjectDef($self, key, NULL, error);
		if (*error != S_SUCCESS)
			return NULL;

		object = sobject_2_pyobject(mapObject, error, FALSE);
		if (*error != S_SUCCESS)
			return NULL;

		return object;
	}


	int __len__(s_erc *error)
	{
		size_t num_feats;


		num_feats = SMapSize($self, error);
		if (*error != S_SUCCESS)
			return 0;

		return (int)num_feats;
	}


	void __setitem__(const char *key, PyObject *val, s_erc *error)
	{
		SObject *newObject;


		newObject = pyobject_2_sobject(val, error);
		if (*error != S_SUCCESS)
			return;

		if (newObject == NULL)
			return;

		SMapSetObject($self, key, newObject, error);
		if (*error != S_SUCCESS)
			S_DELETE(newObject, "SMap::__setitem__", error);
	}


	void __delitem__(const char *key, s_erc *error)
	{
		s_bool is_present;


		is_present = SMapObjectPresent($self, key, error);
		if (*error != S_SUCCESS)
			return;

		if (!is_present)
			return;

		SMapObjectDelete($self, key, error);
	}


	PMapIterator *__iter__()
	{
		PMapIterator *pitr;
		SIterator *itr;
		s_erc error;


		S_CLR_ERR(&error);
		itr = SMapIterator($self, &error);
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
    Return a string representation of the key-value pairs that are in the map.

    :return: A string representation of the key-value pairs that are in the map.
    :rtype: string
    """

    num_features = len(self)
    if num_features > 0:
        stri = "{ "

        first = True
        count = num_features

        for ik in self:
            if not first:
                stri += '\n    '
            first = False
            count -= 1
            if count != 0:
                stri += '%s : %s,' %(ik, repr(self[ik]))
            else:
                stri += '%s : %s' %(ik, repr(self[ik]))

        if  num_features > 0:
            stri += '    }\n'

    else:
        stri = "{}"

    return stri
%}


};

