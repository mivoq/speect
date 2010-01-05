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
/* Synthesis example.                                                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


#include <stdio.h>
#include "speect.h"



int main()
{
	s_erc error = S_SUCCESS;
	SVoice *voice = NULL;
	SUtterance *utt = NULL;
	const SObject *audio;
	SPlugin *riffAudio = NULL;


	/*
	 * initialize speect
	 */
	error = speect_init();
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}

	/* load audio riff plug-in, so that we can save the audio */
	riffAudio = s_pm_load_plugin("audio-riff.spi", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_pm_load_plugin\" failed"))
		goto quit;

	/* load voice */
	voice = s_vm_load_voice("/home/aby/Development/speect/voices/english/lwazi/damon/etc/voice.sconf",
							TRUE,
							&error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_vm_load_voice\" failed"))
		goto quit;

	/* synthesize utterance */
	utt = SVoiceSynthUtt(voice, "text", SObjectSetString("hello world, this is the speect synthesis engine", &error), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SVoiceSynthUtt\" failed"))
		goto quit;

	/* get audio object */
	audio = SUtteranceGetFeature(utt, "audio", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SUtteranceGetFeature\" failed"))
		goto quit;

	/* save audio */
	SObjectSave(audio, "test.wav", "riff", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectSave\" failed"))
		goto quit;

quit:
	if (utt != NULL)
		S_DELETE(utt, "main", &error);

	if (voice != NULL)
		S_DELETE(voice, "main", &error);

	if (riffAudio != NULL)
		S_DELETE(riffAudio, "main", &error);

	/*
	 * quit speect
	 */
	error = speect_quit();
	if (error != S_SUCCESS)
	{
		printf("Call to 'speect_quit' failed\n");
		return 1;
	}

	return 0;
}

