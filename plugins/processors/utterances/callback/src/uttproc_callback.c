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
/* DATE    : 3 December 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* The Utterance Processor callback class implementation for scripting languages.   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "uttproc_callback.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SUttProcessorCBClass UttProcessorCBClass; /* SUttProcessorCB class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_uttprocessor_cb_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&UttProcessorCBClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_uttprocessor_cb_class_reg",
			  "Failed to register SUttProcessorCBClass");
}


S_LOCAL void _s_uttprocessor_cb_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&UttProcessorCBClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_uttprocessor_cb_class_free",
			  "Failed to free SUttProcessorCBClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SUttProcessorCB *self = obj;


	S_CLR_ERR(error);
	self->sexecute = NULL;
	self->sfunction = NULL;
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void Run(const SUttProcessor *uttProc, SUtterance *utt,
				s_erc *error)
{
	SUttProcessorCB *self = S_UTTPROCESSOR_CB(uttProc);


	S_CLR_ERR(error);

	if (self->sexecute == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Execute wrapper function for scripting language callback is NULL");
		return;
	}

	if (self->sfunction == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Scripting language callback function is NULL");
		return;
	}

	self->sexecute(S_OBJECT(utt), self->sfunction, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Run",
			  "Scripting language callback function failed");
}


static void SetCallback(SUttProcessorCB *self, callback sexecute,
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
	self->sfunction = sfunction;
}


/************************************************************************************/
/*                                                                                  */
/* SUttProcessorCB class initialization                                             */
/*                                                                                  */
/************************************************************************************/

static SUttProcessorCBClass UttProcessorCBClass =
{
	{
		/* SObjectClass */
		{
			"SUttProcessor:SUttProcessorCB",
			sizeof(SUttProcessorCB),
			{ 0, 1},
			Init,            /* init    */
			NULL,            /* destroy */
			Dispose,         /* dispose */
			NULL,            /* compare */
			NULL,            /* print   */
			NULL,            /* copy    */
		},
		/* SUttProcessorClass */
		NULL,                /* initialize */
		Run,                 /* run        */
	},
	SetCallback              /* set_callback */
};
