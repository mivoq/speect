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
/* C convenience functions for SAddendum Python wrapper.                            */
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
	PyObject *_addendum_get_word(const SAddendum *self, const char *word,
								 PyObject *features, s_erc *error)
	{
		PyObject *tuple;
		SList *wordlist;
		PyObject *object;
		s_bool syllabified = FALSE;
		SObject *feats = NULL;
		s_bool have_features = FALSE;


		S_CLR_ERR(error);
		if (!S_ADDENDUM_METH_VALID(self, get_word))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "_addendum_get_word",
					  "Addendum method \"get_word\" not implemented");
			return NULL;
		}

		if (features != NULL)
		{
			have_features = TRUE;
			feats = s_pyobject_2_sobject(features, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_addendum_get_word",
						  "Call to \"s_pyobject_2_sobject\" failed"))
				return NULL;
		}

		wordlist = S_ADDENDUM_CALL(self, get_word)(self, word, S_MAP(feats), &syllabified, error);
		if (*error != S_SUCCESS)
		{
			if (have_features)
				S_DELETE(feats, "_addendum_get_word", error);
			return NULL;
		}

		if (have_features)
			S_DELETE(feats, "_addendum_get_word", error);

		if (wordlist == NULL)
			Py_RETURN_NONE;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
				  "_addendum_get_word",
				  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = s_sobject_2_pyobject(S_OBJECT(wordlist), TRUE, error);
		if (S_CHK_ERR(error, S_CONTERR,
		              "_addendum_get_word",
					  "Call to \"s_sobject_2_pyobject\" failed"))
		{
			Py_XDECREF(tuple);
			return NULL;
		}

		PyTuple_SET_ITEM(tuple, 0, object);

		if (syllabified)
		{
			object = Py_True;
			Py_XINCREF(object);
			PyTuple_SET_ITEM(tuple, 1, object);
		}
		else
		{
			object = Py_False;
			Py_XINCREF(object);
			PyTuple_SET_ITEM(tuple, 1, object);
		}

		return tuple;
	}
%}


/************************************************************************************/
/*                                                                                  */
/* Extend the SAddendum class                                                       */
/*                                                                                  */
/************************************************************************************/

%extend SAddendum
{
%pythoncode
%{
def get_word(self, word, features=None):
    """
    get_word(word, features)

    Get a word from the addendum.

    :param word: The word to get.
    :type word: string
    :param features: Specific features which might distinguish the word if multiple
                     entries of the word exists in the addendum. If ``None`` then the
                     first entry of the word is returned.
    :type features: dict
    :return: The return value is dependant on the word definition in the addendum, and can be:

                 * A list of phones for the given word (no syllables were defined in the addendum).
                 * A list of syllables, where the syllables are lists of phones.
                 * ``None`` if word was not found in the addendum.

             As well as a ``bool`` value, specifying if the returned list is phones or syllables.
             If ``True`` then syllables were returned, else if ``False`` a list of phones were
             returned.

             For example::

                 list, syllabified = myaddendum.get_word(\"hello\", None)

    :rtype: list, bool
    """
    tmp_tuple = _addendum_get_word(self, word, features)
    if tmp_tuple is not None:
        wlist = tmp_tuple[0]
        syllabified = tmp_tuple[1]
    else:
        wlist = None
        syllabified = False

    return wlist, syllabified
%}
};





