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
/* DATE    : December 2010                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Python documentation strings for SArrayFloat.                                    */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%define array_float_DOCSTRING
"""
SArrayFloat(flist)

A native Speect class for containing an array (vector) of floating point values.
Create a new object, that contains an array of floating point values.

:param flist: A Python list containing only floating point values.
:type flist: list
:return: Newly created SArrayFloat object.
:rtype: SArrayFloat
"""
%enddef

%feature("autodoc", array_float_DOCSTRING) SArrayFloat;


%define get_DOCSTRING
"""
get()

Return a copy of this SArrayFloat object as a Python list of floating point
values.

:return: Python list of floating point values.
:rtype: list
"""
%enddef

%feature("autodoc", get_DOCSTRING) SArrayFloat::get;


%define count_DOCSTRING
"""
Return a count of the number of elements in the SArrayFloat object.

:return: Number of elements in the SArrayFloat object.
:rtype: int
"""
%enddef
