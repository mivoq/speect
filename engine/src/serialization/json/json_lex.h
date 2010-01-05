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

#ifndef _SPCT_JSON_LEX_H__
#define _SPCT_JSON_LEX_H__


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


/**
 * @file json_lex.h
 * JSON lexical parser.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/containers/buffer/buffer.h"
#include "base/errdbg/errdbg.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/* JSON lexical tokens */
typedef enum
{
	S_JSON_TOK_BOOL,
	S_JSON_TOK_COLON,
	S_JSON_TOK_COMMA,
	S_JSON_TOK_EOF,
	S_JSON_TOK_ERROR,
	S_JSON_TOK_LEFT_BRACE,
	S_JSON_TOK_LEFT_BRACKET,
	S_JSON_TOK_NULL,
	S_JSON_TOK_RIGHT_BRACE,
	S_JSON_TOK_RIGHT_BRACKET,

	/*
	 * we differentiate between integers and doubles to allow the
	 * parser to interpret the number without re-scanning
	 */
	S_JSON_TOK_INTEGER,
	S_JSON_TOK_DOUBLE,

	/*
	 * we differentiate between strings which require further processing,
	 * and strings that do not
	 */
	S_JSON_TOK_STRING,
	S_JSON_TOK_STRING_WITH_ESCAPES,

	/* comment tokens are not currently returned to the parser, ever */
	S_JSON_TOK_COMMENT
} s_json_tok;


/* JSON lexical errors */
typedef enum
{
	S_JSON_LEX_E_OK = 0,
	S_JSON_LEX_STRING_INVALID_UTF8,
	S_JSON_LEX_STRING_INVALID_ESCAPED_CHAR,
	S_JSON_LEX_STRING_INVALID_JSON_CHAR,
	S_JSON_LEX_STRING_INVALID_HEX_CHAR,
	S_JSON_LEX_INVALID_CHAR,
	S_JSON_LEX_INVALID_STRING,
	S_JSON_LEX_MISSING_INTEGER_AFTER_DECIMAL,
	S_JSON_LEX_MISSING_INTEGER_AFTER_EXPONENT,
	S_JSON_LEX_MISSING_INTEGER_AFTER_MINUS,
	S_JSON_LEX_UNALLOWED_COMMENT
} s_json_lex_error;


/* opaque s_json_lexer type */
typedef struct s_json_lexer s_json_lexer;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/* create a new lexer */
S_LOCAL s_json_lexer *s_json_lex_new(s_bool allow_comments, s_bool validate_utf8,
				     s_erc *error);


/* delete the given lexer */
S_LOCAL void s_json_lex_delete(s_json_lexer *lxr, s_erc *error);


/*
 * run/continue a lex. "offset" is an input/output parameter.
 * It should be initialized to zero for a
 * new chunk of target text, and upon subsetquent calls with the same
 * target text should passed with the value of the previous invocation.
 *
 * the client may be interested in the value of offset when an error is
 * returned from the lexer.  This allows the client to render useful
 * error messages.
 *
 * When you pass the next chunk of data, context should be reinitialized
 * to zero.
 *
 * Finally, the output buffer is usually just a pointer into the json_text,
 * however in cases where the entity being lexed spans multiple chunks,
 * the lexer will buffer the entity and the data returned will be
 * a pointer into that buffer.
 *
 * This behavior is abstracted from client code except for the performance
 * implications which require that the client choose a reasonable chunk
 * size to get adequate performance.
 */
S_LOCAL s_json_tok s_json_lex_lex(s_json_lexer *lexer, const uchar *text,
				  size_t text_len, size_t *context,
				  const uchar **out_buf, size_t *out_len, s_erc *error);


/** have a peek at the next token, but don't move the lexer forward */
S_LOCAL s_json_tok s_json_lex_peek(s_json_lexer *lexer, const uchar *text,
				   size_t text_len, size_t offset, s_erc *error);


/* get a string representation of the lexer error */
S_LOCAL const char *s_json_lex_err_to_str(s_json_lex_error error);


/*
 * allows access to more specific information about the lexical
 * error when spct_json_lex_lex returns spct_json_tok_error.
 */
S_LOCAL s_json_lex_error s_json_lex_get_err(s_json_lexer *lexer, s_erc *error);


/* get the number of lines lexed by this lexer instance */
S_LOCAL uint32 s_json_lex_current_line(s_json_lexer *lexer, s_erc *error);


/*
 * get the number of chars lexed by this lexer instance since
 * the last \n or \r
 */
S_LOCAL uint32 s_json_lex_current_char(s_json_lexer *lexer, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


#endif /* _SPCT_JSON_LEX_H__ */

