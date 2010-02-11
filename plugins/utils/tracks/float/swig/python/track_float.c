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
/* C convenience functions for SAudio Python wrapper.                               */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%{
	typedef struct
	{
		float **fpp;
		uint32  row_count;
		uint32  col_count;
		float  *times;
	} float_track_t;
%}



typedef struct
{
	%extend
	{
		const uint32 row_count;
		const uint32 col_count;
	}
} STrackFloat;


%types(STrackFloat = SObject, SObject*);

%extend STrackFloat
{
	STrackFloat(const float *times, uint32 len, const float **fm,
				uint32 row_count, uint32 col_count, s_erc *error)
	{
		STrackFloat *tmp;


		if (len != row_count)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "STrackFloat()",
					  "times count and row count differ");
			return NULL;
		}

		tmp = (STrackFloat*)S_NEW("STrackFloat", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "STrackFloat()",
					  "Failed to create new 'STrackFloat' object"))
			return NULL;

		tmp->time = (float*)times;
		tmp->data->row_count = row_count;
		tmp->data->col_count = col_count;
		tmp->data->f = (float**)fm;

		return tmp;
	}

	~STrackFloat()
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE($self, "~STrackFloat()", &error);
	}

	float_track_t get()
	{
		float_track_t tmp;


		tmp.fpp = $self->data->f;
		tmp.row_count = $self->data->row_count;
		tmp.col_count = $self->data->col_count;
		tmp.times = $self->time;

		return tmp;
	}
}

%{
	const uint32 STrackFloat_row_count_get(STrackFloat *track)
	{
		return (const uint32)track->data->row_count;
	}

	const uint32 STrackFloat_col_count_get(STrackFloat *track)
	{
		return (const uint32)track->data->col_count;
	}
%}

