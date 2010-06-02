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
/* C convenience functions for SRelation Python wrapper.                            */
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
:type utt: SUtterance
:param name: The name of the newly created relation.
:type name: string
:return: Newly created relation object.
:rtype: SRelation
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
:type toShare: SItem
:return: Newly created and appended item object.
:rtype: SItem
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
:type toShare: SItem
:return: Newly created and prepended item object.
:rtype: SItem
:raises: RuntimeError if Speect was unable to create the item.
"""
%enddef

%feature("autodoc", relation_prepend_DOCSTRING) SRelation::prepend;


%define relation_head_DOCSTRING
"""
head()

Get the first item in the relation.

:return: The first item in the relation or ``None`` if the relation is empty.
:rtype: SItem
"""
%enddef

%feature("autodoc", relation_head_DOCSTRING) SRelation::head;


%define relation_tail_DOCSTRING
"""
tail()

Get the last item in the relation.

:return: The last item in the relation or ``None`` if the relation is empty.
:rtype: SItem
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
:rtype: SUtterance
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


typedef struct
{
} SRelation;


%types(SRelation = SObject);

%nodefaultdtor SRelation;

%extend SRelation
{
	SRelation(const SUtterance *utt, const char *name, s_erc *error)
	{
		SRelation *rel;


		rel = (SRelation*)S_NEW("SRelation", error);
		if (*error != S_SUCCESS)
			return NULL;

		SRelationInit(&rel, name, error);
		if (*error != S_SUCCESS)
			return NULL;

		if (utt != NULL)
		{
			SUtteranceSetRelation(S_UTTERANCE(utt), rel, error);
			if (*error != S_SUCCESS)
			{
				S_FORCE_DELETE(rel, "SRelation", error);
				return NULL;
			}
		}

		return rel;
	}


	const SItem *append(const SItem *toShare=NULL, s_erc *error)
	{
		const SItem *appended;


		appended = SRelationAppend($self, toShare, error);
		if (*error != S_SUCCESS)
			return NULL;

		return appended;
	}


	const SItem *prepend(const SItem *toShare=NULL, s_erc *error)
	{
		const SItem *prepended;


		prepended = SRelationPrepend($self, toShare, error);
		if (*error != S_SUCCESS)
			return NULL;

		return prepended;
	}


	const SItem *head(s_erc *error)
	{
		const SItem *item;


		item = SRelationHead($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return item;
	}


	const SItem *tail(s_erc *error)
	{
		const SItem *item;


		item = SRelationTail($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return item;
	}


	const char *name(s_erc *error)
	{
		const char *name;


		name = SRelationName($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return name;
	}


	const SUtterance *utterance(s_erc *error)
	{
		const SUtterance *utt;


		utt = SRelationUtterance($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return utt;
	}

	SRelationItr *__iter__()
	{
		SRelationItr *pitr;
		s_erc error;


		S_CLR_ERR(&error);
		pitr = make_SRelationItr($self, &error);
		if (error != S_SUCCESS)
			return NULL;

		return pitr;
	}

%pythoncode
%{
def __str__(self):
    """
    Get a string representation of the relation.

    :return: A string representation of the relation.
    :rtype: string
    """
    return self.to_string()


def to_string(self, prefix=""):
    # helper function
    stri = "%sRelation \'%s\':\n" %(prefix, self.name())
    item_prefix = "%s    " %prefix
    for i in self:
        stri += i.to_string(prefix=item_prefix)

    return stri
%}
}
