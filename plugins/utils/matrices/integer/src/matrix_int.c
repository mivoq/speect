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
/* A simple integer (signed) matrix class which allows us to use integer            */
/* matrices as SObjects                                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "matrix_int.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SMatrixIntClass MatrixIntClass; /* SMatrixInt class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_matrix_int_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&MatrixIntClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_matrix_int_class_reg",
			  "Failed to register SMatrixIntClass");
}


S_LOCAL void _s_matrix_int_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&MatrixIntClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_matrix_int_class_free",
			  "Failed to free SMatrixIntClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SMatrixInt *self = obj;


	S_CLR_ERR(error);
	self->i = NULL;
	self->row_count = 0;
	self->col_count = 0;
}


static void Destroy(void *obj, s_erc *error)
{
	SMatrixInt *self = obj;
	uint32 k;


	S_CLR_ERR(error);
	if (self->i != NULL)
	{
		for (k = 0; k < self->row_count; k++)
		{
			if (self->i[k] != NULL)
				S_FREE(self->i[k]);
		}

		S_FREE(self->i);
	}
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SMatrixInt class initialization                                                  */
/*                                                                                  */
/************************************************************************************/

static SMatrixIntClass MatrixIntClass =
{
	/* SObjectClass */
	"SMatrixInt",
	sizeof(SMatrixInt),
	{ 0, 1},
	Init,            /* init    */
	Destroy,         /* destroy */
	Dispose,         /* dispose */
	NULL,            /* compare */
	NULL,            /* print   */
	NULL             /* copy    */
};

