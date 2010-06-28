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


/************************************************************************************/
/*                                                                                  */
/* Extend the SUtterance class                                                      */
/*                                                                                  */
/************************************************************************************/

%pythoncode
%{
def _saudio_utt_play(self):
    """
    Play utterance waveform. Will do nothing
    if the utterance does not have an "audio"
    feature.

    :note: Uses SAudio.play() internally. 
    """

    audio = self.features["audio"]
    if audio is None:
        return

    audio.play()


setattr(speect.SUtterance, "play", _saudio_utt_play)
%}


/************************************************************************************/
/*                                                                                  */
/* Extend the SAudio class                                                          */
/*                                                                                  */
/************************************************************************************/

%extend SAudio
{	
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


def play(self):
    """
    Play the audio waveform (supports only 'posix' and 'nt' systems).

    :note: On 'posix' systems, this function tries to 
           open an audio device (environment variable ``AUDIODEV``,
           or fallback ``/dev/dsp``) and write 16 bit *little endian*
           values to it.
    """

    import os
    
    opsys = os.name
    
    if opsys not in ("posix", "nt"):
        raise EnvironmentError("SAudio.play() currently works only on" + \
                               " \"posix\" and \"nt\" compatible systems")

    waveform = self.get_audio_waveform()

    if opsys == "posix":
        import ossaudiodev

        dsp = ossaudiodev.open("w")
        dsp.setparameters(ossaudiodev.AFMT_S16_LE,
                          1,
                          waveform["samplerate"],
                          True)
        dsp.writeall(waveform["samples"])
        dsp.close()
    
    else:     # nt (win)
        import winsound
        
        winsound.PlaySound(waveform["samples"], winsound.SND_MEMORY)

%}
};

