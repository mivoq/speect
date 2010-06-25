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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* EBML reader for SHalfphoneDB files.                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "halfphone_db_ebml.h"
#include "serialized_halfphone_db.h"
#include "plugin_info.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SPlugin *halfphoneDBPlugin = NULL;

static SPlugin *ebmlPlugin = NULL;

static SPlugin *ebmlPrimitivesPlugin = NULL;

static SPlugin *ebmlArrayIntPlugin = NULL;

static SPlugin *ebmlArrayFloatPlugin = NULL;

static SPlugin *ebmlMatrixIntPlugin = NULL;

static SPlugin *ebmlMatrixFloatPlugin = NULL;

static SPlugin *ebmlTrackIntPlugin = NULL;

static SPlugin *ebmlTrackFloatPlugin = NULL;


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

const s_plugin_params *s_plugin_init(s_erc *error)
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

	/* load additional plug-ins */
	halfphoneDBPlugin = s_pm_load_plugin("halfphone_db.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Call to \"s_pm_load_plugin\" failed"))
		return;

	ebmlPlugin = s_pm_load_plugin("ebml.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Call to \"s_pm_load_plugin\" failed"))
	{
		S_DELETE(halfphoneDBPlugin, SPCT_PLUGIN_REG_STR, error);
		return;
	}

	ebmlPrimitivesPlugin = s_pm_load_plugin("prim_ebml.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Call to \"s_pm_load_plugin\" failed"))
	{
		S_DELETE(halfphoneDBPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPlugin, SPCT_PLUGIN_REG_STR, error);
		return;
	}

	ebmlArrayIntPlugin = s_pm_load_plugin("array_int_ebml.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Call to \"s_pm_load_plugin\" failed"))
	{
		S_DELETE(halfphoneDBPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPrimitivesPlugin, SPCT_PLUGIN_REG_STR, error);
		return;
	}

	ebmlArrayFloatPlugin = s_pm_load_plugin("array_float_ebml.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Call to \"s_pm_load_plugin\" failed"))
	{
		S_DELETE(halfphoneDBPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPrimitivesPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlArrayIntPlugin, SPCT_PLUGIN_REG_STR, error);
		return;
	}

	ebmlMatrixIntPlugin = s_pm_load_plugin("matrix_int_ebml.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Call to \"s_pm_load_plugin\" failed"))
	{
		S_DELETE(halfphoneDBPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPrimitivesPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlArrayIntPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlArrayFloatPlugin, SPCT_PLUGIN_REG_STR, error);
		return;
	}

	ebmlMatrixFloatPlugin = s_pm_load_plugin("matrix_float_ebml.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Call to \"s_pm_load_plugin\" failed"))
	{
		S_DELETE(halfphoneDBPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPrimitivesPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlArrayIntPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlArrayFloatPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlMatrixIntPlugin, SPCT_PLUGIN_REG_STR, error);
		return;
	}

	ebmlTrackIntPlugin = s_pm_load_plugin("track_int_ebml.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Call to \"s_pm_load_plugin\" failed"))
	{
		S_DELETE(halfphoneDBPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPrimitivesPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlArrayIntPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlArrayFloatPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlMatrixIntPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlMatrixFloatPlugin, SPCT_PLUGIN_REG_STR, error);
		return;
	}

	ebmlTrackFloatPlugin = s_pm_load_plugin("track_float_ebml.spi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Call to \"s_pm_load_plugin\" failed"))
	{
		S_DELETE(halfphoneDBPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPrimitivesPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlArrayIntPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlArrayFloatPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlMatrixIntPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlMatrixFloatPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlTrackIntPlugin, SPCT_PLUGIN_REG_STR, error);
		return;
	}

	/* register plug-in classes here */
	_s_halfphone_db_ebml_class_reg(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  SPCT_PLUGIN_REG_FAIL_STR))

	{
		S_DELETE(halfphoneDBPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPrimitivesPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlArrayIntPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlArrayFloatPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlMatrixIntPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlMatrixFloatPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlTrackIntPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlTrackFloatPlugin, SPCT_PLUGIN_REG_STR, error);
		return;
	}

	_s_serialized_ebml_halfphone_db_reg(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  SPCT_PLUGIN_REG_STR,
				  "Failed to register SEBMLHalfphoneDBFile class"))
	{
		s_erc local_err = S_SUCCESS;


		_s_halfphone_db_ebml_class_free(&local_err);
		S_DELETE(halfphoneDBPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlPrimitivesPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlArrayIntPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlArrayFloatPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlMatrixIntPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlMatrixFloatPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlTrackIntPlugin, SPCT_PLUGIN_REG_STR, error);
		S_DELETE(ebmlTrackFloatPlugin, SPCT_PLUGIN_REG_STR, error);
		return;
	}
}


/* plug-in exit function */
static void plugin_exit_function(s_erc *error)
{
	s_erc local_err = S_SUCCESS;


	S_CLR_ERR(error);

	/* free plug-in classes here */
	_s_serialized_ebml_halfphone_db_free(error);
	S_CHK_ERR(error, S_CONTERR,
			  SPCT_PLUGIN_EXIT_STR,
			  "Failed to free SEBMLHalfphoneDBFile class");


	_s_halfphone_db_ebml_class_free(&local_err);
	S_CHK_ERR(&local_err, S_CONTERR,
			  SPCT_PLUGIN_EXIT_STR,
			  SPCT_PLUGIN_EXIT_FAIL_STR);

	if ((error != NULL)
		&& (*error == S_SUCCESS)
		&& (local_err != S_SUCCESS))
		*error = local_err;

	/* unload plug-ins */
	S_DELETE(halfphoneDBPlugin, SPCT_PLUGIN_EXIT_STR, error);
	S_DELETE(ebmlPlugin, SPCT_PLUGIN_EXIT_STR, error);
	S_DELETE(ebmlPrimitivesPlugin, SPCT_PLUGIN_EXIT_STR, error);
	S_DELETE(ebmlArrayIntPlugin, SPCT_PLUGIN_EXIT_STR, error);
	S_DELETE(ebmlArrayFloatPlugin, SPCT_PLUGIN_EXIT_STR, error);
	S_DELETE(ebmlMatrixIntPlugin, SPCT_PLUGIN_EXIT_STR, error);
	S_DELETE(ebmlMatrixFloatPlugin, SPCT_PLUGIN_EXIT_STR, error);
	S_DELETE(ebmlTrackIntPlugin, SPCT_PLUGIN_EXIT_STR, error);
	S_DELETE(ebmlTrackFloatPlugin, SPCT_PLUGIN_EXIT_STR, error);
}
