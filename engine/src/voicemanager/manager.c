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
/* DATE    : 9 January 2009                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Implementation of the voice manager.                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "base/strings/sprint.h"
#include "serialization/json/json_parse_config.h"
#include "containers/containers.h"
#include "serialization/serialize.h"
#include "pluginmanager/pluginmanager.h"
#include "voicemanager/loaders/loaders.h"
#include "voicemanager/voice.h"
#include "voicemanager/manager.h"


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

typedef struct
{
	SObject *dataObject;
	SPlugin *dataPlugin;
} dataType;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/*
 * key = data_path
 * value = dataType
 */
static SMap *dataTypes = NULL;

/*
 * key = get_string_of_pointer(dataObject)
 * value = data_path
 */
static SMap *dataIdentity = NULL;

static s_bool initialized = FALSE;

S_DECLARE_MUTEX_STATIC(vm_mutex);


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static const SObject *load_data(const char *plugin_path,
								const char *data_path,
								const char *data_format,
								s_erc *error);

static void unload_data(SObject *dataObject, s_erc *error);

static char *get_string_of_pointer(void *ptr, s_erc *error);

static void cache_add(const SObject *dataEntry, const char *data_path,
					  const char *data_identity, s_erc *error);

static void cache_remove(const char *data_path, const char *data_identity,
						 s_erc *error);

static SVoice *load_voice_no_data(const char *path, SMap **dataConfig, s_erc *error);

static s_bool data_loaded(SObject *dataObject, s_erc *error);

static void free_dataType(void *ptr, s_erc *error);

static s_bool data_loaded_inc_ref(SObject *dataObject, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API SVoice *s_vm_load_voice(const char *path, s_erc *error)
{
	SVoice *voice;
	SMap *dataConfig = NULL;


	S_CLR_ERR(error);

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_vm_load_voice",
				  "Argument \"path\" is NULL");
		return NULL;
	}

	s_mutex_lock(&vm_mutex);
	voice = load_voice_no_data(path, &dataConfig, error);
	s_mutex_unlock(&vm_mutex);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_vm_load_voice",
				  "Call to \"load_voice_no_data\" failed"))
		return NULL;

	/*
	 * Now load data.
	 * Data is loaded by voice and not above because the voice data
	 * type is opaque and defined in the voice.
	 */
	_s_voice_load_data(voice, dataConfig, error); /* mutex is locked by _s_vm_load_data */
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_vm_load_voice",
				  "Call to \"_s_voice_load_data\" failed"))
	{
		S_DELETE(voice, "s_vm_load_voice", error);
		S_DELETE(dataConfig, "s_vm_load_voice", error);
		return NULL;
	}

	S_DELETE(dataConfig, "s_vm_load_voice", error);
	return voice;
}


S_LOCAL s_bool _s_vm_data_loaded_inc_ref(SObject *dataObject, s_erc *error)
{
	s_bool loaded;


	S_CLR_ERR(error);

	if (dataObject == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "_s_vm_data_loaded_inc_ref",
				  "Argument \"dataObject\" is NULL");
		return FALSE;
	}

	s_mutex_lock(&vm_mutex);
	loaded = data_loaded_inc_ref(dataObject, error);
	s_mutex_unlock(&vm_mutex);

	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_vm_data_loaded_inc_ref",
				  "Call to \"data_loaded_inc_ref\" failed"))
		return FALSE;

	return loaded;
}


S_LOCAL const SObject *_s_vm_load_data(const char *plugin_path,
									   const char *data_path,
									   const char *data_format,
									   s_erc *error)
{
	const SObject *dataObject;


	S_CLR_ERR(error);

	if (plugin_path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "_s_vm_load_data",
				  "Argument \"plugin_path\" is NULL");
		return NULL;
	}

	if (data_path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "_s_vm_load_data",
				  "Argument \"data_path\" is NULL");
		return NULL;
	}

	if (data_format == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "_s_vm_load_data",
				  "Argument \"data_format\" is NULL");
		return NULL;
	}

	s_mutex_lock(&vm_mutex);
	dataObject = load_data(plugin_path, data_path, data_format, error);
	s_mutex_unlock(&vm_mutex);

	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_vm_load_data",
				  "Call to \"load_data\" failed"))
		return NULL;

	return dataObject;
}


S_LOCAL void _s_vm_unload_data(SObject *dataObject, s_erc *error)
{
	S_CLR_ERR(error);

	if (dataObject == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "_s_vm_unload_data",
				  "Argument \"dataObject\" is NULL");
		return;
	}

	s_mutex_lock(&vm_mutex);
	unload_data(dataObject, error);
	s_mutex_unlock(&vm_mutex);

	S_CHK_ERR(error, S_CONTERR,
			  "_s_vm_unload_data",
			  "Call to \"unload_data\" failed");
}


S_LOCAL s_bool _s_vm_data_loaded(SObject *dataObject, s_erc *error)
{
	s_bool loaded;


	S_CLR_ERR(error);

	if (dataObject == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "_s_vm_data_loaded",
				  "Argument \"dataObject\" is NULL");
		return FALSE;
	}

	s_mutex_lock(&vm_mutex);
	loaded = data_loaded(dataObject, error);
	s_mutex_unlock(&vm_mutex);

	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_vm_data_loaded",
				  "Call to \"data_loaded\" failed"))
		return FALSE;

	return loaded;
}


S_LOCAL void _s_vm_init(s_erc *error)
{
	S_CLR_ERR(error);

	if (initialized == TRUE)
		return;

	initialized = TRUE;
	s_mutex_init(&vm_mutex);

	dataTypes = S_MAP(S_NEW(SMapList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_vm_init",
				  "Failed to create new map for data cache"))
	{
		s_mutex_destroy(&vm_mutex);
		initialized = FALSE;
		return;
	}

	dataIdentity = S_MAP(S_NEW(SMapList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_vm_init",
				  "Failed to create new map for data identity"))
	{
		s_mutex_destroy(&vm_mutex);
		initialized = FALSE;
		S_DELETE(dataTypes, "_s_vm_init", error);
		return;
	}
}


S_LOCAL void _s_vm_quit(s_erc *error)
{
	S_CLR_ERR(error);

	s_mutex_lock(&vm_mutex);

	if (initialized != TRUE)
		return;

	initialized = FALSE;

	if (dataTypes != NULL)
		S_DELETE(dataTypes, "_s_vm_quit", error);

	if (dataIdentity != NULL)
		S_DELETE(dataIdentity, "_s_vm_quit", error);

	s_mutex_unlock(&vm_mutex);
	s_mutex_destroy(&vm_mutex);
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static SVoice *load_voice_no_data(const char *path, SMap **dataConfig, s_erc *error)
{
	SVoice *voice;
	SMap *voiceConfig;


	S_CLR_ERR(error);

	voiceConfig = s_json_parse_config_file(path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_voice_no_data",
				  "Call to \"s_json_parse_config_file\" failed for voice config file '%s'",
				  path))
		return NULL;

	voice = S_NEW(SVoice, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_voice_no_data",
				  "Failed to create new \'SVoice\' object"))
	{
		S_DELETE(voiceConfig, "load_voice_no_data", error);
		return NULL;
	}

	voice->info = _s_parse_voice_info(voiceConfig, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_voice_no_data",
				  "Call to \"_s_parse_voice_info\" failed for '%s' voice config file",
				  path))
	{
		S_DELETE(voice, "load_voice_no_data", error);
		S_DELETE(voiceConfig, "load_voice_no_data", error);
		return NULL;
	}

	voice->plugins = _s_load_voice_plugins(voiceConfig, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_voice_no_data",
				  "Call to \"_s_load_voice_plugins\" failed for '%s' voice config file",
				  path))
	{
		S_DELETE(voice, "load_voice_no_data", error);
		S_DELETE(voiceConfig, "load_voice_no_data", error);
		return NULL;
	}

	voice->features = _s_get_voice_features(voiceConfig, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_voice_no_data",
				  "Call to \"_s_get_voice_features\" failed for '%s' voice config file",
				  path))
	{
		S_DELETE(voice, "load_voice_no_data", error);
		S_DELETE(voiceConfig, "load_voice_no_data", error);
		return NULL;
	}

	/* add config file as voice feature */
	_SVoiceSetFeature_no_lock(voice, "config_file",
							  SObjectSetString(path, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_voice_no_data",
				  "Call to \"SObjectSetString/_SVoiceSetFeature_no_lock\" failed"))
	{
		S_DELETE(voice, "load_voice_no_data", error);
		S_DELETE(voiceConfig, "load_voice_no_data", error);
		return NULL;
	}

	voice->featProcessors = _s_load_voice_feature_processors(voiceConfig,
															 voice->plugins,
															 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_voice_no_data",
				  "Call to \"_s_load_voice_feature_processors\" failed for '%s' voice config file",
				  path))
	{
		S_DELETE(voice, "load_voice_no_data", error);
		S_DELETE(voiceConfig, "load_voice_no_data", error);
		return NULL;
	}

	_s_load_voice_utterance_processors(voiceConfig, voice, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_voice_no_data",
				  "Call to \"_s_load_voice_utterance_processors\" failed for '%s' voice config file",
				  path))
	{
		S_DELETE(voice, "load_voice_no_data", error);
		S_DELETE(voiceConfig, "load_voice_no_data", error);
		return NULL;
	}

	voice->uttTypes = _s_get_voice_utterance_types(voiceConfig, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_voice_no_data",
				  "Call to \"_s_load_voice_utterance_processors\" failed for '%s' voice config file",
				  path))
	{
		S_DELETE(voice, "load_voice_no_data", error);
		S_DELETE(voiceConfig, "load_voice_no_data", error);
		return NULL;
	}

	(*dataConfig) = _s_load_voice_data_config(voiceConfig, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_voice_no_data",
				  "Call to \"_s_load_voice_data_config\" failed for '%s' voice config file",
				  path))
	{
		S_DELETE(voice, "load_voice_no_data", error);
		S_DELETE(voiceConfig, "load_voice_no_data", error);
		return NULL;
	}

	S_DELETE(voiceConfig, "load_voice_no_data", error);

	return voice;
}


static const SObject *load_data(const char *plugin_path,
								const char *data_path,
								const char *data_format,
								s_erc *error)
{
	SObject *dataObject;
	SPlugin *dataPlugin;
	char *data_identity;
	dataType *dataEntry;
	SObject *tmp;


	S_CLR_ERR(error);

	tmp = S_OBJECT(SMapGetObjectDef(dataTypes, data_path, NULL, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_data",
				  "Call to \"SMapGetObjectDef\" for data object at path \'%s\' failed",
				  data_path))
		return NULL;

	if (tmp != NULL)
	{
		/*
		 * it's loaded, increase the dataType object tmp's reference
		 * count, and return the data object.
		 */
		dataEntry = (dataType*)SObjectGetVoid(tmp, "dataType", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "load_data",
					  "Call to \"SObjectGetVoid\" for type \'dataType\' failed"))
			return NULL;

		SObjectIncRef(tmp);

		return dataEntry->dataObject;
	}

	/*
	 * not loaded, load the plug-in and create the data object. Create
	 * the data identity string, add the data object, data plug-in and
	 * data identity to their respective maps.
	 */
	dataPlugin = s_pm_load_plugin(plugin_path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_data",
				  "Call to \"s_pm_load_plugin\" failed of data plug-in at \'%s\'",
				  plugin_path))
		return NULL;

	dataObject = SObjectLoad(data_path, data_format, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_data",
				  "Call to \"SObjectLoad\" failed for data at path \'%s\'",
				  data_path))
	{
		S_DELETE(dataPlugin, "load_data", error);
		return NULL;
	}

	data_identity = get_string_of_pointer(dataObject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_data",
				  "Call to \"get_string_of_pointer\" failed for data at path \'%s\'",
				  data_path))
	{
		S_DELETE(dataPlugin, "load_data", error);
		S_DELETE(dataObject, "load_data", error);
		return NULL;
	}

	dataEntry = S_MALLOC(dataType, 1);
	if (dataEntry == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "load_data",
				  "Failed to allocate memory for 'dataType' object");
		S_DELETE(dataPlugin, "load_data", error);
		S_DELETE(dataObject, "load_data", error);
		S_FREE(data_identity);
		return NULL;
	}

	dataEntry->dataObject = dataObject;
	dataEntry->dataPlugin = dataPlugin;
	tmp = SObjectSetVoid(dataEntry, "dataType", free_dataType, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_data",
				  "Call to \"SObjectSetVoid\" failed for data at path \'%s\'",
				  data_path))
	{
		S_DELETE(dataPlugin, "load_data", error);
		S_DELETE(dataObject, "load_data", error);
		S_FREE(dataEntry);
		S_FREE(data_identity);
		return NULL;
	}

	cache_add(tmp, data_path, data_identity, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_data",
				  "Call to \"cache_add\" failed for data at path \'%s\'",
				  data_path))
	{
		S_DELETE(tmp, "load_data", error);
		S_FREE(data_identity);
		return NULL;
	}

	S_FREE(data_identity);
	return dataObject;
}


static s_bool data_loaded_inc_ref(SObject *dataObject, s_erc *error)
{
	const SObject *dataPath;
	const char *data_path;
	SObject *tmp;
	char *data_identity;


	S_CLR_ERR(error);

	data_identity = get_string_of_pointer(dataObject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "data_loaded_inc_ref",
				  "Call to \"get_string_of_pointer\" failed"))
		return FALSE;

	dataPath = SMapGetObjectDef(dataIdentity, data_identity, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "data_loaded_inc_ref",
				  "Call to \"SMapGetObjectDef\" failed"))
	{
		S_FREE(data_identity);
		return FALSE;
	}

	S_FREE(data_identity);

	if (dataPath == NULL)
	{
		/* not loaded */
		return FALSE;
	}

	/* loaded, inc ref */
	data_path = SObjectGetString(dataPath, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "data_loaded_inc_ref",
				  "Call to \"SObjectGetString\" failed"))
		return FALSE;

	tmp = S_OBJECT(SMapGetObjectDef(dataTypes, data_path, NULL, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "data_loaded_inc_ref",
				  "Call to \"SMapGetObjectDef\" for data object at path \'%s\' failed",
				  data_path))
		return FALSE;

	if (tmp != NULL)
	{
		/*
		 * it's loaded, increase reference count.
		 */
		SObjectIncRef(tmp);
		return TRUE;
	}

	/* error here, dataIdentity says it's loaded, but we can't find
	 * it in dataTypes
	 */
	S_CTX_ERR(error, S_FAILURE,
			  "data_loaded_inc_ref",
			  "Given object appears to be loaded, but could not be found in cache.");
	return FALSE;
}


static void unload_data(SObject *dataObject, s_erc *error)
{
	char *data_identity;
	const char *data_path;
	const SObject *tmp;


	S_CLR_ERR(error);

	data_identity = get_string_of_pointer(dataObject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "unload_data",
				  "Call to \"get_string_of_pointer\" failed"))
		return;

	data_path = SMapGetStringDef(dataIdentity, data_identity, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "unload_data",
				  "Call to \"SMapGetStringDef\" failed"))
	{
		S_FREE(data_identity);
		return;
	}

	if (data_path == NULL)
	{
		/*
		 * data object is not in VoiceManager cache.
		 * This is an error.
		 */
		S_CTX_ERR(error, S_FAILURE,
				  "unload_data",
				  "Data object not in VoiceManager cache");
		S_FREE(data_identity);
		return;
	}

	tmp = SMapGetObjectDef(dataTypes, data_path, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "unload_data",
				  "Call to \"SMapGetObjectDef\" for data path \'%s\' failed",
				  data_path))
	{
		S_FREE(data_identity);
		return;
	}

	if (tmp == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "unload_data",
				  "Data at path \'%s\' was not loaded",
				  data_path);
		S_FREE(data_identity);
		return;
	}

	if (S_OBJECT_REF(tmp) == 1)
	{
		/*
		 * Data belongs to caller and VoiceManager,
		 * remove from cache.
		 */
		cache_remove(data_path, data_identity, error);
		S_CHK_ERR(error, S_CONTERR,
				  "unload_data",
				  "Call to \"cache_remove\" for data path \'%s\' failed",
				  data_path);
	}
	else
	{
		/*
		 * Data is referenced by someone else,
		 * just decrement reference count.
		 */
		SObjectDecRef((SObject*)tmp);
	}

	S_FREE(data_identity);
}


static s_bool data_loaded(SObject *dataObject, s_erc *error)
{
	char *data_identity;
	s_bool data_loaded;


	S_CLR_ERR(error);

	data_identity = get_string_of_pointer(dataObject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "data_loaded",
				  "Call to \"get_string_of_pointer\" failed"))
		return FALSE;

	data_loaded = SMapObjectPresent(dataIdentity, data_identity, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "data_loaded",
				  "Call to \"SMapObjectPresent\" failed"))
	{
		S_FREE(data_identity);
		return FALSE;
	}

	S_FREE(data_identity);
	return data_loaded;
}


static char *get_string_of_pointer(void *ptr, s_erc *error)
{
	char *string;


	S_CLR_ERR(error);
	s_asprintf(&string, error, "%p", ptr);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_string_of_pointer",
				  "Call to \"s_asprintf\" failed"))
		return NULL;

	return string;
}


static void cache_add(const SObject *dataEntry, const char *data_path,
					  const char *data_identity, s_erc *error)
{
	S_CLR_ERR(error);

	SMapSetObject(dataTypes, data_path, dataEntry, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "cache_add",
				  "Call to \"SMapSetObject\" failed, could not"
				  " add data entry for data path at \'%s\' to cache", data_path))
		return;

	SMapSetObject(dataIdentity, data_identity, SObjectSetString(data_path, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "cache_add",
				  "Call to \"SMapSetObject\" failed, could not"
				  " add data identity for data path at \'%s\' to cache", data_path))
	{
		/* we need to unlink dataEntry from dataTypes */
		SMapObjectUnlink(dataTypes, data_path, error);
		S_CHK_ERR(error, S_CONTERR,
				  "cache_add",
				  "Call to \"SMapObjectUnlink\" failed");
		return;
	}
}


static void cache_remove(const char *data_path, const char *data_identity,
						 s_erc *error)
{
	s_erc local_err = S_SUCCESS;


	S_CLR_ERR(error);

	SMapObjectDelete(dataTypes, data_path, error);
	S_CHK_ERR(error, S_CONTERR,
			  "cache_remove",
			  "Call to \"SMapObjectDelete\" for data type of data path \'%s\' failed",
			  data_path);


	SMapObjectDelete(dataIdentity, data_identity, error);
	S_CHK_ERR(&local_err, S_CONTERR,
			  "cache_remove",
			  "Call to \"SMapObjectDelete\" failed");

	if ((local_err != S_SUCCESS) && (*error == S_SUCCESS))
		*error = local_err;
}


static void free_dataType(void *ptr, s_erc *error)
{
	dataType *dataEntry = ptr;

	S_DELETE(dataEntry->dataObject, "free_dataType", error);
	S_DELETE(dataEntry->dataPlugin, "free_dataType", error);
	S_FREE(dataEntry);
}

