/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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
/* AUTHOR  : Simone Daminato                                                        */
/* DATE    : January 2016                                                           */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Generate HTS Labels for italian voices.                                          */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "hts_label_generator.h"
#include "hts_data_collector.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

#define MAX_PHONE_NAME_LENGTH 8


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SHTSLabelsConfigurableFeatProc class declaration. */
static SHTSLabelsGeneratorItFeatProcClass HTSLabelsGeneratorItFeatProcClass;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_hts_labels_generator_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&HTSLabelsGeneratorItFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_hts_labels_generator_class_reg",
			  "Failed to register SHTSLabelsGeneratorItFeatProcClass");
}


S_LOCAL void _s_hts_labels_generator_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&HTSLabelsGeneratorItFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_hts_labels_configurable_class_free",
			  "Failed to free SHTSLabelsGeneratorItFeatProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/
static void LoadConfiguration(SHTSLabelsGeneratorItFeatProc *self, const SItem *item, s_erc *error)
{
	const SVoice *voice;
	const SMap *labelsGenerator_data;

	S_CLR_ERR(error);

	voice = SItemVoice(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadConfiguration",
				  "Call to \"SItemVoice\" failed"))
		return;

	if (voice == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "LoadConfiguration",
				  "Item voice is NULL, voice is required to get phoneset");
		return;
	}
	
	/* get the hunpos data map */
	labelsGenerator_data = S_MAP(SVoiceGetFeature(voice, "labels generator data", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadConfiguration",
				  "Call to \"SVoiceGetFeature\" failed"))
		return;

	if (labelsGenerator_data == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "LoadConfiguration",
				  "Failed to get \"labels generator data\" map from voice features");
		return;
	}
	
	self->specialPhones = S_MAP(SMapGetObject(labelsGenerator_data, "special phones it", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadConfiguration",
				  "Call to \"SMapGetObject\" failed"))
		return;
	if (self->specialPhones == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "LoadConfiguration",
				  "Failed to get \"special phones it\" map from voice features");
		return;
	}
	
	self->nullPhoneme = SMapGetString(labelsGenerator_data, "null phoneme it", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "LoadConfiguration",
				  "Call to \"SMapGetString\" failed"))
		return;
	if (self->nullPhoneme == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "LoadConfiguration",
				  "Failed to get \"null phoneme it\" string from voice features");
		return;
	}
	
}

static void CheckPhoneme(SHTSLabelsGeneratorItFeatProc *self, char ** phone, s_erc *error)
{
	S_CLR_ERR(error);
	
	*phone = (char*) SMapGetStringDef(self->specialPhones, *phone, *phone, error);
}

static void Init(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	
	SHTSLabelsGeneratorItFeatProc* self = (SHTSLabelsGeneratorItFeatProc*) obj;
	self->specialPhones = NULL;
	self->nullPhoneme = NULL;
}

static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	
	SObjectDecRef(obj);
}

static void Destroy(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	
	SHTSLabelsGeneratorItFeatProc* self = (SHTSLabelsGeneratorItFeatProc*) obj;
	if (self->specialPhones != NULL)
	{
		SObjectDecRef((SObject*)self->specialPhones);
		self->specialPhones = NULL;
	}
	self->nullPhoneme = NULL;
}

static SObject *Run(const SFeatProcessor *self, const SItem *item,
					s_erc *error)
{
	SHTSLabelsGeneratorItFeatProc* labelGenerator = (SHTSLabelsGeneratorItFeatProc*) self;
	const SHTSLabelsDataCollectorFeatProc *data;
	const SObject *feature;
	char *resultString = NULL;
	char *tmp = NULL;
	char *phone = NULL;
	s_bool isPresent;
	
	S_CLR_ERR(error);
	
	/* load configuration if it's the first run */
	if (labelGenerator->specialPhones == NULL || labelGenerator->nullPhoneme == NULL)
	{
		LoadConfiguration(labelGenerator, item, error);
		if (labelGenerator->specialPhones == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to load special phones list.");
			goto quit_error;
		}
		if (labelGenerator->nullPhoneme == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to load null phoneme string.");
			goto quit_error;
		}
	}
	
	SItem* segItem = SItemAs(item, "Segment", error);
	
	data = (SHTSLabelsDataCollectorFeatProc *) SItemPathToFeatProc(segItem, "hts_label_data_collector", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemPathToFeatProc\" failed"))
		goto quit_error;

	if (data == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "data is NULL, HTS label data collector has encountered an error.");
		goto quit_error;
	}

	if (! S_HTSLABELSDATACOLLECTOR_METH_VALID(data, has_feature))
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "labelsGenerator's has_feature method is NULL.");
		goto quit_error;
	}
	
	if (! S_HTSLABELSDATACOLLECTOR_METH_VALID(data, get_feature))
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "labelsGenerator's get_feature method is NULL.");
		goto quit_error;
	}
	
	/**** phone context ****/
	/* p1 */
	isPresent = S_HTSLABELSDATACOLLECTOR_CALL(data, has_feature)(data, "p.p.phone", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorFeatureIsPresent\" failed"))
		goto quit_error;
	if (isPresent)
	{
		feature = S_HTSLABELSDATACOLLECTOR_CALL(data, get_feature)(data, "p.p.phone", error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorGetFeature\" failed"))
			goto quit_error;
		phone = (char*) SObjectGetString(feature, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectGetString\" failed"))
			goto quit_error;
		CheckPhoneme(labelGenerator, &phone, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"CheckPhoneme\" failed"))
			goto quit_error;
		
		s_asprintf(&tmp, error, "%s", phone);
	}
	else
	{
		s_asprintf(&tmp, error, "%s", labelGenerator->nullPhoneme);
		if (S_CHK_ERR(error, S_CONTERR,
				"Run",
				"Call to \"s_asprintf\" failed"))
		goto quit_error;
	}
	s_sappend(&resultString, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_sappend\" failed"))
		goto quit_error;
	S_FREE(tmp);
	
	/* p2 */
	isPresent = S_HTSLABELSDATACOLLECTOR_CALL(data, has_feature)(data, "p.phone", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorFeatureIsPresent\" failed"))
		goto quit_error;
	if (isPresent)
	{
		feature = S_HTSLABELSDATACOLLECTOR_CALL(data, get_feature)(data, "p.phone", error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorGetFeature\" failed"))
			goto quit_error;
		phone = (char*) SObjectGetString(feature, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectGetString\" failed"))
			goto quit_error;
		CheckPhoneme(labelGenerator, &phone, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"CheckPhoneme\" failed"))
			goto quit_error;
		
		s_asprintf(&tmp, error, "^%s", phone);
	}
	else
	{
		s_asprintf(&tmp, error, "^%s", labelGenerator->nullPhoneme);
		if (S_CHK_ERR(error, S_CONTERR,
				"Run",
				"Call to \"s_asprintf\" failed"))
		goto quit_error;
	}
	s_sappend(&resultString, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_sappend\" failed"))
		goto quit_error;
	S_FREE(tmp);
	
	/* p3 */
	isPresent = S_HTSLABELSDATACOLLECTOR_CALL(data, has_feature)(data, "phone", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorFeatureIsPresent\" failed"))
		goto quit_error;
	if (isPresent)
	{
		feature = S_HTSLABELSDATACOLLECTOR_CALL(data, get_feature)(data, "phone", error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorGetFeature\" failed"))
			goto quit_error;
		phone = (char*) SObjectGetString(feature, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectGetString\" failed"))
			goto quit_error;
		CheckPhoneme(labelGenerator, &phone, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"CheckPhoneme\" failed"))
			goto quit_error;
		
		s_asprintf(&tmp, error, "-%s", phone);
	}
	else
	{
		s_asprintf(&tmp, error, "-%s", labelGenerator->nullPhoneme);
		if (S_CHK_ERR(error, S_CONTERR,
				"Run",
				"Call to \"s_asprintf\" failed"))
		goto quit_error;
	}
	s_sappend(&resultString, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_sappend\" failed"))
		goto quit_error;
	S_FREE(tmp);
	
	/* p4 */
	isPresent = S_HTSLABELSDATACOLLECTOR_CALL(data, has_feature)(data, "n.phone", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorFeatureIsPresent\" failed"))
		goto quit_error;
	if (isPresent)
	{
		feature = S_HTSLABELSDATACOLLECTOR_CALL(data, get_feature)(data, "n.phone", error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorGetFeature\" failed"))
			goto quit_error;
		phone = (char*) SObjectGetString(feature, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectGetString\" failed"))
			goto quit_error;
		CheckPhoneme(labelGenerator, &phone, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"CheckPhoneme\" failed"))
			goto quit_error;
		
		s_asprintf(&tmp, error, "+%s", phone);
	}
	else
	{
		s_asprintf(&tmp, error, "+%s", labelGenerator->nullPhoneme);
		if (S_CHK_ERR(error, S_CONTERR,
				"Run",
				"Call to \"s_asprintf\" failed"))
		goto quit_error;
	}
	s_sappend(&resultString, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_sappend\" failed"))
		goto quit_error;
	S_FREE(tmp);
	
	/* p5 */
	isPresent = S_HTSLABELSDATACOLLECTOR_CALL(data, has_feature)(data, "n.n.phone", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorFeatureIsPresent\" failed"))
		goto quit_error;
	if (isPresent)
	{
		feature = S_HTSLABELSDATACOLLECTOR_CALL(data, get_feature)(data, "n.n.phone", error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorGetFeature\" failed"))
			goto quit_error;
		phone = (char*) SObjectGetString(feature, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SObjectGetString\" failed"))
			goto quit_error;
		CheckPhoneme(labelGenerator, &phone, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"CheckPhoneme\" failed"))
			goto quit_error;
		
		s_asprintf(&tmp, error, "=%s||", phone);
	}
	else
	{
		s_asprintf(&tmp, error, "=%s||", labelGenerator->nullPhoneme);
		if (S_CHK_ERR(error, S_CONTERR,
				"Run",
				"Call to \"s_asprintf\" failed"))
		goto quit_error;
	}
	s_sappend(&resultString, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_sappend\" failed"))
		goto quit_error;
	S_FREE(tmp);
	
	
	
	/* f48: syls_to_next_stressed */
	isPresent = S_HTSLABELSDATACOLLECTOR_CALL(data, has_feature)(data, "syls.to.next.stress", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorFeatureIsPresent\" failed"))
		goto quit_error;
	if (isPresent)
	{
		feature = S_HTSLABELSDATACOLLECTOR_CALL(data, get_feature)(data, "syls.to.next.stress", error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorGetFeature\" failed"))
			goto quit_error;
		
		s_asprintf(&tmp, error, "f48=%d|", SObjectGetInt(feature, error));
	}
	else
	{
		s_asprintf(&tmp, error, "f48=0|");
		if (S_CHK_ERR(error, S_CONTERR,
				"Run",
				"Call to \"s_asprintf\" failed"))
		goto quit_error;
	}
	s_sappend(&resultString, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_sappend\" failed"))
		goto quit_error;
	S_FREE(tmp);
	
	/* f6: next_accent */
	isPresent = S_HTSLABELSDATACOLLECTOR_CALL(data, has_feature)(data, "n.accent", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorFeatureIsPresent\" failed"))
		goto quit_error;
	if (isPresent)
	{
		feature = S_HTSLABELSDATACOLLECTOR_CALL(data, get_feature)(data, "n.accent", error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorGetFeature\" failed"))
			goto quit_error;
		
		s_asprintf(&tmp, error, "f6=%d|", SObjectGetInt(feature, error));
	}
	else
	{
		s_asprintf(&tmp, error, "f6=0|");
		if (S_CHK_ERR(error, S_CONTERR,
				"Run",
				"Call to \"s_asprintf\" failed"))
		goto quit_error;
	}
	s_sappend(&resultString, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_sappend\" failed"))
		goto quit_error;
	S_FREE(tmp);
	
	/* f22: next_accent */
	isPresent = S_HTSLABELSDATACOLLECTOR_CALL(data, has_feature)(data, "n.accent", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorFeatureIsPresent\" failed"))
		goto quit_error;
	if (isPresent)
	{
		feature = S_HTSLABELSDATACOLLECTOR_CALL(data, get_feature)(data, "n.accent", error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SHTSLabelDataCollectorGetFeature\" failed"))
			goto quit_error;
		
		s_asprintf(&tmp, error, "f6=%d|", SObjectGetInt(feature, error));
	}
	else
	{
		s_asprintf(&tmp, error, "f6=0|");
		if (S_CHK_ERR(error, S_CONTERR,
				"Run",
				"Call to \"s_asprintf\" failed"))
		goto quit_error;
	}
	s_sappend(&resultString, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_sappend\" failed"))
		goto quit_error;
	S_FREE(tmp);
	
	return SObjectSetString(resultString, error);
	
	/* error cleanup */
quit_error:
	S_DELETE(data, "Run", error);
	S_FREE(tmp);
	S_FREE(resultString);
	return NULL;
	
	S_UNUSED(self);
}


/************************************************************************************/
/*                                                                                  */
/* SHTSLabelsConfigurableFeatProc class initialization                              */
/*                                                                                  */
/************************************************************************************/

static SHTSLabelsGeneratorItFeatProcClass HTSLabelsGeneratorItFeatProcClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor:SHTSLabelsGeneratorItFeatProc",
		sizeof(SHTSLabelsGeneratorItFeatProc),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SFeatProcessorClass */
	Run                  /* run     */
};
