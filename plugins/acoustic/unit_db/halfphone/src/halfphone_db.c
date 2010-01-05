/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* An abstract halfphone database class.                                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "halfphone_db.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SHalfphoneDBClass HalfphoneDBClass; /* SHalfphoneDB class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_halfphone_db_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&HalfphoneDBClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_halfphone_db_class_reg",
			  "Failed to register SHalfphoneDBClass");
}


S_LOCAL void _s_halfphone_db_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&HalfphoneDBClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_halfphone_db_class_free",
			  "Failed to free SHalfphoneDBClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SHalfphoneDB *self = obj;


	S_CLR_ERR(error);
	self->info = S_CALLOC(s_halfphone_db_info, 1);
	if (self->info == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "Init",
				  "Failed to allocate memory for 's_halfphone_db_info' object");
		return;
	}

	self->features = NULL;
}


static void Destroy(void *obj, s_erc *error)
{
	SHalfphoneDB *self = obj;


	S_CLR_ERR(error);
	if (self->info != NULL)
	{
		if (self->info->name != NULL)
			S_FREE(self->info->name);

		if (self->info->description != NULL)
			S_FREE(self->info->description);

		if (self->info->language != NULL)
			S_FREE(self->info->language);

		if (self->info->lang_code != NULL)
			S_FREE(self->info->lang_code);

		S_FREE(self->info);
	}

	if (self->features != NULL)
		S_DELETE(self->features, "Destroy", error);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SHalfphoneDB class initialization                                                */
/*                                                                                  */
/************************************************************************************/

static SHalfphoneDBClass HalfphoneDBClass =
{
	/* SObjectClass */
	{
		"SHalfphoneDB",
		sizeof(SHalfphoneDB),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SHalfphoneDBClass */
	NULL,             /* get_name        */
	NULL,             /* get_description */
	NULL,             /* get_language    */
	NULL,             /* get_lang_code   */
	NULL,             /* get_version     */
	NULL,             /* get_feature     */
	NULL              /* get_units       */
};
