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
/* An abstract phoneset class. The phones in the phoneset have binary features,     */
/* i.e. a phone either has a specific named feature or it doesn't.                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "phoneset.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SPhonesetClass PhonesetClass; /* SPhoneset class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_phoneset_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&PhonesetClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phoneset_class_reg",
			  "Failed to register SPhonesetClass");
}


S_LOCAL void _s_phoneset_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&PhonesetClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phoneset_class_free",
			  "Failed to free SPhonesetClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SPhoneset *self = obj;


	S_CLR_ERR(error);
	self->info = S_CALLOC(s_phoneset_info, 1);
	if (self->info == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "Init",
				  "Failed to allocate memory for 's_phoneset_info' object");
		return;
	}

	self->features = NULL;
}


static void Destroy(void *obj, s_erc *error)
{
	SPhoneset *self = obj;


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
/* SPhoneset class initialization                                                   */
/*                                                                                  */
/************************************************************************************/

static SPhonesetClass PhonesetClass =
{
	/* SObjectClass */
	{
		"SPhoneset",
		sizeof(SPhoneset),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SPhonesetClass */
	NULL,             /* get_name           */
	NULL,             /* get_description    */
	NULL,             /* get_language       */
	NULL,             /* get_lang_code      */
	NULL,             /* get_version        */
	NULL,             /* get_feature        */
	NULL,             /* phone_has_feature  */
	NULL,             /* has_phone          */
	NULL,             /* get_phone_features */
	NULL              /* get_phone_list     */
};
