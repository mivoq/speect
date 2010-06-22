/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* DATE    : 8 September 2008                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Viterbi decoder implementation, the decoder paths.                             */
/* Loosly based on EST_Viterbi of Edinburgh Speech Tools,                           */
/* http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96).                         */
/* Note that this is a derived work of EST_Viterbi, with no verbatim                */
/* source code.                                                                     */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* Edinburgh Speech Tools license,  EST_Viterbi                                     */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                            Copyright (c) 1996,1997                               */
/*                             All Rights Reserved.                                 */
/*       Permission is hereby granted, free of charge, to use and distribute        */
/*       this software and its documentation without restriction, including         */
/*       without limitation the rights to use, copy, modify, merge, publish,        */
/*       distribute, sublicense, and/or sell copies of this work, and to            */
/*       permit persons to whom this work is furnished to do so, subject to         */
/*       the following conditions:                                                  */
/*        1. The code must retain the above copyright notice, this list of          */
/*           conditions and the following disclaimer.                               */
/*        2. Any modifications must be clearly marked as such.                      */
/*        3. Original authors' names are not deleted.                               */
/*        4. The authors' names are not used to endorse or promote products         */
/*           derived from this software without specific prior written              */
/*           permission.                                                            */
/*       THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK              */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE           */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author :  Alan W Black                                                     */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "vit_path.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SViterbiPathClass ViterbiPathClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_vit_path_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&ViterbiPathClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_vit_path_class_reg",
			  "Failed to register SViterbiPathClass");
}


S_LOCAL void _s_vit_path_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&ViterbiPathClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_vit_path_class_free",
			  "Failed to free SViterbiPathClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SViterbiPath *self = obj;


	S_CLR_ERR(error);
	self->score = 0.0;
	self->state = 0;
	self->c = NULL;
	self->from = NULL;
	self->next = NULL;

	self->features = (SMap*)S_NEW("SMapList", error);
	S_CHK_ERR(error, S_CONTERR,
			  "Init",
			  "Failed to create new 'SMap' object");
}


static void Destroy(void *obj, s_erc *error)
{
	SViterbiPath *self = obj;


	S_CLR_ERR(error);
	if (self->features != NULL)
		S_DELETE(self->features, "Destroy", error);

	if (self->next != NULL)
		S_DELETE(self->next, "Destroy", error);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SViterbiPath class initialization                                                */
/*                                                                                  */
/************************************************************************************/

static SViterbiPathClass ViterbiPathClass =
{
	/* SObjectClass */
	"SViterbiPath",
	sizeof(SViterbiPath),
	{ 0, 1},
	Init,            /* init    */
	Destroy,         /* destroy */
	Dispose,         /* dispose */
	NULL,            /* compare */
	NULL,            /* print   */
	NULL             /* copy    */
};
