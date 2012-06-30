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
/* A tokenstream class implementation.                                              */
/* Loosely based on EST_Token of Edinburgh Speech Tools,                            */
/* http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96)                          */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_TOKENSTREAM_H__
#define _SPCT_TOKENSTREAM_H__


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

/**
 * @file tokenstream.h
 * A tokenstream class implementation.
 */


/**
 * @defgroup STokenstream Tokenstream
 * A tokenstream class implementation. A class that allows the reading
 * of tokens (#SToken) from a file or string.  It automatically
 * tokenizes a stream based on user definable whitespace and
 * punctuation.
 *
 * The definitions of whitespace and punctuation are user definable.
 * Also support for single character symbols is included.  Single
 * character symbols are @e always are treated as individual tokens
 * irrespective of their white space context.  Also a quote mode can
 * be used to read quoted tokens.
 *
 * The setting of whitespace, pre and post punctuation, single
 * character symbols and quote mode must be done (immediately) after
 * opening the stream.
 *
 * There is no unget but peek provides look ahead of one token.
 *
 * The default character symbols are:
 * <table>
 *  <tr>
 *   <td> White-space </td>
 *   <td> @code \t \n \r @endcode </td>
 *  </tr>
 *  <tr>
 *   <td> Single characters </td>
 *   <td> @code ( ) { } [ ] @endcode </td>
 *  </tr>
 *  <tr>
 *   <td> Pre-punctuation </td>
 *   <td> @code " ' ` ( { [ @endcode </td>
 *  </tr>
 *  <tr>
 *   <td> Post-punctuation </td>
 *   <td> @code " ' ` . , : ; ! ? ( ) { } [ ] @endcode </td>
 *  </tr>
 * </table>
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"
#include "utils/token.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given #STokenstream child/parent class object as a
 * #STokenstream object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #STokenstream* type.
 * @note This casting is not safety checked.
 */
#define S_TOKENSTREAM(SELF)    ((STokenstream *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* STokenstream definition                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * The tokenstream structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject     obj;

	/**
	 * @protected Preceding white-space symbols definition.
	 */
	char       *white_space_chars;

	/**
	 * @protected Single character symbols definition.
	 */
	char       *single_char_symbols;

	/**
	 * @protected Pre-punctuation symbols definition.
	 */
	char       *pre_punc_symbols;

	/**
	 * @protected Post-punctuation symbols definition.
	 */
	char       *post_punc_symbols;

	/**
	 * @protected Current token.
	 */
	SToken     *currentToken;

	/**
	 * @protected End-of-file flag.
	 */
	s_bool      eof;

	/**
	 * @protected Current character.
	 */
	uint32      current_char;

	/**
	 * @protected Peeked flag.
	 */
	s_bool      peeked;

	/**
	 * @protected Quote character.
	 */
	uint32      quote;

	/**
	 * @protected Escape character.
	 */
	uint32      escape;

	/**
	 * @protected Quote mode flag.
	 */
	s_bool      quote_mode;
} STokenstream;


/************************************************************************************/
/*                                                                                  */
/* STokenstreamClass definition                                                     */
/*                                                                                  */
/************************************************************************************/

/**
 * The tokenstream class structure.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SObjectClass.
	 */
	SObjectClass  _inherit;

	/* Class methods */
	/**
	 * Get character. Get the next character from the tokenstream and
	 * set it the @c current_char tokenstream member.
	 *
	 * @param self The tokenstream.
	 * @param error Error code.
	 *
	 * @return The next character from the tokenstream, as UTF8
	 * character represented as a 4-byte unsigned integer.
	 */
	uint32 (* const get_char)(STokenstream *self, s_erc *error);

	/**
	 * Seek to the given position. Seek is relative to beginning of
	 * tokenstream source.
	 *
	 * @param self The tokenstream.
	 * @param pos The position to seek to.
	 * @param error Error code.
	 */
	void (* const seek)(STokenstream *self, ulong pos, s_erc *error);

	/**
	 * Query the position in the tokenstream.
	 *
	 * @param self The tokenstream.
	 * @param error Error code.
	 *
	 * @return The position in the tokenstream.
	 */
	ulong (* const tell)(const STokenstream *self, s_erc *error);

	/**
	 * Get the next token.
	 *
	 * @param self The tokenstream.
	 * @param error Error code.
	 *
	 * @return The next token.
	 */
	const SToken *(* const get_token)(STokenstream *self, s_erc *error);

	/**
	 * Peek the next token.
	 *
	 * @param self The tokenstream.
	 * @param error Error code.
	 *
	 * @return The next token (peeked).
	 */
	const SToken *(* const peek_token)(STokenstream *self, s_erc *error);

	/**
	 * Set the tokenstream white-space characters.
	 *
	 * @param self The tokenstream.
	 * @param white_space_chars The white-space characters to set.
	 * @param error Error code.
	 */
	void (* const set_whitespace_chars)(STokenstream *self, const char *white_space_chars, s_erc *error);

	/**
	 * Set the tokenstream single character symbols.
	 *
	 * @param self The tokenstream.
	 * @param single_chars The single characters symbols to set.
	 * @param error Error code.
	 */
	void (* const set_single_chars)(STokenstream *self, const char *single_chars, s_erc *error);

	/**
	 * Set the tokenstream pre-punctuation symbols.
	 *
	 * @param self The tokenstream.
	 * @param single_chars The pre-punctuation symbols to set.
	 * @param error Error code.
	 */
	void (* const set_prepunc_chars)(STokenstream *self, const char *pre_punc_chars, s_erc *error);

	/**
	 * Set the tokenstream post-punctuation symbols.
	 *
	 * @param self The tokenstream.
	 * @param single_chars The post-punctuation symbols to set.
	 * @param error Error code.
	 */
	void (* const set_postpunc_chars)(STokenstream *self, const char *post_punc_chars, s_erc *error);

	/**
	 * Set the tokenstream quote symbols.
	 *
	 * @param self The tokenstream.
	 * @param quote The quote symbol (use #s_getc to get the character
	 * in a 4-byte unsigned integer).
	 * @param escape The escape symbol (use #s_getc to get the character
	 * in a 4-byte unsigned integer).
	 * @param error Error code.
	 */
	void (* const set_quotes)(STokenstream *self, uint32 quote, uint32 escape, s_erc *error);

	/**
	 * Query if the tokenstream is in quote mode.
	 *
	 * @param self The tokenstream.
	 * @param error Error code.
	 *
	 * @return @c TRUE if in quote mode else @c FALSE
	 */
	s_bool (* const query_quote_mode)(const STokenstream *self, s_erc *error);

	/**
	 * Query if the tokenstream is at the end of the file.
	 *
	 * @param self The tokenstream.
	 * @param error Error code.
	 *
	 * @return @c TRUE if at the end of the file else @c FALSE
	 */
	s_bool (* const query_eof)(const STokenstream *self, s_erc *error);
} STokenstreamClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Get character. Get the next character from the tokenstream and
 * set it the @c current_char tokenstream member.
 *
 * @public @memberof STokenstream
 *
 * @param self The tokenstream.
 * @param error Error code.
 *
 * @return The next character from the tokenstream, as UTF8
 * character represented as a 4-byte unsigned integer.
 */
S_API uint32 STokenstreamGetChar(STokenstream *self, s_erc *error);


/**
 * Seek to the given position. Seek is relative to beginning of
 * tokenstream source.
 *
 * @public @memberof STokenstream
 *
 * @param self The tokenstream.
 * @param pos The position to seek to.
 * @param error Error code.
 */
S_API void STokenstreamSeek(STokenstream *self, ulong pos, s_erc *error);


/**
 * Query the position in the tokenstream.
 *
 * @public @memberof STokenstream
 *
 * @param self The tokenstream.
 * @param error Error code.
 *
 * @return The position in the tokenstream.
 */
S_API ulong STokenstreamTell(const STokenstream *self, s_erc *error);


/**
 * Get the next token.
 *
 * @public @memberof STokenstream
 *
 * @param self The tokenstream.
 * @param error Error code.
 *
 * @return The next token.
 */
S_API const SToken *STokenstreamGetToken(STokenstream *self, s_erc *error);


/**
 * Peek the next token.
 *
 * @public @memberof STokenstream
 *
 * @param self The tokenstream.
 * @param error Error code.
 *
 * @return The next token (peeked).
 */
S_API const SToken *STokenstreamPeekToken(STokenstream *self, s_erc *error);


/**
 * Set the tokenstream white-space characters.
 *
 * @public @memberof STokenstream
 *
 * @param self The tokenstream.
 * @param white_space_chars The white-space characters to set.
 * @param error Error code.
 */
S_API void STokenstreamSetWhitespaceChars(STokenstream *self, const char *white_space_chars, s_erc *error);


/**
 * Set the tokenstream single character symbols.
 *
 * @public @memberof STokenstream
 *
 * @param self The tokenstream.
 * @param single_chars The single characters symbols to set.
 * @param error Error code.
 */
S_API void STokenstreamSetSingleChars(STokenstream *self, const char *single_chars, s_erc *error);


/**
 * Set the tokenstream pre-punctuation symbols.
 *
 * @public @memberof STokenstream
 *
 * @param self The tokenstream.
 * @param single_chars The pre-punctuation symbols to set.
 * @param error Error code.
 */
S_API void STokenstreamSetPrePuncChars(STokenstream *self, const char *pre_punc_chars, s_erc *error);


/**
 * Set the tokenstream post-punctuation symbols.
 *
 * @public @memberof STokenstream
 *
 * @param self The tokenstream.
 * @param single_chars The post-punctuation symbols to set.
 * @param error Error code.
 */
S_API void STokenstreamSetPostPuncChars(STokenstream *self, const char *post_punc_chars, s_erc *error);


/**
 * Set the tokenstream quote symbols.
 *
 * @public @memberof STokenstream
 *
 * @param self The tokenstream.
 * @param quote The quote symbol (use #s_getc to get the character
 * in a 4-byte unsigned integer).
 * @param escape The escape symbol (use #s_getc to get the character
 * in a 4-byte unsigned integer).
 * @param error Error code.
 */
S_API void STokenstreamSetQuotes(STokenstream *self, uint32 quote, uint32 escape, s_erc *error);


/**
 * Query if the tokenstream is in quote mode.
 *
 * @public @memberof STokenstream
 *
 * @param self The tokenstream.
 * @param error Error code.
 *
 * @return @c TRUE if in quote mode else @c FALSE
 */
S_API s_bool STokenstreamQueryQuoteMode(const STokenstream *self, s_erc *error);


/**
 * Query if the tokenstream is at the end of the file.
 *
 * @public @memberof STokenstream
 *
 * @param self The tokenstream.
 * @param error Error code.
 *
 * @return @c TRUE if at the end of the file else @c FALSE
 */
S_API s_bool STokenstreamQueryEOF(const STokenstream *self, s_erc *error);


/**
 * Add the SToken class to the object system.
 * @private @memberof SToken
 * @param error Error code.
 */
S_LOCAL void _s_tokenstream_class_add(s_erc *error);

/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


/**
 * @}
 * end documentation
 */

#endif /* _SPCT_TOKENSTREAM_H__ */
