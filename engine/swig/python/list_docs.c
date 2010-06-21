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
/* Python documentation strings for SList.                                          */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%define list_DOCSTRING
"""
A list class, an abstract data structure that implements an ordered collection
of values (``SObject``), where the same value may occur more than once.
"""
%enddef

%feature("autodoc", list_DOCSTRING) SList;


%define list_getitem_DOCSTRING
"""
__getitem__(key)

Get the value in the map that is associated with the given key, where the key
is the index of the value in the list. Called to implement evaluation of::

    i = self[key]

:param key: The index of the value in the list.
:type key: integer
:return: The value associated with the given key (index).
:note: Indexing starts from 0.
"""
%enddef

%feature("autodoc", list_getitem_DOCSTRING) SList::__getitem__;


%define list_setitem_DOCSTRING
"""
__setitem__(key, value)

Set the given key-value pair in the list, where the key is an index to the
list. Called to implement assignment::

    self[key] = 10

:param key: The index of the value in the list to set.
:type key: int
:param value: The value of the associated index.
:note: The value at the given index will be deleted.
:note: Indexing starts from 0.
"""
%enddef

%feature("autodoc", list_setitem_DOCSTRING) SList::__setitem__;


%define list_delitem_DOCSTRING
"""
__delitem__(key)

Delete the value at the given key (index) in the list. Called to implement deletion::

    del self[key]

:param key: The index of the value in the list to delete.
:type key: int
:note: Indexing starts from 0.
"""
%enddef

%feature("autodoc", list_delitem_DOCSTRING) SList::__delitem__;


%define list_value_get_DOCSTRING
"""
value_get(key)

Get the raw ``SObject`` value in the list that is associated with the given key (index).
This function can be used in place of the normal::

    list['key']

The reason for this is that the above will try to convert the returned
value into a Python object if possible. Sometimes we want the value
as a Speect object. So instead we can use this function::

    list.value_get('key')

which will return the value object as it is in Speect, i.e. an ``SObject``.

:param key: The index of the value in the list to delete.
:type key: int
:return: The value at the given index.
:rtype: SObject
:note: Indexing starts from 0.
"""
%enddef

%feature("autodoc", list_value_get_DOCSTRING) SList::value_get;


%define list_len_DOCSTRING
"""
__len__()

Return the number of key-value pairs that are in the map. Called to implement the
built-in function::

  len()

:return: The number of values that are in the list.
:rtype: int
"""
%enddef

%feature("autodoc", list_len_DOCSTRING) SList::__len__;


%define list_iter_DOCSTRING
"""
__iter__()

The Python iterator protocol for iteration over values in a list.
"""
%enddef

%feature("autodoc", list_iter_DOCSTRING) SList::__iter__;

