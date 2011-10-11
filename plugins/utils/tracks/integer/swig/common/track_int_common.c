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
/* SWIG common C convenience functions for STrackInt.                               */
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
		sint32 **ipp;
		uint32  row_count;
		uint32  col_count;
		float  *times;
	} int_track_t;
%}


/************************************************************************************/
/*                                                                                  */
/* Extend the STrackInt class                                                       */
/*                                                                                  */
/************************************************************************************/


typedef struct
{
	%extend
	{
		const uint32 row_count;
		const uint32 col_count;
	}
} STrackInt;


%types(STrackInt = SObject, SObject*);

%extend STrackInt
{
	STrackInt(const float *times, uint32 len, const sint32 **im,
			  uint32 row_count, uint32 col_count, s_erc *error)
	{
		STrackInt *tmp;


		if (len != row_count)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "STrackInt()",
					  "times count and row count differ");
			return NULL;
		}

		tmp = S_NEW(STrackInt, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "STrackInt()",
					  "Failed to create new 'STrackInt' object"))
			return NULL;

		tmp->time = (float*)times;
		tmp->data->row_count = row_count;
		tmp->data->col_count = col_count;
		tmp->data->i = (sint32**)im;

		return tmp;
	}

	~STrackInt()
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE($self, "~STrackInt()", &error);
	}

	int_track_t get()
	{
		int_track_t tmp;


		tmp.ipp = $self->data->i;
		tmp.row_count = $self->data->row_count;
		tmp.col_count = $self->data->col_count;
		tmp.times = $self->time;

		return tmp;
	}
}

%{
	const uint32 STrackInt_row_count_get(STrackInt *track)
	{
		return (const uint32)track->data->row_count;
	}

	const uint32 STrackInt_col_count_get(STrackInt *track)
	{
		return (const uint32)track->data->col_count;
	}
%}

