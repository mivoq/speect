/************************************************************************************/
/* Copyright (c) 2011 The Department of Arts and Culture,                           */
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
/* DATE    : 20 April 2011                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C miscellaneous functions for Python wrapper.                                    */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/



/*
 * Speect Engine initialization on module loading
 */
%init
%{
	/* delimiters because of the need for local variables */
	{
		s_erc rv = S_SUCCESS;
		s_logger *logger = NULL;


		logger = _s_logger_swig_new();
		if (logger == NULL)
			SWIG_Error(SWIG_RuntimeError, "Call to \"s_logger_swig_new\" failed");

		rv = speect_init(logger);
		if (rv != S_SUCCESS)
			SWIG_Error(SWIG_RuntimeError, "Failed to initialize Speect Engine");
	}
%}