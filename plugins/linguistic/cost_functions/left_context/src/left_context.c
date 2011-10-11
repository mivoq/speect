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
/* A left context cost function class used in unit selection.                       */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "left_context.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SLeftContextCostFuncClass LeftContextCostFuncClass; /* SLeftContextCostFunc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_left_context_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&LeftContextCostFuncClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_left_context_class_reg",
			  "Failed to register SLeftContextCostFuncClass");
}


S_LOCAL void _s_left_context_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&LeftContextCostFuncClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_left_context_class_free",
			  "Failed to free SLeftContextCostFuncClass");
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


static void SetFeature(const SCostFunction *self, SItem *item, s_erc *error)
{
	const SItem *prevItem;
	const char *prev_item_name;


	S_CLR_ERR(error);
	if (item == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetFeature",
				  "Argument \"item\" is NULL");
		return;
	}

	prevItem = SItemPrev(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SetFeature",
				  "Call to \"SItemPrev\" failed"))
		return;

	if (prevItem != NULL)
	{
		prev_item_name = SItemGetName(prevItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SetFeature",
					  "Call to \"SItemGetName\" failed"))
			return;

		SItemSetString(item, self->feature_name, prev_item_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SetFeature",
					  "Call to \"SItemGetName\" failed"))
			return;
	}
	else
	{
		SItemSetString(item, self->feature_name, "NULL", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SetFeature",
					  "Call to \"SItemGetName\" failed"))
			return;
	}
}


float GetCost(const SCostFunction *self, const SItem *item1, const SItem *item2,
			  s_erc *error)
{
	const char *targ_prev_context;
	const char *cand_prev_context;
	int scomp;


	S_CLR_ERR(error);
	if (item1 == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetCost",
				  "Argument \"item1\" is NULL");
		return 0.0;
	}

	if (item2 == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetCost",
				  "Argument \"item2\" is NULL");
		return 0.0;
	}

	targ_prev_context = SItemGetString(item1, self->feature_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetCost",
				  "Call to \"SItemGetString\" failed"))
		return 0.0;

	cand_prev_context = SItemGetString(item2, self->feature_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetCost",
				  "Call to \"SItemGetString\" failed"))
		return 0.0;

	scomp = s_strcmp(targ_prev_context, cand_prev_context, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetCost",
				  "Call to \"s_strcmp\" failed"))
		return 0.0;

	if (scomp == 0)
		return 0.0;

	return 1.0;
}


/************************************************************************************/
/*                                                                                  */
/* SLeftContextCostFunc class initialization                                        */
/*                                                                                  */
/************************************************************************************/

static SLeftContextCostFuncClass LeftContextCostFuncClass =
{
	{
		/* SObjectClass */
		"SCostFunction:SLeftContextCostFunc",
		sizeof(SLeftContextCostFunc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SCostFunctionClass */
	SetFeature,          /* set_feature */
	GetCost              /* get_cost    */
};

