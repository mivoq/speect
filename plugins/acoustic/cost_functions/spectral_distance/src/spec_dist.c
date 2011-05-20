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
/* A spectral distance cost function class used in unit selection.                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "spec_dist.h"
#include "matrix_float.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SSpecDistCostFuncClass SpecDistCostFuncClass; /* SSpecDistCostFunc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_spec_dist_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&SpecDistCostFuncClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_spec_dist_class_reg",
			  "Failed to register SSpecDistCostFuncClass");
}


S_LOCAL void _s_spec_dist_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&SpecDistCostFuncClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_spec_dist_class_free",
			  "Failed to free SSpecDistCostFuncClass");
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


float GetCost(const SCostFunction *self, const SItem *item1, const SItem *item2,
			  s_erc *error)
{
	const SMatrixFloat *leftJoinCoef;
	const SMatrixFloat *rightJoinCoef;
	float **left_spectral = NULL;
	float **right_spectral = NULL;
	double spectral_distance = 0.0;
	uint32 i;


	S_CLR_ERR(error);
	if (item1 == NULL)
	{
		self = NULL; /* compiler noise about unused parameters */

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

	/* left join-coefficient is right join-coefficient of first item */
	leftJoinCoef = (SMatrixFloat*)SItemGetObject(item1, "right-joincoef", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetCost",
				  "Call to \"SItemGetObject\" failed"))
		return 0.0;

	if (leftJoinCoef == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "GetCost",
				  "Failed to get \"right-joincoef\" of first item");
		return 0.0;
	}

	/* right join-coefficient is left join-coefficient of second item */
	rightJoinCoef = (SMatrixFloat*)SItemGetObject(item2, "left-joincoef", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetCost",
				  "Call to \"SItemGetObject\" failed"))
		return 0.0;

	if (rightJoinCoef == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "GetCost",
				  "Failed to get \"left-joincoef\" of second item");
		return 0.0;
	}

	left_spectral = leftJoinCoef->f;
	right_spectral = rightJoinCoef->f;

	/* spectral is everything but last two columns */
	for (i = 0; i < leftJoinCoef->col_count - 2; i++)
	{
		spectral_distance = spectral_distance + pow((left_spectral[0][i] - right_spectral[0][i]), 2);
	}

	spectral_distance = sqrt(spectral_distance/i);

	return (float)spectral_distance;
}


/************************************************************************************/
/*                                                                                  */
/* SSpecDistCostFunc class initialization                                           */
/*                                                                                  */
/************************************************************************************/

static SSpecDistCostFuncClass SpecDistCostFuncClass =
{
	{
		/* SObjectClass */
		"SCostFunction:SSpecDistCostFunc",
		sizeof(SSpecDistCostFunc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SCostFunctionClass */
	NULL,                /* set_feature */
	GetCost              /* get_cost    */
};

