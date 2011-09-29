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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* RELP synthesis.                                                                  */
/* Loosely based on us_unit.cc of Festival                                          */
/* (http://www.cstr.ed.ac.uk/projects/festival.html (1.96 beta) and filter.cc       */
/* of Edinburgh Speech Tools,http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96)*/
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned projects.                                                              */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* Festival us_unit.cc license,                                                     */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                              Copyright (c) 1996,1997                             */
/*                             All Rights Reserved.                                 */
/*       Permission is hereby granted, free of charge, to use and distribute        */
/*       this software and its documentation without restriction, including         */
/*       without limitation the rights to use, copy, modify, merge, publish,        */
/*       distribute, sublicense, and/or sell copies of this work, and to            */
/*       permit persons to whom this work is furnished to do so, subject to         */
/*       the following conditions:                                                  */
/*        1. The code must retain the above copyright notice, this list of          */
/*           conditions and the following disclaimer.                               */
/*        2. Any modifications must be clearly marked as such.                      */
/*        3. Original authors' names are not deleted.                               */
/*        4. The authors' names are not used to endorse or promote products         */
/*           derived from this software without specific prior written              */
/*           permission.                                                            */
/*       THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK              */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE           */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author :  Paul Taylor                                                      */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* Edinburgh Speech Tools filter.cc license,                                        */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                           Copyright (c) 1994,1995,1996                           */
/*                             All Rights Reserved.                                 */
/*       Permission is hereby granted, free of charge, to use and distribute        */
/*       this software and its documentation without restriction, including         */
/*       without limitation the rights to use, copy, modify, merge, publish,        */
/*       distribute, sublicense, and/or sell copies of this work, and to            */
/*       permit persons to whom this work is furnished to do so, subject to         */
/*       the following conditions:                                                  */
/*        1. The code must retain the above copyright notice, this list of          */
/*           conditions and the following disclaimer.                               */
/*        2. Any modifications must be clearly marked as such.                      */
/*        3. Original authors' names are not deleted.                               */
/*        4. The authors' names are not used to endorse or promote products         */
/*           derived from this software without specific prior written              */
/*           permission.                                                            */
/*       THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK              */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE           */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author :  Simon Kin                                                        */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include "relp.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void copy_source_track(SRelp *self, s_erc *error);

static void td_synthesis(SRelp *self, s_erc *error);

static void map_coefs(SRelp *self, s_erc *error);

static void lpc_filter_fast(STrackFloat *lpc, SAudio *res, SAudio *sig, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SAudio *synthesis(SRelp *self, s_erc *error)
{
	SAudio *waveform;


	copy_source_track(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "synthesis",
				  "Call to \"copy_source_track\" failed"))
		return NULL;

	td_synthesis(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "synthesis",
				  "Call to \"td_synthesis\" failed"))
		return NULL;

	map_coefs(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "synthesis",
				  "Call to \"map_coefs\" failed"))
		return NULL;

	waveform = S_NEW(SAudio, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "synthesis",
				  "Failed to create new 'SAudio' object"))
		return NULL;

	S_AUDIO_CALL(waveform, resize)(waveform, self->wave_res->num_samples, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "synthesis",
				  "Call to SAudio method \"resize\" failed"))
	{
		S_DELETE(waveform, "synthesis", error);
		return NULL;
	}

	waveform->sample_rate = self->wave_res->sample_rate;

	lpc_filter_fast(self->target, self->wave_res, waveform, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "synthesis",
				  "Call to \"lpc_filter_fast\" failed"))
	{
		S_DELETE(waveform, "synthesis", error);
		return NULL;
	}

	return waveform;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/* copy the source track to the target track */
static void copy_source_track(SRelp *self, s_erc *error)
{
	uint32 i;


	S_CLR_ERR(error);

	/* copy the track to the target */
	self->target = S_NEW(STrackFloat, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "copy_source_track",
				  "Failed to create new 'STrackFloat' object"))
		return;

	self->target->time = S_CALLOC(float, self->num_frames);
	if (self->target->time  == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "copy_source_track",
				  "Failed to allocate memory for 'float' object");
		return;
	}

	/* copy the track to the target */
 	memcpy(self->target->time, self->track->time, self->num_frames * sizeof(float));

	self->target->data->row_count = self->track->data->row_count;
	self->target->data->col_count = self->track->data->col_count;
	self->target->data->f = S_CALLOC(float*, self->num_frames);
	if (self->target->data->f  == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "copy_source_track",
				  "Failed to allocate memory for 'float*' object");
		return;
	}

	for (i = 0; i < self->num_frames; i++)
	{
		self->target->data->f[i] = S_CALLOC(float, self->num_channels);
		if (self->target->data->f[i]  == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "copy_source_track",
					  "Failed to allocate memory for 'float' object");
			return;
		}
		memcpy(self->target->data->f[i], self->track->data->f[i],
			   self->num_channels * sizeof(float));
	}
}


static void td_synthesis(SRelp *self, s_erc *error)
{
	SAudio *wave;
	float sr;
	SArrayInt *residuals;
	float last_time;
	sint32 last_sample = 0;
	float *samples = NULL;
	uint32 i;
	SArrayInt *frame = NULL;
	sint32 t_start = 0;
	sint32 j;


	S_CLR_ERR(error);

	if ((self->num_frames > 0) && (self->map->count > 0))
	{
		sr = (float)self->sample_rate;
		self->wave_res = S_NEW(SAudio, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "td_synthesis",
					  "Failed to create new 'SAudio' object"))
			return;

		wave = self->wave_res;
		wave->sample_rate = self->sample_rate;
		residuals = self->residuals[self->num_frames - 1];
		last_time = self->target->time[self->num_frames - 1];

		/* window_signal guarantees odd */
		last_sample = (sint32)(last_time * sr + 0.5) + (sint32)((residuals->count - 1)/2.0);

		samples = S_CALLOC(float, last_sample + 1);
		if (samples == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "td_synthesis",
					  "Failed to allocate memory for 'float' object");
			return;
		}

		for (i = 0; i < self->map->count; i++)
		{
			sint32 mapped_i = 0;


			if (self->map->i[i] < (sint32)self->num_frames)
				mapped_i = self->map->i[i];
			else
				mapped_i = self->num_frames - 1;

			frame = self->residuals[mapped_i];

			/* window_signal guarantees odd */
			t_start = (sint32)(self->target->time[i] * sr + 0.5) - (sint32)((frame->count - 1)/2.0);

			for (j = 0; j < (sint32)frame->count; j++)
				if (j + t_start >= 0)
					samples[j + t_start] += frame->i[j];
		}

		wave->samples = samples;
		wave->num_samples = last_sample + 1;
	}
}


static void map_coefs(SRelp *self, s_erc *error)
{
    uint32 i;
	uint32 j;
    uint32 m;


	S_CLR_ERR(error);

    if (self->map->count > self->num_frames)
	    m = self->num_frames;
    else
	    m = self->map->count;

    for (i = 0; i < m; i++)
		for (j = 0; j < self->num_channels; ++j)
			self->target->data->f[i][j] = self->track->data->f[self->map->i[i]][j];

    /*
     * There can be one or two frames at the end of target_coef without
     * a map.  Here we zero them, blindly assuming they are in silence
     */
    for ( ; i < self->num_frames; i++)
		for (j = 0; j < self->num_channels; ++j)
			self->target->data->f[i][j] = 0;
}


static void lpc_filter_fast(STrackFloat *lpc, SAudio *res, SAudio *sig, s_erc *error)
{
	float *buff;
	float *filt;
	float s = 0;
	sint32 order = (sint32)lpc->data->col_count - 1;
	float *residual;
	float *signal;
	uint32 i, j, k, m, n; /* counter variables */
	uint32 start, end; /* counter variables */


	S_CLR_ERR(error);

	if (order < 0)
		order = 0;

	buff = S_CALLOC(float, res->num_samples + order + 1);
	if (buff == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "lpc_filter_fast",
				  "Failed to allocate memory for 'float' object");
		return;
	}
	for (k = 0; k < (uint32)order; k++)
		buff[k] = 0;

	filt = S_CALLOC(float, order + 1);
	if (buff == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "lpc_filter_fast",
				  "Failed to allocate memory for 'float' object");
		return;
	}

	residual = res->samples;
	for (start = k, m = 0, i = 0; i < lpc->data->row_count - 1; ++i)
	{
		end = (uint32)((lpc->time[i] + lpc->time[i + 1]) * (float)res->sample_rate)/2;
		if (end > res->num_samples)
			end = res->num_samples;

		for (j = 1; j < lpc->data->col_count; j++)
			filt[j] = lpc->data->f[i][j];
		n = j;

		for (k = start; k < end; ++k,++m)
		{
			s = 0;

			for (j = 1; j < n; ++j)
				s += filt[j] * buff[k-j];

			buff[k] = s + residual[m];
		}
		start = end;
	}

	signal = sig->samples;
	for (j = 0, i = order; i < k; j++, i++)
		signal[j] = buff[i];

	S_FREE(buff);
	S_FREE(filt);
}
