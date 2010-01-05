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
/* A Rectangular window function class implementation.                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "rectangular.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SWinRectClass WinRectClass; /* SWinRect class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_winrect_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&WinRectClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_winrect_class_reg",
			  "Failed to register SWinRectClass");
}


S_LOCAL void _s_winrect_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&WinRectClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_winrect_class_free",
			  "Failed to free SWinRectClass");
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


static void InitWindow(SWinFunc **self, sint32 size, sint32 centre, s_erc *error)
{
	uint32 i;


	S_CLR_ERR(error);

	if (size <= 0)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "InitWindow",
				  "Window function given a 0 or negative size (%d)",
				  size);
		goto quit_error;
	}

	/* nothing to do, window is correct */
	if ((uint32)size == (*self)->num_samples)
		return;

	if ((*self)->samples != NULL)
		S_FREE((*self)->samples);

	(*self)->samples = S_MALLOC(float, size);
	if ((*self)->samples == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "InitWindow",
				  "Failed to allocate memory for 'float' object");
		goto quit_error;
	}

	(*self)->num_samples = size;

	for (i = 0; i < (*self)->num_samples; i++)
		(*self)->samples[i] = 1.0;


quit_error:
	S_DELETE(*self, "InitWindow", error);
	*self = NULL;
	centre = 0; /* suppress compiler noise about unused parameters */
}


/************************************************************************************/
/*                                                                                  */
/* SWinFunc class initialization                                                    */
/*                                                                                  */
/************************************************************************************/

static SWinRectClass WinRectClass =
{
	/* SObjectClass */
	{
		"SWinFunc:SWinRect",
		sizeof(SWinRect),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SWinFuncClass */
	InitWindow           /* init_window */
};
