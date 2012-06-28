/************************************************************************************/
/* LICENSE INFO                                                                     */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* AUTHOR  :                                                                        */
/* DATE    :                                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Description, ...                                                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

/* see TODO:  where to change, rest stays the same */

/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "source.h" /* TODO: files to include */
#include "plugin_info.h" /* always include plugin_info.h */


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

	/* TODO: your plug-in register function */
	/* register plug-in classes here with s_class_reg */

	S_CHK_ERR(error, S_CONTERR,
			  SPCT_PLUGIN_REG_STR,
			  SPCT_PLUGIN_REG_FAIL_STR);
}


/* plug-in exit function */
static void plugin_exit_function(s_erc *error)
{
	S_CLR_ERR(error);

	/* TODO: your plug-in free function here */
	/* free plug-in classes here with s_class_free */

	S_CHK_ERR(error, S_CONTERR,
			  SPCT_PLUGIN_EXIT_STR,
			  SPCT_PLUGIN_EXIT_FAIL_STR);
}
