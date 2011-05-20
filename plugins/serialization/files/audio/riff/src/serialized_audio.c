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
/* SSerializedFile implementation of riff format audio.                             */
/* Write SAudio to riff (Microsoft wav) formatted files.                            */
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
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

typedef SSerializedFile SRIFFAudioFile;

typedef SSerializedFileClass SRIFFAudioFileClass;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SRIFFAudioFileClass RIFFAudioFileClass; /* SRIFFAudioFile class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_serialized_riff_audio_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&RIFFAudioFileClass, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_riff_audio_reg",
				  "Failed to register SRIFFAudioFileClass"))
		return;

	SSerializedFileRegister(&RIFFAudioFileClass, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_riff_audio_reg",
				  "Failed to add serialized file class SRIFFAudioFileClass"))
	{
		s_erc local_err = S_SUCCESS;


		s_class_free(&RIFFAudioFileClass, &local_err);
		return;
	}
}


S_LOCAL void _s_serialized_riff_audio_free(s_erc *error)
{
	s_erc local_err;


	S_CLR_ERR(&local_err);
	S_CLR_ERR(error);

	SSerializedFileFree(&RIFFAudioFileClass, &local_err);
	S_CHK_ERR(&local_err, S_CONTERR,
			  "_s_serialized_riff_audio_free",
			  "Failed to remove serialized file class SRIFFAudioFileClass");

	s_class_free(&RIFFAudioFileClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_serialized_riff_audio_free",
			  "Failed to free SRIFFAudioFileClass");

	if ((local_err != S_SUCCESS)
		&& (error != NULL)
		&& (*error == S_SUCCESS))
		*error = local_err;
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static SObject *Load(const char *path, s_erc *error)
{
	S_CTX_ERR(error, S_FAILURE,
			  "Load",
			  "Failed to load audio from file '%s', load method not implemented",
			  path);
	path = NULL;
	return NULL;
}


static void Save(const SObject *object, const char *path, s_erc *error)
{
	SAudio *audio = S_AUDIO(object);


	S_CLR_ERR(error);
	s_write_audio_riff_16(audio, path, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Save",
			  "Call to \"s_write_audio_riff_16\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* SRIFFAudioFile class initialization                                              */
/*                                                                                  */
/************************************************************************************/

static SRIFFAudioFileClass RIFFAudioFileClass =
{
	/* SObjectClass */
	{
		"SSerializedFile:SRIFFAudioFile",
		sizeof(SRIFFAudioFile),
		{ 0, 1},
		NULL,                  /* init    */
		NULL,                  /* destroy */
		Dispose,               /* dispose */
		NULL,                  /* compare */
		NULL,                  /* print   */
		NULL,                  /* copy    */
	},
	/* SSerializedFileClass */
	"riff",                    /* format  */
	Load,                      /* load    */
	Save                       /* save    */
};
