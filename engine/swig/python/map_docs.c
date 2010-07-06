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
/* Python documentation strings for SMap.                                           */
/*                                                                                  */
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
:return: The value associated with the given key.
:note: Raises ``KeyError`` if key not in map.
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


%define map_value_get_DOCSTRING
"""
value_get(key)

Get the raw ``SObject`` value in the map that is associated with the given key.
This function can be used in place of the normal::

    map['key']

The reason for this is that the above will try to convert the returned
value into a Python object if possible. Sometimes we want the value
as a Speect object. So instead we can use this function::

    map.value_get('key')

which will return the value object as it is in Speect, i.e. an ``SObject``.

:param key: The key of the key-value pair to get.
:type key: string
:return: The value associated with the given key or ``None`` if no such key-value pair.
:rtype: SObject
"""
%enddef

%feature("autodoc", map_value_get_DOCSTRING) SMap::value_get;


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

%feature("autodoc", map_iter_DOCSTRING) SMap::__iter__;


%define map_contains_DOCSTRING
"""
__contains__(key)

Query if named feature is present in this map.

:param key: The feature key which to test for.
:type key: string
:return: ``True`` or ``False``.
:rtype: bool
"""
%enddef

%feature("autodoc", map_contains_DOCSTRING) SMap::__contains__;
