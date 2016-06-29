/************************************************************************************/
/* Copyright (c) 2015 Mivoq SRL <info@mivoq.it>                                     */
/*                                                                                  */
/* Contributors: Matteo Lisotto                                                     */
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
/* AUTHOR  : Matteo Lisotto <matteo.lisotto@gmail.com>                              */
/* DATE    : September 2015                                                         */
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
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

#define SPCT_WAIT_MEM 0

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
  const char * text;
  const char * wavfile;
  const char * voicefile;
  const char * utt_type;
};

/************************************************************************************/
/*                                                                                  */
/*  Static function implementations                                                 */
/*                                                                                  */
/************************************************************************************/

static void usage(int rv) {
  printf("usage: speect_test -t TEXT -v VOICEFILE -m UTTERANCE_TYPE -o WAVEFILE\n"
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


int main(int argc, char **argv) {
  s_erc error = S_SUCCESS;
  SVoice *voice = NULL;
  SUtterance *utt = NULL;
  const SObject *audio;
  SPlugin *riffAudio = NULL;
  SPlugin *textGrid = NULL;
  SPlugin *maryXML = NULL;
  SPlugin *HTSLabelsToXML = NULL;
  struct Config config;
  char *textgrid_file = NULL;
  char *maryxml_file = NULL;
  char *htslabels_file = NULL;
  int isText;

  SPCT_PRINT_AND_WAIT("going to initialize speect, press ENTER\n");

  /*
   * initialize speect
   */
  error = speect_init(NULL);
  if (error != S_SUCCESS) {
    printf("Failed to initialize Speect\n");
    return 1;
  }

  SPCT_PRINT_AND_WAIT("initialized speect, parsing options, press ENTER\n");

  init(&argc, argv, &config, &error);
  if (S_CHK_ERR(&error, S_CONTERR,
		"main",
		"Failed to load arguments"))
    goto quit;

  SPCT_PRINT_AND_WAIT("parsed options, loading audio riff plug-in, press ENTER\n");

  /* load audio riff plug-in, so that we can save the audio */
  riffAudio = s_pm_load_plugin("audio_riff.spi", &error);
  if (S_CHK_ERR(&error, S_CONTERR,
		"main",
		"Call to \"s_pm_load_plugin\" failed"))
    goto quit;

  if (S_CHK_ERR(&error, S_IOEOF,
		"main",
		"Call to \"s_pm_load_plugin\" failed"))
    goto quit;

    if (S_CHK_ERR(&error, S_ARGERROR,
		"main",
		"Call to \"s_pm_load_plugin\" failed"))
    goto quit;




  SPCT_PRINT_AND_WAIT("loaded audio riff plug-in, loading voice, press ENTER\n");

  /* load textgrid plug-in, so that we can save the textgrid */
  textGrid = s_pm_load_plugin("utt_textgrid.spi", &error);
  if (S_CHK_ERR(&error, S_CONTERR,
		"main",
		"Call to \"s_pm_load_plugin\" failed"))
    goto quit;

  SPCT_PRINT_AND_WAIT("loaded textgrid plug-in, loading voice, press ENTER\n");

  /* load maryxml plug-in, so that we can save the maryxml */
  maryXML = s_pm_load_plugin("utt_maryxml.spi", &error);
  if (S_CHK_ERR(&error, S_CONTERR,
		"main",
		"Call to \"s_pm_load_plugin\" failed"))
    goto quit;

  SPCT_PRINT_AND_WAIT("loaded maryxml plug-in, loading voice, press ENTER\n");

  /* load maryxml plug-in, so that we can save the maryxml */
  HTSLabelsToXML = s_pm_load_plugin("utt_htslabelsexport.spi", &error);
  if (S_CHK_ERR(&error, S_CONTERR,
		"main",
		"Call to \"s_pm_load_plugin\" failed"))
    goto quit;

  SPCT_PRINT_AND_WAIT("loaded htslabelsexport plug-in, loading voice, press ENTER\n");

  /* load voice */
  voice = s_vm_load_voice(config.voicefile, &error);
  if (S_CHK_ERR(&error, S_CONTERR,
		"main",
		"Call to \"s_vm_load_voice\" failed"))
    goto quit;

  SPCT_PRINT_AND_WAIT("loaded voice, doing synthesis, press ENTER\n");

  /* synthesize utterance */
  utt = SVoiceSynthUtt(voice, config.utt_type, SObjectSetString(config.text, &error), &error);
  if (S_CHK_ERR(&error, S_CONTERR,
		"main",
		"Call to \"SVoiceSynthUtt\" failed"))
    goto quit;

  SPCT_PRINT_AND_WAIT("synthesized utterance, getting audio object, press ENTER\n");

  isText = s_strcmp(config.utt_type, "text", &error);
  if (S_CHK_ERR(&error, S_CONTERR,
		"main",
		"Call to \"s_strcmp\" failed"))
    goto quit;

  if (isText == 0) {
    /* get audio object */
    audio = SUtteranceGetFeature(utt, "audio", &error);
    if (S_CHK_ERR(&error, S_CONTERR,
		  "main",
		  "Call to \"SUtteranceGetFeature\" failed"))
      goto quit;

    SPCT_PRINT_AND_WAIT("got audio object, saving audio object, press ENTER\n");

    /* save audio */
    SObjectSave(audio, config.wavfile, "riff", &error);
    if (S_CHK_ERR(&error, S_CONTERR,
		  "main",
		  "Call to \"SObjectSave\" failed"))
      goto quit;

    SPCT_PRINT_AND_WAIT("saving textgrid, press ENTER\n");

    /* save textgrid */
    s_asprintf(&textgrid_file, &error, "%s%s", config.wavfile, ".TextGrid");
    if (S_CHK_ERR(&error, S_CONTERR,
		  "main",
		  "Call to \"s_asprinf\" failed"))
      goto quit;

    SObjectSave(S_OBJECT(utt), textgrid_file, "spct_utt_textgrid", &error);
    if (S_CHK_ERR(&error, S_CONTERR,
		  "main",
		  "Call to \"SObjectSave\" failed"))
      {
	S_FREE(textgrid_file);
	goto quit;
      }

    S_FREE(textgrid_file);

    SPCT_PRINT_AND_WAIT("saved textgrid, press ENTER\n");

    SPCT_PRINT_AND_WAIT("saving maryxml, press ENTER\n");

    /* save maryxml */
    s_asprintf(&maryxml_file, &error, "%s%s", config.wavfile, ".MaryXML");
    if (S_CHK_ERR(&error, S_CONTERR,
		  "main",
		  "Call to \"s_asprinf\" failed"))
      goto quit;

    SObjectSave(S_OBJECT(utt), maryxml_file, "spct_utt_maryxml", &error);
    if (S_CHK_ERR(&error, S_CONTERR,
		  "main",
		  "Call to \"SObjectSave\" failed"))
      {
	S_FREE(maryxml_file);
	goto quit;
      }

    S_FREE(maryxml_file);

    /* save htslabels */
    s_asprintf(&htslabels_file, &error, "%s%s", config.wavfile, ".htslabels.XML");
    if (S_CHK_ERR(&error, S_CONTERR,
		  "main",
		  "Call to \"s_asprinf\" failed"))
      goto quit;

    SObjectSave(S_OBJECT(utt), htslabels_file, "spct_utt_htslabelsXML", &error);
    if (S_CHK_ERR(&error, S_CONTERR,
		  "main",
		  "Call to \"SObjectSave\" failed"))
      {
	S_FREE(htslabels_file);
	goto quit;
      }

    S_FREE(htslabels_file);

    SPCT_PRINT_AND_WAIT("saved htslabels, press ENTER\n");
  }

 quit:

  SPCT_PRINT_AND_WAIT("deleting utterance, press ENTER\n");

  if (utt != NULL)
    S_DELETE(utt, "main", &error);

  SPCT_PRINT_AND_WAIT("deleting voice, press ENTER\n");

  if (voice != NULL)
    S_DELETE(voice, "main", &error);

  SPCT_PRINT_AND_WAIT("deleting audio riff plug-in, press ENTER\n");

  if (riffAudio != NULL)
    S_DELETE(riffAudio, "main", &error);

  SPCT_PRINT_AND_WAIT("deleting textgrid plug-in, press ENTER\n");

  if (textGrid != NULL)
    S_DELETE(textGrid, "main", &error);

  SPCT_PRINT_AND_WAIT("deleting maryxml plug-in, press ENTER\n");

  if (maryXML != NULL)
    S_DELETE(maryXML, "main", &error);

   SPCT_PRINT_AND_WAIT("deleting HTSLabelsToXML plug-in, press ENTER\n");

  if (HTSLabelsToXML != NULL)
    S_DELETE(HTSLabelsToXML, "main", &error);

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

int init (int * argc, char ** argv, struct Config * config, s_erc * error) {
  int scomp;
  int i;
  int j = 0;
  for (i=1; i<*argc; i++) {
    scomp = s_strcmp(argv[i],"-h", error);
    if (S_CHK_ERR(error, S_CONTERR,
		  "init",
		  "Call to \"s_strcmp\" failed"))
      goto returnFunction;

    if (scomp == 0)
      usage(0);

    scomp = s_strcmp(argv[i],"--help", error);
    if (S_CHK_ERR(error, S_CONTERR,
		  "init",
		  "Call to \"s_strcmp\" failed"))
      goto returnFunction;

    if (scomp == 0)
      usage(0);

    scomp = s_strcmp(argv[i],"-t", error);
    if (S_CHK_ERR(error, S_CONTERR,
		  "init",
		  "Call to \"s_strcmp\" failed"))
      goto returnFunction;

    if ((scomp == 0) && (i + 1 < *argc)) {
      config->text = argv[i+1];
      i++;
      j+=2;
    }

    scomp = s_strcmp(argv[i],"-v", error);
    if (S_CHK_ERR(error, S_CONTERR,
		  "init",
		  "Call to \"s_strcmp\" failed"))
      goto returnFunction;

    if ((scomp == 0) && (i + 1 < *argc))
      {
	config->voicefile = argv[i+1];
	i++;
	j+=2;
      }

    scomp = s_strcmp(argv[i], "-m", error);
    if (S_CHK_ERR(error, S_CONTERR,
		  "init",
		  "Call to \"s_strcmp\" failed"))
      goto returnFunction;

    if ((scomp == 0) && (i + 1 < *argc)) {
      config->utt_type = argv[i+1];
      i++;
      j+=2;
    }

    scomp = s_strcmp(argv[i],"-o", error);
    if (S_CHK_ERR(error, S_CONTERR,
		  "init",
		  "Call to \"s_strcmp\" failed"))
      goto returnFunction;

    if ((scomp == 0) && (i + 1 < *argc)) {
      config->wavfile = argv[i+1];
      i++;
      j+=2;
    }
  }

  if ((config->wavfile == NULL) || (config->voicefile == NULL) || (config->text == NULL) || (config->utt_type == NULL)) {
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
