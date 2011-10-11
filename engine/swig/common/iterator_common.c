/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
/* SWIG common C convenience functions for SIterator.                               */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%nodefaultctor _PListIterator;
%nodefaultctor _PMapIterator;

%inline
{
	typedef struct
	{
		SIterator *_itr;
	} _PMapIterator;


	_PMapIterator *_make_PMapIterator(SIterator *itr, s_erc *error)
	{
		_PMapIterator *pitr;


		S_CLR_ERR(error);
		pitr = S_MALLOC(_PMapIterator, 1);
		if (pitr == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "_make_PMapIterator",
					  "Failed to allocate memory for '_PMapIterator' object");
			return NULL;
		}

		pitr->_itr = itr;
		return pitr;
	}

	typedef struct
	{
		SIterator *_itr;
	} _PListIterator;


	_PListIterator *_make_PListIterator(SIterator *itr, s_erc *error)
	{
		_PListIterator *pitr;


		S_CLR_ERR(error);
		pitr = S_MALLOC(_PListIterator, 1);
		if (pitr == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "_make_PListIterator",
					  "Failed to allocate memory for '_PListIterator' object");
			return NULL;
		}

		pitr->_itr = itr;
		return pitr;
	}
}
