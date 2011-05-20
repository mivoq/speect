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
/* Python documentation strings for SUtterance.                                     */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%define utterance_ctor_DOCSTRING
"""
SUtterance([voice = None])

The Utterance class. An Utterance consists of a set of relations,
which in turn consists of a set of items.

Initialize a newly created utterance for the given voice.

:param voice: The voice associated with this utterance.
:type voice: SVoice
:return: Newly created utterance object
:rtype: SUtterance
:raises: RuntimeError if Speect was unable to create the utterance.
"""
%enddef

%feature("autodoc", utterance_ctor_DOCSTRING) SUtterance;


%define utterance_relation_new_DOCSTRING
"""
relation_new(name)

Create a new named relation in the utterance.  Creates a
relation, initializes it, and sets it in the utterance.


:param name: The name of the new relation to create.
:type name: string
:return: Newly created relation object
:rtype: SRelation
:raises: RuntimeError if Speect was unable to create the relation.
:note: If a relation with the given name already exists in the utterance, then it will be deleted.
"""
%enddef

%feature("autodoc", utterance_relation_new_DOCSTRING) SUtterance::relation_new;


%define utterance_relation_get_DOCSTRING
"""
relation_get(name)

Get the named relation from the utterance.


:param name: The name of the relation to get.
:type name: string
:return: Named relation, or ``None`` if such a relation does not exist in the utterance.
:rtype: SRelation
"""
%enddef

%feature("autodoc", utterance_relation_get_DOCSTRING) SUtterance::relation_get;


%define utterance_relation_set_DOCSTRING
"""
relation_set(rel)

Set the given relation in the utterance.


:param rel: The relation to set in the utterance.
:type name: SRelation
"""
%enddef

%feature("autodoc", utterance_relation_set_DOCSTRING) SUtterance::relation_set;


%define utterance_relation_del_DOCSTRING
"""
relation_del(name)

Delete the relation with the given name from the utterance.


:param name: The relation to delete from the utterance.
:type name: string
"""
%enddef

%feature("autodoc", utterance_relation_del_DOCSTRING) SUtterance::relation_del;


%define utterance_contains_DOCSTRING
"""
__contains__(name)

Test if the utterance contains the named relation.


:param name: The relation name to test for.
:type name: string
:return: ``True`` or ``False``.
:rtype: bool
"""
%enddef

%feature("autodoc", utterance_contains_DOCSTRING) SUtterance::__contains__;


%define utterance_iter_DOCSTRING
"""
__iter__()

The Python iterator protocol for iteration over relation names in an utterance.
"""
%enddef

%feature("autodoc", utterance_iter_DOCSTRING) SUtterance::__iter__;


%define utterance_voice_DOCSTRING
"""
Get the voice that is associated with the utterance.

:return: The voice that is associated with the utterance, or ``None`` if the utterance does not have a defined voice.
:rtype: SVoice
"""
%enddef

%feature("autodoc", utterance_voice_DOCSTRING) voice;


%define utterance_features_DOCSTRING
"""
Get the features that are defined for the utterance.

:return: A map of the utterance features.
:rtype: SMap
"""
%enddef

%feature("autodoc", utterance_features_DOCSTRING) features;
