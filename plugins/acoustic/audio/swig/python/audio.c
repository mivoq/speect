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

%define audio_DOCSTRING
"""
A container class for audio objects. Provides a Speect native class for creating/adding audio
objects. The audio samples are internally represented as floats.
"""
%enddef

%feature("autodoc", audio_DOCSTRING) SAudio;


%define num_samples_DOCSTRING
"""
num_samples()

Return the number of samples of this audio object.

:return: Number of samples of this audio object.
:rtype: int
"""
%enddef

%feature("autodoc", num_samples_DOCSTRING) SAudio::num_samples;


%define sample_rate_DOCSTRING
"""
sample_rate()

Return the sample rate of the samples of this audio object.

:return: Sample rate of this audio object (Hertz).
:rtype: int
"""
%enddef

%feature("autodoc", sample_rate_DOCSTRING) SAudio::sample_rate;


%include "cstring.i"

%cstring_output_allocate_size(char **s, int *slen, free(*$1));

%pythoncode
%{
import ossaudiodev

def utt_play(self):
    """
    Play utterance waveform. Will do nothing
    if the utterance does not have an "audio"
    feature.

    :note: This function tries to open ``/dev/dsp1``
           and write 16bit little endian values to it.
    """

    audio = self.features["audio"]
    if audio is None:
        return

    waveform = audio.get_audio_waveform()

    dsp = ossaudiodev.open("/dev/dsp", "w")
    dsp.setparameters(ossaudiodev.AFMT_S16_LE,
                      1,
                      waveform["samplerate"],
                      True)

    dsp.writeall(waveform["samples"])
    dsp.close()

setattr(speect.SUtterance, "play", utt_play)
%}

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


typedef struct
{
} SAudio;


%types(SAudio = SObject);

%nodefaultctor SAudio;

%nodefaultdtor SAudio;

%extend SAudio
{
	uint32 num_samples()
	{
		return $self->num_samples;
	}


	uint32 sample_rate()
	{
		return $self->sample_rate;
	}


%pythoncode
%{
def get_audio_waveform(self):
    """
    Return the waveform of this SAudio object in Python dict  e.g.::
    
        {'sampletype': 'int16', 
         'samplerate': 16000,
         'samples': '...bytestring...samples....'}


    :return: Audio waveform.
    :rtype: dict
    :note: Currently supports only 'int16' sample types.
    """

    sample_type = "int16"   # currently only option

    sample_rate = self.sample_rate()
    samples = saudio_samples(self)
    wave_dict = {
        "sampletype" : sample_type,
        "samplerate" : sample_rate,
        "samples" : samples
        }
    return wave_dict
%}
}
