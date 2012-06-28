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
#include "main/plugin_path.h"
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
	char *plugin_path;
	s_logger *local_logger;


	if (initialized_count++ > 0)
		return S_SUCCESS;

#if 0  /* this seems to break stuff */
	/* set the current locale */
	if (setlocale(LC_ALL, "") == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "speect_init",
					"Failed to set the locale");
		initialized_count--;
		return S_FAILURE;
	}
#endif

#ifdef SPCT_ERROR_HANDLING
	/* create a logger if no logger was given and
	 * SPCT_ERROR_HANDLING is defined.
	 */
	if (logger == NULL)
	{
		/* if SPCT_DEBUGMODE is defined (defined for CMake Debug
		 * builds) then log to console (stderr), else a null logger
		 * will be created (error checking still takes place, not
		 * logged).
		 */
#ifdef SPCT_DEBUGMODE
		/* FALSE == log to stderr */
		local_logger = s_logger_console_new(FALSE);
#else /* !SPCT_DEBUGMODE */
		local_logger = s_logger_null_new();
#endif /* SPCT_DEBUGMODE */
		if (local_logger == NULL)
		{
			S_ERR_PRINT(S_FAILURE, "speect_init",
						"Failed to create a null logger");
			initialized_count--;
			return S_FAILURE;
		}

		logger = local_logger;
	}
#else /* !SPCT_ERROR_HANDLING */
	local_logger = NULL;
#endif /* SPCT_ERROR_HANDLING */

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
#ifdef SPCT_ERROR_HANDLING
		_s_destroy_loggers(logger, &local_err);
		logger = NULL;
#endif /* SPCT_ERROR_HANDLING */
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

	/* get the plug-in path */
	plugin_path = _s_find_plugin_path(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "speect_init",
				  "Unable to find the plug-in path, call to \"_s_find_plugin_path\" failed"))
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
		S_FREE(plugin_path);
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
		S_FREE(plugin_path);
		_s_modules_quit(&local_err);
		_s_classes_clear(&local_err);
		_s_errdbg_quit(&local_err);
		initialized_count--;
		return S_FAILURE;
	}


	/* set the plug-in path in the plugin-manager */
	_s_pm_set_plugin_path(plugin_path, &local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "speect_init",
				  "Failed to set plug-in path in the plugin-manager"))
	{
		logger = NULL;
		S_FREE(plugin_path);
		_s_managers_quit(&local_err);
		_s_modules_quit(&local_err);
		_s_classes_clear(&local_err);
		_s_errdbg_quit(&local_err);
		initialized_count--;
		return S_FAILURE;
	}

	S_FREE(plugin_path);

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

