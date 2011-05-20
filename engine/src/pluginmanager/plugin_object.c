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
/* DATE    : 5 December 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Implementation of the plugin object.                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "base/strings/utf8.h"
#include "pluginmanager/manager.h"
#include "pluginmanager/plugin_object.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SPluginClass PluginClass; /* Plugin class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL s_bool SPluginIsReady(const SPlugin *self)
{
	s_bool ready_status;
	s_erc error;


	S_CLR_ERR(&error);
	if (self == NULL)
	{
		S_CTX_ERR(&error, S_ARGERROR,
				  "SPluginIsReady",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (!S_PLUGIN_METH_VALID(self, is_ready))
	{
		S_WARNING(S_METHINVLD,
				  "SPluginIsReady",
				  "Plug-in method \"is_ready\" not implemented");
		return FALSE;
	}

	s_mutex_lock((s_mutex*)&(self->plugin_mutex));
	ready_status = S_PLUGIN_CALL(self, is_ready)(self);
	s_mutex_unlock((s_mutex*)&(self->plugin_mutex));

	return ready_status;
}


S_LOCAL void SPluginSetReady(SPlugin *self)
{
	s_erc error;


	S_CLR_ERR(&error);
	if (self == NULL)
	{
		S_CTX_ERR(&error, S_ARGERROR,
				  "SPluginSetReady",
				  "Argument \"self\" is NULL");
		return;
	}

	if (!S_PLUGIN_METH_VALID(self, set_ready))
	{
		S_WARNING(S_METHINVLD,
				  "SPluginIsReady",
				  "Plug-in method \"set_ready\" not implemented");
		return;
	}

	s_mutex_lock((s_mutex*)&(self->plugin_mutex));
	S_PLUGIN_CALL(self, set_ready)(self);
	s_mutex_unlock((s_mutex*)&(self->plugin_mutex));
}


S_LOCAL void _s_plugin_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&PluginClass, error);
	S_CHK_ERR(error, S_CONTERR,
		  "_s_plugin_class_add",
		  "Failed to add SPluginClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitPlugin(void *obj, s_erc *error)
{
	SPlugin *self = obj;


	S_CLR_ERR(error);
	self->library = NULL;
	self->plugin_info = NULL;
	self->path = NULL;
	self->in_pluginmanager = FALSE;
	self->ready = FALSE;
	s_mutex_init(&(self->plugin_mutex));
}


static void DestroyPlugin(void *obj, s_erc *error)
{
	SPlugin *self = obj;
	s_plugin_exit_fp exit_function;
	char *path_c;
	s_erc local_err = S_SUCCESS;


	S_CLR_ERR(error);

	s_mutex_lock(&(self->plugin_mutex));

	if (self->library != NULL)
	{
		exit_function = self->plugin_info->at_exit;

		if (exit_function != NULL)
		{
			exit_function(&local_err);
			S_CHK_ERR(&local_err, S_CONTERR,
				  "DestroyPlugin",
				  "Plugin dso at_exit function reported error");
		}

		S_DELETE(self->library, "DestroyPlugin", error);
	}

	if (self->path != NULL)
	{
		path_c = (char*)self->path;
		S_FREE(path_c);
		self->path = NULL;
	}

	self->plugin_info = NULL;
	self->in_pluginmanager = FALSE;

	if ((local_err != S_SUCCESS) && (*error == S_SUCCESS))
		*error = local_err;

	s_mutex_unlock(&(self->plugin_mutex));
	s_mutex_destroy(&(self->plugin_mutex));
}


static void DisposePlugin(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
	_s_pm_unload_plugin(S_PLUGIN(obj), error);
}


static s_bool IsReady(const SPlugin *self)
{
	return self->ready;
}


static void SetReady(SPlugin *self)
{
	self->ready = TRUE;
}


/************************************************************************************/
/*                                                                                  */
/* SPlugin class initialization                                                     */
/*                                                                                  */
/************************************************************************************/

/* same as SObjectClass */
static SPluginClass PluginClass =
{
	/* SObjectClass */
	{
		"SPlugin",
		sizeof(SPlugin),
		{ 0, 1},
		InitPlugin,    /* init    */
		DestroyPlugin, /* destroy */
		DisposePlugin, /* dispose */
		NULL,          /* compare */
		NULL,          /* print   */
		NULL,          /* copy    */
	},
	IsReady,           /* is_ready  */
	SetReady           /* set_ready */
};
