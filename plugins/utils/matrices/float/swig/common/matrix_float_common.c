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
/* DATE    : February 2010                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SWIG common C convenience functions for SMatrixFloat.                            */
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
		float **fpp;
		uint32  row_count;
		uint32  col_count;
	} float_matrix_t;
%}


/************************************************************************************/
/*                                                                                  */
/* Extend the SMatrixFloat class                                                    */
/*                                                                                  */
/************************************************************************************/


typedef struct
{
	%extend
	{
		const uint32 row_count;
		const uint32 col_count;
	}
} SMatrixFloat;


%types(SMatrixFloat = SObject, SObject*);

%extend SMatrixFloat
{
	SMatrixFloat(const float **fm, uint32 row_count, uint32 col_count, s_erc *error)
	{
		SMatrixFloat *tmp;


		tmp = (SMatrixFloat*)S_NEW("SMatrixFloat", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SMatrixFloat()",
					  "Failed to create new 'SMatrixFloat' object"))
			return NULL;

		tmp->row_count = row_count;
		tmp->col_count = col_count;
		tmp->f = (float**)fm;

		return tmp;
	}

	~SMatrixFloat()
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE($self, "~SMatrixFloat()", &error);
	}

	float_matrix_t get()
	{
		float_matrix_t tmp;


		tmp.fpp = $self->f;
		tmp.row_count = $self->row_count;
		tmp.col_count = $self->col_count;

		return tmp;
	}
}

%{
	const uint32 SMatrixFloat_row_count_get(SMatrixFloat *matrix)
	{
		return (const uint32)matrix->row_count;
	}

	const uint32 SMatrixFloat_col_count_get(SMatrixFloat *matrix)
	{
		return (const uint32)matrix->col_count;
	}
%}

