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
/* DATE    : 29 October 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A loader for the voice data in the voice configuration file.                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "hrg/processors/featprocessor.h"
#include "pluginmanager/manager.h"
#include "serialization/serialize.h"
#include "voicemanager/loaders/features.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SMap *_s_load_voice_data(const SMap *voiceConfig,
								 SList *voicePlugins,
								 s_erc *error)
{
	SMap *data;
	const SObject *tmp;
	const SMap *voiceConfigData;
	s_bool key_present;
	s_bool has_more;
	SIterator *itr;
	const char *data_name;
	const char *data_format;
	const char *data_plugin;
	const char *data_path;
	const SMap *dataInfo;
	SObject *dataObject;
	SPlugin *plugin;
	SList *dataPlugins;


	S_CLR_ERR(error);

	/* create a map container for the data */
	data = S_MAP(S_NEW(SMapList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_data",
				  "Failed to create new map for voice data"))
		return NULL;

	SMapListInit(data, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_data",
				  "Failed to initialize new map for voice data"))
	{
		S_DELETE(data, error, "_s_load_voice_data");
		return NULL;
	}

	/* look for "data" key in voiceConfig map */
	key_present = SMapObjectPresent(voiceConfig, "data", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_data",
				  "Call to \"SMapObjectPresent\" failed for \'data\' key"))
	{
		S_DELETE(data, error, "_s_load_voice_data");
		return NULL;
	}

	if (!key_present)
	{
		/* no defined data  */
		return data;
	}

	/* get data from voiceConfig */
	tmp = SMapGetObject(voiceConfig, "data", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_data",
				  "Call to \"SMapGetObject\" failed"))
	{
		S_DELETE(data, error, "_s_load_voice_data");
		return NULL;
	}

	voiceConfigData = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_data",
				  "Call to \"S_CAST (SMap)\" failed for \'data\' object"))
	{
		S_DELETE(data, error, "_s_load_voice_data");
		return NULL;
	}

	/*
	 * iterate through the voiceConfigData, create the
	 * data objects.
	 */
	itr = SMapIterator(voiceConfigData, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_data",
				  "Call to \"SMapIterator\" failed"))
	{
		S_DELETE(data, error, "_s_load_voice_data");
		return NULL;
	}

	has_more = SIteratorHasMore(itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_data",
				  "Call to \"SIteratorHasMore\" failed"))
	{
		S_DELETE(itr, error, "_s_load_voice_data");
		S_DELETE(data, error, "_s_load_voice_data");
		return NULL;
	}

	if (has_more == TRUE)
	{
		/* create a plug-in list */
		dataPlugins = S_LIST(S_NEW(SListList, error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_data",
					  "Failed to create new list for voice data plug-ins"))
		{
			S_DELETE(itr, error, "_s_load_voice_data");
			S_DELETE(data, error, "_s_load_voice_data");
			return NULL;
		}

		SListListInit(dataPlugins, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_data",
					  "Failed to initialize new list for voice data plug-ins"))
		{
			S_DELETE(dataPlugins, error, "_s_load_voice_data");
			S_DELETE(itr, error, "_s_load_voice_data");
			S_DELETE(data, error, "_s_load_voice_data");
			return NULL;
		}
	}
	else
		dataPlugins = NULL;

	while (has_more == TRUE)
	{
		/* the data name */
		data_name = SMapIteratorKey(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_data",
					  "Call to \"SMapIteratorKey\" failed"))
		{
			S_DELETE(itr, error, "_s_load_voice_data");
			S_DELETE(data, error, "_s_load_voice_data");
			S_DELETE(dataPlugins, error, "_s_load_voice_data");
			return NULL;
		}

		/* the data info (SMap) */
		tmp = SMapIteratorValue(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_data",
					  "Call to \"SMapIteratorValue\" failed"))
		{
			S_DELETE(itr, error, "_s_load_voice_data");
			S_DELETE(data, error, "_s_load_voice_data");
			S_DELETE(dataPlugins, error, "_s_load_voice_data");
			return NULL;
		}

		dataInfo = S_CAST(tmp, SMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_data",
					  "Call to \"S_CAST (SMap)\" failed for data \'%s\'",
					  data_name))
		{
			S_DELETE(itr, error, "_s_load_voice_data");
			S_DELETE(data, error, "_s_load_voice_data");
			S_DELETE(dataPlugins, error, "_s_load_voice_data");
			return NULL;
		}

		/*
		 * dataInfo must have "format", "plug-in" and
		 * "data" keys and entries
		 */
		data_format = SMapGetString(dataInfo, "format", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_data",
					  "Call to \"SMapGetString\" failed for \'format\' name of data \'%s\'",
					  data_name))
		{
			S_DELETE(itr, error, "_s_load_voice_data");
			S_DELETE(data, error, "_s_load_voice_data");
			S_DELETE(dataPlugins, error, "_s_load_voice_data");
			return NULL;
		}

		data_plugin = SMapGetString(dataInfo, "plug-in", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_data",
					  "Call to \"SMapGetString\" failed for \'plug-in\' name of data \'%s\'",
					  data_name))
		{
			S_DELETE(itr, error, "_s_load_voice_data");
			S_DELETE(data, error, "_s_load_voice_data");
			S_DELETE(dataPlugins, error, "_s_load_voice_data");
			return NULL;
		}

		data_path = SMapGetString(dataInfo, "data", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_data",
					  "Call to \"SMapGetString\" failed for \'data\' name of data \'%s\'",
					  data_name))
		{
			S_DELETE(itr, error, "_s_load_voice_data");
			S_DELETE(data, error, "_s_load_voice_data");
			S_DELETE(dataPlugins, error, "_s_load_voice_data");
			return NULL;
		}

		/*
		 * load the plug-in, add it to the list of data plug-ins and create
		 * the data object
		 */
		plugin = s_pm_load_plugin(data_plugin, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_data",
					  "Call to \"s_pm_load_plugin\" failed of data \'%s\'",
					  data_name))
		{
			S_DELETE(itr, error, "_s_load_voice_data");
			S_DELETE(data, error, "_s_load_voice_data");
			S_DELETE(dataPlugins, error, "_s_load_voice_data");
			return NULL;
		}

		SListPush(dataPlugins, S_OBJECT(plugin), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_data",
					  "Call to \"SListPush\" failed for data plug-ins"))
		{
			S_DELETE(plugin, error, "_s_load_voice_data");
			S_DELETE(itr, error, "_s_load_voice_data");
			S_DELETE(data, error, "_s_load_voice_data");
			S_DELETE(dataPlugins, error, "_s_load_voice_data");
			return NULL;
		}

		dataObject = SObjectLoad(data_path, data_format, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_data",
					  "Call to \"SObjectLoad\" failed for data \'%s\' at path \'%s\'",
					  data_name, data_path))
		{
			S_DELETE(itr, error, "_s_load_voice_data");
			S_DELETE(data, error, "_s_load_voice_data");
			S_DELETE(dataPlugins, error, "_s_load_voice_data");
			return NULL;
		}

		/* add the data to the data map  */
		SMapSetObject(data, data_name, dataObject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_data",
					  "Call to \"SMapSetObject\" failed"))
		{
			S_DELETE(dataObject, error, "_s_load_voice_data");
			S_DELETE(itr, error, "_s_load_voice_data");
			S_DELETE(data, error, "_s_load_voice_data");
			S_DELETE(dataPlugins, error, "_s_load_voice_data");
			return NULL;
		}
	}

	S_DELETE(itr, error, "_s_load_voice_data");

	if (dataPlugins != NULL)
	{
		SListMerge(voicePlugins, dataPlugins, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_data",
					  "Call to \"SListMerge\" failed"))
		{
			S_DELETE(data, error, "_s_load_voice_data");
			S_DELETE(dataPlugins, error, "_s_load_voice_data");
			return NULL;
		}

		S_DELETE(dataPlugins, error, "_s_load_voice_data");
	}

	return data;
}



