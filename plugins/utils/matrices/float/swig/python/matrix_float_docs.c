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
/* DATE    : February 2010                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Python documentation strings for SMatrixFloat.                                   */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%define matrix_float_DOCSTRING
"""
SMatrixFloat(mlist)

A native Speect class for containing an matrix of floating point values.
Create a new object, that contains an matrix of floating point point values.

:param mlist: A Python list of lists containing only floating point values.
:type mlist: list
:return: Newly created SMatrixFloat object.
:rtype: SMatrixFloat
"""
%enddef

%feature("autodoc", matrix_float_DOCSTRING) SMatrixFloat;


%define get_DOCSTRING
"""
get()

Return a copy of this SMatrixFloat object as a Python list of lists of floating point
values.

:return: Python list of lists floating point values.
:rtype: list
"""
%enddef

%feature("autodoc", get_DOCSTRING) SMatrixFloat::get;


%define row_count_DOCSTRING
"""
Return a count of the number of rows in the SMatrixFloat object.

:return: Number of rows in the SMatrixFloat object.
:rtype: int
"""
%enddef

%feature("autodoc", row_count_DOCSTRING) row_count;


%define col_count_DOCSTRING
"""
Return a count of the number of columns in the SMatrixFloat object.

:return: Number of columns in the SMatrixFloat object.
:rtype: int
"""
%enddef

%feature("autodoc", col_count_DOCSTRING) col_count;

