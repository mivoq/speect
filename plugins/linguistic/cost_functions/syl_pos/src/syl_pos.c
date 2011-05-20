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
/* A syllable position cost function class used in unit selection.                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "syl_pos.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSylPosCostFuncClass SylPosCostFuncClass; /* SSylPosCostFunc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_syl_pos_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&SylPosCostFuncClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_pos_class_reg",
			  "Failed to register SSylPosCostFuncClass");
}


S_LOCAL void _s_syl_pos_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&SylPosCostFuncClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_syl_pos_class_free",
			  "Failed to free SSylPosCostFuncClass");
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
	SObject *tmp = NULL;
	const SItem *thisItemInSylRel;
	const SItem *nextItemInSylRel;
	const SItem *prevItemInSylRel;


	S_CLR_ERR(error);
	if (item == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetFeature",
				  "Argument \"item\" is NULL");
		goto quit_error;
	}

	thisItemInSylRel = s_path_to_item(item, "R:SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SetFeature",
				  "Call to \"s_path_to_item\" failed"))
		goto quit_error;

	nextItemInSylRel = s_path_to_item(item, "R:SylStructure.n", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SetFeature",
				  "Call to \"s_path_to_item\" failed"))
		goto quit_error;

	prevItemInSylRel = s_path_to_item(item, "R:SylStructure.p", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SetFeature",
				  "Call to \"s_path_to_item\" failed"))
		goto quit_error;

	if (thisItemInSylRel == NULL)
	{
		tmp = SObjectSetString("none", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SetFeature",
					  "Call to \"SObjectSetString\" failed"))
			goto quit_error;

		SItemSetObject(item, self->feature_name, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SetFeature",
					  "Call to \"SItemSetObject\" failed"))
			goto quit_error;
	}
	else if ((thisItemInSylRel != NULL) && (prevItemInSylRel == NULL))
	{
		tmp = SObjectSetString("initial", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SetFeature",
					  "Call to \"SObjectSetString\" failed"))
			goto quit_error;

		SItemSetObject(item, self->feature_name, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SetFeature",
					  "Call to \"SItemSetObject\" failed"))
			goto quit_error;
	}
	else if ((thisItemInSylRel != NULL) && (nextItemInSylRel == NULL))
	{
		tmp = SObjectSetString("final", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SetFeature",
					  "Call to \"SObjectSetString\" failed"))
			goto quit_error;

		SItemSetObject(item, self->feature_name, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SetFeature",
					  "Call to \"SItemSetObject\" failed"))
			goto quit_error;
	}
	else
	{
		tmp = SObjectSetString("medial", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SetFeature",
					  "Call to \"SObjectSetString\" failed"))
			goto quit_error;

		SItemSetObject(item, self->feature_name, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SetFeature",
					  "Call to \"SItemSetObject\" failed"))
			goto quit_error;
	}

	/* all OK here */
	return;

	/* errors be here */
quit_error:
	if (tmp != NULL)
		S_DELETE(tmp, "SetFeature", error);
}


float GetCost(const SCostFunction *self, const SItem *item1, const SItem *item2,
			  s_erc *error)
{
	const char *targ_syl_position;
	const char *cand_syl_position;
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

	targ_syl_position = SItemGetString(item1, self->feature_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetCost",
				  "Call to \"SItemGetString\" failed"))
		return 0.0;

	cand_syl_position = SItemGetString(item2, self->feature_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetCost",
				  "Call to \"SItemGetString\" failed"))
		return 0.0;

	scomp = s_strcmp(targ_syl_position, cand_syl_position, error);
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
/* SSylPosCostFunc class initialization                                             */
/*                                                                                  */
/************************************************************************************/

static SSylPosCostFuncClass SylPosCostFuncClass =
{
	{
		/* SObjectClass */
		"SCostFunction:SSylPosCostFunc",
		sizeof(SSylPosCostFunc),
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

