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
/* SSerializedFile implementation for EBML format halfphone databases.              */
/* Read halfphone databases from EBML formatted files.                              */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "serialized_halfphone_db.h"


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

typedef SSerializedFile SEBMLHalfphoneDBFile;

typedef SSerializedFileClass SEBMLHalfphoneDBFileClass;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SEBMLHalfphoneDBFileClass EBMLHalfphoneDBFileClass; /* SEBMLHalfphoneDBFile class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_serialized_ebml_halfphone_db_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&EBMLHalfphoneDBFileClass), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_ebml_halfphone_db_reg",
				  "Failed to register SEBMLHalfphoneDBFileClass"))
		return;

	SSerializedFileRegister(&EBMLHalfphoneDBFileClass, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_serialized_ebml_halfphone_db_reg",
				  "Failed to add serialized file class SEBMLHalfphoneDBFileClass"))
	{
		s_erc local_err = S_SUCCESS;


		s_class_free(S_OBJECTCLASS(&EBMLHalfphoneDBFileClass), &local_err);
		return;
	}
}


S_LOCAL void _s_serialized_ebml_halfphone_db_free(s_erc *error)
{
	s_erc local_err;


	S_CLR_ERR(&local_err);
	S_CLR_ERR(error);

	SSerializedFileFree(&EBMLHalfphoneDBFileClass, &local_err);
	S_CHK_ERR(&local_err, S_CONTERR,
			  "_s_serialized_ebml_halfphone_db_free",
			  "Failed to remove serialized file class SEBMLHalfphoneDBFileClass");

	s_class_free(S_OBJECTCLASS(&EBMLHalfphoneDBFileClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_serialized_ebml_halfphone_db_free",
			  "Failed to free SEBMLHalfphoneDBFileClass");

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
	SHalfphoneDBEbml *db;
	SDatasource *ds;


	S_CLR_ERR(error);

	ds = SFilesourceOpenFile(path, "rb", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Load",
				  "Call to \"SFilesourceOpenFile\" failed"))
		return NULL;

	db = s_read_halfphone_db_ebml(ds, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Load",
				  "Call to \"s_read_halfphone_db_ebml\" failed"))
		return NULL;

	return S_OBJECT(db);
}


static void Save(const SObject *object, const char *path, s_erc *error)
{
	S_CTX_ERR(error, S_FAILURE,
			  "Save",
			  "Failed to save halfphone database to file '%s', save method not implemented",
			  path);
	S_UNUSED(object);
}


/************************************************************************************/
/*                                                                                  */
/* SEBMLHalfphoneDBFile class initialization                                        */
/*                                                                                  */
/************************************************************************************/

static SEBMLHalfphoneDBFileClass EBMLHalfphoneDBFileClass =
{
	/* SObjectClass */
	{
		"SSerializedFile:SEBMLHalfphoneDBFile",
		sizeof(SEBMLHalfphoneDBFile),
		{ 0, 1},
		NULL,                  /* init    */
		NULL,                  /* destroy */
		Dispose,               /* dispose */
		NULL,                  /* compare */
		NULL,                  /* print   */
		NULL,                  /* copy    */
	},
	/* SSerializedFileClass */
	"spct_halfphone_db",       /* format  */
	Load,                      /* load    */
	Save,                      /* save    */
	NULL                       /* save_to_datasource */
};
