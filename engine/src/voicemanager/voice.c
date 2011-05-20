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
/* DATE    : 6 January 2009                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Implementation of the voice object.                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/path.h"
#include "base/utils/alloc.h"
#include "base/strings/strings.h"
#include "voicemanager/loaders/data_config.h"
#include "voicemanager/manager.h"
#include "voicemanager/voice.h"


/************************************************************************************/
/*                                                                                  */
/* Voice data (opaque)                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Type definition of the opaque voice data.
 */
struct s_voice_data
{
	SMap *dataObjects;
	SMap *dataConfig;
};


/**
 * Type definition of data info structure.
 */
typedef struct
{
	const char *format;
	const char *plugin;
	const char *path;
} s_data_info;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SVoiceClass VoiceClass;  /* Voice class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void sync_data_config(SVoice *self, SMap *dataConfig, s_erc *error);

static const SObject *load_data_entry(SMap *newDataConfig, const SMap *dataConfig,
									  const char *data_name, s_erc *error);

static void unload_data_entry(SVoice *self, SMap *dataConfig, const char *data_name,
							  s_erc *error);

static s_bool compare_data_config_entries(const SMap *newDataConfig,
										  const SMap *currentDataConfig,
										  const char *data_name,
										  s_erc *error);

static s_data_info *get_data_info(const SMap *map, s_erc *error);

static s_bool compare_data_info(const s_data_info *d1, const s_data_info *d2,
								s_erc *error);

static void free_voice_info(s_voice_info *info);

static void unload_voice_plugins(SList *plugins, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/* synthesis */

S_API SUtterance *SVoiceSynthUtt(const SVoice *self, const char *utt_type,
								 SObject *input, s_erc *error)
{
	SUtterance *utt;
	s_bool key_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSynthUtt",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (utt_type == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSynthUtt",
				  "Argument \"utt_type\" is NULL");
		return NULL;
	}

	if (input == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSynthUtt",
				  "Argument \"input\" is NULL");
		return NULL;
	}

	if (!S_VOICE_METH_VALID(self, synth_utt))
	{
		S_WARNING(S_METHINVLD,
				  "SVoiceSynthUtt",
				  "Voice method \"synth_utt\" not implemented");
		return NULL;
	}

	s_mutex_lock((s_mutex*)&self->voice_mutex);
	key_present = SVoiceUttTypeIsPresent(self, utt_type, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceSynthUtt",
				  "Call to \"SVoiceUttTypeIsPresent\" failed"))
	{
		s_mutex_unlock((s_mutex*)&self->voice_mutex);
		return NULL;
	}

	if (!key_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SVoiceSynthUtt",
				  "Given voice does not have a \'%s\' utterance type",
				  utt_type);
		s_mutex_unlock((s_mutex*)&self->voice_mutex);
		return NULL;
	}

	utt = S_VOICE_CALL(self, synth_utt)(self, utt_type, input, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceSynthUtt",
				  "Call to class method \"synth_utt\" failed"))
	{
		s_mutex_unlock((s_mutex*)&self->voice_mutex);
		return NULL;
	}

	s_mutex_unlock((s_mutex*)&self->voice_mutex);
	return utt;
}


S_API void SVoiceReSynthUtt(const SVoice *self, const char *utt_type,
							SUtterance *utt, s_erc *error)
{
	s_bool key_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceReSynthUtt",
				  "Argument \"self\" is NULL");
		return;
	}

	if (utt_type == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceReSynthUtt",
				  "Argument \"utt_type\" is NULL");
		return;
	}

	if (utt == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceReSynthUtt",
				  "Argument \"utt\" is NULL");
		return;
	}

	if (!S_VOICE_METH_VALID(self, re_synth_utt))
	{
		S_WARNING(S_METHINVLD,
				  "SVoiceReSynthUtt",
				  "Voice method \"re_synth_utt\" not implemented");
		return;
	}

	s_mutex_lock((s_mutex*)&self->voice_mutex);
	key_present = SVoiceUttTypeIsPresent(self, utt_type, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceReSynthUtt",
				  "Call to \"SVoiceUttTypeIsPresent\" failed"))
	{
		s_mutex_unlock((s_mutex*)&self->voice_mutex);
		return;
	}

	if (!key_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SVoiceReSynthUtt",
				  "Given voice does not have a \'%s\' utterance type",
				  utt_type);
		s_mutex_unlock((s_mutex*)&self->voice_mutex);
		return;
	}

	S_VOICE_CALL(self, re_synth_utt)(self, utt_type, utt, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceReSynthUtt",
				  "Call to class method \"re_synth_utt\" failed"))
	{
		s_mutex_unlock((s_mutex*)&self->voice_mutex);
		return;
	}

	s_mutex_unlock((s_mutex*)&self->voice_mutex);
}


/* info */

S_API const char *SVoiceGetName(const SVoice *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetName",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	return self->info? self->info->name : NULL;
}


S_API const char *SVoiceGetDescription(const SVoice *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetDescription",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	return self->info? self->info->description : NULL;
}


S_API const char *SVoiceGetGender(const SVoice *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetGender",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	return self->info? self->info->gender : NULL;
}


S_API const char *SVoiceGetLanguage(const SVoice *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetLanguage",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	return self->info? self->info->language : NULL;
}


S_API const char *SVoiceGetLangCode(const SVoice *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetLangCode",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	return self->info? self->info->lang_code : NULL;
}


S_API const s_version *SVoiceGetVersion(const SVoice *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetVersion",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	return self->info? &(self->info->version) : NULL;
}


/* data configuration */

S_API SMap *SVoiceGetDataConfig(const SVoice *self, s_erc *error)
{
	SMap *dataConfigCopy;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetDataConfig",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	dataConfigCopy = SMapCopy(NULL, self->data->dataConfig, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceGetDataConfig",
				  "Call to \"SMapCopy\" failed"))
		return NULL;

	return dataConfigCopy;
}


S_API void SVoiceSetDataConfig(SVoice *self, SMap *dataConfig, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetDataConfig",
				  "Argument \"self\" is NULL");
		return;
	}

	if (dataConfig == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetDataConfig",
				  "Argument \"self\" is NULL");
		return;
	}

	s_mutex_lock(&self->voice_mutex);
	sync_data_config(self, dataConfig, error);
	s_mutex_unlock(&self->voice_mutex);

	S_CHK_ERR(error, S_CONTERR,
			  "SVoiceSetDataConfig",
			  "Call to \"sync_data_config\" failed");
}


/* data */

S_API SList *SVoiceGetDataKeys(const SVoice *self, s_erc *error)
{
	SList *tmp;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetDataKeys",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	tmp = SMapGetKeys(self->data->dataConfig, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceGetDataKeys",
				  "Call to \"SMapGetKeys\" failed"))
		return NULL;

	return tmp;
}


S_API s_bool SVoiceDataIsPresent(const SVoice *self, const char *name,
								 s_erc *error)
{
	s_bool key_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceDataIsPresent",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceDataIsPresent",
				  "Argument \"name\" is NULL");
		return FALSE;
	}

	key_present = SMapObjectPresent(self->data->dataObjects, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceDataIsPresent",
				  "Call to \"SMapObjectPresent\" failed"))
		return FALSE;

	return key_present;
}


S_API const SObject *SVoiceGetData(const SVoice *self, const char *key, s_erc *error)
{
	const SObject *tmp;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetData",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetData",
				  "Argument \"key\" is NULL");
		return NULL;
	}

	tmp = SMapGetObjectDef(self->data->dataObjects, key, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceGetData",
				  "Call to \"SMapGetObjectDef\" failed"))
		return NULL;

	return tmp;
}


S_API void SVoiceSetData(SVoice *self, const char *key,
						 SObject *object,  s_erc *error)
{
	s_bool key_present;
	SMap *dataObjectMap;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetData",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetData",
				  "Argument \"key\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetData",
				  "Argument \"object\" is NULL");
		return;
	}

	s_mutex_lock(&self->voice_mutex);

	/* check if the data is in the configuration */
	key_present = SMapObjectPresent(self->data->dataConfig, key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceSetData",
				  "Call to \"SMapObjectPresent\" failed"))
	{
		s_mutex_unlock(&self->voice_mutex);
		return;
	}

	if (key_present)
	{
		/* delete the data and config key */
		unload_data_entry(self, self->data->dataConfig, key, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SVoiceSetData",
					  "Call to \"unload_data_entry\" failed"))
		{
			s_mutex_unlock(&self->voice_mutex);
			return;
		}
	}

	/* add data and dummy data object map */
	dataObjectMap = S_MAP(S_NEW("SMapList", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceSetData",
				  "Failed to create new data object map"))
	{
		s_mutex_unlock(&self->voice_mutex);
		return;
	}

	/* add dummy entries */
	SMapSetString(dataObjectMap, "format", "NULL", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceSetData",
				  "Call to \"SMapSetString\" failed"))
	{
		S_DELETE(dataObjectMap, "SVoiceSetData", error);
		s_mutex_unlock(&self->voice_mutex);
		return;
	}

	SMapSetString(dataObjectMap, "plug-in", "NULL", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceSetData",
				  "Call to \"SMapSetString\" failed"))
	{
		S_DELETE(dataObjectMap, "SVoiceSetData", error);
		return;
	}

	SMapSetString(dataObjectMap, "path", "NULL", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceSetData",
				  "Call to \"SMapSetString\" failed"))
	{
		S_DELETE(dataObjectMap, "SVoiceSetData", error);
		s_mutex_unlock(&self->voice_mutex);
		return;
	}

	SMapSetObject(self->data->dataConfig, key, S_OBJECT(dataObjectMap), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceSetData",
				  "Call to \"SMapSetObject\" failed"))
	{
		S_DELETE(dataObjectMap, "SVoiceSetData", error);
		s_mutex_unlock(&self->voice_mutex);
		return;
	}

	SMapSetObject(self->data->dataObjects, key, object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceSetData",
				  "Call to \"SMapSetObject\" failed"))
		SMapObjectDelete(self->data->dataConfig, key, error);

	s_mutex_unlock(&self->voice_mutex);
}


S_API void SVoiceDelData(SVoice *self, const char *key, s_erc *error)
{
	s_bool key_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceDelData",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceDelData",
				  "Argument \"key\" is NULL");
		return;
	}

	/* check if the data is in the configuration */
	s_mutex_lock(&self->voice_mutex);
	key_present = SMapObjectPresent(self->data->dataConfig, key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceDelData",
				  "Call to \"SMapObjectPresent\" failed"))
	{
		s_mutex_unlock(&self->voice_mutex);
		return;
	}

	if (key_present)
	{
		/* delete the data and config key */
		unload_data_entry(self, self->data->dataConfig, key, error);
		S_CHK_ERR(error, S_CONTERR,
				  "SVoiceDelData",
				  "Call to \"unload_data_entry\" failed");
	}

	s_mutex_unlock(&self->voice_mutex);
}


S_API void SVoiceLoadData(SVoice *self, s_erc *error)
{
	SIterator *itr;
	size_t num_data_objects;
	const char *data_name;
	const SObject *loaded;
	const SMap *dataObjectMap;
	s_data_info *data_info;
	const SObject *vcfgObject;
	char *voice_base_path;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceLoadData",
				  "Argument \"self\" is NULL");
		return;
	}

	/* get voice base path */
	vcfgObject = SVoiceGetFeature(self, "config_file", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceLoadData",
				  "Call to \"SVoiceGetFeature\" failed, failed to get voice config file"))
		return;

	voice_base_path = s_get_base_path(SObjectGetString(vcfgObject, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceLoadData",
				  "Call to \"s_get_base_path/SObjectGetString\" failed"))
		return;

	num_data_objects = SMapSize(self->data->dataObjects, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceLoadData",
				  "Call to \"SMapSize\" failed"))
	{
		S_FREE(voice_base_path);
		return;
	}

	if (num_data_objects != 0)
	{
		S_WARNING(S_FAILURE,
				  "SVoiceLoadData",
				  "SVoiceLoadData can only be used when no objects have been loaded");
		S_FREE(voice_base_path);
		return;
	}

	/*
	 * now iterate through data config and load everything
	 */
	itr = S_ITERATOR_GET(self->data->dataConfig, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceLoadData",
				  "Call to \"S_ITERATOR_GET\" failed"))
	{
		S_DELETE(itr, "SVoiceLoadData", error);
		S_FREE(voice_base_path);
		return;
	}

	while (itr)
	{
		char *combined_path;


		data_name = SIteratorKey(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SVoiceLoadData",
					  "Call to \"SIteratorKey\" failed"))
		{
			S_DELETE(itr, "SVoiceLoadData", error);
			S_FREE(voice_base_path);
			return;
		}

		/*
		 * We have already checked this cast in
		 * SVoiceLoadDataConfig (_s_load_voice_data_config)
		 */
		dataObjectMap = (const SMap*)SMapGetObjectDef(self->data->dataConfig, data_name, NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SVoiceLoadData",
					  "Call to \"SMapGetObjectDef\" for data '%s' failed",
					  data_name))
		{
			S_DELETE(itr, "SVoiceLoadData", error);
			S_FREE(voice_base_path);
			return;
		}

		if (dataObjectMap == NULL)
		{
			S_CTX_ERR(error, S_CONTERR,
					  "SVoiceLoadData",
					  "Data object map for data '%s' in data config in NULL",
					  data_name);
			S_DELETE(itr, "SVoiceLoadData", error);
			S_FREE(voice_base_path);
			return;
		}

		data_info = get_data_info(dataObjectMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SVoiceLoadData",
					  "Call to \"get_data_info\" for data '%s' in data config failed",
					  data_name))
		{
			S_DELETE(itr, "SVoiceLoadData", error);
			S_FREE(voice_base_path);
			return;
		}

		/* get data path, the one in the config file may be relative
		 * to the voice base path
		 */
		combined_path = s_path_combine(voice_base_path, data_info->path,
									   error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SVoiceLoadData",
					  "Call to \"s_path_combine\" failed"))
		{
			S_DELETE(itr, "SVoiceLoadData", error);
			S_FREE(voice_base_path);
			return;
		}

		loaded = _s_vm_load_data(data_info->plugin, combined_path,
								 data_info->format, error);
		S_FREE(combined_path);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SVoiceLoadData",
					  "Call to \"_s_vm_load_voice_data\" for data '%s' in data config failed",
					  data_name))
		{
			S_FREE(data_info);
			S_DELETE(itr, "SVoiceLoadData", error);
			S_FREE(voice_base_path);
			return;
		}

		S_FREE(data_info);

		SMapSetObject(self->data->dataObjects, data_name, loaded, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SVoiceLoadData",
					  "Call to \"SMapSetObject\" for data '%s' in data config failed",
					  data_name))
		{
			s_erc local_err = S_SUCCESS;


			S_FREE(voice_base_path);
			S_DELETE(itr, "SVoiceLoadData", error);
			_s_vm_unload_data((SObject*)loaded, &local_err); /* error is already set */
			return;
		}

		itr = SIteratorNext(itr);
	}

	S_FREE(voice_base_path);
}


/* features */

S_API SList *SVoiceGetFeatureKeys(const SVoice *self, s_erc *error)
{
	SList *tmp;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetFeatureKeys",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	tmp = SMapGetKeys(self->features, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceGetFeatureKeys",
				  "Call to \"SMapGetKeys\" failed"))
		return NULL;

	return tmp;
}


S_API s_bool SVoiceFeatureIsPresent(const SVoice *self, const char *name,
								 s_erc *error)
{
	s_bool key_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceFeatureIsPresent",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceFeatureIsPresent",
				  "Argument \"name\" is NULL");
		return FALSE;
	}

	key_present = SMapObjectPresent(self->features, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceFeatureIsPresent",
				  "Call to \"SMapObjectPresent\" failed"))
		return FALSE;

	return key_present;
}


S_API const SObject *SVoiceGetFeature(const SVoice *self, const char *key, s_erc *error)
{
	const SObject *tmp;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetFeature",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetFeature",
				  "Argument \"key\" is NULL");
		return NULL;
	}

	tmp = SMapGetObjectDef(self->features, key, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceGetFeature",
				  "Call to \"SMapGetObjectDef\" failed"))
		return NULL;

	return tmp;
}


S_API void SVoiceSetFeature(SVoice *self, const char *key,
							SObject *object,  s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetFeature",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetFeature",
				  "Argument \"key\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetFeature",
				  "Argument \"object\" is NULL");
		return;
	}

	s_mutex_lock((s_mutex*)&(self->voice_mutex));
	SMapSetObject(self->features, key, object, error);
	s_mutex_unlock((s_mutex*)&(self->voice_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "SVoiceSetFeature",
			  "Call to \"SMapSetObject\" failed");
}


S_LOCAL void _SVoiceSetFeature_no_lock(SVoice *self, const char *key,
									   SObject *object,  s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "_SVoiceSetFeature_no_lock",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "_SVoiceSetFeature_no_lock",
				  "Argument \"key\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "_SVoiceSetFeature_no_lock",
				  "Argument \"object\" is NULL");
		return;
	}

	SMapSetObject(self->features, key, object, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_SVoiceSetFeature_no_lock",
			  "Call to \"SMapSetObject\" failed");
}


S_API void SVoiceDelFeature(SVoice *self, const char *key, s_erc *error)
{
	s_bool key_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceDelFeature",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceDelFeature",
				  "Argument \"key\" is NULL");
		return;
	}

	s_mutex_lock((s_mutex*)&self->voice_mutex);
	key_present = SMapObjectPresent(self->features, key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceDelFeature",
				  "Call to \"SMapObjectPresent\" failed"))
	{
		s_mutex_unlock((s_mutex*)&self->voice_mutex);
		return;
	}

	if (key_present)
		SMapObjectDelete(self->features, key, error);

	s_mutex_unlock((s_mutex*)&self->voice_mutex);
	S_CHK_ERR(error, S_CONTERR,
			  "SVoiceDelFeature",
			  "Call to \"SMapObjectDelete\" failed");
}


/* feature processors */

S_API SList *SVoiceGetFeatProcKeys(const SVoice *self, s_erc *error)
{
	SList *tmp;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetFeatProcKeys",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	tmp = SMapGetKeys(self->featProcessors, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceGetFeatProcKeys",
				  "Call to \"SMapGetKeys\" failed"))
		return NULL;

	return tmp;
}


S_API s_bool SVoiceFeatProcIsPresent(const SVoice *self, const char *name,
									s_erc *error)
{
	s_bool key_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceFeatProcIsPresent",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceFeatProcIsPresent",
				  "Argument \"name\" is NULL");
		return FALSE;
	}

	key_present = SMapObjectPresent(self->featProcessors, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceFeatProcIsPresent",
				  "Call to \"SMapObjectPresent\" failed"))
		return FALSE;

	return key_present;
}


S_API const SFeatProcessor *SVoiceGetFeatProc(const SVoice *self, const char *key,
											  s_erc *error)
{
	const SFeatProcessor *tmp;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetFeatProc",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetFeatProc",
				  "Argument \"key\" is NULL");
		return NULL;
	}

	tmp = S_FEATPROCESSOR(SMapGetObjectDef(self->featProcessors, key, NULL, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceGetFeatProc",
				  "Call to \"SMapGetObjectDef\" failed"))
		return NULL;

	return tmp;
}


S_API void SVoiceSetFeatProc(SVoice *self, const char *key,
							 SFeatProcessor *featProc,  s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetFeatProc",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetFeatProc",
				  "Argument \"key\" is NULL");
		return;
	}

	if (featProc == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetFeatProc",
				  "Argument \"featProc\" is NULL");
		return;
	}

	s_mutex_lock(&self->voice_mutex);
	SMapSetObject(self->featProcessors, key, S_OBJECT(featProc), error);
	s_mutex_unlock(&self->voice_mutex);

	S_CHK_ERR(error, S_CONTERR,
			  "SVoiceSetFeatProc",
			  "Call to \"SMapSetObject\" failed");
}


S_API void SVoiceDelFeatProc(SVoice *self, const char *key, s_erc *error)
{
	s_bool key_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceDelFeatProc",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceDelFeatProc",
				  "Argument \"key\" is NULL");
		return;
	}

	s_mutex_lock(&self->voice_mutex);
	key_present = SMapObjectPresent(self->featProcessors, key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceDelFeatProc",
				  "Call to \"SMapObjectPresent\" failed"))
	{
		s_mutex_unlock(&self->voice_mutex);
		return;
	}

	if (key_present)
		SMapObjectDelete(self->featProcessors, key, error);

	s_mutex_unlock(&self->voice_mutex);
	S_CHK_ERR(error, S_CONTERR,
			  "SVoiceDelFeatProc",
			  "Call to \"SMapObjectDelete\" failed");
}


/* utterance processors */

S_API SList *SVoiceGetUttProcKeys(const SVoice *self, s_erc *error)
{
	SList *tmp;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetUttProcKeys",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	tmp = SMapGetKeys(self->uttProcessors, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceGetUttProcKeys",
				  "Call to \"SMapGetKeys\" failed"))
		return NULL;

	return tmp;
}


S_API s_bool SVoiceUttProcIsPresent(const SVoice *self, const char *name,
									s_erc *error)
{
	s_bool key_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceUttProcIsPresent",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceUttProcIsPresent",
				  "Argument \"name\" is NULL");
		return FALSE;
	}

	key_present = SMapObjectPresent(self->uttProcessors, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceUttProcIsPresent",
				  "Call to \"SMapObjectPresent\" failed"))
		return FALSE;

	return key_present;
}


S_API const SUttProcessor *SVoiceGetUttProc(const SVoice *self, const char *key,
											s_erc *error)
{
	const SUttProcessor *tmp;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetUttProc",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetUttProc",
				  "Argument \"key\" is NULL");
		return NULL;
	}

	tmp = S_UTTPROCESSOR(SMapGetObjectDef(self->uttProcessors, key, NULL, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceGetUttProc",
				  "Call to \"SMapGetObjectDef\" failed"))
		return NULL;

	return tmp;
}


S_API void SVoiceSetUttProc(SVoice *self, const char *key,
							SUttProcessor *uttProc,  s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetUttProc",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetUttProc",
				  "Argument \"key\" is NULL");
		return;
	}

	if (uttProc == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetUttProc",
				  "Argument \"uttProc\" is NULL");
		return;
	}

	s_mutex_lock(&self->voice_mutex);
	SMapSetObject(self->uttProcessors, key, S_OBJECT(uttProc), error);
	s_mutex_unlock(&self->voice_mutex);

	S_CHK_ERR(error, S_CONTERR,
			  "SVoiceSetUttProc",
			  "Call to \"SMapSetObject\" failed");
}


S_API void SVoiceDelUttProc(SVoice *self, const char *key, s_erc *error)
{
	s_bool key_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceDelUttProc",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceDelUttProc",
				  "Argument \"key\" is NULL");
		return;
	}

	s_mutex_lock(&self->voice_mutex);
	key_present = SMapObjectPresent(self->uttProcessors, key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceDelUttProc",
				  "Call to \"SMapObjectPresent\" failed"))
	{
		s_mutex_unlock(&self->voice_mutex);
		return;
	}

	if (key_present)
		SMapObjectDelete(self->uttProcessors, key, error);

	s_mutex_unlock(&self->voice_mutex);
	S_CHK_ERR(error, S_CONTERR,
			  "SVoiceDelUttProc",
			  "Call to \"SMapObjectDelete\" failed");
}


/* utterance types */

S_API SList *SVoiceGetUttTypesKeys(const SVoice *self, s_erc *error)
{
	SList *tmp;

	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetUttTypeKeys",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	tmp = SMapGetKeys(self->uttTypes, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceGetUttTypeKeys",
				  "Call to \"SMapGetKeys\" failed"))
		return NULL;

	return tmp;
}


S_API s_bool SVoiceUttTypeIsPresent(const SVoice *self, const char *name,
									s_erc *error)
{
	s_bool key_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceUttTypeIsPresent",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceUttTypeIsPresent",
				  "Argument \"name\" is NULL");
		return FALSE;
	}

	key_present = SMapObjectPresent(self->uttTypes, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceUttTypeIsPresent",
				  "Call to \"SMapObjectPresent\" failed"))
		return FALSE;

	return key_present;
}


S_API const SList *SVoiceGetUttType(const SVoice *self, const char *key,
									s_erc *error)
{
	const SList *tmp;

	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetUttType",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceGetUttType",
				  "Argument \"key\" is NULL");
		return NULL;
	}

	tmp = S_LIST(SMapGetObjectDef(self->uttTypes, key, NULL, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceGetUttType",
				  "Call to \"SMapGetObjectDef\" failed"))
		return NULL;

	return tmp;
}


S_API void SVoiceSetUttType(SVoice *self, const char *key,
							SList *uttType,  s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetUttType",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetUttType",
				  "Argument \"key\" is NULL");
		return;
	}

	if (uttType == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceSetUttType",
				  "Argument \"uttType\" is NULL");
		return;
	}

	s_mutex_lock(&self->voice_mutex);
	SMapSetObject(self->uttTypes, key, S_OBJECT(uttType), error);
	s_mutex_unlock(&self->voice_mutex);

	S_CHK_ERR(error, S_CONTERR,
			  "SVoiceSetUttType",
			  "Call to \"SMapSetObject\" failed");
}


S_API void SVoiceDelUttType(SVoice *self, const char *key, s_erc *error)
{
	s_bool key_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceDelUttType",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceDelUttType",
				  "Argument \"key\" is NULL");
		return;
	}

	s_mutex_lock(&self->voice_mutex);
	key_present = SMapObjectPresent(self->features, key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SVoiceDelUttType",
				  "Call to \"SMapObjectPresent\" failed"))
	{
		s_mutex_unlock(&self->voice_mutex);
		return;
	}

	if (key_present)
		SMapObjectDelete(self->uttTypes, key, error);

	s_mutex_unlock(&self->voice_mutex);
	S_CHK_ERR(error, S_CONTERR,
			  "SVoiceDelUttType",
			  "Call to \"SMapObjectDelete\" failed");
}


S_LOCAL void SVoiceLoadDataConfig(SVoice *self, const SMap *voiceConfig,
								  s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceLoadDataConfig",
				  "Argument \"self\" is NULL");
		return;
	}

	if (voiceConfig == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SVoiceLoadDataConfig",
				  "Argument \"voiceConfig\" is NULL");
		return;
	}

	if (self->data->dataConfig != NULL)
	{
		S_WARNING(S_FAILURE,
				  "SVoiceLoadDataConfig",
				  "Voice configuration already loaded, ignoring request");
		return;
	}

	self->data->dataConfig = _s_load_voice_data_config(voiceConfig, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SVoiceLoadDataConfig",
			  "Call to \"_s_load_voice_data_config\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_voice_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&VoiceClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_voice_class_add",
			  "Failed to add SVoiceClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void sync_data_config(SVoice *self, SMap *dataConfig, s_erc *error)
{
	SMap *newDataConfig;
	SIterator *itr;
	s_bool key_present;
	s_bool entries_match;
	const char *data_name;


	S_CLR_ERR(error);
	newDataConfig = S_MAP(S_NEW("SMapList", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "sync_data_config",
				  "Failed to create a new data config map object"))
		return;

	/* iterate through given map */
	itr = S_ITERATOR_GET(dataConfig, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "sync_data_config",
				  "Call to \"S_ITERATOR_GET\" failed"))
	{
		S_DELETE(newDataConfig, "sync_data_config", error);
		return;
	}

	while (itr)
	{
		data_name = SIteratorKey(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "sync_data_config",
					  "Call to \"SIteratorKey\" failed"))
		{
			S_DELETE(itr, "sync_data_config", error);
			S_DELETE(newDataConfig, "sync_data_config", error);
			return;
		}

		/* check if data_name is in current dataConfig */
		key_present = SMapObjectPresent(self->data->dataConfig, data_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "sync_data_config",
					  "Call to \"SMapObjectPresent\" failed"))
		{
			S_DELETE(itr, "sync_data_config", error);
			S_DELETE(newDataConfig, "sync_data_config", error);
			return;
		}

		if (key_present)
		{
			/*
			 * compare the entries of data_name of current dataConfig
			 * and given dataConfig
			 */
			entries_match = compare_data_config_entries(dataConfig,
														self->data->dataConfig,
														data_name,
														error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "sync_data_config",
						  "Call to \"compare_data_config_entries\" for data name '%s' failed",
						  data_name))
			{
				S_DELETE(itr, "sync_data_config", error);
				S_DELETE(newDataConfig, "sync_data_config", error);
				return;
			}

			if (!entries_match)
			{
				const SObject *loaded;


				/* load new entry */
				loaded = load_data_entry(newDataConfig, dataConfig, data_name, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "sync_data_config",
							  "Call to \"load_data_entry\" for data name '%s' failed",
							  data_name))
				{
					S_DELETE(itr, "sync_data_config", error);
					S_DELETE(newDataConfig, "sync_data_config", error);
					return;
				}

				/* unload old one */
				unload_data_entry(self, self->data->dataConfig, data_name, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "sync_data_config",
							  "Call to \"unload_data_entry\" for data name '%s' failed",
							  data_name))
				{
					S_DELETE(itr, "sync_data_config", error);
					S_DELETE(newDataConfig, "sync_data_config", error);
					return;
				}

				/* add loaded to dataObjects map */
				SMapSetObject(self->data->dataObjects, data_name, loaded, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "sync_data_config",
							  "Call to \"SMapSetObject\" for data name '%s' failed",
							  data_name))
				{
					S_DELETE(itr, "sync_data_config", error);
					S_DELETE(newDataConfig, "sync_data_config", error);
					return;
				}
			}
		}
		else
		{
			const SObject *loaded;


			/* load new entry */
			loaded = load_data_entry(newDataConfig, dataConfig, data_name, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "sync_data_config",
						  "Call to \"load_data_entry\" for data name '%s' failed",
						  data_name))
			{
				S_DELETE(itr, "sync_data_config", error);
				S_DELETE(newDataConfig, "sync_data_config", error);
				return;
			}

			/* add loaded to dataObjects map */
			SMapSetObject(self->data->dataObjects, data_name, loaded, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "sync_data_config",
						  "Call to \"SMapSetObject\" for data name '%s' failed",
						  data_name))
			{
				S_DELETE(itr, "sync_data_config", error);
				S_DELETE(newDataConfig, "sync_data_config", error);
				return;
			}
		}

		itr = SIteratorNext(itr);
	}

	/*
	 * now iterate through current data config and delete everything
	 * that is left over.
	 */
	itr = S_ITERATOR_GET(self->data->dataConfig, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "sync_data_config",
				  "Call to \"S_ITERATOR_GET\" failed"))
	{
		S_DELETE(newDataConfig, "sync_data_config", error);
		return;
	}

	while (itr)
	{
		SObject *toUnload;


		data_name = SIteratorKey(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "sync_data_config",
					  "Call to \"SIteratorKey\" failed"))
		{
			S_DELETE(itr, "sync_data_config", error);
			S_DELETE(newDataConfig, "sync_data_config", error);
			return;
		}

		/* unload data */
		toUnload = SMapObjectUnlink(self->data->dataObjects, data_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "sync_data_config",
					  "Call to \"SMapObjectUnlink\" for data '%s' failed",
					  data_name))
		{
			S_DELETE(itr, "sync_data_config", error);
			S_DELETE(newDataConfig, "sync_data_config", error);
			return;
		}

		_s_vm_unload_data(toUnload, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "sync_data_config",
					  "Call to \"_s_vm_unload_data\" for data '%s' failed",
					  data_name))
		{
			S_DELETE(itr, "sync_data_config", error);
			S_DELETE(newDataConfig, "sync_data_config", error);
			return;
		}

		itr = SIteratorNext(itr);
	}

	/* delete old config */
	S_DELETE(self->data->dataConfig, "sync_data_config", error);

	/* set new one */
	self->data->dataConfig = newDataConfig;

	/* delete the given one */
	S_DELETE(dataConfig, "sync_data_config", error);
}


static s_bool compare_data_config_entries(const SMap *newDataConfig,
										  const SMap *currentDataConfig,
										  const char *data_name,
										  s_erc *error)
{
	const SObject *tmp;
	const SMap *newDataObjectMap;
	const SMap *currentDataObjectMap;
	s_data_info *new_data_info;
	s_data_info *current_data_info;
	s_bool entries_match;


	S_CLR_ERR(error);

	tmp = SMapGetObject(newDataConfig, data_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "compare_data_config_entries",
				  "Call to \"SMapGetObject\" for new data config map failed"))
		return FALSE;

	newDataObjectMap = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "compare_data_config_entries",
				  "Call to \"S_CAST (SMap)\" for new data config map failed"))
		return FALSE;

	/* We put this one in, so unsafe cast */
	currentDataObjectMap = (const SMap*)SMapGetObjectDef(currentDataConfig, data_name, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "compare_data_config_entries",
				  "Call to \"SMapGetObjectDef\" for current data config map failed"))
		return FALSE;

	if (currentDataObjectMap == NULL)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "compare_data_config_entries",
				  "Current data object map is NULL");
		return FALSE;
	}

	new_data_info = get_data_info(newDataObjectMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "compare_data_config_entries",
				  "Call to \"get_data_info\" for new data config map failed"))
		return FALSE;

	current_data_info = get_data_info(currentDataObjectMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "compare_data_config_entries",
				  "Call to \"get_data_info\" for current data config map failed"))
	{
		S_FREE(new_data_info);
		return FALSE;
	}

	entries_match = compare_data_info(new_data_info, current_data_info, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "compare_data_config_entries",
				  "Call to \"compare_data_info\" failed"))
	{
		S_FREE(new_data_info);
		S_FREE(current_data_info);
		return FALSE;
	}

	S_FREE(new_data_info);
	S_FREE(current_data_info);

	return entries_match;
}


static s_data_info *get_data_info(const SMap *map, s_erc *error)
{
	s_data_info *data_info;
	const char *value;


	S_CLR_ERR(error);

	data_info = S_MALLOC(s_data_info, 1);
	if (data_info == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "get_data_info",
				  "Failed to allocate memory for 's_data_info' object");
		return NULL;
	}

	/* format */
	value = SMapGetStringDef(map, "format", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_data_info",
				  "Call to \"SMapGetStringDef\" of key 'format' failed"))
	{
		S_FREE(data_info);
		return NULL;
	}

	if (value == NULL)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "get_data_info",
				  "Value of key 'format' is NULL");
		S_FREE(data_info);
		return NULL;
	}

	data_info->format = value;

	/* plug-in */
	value = SMapGetStringDef(map, "plug-in", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_data_info",
				  "Call to \"SMapGetStringDef\" of key 'plug-in' failed"))
	{
		S_FREE(data_info);
		return NULL;
	}

	if (value == NULL)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "get_data_info",
				  "Value of key 'plug-in' is NULL");
		S_FREE(data_info);
		return NULL;
	}

	data_info->plugin = value;

	/* path */
	value = SMapGetStringDef(map, "path", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_data_info",
				  "Call to \"SMapGetStringDef\" of key 'path' failed"))
	{
		S_FREE(data_info);
		return NULL;
	}

	if (value == NULL)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "get_data_info",
				  "Value of key 'path' is NULL");
		S_FREE(data_info);
		return NULL;
	}

	data_info->path = value;

	return data_info;
}


static s_bool compare_data_info(const s_data_info *d1, const s_data_info *d2,
								s_erc *error)
{
	int rv;

	S_CLR_ERR(error);

	/* format */
	rv = s_strcmp(d1->format, d2->format, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "compare_data_info",
				  "Call to \"s_strcmp\" for 'format' values '%s' and '%s' failed",
				  d1->format, d2->format))
		return FALSE;

	if (rv != 0)
		return FALSE;

	/* plug-in */
	rv = s_strcmp(d1->plugin, d2->plugin, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "compare_data_info",
				  "Call to \"s_strcmp\" for 'plug-in' values '%s' and '%s' failed",
				  d1->plugin, d2->plugin))
		return FALSE;

	if (rv != 0)
		return FALSE;

	/* path */
	rv = s_strcmp(d1->path, d2->path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "compare_data_info",
				  "Call to \"s_strcmp\" for 'path' values '%s' and '%s' failed",
				  d1->path, d2->path))
		return FALSE;

	if (rv != 0)
		return FALSE;

	return TRUE;
}


static const SObject *load_data_entry(SMap *newDataConfig, const SMap *dataConfig,
									  const char *data_name, s_erc *error)
{
	s_data_info *data_info;
	const SMap *newDataObjectMap;
	const SObject *loaded;


	S_CLR_ERR(error);

	/* We have already checked this cast in compare_data_config_entries */
	newDataObjectMap = (const SMap*)SMapGetObjectDef(dataConfig, data_name, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_data_entry",
				  "Call to \"SMapGetObjectDef\" for current data config map failed"))
		return NULL;

	if (newDataObjectMap == NULL)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "load_data_entry",
				  "Current data object map is NULL");
		return NULL;
	}

	data_info = get_data_info(newDataObjectMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_data_entry",
				  "Call to \"get_data_info\" for current data object map failed"))
		return NULL;

	loaded = _s_vm_load_data(data_info->plugin, data_info->path,
							 data_info->format, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_data_entry",
				  "Call to \"_s_vm_load_voice_data\" failed"))
	{
		S_FREE(data_info);
		return NULL;
	}

	SMapSetObject(newDataConfig, data_name, S_OBJECT(newDataObjectMap), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "load_data_entry",
				  "Call to \"SMapSetObject\" failed"))
	{
		s_erc local_err = S_SUCCESS;


		S_FREE(data_info);
		_s_vm_unload_data(S_OBJECT(loaded), &local_err); /* error is already set */
		return NULL;
	}


	return loaded;
}


static void unload_data_entry(SVoice *self, SMap *dataConfig, const char *data_name,
							  s_erc *error)
{
	SObject *toUnload;
	s_bool data_loaded;


	S_CLR_ERR(error);

	toUnload = SMapObjectUnlink(self->data->dataObjects, data_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "unload_data_entry",
				  "Call to \"SMapObjectUnlink\" failed"))
		return;

	/*
	 * check if the data object was loaded with
	 * the VoiceManager.
	 */
	data_loaded = _s_vm_data_loaded(toUnload, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "unload_data_entry",
				  "Call to \"_s_vm_data_loaded\" failed"))
		return;

	if (data_loaded)
	{
		_s_vm_unload_data(toUnload, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "unload_data_entry",
					  "Call to \"_s_vm_unload_data\" failed"))
			return;
	}
	else
	{
		/*
		 * just do a delete, the VoiceManager does not have a handle
		 * on this data object
		 */
		S_DELETE(toUnload, "unload_data_entry", error);
	}

	/* remove entry from data config */
	SMapObjectDelete(dataConfig, data_name, error);
	S_CHK_ERR(error, S_CONTERR,
			  "unload_data_entry",
			  "Call to \"SMapObjectDelete\" failed");
}


static void free_voice_info(s_voice_info *info)
{
	if (info != NULL)
	{
		if (info->name != NULL)
			S_FREE(info->name);

		if (info->description != NULL)
			S_FREE(info->description);

		if (info->gender != NULL)
			S_FREE(info->gender);

		if (info->language != NULL)
			S_FREE(info->language);

		if (info->lang_code != NULL)
			S_FREE(info->lang_code);

		S_FREE(info);
	}
}


static void unload_voice_plugins(SList *plugins, s_erc *error)
{
	SIterator *itr;
	SObject *plugin;


	S_CLR_ERR(error);

	itr = S_ITERATOR_GET(plugins, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "unload_voice_plugins",
				  "Call to \"S_ITERATOR_GET\" failed"))
		return;

	while (itr != NULL)
	{
		plugin = SIteratorUnlink(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "unload_voice_plugins",
					  "Call to \"SIteratorUnlink\" failed"))
		{
			S_DELETE(itr, "unload_voice_plugins", error);
			return;
		}


		S_DELETE(plugin, "unload_voice_plugins", error);
		itr = SIteratorNext(itr);
	}

	S_DELETE(plugins, "unload_voice_plugins", error);
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitVoice(void *obj, s_erc *error)
{
	SVoice *self = obj;


	S_CLR_ERR(error);

	self->info = NULL;
	self->plugins =  NULL;
	self->features = NULL;
	self->featProcessors = NULL;
	self->uttProcessors = NULL;
	self->uttTypes = NULL;

	self->data = S_CALLOC(s_voice_data, 1);
	if (self->data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "InitVoice",
				  "Failed to allocate memory for 's_voice_data' object");
		return;
	}

	self->data->dataObjects = S_MAP(S_NEW("SMapList", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "InitVoice",
				  "Failed to create new data objects map"))
		return;

	s_mutex_init(&self->voice_mutex);
}


static void DestroyVoice(void *obj, s_erc *error)
{
	SVoice *self = obj;
	SIterator *itr;
	const char *data_name;


	S_CLR_ERR(error);
	s_mutex_lock(&self->voice_mutex);

	free_voice_info(self->info);

	if (self->features != NULL)
		S_DELETE(self->features, "DestroyVoice", error);

	if (self->featProcessors != NULL)
		S_DELETE(self->featProcessors, "DestroyVoice", error);

	if (self->uttProcessors != NULL)
		S_DELETE(self->uttProcessors, "DestroyVoice", error);

	if (self->uttTypes != NULL)
		S_DELETE(self->uttTypes, "DestroyVoice", error);

	if (self->plugins != NULL)
	{
		unload_voice_plugins(self->plugins, error);
		S_CHK_ERR(error, S_CONTERR,
				  "DestroyVoice",
				  "Call to \"unload_voice_plugins\" failed");
	}

	if (self->data != NULL)
	{
		/*
		 * now iterate through data config and delete everything
		 */
		itr = S_ITERATOR_GET(self->data->dataConfig, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "DestroyVoice",
					  "Call to \"S_ITERATOR_GET\" failed"))
		{
			S_DELETE(itr, "DestroyVoice", error);
			S_DELETE(self->data->dataConfig, "DestroyVoice", error);

			/*
			 * leave data objects, this will probably cause
			 * memory leaks, but at the risk of deleting
			 * shared data.
			 */
			S_FREE(self->data);
			s_mutex_unlock(&self->voice_mutex);
			s_mutex_destroy(&self->voice_mutex);
			return;
		}

		while (itr)
		{
			data_name = SIteratorKey(itr, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "DestroyVoice",
						  "Call to \"SIteratorKey\" failed"))
			{
				S_DELETE(itr, "DestroyVoice", error);
				S_DELETE(self->data->dataConfig, "DestroyVoice", error);

				/*
				 * leave data objects, this will probably cause
				 * memory leaks, but at the risk of deleting
				 * shared data.
				 */
				S_FREE(self->data);
				s_mutex_unlock(&self->voice_mutex);
				s_mutex_destroy(&self->voice_mutex);
				return;
			}

			unload_data_entry(self, self->data->dataConfig, data_name, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "DestroyVoice",
						  "Call to \"unload_data_entry\" failed"))
			{
				S_DELETE(itr, "DestroyVoice", error);
				S_DELETE(self->data->dataConfig, "DestroyVoice", error);

				/*
				 * leave data objects, this will probably cause
				 * memory leaks, but at the risk of deleting
				 * shared data.
				 */
				S_FREE(self->data);
				s_mutex_unlock(&self->voice_mutex);
				s_mutex_destroy(&self->voice_mutex);
				return;
			}

			itr = SIteratorNext(itr);
		}

		S_DELETE(self->data->dataConfig, "DestroyVoice", error);
		S_DELETE(self->data->dataObjects, "DestroyVoice", error);
		S_FREE(self->data);
	}

	s_mutex_unlock(&self->voice_mutex);
	s_mutex_destroy(&self->voice_mutex);
}


static void DisposeVoice(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static SUtterance *SynthUtt(const SVoice *self, const char *utt_type,
							SObject *input, s_erc *error)
{
	const SList *uttType;
	const SUttProcessor *uttProc; /* utterance processor */
	const char *utt_processor_name;
	SUtterance *utt;
	SIterator *itr;


	S_CLR_ERR(error);

	uttType = SVoiceGetUttType(self, utt_type, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SynthUtt",
				  "Call to \"SVoiceGetUttType\" failed"))
		return NULL;

	/* create new utterance */
	utt = (SUtterance*)S_NEW("SUtterance", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SynthUtt",
				  "Failed to create new utterance"))
		return NULL;

	/* initialize utterance */
	SUtteranceInit(&utt, self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SynthUtt",
				  "Failed to initialize new utterance"))
		return NULL;

	SUtteranceSetFeature(utt, "input", input, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SynthUtt",
				  "Failed to set utterance \'input\' feature"))
	{
		S_DELETE(utt, "SynthUtt", error);
		return NULL;
	}

	SUtteranceSetFeature(utt, "utterance-type",
						 SObjectSetString(utt_type, error),
						 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SynthUtt",
				  "Failed to set utterance \'utterance-type\' feature"))
	{
		S_DELETE(utt, "SynthUtt", error);
		return NULL;
	}

	/* run utterance processors on utterance */
	itr = S_ITERATOR_GET(uttType, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SynthUtt",
				  "Call to \"S_ITERATOR_GET\" failed"))
	{
		S_DELETE(utt, "SynthUtt", error);
		return NULL;
	}

	for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
	{
		/* get utterance processor name */
		utt_processor_name = SObjectGetString(SIteratorObject(itr, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SynthUtt",
					  "Failed to get utterance processor name"))
		{
			S_DELETE(itr, "SynthUtt", error);
			return utt;
		}

		uttProc = S_UTTPROCESSOR(SMapGetObjectDef(self->uttProcessors, utt_processor_name,
												  NULL, error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "SynthUtt",
					  "Call to \"SMapGetObjectDef\" failed"))
		{
			S_DELETE(itr, "SynthUtt", error);
			return utt;
		}

		if (uttProc == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "SynthUtt",
					  "Utterance processor \'%s\' not defined",
					  utt_processor_name);
			S_DELETE(itr, "SynthUtt", error);
			return utt;
		}

		S_DEBUG(S_DBG_INFO,
				"executing \'%s\' utterance processor ...",
				utt_processor_name);

		SUttProcessorRun(uttProc, utt, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SynthUtt",
					  "Execution of utterance processor \'%s\' failed",
					  utt_processor_name))
		{
			S_DELETE(itr, "SynthUtt", error);
			return utt;
		}
	}

	return utt;
}


static void ReSynthUtt(const SVoice *self, const char *utt_type,
					   SUtterance *utt, s_erc *error)
{
	const SList *uttType;
	const SUttProcessor *uttProc; /* utterance processor */
	const char *utt_processor_name;
	SIterator *itr;


	S_CLR_ERR(error);

	uttType = SVoiceGetUttType(self, utt_type, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ReSynthUtt",
				  "Call to \"SVoiceGetUttType\" failed"))
		return;

	SUtteranceSetFeature(utt, "utterance-type",
						 SObjectSetString(utt_type, error),
						 error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ReSynthUtt",
				  "Failed to set utterance \'utterance-type\' feature"))
		return;

	/* run utterance processors on utterance */
	itr = S_ITERATOR_GET(uttType, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ReSynthUtt",
				  "Call to \"S_ITERATOR_GET\" failed"))
		return;

	for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
	{
		/* get utterance processor name */
		utt_processor_name = SObjectGetString(SIteratorObject(itr, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ReSynthUtt",
					  "Failed to get utterance processor name"))
		{
			S_DELETE(itr, "ReSynthUtt", error);
			return;
		}

		uttProc = S_UTTPROCESSOR(SMapGetObjectDef(self->uttProcessors, utt_processor_name,
												  NULL, error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "ReSynthUtt",
					  "Call to \"SMapGetObjectDef\" failed"))
		{
			S_DELETE(itr, "ReSynthUtt", error);
			return;
		}

		if (uttProc == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ReSynthUtt",
					  "Utterance processor \'%s\' not defined",
					  utt_processor_name);
			S_DELETE(itr, "ReSynthUtt", error);
			return;
		}

		S_DEBUG(S_DBG_INFO,
				"executing \'%s\' utterance processor ...",
				utt_processor_name);

		SUttProcessorRun(uttProc, utt, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ReSynthUtt",
					  "Execution of utterance processor \'%s\' failed",
					  utt_processor_name))
		{
			S_DELETE(itr, "ReSynthUtt", error);
			return;
		}
	}
}


/************************************************************************************/
/*                                                                                  */
/* SVoice class initialization                                                      */
/*                                                                                  */
/************************************************************************************/

static SVoiceClass VoiceClass =
{
	/* SObjectClass */
	{
		"SVoice",
		sizeof(SVoice),
		{ 0, 1},
		InitVoice,     /* init    */
		DestroyVoice,  /* destroy */
		DisposeVoice,  /* dispose */
		NULL,          /* compare */
		NULL,          /* print   */
		NULL,          /* copy    */
	},
	SynthUtt,          /* synth_utt    */
	ReSynthUtt         /* re_synth_utt */
};



