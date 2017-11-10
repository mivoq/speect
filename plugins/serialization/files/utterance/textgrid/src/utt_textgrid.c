/************************************************************************************/
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
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
/* DATE    : June 2012                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SSerializedFile implementation for TextGrid format utterances.                   */
/* Write utterances to TextGrid formatted files.                                    */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "utt_textgrid.h"


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

typedef SSerializedFile STextGridUttFile;

typedef SSerializedFileClass STextGridUttFileClass;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static STextGridUttFileClass TextGridUttFileClass; /* STextGridUttFile class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_serialized_textgrid_utt_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&TextGridUttFileClass), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_textgrid_utt_reg",
				  "Failed to register STextGridUttFileClass"))
		return;

	SSerializedFileRegister(&TextGridUttFileClass, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_textgrid_utt_reg",
				  "Failed to add serialized file class STextGridUttFileClass"))
	{
		s_erc local_err = S_SUCCESS;


		s_class_free(S_OBJECTCLASS(&TextGridUttFileClass), &local_err);
		return;
	}
}


S_LOCAL void _s_serialized_textgrid_utt_free(s_erc *error)
{
	s_erc local_err;

	S_CLR_ERR(&local_err);
	S_CLR_ERR(error);

	SSerializedFileFree(&TextGridUttFileClass, &local_err);
	S_CHK_ERR(&local_err, S_CONTERR,
			  "_s_serialized_textgrid_utt_free",
			  "Failed to remove serialized file class STextGridUttFileClass");

	s_class_free(S_OBJECTCLASS(&TextGridUttFileClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_serialized_textgrid_utt_free",
			  "Failed to free STextGridUttFileClass");

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
			  "Failed to load TextGrid from file '%s', load method not implemented",
			  path);

	return NULL;

	S_UNUSED(path);
}


static void Save(const SObject *object, const char *path, s_erc *error)
{
	SUtterance *utt = S_UTTERANCE(object);
	SDatasource *ds;


	S_CLR_ERR(error);
	ds = SFilesourceOpenFile(path, "wt", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Save",
				  "Call to \"SFilesourceOpenFile\" failed"))
		return;

	s_write_utt_textgrid(utt, ds, error); /* datasource closed here */
	S_CHK_ERR(error, S_CONTERR,
			  "Save",
			  "Call to \"s_write_utt_textgrid\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* STextGridUttFileClass class initialization                                       */
/*                                                                                  */
/************************************************************************************/

static STextGridUttFileClass TextGridUttFileClass =
{
	/* SObjectClass */
	{
		"SSerializedFile:STextGridUttFile",
		sizeof(SSerializedFile),
		{ 0, 1},
		NULL,                  /* init    */
		NULL,                  /* destroy */
		Dispose,               /* dispose */
		NULL,                  /* compare */
		NULL,                  /* print   */
		NULL,                  /* copy    */
	},
	/* SSerializedFileClass */
	"spct_utt_textgrid",       /* format  */
	Load,                      /* load    */
	Save,                      /* save    */
	NULL                       /* save_to_datasource */
};
