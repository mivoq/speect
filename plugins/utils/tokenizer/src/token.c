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
/* A token class                                                                    */
/* Loosely based on EST_Token of Edinburgh Speech Tools,                            */
/* http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96)                          */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* Edinburgh Speech Tools license,  EST_Token                                       */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                           Copyright (c) 1996                                     */
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

#include "token.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static STokenClass TokenClass; /* SToken class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_token_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&TokenClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_token_class_reg",
			  "Failed to register STokenClass");
}


S_LOCAL void _s_token_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&TokenClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_token_class_free",
			  "Failed to free STokenClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SToken *self = obj;


	S_CLR_ERR(error);
	self->whitespace = NULL;
	self->pre_punc = NULL;
	self->post_punc = NULL;
	self->string = NULL;
}


static void Destroy(void *obj, s_erc *error)
{
	SToken *self = obj;


	S_CLR_ERR(error);
	if (self->whitespace != NULL)
		S_FREE(self->whitespace);

	if (self->pre_punc != NULL)
		S_FREE(self->pre_punc);

	if (self->post_punc != NULL)
		S_FREE(self->post_punc);

	if (self->string != NULL)
		S_FREE(self->string);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static const char *GetWhiteSpace(SToken *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->whitespace;
}


static void SetWhiteSpace(SToken *self, const char *whitespace, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->whitespace != NULL)
		S_FREE(self->whitespace);

	if (whitespace == NULL)
		return;

	self->whitespace = s_strdup(whitespace, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetWhiteSpace",
			  "Call to \"s_strdup\" failed");
}


static const char *GetPrePunc(SToken *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->pre_punc;
}


static void SetPrePunc(SToken *self, const char *pre_punc, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->pre_punc != NULL)
		S_FREE(self->pre_punc);

	if (pre_punc == NULL)
		return;

	self->pre_punc = s_strdup(pre_punc, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetPrePunc",
			  "Call to \"s_strdup\" failed");
}


static const char *GetPostPunc(SToken *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->post_punc;
}


static void SetPostPunc(SToken *self, const char *post_punc, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->post_punc != NULL)
		S_FREE(self->post_punc);

	if (post_punc == NULL)
		return;

	self->post_punc = s_strdup(post_punc, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetPostPunc",
			  "Call to \"s_strdup\" failed");
}


static const char *GetString(SToken *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->string;
}


static void SetString(SToken *self, const char *string, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->string != NULL)
		S_FREE(self->string);

	if (string == NULL)
		return;

	self->string = s_strdup(string, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetString",
			  "Call to \"s_strdup\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* SToken class initialization                                                      */
/*                                                                                  */
/************************************************************************************/

static STokenClass TokenClass =
{
	/* SObjectClass */
	{
		"SToken",
		sizeof(SToken),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* STokenClass */
	GetWhiteSpace,       /* get_whitespace */
	SetWhiteSpace,       /* set_whitespace */
	GetPrePunc,          /* get_pre_punc   */
	SetPrePunc,          /* set_pre_punc   */
	GetPostPunc,         /* get_post_punc  */
	SetPostPunc,         /* set_post_punc  */
	GetString,           /* get_string     */
	SetString            /* set_string     */
};
