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
/* AUTHOR  : Richard Carlson, Aby Louw                                              */
/* DATE    : March 2009                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* The Speect Engine library SWIG/Python interface definition.                      */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%module speect

%include "typemaps.i"
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


/************************************************************************************/
/*                                                                                  */
/* SWIG/Python interface files.                                                     */
/*                                                                                  */
/************************************************************************************/

/*
 * miscellaneous Speect functions and Python
 * helper functions
 */
%include "misc.c"
%include "misc.py"


/*
 * plug-in loading Speect functions and Python
 * helper functions
  */
%include "plugins.c"
%include "plugins.py"


/*
 * SObject wrapper functions
 */
%include "object.c"
%include "object.py"


/*
 * SInt, SFloat, SString and SVoid wrapper functions
 */
%include "primitives.c"
%include "primitives.py"


/*
 * SIterator wrapper functions
 */
%include "iterator.c"
%include "iterator.py"


/*
 * SContainer Python class
 */
%include "container.py"


/*
 * SList wrapper functions
 */
%include "list.c"
%include "list.py"


/*
 * SMap wrapper functions
 */
%include "map.c"
%include "map.py"


/*
 * SMapInternal Python class
 */
%include "map_internal.py"


/*
 * SItem wrapper functions
 */
%include "item.c"
%include "item.py"


/*
 * SRelation wrapper functions
 */
%include "relation.c"
%include "relation.py"


/*
 * SUtterance wrapper functions
 */
%include "utterance.c"
%include "utterance.py"


/*
 * SVoice wrapper functions
 */
%include "voice.c"
%include "voice.py"


/*
 * SUttProcessor wrapper functions
 */
%include "utt_processor.c"
%include "utt_processor.py"


/*
 * SFeatProcessor wrapper functions
 */
%include "feat_processor.c"
%include "feat_processor.py"

