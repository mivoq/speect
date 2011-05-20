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
/* C convenience functions for SG2P Python wrapper.                                 */
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
	PyObject *_g2p_apply(const SG2P *self, const char *word, s_erc *error)
	{
		PyObject *list;
		SList *phonelist;


		S_CLR_ERR(error);
		if (!S_G2P_METH_VALID(self, apply))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "_g2p_apply",
					  "G2P method \"apply\" not implemented");
			return NULL;
		}

		phonelist = S_G2P_CALL(self, apply)(self, word, error);
		if (*error != S_SUCCESS)
			return NULL;

		list = s_sobject_2_pyobject(S_OBJECT(phonelist), TRUE, error);
		if (S_CHK_ERR(error, S_CONTERR,
		              "_g2p_apply",
					  "Call to \"s_sobject_2_pobject\" failed"))
			return NULL;

		return list;
	}
%}


/************************************************************************************/
/*                                                                                  */
/* Extend the SG2P class                                                            */
/*                                                                                  */
/************************************************************************************/

%extend SG2P
{
%pythoncode
%{
def apply(self, word):
    """
    apply(word)

    Apply the g2p to the given word to get a phone list.

    :param word: The word to convert to a phone list.
    :type word: string
    :return: A list of phones for the given word or ``None`` if the
                 g2p failed. If the g2p failed a warning will be set in the
                 error log.
    :rtype: list
    """

    if not isinstance(word, unicode) and not isinstance(word, str):
        raise TypeError("Argument \"word\" must be a str or unicode type")

    return _g2p_apply(self, word)
%}
};





