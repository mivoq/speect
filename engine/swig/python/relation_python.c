/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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


%extend SRelation
{
	_SRelationItr *__iter__()
	{
		_SRelationItr *pitr;
		s_erc error;


		S_CLR_ERR(&error);
		pitr = _make_SRelationItr($self, &error);
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
    return self._to_string()


def _to_string(self, prefix=""):
    # helper function
    stri = "%sRelation \'%s\':\n" %(prefix, self.name())
    item_prefix = "%s    " %prefix
    for i in self:
        stri += i._to_string(prefix=item_prefix)

    return stri
%}
}
