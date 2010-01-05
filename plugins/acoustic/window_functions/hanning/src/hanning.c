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
/* A Hanning window function class implementation.                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "hanning.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SWinHannClass WinHannClass; /* SWinHann class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_winhann_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&WinHannClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_winhann_class_reg",
			  "Failed to register SWinHannClass");
}


S_LOCAL void _s_winhann_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&WinHannClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_winhann_class_free",
			  "Failed to free SWinHannClass");
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
	sint32 c;
	sint32 i;
	float k;
	sint32 end = size - 1;


	S_CLR_ERR(error);

	if (size <= 0)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "InitWindow",
				  "Window function given a 0 or negative size (%d)",
				  size);
		goto quit_error;
	}

	if (centre >= size)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "InitWindow",
				  "Given centre (%d) is greater or equal to the size (%d) of the window",
				  centre, size);
		goto quit_error;
	}


	/* nothing to do if window is already the given size and centre */
	if (((uint32)size == (*self)->num_samples) && (centre == (*self)->centre))
		return;

	if ((*self)->samples != NULL)
		S_FREE((*self)->samples);

	(*self)->samples = S_CALLOC(float, size);
	if ((*self)->samples == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "InitWindow",
				  "Failed to allocate memory for 'float' object");
		goto quit_error;
	}

	(*self)->num_samples = size;
	(*self)->centre = centre;

	/* symmetric window */
	if (centre < 0 )
	{
		c = size/2;

		/* only need to calculate one half + copy */
		if ((size & 1) != 0) /* odd */
			(*self)->samples[c] = 1.0;

		k = 2.0 * S_PI/size;

		for (i = 0; i < c; i++)
		{
			(*self)->samples[i] = 0.5 - 0.5 * cos(k * (i + 0.5));
			(*self)->samples[end-i] = (*self)->samples[i];
		}
	}
	else /* asymmetric window */
	{
		sint32 effective_size = 0;
		sint32 righthand_size = 0;


		c = centre;
		(*self)->samples[c] = 1.0; /* we assume "centre" is 1.0  */

		/* first half */
		effective_size = (2 * centre) + 1;
		k = 2.0 * S_PI/effective_size;
		for (i = 0; i < c; i++)
			(*self)->samples[i] = 0.5 - 0.5 * cos(k * (i + 0.5));

		/* second half */
		righthand_size = size - 1 - centre;
		effective_size = (2 * righthand_size) + 1;
		k = 2.0 * S_PI/effective_size;
		for (i = 0; i < righthand_size; i++)
			(*self)->samples[end-i] = 0.5 - 0.5 * cos(k * (i + 0.5));
	}

	return;

quit_error:
	S_DELETE(*self, "InitWindow", error);
	*self = NULL;
}


/************************************************************************************/
/*                                                                                  */
/* SWinFunc class initialization                                                    */
/*                                                                                  */
/************************************************************************************/

static SWinHannClass WinHannClass =
{
	/* SObjectClass */
	{
		"SWinFunc:SWinHann",
		sizeof(SWinHann),
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
