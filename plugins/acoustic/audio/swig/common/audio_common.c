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
/* SWIG common C convenience functions for SAudio.                                  */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* SWIG helper functions                                                            */
/*                                                                                  */
/************************************************************************************/

%include "cstring.i"

%cstring_output_allocate_size(char **s, int *slen, free(*$1));


/************************************************************************************/
/*                                                                                  */
/* SAudio helper functions                                                          */
/*                                                                                  */
/************************************************************************************/

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


/************************************************************************************/
/*                                                                                  */
/* Extend the SAudio class                                                          */
/*                                                                                  */
/************************************************************************************/


typedef struct
{
} SAudio;

%types(SAudio = SObject);

%nodefaultctor SAudio;

%nodefaultdtor SAudio;

%extend SAudio
{
	void resize(uint32 new_size, s_erc *error)
	{
		S_CLR_ERR(error);

		if (!S_AUDIO_METH_VALID($self, resize))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "resize",
					  "Audio method \"resize\" not implemented");
			return;
		}

		S_AUDIO_CALL($self, resize)($self, new_size, error);
		S_CHK_ERR(error, S_CONTERR,
				  "resize",
				  "Call to Audio method \"resize\" failed");
		return;
	}


	void scale(float factor, s_erc *error)
	{
		S_CLR_ERR(error);

		if (!S_AUDIO_METH_VALID($self, scale))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "scale",
					  "Audio method \"scale\" not implemented");
			return;
		}

		S_AUDIO_CALL($self, scale)($self, factor, error);
		S_CHK_ERR(error, S_CONTERR,
				  "scale",
				  "Call to Audio method \"scale\" failed");
		return;
	}


	uint32 num_samples()
	{
		return $self->num_samples;
	}


	uint32 sample_rate()
	{
		return $self->sample_rate;
	}
};

