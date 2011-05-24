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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Write an SAudio object to a 16 bit Microsoft riff (wav) format file.             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "serialized_audio.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

/* format */
#define RIFF_FORMAT_PCM    0x0001
#define RIFF_FORMAT_ADPCM  0x0002
#define RIFF_FORMAT_MULAW  0x0006
#define RIFF_FORMAT_ALAW   0x0007

#define S_AUDIO_NUM_CHANNELS 1


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void s_write_audio_riff_16(const SAudio *audio, const char *path, s_erc *error)
{
	SDatasource *ds = NULL;
	const char *info;
	uint32 dword;
	uint16 word;
	sint16 *samples = NULL;
	uint32 counter;


	S_CLR_ERR(error);

	/* create a datasource */
	ds = SFilesourceOpenFile(path, "wb", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"SFilesourceOpenFile\" failed"))
		goto quit_error;

	/* riff is little endian */
	SDatasourceSetByteOrder(ds, S_BYTEORDER_LE, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"SDatasourceSetByteOrder\" failed"))
		goto quit_error;

	/* write header */
	info = "RIFF";
	SDatasourceWrite(ds, info, sizeof(char), 4, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"SDatasourceWrite\" failed"))
		goto quit_error;

	/* num bytes in whole file */
	dword = (audio->num_samples * S_AUDIO_NUM_CHANNELS * sizeof(sint16)) + 8 + 16 + 12;
	s_uint32_write(ds, dword, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"s_uint32_write\" failed"))
		goto quit_error;

	info = "WAVE";
	SDatasourceWrite(ds, info, sizeof(char), 4, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"SDatasourceWrite\" failed"))
		goto quit_error;

	info = "fmt ";
	SDatasourceWrite(ds, info, sizeof(char), 4, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"SDatasourceWrite\" failed"))
		goto quit_error;

	/* size of header */
	dword = 16;
	s_uint32_write(ds, dword, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"s_uint32_write\" failed"))
		goto quit_error;

	/* format type */
	word = RIFF_FORMAT_PCM;
	s_uint16_write(ds, word, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"s_uint16_write\" failed"))
		goto quit_error;

	/* number of channels */
	word = S_AUDIO_NUM_CHANNELS;
	s_uint16_write(ds, word, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"s_uint16_write\" failed"))
		goto quit_error;

	/* sample rate */
	dword = audio->sample_rate;
	s_uint32_write(ds, dword, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"s_uint32_write\" failed"))
		goto quit_error;

	/* average bytes per second */
	dword = (audio->sample_rate * S_AUDIO_NUM_CHANNELS * sizeof(sint16));
	s_uint32_write(ds, dword, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"s_uint32_write\" failed"))
		goto quit_error;

	/* block align */
	word = S_AUDIO_NUM_CHANNELS * sizeof(sint16);
	s_uint16_write(ds, word, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"s_uint16_write\" failed"))
		goto quit_error;

	/* bits per sample */
	word = sizeof(sint16) * 8;
	s_uint16_write(ds, word, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"s_uint16_write\" failed"))
		goto quit_error;

	info = "data";
	SDatasourceWrite(ds, info, sizeof(char), 4, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"SDatasourceWrite\" failed"))
		goto quit_error;

	/* number of bytes in data */
	dword = (audio->num_samples * S_AUDIO_NUM_CHANNELS * sizeof(sint16));
	s_uint32_write(ds, dword, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"s_uint32_write\" failed"))
		goto quit_error;

	/* get the audio samples in 16 bit integer format */
	samples = S_CALLOC(sint16, audio->num_samples * S_AUDIO_NUM_CHANNELS);
	for (counter = 0; counter < audio->num_samples; counter++)
		samples[counter] = (sint16)audio->samples[counter];

/* Do byte swap if big-endian */
#ifdef SPCT_BIG_ENDIAN
	samples = (sint16*)s_swap_bytes16((uint16*)samples, audio->num_samples * S_AUDIO_NUM_CHANNELS, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"s_swap_bytes16\" failed"))
		goto quit_error;
#endif

	/* write audio to file */
	SDatasourceWrite(ds, samples, sizeof(sint16), audio->num_samples * S_AUDIO_NUM_CHANNELS, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_audio_riff_16",
				  "Call to \"SDatasourceWrite\" failed"))
		goto quit_error;

	/* done */
	goto quit;

	/* errors and normal clean up code here */
quit_error:
quit:
	if (samples != NULL)
		S_FREE(samples);

	if (ds != NULL)
		S_DELETE(ds, "s_read_lexicon_json", error);
}
