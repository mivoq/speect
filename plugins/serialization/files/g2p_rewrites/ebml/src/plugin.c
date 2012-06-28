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
/* A g2p (grapheme-to-phoneme) rewrites class implementation plugin.                */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "serialized_g2p_rewrites.h"
#include "plugin_info.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SPlugin *g2pPlugin = NULL;

static SPlugin *ebmlPlugin = NULL;


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
	SPCT_PLUGIN_NAME,

	/* description */
	SPCT_PLUGIN_DESCRIPTION,

	/* version */
	{
		SPCT_PLUGIN_VERSION_MAJOR,
		SPCT_PLUGIN_VERSION_MINOR
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

S_PLUGIN_API const s_plugin_params *s_plugin_init(s_erc *error)
{
	S_CLR_ERR(error);

	if (!s_lib_version_ok(SPCT_MAJOR_VERSION_MIN, SPCT_MINOR_VERSION_MIN))
	{
		S_CTX_ERR(error, S_FAILURE,
				  SPCT_PLUGIN_INIT_STR,
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
	S_CLR_ERR(error);

	/* load plug-ins */
	g2pPlugin = s_pm_load_plugin("g2p.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Call to \"s_pm_load_plugin\" failed"))
		return;

	ebmlPlugin = s_pm_load_plugin("ebml.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Call to \"s_pm_load_plugin\" failed"))
	{
		S_DELETE(g2pPlugin, SPCT_PLUGIN_REG_STR, error);
		return;
	}

	/* register plug-in classes here */
	_s_g2p_rewrites_rule_class_reg(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Failed to register SG2PRewritesRule class"))
	{
		S_DELETE(g2pPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPlugin, SPCT_PLUGIN_REG_STR, error);
		return;
	}

	_s_g2p_rewrites_class_reg(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
			  SPCT_PLUGIN_REG_FAIL_STR))
	{
		s_erc local_err = S_SUCCESS;


		_s_g2p_rewrites_rule_class_free(&local_err);
		S_DELETE(g2pPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPlugin, SPCT_PLUGIN_REG_STR, error);
		return;
	}

	_s_serialized_ebml_g2p_rewrites_reg(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Failed to register SEBMLG2PRewritesFile class"))
	{
		s_erc local_err = S_SUCCESS;


		S_DELETE(g2pPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPlugin, SPCT_PLUGIN_REG_STR, error);
		_s_g2p_rewrites_rule_class_free(&local_err);
		_s_g2p_rewrites_class_free(&local_err);
		return;
	}


}


/* plug-in exit function */
static void plugin_exit_function(s_erc *error)
{
	s_erc local_err = S_SUCCESS;


	S_CLR_ERR(error);

	/* free plug-in classes here */
	_s_serialized_ebml_g2p_rewrites_free(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_EXIT_STR,
				  "Failed to free SEBMLG2PRewritesFile class"))
		if (error != NULL)
			local_err = *error;

	_s_g2p_rewrites_rule_class_free(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_EXIT_STR,
				  "Failed to free SG2PRewritesRule class"))
		if (error != NULL)
			local_err = *error;

	_s_g2p_rewrites_class_free(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_EXIT_STR,
				  SPCT_PLUGIN_EXIT_FAIL_STR))
		if (error != NULL)
			local_err = *error;

	if ((error != NULL)
		&& (*error == S_SUCCESS)
		&& (local_err != S_SUCCESS))
		*error = local_err;

	S_DELETE(g2pPlugin, SPCT_PLUGIN_EXIT_STR, error);
	S_DELETE(ebmlPlugin, SPCT_PLUGIN_EXIT_STR, error);
}
