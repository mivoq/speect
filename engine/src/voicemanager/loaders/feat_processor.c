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
/* A loader for the voice feature processors in the voice configuration file.       */
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
#include "voicemanager/loaders/features.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SMap *_s_load_voice_feature_processors(const SMap *voiceConfig,
											   SList *voicePlugins,
											   s_erc *error)
{
	SMap *featProcessors;
	const SObject *tmp;
	const SMap *voiceConfigFeatProcessors;
	s_bool key_present;
	SIterator *itr;
	const char *featproc_name;
	const char *featproc_class;
	const char *featproc_plugin;
	const SMap *featProcInfo;
	const SMap *featProcFeat;
	SFeatProcessor *fProcessor;
	SPlugin *plugin;
	SList *featProcPlugins;


	S_CLR_ERR(error);

	/* create a map container for feature-processors */
	featProcessors = S_MAP(S_NEW(SMapList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_feature_processors",
				  "Failed to create new map for voice feature-processors"))
		return NULL;

	/* look for "feature-processors" key in voiceConfig map */
	key_present = SMapObjectPresent(voiceConfig, "feature-processors", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_feature_processors",
				  "Call to \"SMapObjectPresent\" failed for \'feature-processors\' key"))
	{
		S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
		return NULL;
	}

	if (!key_present)
	{
		/* no defined feature-processors  */
		return featProcessors;
	}

	/* get feature-processors from voiceConfig */
	tmp = SMapGetObject(voiceConfig, "feature-processors", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_feature_processors",
				  "Call to \"SMapGetObject\" failed"))
	{
		S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
		return NULL;
	}

	voiceConfigFeatProcessors = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_feature_processors",
				  "Call to \"S_CAST (SMap)\" failed for \'feature-processors\' object"))
	{
		S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
		return NULL;
	}

	/*
	 * iterate through the voiceConfigFeatProcessors, create the
	 * feature-processors and add their features (if any), and add
	 * them to the featProcessors map
	 */
	itr = S_ITERATOR_GET(voiceConfigFeatProcessors, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_feature_processors",
				  "Call to \"S_ITERATOR_GET\" failed"))
	{
		S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
		return NULL;
	}

	if (itr)
	{
		/* create a plug-in list */
		featProcPlugins = S_LIST(S_NEW(SListList, error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_feature_processors",
					  "Failed to create new list for voice feature-processor plug-ins"))
		{
			S_DELETE(itr, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
			return NULL;
		}
	}
	else
		featProcPlugins = NULL;

	while (itr)
	{
		/* the feature-processor name */
		featproc_name = SIteratorKey(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_feature_processors",
					  "Call to \"SIteratorKey\" failed"))
		{
			S_DELETE(itr, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
			return NULL;
		}

		/* the feature-processor info (SMap) */
		tmp = SIteratorObject(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_feature_processors",
					  "Call to \"SIteratorObject\" failed"))
		{
			S_DELETE(itr, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
			return NULL;
		}

		featProcInfo = S_CAST(tmp, SMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_feature_processors",
					  "Call to \"S_CAST (SMap)\" failed for feature-processor \'%s\'",
					  featproc_name))
		{
			S_DELETE(itr, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
			return NULL;
		}

		/*
		 * featProcInfo must have "class" and "plug-in" and optionally "features"
		 */
		featproc_class = SMapGetString(featProcInfo, "class", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_feature_processors",
					  "Call to \"SMapGetString\" failed for \'class\' name of feature-processor \'%s\'",
					  featproc_name))
		{
			S_DELETE(itr, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
			return NULL;
		}

		featproc_plugin = SMapGetString(featProcInfo, "plug-in", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_feature_processors",
					  "Call to \"SMapGetString\" failed for \'plug-in\' name of feature-processor \'%s\'",
					  featproc_name))
		{
			S_DELETE(itr, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
			return NULL;
		}

		/*
		 * load the plug-in, add it to the list of feature-processor plug-ins and create
		 * the feature-processor object
		 */
		plugin = s_pm_load_plugin(featproc_plugin, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_feature_processors",
					  "Call to \"s_pm_load_plugin\" failed for feature-processor \'%s\'",
					  featproc_name))
		{
			S_DELETE(itr, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
			return NULL;
		}

		SListPush(featProcPlugins, S_OBJECT(plugin), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_feature_processors",
					  "Call to \"SListPush\" failed for feature-processor plug-ins"))
		{
			S_DELETE(plugin, "_s_load_voice_feature_processors", error);
			S_DELETE(itr, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
			return NULL;
		}

		fProcessor = S_FEATPROCESSOR(S_NEW_FROM_NAME(featproc_class, error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_feature_processors",
					  "Failed to create new feature-processor of class \'%s\'",
					  featproc_class))
		{
			S_DELETE(itr, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
			return NULL;
		}

		/* Check if this feature-processor has any defined features */
		key_present = SMapObjectPresent(featProcInfo, "features", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_feature_processors",
					  "Call to \"SMapObjectPresent\" failed for \'class\' name of feature-processor \'%s\'",
					  featproc_name))
		{
			S_DELETE(itr, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
			return NULL;
		}

		if(key_present)
		{
			/* get feature-processors "features" */
			tmp = SMapGetObject(featProcInfo, "features", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_s_load_voice_feature_processors",
						  "Call to \"SMapGetObject\" failed for \'features\' of feature-processor \'%s\'",
						  featproc_name))
			{
				S_DELETE(itr, "_s_load_voice_feature_processors", error);
				S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
				S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
				return NULL;
			}

			featProcFeat = S_CAST(tmp, SMap, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_s_load_voice_feature_processors",
						  "Call to \"S_CAST (SMap)\" failed for \'features\' of feature-processor \'%s\'",
						  featproc_name))
			{
				S_DELETE(itr, "_s_load_voice_feature_processors", error);
				S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
				S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
				return NULL;
			}

			SFeatProcessorInit(&fProcessor, featProcFeat, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_s_load_voice_feature_processors",
						  "Call to \"SFeatProcessorInit\" failed for \'features\' of feature-processor \'%s\'",
						  featproc_name))
			{
				S_DELETE(itr, "_s_load_voice_feature_processors", error);
				S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
				S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
				return NULL;
			}
		}
		else
		{
			SFeatProcessorInit(&fProcessor, NULL, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_s_load_voice_feature_processors",
						  "Call to \"SFeatProcessorInit\" failed for \'features\' of feature-processor \'%s\'",
						  featproc_name))
			{
				S_DELETE(itr, "_s_load_voice_feature_processors", error);
				S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
				S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
				return NULL;
			}
		}

		/* add the feature-processor to the map of feature-processors */
		SMapSetObject(featProcessors, featproc_name, S_OBJECT(fProcessor), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_feature_processors",
					  "Call to \"SMapSetObject\" failed"))
		{
			S_DELETE(fProcessor, "_s_load_voice_feature_processors", error);
			S_DELETE(itr, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
			return NULL;
		}

		itr = SIteratorNext(itr);
	}

	if (featProcPlugins != NULL)
	{
		SListMerge(voicePlugins, featProcPlugins, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_feature_processors",
					  "Call to \"SListMerge\" failed"))
		{
			S_DELETE(featProcessors, "_s_load_voice_feature_processors", error);
			S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
			return NULL;
		}

		S_DELETE(featProcPlugins, "_s_load_voice_feature_processors", error);
	}

	return featProcessors;
}



