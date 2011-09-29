/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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
/* SWIG common C convenience functions for SArrayFloat.                             */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Typedef used in typemaps                                                         */
/*                                                                                  */
/************************************************************************************/


%{
	typedef struct
	{
		float *fa_fp;
		uint32 fa_count;
	} float_array_t;
%}


/************************************************************************************/
/*                                                                                  */
/* Extend the SArrayFloat class                                                     */
/*                                                                                  */
/************************************************************************************/

typedef struct
{
	%extend
	{
		const uint32 count;
	}
} SArrayFloat;


%types(SArrayFloat = SObject, SObject*);

%extend SArrayFloat
{
	SArrayFloat(const float *array, uint32 len, s_erc *error)
	{
		SArrayFloat *tmp;


		tmp = S_NEW(SArrayFloat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SArrayFloat()",
					  "Failed to create new 'SArrayFloat' object"))
			return NULL;

		tmp->count = len;
		tmp->f = (float*)array;
		return tmp;
	}

	~SArrayFloat()
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE($self, "~SArrayFloat()", &error);
	}

	float_array_t get()
	{
		float_array_t tmp;


		tmp.fa_fp = $self->f;
		tmp.fa_count = $self->count;

		return tmp;
	}
}

%{

	const uint32 SArrayFloat_count_get(SArrayFloat *array)
	{
		return (const uint32)array->count;
	}
%}

