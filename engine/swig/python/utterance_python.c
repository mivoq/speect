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
/* C convenience functions for SUtterance Python wrapper.                           */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%extend SUtterance
{
	s_bool __contains__(const char *name, s_erc *error)
	{
		s_bool is_present;


		is_present = SUtteranceRelationIsPresent($self, name, error);
		if (*error != S_SUCCESS)
			return FALSE;

		return is_present;
	}


	_PMapIterator *__iter__()
	{
		_PMapIterator *pitr;
		SIterator *itr;
		s_erc error;


		S_CLR_ERR(&error);
		itr = S_ITERATOR_GET($self->relations, &error);
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
    Get a string representation of the utterance.

    :return: A string representation of the utterance.
    :rtype: string
    """

    stri = "Utterance:\n"
    for f in self.features:
        stri += '    Feature: %20.20s => %s\n' %(f, repr(self.features[f]))

    for r in self:
        stri += self.relation_get(r).to_string(prefix="        ")

    return stri
%}

};
