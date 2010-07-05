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


/************************************************************************************/
/*                                                                                  */
/*  Static function implementations                                                 */
/*                                                                                  */
/************************************************************************************/

static void usage(int rv)
{
    printf("usage: synth_test -t TEXT -v VOICEFILE -o WAVEFILE\n"
           "  Converts text in TEXT, with voice specification in VOICEFILE\n"
		   "  to a waveform in WAVEFILE.\n"
		   "  None of the arguments are optional.\n"
           "  --help      Output usage string\n");
	exit(rv);
}


/************************************************************************************/
/*                                                                                  */
/*  Main function                                                                   */
/*                                                                                  */
/************************************************************************************/


int main(int argc, char **argv)
{
	s_erc error = S_SUCCESS;
	SVoice *voice = NULL;
	SUtterance *utt = NULL;
	const SObject *audio;
	SPlugin *riffAudio = NULL;
	int i;
	int scomp;
	const char *wavfile = NULL;
	const char *voicefile = NULL;
	const char *text = NULL;


	/*
	 * initialize speect
	 */
	error = speect_init();
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}

	/* parse options */
    for (i=1; i<argc; i++)
    {
		scomp = s_strcmp(argv[i],"-h", &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_strcmp\" failed"))
			return 1;

		if (scomp == 0)
			usage(0);

		scomp = s_strcmp(argv[i],"--help", &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_strcmp\" failed"))
			return 1;

		if (scomp == 0)
			usage(0);

		scomp = s_strcmp(argv[i],"-t", &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_strcmp\" failed"))
			return 1;

		if ((scomp == 0) && (i + 1 < argc))
		{
			text = argv[i+1];
			i++;
		}

		scomp = s_strcmp(argv[i],"-v", &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_strcmp\" failed"))
			return 1;

		if ((scomp == 0) && (i + 1 < argc))
		{
			voicefile = argv[i+1];
			i++;
		}

		scomp = s_strcmp(argv[i],"-o", &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_strcmp\" failed"))
			return 1;

		if ((scomp == 0) && (i + 1 < argc))
		{
			wavfile = argv[i+1];
			i++;
		}
	}

	if ((wavfile == NULL) || (voicefile == NULL) || (text == NULL))
	{
		S_CTX_ERR(&error, S_ARGERROR,
				  "main",
				  "Arguments are not optional, see usage");
		usage(1);
	}

	/* load audio riff plug-in, so that we can save the audio */
	riffAudio = s_pm_load_plugin("audio_riff.spi", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_pm_load_plugin\" failed"))
		goto quit;

	/* load voice */
	voice = s_vm_load_voice(voicefile, TRUE, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"s_vm_load_voice\" failed"))
		goto quit;

	/* synthesize utterance */
	utt = SVoiceSynthUtt(voice, "text", SObjectSetString(text, &error), &error);
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
	SObjectSave(audio, wavfile, "riff", &error);
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

