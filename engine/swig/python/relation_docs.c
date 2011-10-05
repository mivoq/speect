/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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
/* Python documentation strings for SRelation.                                      */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%define relation_ctor_DOCSTRING
"""
SRelation(utt, name)

The Relation class. A Relation is a named list of items. An Utterance
can hold an arbitrary number of Relations.

Initialize a newly created relation with the given name, and set it in
the given utterance.


:param utt: The utterance that the newly created relation should be placed in.
:type utt: :class:`SUtterance`
:param name: The name of the newly created relation.
:type name: string
:return: Newly created relation object.
:rtype: :class:`SRelation`
:raises: RuntimeError if Speect was unable to create the utterance.
:note: Parameter *utt* may be ``None``.
:note: A relation with the same name that already exists in the utterance will be deleted.
"""
%enddef

%feature("autodoc", relation_ctor_DOCSTRING) SRelation;


%define relation_append_DOCSTRING
"""
append([toShare = None])

Create a new item and append it to the end of the items
in the relation.


:param toShare: The item with which the newly created item will share it's content. If ``None`` then a new content will be created for the appended item.
:type toShare: :class:`SItem`
:return: Newly created and appended item object.
:rtype: :class:`SItem`
:raises: RuntimeError if Speect was unable to create the item.
"""
%enddef

%feature("autodoc", relation_append_DOCSTRING) SRelation::append;


%define relation_prepend_DOCSTRING
"""
prepend([toShare = None])

Create a new item and prepend it to the beginning of the items
in the relation.


:param toShare: The item with which the newly created item will share it's content. If ``None`` then a new content will be created for the prepended item.
:type toShare: :class:`SItem`
:return: Newly created and prepended item object.
:rtype: :class:`SItem`
:raises: RuntimeError if Speect was unable to create the item.
"""
%enddef

%feature("autodoc", relation_prepend_DOCSTRING) SRelation::prepend;


%define relation_head_DOCSTRING
"""
head()

Get the first item in the relation.

:return: The first item in the relation or ``None`` if the relation is empty.
:rtype: :class:`SItem`
"""
%enddef

%feature("autodoc", relation_head_DOCSTRING) SRelation::head;


%define relation_tail_DOCSTRING
"""
tail()

Get the last item in the relation.

:return: The last item in the relation or ``None`` if the relation is empty.
:rtype: :class:`SItem`
"""
%enddef

%feature("autodoc", relation_tail_DOCSTRING) SRelation::tail;


%define relation_name_DOCSTRING
"""
name()

Get the relation's name.

:return: The relation's name.
:rtype: string
"""
%enddef

%feature("autodoc", relation_name_DOCSTRING) SRelation::name;


%define relation_utt_DOCSTRING
"""
utterance()

Get the relation's utterance.

:return: The relation's utterance or ``None`` if no utterance was set.
:rtype: :class:`SUtterance`
"""
%enddef

%feature("autodoc", relation_utt_DOCSTRING) SRelation::utterance;


%define relation_iter_DOCSTRING
"""
__iter__()

The Python iterator protocol for iteration over items in a relation.
"""
%enddef

%feature("autodoc", relation_iter_DOCSTRING) SRelation::__iter__;
