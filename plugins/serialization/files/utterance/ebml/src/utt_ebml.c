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
/* DATE    : April 2009                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SSerializedFile implementation for EBML format utterances.                       */
/* Read/Write utterances from/to EBML formatted files.                              */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "utt_ebml.h"


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

typedef SSerializedFile SEbmlUttFile;

typedef SSerializedFileClass SEbmlUttFileClass;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SEbmlUttFileClass EbmlUttFileClass; /* SEbmlUttFile  class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_serialized_ebml_utt_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&EbmlUttFileClass, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_ebml_utt_reg",
				  "Failed to register SEbmlUttFileClass"))
		return;

	SSerializedFileRegister(&EbmlUttFileClass, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_ebml_utt_reg",
				  "Failed to add serialized file class SEbmlUttFileClass"))
	{
		s_erc local_err = S_SUCCESS;


		s_class_free(&EbmlUttFileClass, &local_err);
		return;
	}
}


S_LOCAL void _s_serialized_ebml_utt_free(s_erc *error)
{
	s_erc local_err;


	S_CLR_ERR(&local_err);
	S_CLR_ERR(error);

	SSerializedFileFree(&EbmlUttFileClass, &local_err);
	S_CHK_ERR(&local_err, S_CONTERR,
			  "_s_serialized_ebml_utt_free",
			  "Failed to remove serialized file class SEbmlUttFileClass");

	s_class_free(&EbmlUttFileClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_serialized_ebml_utt_free",
			  "Failed to free SEbmlUttFileClass");

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
	SUtterance *utt;
	SDatasource *ds;


	S_CLR_ERR(error);
	ds = SFilesourceOpenFile(path, "rb", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Load",
				  "Call to \"SFilesourceOpenFile\" failed"))
		return NULL;

	utt = s_read_utt_ebml(ds, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Load",
				  "Call to \"s_read_utt_ebml\" failed"))
		return NULL;

	return S_OBJECT(utt);
}


static void Save(const SObject *object, const char *path, s_erc *error)
{
	SUtterance *utt = S_UTTERANCE(object);
	SDatasource *ds;


	S_CLR_ERR(error);
	ds = SFilesourceOpenFile(path, "wb", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Save",
				  "Call to \"SFilesourceOpenFile\" failed"))
		return;

	s_write_utt_ebml(utt, ds, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Save",
			  "Call to \"s_write_utt_ebml\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* YourClass class initialization                                                   */
/*                                                                                  */
/************************************************************************************/

static SEbmlUttFileClass EbmlUttFileClass =
{
	/* SObjectClass */
	{
		"SSerializedFile:SEbmlUttFile",
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
	"spct_utt",                /* format  */
	Load,                      /* load    */
	Save                       /* save    */
};
