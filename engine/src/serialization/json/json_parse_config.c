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
/* JSON config file parser.                                                         */
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

#include <string.h>
#include "base/utils/alloc.h"
#include "base/objsystem/objsystem.h"
#include "containers/containers.h"
#include "datasources/file_source.h"
#include "serialization/json/json_parser.h"
#include "serialization/json/json_parse_config.h"


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

typedef struct
{
	SList   *containers;
	SList   *keys;
	SObject *final;
} s_json_context;


/************************************************************************************/
/*                                                                                  */
/*  Static funtion prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void add_value_to_container(s_json_context *context, SObject *value, s_erc *error);

/* JSON callbacks */
static void s_json_callback_null(void *ctx, s_erc *error);

static void s_json_callback_boolean(void * ctx, uint8 bool_val, s_erc *error);

static void s_json_callback_integer(void *ctx, sint32 integer_val, s_erc *error);

static void s_json_callback_double(void *ctx, double double_val, s_erc *error);

static void s_json_callback_string(void *ctx, const uchar *string_val,
								   uint string_len, s_erc *error);

static void s_json_callback_map_key(void *ctx, const uchar *string_val,
									uint string_len, s_erc *error);

static void s_json_callback_start_map(void *ctx, s_erc *error);

static void s_json_callback_end_map(void *ctx, s_erc *error);

static void s_json_callback_start_array(void *ctx, s_erc *error);

static void s_json_callback_end_array(void *ctx, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/*  Static variables                                                                */
/*                                                                                  */
/************************************************************************************/

static s_json_callbacks callbacks;


/************************************************************************************/
/*                                                                                  */
/*  Function implementations                                                        */
/*                                                                                  */
/************************************************************************************/

S_API SMap *s_json_parse_config_file(const char *path, s_erc *error)
{
	SDatasource *ds;
	s_json_context *context;
	SJSONParser *JSON;
	SMap *retVal;


	S_CLR_ERR(error);
	ds = SFilesourceOpenFile(path, "r", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_parse_config_file",
				  "Failed to could not open data source file \"%s\" for reading", path))
		return NULL;


	context = S_MALLOC(s_json_context, 1);
	if (context == NULL)
	{
		S_DELETE(ds, "s_json_parse_config_file", error);
		S_FTL_ERR(error, S_MEMERROR,
				  "s_json_parse_config_file",
				  "Failed to allocate memory for context structure");
		return NULL;
	}

	context->containers = (SList*)S_NEW("SListList", error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "s_json_parse_config_file",
				  "Failed to create 'SListList' object"))
	{
		S_DELETE(ds, "s_json_parse_config_file", error);
		S_FREE(context);
		return NULL;
	}

	context->keys = (SList*)S_NEW("SListList", error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "s_json_parse_config_file",
				  "Failed to create 'SListList' object"))
	{
		S_DELETE(ds, "s_json_parse_config_file", error);
		S_DELETE(context->containers, "s_json_parse_config_file", error);
		S_FREE(context);
		return NULL;
	}

	context->final = NULL;

	JSON = (SJSONParser*)S_NEW("SJSONParser", error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "s_json_parse_config_file",
				  "Failed to create json object"))
	{
		S_DELETE(ds, "s_json_parse_config_file", error);
		S_DELETE(context->containers, "s_json_parse_config_file", error);
		S_DELETE(context->keys, "s_json_parse_config_file", error);
		S_FREE(context);
		return NULL;
	}

	SJSONParserInit(&JSON, ds, TRUE, TRUE, &callbacks, (void*)context, error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "s_json_parse_config_file",
				  "Failed to initialize json object"))
	{
		S_DELETE(ds, "s_json_parse_config_file", error);
		S_DELETE(context->containers, "s_json_parse_config_file", error);
		S_DELETE(context->keys, "s_json_parse_config_file", error);
		S_FREE(context);
		return NULL;
	}

	SJSONParserParse(JSON, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_parse_config_file",
				  "Failed to parse JSON file, \"%s\"", path))
	{
		S_DELETE(context->containers, "s_json_parse_config_file", error);
		S_DELETE(context->keys, "s_json_parse_config_file", error);
		S_DELETE(JSON, "s_json_parse_config_file", error);

		if (context->final != NULL)
			S_DELETE(context->final, "s_json_parse_config_file", error);

		S_FREE(context);
		return NULL;
	}


	S_DELETE(JSON, "s_json_parse_config_file", error);

	retVal = S_CAST(context->final, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR, "s_json_parse_config_file",
				  "Failed to cast return value to SMap"))
		retVal = NULL;

	S_DELETE(context->containers, "s_json_parse_config_file", error);
	S_DELETE(context->keys, "s_json_parse_config_file", error);
	S_FREE(context);

	return retVal;
}


/************************************************************************************/
/*                                                                                  */
/*  Static function implementations                                                 */
/*                                                                                  */
/************************************************************************************/

static void add_value_to_container(s_json_context *context, SObject *value, s_erc *error)
{
	SObject *conVal;
	SObject *key;
	SMap *myMap;
	SList *myArray;
	size_t size;
	s_bool is_type;


	S_CLR_ERR(error);

	/* test for last */
	size = SListSize(context->containers, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_value_to_container",
				  "Call to SListSize failed"))
		return;

	if (size == 0)
	{
		/* at end */
		context->final = value;
		return;
	}

	conVal = SListPop(context->containers, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_value_to_container",
				  "Call to SListPop failed"))
	{
		S_DELETE(conVal, "add_value_to_container", error);
		return;
	}

	is_type = SObjectIsType(conVal, "SMap", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_value_to_container",
				  "Call to SObjectIsType failed"))
	{
		S_DELETE(conVal, "add_value_to_container", error);
		return;
	}

	if (is_type == TRUE)
	{
		key = SListPop(context->keys, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_value_to_container",
					  "Call to SListPop failed"))
		{
			S_DELETE(conVal, "add_value_to_container", error);
			S_DELETE(key, "add_value_to_container", error);
			return;
		}

		myMap = S_CAST(conVal, SMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_value_to_container",
					  "Cast failed"))
		{
			S_DELETE(conVal, "add_value_to_container", error);
			S_DELETE(key, "add_value_to_container", error);
			return;
		}

		SMapSetObject(myMap, SObjectGetString(key, error), value, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_value_to_container",
					  "Call to SMapValSet failed"))
		{
			S_DELETE(conVal, "add_value_to_container", error);
			S_DELETE(key, "add_value_to_container", error);
			return;
		}

		SListPush(context->containers, conVal, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_value_to_container",
					  "Call to SListPush failed"))
		{
			S_DELETE(conVal, "add_value_to_container", error);
			S_DELETE(key, "add_value_to_container", error);
			return;
		}

		S_DELETE(key, "add_value_to_container", error);
		return;
	}

	is_type = SObjectIsType(conVal, "SList", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_value_to_container",
				  "Call to SObjectIsType failed"))
	{
		S_DELETE(conVal, "add_value_to_container", error);
		return;
	}

	if (is_type == TRUE)
	{
		myArray = S_CAST(conVal, SList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_value_to_container",
					  "Cast failed"))
		{
			S_DELETE(conVal, "add_value_to_container", error);
			return;
		}

		SListAppend(myArray, value, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_value_to_container",
					  "Call to SListAppend failed"))
		{
			S_DELETE(conVal, "add_value_to_container", error);
			return;
		}

		SListPush(context->containers, conVal, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_value_to_container",
					  "Call to SListPush failed"))
		{
			S_DELETE(conVal, "add_value_to_container", error);
			return;
		}
	}
}


/*** SJSONParser callbacks ***/

static void s_json_callback_null(void *ctx, s_erc *error)
{
	ctx = NULL;
	S_CTX_ERR(error, S_FAILURE,
			  "s_json_callback_null",
			  "file contains a \"null\"");
}


static void s_json_callback_boolean(void * ctx, uint8 bool_val, s_erc *error)
{
	SObject *tmp;
	s_json_context *context = (s_json_context*)ctx;


	S_CLR_ERR(error);

	tmp = SObjectSetInt(bool_val, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_boolean",
				  "Failed to set boolean object"))
	{
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_boolean", error);
		return;
	}

	add_value_to_container(context, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_boolean",
				  "Failed to add value to container"))
	{
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_boolean", error);
		return;
	}
}


static void s_json_callback_integer(void *ctx, sint32 integer_val, s_erc *error)
{
	SObject *tmp;
	s_json_context *context = (s_json_context*)ctx;


	S_CLR_ERR(error);

	tmp = SObjectSetInt(integer_val, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_integer",
				  "Failed to set integer object"))
	{
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_integer", error);
		return;
	}

	add_value_to_container(context, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_integer",
				  "Failed to add value to container"))
	{
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_integer", error);
		return;
	}
}


static void s_json_callback_double(void *ctx, double double_val, s_erc *error)
{
	SObject *tmp;
	s_json_context *context = (s_json_context*)ctx;


	S_CLR_ERR(error);

	tmp = SObjectSetFloat((float)double_val, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_double",
				  "Failed to set double object"))
	{
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_double", error);
		return;
	}

	add_value_to_container(context, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_double",
				  "Failed to add value to container"))
	{
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_double", error);
		return;
	}
}


static void s_json_callback_string(void *ctx, const uchar *string_val,
								   uint string_len, s_erc *error)
{
	SObject *tmp;
	s_json_context *context = (s_json_context*)ctx;
	char *str;


	S_CLR_ERR(error);
	str = S_MALLOC(char, string_len + 1);
	if (str == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_json_callback_string",
				  "Failed to allocate memory for string");
		return;
	}

 	str[string_len] = 0;
 	memcpy(str, string_val, string_len);

	tmp = SObjectSetString((const char*)str, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_string",
				  "Failed to set string object"))
	{
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_string", error);
		S_FREE(str);
		return;
	}

	add_value_to_container(context, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_string",
				  "Failed to add value to container"))
	{
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_string", error);
		S_FREE(str);
		return;
	}

	S_FREE(str);
}


static void s_json_callback_map_key(void *ctx, const uchar *string_val,
									uint string_len, s_erc *error)
{
	SObject *tmp;
	s_json_context *context = (s_json_context*)ctx;
	char *str;


	S_CLR_ERR(error);
	str = S_MALLOC(char, string_len + 1);
	if (str == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_json_callback_map_key",
				  "Failed to allocate memory for string");
		return;
	}

 	str[string_len] = 0;
 	memcpy(str, string_val, string_len);

	tmp = SObjectSetString((const char*)str, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_map_key",
				  "Failed to set string object"))
	{
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_map_key", error);
		S_FREE(str);
		return;
	}

	SListPush(context->keys, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_map_key",
				  "Failed to add value to container"))
	{
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_map_key", error);
		S_FREE(str);
		return;
	}

	S_FREE(str);
}


static void s_json_callback_start_map(void *ctx, s_erc *error)
{
	s_json_context *context = (s_json_context*)ctx;
	SMap *lMap;


	S_CLR_ERR(error);
	lMap = S_MAP(S_NEW("SMapList", error));
	if (S_CHK_ERR(error, S_FAILURE,
				  "s_json_callback_start_map",
				  "Failed to create map-list object"))
		return;

	SListPush(context->containers, S_OBJECT(lMap), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_start_map",
				  "Failed to add map to container"))
		S_DELETE(lMap, "s_json_callback_start_map", error);
}


static void s_json_callback_end_map(void *ctx, s_erc *error)
{
	SObject *tmp;
	s_json_context *context = (s_json_context*)ctx;


	S_CLR_ERR(error);
	tmp = SListPop(context->containers, error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "s_json_callback_end_map",
				  "Failed to pop object from list"))
	{
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_end_map", error);
		return;
	}

	add_value_to_container(context, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_end_map",
				  "Failed to add value to container"))
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_end_map", error);
}


static void s_json_callback_start_array(void *ctx, s_erc *error)
{
	s_json_context *context = (s_json_context*)ctx;
	SList *newArray;


	S_CLR_ERR(error);
	newArray = S_LIST(S_NEW("SListList", error));
	if (S_CHK_ERR(error, S_FAILURE,
				  "s_json_callback_start_array",
				  "Failed to create list-list object"))
		return;

	SListPush(context->containers, S_OBJECT(newArray), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_start_array",
				  "Failed to add list to container"))
		S_DELETE(newArray, "s_json_callback_start_array", error);
}


static void s_json_callback_end_array(void *ctx, s_erc *error)
{
	SObject *tmp;
	s_json_context *context = (s_json_context*)ctx;


	S_CLR_ERR(error);
	tmp = SListPop(context->containers, error);
	if (S_CHK_ERR(error, S_FAILURE,
				  "s_json_callback_end_array",
				  "Failed to pop object from list"))
	{
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_end_array", error);
		return;
	}

	add_value_to_container(context, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_json_callback_end_array",
				  "Failed to add value to container"))
		if (tmp != NULL)
			S_DELETE(tmp, "s_json_callback_end_array", error);
}


/************************************************************************************/
/*                                                                                  */
/*  Static variables initialization                                                 */
/*                                                                                  */
/************************************************************************************/

static s_json_callbacks callbacks =
{
	s_json_callback_null,
	s_json_callback_boolean,
	s_json_callback_integer,
	s_json_callback_double,
	NULL,
	s_json_callback_string,
	s_json_callback_start_map,
	s_json_callback_map_key,
	s_json_callback_end_map,
	s_json_callback_start_array,
	s_json_callback_end_array
};
