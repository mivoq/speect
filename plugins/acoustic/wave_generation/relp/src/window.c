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
/* RELP residual windowing.                                                         */
/* Loosely based on us_unit.cc of Festival                                          */
/* (http://www.cstr.ed.ac.uk/projects/festival.html (1.96 beta),                    */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
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
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "relp.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void window_this_residual(SArrayInt *frame, SArrayInt *residual, sint32 first_sample,
								 sint32 last_sample, SWinFunc *window, sint32 centre,
								 s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void window_residuals(SRelp *self, SArrayInt *residual,
							  STrackFloat *coefs, uint frame_number, s_erc *error)
{
	float prev_pm;
	float first_pos;
	uint32 j = 0;
	float current_pm = 0.0;
	float period = 0.0;
	uint32 pm_num_frames = coefs->data->row_count;
	sint32 centre_sample = 0;
	sint32 first_sample;
	sint32 last_sample;


	S_CLR_ERR(error);

	/*
	 * estimate first period as pitchmark time itself (i.e. assume a previous
	 * pitchmark at 0.0 time, waveform sample 0)
	 */
	prev_pm = 0.0;


	if (self->symmetric == TRUE)
	{
		if (pm_num_frames < 1)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "window_residuals",
					  "Attempted to window around less than 1 pitchmark");
			return;
		}

		for (j = 0; j < pm_num_frames; j++, frame_number++ )
		{
			current_pm = coefs->time[j];
			period = current_pm - prev_pm;
			centre_sample = (sint32)(current_pm * (float)self->sample_rate + 0.5);
			first_pos = prev_pm - (period * (self->window_factor - 1.0));
			first_sample = (int)(first_pos * (float)self->sample_rate + 0.5);
			last_sample  = (2 * centre_sample) - first_sample;

			self->residuals[frame_number] = S_NEW(SArrayInt, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "window_residuals",
						  "Failed to create new 'SArrayInt' object"))
				return;

			window_this_residual(self->residuals[frame_number], residual,
								 first_sample, last_sample, self->window,
								 -1, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "window_residuals",
						  "Call to \"window_this_residual\" failed"))
				return;

			prev_pm = current_pm;
		}
	}
	else
	{
		if (self->pmIndx == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "window_residuals",
					  "Required pitchmark indices is NULL");
			return;
		}

		if (pm_num_frames < 1)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "window_residuals",
					  "Attempting to window around less than 1 pitchmark");
			return;
		}
		else
		{
			float next_pm;
			float last_pos;

			for (j = 0; j < pm_num_frames - 1; j++, frame_number++)
			{
				current_pm = coefs->time[j];
				period = current_pm - prev_pm;
				centre_sample = (int)(current_pm * (float)self->sample_rate + 0.5);
				first_pos = prev_pm - (period * (self->window_factor - 1.0));
				first_sample = (int)(first_pos * (float)self->sample_rate + 0.5);
				next_pm = coefs->time[j + 1];
				last_pos = next_pm + ((next_pm - current_pm) * (self->window_factor - 1.0));
				last_sample = (int)(last_pos * (float)self->sample_rate + 0.5);

				window_this_residual(self->residuals[frame_number], residual,
									 first_sample, last_sample, self->window,
									 centre_sample, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "window_residuals",
							  "Call to \"window_this_residual\" failed"))
					return;

				self->pmIndx->i[frame_number] = centre_sample - first_sample;
				prev_pm = current_pm;
			}

			/*
			 * last frame window size is set according to coefs->time[end] and the number
			 * of samples in the waveform (it is presumed the waveform begins at the
			 * preceeding pitchmark and ends at the pitchmark following the current
			 * unit...)
			 */
			current_pm = coefs->time[j];
			centre_sample = (int)(current_pm * (float)self->sample_rate + 0.5);
			first_pos = prev_pm - (period * (self->window_factor - 1.0));
			first_sample = (int)(first_pos * (float)self->sample_rate + 0.5);
			last_sample = residual->count - 1;

			window_this_residual(self->residuals[frame_number], residual,
								 first_sample, last_sample, self->window,
								 -1, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "window_residuals",
						  "Call to \"window_this_residual\" failed"))
				return;


			self->pmIndx->i[frame_number] = centre_sample - first_sample;
			frame_number++;
		}
	}
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void window_this_residual(SArrayInt *frame, SArrayInt *residual, sint32 first_sample,
								 sint32 last_sample, SWinFunc *window, sint32 centre,
								 s_erc *error)
{
	uint32 i;
	sint32 j;
	sint32 send;
	sint32 window_length = (last_sample - first_sample) + 1;


	S_CLR_ERR(error);

	frame->i = S_CALLOC(sint32, window_length);
	if (frame->i == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "window_this_residual",
				  "Failed to allocate memory for 'sint32' object");
		return;
	}

	frame->count = window_length;

	/* Ensure we have a safe end */
	if (last_sample < (sint32)residual->count)
		send = last_sample;
	else
		send = residual->count;

	if (centre < 0)
	{
		/* symmetric window */
		S_WINFUNC_CALL(window, init_window)(&window, window_length,
											centre, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "window_this_residual",
					  "Call to SWinFunc method \"init_window\" failed"))
			return;
	}
	else
	{
		/* asymmetric window */
		S_WINFUNC_CALL(window, init_window)(&window, window_length,
											centre - first_sample, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "window_this_residual",
					  "Call to SWinFunc method \"init_window\" failed"))
			return;
	}

	for (i = 0, j = first_sample; j < 0; i++, j++)
		frame->i[i] = 0;
	for (; j < send; i++, j++)
		frame->i[i] = (int)((float)residual->i[j] * window->samples[i] + 0.5);
	for (; j < last_sample; i++, j++)
		frame->i[i] = 0;
}


