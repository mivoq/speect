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
/* An abstract g2p class from which specific implementations can inherit.           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "g2p.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SG2PClass G2PClass; /* SG2P class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_g2p_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&G2PClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_g2p_class_reg",
			  "Failed to register SG2PClass");
}


S_LOCAL void _s_g2p_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&G2PClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_g2p_class_free",
			  "Failed to free SG2PClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SG2P *self = obj;


	S_CLR_ERR(error);
	self->info = S_CALLOC(s_g2p_info, 1);
	if (self->info == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "Init",
				  "Failed to allocate memory for 's_g2p_info' object");
		return;
	}

	self->features = NULL;
}



static void Destroy(void *obj, s_erc *error)
{
	SG2P *self = obj;


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
/* SG2P class initialization                                                        */
/*                                                                                  */
/************************************************************************************/

static SG2PClass G2PClass =
{
	/* SObjectClass */
	{
		"SG2P",
		sizeof(SG2P),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SG2PClass */
	NULL,             /* get_name        */
	NULL,             /* get_description */
	NULL,             /* get_language    */
	NULL,             /* get_lang_code   */
	NULL,             /* get_version     */
	NULL,             /* get_feature     */
	NULL,             /* apply           */
	NULL              /* apply_at        */
};
