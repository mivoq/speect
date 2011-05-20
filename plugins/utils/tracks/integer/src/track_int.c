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
/* A track class, for storing time aligned integer coefficients.                    */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "track_int.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static STrackIntClass TrackIntClass; /* STrackInt class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_track_int_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&TrackIntClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_track_int_class_reg",
			  "Failed to register STrackIntClass");
}


S_LOCAL void _s_track_int_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&TrackIntClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_track_int_class_free",
			  "Failed to free STrackIntClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	STrackInt *self = obj;


	S_CLR_ERR(error);
	self->time = NULL;
	self->data = (SMatrixInt*)S_NEW("SMatrixInt", error);
	S_CHK_ERR(error, S_CONTERR,
			  "Init",
			  "Failed to create new 'SMatrixInt' object");
}


static void Destroy(void *obj, s_erc *error)
{
	STrackInt *self = obj;


	S_CLR_ERR(error);
	if (self->data != NULL)
		S_DELETE(self->data, "Destroy", error);

	if (self->time != NULL)
		S_FREE(self->time);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* STrackInt class initialization                                                   */
/*                                                                                  */
/************************************************************************************/

static STrackIntClass TrackIntClass =
{
	/* SObjectClass */
	"STrackInt",
	sizeof(STrackInt),
	{ 0, 1},
	Init,            /* init    */
	Destroy,         /* destroy */
	Dispose,         /* dispose */
	NULL,            /* compare */
	NULL,            /* print   */
	NULL             /* copy    */
};

