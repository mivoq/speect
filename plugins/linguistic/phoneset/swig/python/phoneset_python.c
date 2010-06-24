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
/* C convenience functions for SPhoneset Python wrapper.                            */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Inline helper functions                                                          */
/*                                                                                  */
/************************************************************************************/

%inline
%{
	PyObject *_get_phone_features(const SPhoneset *self, const char *phone,
								  s_erc *error)
	{
		const SList *phoneFeatures;
		PyObject *pyList;


		S_CLR_ERR(error);
		if (!S_PHONESET_METH_VALID(self, get_phone_features))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "_get_phone_features",
					  "Phoneset method \"get_phone_features\" not implemented");
			return NULL;
		}

		phoneFeatures = S_PHONESET_CALL(self, get_phone_features)(self, phone, error);
		if (*error != S_SUCCESS)
			return NULL;

		/* false because Python does not own the list */
		pyList = s_sobject_2_pyobject(S_OBJECT(phoneFeatures), FALSE, error);
		if (S_CHK_ERR(error, S_CONTERR,
		              "_get_phone_features",
					  "Call to \"s_sobject_2_pobject\" failed"))
			return NULL;

		return pyList;
	}


	PyObject *_get_phone_list(const SPhoneset *self, s_erc *error)
	{
		SList *phoneList;
		PyObject *pyList;


		S_CLR_ERR(error);
		if (!S_PHONESET_METH_VALID(self, get_phone_list))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "_get_phone_list",
					  "Phoneset method \"get_phone_list\" not implemented");
			return NULL;
		}

		phoneList = S_PHONESET_CALL(self, get_phone_list)(self, error);
		if (*error != S_SUCCESS)
			return NULL;

		/* true because Python does own the list */
		pyList = s_sobject_2_pyobject(S_OBJECT(phoneList), TRUE, error);
		if (S_CHK_ERR(error, S_CONTERR,
		              "_get_phone_list",
					  "Call to \"s_sobject_2_pobject\" failed"))
			return NULL;

		return pyList;
	}
%}


/************************************************************************************/
/*                                                                                  */
/* Extend the SPhoneset class                                                       */
/*                                                                                  */
/************************************************************************************/

%extend SPhoneset
{
%pythoncode
%{
def get_phone_features(self, phone):
    """
    get_phone_features(phone)
    
    Get a list of the given phone's features.
    
    :param phone: The phone for which the feature list is requested.
    :type word: string
    :return: A list of named features which are the features of the
             phones, or ``None`` if the phone is not in the phoneset.
    :rtype: list
    :note: The returned list *must* be considered constant in the C sence, and
           not modified in any way.
    """

    if phone == None:
        raise TypeError("Argument \"phone\" can not be 'None'")
    return _get_phone_features(self, phone)


def get_phone_list(self):
    """
    get_phone_list()

    Get a list of phones defined in the phoneset.

    :return:  A list of named phones defined in the phoneset.
    :rtype: list
    """

    return _get_phone_list(self)
%}
};





