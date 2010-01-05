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
/* A simple floating point matrix class which allows us to use                      */
/* floating point matrices as SObjects                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "matrix_float.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SMatrixFloatClass MatrixFloatClass; /* SMatrixFloat class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_matrix_float_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&MatrixFloatClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_matrix_float_class_reg",
			  "Failed to register SMatrixFloatClass");
}


S_LOCAL void _s_matrix_float_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&MatrixFloatClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_matrix_float_class_free",
			  "Failed to free SMatrixFloatClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SMatrixFloat *self = obj;


	S_CLR_ERR(error);
	self->f = NULL;
	self->row_count = 0;
	self->col_count = 0;
}


static void Destroy(void *obj, s_erc *error)
{
	SMatrixFloat *self = obj;
	uint32 k;


	S_CLR_ERR(error);
	if (self->f != NULL)
	{
		for (k = 0; k < self->row_count; k++)
		{
			if (self->f[k] != NULL)
				S_FREE(self->f[k]);
		}

		S_FREE(self->f);
	}
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SMatrixFloat class initialization                                                */
/*                                                                                  */
/************************************************************************************/

static SMatrixFloatClass MatrixFloatClass =
{
	/* SObjectClass */
	"SMatrixFloat",
	sizeof(SMatrixFloat),
	{ 0, 1},
	Init,            /* init    */
	Destroy,         /* destroy */
	Dispose,         /* dispose */
	NULL,            /* compare */
	NULL,            /* print   */
	NULL             /* copy    */
};

