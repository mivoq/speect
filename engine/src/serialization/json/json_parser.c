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
/* DATE    : 27 April 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* JSON data source parser class.                                                   */
/*                                                                                  */
/* Adapted from Yet Another JSON Library (YAJL) (0.4.0)                             */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* Yet Another JSON Library License                                                 */
/*                                                                                  */
/* Copyright 2007, Lloyd Hilaiel.                                                   */
/*                                                                                  */
/* Redistribution and use in source and binary forms, with or without               */
/* modification, are permitted provided that the following conditions are           */
/* met:                                                                             */
/*                                                                                  */
/*  1. Redistributions of source code must retain the above copyright               */
/*     notice, this list of conditions and the following disclaimer.                */
/*                                                                                  */
/*  2. Redistributions in binary form must reproduce the above copyright            */
/*     notice, this list of conditions and the following disclaimer in              */
/*     the documentation and/or other materials provided with the                   */
/*     distribution.                                                                */
/*                                                                                  */
/*  3. Neither the name of Lloyd Hilaiel nor the names of its                       */
/*     contributors may be used to endorse or promote products derived              */
/*     from this software without specific prior written permission.                */
/*                                                                                  */
/* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR             */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED                   */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           */
/* DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,               */
/* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES               */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR               */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)               */
/* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,              */
/* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING            */
/* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE               */
/* POSSIBILITY OF SUCH DAMAGE.                                                      */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <errno.h>
#include <math.h>
#include "base/utils/alloc.h"
#include "base/strings/strings.h"
#include "base/objsystem/objsystem.h"
#include "serialization/json/json_decode.h"
#include "serialization/json/json_parser.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given #SJSONParser child class object as a #SJSONParser object.
 * @param SELF The given object.
 * @return Given object as #SJSONParser* type.
 * @note This casting is not safety checked.
 */
#define S_JSONPARSER(SELF)    ((SJSONParser *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SJSONParser,
 * see full description #S_JSONPARSER_CALL for usage.
 * @param SELF The given #SJSONParser*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage: @code S_JSONPARSER_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_JSONPARSER_CALL(SELF, FUNC)				\
	((SJSONParserClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SJSONParser
 * can be called.
 * @param SELF The given #SJSONParser*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_JSONPARSER_METH_VALID(SELF, FUNC)				\
	S_JSONPARSER_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

typedef enum
{
	S_JSON_STATE_START = 0,
	S_JSON_STATE_PARSE_COMPLETE,
	S_JSON_STATE_PARSE_ERROR,
	S_JSON_STATE_LEXICAL_ERROR,
	S_JSON_STATE_MAP_START,
	S_JSON_STATE_MAP_SEP,
	S_JSON_STATE_MAP_NEED_VAL,
	S_JSON_STATE_MAP_GOT_VAL,
	S_JSON_STATE_MAP_NEED_KEY,
	S_JSON_STATE_ARRAY_START,
	S_JSON_STATE_ARRAY_GOT_VAL,
	S_JSON_STATE_ARRAY_NEED_VAL
} s_json_state;


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

#define S_JSON_BUF_SIZE 4096


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SJSONParserClass JSONParserClass; /* SJSONParser class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void s_json_state_push(SJSONParser *self, s_json_state s, s_erc *error);

static s_json_state s_json_state_current(SJSONParser *self, s_erc *error);

static s_json_state s_json_state_pop(SJSONParser *self, s_erc *error);

static void s_json_state_set(SJSONParser *self, s_json_state state, s_erc *error);

static void s_json_do_parse(SJSONParser *self, uint *offset,
							const uchar *text, uint text_len, s_erc *error);

static void s_json_parse(SJSONParser *self, const uchar *text, uint text_len, s_erc *error);

static uchar *s_json_render_error_string(SJSONParser *self, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SJSONParserInit(SJSONParser **self, SDatasource *ds,
						   s_bool allow_comments, s_bool check_utf8,
						   const s_json_callbacks *callbacks, void *ctx, s_erc *error)

{
	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SJSONParserInit",
				  "Argument \"self\" is NULL");
		return;
	}

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SJSONParserInit",
				  "Argument \"ds\" is NULL");
		S_DELETE(*self, "SJSONParserInit", error);
		*self = NULL;
		return;
	}

	if (callbacks == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SJSONParserInit",
				  "Argument \"callbacks\" is NULL");
		S_DELETE(*self, "SJSONParserInit", error);
		*self = NULL;
		return;
	}

	if (ctx == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SJSONParserInit",
				  "Argument \"ctx\" is NULL");
		S_DELETE(*self, "SJSONParserInit", error);
		*self = NULL;
		return;
	}

	s_mutex_lock(&(*self)->json_mutex);

	S_DATAREADER(*self)->ds = ds;
	(*self)->callbacks = callbacks;
	(*self)->ctx = ctx;
	(*self)->lexer = s_json_lex_new(allow_comments, check_utf8, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SJSONParserInit",
				  "Failed to create new lexer"))
	{
		s_mutex_unlock(&(*self)->json_mutex);
		S_DELETE(*self, "SJSONParserInit", error);
		*self = NULL;
		return;
	}

	(*self)->decode_buf = s_buffer_new(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SJSONParserInit",
				  "Failed to create new decode buffer"))
	{
		s_json_lex_delete((*self)->lexer, error);
		S_NEW_ERR(error, S_FAILURE);
		s_mutex_unlock(&(*self)->json_mutex);
		S_DELETE(*self, "SJSONParserInit", error);
		*self = NULL;
		return;
	}

	(*self)->state_buf = s_buffer_new(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SJSONParserInit",
				  "Failed to create new state buffer"))
	{
		s_json_lex_delete((*self)->lexer, error);
		s_buffer_delete((*self)->decode_buf, error);
		S_NEW_ERR(error, S_FAILURE);
		s_mutex_unlock(&(*self)->json_mutex);
		S_DELETE(*self, "SJSONParserInit", error);
		*self = NULL;
		return;
	}

	s_json_state_push(*self, S_JSON_STATE_START, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SJSONParserInit",
				  "Failed to push state"))
	{
		s_json_lex_delete((*self)->lexer, error);
		s_buffer_delete((*self)->decode_buf, error);
		s_buffer_delete((*self)->state_buf, error);
		S_NEW_ERR(error, S_FAILURE);
		s_mutex_unlock(&(*self)->json_mutex);
		S_DELETE(*self, "SJSONParserInit", error);
		*self = NULL;
		return;
	}

	s_mutex_unlock(&self->json_mutex);
}


S_API void SJSONParserParse(SJSONParser *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SJSONParserParse",
				  "Argument \"self\" is NULL");
		return;
	}

	if (!S_JSONPARSER_METH_VALID(self, parse))
	{
		S_WARNING(S_METHINVLD,
				  "SJSONParserParse",
				  "Parser method \"parse\" not implemented");
		return;
	}

	s_mutex_lock(&self->json_mutex);
	S_JSONPARSER_CALL(self, parse)(self, error);
	s_mutex_unlock(&self->json_mutex);

	S_CHK_ERR(error, S_CONTERR,
			  "SJSONParserParse",
			  "Failed to parse data source");
}


S_API const char *SJSONParseGetError(SJSONParser *self, s_erc *error)
{
	/* don't clear error */

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SJSONParseGetError",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	return self->error;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_json_parser_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&JSONParserClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_json_parser_class_add",
			  "Failed to add SJSONParserClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void s_json_state_push(SJSONParser *self, s_json_state s, s_erc *error)
{
	uchar c = (uchar) s;

	S_CLR_ERR(error);
	s_buffer_append(self->state_buf, &c, sizeof(c), error);
	S_CHK_ERR(error, S_CONTERR,
			  "s_json_state_push",
			  "Call to s_buffer_append failed");
}


static s_json_state s_json_state_current(SJSONParser *self, s_erc *error)
{
	size_t size;


	S_CLR_ERR(error);
	size = s_buffer_size(self->state_buf, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_state_current",
				  "Call to s_buffer_size failed"))
		return S_JSON_STATE_PARSE_ERROR;

	if (size <= 0)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "s_json_state_current",
				  "State buffer length <= 0");
		return S_JSON_STATE_PARSE_ERROR;
	}

	return ((s_json_state) *(s_buffer_data(self->state_buf, error) +
							 s_buffer_size(self->state_buf, error) - 1));
}


static s_json_state s_json_state_pop(SJSONParser *self, s_erc *error)
{
	s_json_state s;
	size_t size;

	S_CLR_ERR(error);
	size = s_buffer_size(self->state_buf, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_state_pop",
				  "Call to s_buffer_size failed"))
		return S_JSON_STATE_PARSE_ERROR;

	/* start state is never popped */
	if (size <= 1)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "s_json_state_pop",
				  "State buffer length <= 1");
		return S_JSON_STATE_PARSE_ERROR;
	}

	s = (s_json_state) (*(s_buffer_data(self->state_buf, error) + size - 1));
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_state_pop",
				  "Call to s_buffer_size failed"))
		return S_JSON_STATE_PARSE_ERROR;

	s_buffer_truncate(self->state_buf, size - 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_state_pop",
				  "Call to s_buffer_truncate failed"))
		return S_JSON_STATE_PARSE_ERROR;

	return s;
}


static void s_json_state_set(SJSONParser *self, s_json_state state, s_erc *error)
{
	size_t size;


	S_CLR_ERR(error);
	size = s_buffer_size(self->state_buf, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_state_set",
				  "Call to s_buffer_size failed"))
		return;

	if (size <= 0)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "s_json_state_set",
				  "State buffer length <= 0");
		return;
	}


	*(uchar*) (s_buffer_data(self->state_buf, error) +
			   s_buffer_size(self->state_buf, error) - 1) = (uchar) state;
	S_CHK_ERR(error, S_CONTERR,
			  "s_json_state_set",
			  "Call to s_buffer_date/size failed");
}


/* check for client cancelation */
#define _CC_CHK(x)													\
	/* run callback */												\
	(x);															\
	/* check error */												\
	if (S_CHK_ERR(error, S_CONTERR,									\
				  "s_json_do_parse",								\
				  "Callback \"%s\" failed", # x))					\
	{																\
		s_json_state_set(self, S_JSON_STATE_PARSE_ERROR, error);	\
		self->parse_error =											\
			"client cancelled parse via callback return value";		\
		S_CTX_ERR(error, S_CONTERR,									\
				  "s_json_do_parse",								\
				  "Parse error \"%s\"", self->parse_error);			\
		return;														\
	}



static void s_json_do_parse(SJSONParser *self, uint *offset,
							const uchar *text, uint text_len, s_erc *error)
{

	s_json_tok tok;
	const uchar *buf;
	uint buf_len;


	S_CLR_ERR(error);
around_again:
	switch (s_json_state_current(self, error))
	{
	case S_JSON_STATE_PARSE_COMPLETE:
		return;
	case S_JSON_STATE_LEXICAL_ERROR:
	case S_JSON_STATE_PARSE_ERROR:
	{
		self->error_offset = *offset;
		S_CTX_ERR(error, S_FAILURE,
				  "s_json_do_parse",
				  "IO error occured");
		return;
	}
	case S_JSON_STATE_START:
	case S_JSON_STATE_MAP_NEED_VAL:
	case S_JSON_STATE_ARRAY_NEED_VAL:
	case S_JSON_STATE_ARRAY_START:
	{
		/* for arrays and maps, we advance the state for this
		 * depth, then push the state of the next depth.
		 * If an error occurs during the parsing of the nesting
		 * enitity, the state at this level will not matter.
		 * a state that needs pushing will be anything other
		 * than state_start */
		s_json_state state_to_push = S_JSON_STATE_START;

		tok = s_json_lex_lex(self->lexer, text, text_len,
							 (size_t *)offset, &buf, &buf_len, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_do_parse",
					  "Call to s_json_lex_lex failed"))
			return;

		switch (tok)
		{
		case S_JSON_TOK_EOF:
			/* don't log S_IOEOF, just send it up, caller must decide
			 * if it is an error
			 */
			S_NEW_ERR(error, S_IOEOF);
			return;
		case S_JSON_TOK_ERROR:
			s_json_state_set(self, S_JSON_STATE_LEXICAL_ERROR, error);
			S_CHK_ERR(error, S_CONTERR,
					  "s_json_do_parse",
					  "Call to s_json_state_set failed");
			goto around_again;
		case S_JSON_TOK_STRING:
			if (self->callbacks && self->callbacks->s_json_string)
			{
				_CC_CHK(self->callbacks->s_json_string(self->ctx,
													   buf, buf_len, error));
			}
			break;
		case S_JSON_TOK_STRING_WITH_ESCAPES:
			if (self->callbacks && self->callbacks->s_json_string)
			{
				s_buffer_clear(self->decode_buf, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_do_parse",
							  "Call to s_buffer_clear failed"))
					return;
				s_json_string_decode(self->decode_buf, buf, buf_len, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_do_parse",
							  "Call to s_json_string_decode failed"))
					return;
				_CC_CHK(self->callbacks->s_json_string(
							self->ctx, s_buffer_data(self->decode_buf, error),
							s_buffer_size(self->decode_buf, error), error));
			}
			break;
		case S_JSON_TOK_BOOL:
			if (self->callbacks && self->callbacks->s_json_boolean)
			{
				_CC_CHK(self->callbacks->s_json_boolean(self->ctx,
														*buf == 't', error));
			}
			break;
		case S_JSON_TOK_NULL:
			if (self->callbacks && self->callbacks->s_json_null)
			{
				_CC_CHK(self->callbacks->s_json_null(self->ctx, error));
			}
			break;
		case S_JSON_TOK_LEFT_BRACKET:
			if (self->callbacks && self->callbacks->s_json_start_map)
			{
				_CC_CHK(self->callbacks->s_json_start_map(self->ctx, error));
			}
			state_to_push = S_JSON_STATE_MAP_START;
			break;
		case S_JSON_TOK_LEFT_BRACE:
			if (self->callbacks && self->callbacks->s_json_start_array)
			{
				_CC_CHK(self->callbacks->s_json_start_array(self->ctx, error));
			}
			state_to_push = S_JSON_STATE_ARRAY_START;
			break;
		case S_JSON_TOK_INTEGER:
			/*
			 * note.  strtol does not respect the length of
			 * the lexical token.  in a corner case where the
			 * lexed number is a integer with a trailing zero,
			 * immediately followed by the end of buffer,
			 * sscanf could run off into oblivion and cause a
			 * crash.  for this reason we copy the integer
			 * (and doubles), into our parse buffer (the same
			 * one used for unescaping strings), before
			 * calling strtol.  s_json_buf ensures null padding,
			 * so we're safe.
			 */
			if (self->callbacks)
			{
				if (self->callbacks->s_json_number)
				{
					_CC_CHK(self->callbacks->s_json_number(self->ctx,
														   (char *) buf,
														   buf_len, error));
				}
				else if (self->callbacks->s_json_integer)
				{
					long int i = 0;
					s_buffer_clear(self->decode_buf, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_json_do_parse",
								  "Call to s_buffer_clear failed"))
						return;

					s_buffer_append(self->decode_buf, buf, buf_len, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_json_do_parse",
								  "Call to s_buffer_append failed"))
						return;

					buf = s_buffer_data(self->decode_buf, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_json_do_parse",
								  "Call to s_buffer_data failed"))
						return;

					i = s_strtol((char *) buf, NULL, 10, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_json_do_parse",
								  "Call to s_strtol failed"))
						return;

					if ((i == LONG_MIN || i == LONG_MAX) &&
					    errno == ERANGE)
					{
						s_json_state_set(self, S_JSON_STATE_PARSE_ERROR, error);
						if (S_CHK_ERR(error, S_CONTERR,
									  "s_json_do_parse",
									  "Call to s_json_state_set failed"))
							return;
						self->parse_error = "integer overflow" ;
						/* try to restore error offset */
						if (*offset >= buf_len)
							(*offset) -= buf_len;
						else
							(*offset) = 0;
						goto around_again;
					}
					_CC_CHK(self->callbacks->s_json_integer(self->ctx,
															i, error));
				}
			}
			break;
		case S_JSON_TOK_DOUBLE:
			if (self->callbacks)
			{
				if (self->callbacks->s_json_number)
				{
					_CC_CHK(self->callbacks->s_json_number(self->ctx,
														   (char *) buf,
														   buf_len, error));
				}
				else if (self->callbacks->s_json_double)
				{
					double d = 0.0;
					s_buffer_clear(self->decode_buf, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_json_do_parse",
								  "Call to s_buffer_clear failed"))
						return;

					s_buffer_append(self->decode_buf, buf, buf_len, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_json_do_parse",
								  "Call to s_buffer_append failed"))
						return;

					buf = s_buffer_data(self->decode_buf, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_json_do_parse",
								  "Call to s_buffer_data failed"))
						return;

					d = s_strtod((char *) buf, NULL, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_json_do_parse",
								  "Call to s_strtod failed"))
						return;

					if ((d == HUGE_VAL || d == -HUGE_VAL) &&
					    errno == ERANGE)
					{
						s_json_state_set(self, S_JSON_STATE_PARSE_ERROR, error);
						if (S_CHK_ERR(error, S_CONTERR,
									  "s_json_do_parse",
									  "Call to s_json_state_set failed"))
							return;
						self->parse_error = "numeric (floating point) "
							"overflow";
						/* try to restore error offset */
						if (*offset >= buf_len)
							(*offset) -= buf_len;
						else
							(*offset) = 0;
						goto around_again;
					}
					_CC_CHK(self->callbacks->s_json_double(self->ctx,
														   d, error));
				}
			}
			break;
		case S_JSON_TOK_RIGHT_BRACE:
		{
			if (s_json_state_current(self, error) == S_JSON_STATE_ARRAY_START)
			{
				if (self->callbacks &&
				    self->callbacks->s_json_end_array)
				{
					_CC_CHK(self->callbacks->s_json_end_array(self->ctx, error));
				}
				(void) s_json_state_pop(self, error);
				goto around_again;
			}
			/* intentional fall-through */
		}
		case S_JSON_TOK_COLON:
		case S_JSON_TOK_COMMA:
		case S_JSON_TOK_RIGHT_BRACKET:
			s_json_state_set(self, S_JSON_STATE_PARSE_ERROR, error);
			self->parse_error =
				"unallowed token at this point in JSON text";
			S_CTX_ERR(error, S_CONTERR,
					  "s_json_do_parse",
					  "unallowed token at this point in JSON text");
			goto around_again;
		default:
			s_json_state_set(self, S_JSON_STATE_PARSE_ERROR, error);
			self->parse_error = "invalid token, internal error";
			S_CTX_ERR(error, S_CONTERR,
					  "s_json_do_parse",
					  "invalid token, internal error");
			goto around_again;
		}
		/* got a value.  transition depends on the state we're in. */
		{
			s_json_state s = s_json_state_current(self, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_set failed"))
				return;

			if (s == S_JSON_STATE_START)
			{
				s_json_state_set(self, S_JSON_STATE_PARSE_COMPLETE, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_do_parse",
							  "Call to s_json_state_set failed"))
					return;
			}
			else if (s == S_JSON_STATE_MAP_NEED_VAL)
			{
				s_json_state_set(self, S_JSON_STATE_MAP_GOT_VAL, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_do_parse",
							  "Call to s_json_state_set failed"))
					return;
			}
			else
			{
				s_json_state_set(self, S_JSON_STATE_ARRAY_GOT_VAL, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_do_parse",
							  "Call to s_json_state_set failed"))
					return;
			}
		}
		if (state_to_push != S_JSON_STATE_START)
		{
			s_json_state_push(self, state_to_push, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_push failed"))
				return;
		}

		goto around_again;
	}
	case S_JSON_STATE_MAP_START:
	case S_JSON_STATE_MAP_NEED_KEY:
	{
		/* only difference between these two states is that in
		 * start '}' is valid, whereas in need_key, we've parsed
		 * a comma, and a string key _must_ follow */
		tok = s_json_lex_lex(self->lexer, text, text_len,
							 (size_t *) offset, &buf, &buf_len, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_do_parse",
					  "Call to s_json_lex_lex failed"))
			return;
		switch (tok)
		{
		case S_JSON_TOK_EOF:
			/* don't log S_IOEOF, just send it up, caller must decide
			 * if it is an error
			 */
			S_NEW_ERR(error, S_IOEOF);
			return;
		case S_JSON_TOK_ERROR:
			s_json_state_set(self, S_JSON_STATE_LEXICAL_ERROR, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_set failed"))
				return;
			goto around_again;
		case S_JSON_TOK_STRING_WITH_ESCAPES:
			if (self->callbacks && self->callbacks->s_json_map_key)
			{
				s_buffer_clear(self->decode_buf, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_do_parse",
							  "Call to s_buffer_clear failed"))
					return;

				s_json_string_decode(self->decode_buf, buf, buf_len, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_do_parse",
							  "Call to s_json_string_decode failed"))
					return;

				buf = s_buffer_data(self->decode_buf, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_do_parse",
							  "Call to s_buffer_data failed"))
					return;

				buf_len = s_buffer_size(self->decode_buf, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_do_parse",
							  "Call to s_buffer_size failed"))
					return;
			}
			/* intentional fall-through */
		case S_JSON_TOK_STRING:
			if (self->callbacks && self->callbacks->s_json_map_key)
			{
				_CC_CHK(self->callbacks->s_json_map_key(self->ctx, buf,
														buf_len, error));
			}
			s_json_state_set(self, S_JSON_STATE_MAP_SEP, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_set failed"))
				return;
			goto around_again;
		case S_JSON_TOK_RIGHT_BRACKET:
			if (s_json_state_current(self, error) == S_JSON_STATE_MAP_START)
			{
				if (self->callbacks && self->callbacks->s_json_end_map)
				{
					_CC_CHK(self->callbacks->s_json_end_map(self->ctx, error));
				}
				(void) s_json_state_pop(self, error);
				goto around_again;
			}
		default:
			s_json_state_set(self, S_JSON_STATE_PARSE_ERROR, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_set failed"))
				return;
			self->parse_error =
				"invalid object key (must be a string)";
			S_CTX_ERR(error, S_FAILURE,
					  "s_json_do_parse",
					  "invalid object key (must be a string)");
			goto around_again;
		}
	}
	case S_JSON_STATE_MAP_SEP:
	{
		tok = s_json_lex_lex(self->lexer, text, text_len,
							 (size_t *)offset, &buf, &buf_len, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_do_parse",
					  "Call to s_json_lex_lex failed"))
			return;
		switch (tok)
		{
		case S_JSON_TOK_COLON:
			s_json_state_set(self, S_JSON_STATE_MAP_NEED_VAL, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_set failed"))
				return;
			goto around_again;
		case S_JSON_TOK_EOF:
			/* don't log S_IOEOF, just send it up, caller must decide
			 * if it is an error
			 */
			S_NEW_ERR(error, S_IOEOF);
			return;
		case S_JSON_TOK_ERROR:
			s_json_state_set(self, S_JSON_STATE_LEXICAL_ERROR, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_set failed"))
				return;
			goto around_again;
		default:
			s_json_state_set(self, S_JSON_STATE_PARSE_ERROR, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_set failed"))
				return;
			self->parse_error = "object key and value must "
				"be separated by a colon (':')";
			S_CTX_ERR(error, S_FAILURE,
					  "s_json_do_parse",
					  "object key and value must be separated by a colon (':')");
			goto around_again;
		}
	}
	case S_JSON_STATE_MAP_GOT_VAL:
	{
		tok = s_json_lex_lex(self->lexer, text, text_len,
							 (size_t *)offset, &buf, &buf_len, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_do_parse",
					  "Call to s_json_lex_lex failed"))
			return;
		switch (tok)
		{
		case S_JSON_TOK_RIGHT_BRACKET:
			if (self->callbacks && self->callbacks->s_json_end_map)
			{
				_CC_CHK(self->callbacks->s_json_end_map(self->ctx, error));
			}
			(void) s_json_state_pop(self, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_pop failed"))
				return;
			goto around_again;
		case S_JSON_TOK_COMMA:
			s_json_state_set(self, S_JSON_STATE_MAP_NEED_KEY, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_set failed"))
				return;
			goto around_again;
		case S_JSON_TOK_EOF:
			/* don't log S_IOEOF, just send it up, caller must decide
			 * if it is an error
			 */
			S_NEW_ERR(error, S_IOEOF);
			return;
		case S_JSON_TOK_ERROR:
			s_json_state_set(self, S_JSON_STATE_LEXICAL_ERROR, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_set failed"))
				return;
			goto around_again;
		default:
			s_json_state_set(self, S_JSON_STATE_PARSE_ERROR, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_set failed"))
				return;
			self->parse_error = "after key and value, inside map, "
				"I expect ',' or '}'";
			/* try to restore error offset */
			if (*offset >= buf_len)
				(*offset) -= buf_len;
			else
				(*offset) = 0;
			S_CTX_ERR(error, S_FAILURE,
					  "s_json_do_parse",
					  "after key and value, inside map, I expect ',' or '}'");
			goto around_again;
		}
	}
	case S_JSON_STATE_ARRAY_GOT_VAL:
	{
		tok = s_json_lex_lex(self->lexer, text, text_len,
							 (size_t *)offset, &buf, &buf_len, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_do_parse",
					  "Call to s_json_lex_lex failed"))
			return;
		switch (tok)
		{
		case S_JSON_TOK_RIGHT_BRACE:
			if (self->callbacks && self->callbacks->s_json_end_array)
			{
				_CC_CHK(self->callbacks->s_json_end_array(self->ctx, error));
			}
			(void) s_json_state_pop(self, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_pop failed"))
				return;
			goto around_again;
		case S_JSON_TOK_COMMA:
			s_json_state_set(self, S_JSON_STATE_ARRAY_NEED_VAL, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_set failed"))
				return;
			goto around_again;
		case S_JSON_TOK_EOF:
			/* don't log S_IOEOF, just send it up, caller must decide
			 * if it is an error
			 */
			S_NEW_ERR(error, S_IOEOF);
			return;
		case S_JSON_TOK_ERROR:
			s_json_state_set(self, S_JSON_STATE_LEXICAL_ERROR, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_set failed"))
				return;
			goto around_again;
		default:
			s_json_state_set(self, S_JSON_STATE_PARSE_ERROR, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_do_parse",
						  "Call to s_json_state_set failed"))
				return;
			self->parse_error =
				"after array element, I expect ',' or ']'";
			S_CTX_ERR(error, S_FAILURE,
					  "s_json_do_parse",
					  "after array element, I expect ',' or ']'");
			goto around_again;
		}
	}
	}

	return;
}


static void s_json_parse(SJSONParser *self, const uchar *text, uint text_len, s_erc *error)
{
	uint offset = 0;


	S_CLR_ERR(error);
	s_json_do_parse(self, &offset, text, text_len, error);
	if ((error != NULL) && (*error != S_IOEOF))
		S_CHK_ERR(error, S_CONTERR,
				  "s_json_parse",
				  "Call to s_json_do_parse failed");
}


static uchar *s_json_render_error_string(SJSONParser *self, s_erc *error)
{
	uchar *str;
	const char *error_type;
	const char *error_text = NULL;


	S_CLR_ERR(error);


	if (s_json_state_current(self, error) == S_JSON_STATE_PARSE_ERROR)
	{
		error_type = "parse";
		error_text = self->parse_error;
	}
	else if (s_json_state_current(self, error) == S_JSON_STATE_LEXICAL_ERROR)
	{
		error_type = "lexical";
		error_text = s_json_lex_err_to_str(s_json_lex_get_err(self->lexer,
															  error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_render_error_string",
					  "Call to s_json_lex_err_to_str failed"))
			return NULL;
	}
	else
	{
		error_type = "unknown";
	}

	{
		uint memneeded = 0;
		memneeded += s_strzsize(error_type, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_render_error_string",
					  "Call to s_strzsize failed"))
			return NULL;

		memneeded += s_strzsize(" error", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_render_error_string",
					  "Call to s_strzsize failed"))
			return NULL;

		if (error_text != NULL)
		{
			memneeded += s_strzsize(": ", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_render_error_string",
						  "Call to s_strzsize failed"))
				return NULL;

			memneeded += s_strzsize(error_text, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_render_error_string",
						  "Call to s_strzsize failed"))
				return NULL;
		}

		str = (uchar*)S_MALLOC(char, memneeded + 2);
		if (str == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "s_json_render_error_string",
					  "Failed to allocate memory for string");
			return NULL;
		}
		str[0] = 0;
		s_strcat((char*) str, error_type, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_render_error_string",
					  "Call to s_strcat failed"))
		{
			S_FREE(str);
			return NULL;
		}

		s_strcat((char*) str, " error", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_render_error_string",
					  "Call to s_strcat failed"))
		{
			S_FREE(str);
			return NULL;
		}

		if (error_text != NULL)
		{
			s_strcat((char*) str, ": ", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_render_error_string",
						  "Call to s_strcat failed"))
			{
				S_FREE(str);
				return NULL;
			}

			s_strcat((char*) str, error_text, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_render_error_string",
						  "Call to s_strcat failed"))
			{
				S_FREE(str);
				return NULL;
			}
		}
	}

	return str;
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitJSONParser(void *obj, s_erc *error)
{
	SJSONParser *self = obj;


	S_CLR_ERR(error);
	self->callbacks = NULL;
	self->ctx = NULL;
	self->lexer = NULL;
	self->decode_buf = NULL;
	self->state_buf = NULL;
	self->error_offset = 0;
	self->error = NULL;

	s_mutex_init(&self->json_mutex);
}


static void DestroyJSONParser(void *obj, s_erc *error)
{
	SJSONParser *self = obj;
	char *tmp;
	s_erc derror = S_SUCCESS;


	S_CLR_ERR(error);

	s_mutex_lock(&self->json_mutex);

	if (self->lexer != NULL)
	{
		s_json_lex_delete(self->lexer, error);
		if (*error != S_SUCCESS)
			derror = *error;
	}

	if (self->decode_buf != NULL)
	{
		s_buffer_delete(self->decode_buf, error);
		if (*error != S_SUCCESS)
			derror = *error;
	}

	if (self->state_buf != NULL)
	{
		s_buffer_delete(self->state_buf, error);
		if (*error != S_SUCCESS)
			derror = *error;
	}

	if (self->error != NULL)
	{
		tmp = (char*)self->error;
		S_FREE(tmp);
	}

	if (derror != S_SUCCESS)
		S_CTX_ERR(error, S_FAILURE,
				  "DestroyJSONParser",
				  "Failed to successfully destroy parser");

	s_mutex_unlock(&self->json_mutex);
	s_mutex_destroy(&self->json_mutex);
}


static void DisposeJSONParser(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void ParseJSONParser(SJSONParser *self, s_erc *error)
{
	uchar *file_data;
	s_erc local_err = S_SUCCESS;
	s_bool eof = FALSE;


	S_CLR_ERR(error);

	while (!eof)
	{
		file_data = S_MALLOC(uchar, S_JSON_BUF_SIZE);
		if (file_data == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "ParseJSONParser",
					  "Failed to allocate memory for file_data object");
			return;
		}

		SDatasourceRead(S_DATAREADER(self)->ds, (void*)file_data,
						1, S_JSON_BUF_SIZE, &local_err);
		if ((local_err != S_SUCCESS) && (local_err != S_IOEOF))
		{
			S_CTX_ERR(error, local_err,
					  "ParseJSONParser",
					  "Error while reading from data source");
			S_FREE(file_data);
			return;
		}

		if (local_err == S_IOEOF)
			eof = TRUE;

		/* read file data, pass to parser */
		s_json_parse(self, file_data, S_DATAREADER(self)->ds->rd_last, &local_err);
		if ((local_err != S_SUCCESS) && (local_err != S_IOEOF))
		{
			self->error = (const char*)s_json_render_error_string(self, error);
			S_CTX_ERR(error, S_CONTERR,
					  "ParseJSONParser",
					  "Error while parsing data read from file, \"%s\"", self->error);
			S_FREE(file_data);
			return;
		}

		if ((local_err == S_IOEOF) && (eof))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ParseJSONParser",
					  "Parser expects more data but end-of-file has been reached");
			S_FREE(file_data);
			return;
		}

		S_FREE(file_data);
	}
}


/************************************************************************************/
/*                                                                                  */
/* SJSONParser class initialization                                                 */
/*                                                                                  */
/************************************************************************************/

static SJSONParserClass JSONParserClass =
{
	{
		/* SObjectClass */
		{
			"SDataReader:SJSONParser",
			sizeof(SJSONParser),
			{ 0, 1},
			InitJSONParser,    /* init    */
			DestroyJSONParser, /* destroy */
			DisposeJSONParser, /* dispose */
			NULL,              /* compare */
			NULL,              /* print   */
			NULL,              /* copy    */
		},
		/* SDataReader */
		/* nothing */
	},
	/* SJSONParserClass */
	ParseJSONParser            /* parse   */
};
