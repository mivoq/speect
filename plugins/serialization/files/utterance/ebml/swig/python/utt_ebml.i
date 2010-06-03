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
/* Loading/Saving SUtterances in EBML format.                                       */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%module utt_ebml


/************************************************************************************/
/*                                                                                  */
/* Speect Engine header.                                                            */
/*                                                                                  */
/************************************************************************************/

%header
%{
#include "speect.h"
%}

%include "exception.i"
%import speect.i


/************************************************************************************/
/*                                                                                  */
/* Load the Speect EBML primitives and EBML utterance plug-ins                      */
/*                                                                                  */
/************************************************************************************/

%init
%{
	{
		s_erc rv = S_SUCCESS;
		SPlugin *uttEBMLPlugin;
		SPlugin *primEBMLPlugin;


		uttEBMLPlugin = s_pm_load_plugin("utt-ebml.spi", &rv);
		if (rv != S_SUCCESS)
			SWIG_exception(SWIG_RuntimeError, "Failed to load Speect utterance EBML plug-in");

		primEBMLPlugin = s_pm_load_plugin("prim-ebml.spi", &rv);
		if (rv != S_SUCCESS)
		{
			S_DELETE(uttEBMLPlugin, "utt_ebml.i init", &rv);
			SWIG_exception(SWIG_RuntimeError, "Failed to load Speect primitives EBML plug-in");
			return;
		}

	fail:
		return;
	}
%}


/************************************************************************************/
/*                                                                                  */
/* SWIG/Python interface files.                                                     */
/*                                                                                  */
/************************************************************************************/

/*
 * Utterance loading/saving in EBML format
 */
%include "utt_ebml.c"



