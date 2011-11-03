/************************************************************************************/
/* Copyright (c) 2011 The Department of Arts and Culture,                           */
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
/* DATE    : May 2011                                                               */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SSerializedFile implementation for JSON format syllabification                   */
/* rewrites rule-sets.                                                              */
/* Read syllabification rewrite implementations from JSON formatted files.          */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "serialized_syllabification.h"


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

typedef SSerializedFile SJSONSyllabificationRewritesFile;

typedef SSerializedFileClass SJSONSyllabificationRewritesFileClass;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SJSONSyllabificationRewritesFile class declaration. */
static SJSONSyllabificationRewritesFileClass  JSONSyllabificationRewritesFileClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_serialized_json_syllabification_rewrites_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&JSONSyllabificationRewritesFileClass), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_json_syllabification_rewrites_reg",
				  "Failed to register SJSONSyllabificationRewritesFile"))
		return;

	SSerializedFileRegister(&JSONSyllabificationRewritesFileClass, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_json_syllabification_rewrites_reg",
				  "Failed to add serialized file class SJSONSyllabificationRewritesFile"))
	{
		s_erc local_err = S_SUCCESS;


		s_class_free(S_OBJECTCLASS(&JSONSyllabificationRewritesFileClass), &local_err);
		return;
	}
}


S_LOCAL void _s_serialized_json_syllabification_rewrites_free(s_erc *error)
{
	s_erc local_err;


	S_CLR_ERR(&local_err);
	S_CLR_ERR(error);

	SSerializedFileFree(&JSONSyllabificationRewritesFileClass, &local_err);
	S_CHK_ERR(&local_err, S_CONTERR,
			  "_s_serialized_json_syllabification_rewrites_free",
			  "Failed to remove serialized file class SJSONSyllabificationRewritesFile");

	s_class_free(S_OBJECTCLASS(&JSONSyllabificationRewritesFileClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_serialized_json_syllabification_rewrites_free",
			  "Failed to free SJSONSyllabificationRewritesFile");

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
	SSyllabificationRewrites *syllab;


	S_CLR_ERR(error);

	syllab = s_read_syllabification_rewrites_json(path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Load",
				  "Call to \"s_read_syllabification_rewrites_json\" failed"))
		return NULL;

	return S_OBJECT(syllab);
}


static void Save(const SObject *object, const char *path, s_erc *error)
{
	S_CTX_ERR(error, S_FAILURE,
			  "Save",
			  "Failed to save syllabification rewrites rule-set to file '%s', save method not implemented",
			  path);
	S_UNUSED(object);
}


/************************************************************************************/
/*                                                                                  */
/* SJSONPhonesetFile class initialization                                           */
/*                                                                                  */
/************************************************************************************/

static SJSONSyllabificationRewritesFileClass JSONSyllabificationRewritesFileClass =
{
	/* SObjectClass */
	{
		"SSerializedFile:SJSONSyllabificationRewritesFile",
		sizeof(SJSONSyllabificationRewritesFile),
		{ 0, 1},
		NULL,                  /* init    */
		NULL,                  /* destroy */
		Dispose,               /* dispose */
		NULL,                  /* compare */
		NULL,                  /* print   */
		NULL,                  /* copy    */
	},
	/* SSerializedFileClass */
	"spct_syllabification_rewrites_json",    /* format  */
	Load,                                    /* load    */
	Save                                     /* save    */
};
