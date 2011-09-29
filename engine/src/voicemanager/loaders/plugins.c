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
/* DATE    : 29 October 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A loader for the voice plug-ins in the voice configuration file.                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/strings/utf8.h"
#include "base/utils/alloc.h"
#include "pluginmanager/manager.h"
#include "voicemanager/loaders/plugins.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SList *_s_load_voice_plugins(const SMap *voiceConfig, s_erc *error)
{
	const SObject *tmp;
	SIterator *itr;
	const SList *pluginList;
	SList *pluginLoadedList;
	SPlugin *plugin;
	s_bool key_present;
	const char *plugin_path;


	S_CLR_ERR(error);

	/* look for "plug-ins" key in voiceConfig map */
	key_present = SMapObjectPresent(voiceConfig, "plug-ins", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_plugins",
				  "Call to \"SMapObjectPresent\" failed for \'plug-ins\' key"))
		return NULL;

	if (!key_present)
	{
		/* no plug-ins, make a new list and return it */
		pluginLoadedList = S_LIST(S_NEW(SListList, error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_plugins",
					  "Failed to create new list for loaded voice plug-ins"))
			return NULL;

		return pluginLoadedList;
	}

	tmp = SMapGetObject(voiceConfig, "plug-ins", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_plugins",
				  "Call to \"SMapGetObject\" failed for \'plug-ins\' key"))
		return NULL;

	pluginList = S_CAST(tmp, SList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_plugins",
				  "Call to \"S_CAST (SList)\" failed for \'plug-ins\' object"))
		return NULL;

	pluginLoadedList = S_LIST(S_NEW(SListList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_plugins",
				  "Failed to create new list for loaded voice plug-ins"))
		return NULL;

	itr = S_ITERATOR_GET(pluginList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_plugins",
				  "Call to \"S_ITERATOR_GET\" failed"))
	{
		S_DELETE(pluginLoadedList, "_s_load_voice_plugins", error);
		return NULL;
	}

	while (itr)
	{
		tmp = SIteratorObject(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_plugins",
					  "Call to \"SIteratorObject\" failed"))
		{
			S_DELETE(itr, "_s_load_voice_plugins", error);
			S_DELETE(pluginLoadedList, "_s_load_voice_plugins", error);
			return NULL;
		}

		plugin_path = SObjectGetString(tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_plugins",
					  "Call to \"SObjectGetString\" failed"))
		{
			S_DELETE(itr, "_s_load_voice_plugins", error);
			S_DELETE(pluginLoadedList, "_s_load_voice_plugins", error);
			return NULL;
		}

		plugin = s_pm_load_plugin(plugin_path, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_plugins",
					  "Call to \"s_pm_load_plugin\" failed for plug-in at path \'%s\'",
					  plugin_path))
		{
			S_DELETE(itr, "_s_load_voice_plugins", error);
			S_DELETE(pluginLoadedList, "_s_load_voice_plugins", error);
			return NULL;
		}

		SListPush(pluginLoadedList, S_OBJECT(plugin), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_plugins",
					  "Call to \"SListPush\" failed"))
		{
			S_DELETE(plugin, "_s_load_voice_plugins", error);
			S_DELETE(itr, "_s_load_voice_plugins", error);
			S_DELETE(pluginLoadedList, "_s_load_voice_plugins", error);
			return NULL;
		}

		itr = SIteratorNext(itr);
	}

	return pluginLoadedList;
}
