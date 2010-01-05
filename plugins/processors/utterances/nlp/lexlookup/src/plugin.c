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
/* A LexLookup utterance processor.                                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "lexlookup_proc.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static const char * const plugin_init_func = "SLexLookupUttProc plug-in initialization";

static const char * const plugin_exit_func = "SLexLookupUttProc plug-in free";

static SPlugin *addendumPlugin = NULL;

static SPlugin *lexiconPlugin = NULL;

static SPlugin *g2pPlugin = NULL;

static SPlugin *syllabificationPlugin = NULL;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_bool version_ok(const s_lib_version version);

static void plugin_exit_function(s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in parameters                                                               */
/*                                                                                  */
/************************************************************************************/

static const s_plugin_params plugin_params =
{
	/* plug-in name */
	"LexLookup utterance processor",

	/* description */
	"An Utterance Processor class that does the lexical lookup (addendum, lexicon, g2p) "
	"of the words in the Word relation. Creates SylStructure, Syllable, and Phone relations",

	/* version */
	{
		0,
		1
	},

	/* Speect ABI version (which plug-in was compiled with) */
	{
		0,
		9
	},

	/* exit function pointer */
	plugin_exit_function
};


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

const s_plugin_params *s_plugin_init(const s_lib_version version, s_erc *error)
{
	S_CLR_ERR(error);

	if (!version_ok(version))
	{
		S_CTX_ERR(error, S_FAILURE,
				  plugin_init_func,
				  "Incorrect Speect Engine version, require '0.9.x'");
		return NULL;
	}

	/* load plug-ins */
	addendumPlugin = s_pm_load_plugin("addendum.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  plugin_init_func,
				  "Call to \"s_pm_load_plugin\" failed"))
		return NULL;

	lexiconPlugin = s_pm_load_plugin("lexicon.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  plugin_init_func,
				  "Call to \"s_pm_load_plugin\" failed"))
	{
		S_DELETE(addendumPlugin, plugin_init_func, error);
		return NULL;
	}

	g2pPlugin = s_pm_load_plugin("g2p.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  plugin_init_func,
				  "Call to \"s_pm_load_plugin\" failed"))
	{
		S_DELETE(addendumPlugin, plugin_init_func, error);
		S_DELETE(lexiconPlugin, plugin_init_func, error);
		return NULL;
	}

	syllabificationPlugin = s_pm_load_plugin("syllabification.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  plugin_init_func,
				  "Call to \"s_pm_load_plugin\" failed"))
	{
		S_DELETE(addendumPlugin, plugin_init_func, error);
		S_DELETE(lexiconPlugin, plugin_init_func, error);
		S_DELETE(g2pPlugin, plugin_init_func, error);
		return NULL;
	}

	/* register plug-in classes here */
	_s_lexlookup_utt_proc_class_reg(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  plugin_init_func,
				  "Failed to register SLexLookupUttProc class"))
	{
		S_DELETE(addendumPlugin, plugin_init_func, error);
		S_DELETE(lexiconPlugin, plugin_init_func, error);
		S_DELETE(g2pPlugin, plugin_init_func, error);
		S_DELETE(syllabificationPlugin, plugin_init_func, error);
		return NULL;
	}

	return &plugin_params;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/* check the Speect Engine version */
static s_bool version_ok(const s_lib_version version)
{
	/*
	 * we want Speect Engine 0.9.x
	 */
	if ((version.major == 0)
		&& (version.minor == 9))
		return TRUE;

	return FALSE;
}


/* plug-in exit function */
static void plugin_exit_function(s_erc *error)
{
	S_CLR_ERR(error);

	/* free plug-in classes here */
	_s_lexlookup_utt_proc_class_free(error);
	S_CHK_ERR(error, S_CONTERR,
			  plugin_exit_func,
			  "Failed to free SLexLookupUttProc class");

	S_DELETE(addendumPlugin, plugin_init_func, error);
	S_DELETE(lexiconPlugin, plugin_init_func, error);
	S_DELETE(g2pPlugin, plugin_init_func, error);
	S_DELETE(syllabificationPlugin, plugin_init_func, error);
}
