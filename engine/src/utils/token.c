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

#include "base/utils/alloc.h"
#include "base/strings/strings.h"
#include "utils/token.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Call the given function method of the given #SToken,
 * see full description #S_TOKEN_CALL for usage.
 *
 * @param SELF The given #SToken*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_TOKEN_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_TOKEN_CALL(SELF, FUNC)				\
	((STokenClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SToken
 * can be called.
 *
 * @param SELF The given #SToken*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_TOKEN_METH_VALID(SELF, FUNC)			\
	S_TOKEN_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static STokenClass TokenClass; /* SToken class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API const char *STokenGetWhitespace(const SToken *self, s_erc *error)
{
	const char *white_space;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "STokenGetWhitespace",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_TOKEN_METH_VALID(self, get_whitespace))
	{
		S_WARNING(S_METHINVLD,
				  "STokenGetWhitespace",
				  "Token method \"get_whitespace\" not implemented");
		return NULL;
	}

	white_space = S_TOKEN_CALL(self, get_whitespace)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "STokenGetWhitespace",
				  "Call to class method \"get_whitespace\" failed"))
		return NULL;

	return white_space;
}


S_API void STokenSetWhitespace(SToken *self, const char *whitespace, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "STokenSetWhitespace",
				  "Argument \"self\" is NULL");
		return;
	}

	if (whitespace == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "STokenSetWhitespace",
				  "Argument \"whitespace\" is NULL");
		return;
	}

	if (!S_TOKEN_METH_VALID(self, set_whitespace))
	{
		S_WARNING(S_METHINVLD,
				  "STokenSetWhitespace",
				  "Token method \"set_whitespace\" not implemented");
		return;
	}

	S_TOKEN_CALL(self, set_whitespace)(self, whitespace, error);
	S_CHK_ERR(error, S_CONTERR,
			  "STokenSetWhitespace",
			  "Call to class method \"set_whitespace\" failed");
}


S_API const char *STokenGetPrePunc(const SToken *self, s_erc *error)
{
	const char *pre_punc;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "STokenGetPrePunc",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_TOKEN_METH_VALID(self, get_pre_punc))
	{
		S_WARNING(S_METHINVLD,
				  "STokenGetPrePunc",
				  "Token method \"get_pre_punc\" not implemented");
		return NULL;
	}

	pre_punc = S_TOKEN_CALL(self, get_pre_punc)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "STokenGetPrePunc",
				  "Call to class method \"get_pre_punc\" failed"))
		return NULL;

	return pre_punc;
}


S_API void STokenSetPrePunc(SToken *self, const char *pre_punc, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "STokenSetPrePunc",
				  "Argument \"self\" is NULL");
		return;
	}

	if (pre_punc == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "STokenSetPrePunc",
				  "Argument \"pre_punc\" is NULL");
		return;
	}

	if (!S_TOKEN_METH_VALID(self, set_pre_punc))
	{
		S_WARNING(S_METHINVLD,
				  "STokenSetPrePunc",
				  "Token method \"set_pre_punc\" not implemented");
		return;
	}

	S_TOKEN_CALL(self, set_pre_punc)(self, pre_punc, error);
	S_CHK_ERR(error, S_CONTERR,
			  "STokenSetPrePunc",
			  "Call to class method \"set_pre_punc\" failed");
}


S_API const char *STokenGetPostPunc(const SToken *self, s_erc *error)
{
	const char *post_punc;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "STokenGetPostPunc",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_TOKEN_METH_VALID(self, get_post_punc))
	{
		S_WARNING(S_METHINVLD,
				  "STokenGetPostPunc",
				  "Token method \"get_post_punc\" not implemented");
		return NULL;
	}

	post_punc = S_TOKEN_CALL(self, get_post_punc)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "STokenGetPostPunc",
				  "Call to class method \"get_post_punc\" failed"))
		return NULL;

	return post_punc;
}


S_API void STokenSetPostPunc(SToken *self, const char *post_punc, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "STokenSetPostPunc",
				  "Argument \"self\" is NULL");
		return;
	}

	if (post_punc == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "STokenSetPostPunc",
				  "Argument \"post_punc\" is NULL");
		return;
	}

	if (!S_TOKEN_METH_VALID(self, set_post_punc))
	{
		S_WARNING(S_METHINVLD,
				  "STokenSetPostPunc",
				  "Token method \"set_post_punc\" not implemented");
		return;
	}

	S_TOKEN_CALL(self, set_post_punc)(self, post_punc, error);
	S_CHK_ERR(error, S_CONTERR,
			  "STokenSetPostPunc",
			  "Call to class method \"set_post_punc\" failed");
}


S_API const char *STokenGetString(const SToken *self, s_erc *error)

{
	const char *string;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "STokenGetString",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_TOKEN_METH_VALID(self, get_string))
	{
		S_WARNING(S_METHINVLD,
				  "STokenGetString",
				  "Token method \"get_string\" not implemented");
		return NULL;
	}

	string = S_TOKEN_CALL(self, get_string)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "STokenGetString",
				  "Call to class method \"get_string\" failed"))
		return NULL;

	return string;
}


S_API void STokenSetString(SToken *self, const char *string, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "STokenSetString",
				  "Argument \"self\" is NULL");
		return;
	}

	if (string == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "STokenSetString",
				  "Argument \"string\" is NULL");
		return;
	}

	if (!S_TOKEN_METH_VALID(self, set_string))
	{
		S_WARNING(S_METHINVLD,
				  "STokenSetString",
				  "Token method \"set_string\" not implemented");
		return;
	}

	S_TOKEN_CALL(self, set_string)(self, string, error);
	S_CHK_ERR(error, S_CONTERR,
			  "STokenSetString",
			  "Call to class method \"set_string\" failed");
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


static const char *GetWhiteSpace(const SToken *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->whitespace;
}


static void SetWhiteSpace(SToken *self, const char *whitespace, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->whitespace != NULL)
		S_FREE(self->whitespace);

	self->whitespace = s_strdup(whitespace, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetWhiteSpace",
			  "Call to \"s_strdup\" failed");
}


static const char *GetPrePunc(const SToken *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->pre_punc;
}


static void SetPrePunc(SToken *self, const char *pre_punc, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->pre_punc != NULL)
		S_FREE(self->pre_punc);

	self->pre_punc = s_strdup(pre_punc, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetPrePunc",
			  "Call to \"s_strdup\" failed");
}


static const char *GetPostPunc(const SToken *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->post_punc;
}


static void SetPostPunc(SToken *self, const char *post_punc, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->post_punc != NULL)
		S_FREE(self->post_punc);

	self->post_punc = s_strdup(post_punc, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetPostPunc",
			  "Call to \"s_strdup\" failed");
}


static const char *GetString(const SToken *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->string;
}


static void SetString(SToken *self, const char *string, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->string != NULL)
		S_FREE(self->string);

	self->string = s_strdup(string, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetString",
			  "Call to \"s_strdup\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_token_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&TokenClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_token_class_add",
			  "Failed to add STokenClass");
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
