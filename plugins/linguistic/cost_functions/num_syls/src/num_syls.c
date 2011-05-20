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
/* A number of syllables cost function class used in unit selection.                */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "num_syls.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SNumSylsCostFuncClass NumSylsCostFuncClass; /* SNumSylsCostFunc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_num_syls_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&NumSylsCostFuncClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_num_syls_class_reg",
			  "Failed to register SNumSylsCostFuncClass");
}


S_LOCAL void _s_num_syls_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&NumSylsCostFuncClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_num_syls_class_free",
			  "Failed to free SNumSylsCostFuncClass");
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
	SObject *extractedFeature = NULL;


	S_CLR_ERR(error);
	if (item == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetFeature",
				  "Argument \"item\" is NULL");
		goto quit_error;
	}

	extractedFeature = s_path_to_featproc(item,
										  "R:SylStructure.parent.parent.word_num_syls",
										  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SetFeature",
				  "Call to \"s_path_to_featproc\" failed"))
		goto quit_error;

	if (extractedFeature == NULL)
	{
		/* segment item might not have a word, e.g inserted silences. */
		extractedFeature = SObjectSetInt(0, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SetFeature",
					  "Call to \"SObjectSetInt\" failed"))
			goto quit_error;
	}

	SItemSetObject(item, self->feature_name, extractedFeature, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SetFeature",
				  "Call to \"SItemSetObject\" failed"))
		goto quit_error;

	/* all OK here */
	return;

	/* errors be here */
quit_error:
	if (extractedFeature != NULL)
		S_DELETE(extractedFeature, "SetFeature", error);
}


float GetCost(const SCostFunction *self, const SItem *item1, const SItem *item2,
			  s_erc *error)
{
	sint32 targ_num_syls;
	sint32 cand_num_syls;
	float cost;

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

	targ_num_syls = SItemGetInt(item1, self->feature_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetCost",
				  "Call to \"SItemGetInt\" failed"))
		return 0.0;

	cand_num_syls = SItemGetInt(item2, self->feature_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetCost",
				  "Call to \"SItemGetInt\" failed"))
		return 0.0;

	if ((targ_num_syls == 0) && (cand_num_syls == 0))
		return 0.0;

	if ((targ_num_syls == 0) || (cand_num_syls == 0))
		return 1.0;

	cost = (float)((cand_num_syls * 1.0)/(targ_num_syls * 1.0));

	if (cost < 1.5)
		return 0.0;

	return 1.0;
}


/************************************************************************************/
/*                                                                                  */
/* SNumSylsCostFunc class initialization                                            */
/*                                                                                  */
/************************************************************************************/

static SNumSylsCostFuncClass NumSylsCostFuncClass =
{
	{
		/* SObjectClass */
		"SCostFunction:SNumSylsCostFunc",
		sizeof(SNumSylsCostFunc),
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

