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
/* A loader for the voice utterance processors in the voice configuration file.     */
/* This is (currently) an exact copy of the feature processors loader in            */
/* feat_processor.c, with just the names that are changed.                          */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "hrg/processors/uttprocessor.h"
#include "pluginmanager/manager.h"
#include "voicemanager/loaders/utt_processor.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_load_voice_utterance_processors(const SMap *voiceConfig, SVoice *voice, s_erc *error)
{
	SMap *uttProcessors;
	const SObject *tmp;
	const SMap *voiceConfigUttProcessors;
	s_bool key_present;
	SIterator *itr;
	const char *uttproc_name;
	const char *uttproc_class;
	const char *uttproc_plugin;
	const SMap *uttProcInfo;
	const SMap *uttProcFeats;
	SUttProcessor *uProcessor;
	SPlugin *plugin;
	SList *uttProcPlugins;


	S_CLR_ERR(error);

	/* create a map container for utterance-processors */
	uttProcessors = S_MAP(S_NEW("SMapList", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_utterance_processors",
				  "Failed to create new map for voice utterance-processors"))
		return;

	/* look for "utterance-processors" key in voiceConfig map */
	key_present = SMapObjectPresent(voiceConfig, "utterance-processors", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_utterance_processors",
				  "Call to \"SMapObjectPresent\" failed for \'utterance-processors\' key"))
	{
		S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
		return;
	}

	if (!key_present)
	{
		/* no defined utterance-processors  */
		voice->uttProcessors = uttProcessors;
		return;
	}

	/* get utterance-processors from voiceConfig */
	tmp = SMapGetObject(voiceConfig, "utterance-processors", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_utterance_processors",
				  "Call to \"SMapGetObject\" failed"))
	{
		S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
		return;
	}

	voiceConfigUttProcessors = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_utterance_processors",
				  "Call to \"S_CAST (SMap)\" failed for \'utterance-processors\' object"))
	{
		S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
		return;
	}

	/*
	 * iterate through the voiceConfigUttProcessors, create the
	 * utterance-processors and add their features (if any), and add
	 * them to the uttProcessors map
	 */
	itr = SMapIterator(voiceConfigUttProcessors, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_load_voice_utterance_processors",
				  "Call to \"SMapIterator\" failed"))
	{
		S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
		return;
	}

	if (itr)
	{
		/* create a plug-in list */
		uttProcPlugins = S_LIST(S_NEW("SListList", error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_utterance_processors",
					  "Failed to create new list for voice utterance-processor plug-ins"))
		{
			S_DELETE(itr, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
			return;
		}
	}
	else
		uttProcPlugins = NULL;

	while (itr)
	{
		/* the utterance-processor name */
		uttproc_name = SMapIteratorKey(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_utterance_processors",
					  "Call to \"SMapIteratorKey\" failed"))
		{
			S_DELETE(itr, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
			return;
		}

		/* the utterance-processor info (SMap) */
		tmp = SMapIteratorValue(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_utterance_processors",
					  "Call to \"SMapIteratorValue\" failed"))
		{
			S_DELETE(itr, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
			return;
		}

		uttProcInfo = S_CAST(tmp, SMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_utterance_processors",
					  "Call to \"S_CAST (SMap)\" failed for utterance-processor \'%s\'",
					  uttproc_name))
		{
			S_DELETE(itr, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
			return;
		}

		/*
		 * uttProcInfo must have "class" and "plug-in" and
		 * optionally "features"
		 */
		uttproc_class = SMapGetString(uttProcInfo, "class", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_utterance_processors",
					  "Call to \"SMapGetString\" failed for \'class\' name of utterance-processor \'%s\'",
					  uttproc_name))
		{
			S_DELETE(itr, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
			return;
		}

		uttproc_plugin = SMapGetString(uttProcInfo, "plug-in", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_utterance_processors",
					  "Call to \"SMapGetString\" failed for \'plug-in\' name of utterance-processor \'%s\'",
					  uttproc_name))
		{
			S_DELETE(itr, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
			return;
		}

		/*
		 * load the plug-in, add it to the list of utterance-processor plug-ins and create
		 * the utterance-processor object
		 */
		plugin = s_pm_load_plugin(uttproc_plugin, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_utterance_processors",
					  "Call to \"s_pm_load_plugin\" failed for utterance-processor \'%s\'",
					  uttproc_name))
		{
			S_DELETE(itr, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
			return;
		}

		SListPush(uttProcPlugins, S_OBJECT(plugin), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_utterance_processors",
					  "Call to \"SListPush\" failed for utterance-processor plug-ins"))
		{
			S_DELETE(plugin, "_s_load_voice_utterance_processors", error);
			S_DELETE(itr, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
			return;
		}

		uProcessor = S_UTTPROCESSOR(S_NEW(uttproc_class, error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_utterance_processors",
					  "Failed to create new utterance-processor of class \'%s\'",
					  uttproc_class))
		{
			S_DELETE(itr, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
			return;
		}

		/* add the utterance-processor to the map of utterance-processors */
		SMapSetObject(uttProcessors, uttproc_name, S_OBJECT(uProcessor), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_utterance_processors",
					  "Call to \"SMapSetObject\" failed"))
		{
			S_DELETE(uProcessor, "_s_load_voice_utterance_processors", error);
			S_DELETE(itr, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
			return;
		}

		/* Check if this utterance-processor has any defined features */
		key_present = SMapObjectPresent(uttProcInfo, "features", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_utterance_processors",
					  "Call to \"SMapObjectPresent\" failed for \'features\' key of utterance-processor \'%s\'",
					  uttproc_name))
		{
			S_DELETE(itr, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
			return;
		}

		if (key_present)
		{
			/* get utterance-processors "features" */
			tmp = SMapGetObject(uttProcInfo, "features", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_s_load_voice_utterance_processors",
						  "Call to \"SMapGetObject\" failed for \'features\' of utterance-processor \'%s\'",
						  uttproc_name))
			{
				S_DELETE(itr, "_s_load_voice_utterance_processors", error);
				S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
				S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
				return;
			}

			uttProcFeats = S_CAST(tmp, SMap, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_s_load_voice_utterance_processors",
						  "Call to \"S_CAST (SMap)\" failed for \'features\' of utterance-processor \'%s\'",
						  uttproc_name))
			{
				S_DELETE(itr, "_s_load_voice_utterance_processors", error);
				S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
				S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
				return;
			}

			/* copy them to the utterance-processor */
			SMapCopy(uProcessor->features, uttProcFeats, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_s_load_voice_utterance_processors",
						  "Call to \"SMapCopy\" failed for \'features\' of utterance-processor \'%s\'",
						  uttproc_name))
			{
				S_DELETE(itr, "_s_load_voice_utterance_processors", error);
				S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
				S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
				return;
			}
		}

		/* initialize the utterance processor */
		SUttProcessorInit(&uProcessor, voice, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_utterance_processors",
					  "Call to \"SUttProcessorInit\" failed for utterance-processor of class \'%s\'",
					  uttproc_class))
		{
			S_DELETE(itr, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
			return;
		}

		itr = SIteratorNext(itr);
	}

	if (uttProcPlugins != NULL)
	{
		SListMerge(voice->plugins, uttProcPlugins, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_load_voice_utterance_processors",
					  "Call to \"SListMerge\" failed"))
		{
			S_DELETE(uttProcessors, "_s_load_voice_utterance_processors", error);
			S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
			return;
		}

		S_DELETE(uttProcPlugins, "_s_load_voice_utterance_processors", error);
	}

	voice->uttProcessors = uttProcessors;
}



