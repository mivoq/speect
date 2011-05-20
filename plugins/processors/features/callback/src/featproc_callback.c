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
/* DATE    : 1 December 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* The Feature Processor callback class implementation for scripting languages.     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "featproc_callback.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SFeatProcessorCBClass FeatProcessorCBClass; /* SFeatProcessorCB class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_featprocessor_cb_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&FeatProcessorCBClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_featprocessor_cb_class_reg",
			  "Failed to register SFeatProcessorCBClass");
}


S_LOCAL void _s_featprocessor_cb_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&FeatProcessorCBClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_featprocessor_cb_class_free",
			  "Failed to free SFeatProcessorCBClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SFeatProcessorCB *self = obj;


	S_CLR_ERR(error);
	self->sexecute = NULL;
	self->scleanup = NULL;
	self->sfunction = NULL;
}


static void Destroy(void *obj, s_erc *error)
{
	SFeatProcessorCB *self = obj;

	S_CLR_ERR(error);
	if (self->scleanup != NULL)
		self->scleanup(self, error);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static SObject *Run(const SFeatProcessor *featProc, const SItem *item,
					s_erc *error)
{
	SObject *extractedFeat;
	SFeatProcessorCB *self = S_FEATPROCESSOR_CB(featProc);


	S_CLR_ERR(error);

	if (self->sexecute == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Execute wrapper function for scripting language callback is NULL");
		return NULL;
	}

	if (self->sfunction == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Scripting language callback function is NULL");
		return NULL;
	}

	extractedFeat = self->sexecute(item, self->sfunction, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Scripting language callback function failed"))
		return NULL;

	return extractedFeat;
}


static void SetCallback(SFeatProcessorCB *self,
						callback sexecute, delete scleanup,
						void *sfunction, s_erc *error)
{
	S_CLR_ERR(error);

	if (sexecute == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetCallback",
				  "Argument \"sexecute\" is NULL");
		return;
	}

	if (sfunction == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetCallback",
				  "Argument \"sfunction\" is NULL");
		return;
	}

	self->sexecute = sexecute;
	self->scleanup = scleanup;
	self->sfunction = sfunction;
}


/************************************************************************************/
/*                                                                                  */
/* SFeatProcessorCB class initialization                                            */
/*                                                                                  */
/************************************************************************************/

static SFeatProcessorCBClass FeatProcessorCBClass =
{
	{
		/* SObjectClass */
		{
			"SFeatProcessor:SFeatProcessorCB",
			sizeof(SFeatProcessorCB),
			{ 0, 1},
			Init,            /* init    */
			Destroy,         /* destroy */
			Dispose,         /* dispose */
			NULL,            /* compare */
			NULL,            /* print   */
			NULL,            /* copy    */
		},
		/* SFeatProcessorClass */
		Run,                 /* run     */
	},
	SetCallback              /* set_callback */
};
