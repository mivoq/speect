/************************************************************************************/
/* Copyright (c) 2015 Mivoq SRL <info@mivoq.it>                                     */
/*                                                                                  */
/* Contributors: Giovanni Mazzocchin                                                */
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


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "speect.h"
#include "../../plugins/linguistic/syllabification/src/syllabification.h"


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
  do {						\
    printf(STR);				\
    getchar();					\
  } while (0)
#else /* SPCT_WAIT_MEM == 0 */
#  define SPCT_PRINT_AND_WAIT(STR)
#endif

/************************************************************************************/
/*                                                                                  */
/*  Structs                                                                          */
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

static void usage(int rv) {
  printf("usage: speect_test -t TEXT -v VOICEFILE\n"
	 "  Converts text in TEXT, with voice specification in VOICEFILE\n"
	 "  to a step indicated in UTTERANCE_TYPE or to a waveform in WAVEFILE or.\n"
	 "  None of the arguments are optional.\n"
	 "  --help      Output usage string\n");
  exit(rv);
}

/************************************************************************************/
/*                                                                                  */
/*  Functions declaration                                                           */
/*                                                                                  */
/************************************************************************************/

int init (int *, char ** , struct Config *, s_erc *);

/************************************************************************************/
/*                                                                                  */
/*  Main function                                                                   */
/*                                                                                  */
/************************************************************************************/

int main(int argc, char **argv) 
{
  s_erc error = S_SUCCESS;
  
  /* the syllabification can be started from the voice */
  const SVoice *voice;
  
  const SRelation *wordRel;
 
  SUtterance *utt = NULL;
  
  /* I must set a voice for this item */
  SItem *wordItem = NULL;
  
  SList *phones = NULL; 
  
  SList *syllablesPhones; 
  
  /* Config type will handle the command line input */
  struct Config config = {0};

  SPCT_PRINT_AND_WAIT("going to initialize speect, press ENTER\n");

  /*
   * initialize speect
   */
  error = speect_init(NULL);
  if (error != S_SUCCESS) 
  {
    printf("Failed to initialize Speect\n");
    return 1;
  }
  
  SPCT_PRINT_AND_WAIT("initialized speect, parsing options, press ENTER\n");

  init(&argc, argv, &config, &error);
  if (S_CHK_ERR(&error, S_CONTERR,
				"main",
				"Failed to load arguments"))
      goto quit;

  /* load voice, initializations */
  voice = s_vm_load_voice(config.voicefile, &error);
  if (S_CHK_ERR(&error, S_CONTERR,
				"main",
				"Call to \"s_vm_load_voice\" failed"))
      goto quit;
      
  SPCT_PRINT_AND_WAIT("loaded voice, doing synthesis, press ENTER\n");
   
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
      
  wordRel = S_NEW(SRelation, &error);
  if (S_CHK_ERR(&error, S_CONTERR,
				"main",
				"Call to \"S_NEW\" failed"))
      goto quit;
 
  wordItem = S_NEW(SItem, &error);
  if (S_CHK_ERR(&error, S_CONTERR,
				"main",
				"Call to \"S_NEW\" failed"))
      goto quit;
      
  phones = S_LIST(S_NEW(SListList, &error)); 
  if (S_CHK_ERR(&error, S_CONTERR,
				"main",
				"Call to \"S_NEW\" failed"))
	  goto quit;

  SUtteranceInit(&utt, voice, &error);
  if (S_CHK_ERR(&error, S_CONTERR,
				"main",
				"Call to \"SUtteranceInit\" failed"))
      goto quit;
  
  SRelationInit(&wordRel, "wordRel", &error);
  if (S_CHK_ERR(&error, S_CONTERR,
				"main",
				"Call to \"SRelationInit\" failed"))
      goto quit;
  
  SUtteranceSetRelation(utt, wordRel, &error);
  if (S_CHK_ERR(&error, S_CONTERR,
				"main",
				"Call to \"SUtteranceSetRelation\" failed"))
	  goto quit;
  
  SItemInit(&wordItem, wordRel, NULL, &error);
  if (S_CHK_ERR(&error, S_CONTERR,
				"main",
				"Call to \"SItemInit\" failed"))
      goto quit;
  
  SItemSetName(wordItem, "ciao", &error);
  if (S_CHK_ERR(&error, S_CONTERR,
				"main",
				"Call to \"SItemSetName\" failed"))
      goto quit;
     
  SSyllabification* syllab = (SSyllabification*)SVoiceGetData(voice , "syllabification", &error);

  if (syllab == NULL)
  {
	  syllab = (SSyllabification*)SMapGetObjectDef(uttProc -> features , "_syll_func", NULL,
												   &error);
												   
	  if (S_CHK_ERR(&error, S_CONTERR,
					"main",
				    "Call to \"SMapGetObjectDef\" failed"))
			return;		
  }
  
  /* 'buffer' for input lines */
  char* buffer = malloc(sizeof(char) * 200);  
  size_t dimBuffer = sizeof(buffer); 
  ssize_t input_size = 0;
  /* 'inter_buffer' for phonemes in the lines */
  char* inter_buffer = malloc(sizeof(char) * (MAX_PHONEME_LENGTH + 1));
   
  input_size = getline(&buffer, &dimBuffer, stdin);	
  while (input_size != -1)
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
		/* ################# SYLLABIFICATION CALL #################
		   here syllabify phones by the plugin call */
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
		size_t syllables_list_size = SListSize(syllablesPhones, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
		        	  "main",
				      "Call to method \"SListSize\" failed"))
		    goto quit;
   
        while (k < syllables_list_size)
        {
			SList* syl_list = SListNth(syllablesPhones, k, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
						  "main",
				          "Call to method \"SListNth\" failed"))
		        goto quit;
		  
	        size_t syl_list_size = SListSize(syl_list, &error);
	        if (S_CHK_ERR(&error, S_CONTERR,
		          		  "main",
				          "Call to method \"SListSize\" failed"))
		        goto quit;
	   
	        uint32 l = 0;
	        while (l < syl_list_size)
	        {
				SObject* ph = SListNth(syl_list, l, &error);
				if (S_CHK_ERR(&error, S_CONTERR,
							  "main",
				              "Call to method \"SListNth\" failed"))
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
			
		        l++;
			}
			if (k < syllables_list_size - 1)
			{	
				fprintf(stdout, " - ");
			}
			k++;
	   }
	   fprintf(stdout, "\n");
	  
	   input_size = getline(&buffer, &dimBuffer, stdin);
  }   
      

  free(buffer);
  free(inter_buffer);

  SPCT_PRINT_AND_WAIT("synthesized utterance, getting audio object, press ENTER\n");
      
  goto quit;

 quit:
  SPCT_PRINT_AND_WAIT("deleting utterance, press ENTER\n");

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

int init (int *argc, char **argv, struct Config *config, s_erc *error) {
  int scomp;
  int i;
  int j = 0;
  
  for (i = 1; i < *argc; i++) 
  {
    scomp = s_strcmp(argv[i], "-h", error);
    if (S_CHK_ERR(error, S_CONTERR,
		  "init",
		  "Call to \"s_strcmp\" failed"))		  
      goto returnFunction;

    if (scomp == 0)
      usage(0);

    scomp = s_strcmp(argv[i], "--help", error);
    if (S_CHK_ERR(error, S_CONTERR,
		  "init",
		  "Call to \"s_strcmp\" failed"))
      goto returnFunction;

    if (scomp == 0)
      usage(0);

    scomp = s_strcmp(argv[i],"-v", error);
    if (S_CHK_ERR(error, S_CONTERR,
				  "init",
				  "Call to \"s_strcmp\" failed"))
        goto returnFunction;

    if ((scomp == 0) && (i + 1 < *argc))
    {
		config -> voicefile = argv[i + 1];
		i++;
		j += 2;
    }

  }

  if ((config -> voicefile == NULL)) {
    S_CTX_ERR(error, S_ARGERROR,
	      "init",
	      "Arguments are not optional, see usage");
    usage(1);
  }

  returnFunction:
  argv = argv + j;
  *argc = *argc - j;
 
  return i;
}
