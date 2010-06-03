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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SMatrixInt Python wrapper.                           */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%define matrix_int_DOCSTRING
"""
SMatrixInt(mlist)

A native Speect class for containing an matrix of integer values.
Create a new object, that contains an matrix of integer point values.

:param mlist: A Python list of lists containing only integer values.
:type mlist: list
:return: Newly created SMatrixInt object.
:rtype: SMatrixInt
"""
%enddef

%feature("autodoc", matrix_int_DOCSTRING) SMatrixInt;


%define get_DOCSTRING
"""
get()

Return a copy of this SMatrixInt object as a Python list of lists of integer
values.

:return: Python list of lists integer values.
:rtype: list
"""
%enddef

%feature("autodoc", get_DOCSTRING) SMatrixInt::get;


%define row_count_DOCSTRING
"""
Return a count of the number of rows in the SMatrixInt object.

:return: Number of rows in the SMatrixInt object.
:rtype: int
"""
%enddef

%feature("autodoc", row_count_DOCSTRING) row_count;


%define col_count_DOCSTRING
"""
Return a count of the number of columns in the SMatrixInt object.

:return: Number of columns in the SMatrixInt object.
:rtype: int
"""
%enddef

%feature("autodoc", col_count_DOCSTRING) col_count;


%{
	typedef struct
	{
		sint32 **ipp;
		uint32   row_count;
		uint32   col_count;
	} int_matrix_t;
%}



typedef struct
{
	%extend
	{
		const uint32 row_count;
		const uint32 col_count;
	}
} SMatrixInt;


%types(SMatrixInt = SObject, SObject*);

%extend SMatrixInt
{
	SMatrixInt(const sint32 **im, uint32 row_count, uint32 col_count, s_erc *error)
	{
		SMatrixInt *tmp;


		tmp = (SMatrixInt*)S_NEW("SMatrixInt", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SMatrixInt()",
					  "Failed to create new 'SMatrixInt' object"))
			return NULL;

		tmp->row_count = row_count;
		tmp->col_count = col_count;
		tmp->i = (sint32**)im;

		return tmp;
	}

	~SMatrixInt()
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE($self, "~SMatrixInt()", &error);
	}

	int_matrix_t get()
	{
		int_matrix_t tmp;


		tmp.ipp = $self->i;
		tmp.row_count = $self->row_count;
		tmp.col_count = $self->col_count;

		return tmp;
	}
}

%{
	const uint32 SMatrixInt_row_count_get(SMatrixInt *matrix)
	{
		return (const uint32)matrix->row_count;
	}

	const uint32 SMatrixInt_col_count_get(SMatrixInt *matrix)
	{
		return (const uint32)matrix->col_count;
	}
%}

