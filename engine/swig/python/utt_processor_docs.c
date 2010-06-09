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
/* Python documentation strings for SUttProcessor.                                  */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%define UttProcessor_DOCSTRING
"""
An Utterance Processor processes an utterance by extracting information from it
and then modifying it in some way.
"""
%enddef

%feature("autodoc", UttProcessor_DOCSTRING) SUttProcessor;


%define UttProcessor_run_DOCSTRING
"""
run(utt)

Execute the UttProcessor on the given utterance.

:param utt: The utterance on which to execute the utterance processor.
:type utt: SUtterance
"""
%enddef

%feature("autodoc", UttProcessor_run_DOCSTRING) SUttProcessor::run;


%define UttProcessor_features_DOCSTRING
"""
Get the features that are defined for the utterance processor.

:return: A map of the utterance processor features.
:rtype: SMap
"""
%enddef

%feature("autodoc", UttProcessor_features_DOCSTRING) features;
