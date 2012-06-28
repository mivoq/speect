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
#include "pluginmanager/plugin_object.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Call the given function method of the given #SPlugin.
 *
 * @param SELF The given #SPlugin*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage:
 @verbatim
 S_PLUGIN_CALL(self, func)(param1, param2, ..., paramN);
 @endverbatim
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_PLUGIN_CALL(SELF, FUNC)				\
	((SPluginClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SPlugin
 * can be called.
 *
 * @param SELF The given #SPlugin*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_PLUGIN_METH_VALID(SELF, FUNC)			\
	S_PLUGIN_CALL(SELF, FUNC) ? TRUE : FALSE


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
	s_class_add(S_OBJECTCLASS(&PluginClass), error);
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
	s_mutex_init(&(self->plugin_mutex));
}


static void DestroyPlugin(void *obj, s_erc *error)
{
	SPlugin *self = obj;


	S_CLR_ERR(error);

	s_mutex_lock(&(self->plugin_mutex));

	if (self->library != NULL)
		S_DELETE(self->library, "DestroyPlugin", error);

	s_mutex_unlock(&(self->plugin_mutex));
	s_mutex_destroy(&(self->plugin_mutex));
}


static void DisposePlugin(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static s_bool IsReady(const SPlugin *self)
{
	if (self->library != NULL)
		return self->library->ready;

	return FALSE;
}


static void SetReady(SPlugin *self)
{
	if (self->library != NULL)
		self->library->ready = TRUE;
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
