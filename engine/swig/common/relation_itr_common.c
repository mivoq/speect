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
/* SWIG common C convenience functions for SRelation iterator.                      */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%nodefaultctor _SRelationItr;

%inline
{

	typedef struct
	{
		const SItem *item;
	} _SRelationItr;

	_SRelationItr *_make_SRelationItr(const SRelation *rel, s_erc *error)
	{
		_SRelationItr *ritr;


		S_CLR_ERR(error);
		ritr = S_MALLOC(_SRelationItr, 1);
		if (ritr == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "_make_SRelationItr",
					  "Failed to allocate memory for 'SRelationItr' object");
			return NULL;
		}

		ritr->item = SRelationHead(rel, error);
		if (*error != S_SUCCESS)
		{
			S_FREE(ritr);
			return NULL;
		}

		return ritr;
	}
 }


%extend _SRelationItr
{
	_SRelationItr *__iter__()
	{
		return $self;
	}
}




