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
/* JSON lexical parser.                                                             */
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

#include <stdio.h>
#include "base/utils/alloc.h"
#include "serialization/json/json_lex.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

#define S_JSON_LEX_INIT_SIZE 2048

#define S_JSON_LEXER_DEBUG
#undef S_JSON_LEXER_DEBUG

#ifdef SJSON_LEXER_DEBUG
static const char *s_tok_to_str(s_json_tok tok);
#endif


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/* Macro used in lexer errors, sets Speect error.
 *
 *
 * @param ERROR Error code variable pointer (s_erc *)
 * @param LEXER Pointer to lexer (s_json_lexer *)
 * @param FUNC Function name where error occurred (string)
 *
 * @note line_no + 1 is because line_no begins at 0
 */
#define S_JSON_ERR(ERROR, LEXER, FUNC)									\
	S_CTX_ERR(ERROR, S_FAILURE,											\
			  FUNC,														\
			  "JSON error: \"%s\", current character: \"%c\", line %d",	\
			  s_json_lex_err_to_str(lexer->error),						\
			  s_json_lex_current_char(lexer, NULL),						\
			  s_json_lex_current_line(lexer, NULL) + 1)

/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/*
 * Impact of the stream parsing feature on the lexer:
 *
 * SPCT_JSONParser supports stream parsing.  That is, the ability to parse the first
 * bits of a chunk of JSON before the last bits are available (still on
 * the network or disk).  This makes the lexer more complex.  The
 * responsibility of the lexer is to handle transparently the case where
 * a chunk boundary falls in the middle of a token.  This is
 * accomplished is via a buffer and a character reading abstraction.
 *
 * Overview of implementation
 *
 * When we lex to end of input string before end of token is hit, we
 * copy all of the input text composing the token into our lex_buf.
 *
 * Every time we read a character, we do so through the readChar function.
 * readChar's responsibility is to handle pulling all chars from the buffer
 * before pulling chars from input text
 */
struct s_json_lexer
{
	/* the overal line and char offset into the data */
	uint32 line_off;
	uint32 char_off;

	/* error */
	s_json_lex_error error;

	/*
	 * a input buffer to handle the case where a token is spread over
	 * multiple chunks
	 */
	s_buffer *buf;

	/*
	 * in the case where we have data in the lex_buf, buf_off holds
	 * the current offset into the lex_buf.
	 */
	uint32 buf_off;

	/* are we using the lex buf? */
	s_bool buf_in_use;

	/* shall we allow comments? */
	s_bool allow_comments;

	/* shall we validate utf8 inside strings? */
	s_bool validate_utf8;
};


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static const char s_char_lookup_table[256];


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static uchar s_read_char(s_json_lexer *lxr, const uchar *txt,
						 size_t *off, s_erc *error);

static void s_unread_char(s_json_lexer *lxr, size_t *off, s_erc *error);

static s_json_tok s_json_lex_utf8_char(s_json_lexer *lexer, const uchar *text,
									   size_t text_len, size_t *offset,
									   uchar cur_char, s_erc *error);

static s_json_tok s_json_lex_string(s_json_lexer *lexer, const uchar *text,
									size_t text_len, size_t *offset, s_erc *error);

static s_json_tok s_json_lex_number(s_json_lexer *lexer, const uchar *text,
									size_t text_len, size_t *offset, s_erc *error);

static s_json_tok s_json_lex_comment(s_json_lexer *lexer, const uchar *text,
									 size_t text_len, size_t *offset, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL s_json_lexer *s_json_lex_new(s_bool allow_comments, s_bool validate_utf8, s_erc *error)
{
	s_json_lexer *lxr;


	S_CLR_ERR(error);

	lxr = S_CALLOC(s_json_lexer, 1);
	if (lxr == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_json_lex_new",
				  "Failed to allocate memory for object");
		return NULL;
	}

	lxr->buf = s_buffer_new(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_lex_new",
				  "Failed to create lex buffer"))
	{
		S_FREE(lxr);
		return NULL;
	}

	lxr->allow_comments = allow_comments;
	lxr->validate_utf8 = validate_utf8;

	return (lxr);
}


S_LOCAL void s_json_lex_delete(s_json_lexer *lxr, s_erc *error)
{
	S_CLR_ERR(error);

	if (lxr == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_delete",
				  "Argument \"lxr\" is NULL");
		return;
	}

	s_buffer_delete(lxr->buf, error);
	S_CHK_ERR(error, S_CONTERR,
			  "s_json_lex_delete",
			  "Failed to delete buffer");

	S_FREE(lxr);
}


S_LOCAL s_json_tok s_json_lex_lex(s_json_lexer *lexer, const uchar *text,
								  size_t text_len, size_t *context,
								  const uchar **out_buf, size_t *out_len, s_erc *error)
{
	s_json_tok tok = S_JSON_TOK_ERROR;
	uchar c;
	size_t start_ctx = *context;


	S_CLR_ERR(error);
	*out_buf = NULL;
	*out_len = 0;

	if (lexer == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_lex",
				  "Argument \"lexer\" is NULL");
		return S_JSON_TOK_ERROR;
	}

	for (/*void */; /*void */; /*void */)
	{
		if ((*context) >= text_len)
		{
			tok = S_JSON_TOK_EOF;
			goto lexed;
		}

		c = s_read_char(lexer, text, context, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_lex_lex",
					  "Failed to read character from stream"))
			return S_JSON_TOK_ERROR;

		lexer->char_off = c;

		switch (c)
		{
		case '{':
			tok = S_JSON_TOK_LEFT_BRACKET;
			goto lexed;
		case '}':
			tok = S_JSON_TOK_RIGHT_BRACKET;
			goto lexed;
		case '[':
			tok = S_JSON_TOK_LEFT_BRACE;
			goto lexed;
		case ']':
			tok = S_JSON_TOK_RIGHT_BRACE;
			goto lexed;
		case ',':
			tok = S_JSON_TOK_COMMA;
			goto lexed;
		case ':':
			tok = S_JSON_TOK_COLON;
			goto lexed;
		case '\n':
			lexer->line_off++; /* fall through */
		case '\t': case '\v': case '\f': case '\r': case ' ':
			start_ctx++;
			break;
		case 't':
		{
			const char *want = "rue";


			do
			{
				if (*context >= text_len)
				{
					tok = S_JSON_TOK_EOF;
					goto lexed;
				}

				c = s_read_char(lexer, text, context, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_lex_lex",
							  "Failed to read character from stream"))
					return S_JSON_TOK_ERROR;

				if (c != *want)
				{
					s_unread_char(lexer, context, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_json_lex_lex",
								  "Failed to unread character to stream"))
						return S_JSON_TOK_ERROR;

					lexer->error = S_JSON_LEX_INVALID_STRING;
					tok = S_JSON_TOK_ERROR;
					S_JSON_ERR(error, lexer, "s_json_lex_lex");
					goto lexed;
				}
			} while (*(++want));

			tok = S_JSON_TOK_BOOL;
			goto lexed;
		}
		case 'f':
		{
			const char *want = "alse";


			do
			{
				if (*context >= text_len)
				{
					tok = S_JSON_TOK_EOF;
					goto lexed;
				}

				c = s_read_char(lexer, text, context, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_lex_lex",
							  "Failed to read character from stream"))
					return S_JSON_TOK_ERROR;

				if (c != *want)
				{
					s_unread_char(lexer, context, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_json_lex_lex",
								  "Failed to unread character to stream"))
						return S_JSON_TOK_ERROR;

					lexer->error = S_JSON_LEX_INVALID_STRING;
					S_JSON_ERR(error, lexer, "s_json_lex_lex");
					tok = S_JSON_TOK_ERROR;
					goto lexed;
				}
			} while (*(++want));

			tok = S_JSON_TOK_BOOL;
			goto lexed;
		}
		case 'n':
		{
			const char * want = "ull";


			do
			{
				if (*context >= text_len)
				{
					tok = S_JSON_TOK_EOF;
					goto lexed;
				}

				c = s_read_char(lexer, text, context, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_lex_lex",
							  "Failed to read character from stream"))
					return S_JSON_TOK_ERROR;

				if (c != *want)
				{
					s_unread_char(lexer, context, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_json_lex_lex",
								  "Failed to unread character to stream"))
						return S_JSON_TOK_ERROR;

					lexer->error = S_JSON_LEX_INVALID_STRING;
					S_JSON_ERR(error, lexer, "s_json_lex_lex");
					tok = S_JSON_TOK_ERROR;
					goto lexed;
				}
			} while (*(++want));

			tok = S_JSON_TOK_NULL;
			goto lexed;
		}
		case '"':
		{
			tok = s_json_lex_string(lexer, (uchar *) text,
									text_len, context, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_lex",
						  "Failed to get token from string"))
				return S_JSON_TOK_ERROR;
			goto lexed;
		}
		case '-':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			{
				/* integer parsing wants to start from the beginning */
				s_unread_char(lexer, context, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_lex_lex",
							  "Failed to unread character to stream"))
					return S_JSON_TOK_ERROR;

				tok = s_json_lex_number(lexer, (uchar *) text,
										text_len, context, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_lex_lex",
							  "Failed to get token from number"))
					return S_JSON_TOK_ERROR;

				goto lexed;
			}
		case '/':
		{
			/* hey, look, a probable comment!  If comments are disabled
			 * it's an error. */
			if (lexer->allow_comments == FALSE)
			{
				s_unread_char(lexer, context, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_lex_lex",
							  "Failed to unread character to stream"))
					return S_JSON_TOK_ERROR;

				lexer->error = S_JSON_LEX_UNALLOWED_COMMENT;
				tok = S_JSON_TOK_EOF;
				S_JSON_ERR(error, lexer, "s_json_lex_lex");
				goto lexed;
			}
			/* if comments are enabled, then we should try to lex
			 * the thing.  possible outcomes are
			 * - successful lex (tok_comment, which means continue),
			 * - malformed comment opening (slash not followed by
			 *   '*' or '/') (tok_error)
			 * - eof hit. (tok_eof) */
			tok = s_json_lex_comment(lexer, (uchar *) text,
									 text_len, context, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_lex",
						  "Failed to get token from comment"))
				return S_JSON_TOK_ERROR;

			if (tok == S_JSON_TOK_COMMENT)
			{
				/*
				 * "error" is silly, but that's the initial
				 * state of tok.  guilty until proven innocent.
				 */
				tok = S_JSON_TOK_ERROR;
				s_buffer_clear(lexer->buf, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_lex_lex",
							  "Failed to clear buffer"))
					return S_JSON_TOK_ERROR;

				lexer->buf_in_use = 0;
				start_ctx = *context;
				break;
			}
			/* hit error or eof, bail */
			goto lexed;
		}
		default:
		{
			lexer->error = S_JSON_LEX_INVALID_CHAR;
			tok = S_JSON_TOK_ERROR;
			S_JSON_ERR(error, lexer, "s_json_lex_lex");
			goto lexed;
		}
		}
	}


lexed:
	/*
	 * need to append to buffer if the buffer is in use or
	 * if it's an EOF token
	 */
	if (tok == S_JSON_TOK_EOF || (lexer->buf_in_use == TRUE))
	{
		if (lexer->buf_in_use == FALSE)
		{
			s_buffer_clear(lexer->buf, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_lex",
						  "Failed to clear buffer"))
				return S_JSON_TOK_ERROR;
		}
		lexer->buf_in_use = TRUE;
		s_buffer_append(lexer->buf, text + start_ctx, *context - start_ctx,
						error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_lex_lex",
					  "Failed to append to buffer"))
			return S_JSON_TOK_ERROR;

		lexer->buf_off = 0;

		if (tok != S_JSON_TOK_EOF)
		{
			*out_buf = s_buffer_data(lexer->buf, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_lex",
						  "Failed to get buffer data"))
				return S_JSON_TOK_ERROR;

			*out_len = s_buffer_size(lexer->buf, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_lex",
						  "Failed to get buffer size"))
				return S_JSON_TOK_ERROR;

			lexer->buf_in_use = 0;
		}
	}
	else if (tok != S_JSON_TOK_ERROR )
	{
		*out_buf = text + start_ctx;
		*out_len = *context - start_ctx;
	}

	/* special case for strings. skip the quotes. */
	if (tok == S_JSON_TOK_STRING || tok == S_JSON_TOK_STRING_WITH_ESCAPES)
	{
		if ((*out_len) < 2)
		{
			S_CTX_ERR(error, S_CONTERR,
					  "s_json_lex_lex",
					  "out_len < 2");
			return S_JSON_TOK_ERROR;
		}
		(*out_buf)++;
		(*out_len) -= 2;
	}


#ifdef S_JSON_LEXER_DEBUG
	if (tok == S_JSON_TOK_ERROR)
	{
		printf("lexical error: %s\n",
		       s_json_lex_err_to_str(s_json_lex_get_err(lexer, error)));
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_lex_lex",
					  "Failed to get error string from lexer"))
			return S_JSON_TOK_ERROR;
	}
	else if (tok == S_JSON_TOK_EOF)
	{
		printf("EOF hit\n");
	}
	else
	{
		printf("lexed %s: '", s_tok_to_str(tok));
		fwrite(*out_buf, 1, *out_len, stdout);
		printf("'\n");
	}
#endif

	return tok;
}


S_LOCAL const char *s_json_lex_err_to_str(s_json_lex_error error)
{
	switch (error)
	{
	case S_JSON_LEX_E_OK:
		return ("Ok, no error");
	case S_JSON_LEX_STRING_INVALID_UTF8:
		return ("Invalid bytes in UTF8 string");
	case S_JSON_LEX_STRING_INVALID_ESCAPED_CHAR:
		return ("Inside a string, '\\' occurs before a character "
				"which it may not");
	case S_JSON_LEX_STRING_INVALID_JSON_CHAR:
		return ("Invalid character inside string");
	case S_JSON_LEX_STRING_INVALID_HEX_CHAR:
		return ("Invalid (non-hex) character occurs after '\\u' inside "
				"string");
	case S_JSON_LEX_INVALID_CHAR:
		return ("Invalid char in json text");
	case S_JSON_LEX_INVALID_STRING:
		return ("Invalid string in json text");
	case S_JSON_LEX_MISSING_INTEGER_AFTER_EXPONENT:
		return ("Malformed number, a digit is required after the exponent");
	case S_JSON_LEX_MISSING_INTEGER_AFTER_DECIMAL:
		return ("Malformed number, a digit is required after the "
				"decimal point");
	case S_JSON_LEX_MISSING_INTEGER_AFTER_MINUS:
		return ("Malformed number, a digit is required after the "
				"minus sign");
	case S_JSON_LEX_UNALLOWED_COMMENT:
		return ("Probable comment found in input text, comments are "
				"not enabled");
	}
	return ("Unknown error code");
}


/*
 * allows access to more specific information about the lexical
 * error when spct_json_lex_lex returns spct_json_tok_error.
 */
S_LOCAL s_json_lex_error s_json_lex_get_err(s_json_lexer *lexer, s_erc *error)
{
	S_CLR_ERR(error);

	if (lexer == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_get_err",
				  "Argument \"lexer\" is NULL");
		return (s_json_lex_error) -1;
	}

	return lexer->error;
}


S_LOCAL uint32 s_json_lex_current_line(s_json_lexer *lexer, s_erc *error)
{
	S_CLR_ERR(error);

	if (lexer == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_current_line",
				  "Argument \"lexer\" is NULL");
		return 0;
	}

	return lexer->line_off;
}


S_LOCAL uint32 s_json_lex_current_char(s_json_lexer *lexer, s_erc *error)
{
	S_CLR_ERR(error);

	if (lexer == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_current_char",
				  "Argument \"lexer\" is NULL");
		return 0;
	}

	return lexer->char_off;
}


S_LOCAL s_json_tok s_json_lex_peek(s_json_lexer *lexer, const uchar *text,
								   size_t text_len, size_t offset, s_erc *error)
{
	const uchar *out_buf;
	size_t out_len;
	size_t buf_len;
	size_t buf_off;
	size_t buf_in_use;
	s_json_tok tok;


	S_CLR_ERR(error);

	if (lexer == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_peek",
				  "Argument \"lexer\" is NULL");
		return S_JSON_TOK_ERROR;
	}

	buf_len = s_buffer_size(lexer->buf, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_lex_peek",
				  "Failed to get buffer size"))
		return S_JSON_TOK_ERROR;

	buf_off = lexer->buf_off;
	buf_in_use = lexer->buf_in_use;

	tok = s_json_lex_lex(lexer, text, text_len, &offset,
						 &out_buf, &out_len, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_lex_peek",
				  "Failed to get token from lexer"))
		return S_JSON_TOK_ERROR;

	lexer->buf_off = buf_off;
	lexer->buf_in_use = buf_in_use;

	s_buffer_truncate(lexer->buf, buf_len, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_lex_peek",
				  "Failed to truncate buffer"))
		return S_JSON_TOK_ERROR;

	return tok;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

#ifdef S_JSON_LEXER_DEBUG
static const char *s_tok_to_str(s_json_tok tok)
{
	switch (tok)
	{
	case S_JSON_TOK_BOOL: return "bool";
	case S_JSON_TOK_COLON: return "colon";
	case S_JSON_TOK_COMMA: return "comma";
	case S_JSON_TOK_EOF: return "eof";
	case S_JSON_TOK_ERROR: return "error";
	case S_JSON_TOK_LEFT_BRACE: return "brace";
	case S_JSON_TOK_LEFT_BRACKET: return "bracket";
	case S_JSON_TOK_NULL: return "null";
	case S_JSON_TOK_INTEGER: return "integer";
	case S_JSON_TOK_DOUBLE: return "double";
	case S_JSON_TOK_RIGHT_BRACE: return "brace";
	case S_JSON_TOK_RIGHT_BRACKET: return "bracket";
	case S_JSON_TOK_STRING: return "string";
	case S_JSON_TOK_STRING_WITH_ESCAPES: return "string_with_escapes";
	case S_JSON_TOK_COMMENT: return "comment";
	}
	return "unknown";
}
#endif


static uchar s_read_char(s_json_lexer *lxr, const uchar *txt,
						 size_t *off, s_erc *error)
{
	S_CLR_ERR(error);

	if (lxr == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_read_char",
				  "Argument \"lxr\" is NULL");
		return 'e';
	}

	if (txt == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_read_char",
				  "Argument \"txt\" is NULL");
		return 'e';
	}

	if (off == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_read_char",
				  "Argument \"off\" is NULL");
		return 'e';
	}

	if ((lxr->buf_in_use == TRUE)
	    && (s_buffer_size(lxr->buf, error) > 0)
	    && (lxr->buf_off < s_buffer_size(lxr->buf, error))
	    && (S_CHK_ERR(error, S_CONTERR,
					  "s_read_char", "Failed to get buffer size") == FALSE))
	{
		return (*((uchar*)s_buffer_data(lxr->buf, error) + (lxr->buf_off)++));
	}
	return txt[(*off)++];
}


static void s_unread_char(s_json_lexer *lxr, size_t *off, s_erc *error)
{
	S_CLR_ERR(error);

	if (lxr == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_unread_char",
				  "Argument \"lxr\" is NULL");
		return;
	}

	if (off == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_unread_char",
				  "Argument \"off\" is NULL");
		return;
	}

	if (*off > 0)
		(*off)--;
	else (lxr->buf_off)--;
}


/*
 * a lookup table which lets us quickly determine three things:
 * VEC - valid escaped conrol char
 * IJC - invalid json char
 * VHC - valid hex char
 * note.  the solidus '/' may be escaped or not.
 */
#define VEC 1
#define IJC 2
#define VHC 4
static const char s_char_lookup_table[256] =
{
	/*00*/ IJC    , IJC    , IJC    , IJC    , IJC    , IJC    , IJC    , IJC    ,
	/*08*/ IJC    , IJC    , IJC    , IJC    , IJC    , IJC    , IJC    , IJC    ,
	/*10*/ IJC    , IJC    , IJC    , IJC    , IJC    , IJC    , IJC    , IJC    ,
	/*18*/ IJC    , IJC    , IJC    , IJC    , IJC    , IJC    , IJC    , IJC    ,

	/*20*/ 0      , 0      , VEC|IJC, 0      , 0      , 0      , 0      , 0      ,
	/*28*/ 0      , 0      , 0      , 0      , 0      , 0      , 0      , VEC    ,
	/*30*/ VHC    , VHC    , VHC    , VHC    , VHC    , VHC    , VHC    , VHC    ,
	/*38*/ VHC    , VHC    , 0      , 0      , 0      , 0      , 0      , 0      ,

	/*40*/ 0      , VHC    , VHC    , VHC    , VHC    , VHC    , VHC    , 0      ,
	/*48*/ 0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	/*50*/ 0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	/*58*/ 0      , 0      , 0      , 0      , VEC|IJC, 0      , 0      , 0      ,

	/*60*/ 0      , VHC    , VEC|VHC, VHC    , VHC    , VHC    , VEC|VHC, 0      ,
	/*68*/ 0      , 0      , 0      , 0      , 0      , 0      , VEC    , 0      ,
	/*70*/ 0      , 0      , VEC    , 0      , VEC    , 0      , 0      , 0      ,
	/*78*/ 0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,

	/* include these so we don't have to always check the range of the char */
	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,

	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,

	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,

	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0      ,
	0      , 0      , 0      , 0      , 0      , 0      , 0      , 0
};


/*
 * process a variable length utf8 encoded codepoint.
 *
 *  returns:
 *    S_JSON_TOK_STRING - if valid utf8 char was parsed and offset was
 *                        advanced
 *    S_JSON_TOK_EOF - if end of input was hit before validation could
 *                     complete
 *    S_JSON_TOK_ERROR - if invalid utf8 was encountered
 *
 *  NOTE: on error the offset will point to the first char of the
 *  invalid utf8
 */
#define UTF8_CHECK_EOF if (*offset >= text_len) { return S_JSON_TOK_EOF; }

static s_json_tok s_json_lex_utf8_char(s_json_lexer *lexer, const uchar *text,
									   size_t text_len, size_t *offset,
									   uchar cur_char, s_erc *error)
{
	S_CLR_ERR(error);

	if (lexer == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_utf8_char",
				  "Argument \"lxr\" is NULL");
		return S_JSON_TOK_ERROR;
	}

	if (text == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_utf8_char",
				  "Argument \"text\" is NULL");
		return S_JSON_TOK_ERROR;
	}

	if (offset == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_utf8_char",
				  "Argument \"offset\" is NULL");
		return S_JSON_TOK_ERROR;
	}

	if (cur_char <= 0x7f)
	{
		/* single byte */
		return S_JSON_TOK_STRING;
	}
	else if ((cur_char >> 5) == 0x6)
	{
		/* two byte */
		UTF8_CHECK_EOF;
		cur_char = s_read_char(lexer, text, offset, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_lex_utf8_char",
					  "Failed to read char"))
			return S_JSON_TOK_ERROR;

		if ((cur_char >> 6) == 0x2)
			return S_JSON_TOK_STRING;
	}
	else if ((cur_char >> 4) == 0x0e)
	{
		/* three byte */
		UTF8_CHECK_EOF;
		cur_char = s_read_char(lexer, text, offset, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_lex_utf8_char",
					  "Failed to read char"))
			return S_JSON_TOK_ERROR;

		if ((cur_char >> 6) == 0x2)
		{
			UTF8_CHECK_EOF;
			cur_char = s_read_char(lexer, text, offset, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_utf8_char",
						  "Failed to read char"))
				return S_JSON_TOK_ERROR;

			if ((cur_char >> 6) == 0x2)
				return S_JSON_TOK_STRING;
		}
	}
	else if ((cur_char >> 3) == 0x1e)
	{
		/* four byte */
		UTF8_CHECK_EOF;
		cur_char = s_read_char(lexer, text, offset, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_lex_utf8_char",
					  "Failed to read char"))
			return S_JSON_TOK_ERROR;

		if ((cur_char >> 6) == 0x2)
		{
			UTF8_CHECK_EOF;
			cur_char = s_read_char(lexer, text, offset, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_utf8_char",
						  "Failed to read char"))
				return S_JSON_TOK_ERROR;

			if ((cur_char >> 6) == 0x2)
			{
				UTF8_CHECK_EOF;
				cur_char = s_read_char(lexer, text, offset, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_lex_utf8_char",
							  "Failed to read char"))
					return S_JSON_TOK_ERROR;

				if ((cur_char >> 6) == 0x2)
					return S_JSON_TOK_STRING;
			}
		}
	}

	return S_JSON_TOK_ERROR;
}


/*
 * lex a string.  input is the lexer, pointer to beginning of
 * json text, and start of string (offset).
 * a token is returned which has the following meanings:
 * S_JSON_TOK_STRING: lex of string was successful.  offset points to
 *                    terminating '"'.
 * S_JSON_TOK_EOF: end of text was encountered before we could complete
 *                 the lex.
 * S_JSON_TOK_ERROR: embedded in the string were unallowable chars.  offset
 *                   points to the offending char
 */
#define STR_CHECK_EOF							\
	if (*offset >= text_len)					\
	{											\
		tok = S_JSON_TOK_EOF;					\
		goto finish_string_lex;					\
	}

static s_json_tok s_json_lex_string(s_json_lexer *lexer, const uchar *text,
									size_t text_len, size_t *offset, s_erc *error)
{
	s_json_tok tok = S_JSON_TOK_ERROR;
	int has_escapes = 0;


	S_CLR_ERR(error);

	if (lexer == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_string",
				  "Argument \"lexer\" is NULL");
		return S_JSON_TOK_ERROR;
	}

	if (text == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_string",
				  "Argument \"text\" is NULL");
		return S_JSON_TOK_ERROR;
 	}

	if (offset == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_string",
				  "Argument \"offset\" is NULL");
		return S_JSON_TOK_ERROR;
	}

	for (/*void */; /*void */; /*void */)
	{
		uchar cur_char;

		STR_CHECK_EOF;

		cur_char = s_read_char(lexer, text, offset, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_lex_string",
					  "Failed to read char"))
			return S_JSON_TOK_ERROR;

		/* quote terminates */
		if (cur_char == '"')
		{
			tok = S_JSON_TOK_STRING;
			break;
		}
		/* backslash escapes a set of control chars, */
		else if (cur_char == '\\')
		{
			has_escapes = 1;
			STR_CHECK_EOF;

			/* special case \u */
			cur_char = s_read_char(lexer, text, offset, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_string",
						  "Failed to read char"))
				return S_JSON_TOK_ERROR;

			if (cur_char == 'u')
			{
				uint i = 0;

				for (i = 0; i < 4;i++)
				{
					STR_CHECK_EOF;
					cur_char = s_read_char(lexer, text, offset, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_json_lex_string",
								  "Failed to read char"))
						return S_JSON_TOK_ERROR;

					if (!(s_char_lookup_table[cur_char] & VHC))
					{
						/* back up to offending char */
						s_unread_char(lexer, offset, error);
						if (S_CHK_ERR(error, S_CONTERR,
									  "s_json_lex_string",
									  "Failed to read char"))
							return S_JSON_TOK_ERROR;

						lexer->error = S_JSON_LEX_STRING_INVALID_HEX_CHAR;
						S_JSON_ERR(error, lexer, "s_json_lex_string");
						goto finish_string_lex;
					}
				}
			}
			else if (!(s_char_lookup_table[cur_char] & VEC))
			{
				/* back up to offending char */
				s_unread_char(lexer, offset, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_lex_string",
							  "Failed to unread char"))
					return S_JSON_TOK_ERROR;

				lexer->error = S_JSON_LEX_STRING_INVALID_ESCAPED_CHAR;
				S_JSON_ERR(error, lexer, "s_json_lex_string");
				goto finish_string_lex;
			}
		}
		/* when not validating UTF8 it's a simple table lookup to determine
		 * if the present character is invalid */
		else if(s_char_lookup_table[cur_char] & IJC)
		{
			/* back up to offending char */
			s_unread_char(lexer, offset, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_string",
						  "Failed to unread char"))
				return S_JSON_TOK_ERROR;

			lexer->error = S_JSON_LEX_STRING_INVALID_JSON_CHAR;
			S_JSON_ERR(error, lexer, "s_json_lex_string");
			goto finish_string_lex;
		}
		/* when in validate UTF8 mode we need to do some extra work */
		else if (lexer->validate_utf8)
		{
			s_json_tok t = s_json_lex_utf8_char(lexer, text, text_len,
												offset, cur_char, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_string",
						  "Failed to get utf8 char"))
				return S_JSON_TOK_ERROR;

			if (t == S_JSON_TOK_EOF)
			{
				tok = S_JSON_TOK_EOF;
				goto finish_string_lex;
			}
			else if (t == S_JSON_TOK_ERROR)
			{
				lexer->error = S_JSON_LEX_STRING_INVALID_UTF8;
				S_JSON_ERR(error, lexer, "s_json_lex_string");
				goto finish_string_lex;
			}
		}
		/* accept it, and move on */
	}
finish_string_lex:
	/* tell our buddy, the parser, wether he needs to process this string
	 * again */
	if (has_escapes && tok == S_JSON_TOK_STRING)
	{
		tok = S_JSON_TOK_STRING_WITH_ESCAPES;
	}

	return tok;
}


#define RETURN_IF_EOF if (*offset >= text_len) return S_JSON_TOK_EOF;

static s_json_tok s_json_lex_number(s_json_lexer *lexer, const uchar *text,
									size_t text_len, size_t *offset, s_erc *error)
{
	/*
	 * XXX: numbers are the only entities in json that we must lex
	 *       _beyond_ in order to know that they are complete.  There
	 *       is an ambiguous case for integers at EOF.
	 */
	uchar c;
	s_json_tok tok = S_JSON_TOK_INTEGER;


	S_CLR_ERR(error);

	if (lexer == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_number",
				  "Argument \"lexer\" is NULL");
		return S_JSON_TOK_ERROR;
	}

	if (text == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_number",
				  "Argument \"text\" is NULL");
		return S_JSON_TOK_ERROR;
 	}

	if (offset == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_number",
				  "Argument \"offset\" is NULL");
		return S_JSON_TOK_ERROR;
	}

	RETURN_IF_EOF;
	c = s_read_char(lexer, text, offset, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_lex_number",
				  "Failed to read char"))
		return S_JSON_TOK_ERROR;

	/* optional leading minus */
	if (c == '-')
	{
		RETURN_IF_EOF;
		c = s_read_char(lexer, text, offset, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_lex_number",
					  "Failed to read char"))
			return S_JSON_TOK_ERROR;
	}

	/* a single zero, or a series of integers */
	if (c == '0')
	{
		RETURN_IF_EOF;
		c = s_read_char(lexer, text, offset, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_lex_number",
					  "Failed to read char"))
			return S_JSON_TOK_ERROR;
	}
	else if (c >= '1' && c <= '9')
	{
		do
		{
			RETURN_IF_EOF;
			c = s_read_char(lexer, text, offset, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_number",
						  "Failed to read char"))
				return S_JSON_TOK_ERROR;
		} while (c >= '0' && c <= '9');
	}
	else
	{
		s_unread_char(lexer, offset, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_lex_number",
					  "Failed to unread char"))
			return S_JSON_TOK_ERROR;
		lexer->error = S_JSON_LEX_MISSING_INTEGER_AFTER_MINUS;
		S_JSON_ERR(error, lexer, "s_json_lex_number");
		return S_JSON_TOK_ERROR;
	}

	/* optional fraction (indicates this is floating point) */
	if (c == '.')
	{
		int num_rd = 0;

		RETURN_IF_EOF;
		c = s_read_char(lexer, text, offset, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_lex_number",
					  "Failed to read char"))
			return S_JSON_TOK_ERROR;

		while (c >= '0' && c <= '9')
		{
			num_rd++;
			RETURN_IF_EOF;
			c = s_read_char(lexer, text, offset, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_number",
						  "Failed to read char"))
				return S_JSON_TOK_ERROR;
		}

		if (!num_rd)
		{
			s_unread_char(lexer, offset, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_number",
						  "Failed to unread char"))
				return S_JSON_TOK_ERROR;

			lexer->error = S_JSON_LEX_MISSING_INTEGER_AFTER_DECIMAL;
			S_JSON_ERR(error, lexer, "s_json_lex_number");
			return S_JSON_TOK_ERROR;
		}
		tok = S_JSON_TOK_DOUBLE;
	}

	/* optional exponent (indicates this is floating point) */
	if (c == 'e' || c == 'E')
	{
		RETURN_IF_EOF;
		c = s_read_char(lexer, text, offset, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_json_lex_number",
					  "Failed to read char"))
			return S_JSON_TOK_ERROR;

		/* optional sign */
		if (c == '+' || c == '-')
		{
			RETURN_IF_EOF;
			c = s_read_char(lexer, text, offset, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_number",
						  "Failed to read char"))
				return S_JSON_TOK_ERROR;
		}

		if (c >= '0' && c <= '9')
		{
			do
			{
				RETURN_IF_EOF;
				c = s_read_char(lexer, text, offset, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_lex_number",
							  "Failed to read char"))
					return S_JSON_TOK_ERROR;
			} while (c >= '0' && c <= '9');
		}
		else
		{
			s_unread_char(lexer, offset, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_number",
						  "Failed to unread char"))
				return S_JSON_TOK_ERROR;

			lexer->error = S_JSON_LEX_MISSING_INTEGER_AFTER_EXPONENT;
			S_JSON_ERR(error, lexer, "s_json_lex_number");
			return S_JSON_TOK_ERROR;
		}
		tok = S_JSON_TOK_DOUBLE;
	}

	/* we always go "one too far" */
	s_unread_char(lexer, offset, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_lex_number",
				  "Failed to unread char"))
		return S_JSON_TOK_ERROR;

	return tok;
}


static s_json_tok s_json_lex_comment(s_json_lexer *lexer, const uchar *text,
									 size_t text_len, size_t *offset, s_erc *error)
{
	uchar c;
	s_json_tok tok = S_JSON_TOK_COMMENT;


	S_CLR_ERR(error);

	if (lexer == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_comment",
				  "Argument \"lexer\" is NULL");
		return S_JSON_TOK_ERROR;
	}

	if (text == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_comment",
				  "Argument \"text\" is NULL");
		return S_JSON_TOK_ERROR;
 	}

	if (offset == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_json_lex_comment",
				  "Argument \"offset\" is NULL");
		return S_JSON_TOK_ERROR;
	}


	RETURN_IF_EOF;
	c = s_read_char(lexer, text, offset, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_lex_comment",
				  "Failed to read char"))
		return S_JSON_TOK_ERROR;

	/* either slash or star expected */
	if (c == '/')
	{
		/* now we throw away until end of line */
		do
		{
			RETURN_IF_EOF;
			c = s_read_char(lexer, text, offset, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_comment",
						  "Failed to read char"))
				return S_JSON_TOK_ERROR;
		} while (c != '\n');
	}
	else if (c == '*')
	{
		/* now we throw away until end of comment */
		for (;;)
		{
			RETURN_IF_EOF;
			c = s_read_char(lexer, text, offset, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_json_lex_comment",
						  "Failed to read char"))
				return S_JSON_TOK_ERROR;

			if (c == '*')
			{
				RETURN_IF_EOF;
				c = s_read_char(lexer, text, offset, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_json_lex_comment",
							  "Failed to read char"))
					return S_JSON_TOK_ERROR;

				if (c == '/')
				{
					break;
				}
				else
				{
					s_unread_char(lexer, offset, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_json_lex_comment",
								  "Failed to unread char"))
						return S_JSON_TOK_ERROR;
				}
			}
		}
	}
	else
	{
		lexer->error = S_JSON_LEX_INVALID_CHAR;
		tok = S_JSON_TOK_ERROR;
		S_JSON_ERR(error, lexer, "s_json_lex_comment");
	}

	return tok;
}

