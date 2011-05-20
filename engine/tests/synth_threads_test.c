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
/* DATE    : July 2010                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Synthesis with threads test.                                                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "speect.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

typedef struct
{
	const char *voicefile;
	const char *wavfile;
	const char *text;
	s_erc error;
	uint id;
} targ;


/************************************************************************************/
/*                                                                                  */
/*  Static function implementations                                                 */
/*                                                                                  */
/************************************************************************************/

static void usage(int rv)
{
    printf("usage: synth_threads_test -n NUMTHREADS -t TEXT -v VOICEFILE -o WAVEFILE\n"
           "  Converts text in TEXT, with voice specification in VOICEFILE\n"
		   "  to a waveform in WAVEFILE with NUMTHREADS running concurrently.\n"
		   "  None of the arguments are optional.\n"
           "  --help      Output usage string\n");
	exit(rv);
}



void *child_fn(void *args)
{
	SVoice *voice = NULL;
	SUtterance *utt = NULL;
	const SObject *audio;
	SPlugin *riffAudio = NULL;
	targ *myargs;
	char *wavfile = NULL;
	s_erc *error;


	myargs = (targ*)args;
	error = &(myargs->error);


	S_CLR_ERR(error);

	/* load audio riff plug-in, so that we can save the audio */
	riffAudio = s_pm_load_plugin("audio_riff.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "child_fn",
				  "Call to \"s_pm_load_plugin\" failed"))
		pthread_exit((void*)myargs);

	/* load voice */
	voice = s_vm_load_voice(myargs->voicefile, TRUE, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "child_fn",
				  "Call to \"s_vm_load_voice\" failed"))
	{
		S_DELETE(riffAudio, "child_fn", error);
		pthread_exit((void*)myargs);
	}

	/* synthesize utterance */
	utt = SVoiceSynthUtt(voice, "text", SObjectSetString(myargs->text, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "child_fn",
				  "Call to \"SVoiceSynthUtt\" failed"))
	{
		S_DELETE(riffAudio, "child_fn", error);
		S_DELETE(voice, "child_fn", error);
		pthread_exit((void*)myargs);
	}

	/* get audio object */
	audio = SUtteranceGetFeature(utt, "audio", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "child_fn",
				  "Call to \"SUtteranceGetFeature\" failed"))
	{
		S_DELETE(riffAudio, "child_fn", error);
		S_DELETE(utt, "child_fn", error);
		S_DELETE(voice, "child_fn", error);
		pthread_exit((void*)myargs);
	}

	s_asprintf(&wavfile, error, "%s%d\n", myargs->wavfile, myargs->id);
	if (S_CHK_ERR(error, S_CONTERR,
				  "child_fn",
				  "Call to \"s_asprintf\" failed"))
	{
		S_DELETE(riffAudio, "child_fn", error);
		S_DELETE(utt, "child_fn", error);
		S_DELETE(voice, "child_fn", error);
		pthread_exit((void*)myargs);
	}

	/* save audio */
	SObjectSave(audio, wavfile, "riff", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "child_fn",
				  "Call to \"SObjectSave\" failed"))
	{
		S_DELETE(riffAudio, "child_fn", error);
		S_DELETE(utt, "child_fn", error);
		S_DELETE(voice, "child_fn", error);
		S_FREE(wavfile);
		pthread_exit((void*)myargs);
	}


	S_DELETE(riffAudio, "child_fn", error);
	S_DELETE(utt, "child_fn", error);
	S_DELETE(voice, "child_fn", error);
	S_FREE(wavfile);
	pthread_exit((void*)myargs);
}


/************************************************************************************/
/*                                                                                  */
/*  Main function                                                                   */
/*                                                                                  */
/************************************************************************************/


int main(int argc, char **argv)
{
	s_erc error = S_SUCCESS;
	int i;
	int scomp;
	const char *wavfile = NULL;
	const char *voicefile = NULL;
	const char *text = NULL;
	uint num_threads = 2;
	pthread_t *threads;
	pthread_attr_t attr;
	uint t;
	int rc;

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

		scomp = s_strcmp(argv[i],"-n", &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Call to \"s_strcmp\" failed"))
			return 1;

		if ((scomp == 0) && (i + 1 < argc))
		{
			num_threads = (uint)s_atof(argv[i+1], &error);
			if (S_CHK_ERR(&error, S_CONTERR,
						  "main",
						  "Call to \"s_atof\" failed"))
				return 1;
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

	/* Initialize and set thread join attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	threads = S_CALLOC(pthread_t, num_threads);
	if (threads == NULL)
	{
		S_FTL_ERR(&error, S_MEMERROR,
				  "main",
				  "Failed to allocate memory for 'pthread_t' objects");
		pthread_attr_destroy(&attr);
		return 1;
	}


	for(t = 0; t < num_threads; t++)
	{
		targ *thread_args;


		thread_args = S_CALLOC(targ, 1);
		thread_args->voicefile = voicefile;
		thread_args->wavfile = wavfile;
		thread_args->text = text;
		thread_args->id = t;
		S_CLR_ERR(&(thread_args->error));


		rc = pthread_create(&threads[t], NULL, child_fn, (void *)thread_args);
		if (rc)
		{
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			printf("Code %d= %s\n",rc,strerror(rc));
			S_FREE(threads);
			pthread_attr_destroy(&attr);
			exit(-1);
		}
	}

	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
 	for(t = 0; t < num_threads; t++)
	{
		targ *thread_args;

		rc = pthread_join(threads[t], (void**)&thread_args);
		if (rc)
		{
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			S_FREE(threads);
			pthread_attr_destroy(&attr);
			exit(-1);
		}

		printf("Main: completed join with thread %d having a error status of %d\n",t, thread_args->error);
		S_FREE(thread_args);
	}

	S_FREE(threads);

	/*
	 * quit speect
	 */
	error = speect_quit();
	if (error != S_SUCCESS)
	{
		printf("Call to 'speect_quit' failed\n");
		return 1;
	}

	printf("Main: program completed. Exiting.\n");
	pthread_exit(NULL);
}

