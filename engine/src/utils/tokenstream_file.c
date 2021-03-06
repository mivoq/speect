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
/* A file tokenstream class implementation.                                         */
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

#include "utils/tokenstream_file.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Call the given function method of the given #STokenstreamFile,
 * see full description #S_TOKENSTREAM_FILE_CALL for usage.
 *
 * @param SELF The given #STokenstreamFile*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_TOKENSTREAM_FILE_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_TOKENSTREAM_FILE_CALL(SELF, FUNC)				\
	((STokenstreamFileClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Call the given function method of the given #STokenstream,
 * see full description #S_TOKENSTREAM_CALL for usage.
 *
 * @param SELF The given #STokenstream*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_TOKENSTREAM_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_TOKENSTREAM_CALL(SELF, FUNC)				\
	((STokenstreamClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #STokenstreamFile
 * can be called.
 *
 * @param SELF The given #STokenstreamFile*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_TOKENSTREAM_FILE_METH_VALID(SELF, FUNC)			\
	S_TOKENSTREAM_FILE_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static STokenstreamFileClass TokenstreamFileClass; /* STokenstreamFile class declaration. */

/* A tokenstream to give us access to the STokenstreamClass functions */
static STokenstream *tokenstream = NULL;

static uint num_file_tokenstreams = 0;


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void STokenstreamFileInit(STokenstreamFile **self, const char *path, s_erc *error)
{
	S_CLR_ERR(error);

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "STokenstreamFileInit",
				  "Argument \"path\" is NULL");
		return;
	}

	(*self)->ds = SFilesourceOpenFile(path, "r", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "STokenstreamFileInit",
				  "Call to \"SFilesourceOpenFile\" failed"))
	{
		S_DELETE(*self, "STokenstreamFileInit", error);
		*self = NULL;
		return;
	}

	/* get the first character */
	STokenstreamGetChar(S_TOKENSTREAM(*self), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "STokenstreamFileInit",
				  "Call to \"STokenstreamGetChar\" failed"))
	{
		S_DELETE(*self, "STokenstreamFileInit", error);
		*self = NULL;
		return;
	}
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	STokenstreamFile *self = obj;


	S_CLR_ERR(error);

	if (num_file_tokenstreams++ == 0)
	{
		/* create a tokenstream to give us access to the STokenstreamClass functions */
		tokenstream = S_NEW(STokenstream, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Init",
					  "Failed to create tokenstream to give STokenstreamClass function access"))
			return;
	}

	self->ds = NULL;
}


static void Destroy(void *obj, s_erc *error)
{
	STokenstreamFile *self = obj;


	S_CLR_ERR(error);

	if (self->ds != NULL)
		S_DELETE(self->ds, "Destroy", error);

	if (--num_file_tokenstreams == 0)
		S_DELETE(tokenstream, "Destroy", error);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static uint32 GetChar(STokenstream *self, s_erc *error)
{
	uint32 utf8char;
	uchar t;
	int n;
	STokenstreamFile *tf = S_TOKENSTREAM_FILE(self);
	s_erc local_err = S_SUCCESS;


	S_CLR_ERR(error);

	SDatasourceRead(tf->ds, &t, sizeof(uchar), 1, &local_err);
	if (local_err == S_IOEOF)
	{
		self->eof = TRUE;
		self->current_char = 0;
		return 0;
	}
	else if (local_err != S_SUCCESS)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "GetChar",
				  "Call to \"SDatasourceRead\" failed");
		self->current_char = 0;
		return 0;
	}

	utf8char = t;

	if (utf8char & 0x80)
	{
		n = 1;

		while (utf8char & (0x80>>n))
			n++;

		utf8char &= (1 << (8 - n)) - 1;

		while (--n > 0)
		{
			SDatasourceRead(tf->ds, &t, sizeof(uchar), 1, &local_err);
			if (local_err == S_IOEOF)
			{
				self->eof = TRUE;
				self->current_char = 0;
				return 0;
			}
			else if (local_err != S_SUCCESS)
			{
				S_CTX_ERR(error, S_CONTERR,
						  "GetChar",
						  "Call to \"SDatasourceRead\" failed");
				self->current_char = 0;
				return 0;
			}

			if ((!(t & 0x80)) || (t & 0x40))
			{
				self->current_char = '^';
				S_CTX_ERR(error, S_CONTERR,
						  "GetChar",
						  "Read invalid character");
			}

			utf8char = (utf8char << 6) | (t & 0x3F);
		}
	}

	self->current_char = utf8char;
	return utf8char;
}


static void Seek(STokenstream *self, ulong pos, s_erc *error)
{
	STokenstreamFile *tf = S_TOKENSTREAM_FILE(self);


	S_CLR_ERR(error);
	SDatasourceSeek(tf->ds, pos, S_SEEK_SET, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Seek",
			  "Call to \"SDatasourceSeek\" failed");
}


static ulong Tell(const STokenstream *self, s_erc *error)
{
	STokenstreamFile *tf = S_TOKENSTREAM_FILE(self);
	ulong pos;


	S_CLR_ERR(error);
	pos = (ulong)SDatasourceTell(tf->ds,  error);
	S_CHK_ERR(error, S_CONTERR,
			  "Tell",
			  "Call to \"SDatasourceTell\" failed");
	return pos;
}



static const SToken *GetToken(STokenstream *self, s_erc *error)
{
	const SToken *token;


	S_CLR_ERR(error);
	/* calling get_token of STokenstreamClass */
	token = S_TOKENSTREAM_CALL(tokenstream, get_token)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetToken",
				  "Call to method \"get_token\" failed"))
		return NULL;

	return token;
}


static const SToken *PeekToken(STokenstream *self, s_erc *error)
{
	const SToken *token;


	S_CLR_ERR(error);
	/* calling peek_token of STokenstreamClass */
	token = S_TOKENSTREAM_CALL(tokenstream, peek_token)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "PeekToken",
				  "Call to method \"peek_token\" failed"))
		return NULL;

	return token;
}


static void SetWhitespaceChars(STokenstream *self, const char *white_space_chars,
							   s_erc *error)
{
	S_CLR_ERR(error);
	/* calling set_whitespace_chars of STokenstreamClass */
	S_TOKENSTREAM_CALL(tokenstream, set_whitespace_chars)(self, white_space_chars, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetWhitespaceChars",
			  "Call to method \"set_whitespace_chars\" failed");
}


static void SetSingleChars(STokenstream *self, const char *single_chars,
						   s_erc *error)
{
	S_CLR_ERR(error);
	/* calling set_single_chars of STokenstreamClass */
	S_TOKENSTREAM_CALL(tokenstream, set_single_chars)(self, single_chars, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetSingleChars",
			  "Call to method \"set_single_chars\" failed");
}


static void SetPrePuncChars(STokenstream *self, const char *pre_punc_chars,
							s_erc *error)
{
	S_CLR_ERR(error);
	/* calling set_prepunc_chars of STokenstreamClass */
	S_TOKENSTREAM_CALL(tokenstream, set_prepunc_chars)(self, pre_punc_chars, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetPrePuncChars",
			  "Call to method \"set_prepunc_chars\" failed");
}


static void SetPostPuncChars(STokenstream *self, const char *post_punc_chars,
							 s_erc *error)
{
	S_CLR_ERR(error);
	/* calling set_postpunc_chars of STokenstreamClass */
	S_TOKENSTREAM_CALL(tokenstream, set_postpunc_chars)(self, post_punc_chars, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetPostPuncChars",
			  "Call to method \"set_postpunc_chars\" failed");
}


static void SetQuotes(STokenstream *self, uint32 quote, uint32 escape, s_erc *error)
{
	S_CLR_ERR(error);
	/* calling set_quotes of STokenstreamClass */
	S_TOKENSTREAM_CALL(tokenstream, set_quotes)(self, quote, escape, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetQuotes",
			  "Call to method \"set_quotes\" failed");
}


static s_bool QueryQuoteMode(const STokenstream *self, s_erc *error)
{
	S_CLR_ERR(error);

	return self->quote_mode;
}


static s_bool QueryEOF(const STokenstream *self, s_erc *error)
{
	S_CLR_ERR(error);

	return self->eof;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_tokenstream_file_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&TokenstreamFileClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_tokenstream_file_class_add",
			  "Failed to add STokenstreamFileClass");
}


/************************************************************************************/
/*                                                                                  */
/* STokenstreamFile class initialization                                            */
/*                                                                                  */
/************************************************************************************/

static STokenstreamFileClass TokenstreamFileClass =
{
	/* SObjectClass */
	{
		"STokenstream:STokenstreamFile",
		sizeof(STokenstreamFile),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* STokenstreamClass */
	GetChar,             /* get_char             */
	Seek,                /* seek                 */
	Tell,                /* tell                 */
	GetToken,            /* get_token            */
	PeekToken,           /* peek_token           */
	SetWhitespaceChars,  /* set_whitespace_chars */
	SetSingleChars,      /* set_single_chars     */
	SetPrePuncChars,     /* set_prepunc_chars    */
	SetPostPuncChars,    /* set_postpunc_chars   */
	SetQuotes,           /* set_quotes           */
	QueryQuoteMode,      /* query_quote_mode     */
	QueryEOF             /* query_eof            */
};
