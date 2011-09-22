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


%extend SItem
{
	s_bool __eq__(const SItem *other, s_erc *error)
	{
		s_bool is_equal;


		is_equal = SItemEqual($self, other, error);
		if (*error != S_SUCCESS)
			return FALSE;

		return is_equal;
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


	PyObject *path_to_feature(const char *path, s_erc *error)
	{
		const SObject *sfeature;
		PyObject *pfeature;


		sfeature = s_path_to_feature($self, path, error);
		if (*error != S_SUCCESS)
			return NULL;

		pfeature = s_sobject_2_pyobject(sfeature, FALSE, error);
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

		pfeature = s_sobject_2_pyobject(sfeature, TRUE, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(sfeature, "SItem::path_to_derived_feature()", error);
			return NULL;
		}

		return pfeature;
	}


	PyObject *path(const char *path, s_erc *error)
	{
		SObject *sfeature;
		PyObject *pfeature;


		sfeature = SItemPath($self, path, error);
		if (*error != S_SUCCESS)
			return NULL;

		pfeature = s_sobject_2_pyobject(sfeature, TRUE, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(sfeature, "SItem::path_to_derived_feature()", error);
			return NULL;
		}

		return pfeature;
	}


	PyObject *__getitem__(const char *key, s_erc *error)
	{
		const SObject *feature;
		PyObject *object;
		s_bool present;


		present = SItemFeatureIsPresent($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		if (!present)
			Py_RETURN_NONE;

		feature = SItemGetObject($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		object = s_sobject_2_pyobject(feature, FALSE, error);
		if (*error != S_SUCCESS)
			return NULL;

		return object;
	}


	void __setitem__(const char *key, PyObject *val, s_erc *error)
	{
		SObject *newObject;


		newObject = s_pyobject_2_sobject(val, error);
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


	_PMapIterator *__iter__()
	{
		_PMapIterator *pitr;
		SIterator *itr;
		s_erc error;


		S_CLR_ERR(&error);
		itr = S_ITERATOR_GET($self->content->features, &error);
		if (error != S_SUCCESS)
			return NULL;

		pitr = _make_PMapIterator(itr, &error);
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








