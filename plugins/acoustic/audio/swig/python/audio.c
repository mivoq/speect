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

%include "cstring.i"


%cstring_output_allocate_size(char **s, int *slen, free(*$1));

void py_saudio_samples(char **s, int *slen, const SObject *audio);


/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	void py_saudio_samples(char **s, int *slen, const SObject *audio)
	{
		sint16 *samples;
		SAudio *self = S_AUDIO(audio);
		uint32 counter;


		samples = S_CALLOC(sint16, self->num_samples);
		for (counter = 0; counter < self->num_samples; counter++)
			samples[counter] = (sint16)self->samples[counter];

		*slen = self->num_samples * sizeof(sint16);
		*s = (char*)samples;
	}


	uint32 py_saudio_num_samples(const SObject *audio)
	{
		SAudio *self = S_AUDIO(audio);


		return self->num_samples;
	}

	uint32 py_saudio_sample_rate(const SObject *audio)
	{
		SAudio *self = S_AUDIO(audio);


		return self->sample_rate;
	}

/*
 * Do not delete this delimiter, required for SWIG
 */
%}
