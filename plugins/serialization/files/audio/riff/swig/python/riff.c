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
/* C convenience functions for SAudio riff format Python wrapper.                   */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%inline
%{
	void save_audio_riff_inline(const SUtterance *utt, const char *path, s_erc *error)
	{
		const SObject *audio;


		audio = SUtteranceGetFeature(utt, "audio", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "save_audio_riff",
					  "Call to \"SUtteranceGetFeature\" failed"))
			return;

		if (audio == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "save_audio_riff",
					  "Utterance does not have an 'audio' feature");
			return;
		}

		/* save audio */
		SObjectSave(audio, path, "riff", error);
	}
%}


%pythoncode
%{
def save_audio_riff(self, path):
    """
    save_audio_riff(path)

    Save the audio object of the utterance in the RIFF format at
    the given path.

    :param path: Full path and file name where audio should be saved.
    :type path: string
    """

    save_audio_riff_inline(self, path)

	
setattr(speect.SUtterance, "save_audio_riff", save_audio_riff)
%}

