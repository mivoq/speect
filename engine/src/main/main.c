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
/* DATE    : 28 January 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* The Speect Engine start-up and exit functions.                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <locale.h>
#include "speect.h"
#include "main/ini.h"
#include "main/loggers.h"
#include "main/modules.h"
#include "main/managers.h"
#include "main/main.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static uint initialized_count = 0;


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/*
 * Initialize the Speect library.
 * The order of the calls are important.
 */
S_API s_erc speect_init(s_logger *logger)
{
	s_erc local_err = S_SUCCESS;
	s_ini_parser *spct_ini;
	s_bool have_plugin_path;
	const char *def_plugin_path;
	s_logger *local_logger;


	if (initialized_count++ > 0)
		return S_SUCCESS;

	/* set the current locale */
	setlocale(LC_ALL, "");

	/* load the Speect Engine initialization file (INI). */
	spct_ini = _s_load_speect_ini(&local_err);
	if ((spct_ini == NULL) || (local_err != S_SUCCESS))
	{
		S_ERR_PRINT(S_FAILURE, "speect_init",
					"Failed to load Speect Engine INI file");
		initialized_count--;
		return S_FAILURE;
	}

	/* create logger to stderr if no logger was given */
	if (logger == NULL)
	{
		local_logger = s_logger_console_new(FALSE); /* FALSE == log to stderr */
		if (local_logger == NULL)
		{
			S_ERR_PRINT(S_FAILURE, "speect_init",
						"Failed to create a logger to stderr");
			initialized_count--;
			return S_FAILURE;
		}

		logger = local_logger;
	}

	/* initialize error handling module */
	_s_errdbg_init(logger, S_DBG_NONE, &local_err);
	if (local_err != S_SUCCESS)
	{
		S_ERR_PRINT(S_FAILURE, "speect_init",
					"Failed to initialized error handling module");

		/*
		 * this is the only time that we explicitly destroy the
		 * loggers, if errdbg is initialized successfully then
		 * it takes hold of the loggers and destroys them
		 * when it is quit.
		 */
		_s_destroy_loggers(logger, &local_err);
		logger = NULL;
		initialized_count--;
		return S_FAILURE;
	}

	/* create the class repository, size of 128 should be OK for now. */
	_s_classes_create(128, &local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "speect_init",
				  "Failed to create Speect Engine class repository"))
	{
		logger = NULL;
		_s_errdbg_quit(&local_err);
		initialized_count--;
		return S_FAILURE;
	}

	/* initialize the rest of the Speect modules */
	_s_modules_init(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "speect_init",
				  "Failed to initialize Speect Engine modules"))
	{
		logger = NULL;
		_s_errdbg_quit(&local_err);
		initialized_count--;
		return S_FAILURE;
	}

	/* initialize the class repository */
	_s_classes_init(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "speect_init",
				  "Failed to initialize Speect Engine class repository"))
	{
		logger = NULL;
		_s_modules_quit(&local_err);
		_s_errdbg_quit(&local_err);
		initialized_count--;
		return S_FAILURE;
	}

	/* initialize the managers (PluginManager/VoiceManager) */
	_s_managers_init(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "speect_init",
				  "Failed to initialize Speect Engine managers"))
	{
		logger = NULL;
		_s_modules_quit(&local_err);
		_s_classes_clear(&local_err);
		_s_errdbg_quit(&local_err);
		initialized_count--;
		return S_FAILURE;
	}

	/* get default plug-in path from INI parser */
	have_plugin_path = s_iniparser_entry_present(spct_ini, "paths:plug-ins");
	if (have_plugin_path)
	{
		def_plugin_path = s_iniparser_get_string(spct_ini, "paths:plug-ins",
												 NULL);
		_s_pm_set_plugin_path(def_plugin_path, &local_err);
		if (S_CHK_ERR(&local_err, S_CONTERR,
					  "speect_init",
					  "Failed to set default plug-in path"))
		{
			logger = NULL;
			_s_managers_quit(&local_err);
			_s_modules_quit(&local_err);
			_s_classes_clear(&local_err);
			_s_errdbg_quit(&local_err);
			initialized_count--;
			return S_FAILURE;
		}
	}

	s_iniparser_free(spct_ini);

#ifdef SPCT_DEBUGMODE
	_s_classes_print(&local_err);
	S_CHK_ERR(&local_err, S_CONTERR,
			  "speect_init",
			  "Failed to print Speect Engine class repository stats");
#endif

	return S_SUCCESS;
}


/*
 * Request a graceful shutdown of the Speect.
 * The order of the calls are important.
 */
S_API s_erc speect_quit(void)
{
	s_erc local_err = S_SUCCESS;
	s_erc store_err = S_SUCCESS;


	if ((initialized_count == 0)
		|| (--initialized_count > 0))
		return S_SUCCESS;

	/* quit the managers (PluginManager/VoiceManager) */
	_s_managers_quit(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "speect_quit",
				  "Failed to quit Speect Engine managers"))
		store_err = local_err;

	/* quit Speect modules */
	_s_modules_quit(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "speect_quit",
				  "Failed to quit Speect Engine modules"))
		store_err = local_err;

	/* clear the class repository */
	_s_classes_clear(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "speect_quit",
				  "Failed to clear the Speect Engine class repository"))
		store_err = local_err;

	/* loggers are destroyed by errdbg module */
	_s_errdbg_quit(&local_err);
	if (local_err != S_SUCCESS)
	{
		S_ERR_PRINT(S_FAILURE, "speect_quit",
					"Failed to quit error handling module");
		store_err = local_err;
	}

	if ((store_err != S_SUCCESS) && (local_err == S_SUCCESS))
		local_err = store_err;

	return local_err;
}

