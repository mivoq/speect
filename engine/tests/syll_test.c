/************************************************************************************/
/* Copyright (c) 2015 Mivoq SRL <info@mivoq.it>                                     */
/*                                                                                  */
/* Contributors: Giovanni Mazzocchin                                                */
/*               Giulio Paci                                                        */
/*                                                                                  */
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
/* AUTHOR  : Giovanni Mazzocchin <giovanni.mazzocchin@gmail.com>                    */
/* DATE    : June 2016       	                                                    */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Synthesis example.                                                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "speect.h"
#include "syllabification.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

#define SPCT_WAIT_MEM 0
#define MAX_PHONEME_LENGTH 10

/* this is a simple macro to wait for user input so that we can do
 * memory testing, it does nothing if SPCT_WAIT_MEM is 0.
 */
#if SPCT_WAIT_MEM
#  define SPCT_PRINT_AND_WAIT(STR)		\
	do {					\
		printf(STR);			\
		getchar();			\
	} while (0)
#else /* SPCT_WAIT_MEM == 0 */
#  define SPCT_PRINT_AND_WAIT(STR)
#endif

/************************************************************************************/
/*                                                                                  */
/*  Structs                                                                         */
/*                                                                                  */
/************************************************************************************/

struct Config {
	const char * voicefile;
};

/************************************************************************************/
/*                                                                                  */
/*  Static function implementations                                                 */
/*                                                                                  */
/************************************************************************************/

static int usage(FILE*of) {
	return fprintf(of, "usage: speect_test -v VOICEFILE\n"
	       "  Load a syllabifier from the voice specification in VOICEFILE\n"
	       "  and syllabifies phones on standard input.\n"
	       "  --help      Output usage string\n");
}

/************************************************************************************/
/*                                                                                  */
/*  Functions declaration                                                           */
/*                                                                                  */
/************************************************************************************/

static int parse_arguments (int *, char ** , struct Config *);

/************************************************************************************/
/*                                                                                  */
/*  Main function                                                                   */
/*                                                                                  */
/************************************************************************************/

int main(int argc, char **argv)
{
	s_erc error = S_SUCCESS;

	/* the syllabification can be started from the voice */
	const SVoice *voice = NULL;

	SRelation *wordRel = NULL;

	SUtterance *utt = NULL;

	SItem *wordItem = NULL;

	SList *phones = NULL;

	SList *syllablesPhones = NULL;

	char* buffer = NULL;
	size_t buffer_size = 0;
	ssize_t buffer_length = 0;

	/* Config type will handle the command line input */
	struct Config config = {0};

	SPCT_PRINT_AND_WAIT("parsing options, press ENTER\n");

	parse_arguments(&argc, argv, &config);

	SPCT_PRINT_AND_WAIT("going to initialize speect, press ENTER\n");

	/*
	 * initialize speect
	 */
	error = speect_init(NULL);
	if (error != S_SUCCESS) {
		printf("Failed to initialize Speect\n");
		return 1;
	}

	SPCT_PRINT_AND_WAIT("initialized speect, loading voice, press ENTER\n");

	/* load voice */
	voice = s_vm_load_voice(config.voicefile, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
		      "main",
		      "Call to \"s_vm_load_voice\" failed"))
		goto quit;

	SPCT_PRINT_AND_WAIT("loaded voice, doing syllabification, press ENTER\n");

	const SUttProcessor *uttProc = SVoiceGetUttProc(voice, "LexLookup", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
		      "main",
		      "Call to \"SVoiceGetUttProc\" failed"))
		goto quit;


	utt = S_NEW(SUtterance, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
		      "main",
		      "Call to \"S_NEW\" failed"))
		goto quit;

	/* Utterance initialization */
	SUtteranceInit(&utt, voice, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
		      "main",
		      "Call to \"SUtteranceInit\" failed"))
		goto quit;

	/* Create new relation */
	wordRel = SUtteranceNewRelation(utt, "Word", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
		      "main",
		      "Call to \"NewRelation\" failed"))
		goto quit;

	wordItem = SRelationAppend(wordRel, NULL, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
		      "main",
		      "Call to \"SRelationAppend\" failed"))
		goto quit;

	SItemSetName(wordItem, "", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
		      "main",
		      "Call to \"SItemSetName\" failed"))
		goto quit;

	SPCT_PRINT_AND_WAIT("load syllabification function, press ENTER\n");

	SSyllabification* syllab = (SSyllabification*)SVoiceGetData(voice , "syllabification", &error);
	if (syllab == NULL)
	{
		syllab = (SSyllabification*)SMapGetObjectDef(uttProc -> features , "_syll_func", NULL,
							     &error);

		if (S_CHK_ERR(&error, S_CONTERR,
			      "main",
			      "Call to \"SMapGetObjectDef\" failed"))
			goto quit;
	}

	SPCT_PRINT_AND_WAIT("everything ready to perform syllabification, press ENTER\n");
	char inter_buffer[MAX_PHONEME_LENGTH+1] = {0};

	buffer_length = getline(&buffer, &buffer_size, stdin);
	while (buffer_length != -1)
	{
		phones = S_LIST(S_NEW(SListList, &error));
		if (S_CHK_ERR(&error, S_CONTERR,
			      "main",
			      "Call to \"S_NEW\" failed"))
			goto quit;

		int j = 0;
		while (buffer[j] != '\0')
		{
			int k;
			int inter_buffer_c;

			if (!isspace(buffer[j]))
			{
				inter_buffer[0] = buffer[j];

				k = j + 1;
				inter_buffer_c = 1;

				while (!isspace(buffer[k]) && inter_buffer_c < MAX_PHONEME_LENGTH)
				{
					inter_buffer[inter_buffer_c] = buffer[k];

					inter_buffer_c++;
					k++;
				}

				j += (inter_buffer_c - 1);
				inter_buffer[inter_buffer_c] = '\0';

				/* for each phoneme in the line, put it in 'ph' */
				SObject* ph = SObjectSetString(inter_buffer, &error);

				/* then add ph to the phoneme list */
				SListAppend(phones, ph, &error);
				if (S_CHK_ERR(&error, S_CONTERR,
					      "main",
					      "Call to \"SListAppend\" failed"))
					goto quit;
			}
			j++;
		}

		SPCT_PRINT_AND_WAIT("run syllabification on next phones string, press ENTER\n");
		syllablesPhones = NULL;
		syllablesPhones = S_SYLLABIFICATION_CALL(syllab, syllabify)(syllab,
									    wordItem,
									    phones,
									    &error);
		if (S_CHK_ERR(&error, S_CONTERR,
			      "main",
			      "Call to method \"syllabify\" failed"))
			goto quit;


		/* PRINT LOOP */
		uint32 k = 0;
		SIterator *itr_syllablesPhones = S_ITERATOR_GET(syllablesPhones, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
				  "Run",
				  "Execution of \"S_ITERATOR_GET\" failed"))
			goto quit;

		size_t syllables_list_size = SListSize(syllablesPhones, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
			      "main",
			      "Call to method \"SListSize\" failed"))
			goto quit;

		while (itr_syllablesPhones != NULL)
		{
			const SList* syl_list = (const SList*) SIteratorObject(itr_syllablesPhones, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
				      "main",
				      "Call to method \"SIteratorObject\" failed"))
				goto quit;

			size_t syl_list_size = SListSize(syl_list, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
				      "main",
				      "Call to method \"SListSize\" failed"))
				goto quit;

			uint32 l = 0;
			SIterator *itr_syl_list = S_ITERATOR_GET(syl_list, &error);
			while (itr_syl_list != NULL)
			{
				const SObject* ph = SIteratorObject(itr_syl_list, &error);

				if (S_CHK_ERR(&error, S_CONTERR,
					      "main",
					      "Call to method \"SIteratorObject\" failed"))
					goto quit;

				const char* phone_string = SObjectGetString(ph, &error);
				if (S_CHK_ERR(&error, S_CONTERR,
					      "main",
					      "Call to method \"SObjectGetString\" failed"))
					goto quit;

				fprintf(stdout, "%s", phone_string);
				if (l < syl_list_size - 1){
					fprintf(stdout, " ");
				}

				itr_syl_list = SIteratorNext(itr_syl_list);
				l++;
			}
			if (k < syllables_list_size - 1)
			{
				fprintf(stdout, " - ");
			}

			itr_syllablesPhones = SIteratorNext(itr_syllablesPhones);
			k++;
		}
		fprintf(stdout, "\n");


		if (syllablesPhones != NULL)
			S_DELETE(syllablesPhones, "main", &error);

		if (phones != NULL)
			S_DELETE(phones, "main", &error);
		phones = NULL;
		buffer_length = getline(&buffer, &buffer_size, stdin);
	}

quit:
	SPCT_PRINT_AND_WAIT("deleting iteration support structures, press ENTER\n");

	if (buffer != NULL)
		free(buffer);

	if (phones != NULL)
		S_DELETE(phones, "main", &error);

	if (syllablesPhones != NULL)
		S_DELETE(syllablesPhones, "main", &error);

	SPCT_PRINT_AND_WAIT("deleting utterance, press ENTER\n");

	if (utt != NULL)
		S_DELETE(utt, "main", &error);

	SPCT_PRINT_AND_WAIT("deleting voice, press ENTER\n");

	if (voice != NULL)
		S_DELETE(voice, "main", &error);

	SPCT_PRINT_AND_WAIT("quitting speect, press ENTER\n");

	/*
	 * quit speect
	 */
	error = speect_quit();
	if (error != S_SUCCESS)
	{
		printf("Call to 'speect_quit' failed\n");
		return 1;
	}

	SPCT_PRINT_AND_WAIT("done, press ENTER\n");

	return 0;
}

static int parse_arguments (int *argc, char **argv, struct Config *config) {
	int scomp;
	int i;
	int j = 0;

	for (i = 1; i < *argc; i++)
	{
		scomp = strcmp(argv[i], "-h");
		if (scomp == 0) {
			usage(stdout);
			exit(0);
		}

		scomp = strcmp(argv[i], "--help");

		if (scomp == 0) {
			usage(stdout);
			exit(0);
		}

		scomp = strcmp(argv[i],"-v");
		if ((scomp == 0) && (i + 1 < *argc))
		{
			config->voicefile = argv[i + 1];
			i++;
			j += 2;
		}

	}

	if ((config->voicefile == NULL)) {
		fprintf(stderr, "%s\n", "Arguments are not optional, see usage");
		usage(stderr);
		exit(1);
	}
	argv = argv + j;
	*argc = *argc - j;
	return i;
}
