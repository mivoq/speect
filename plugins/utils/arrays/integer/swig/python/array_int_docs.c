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
/* Python documentation strings for SArrayInt.                                      */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%define array_int_DOCSTRING
"""
SArrayInt(ilist)

A native Speect class for containing an array (vector) of integer values.
Create a new object, that contains an array of integer point values.

:param ilist: A Python list containing only integer values.
:type ilist: list
:return: Newly created SArrayInt object.
:rtype: SArrayInt
"""
%enddef

%feature("autodoc", array_int_DOCSTRING) SArrayInt;


%define get_DOCSTRING
"""
get()

Return a copy of this SArrayInt object as a Python list of integer
values.

:return: Python list of integer values.
:rtype: list
"""
%enddef

%feature("autodoc", get_DOCSTRING) SArrayInt::get;


%define count_DOCSTRING
"""
Return a count of the number of elements in the SArrayInt object.

:return: Number of elements in the SArrayInt object.
:rtype: int
"""
%enddef

%feature("autodoc", count_DOCSTRING) count;

