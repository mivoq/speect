/************************************************************************************/
/* Copyright (c) 2009-2010 The Department of Arts and Culture,                      */
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
/* AUTHOR  : Richard Carlson, Aby Louw                                              */
/* DATE    : March 2009                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* The Speect Engine library SWIG/Python interface definition.                      */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%module(docstring="Speect Engine library Python interface") speect

%include "typemaps.i"
%include "exception.i"
%include "speect_typemaps.i"

/************************************************************************************/
/*                                                                                  */
/* Speect Engine header.                                                            */
/*                                                                                  */
/************************************************************************************/

%header
%{
#include "speect.h"
%}

%feature("autodoc", "1");

/************************************************************************************/
/*                                                                                  */
/* Speect Python typemaps.                                                          */
/*                                                                                  */
/************************************************************************************/

%include "speect_py_typemaps.i"


/************************************************************************************/
/*                                                                                  */
/* Miscellaneous C Speect functions.                                                */
/*                                                                                  */
/************************************************************************************/

%include "misc.c"


/************************************************************************************/
/*                                                                                  */
/* SObject wrapper functions                                                        */
/*                                                                                  */
/************************************************************************************/

%include "primitives.c"
%include "iterator.c"
%include "object.c"
%include "utterance.c"
%include "relation_itr.c"
%include "relation.c"
%include "map.c"
%include "list.c"
%include "item.c"
%include "voice.c"
%include "plugin.c"
%include "utt_processor.c"
%include "feat_processor.c"
