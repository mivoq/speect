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
/* C convenience functions for SAddendum iterator Python wrapper.                   */
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
				     const SMap *features, s_erc *error)
	{
		PyObject *tuple;
		SList *wordlist;
		PyObject *object;
		s_bool syllabified = FALSE;


		wordlist = S_ADDENDUM_CALL(self, get_word)(self, word, features,
			   			 	   &syllabified, error);
		if (*error != S_SUCCESS)
			return NULL;

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
			      "Call to \"s_sobject_2_pobject\" failed"))
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
def get_word(self, word, features):
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

             As well as ``bool`` value, specifying if the returned list is phones or syllables.
             If ``True`` then syllables were returned, else if ``False`` a list of phones were
             returned.

             For example::

                 list, syllabified = myaddendum.get_word(\"hello\", None)

    :rtype: list, bool
    """
    wlist, syllabified = _addendum_get_word(self, word, features)

    return wlist, syllabified
%}
};





