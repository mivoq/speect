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
/* C convenience functions for SSyllabification Python wrapper.                     */
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
	PyObject *_syllabification_syllibify(const SSyllabification *self, const SItem *word,
										 PyObject *phoneList, s_erc *error)
	{
		PyObject *list;
		SList *sylList;
		SObject *phList;


		S_CLR_ERR(error);
		if (!S_SYLLABIFICATION_METH_VALID(self, syllabify))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "_syllabification_syllibify",
					  "Syllabification method \"syllabify\" not implemented");
			return NULL;
		}

		phList = s_pyobject_2_sobject(phoneList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_syllabification_syllibify",
					  "Call to \"s_pyobject_2_sobject\" failed"))
			return NULL;

		sylList = S_SYLLABIFICATION_CALL(self, syllabify)(self, word, S_LIST(phList), error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(phList, "_syllabification_syllibify", error);
			return NULL;
		}

		S_DELETE(phList, "_syllabification_syllibify", error);
		list = s_sobject_2_pyobject(S_OBJECT(sylList), TRUE, error);
		if (S_CHK_ERR(error, S_CONTERR,
		              "_syllabification_syllibify",
					  "Call to \"s_sobject_2_pobject\" failed"))
			return NULL;

		return list;
	}
%}


/************************************************************************************/
/*                                                                                  */
/* Extend the SSyllabification class                                                */
/*                                                                                  */
/************************************************************************************/

%extend SSyllabification
{
%pythoncode
%{
def syllabify(self, word, phone_list):
    """
    syllabify(word, phone_list):

    Syllabify the given phone list of the given word item. The word must be an
    ``SItem`` type so that the syllabification algorithm has access to any
    voice features it requires.

    :param word: The word item.
    :type word: SItem
    :param phone_list: The list of phones for the given word item.
    :type phone_list: list
    :return: A List of lists where the primary list are syllables and the secondary
             lists are the phones in the syllables. For example, for the word *mathematics*,
             the phonelist is ::

                 [m , ae , th, ax, m, ae, t, ih, k, s]

             and the returned list is::

                 [[m, ae], [th, ax], [m, ae], [t, ih, k, s]]

    :rtype: list
    """

    if not isinstance(phone_list, list):
        raise TypeError("Argument \"phone_list\" must be is list type")

    return _syllabification_syllibify(self, word, phone_list)
%}
};





