/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* Token, tokenizer, file tokenizer and string tokenizer plug-in.                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "token.h"
#include "tokenizer.h"
#include "tokenizer_file.h"
#include "tokenizer_string.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

/* Minimum major version of Speect Engine required for plug-in */
#define SPCT_MAJOR_VERSION_MIN 1

/* Minimum minor version of Speect Engine required for plug-in */
#define SPCT_MINOR_VERSION_MIN 0


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static const char * const plugin_init_func = "Tokenizers plug-in initialization";

static const char * const plugin_reg_func = "Tokenizers plug-in register";

static const char * const plugin_exit_func = "Tokenizers plug-in free";


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void plugin_register_function(s_erc *error);

static void plugin_exit_function(s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in parameters                                                               */
/*                                                                                  */
/************************************************************************************/

static const s_plugin_params plugin_params =
{
	/* plug-in name */
	"Tokenizers",

	/* description */
	"A file and string tokenizer plug-in",

	/* version */
	{
		0,
		2
	},

	/* Speect ABI version (which plug-in was compiled with) */
	{
		S_MAJOR_VERSION,
		S_MINOR_VERSION
	},

	/* register function pointer */
	plugin_register_function,

	/* exit function pointer */
	plugin_exit_function
};


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

const s_plugin_params *s_plugin_init(s_erc *error)
{
	S_CLR_ERR(error);

	if (!s_lib_version_ok(SPCT_MAJOR_VERSION_MIN, SPCT_MINOR_VERSION_MIN))
	{
		S_CTX_ERR(error, S_FAILURE,
				  plugin_init_func,
				  "Incorrect Speect Engine version, require at least '%d.%d.x'",
				  SPCT_MAJOR_VERSION_MIN, SPCT_MINOR_VERSION_MIN);
		return NULL;
	}

	return &plugin_params;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/* plug-in register function */
static void plugin_register_function(s_erc *error)
{
	s_erc local_err = S_SUCCESS;


	S_CLR_ERR(error);

	/* register plug-in classes here */

	_s_token_class_reg(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  plugin_reg_func,
				  "Failed to register SToken class"))
		return;

	_s_tokenizer_class_reg(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  plugin_reg_func,
				  "Failed to register STokenizer class"))
	{
		_s_token_class_free(&local_err);
		return;
	}

	_s_tokenizer_file_class_reg(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  plugin_reg_func,
				  "Failed to register STokenizerFile class"))
	{
		_s_tokenizer_class_free(&local_err);
		_s_token_class_free(&local_err);
		return;
	}

	_s_tokenizer_string_class_reg(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  plugin_reg_func,
				  "Failed to register STokenizerString class"))
	{
		_s_tokenizer_file_class_free(&local_err);
		_s_tokenizer_class_free(&local_err);
		_s_token_class_free(&local_err);
		return;
	}
}


/* plug-in exit function */
static void plugin_exit_function(s_erc *error)
{
	s_erc local_err = S_SUCCESS;


	S_CLR_ERR(error);

	/* free plug-in classes here */
	_s_tokenizer_string_class_free(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  plugin_exit_func,
				  "Failed to free STokenizerString class"))
		local_err = *error;

	_s_tokenizer_file_class_free(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  plugin_exit_func,
				  "Failed to free STokenizerFile class"))
		local_err = *error;

	_s_tokenizer_class_free(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  plugin_exit_func,
				  "Failed to free STokenizer class"))
		local_err = *error;

	_s_token_class_free(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  plugin_exit_func,
				  "Failed to free SToken class"))
		local_err = *error;

	if ((local_err != S_SUCCESS) && (*error == S_SUCCESS))
		*error = local_err;
}
