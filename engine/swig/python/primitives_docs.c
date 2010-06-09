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
/* Python documentation strings for primitive data types.                           */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%define sobject_2_pyobject_DOCSTRING
"""
sobject_2_pyobject(object, own)

Convert a Speect ``SObject`` type object to a Python object. This can be
use full for conversions where either type is required an is used extensively
internally and in the plug-in implementations.

The ``own`` ownership flag specifies if Python must take ownership of the object,
and hence can delete it. If not then Speect controls the memory of the object.

:param object: The Speect object to convert.
:type key: SObject
:param own: Ownership flag.
:type own: bool
:return: A Python representation of the given object
:rtype: Python type object.
"""
%enddef

%feature("autodoc", sobject_2_pyobject_DOCSTRING) sobject_2_pyobject;


%define pyobject_2_sobject_DOCSTRING
"""
pyobject_2_sobject(pobject)

Convert a Python type object to a Speect ``SObject`` object. This can be
use full for conversions where either type is required an is used extensively
internally and in the plug-in implementations.

Note that for basic types (int, float, string) Speect creates a new type. For
other types, Speect creates an ``SObject``. This function will only work with
basic types if no external plug-ins are loaded.

:param pobject: The Python object to convert.
:return: A Speect representation of the given object
:rtype: ``SObject`` type object.
"""
%enddef

%feature("autodoc", pyobject_2_sobject_DOCSTRING) pyobject_2_sobject;

