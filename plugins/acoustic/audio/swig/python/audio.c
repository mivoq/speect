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

%inline
%{
	void saudio_samples(char **s, int *slen, const SAudio *audio, s_erc *error)
	{
		sint16 *samples;
		uint32 counter;


		samples = S_CALLOC(sint16, audio->num_samples);
		if (samples == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "saudio_samples",
					  "Failed to allocate audio samples");
			return;
		}

		for (counter = 0; counter < audio->num_samples; counter++)
			samples[counter] = (sint16)audio->samples[counter];

		*slen = audio->num_samples * sizeof(sint16);
		*s = (char*)samples;
	}
%}


%nodefaultctor SAudio;

typedef struct
{
} SAudio;


%types(SAudio = SObject);


%extend SAudio
{
	PyObject *get_audio_waveform()
	{
		return NULL;
	}

	uint32 num_samples()
	{
		return $self->num_samples;
	}

	uint32 sample_rate()
	{
		return $self->sample_rate;
	}
}
