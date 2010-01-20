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
/* DATE    : November 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for Speect SIterator.                                    */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	SObject *py_siterator_first(SObject *itr)
	{
		SIterator *iterator = (SIterator*)itr;


		iterator = SIteratorFirst(iterator);
		return S_OBJECT(iterator);
	}


	SObject *py_siterator_last(SObject *itr)
	{
		SIterator *iterator = (SIterator*)itr;


		iterator = SIteratorLast(iterator);
		return S_OBJECT(iterator);
	}


	SObject *py_siterator_next(SObject *itr)
	{
		SIterator *iterator = (SIterator*)itr;


		iterator = SIteratorNext(iterator);
		return S_OBJECT(iterator);
	}


	SObject *py_siterator_prev(SObject *itr)
	{
		SIterator *iterator = (SIterator*)itr;


		iterator = SIteratorPrev(iterator);
		return S_OBJECT(iterator);
	}

/*
 * Do not delete this delimiter, required for SWIG
 */
%}



