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
/* C convenience functions for SHalfphoneDB Python wrapper.                         */
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
	PyObject *_get_units(const SHalfphoneDB *self, const char *unit_type,
						 const char *left_context, const char *right_context,
						 uint min_triphones, uint min_diphones, s_erc *error)
	{
		const SList *unitList;
		PyObject *pyList;


		S_CLR_ERR(error);
		if (!S_HALFPHONE_DB_METH_VALID(self, get_units))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "_get_units",
					  "HalfphoneDB method \"get_units\" not implemented");
			return NULL;
		}

		unitList = S_HALFPHONE_DB_CALL(self, get_units)(self, unit_type, left_context,
														right_context, min_triphones,
														min_diphones, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_get_units",
					  "Call to HalfphoneDB method \"get_units\" failed"))
			return NULL;

		/* false because Python does not own the list */
		pyList = s_sobject_2_pyobject(S_OBJECT(unitList), FALSE, error);
		if (S_CHK_ERR(error, S_CONTERR,
		              "_get_units",
					  "Call to \"s_sobject_2_pobject\" failed"))
			return NULL;

		return pyList;
	}
%}


/************************************************************************************/
/*                                                                                  */
/* Extend the SHalfphoneDB class                                                    */
/*                                                                                  */
/************************************************************************************/

%extend SHalfphoneDB
{
%pythoncode
%{
def get_units(self, unit_type, left_context=None, right_context=None, 
              min_triphones=0, min_diphones=0):
    """
    Get a list of units (``SItem`` type units) from the halfphone database that
    conform to the given context. The current halfphone database implementation
    names unit type as follows:
    
        For each item in the ``Segment`` relation we have 2 halfphone units.
        For example, if the segment item is named ``dh`` then the 2 halphone
        units are named ``left-dh`` and ``right-dh``.

    The units are stored in the database with their context information. This allows
    for a type of pre-selection when requesting units. All units are in the lowest
    tier of the context information (no given context), and the upper tiers contain
    less units, but might return better contexts for concatenation. The left and 
    right contexts are named according to the ``Segment`` relation names, and *not* the
    halfphone names.

    
    :param unit_type: The unit type to get (halfphone unit type).
    :type unit_type: string
    :param left_context: The left context of the units to get.
    :type left_context: string
    :param right_context: The right context of the units to get.
    :type right_context: string
    :param min_triphones: The minimum number of triphone contexts required. 
                          If this number is not satisfied then diphone contexts
                          are also searched for.
    :type min_triphones: int
    :param min_diphones: The minimum number of diphone contexts required. If this
                         number is not satisfied then all units of the given unit
                         type are returned.
    :type min_diphones: int
    :return: A list of unit items (``SItem``) that satisfy the
             given context, or ``None`` if no such units exists.
    :note: The list and the items in the list **must** be regarded as constant in
           the C sence, and **not** be modified in any way.
    """

    return _get_units(self, unit_type, 
                      left_context, right_context, 
                      min_triphones, min_diphones)
%}
};





