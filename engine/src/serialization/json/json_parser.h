/************************************************************************************/
/* Copyright (c) 2008-2011 The Department of Arts and Culture,                      */
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

#ifndef _SPCT_JSON_PARSER_H__
#define _SPCT_JSON_PARSER_H__


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
 * @file json_parser.h
 * JSON data source parser class.
 */


/**
 * @ingroup SSerializedFile
 * @defgroup SJSON JavaScript Object Notation (JSON)
 * JSON data source parser.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"
#include "base/utils/types.h"
#include "base/containers/buffer/buffer.h"
#include "datasources/data_reader.h"
#include "serialization/json/json_lex.h"


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

/**
 * Structure of callback functions for reading different data types.
 */
typedef struct
{
	/**
	 * Callback for "null" object.
	 * @param ctx The context container (where the read object is stored).
	 * @param error Error code.
	 */
	void (*s_json_null)       (void *ctx, s_erc *error);

	/**
	 * Callback for boolean values.
	 * @param ctx The context container (where the read object is stored).
	 * @param bool_val The read boolean value.
	 * @param error Error code.
	 */
	void (*s_json_boolean)    (void *ctx, uint8 bool_val, s_erc *error);

	/**
	 * Callback for integer values.
	 * @param ctx The context container (where the read object is stored).
	 * @param integer_val The read integer value.
	 * @param error Error code.
	 */
	void (*s_json_integer)    (void *ctx, sint32 integer_val, s_erc *error);

	/**
	 * Callback for double values.
	 * @param ctx The context container (where the read object is stored).
	 * @param double_val The read double value.
	 * @param error Error code.
	 */
	void (*s_json_double)     (void *ctx, double double_val, s_erc *error);

	/**
	 * A callback which passes the string representation of the number
	 * back to the client. Will be used for all numbers when present.
	 * @param ctx The context container (where the read object is stored).
	 * @param number_val The read number value as a string.
	 * @param number_len Length of the read string.
	 * @param error Error code.
	 */
	void (*s_json_number)     (void *ctx, const char *number_val,
							   uint number_len, s_erc *error);

	/**
	 * Strings are returned as pointers into the JSON text when,
	 * possible, as a result, they are _not_ null padded
	 * @param ctx The context container (where the read object is stored).
	 * @param string_val The read string.
	 * @param string_len Length of the read string.
	 * @param error Error code.
	 */
	void (*s_json_string)     (void *ctx, const uchar *string_val,
							   uint string_len, s_erc *error);

	/**
	 * A callback for the start of a key-value map container.
	 * @param ctx The context container (where the read object is stored).
	 * @param error Error code.
	 */
	void (*s_json_start_map)  (void *ctx, s_erc *error);

	/**
	 * A callback for a key of a key-value map container.
	 * @param ctx The context container (where the read object is stored).
	 * @param key The read string.
	 * @param string_len Length of the read string.
	 * @param error Error code.
	 */
	void (*s_json_map_key)    (void *ctx, const uchar *key,
							   uint string_len, s_erc *error);

	/**
	 * A callback for the end of a key-value map container.
	 * @param ctx The context container (where the read object is stored).
	 * @param error Error code.
	 */
	void (*s_json_end_map)    (void *ctx, s_erc *error);

	/**
	 * A callback for the start of a list container.
	 * @param ctx The context container (where the read object is stored).
	 * @param error Error code.
	 */
	void (*s_json_start_array)(void *ctx, s_erc *error);

	/**
	 * A callback for the end of a list container.
	 * @param ctx The context container (where the read object is stored).
	 * @param error Error code.
	 */
	void (*s_json_end_array)  (void *ctx, s_erc *error);
} s_json_callbacks;


/************************************************************************************/
/*                                                                                  */
/* SJSONParser definition                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * The json data source parser structure.
 * @extends SSerializedData
 */
typedef struct
{
	/**
	 * @protected Inherit from #SDataReader.
	 */
	SDataReader             obj;

    /**
	 * @protected Callback functions for different data types.
	 */
	const s_json_callbacks *callbacks;

	/**
	 * @protected Context pointer, where read data is stored.
	 */
	void                   *ctx;

	/**
	 * @protected Lexical parser.
	 */
	s_json_lexer           *lexer;

	/**
	 * @protected Parser error string.
	 */
	const char             *parse_error;

	/**
	 * @protected  Parser error offset.
	 */
	uint                    error_offset;

	/**
	 * @protected Temporary storage for decoded strings.
	 */
	s_buffer               *decode_buf;

	/**
	 * @protected A stack of states.
	 */
	s_buffer               *state_buf;

	/**
	 * @protected Flag for comments in files.
	 */
	s_bool                  comments;

	/**
	 * @protected Flag to check UTF8 validity.
	 */
	s_bool                  check_utf8;

	/**
	 * @protected Error string.
	 */
	const char             *error;

	/**
	 * @protected Locking mutex.
	 */
	S_DECLARE_MUTEX(json_mutex);
} SJSONParser;


/************************************************************************************/
/*                                                                                  */
/* SJSONParser class definition                                                     */
/*                                                                                  */
/************************************************************************************/

/**
 * The json data source parserer class structure.
 * @extends SSerializedDataClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SDataReaderClass.
	 */
	SDataReaderClass  _inherit;

	/* Class methods */
	/**
	 * @protected Parse function pointer.
	 * Execute the parser on the given parser structure.
	 * @param self The given parser object.
	 * @param error Error code.
	 */
	void (*parse)(SJSONParser *self, s_erc *error);
} SJSONParserClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize a JSON Parser.
 * @public @memberof SJSONParser
 *
 * @param self The JSON parser to initialize.
 * @param ds The data source to read from.
 * @param allow_comments #TRUE if comments are allowed in JSON files.
 * @param check_utf8 #TRUE if utf8 strings in JSON files should be validated.
 * @param callbacks Structure with callback functions for different data types.
 * @param ctx Pointer to object where read data structure is saved by callback functions.
 * @param error Error code.
 *
 * @note If this function fails the parser will be deleted and the @c
 * self pointer will be set to @c NULL.
 */
S_API void SJSONParserInit(SJSONParser **self, SDatasource *ds,
						   s_bool allow_comments, s_bool check_utf8,
						   const s_json_callbacks *callbacks, void *ctx, s_erc *error);


/**
 * Run the given JSON parser.
 * @public @memberof SJSONParser
 * @param self The JSON parser to run.
 * @param error Error code.
 */
S_API void SJSONParserParse(SJSONParser *self, s_erc *error);


/**
 * Get the error string of the JSON parser.
 *
 * @public @memberof SJSONParser
 * @param self The JSON parser from which to get the error.
 * @param error Error code.
 *
 * @return Pointer to error string.
 */
S_API const char *SJSONParseGetError(SJSONParser *self, s_erc *error);


/**
 * Add the SJSONParser class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_json_parser_class_add(s_erc *error);


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

#endif /* _SPCT_JSON_PARSER_H__ */

