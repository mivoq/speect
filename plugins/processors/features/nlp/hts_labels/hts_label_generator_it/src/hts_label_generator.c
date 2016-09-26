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

static char* _s_escape_tobi_string (char* string, s_erc *error)
{
	char * result = NULL;
	char tmp[6];
	int i = 0;

	if (string == NULL)
		return NULL;

	//Search for '*' and replace it with 'st'
	//and for '%' and replace it with 'pc'

	while (string[i] != '\0')
	{


		switch (string[i])
		{
			case '*':
				s_sappend (&result, "st", error);
				if (S_CHK_ERR(error, S_CONTERR,
						  "_s_escape_tobi_string",
						  "Call to \"s_sappend\" failed"))
					break;

				break;

			case '%':
				s_sappend (&result, "pc", error);
				if (S_CHK_ERR(error, S_CONTERR,
						  "_s_escape_tobi_string",
						  "Call to \"s_sappend\" failed"))
					break;

				break;

			default:
				s_strncpy (tmp, string +i, 1, error);
				if (S_CHK_ERR(error, S_CONTERR,
						  "_s_escape_tobi_string",
						  "Call to \"s_strncpy\" failed"))
					break;

				i += s_width(tmp, error) - 1;
				if (S_CHK_ERR(error, S_CONTERR,
						  "_s_escape_tobi_string",
						  "Call to \"s_width\" failed"))
					break;

				s_sappend (&result, tmp, error);
				if (S_CHK_ERR(error, S_CONTERR,
						  "_s_escape_tobi_string",
						  "Call to \"s_sappend\" failed"))
					break;

				break;
		}
		i++;
	}
	return result;
}


/**
 * @hideinitializer
 * @def EXTRACTFEATURE
 * Extract from @a data the feature with key @a featureName.
 * The feature then is unboxed using the function @a unboxingFunction,
 * a string is generated as defined by @a outputString, and appended to
 * @a appendTo.
 * If the feature @a featureName is not present, @a defaultOutput is appended
 * to @a appendTo instead.
 *
 * @param self self pointer (type #SHTSLabelsGeneratorItFeatProc*)
 * @param data Data container (type #SHTSLabelsDataCollectorFeatProc*).
 * @param featureName Key of the feature (type #char*).
 * @param appendTo String on which we append the output (type #char*).
 * @param ERROR Pointer to error code variable to set (type #s_erc*).
 */
static int EXTRACTFEATURE(SHTSLabelsGeneratorItFeatProc *self, const SHTSLabelsDataCollectorFeatProc *data, const char *featureName, char **appendTo, s_erc *error){
	if (self->specialFeatures != NULL)
	{
		char *tmp = NULL;
		do {
			const SObject *feature = NULL;
			s_bool isPresent;
			s_erc error2;
			S_CLR_ERR(error);
			S_CLR_ERR(&error2);

			isPresent= S_HTSLABELSDATACOLLECTOR_CALL(data, has_feature)(data, featureName, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "EXTRACTFEATURE",
				      "Call to \"SHTSLabelDataCollectorFeatureIsPresent\" failed"))
				break;
			if (isPresent)
			{
				feature = S_HTSLABELSDATACOLLECTOR_CALL(data, get_feature)(data, featureName, error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "EXTRACTFEATURE",
					      "Call to \"SHTSLabelDataCollectorGetFeature\" failed"))
					break;
			}
			if (isPresent || TRUE) {
				const char *value = (char*) SMapGetStringDef(self->specialFeatures, featureName, featureName, error);
				/* Call the right unboxing function */
				if (s_strstr(value, "=%d", error ))
				{
					if (S_CHK_ERR(error, S_CONTERR,
						      "Run",
						      "Execution of \"s_strstr\" failed"))
						break;
					int itmp = (feature == NULL) ? 0 : SObjectGetInt(feature, error);
					if (S_CHK_ERR(error, S_CONTERR,
						      "EXTRACTFEATURE",
						      "Call to \"SObjectGetInt\" failed"))
						break;
					s_asprintf(&tmp, error, value, (itmp > 19)? 19 : itmp);
					if (S_CHK_ERR(error, S_CONTERR,
						      "EXTRACTFEATURE",
						      "Call to \"s_asprintf\" failed"))
						break;
				}
				else if (s_strstr(value, "=%s", error ))
				{
					if (S_CHK_ERR(error, S_CONTERR,
						      "Run",
						      "Execution of \"s_strstr\" failed"))
						break;
					s_asprintf(&tmp, error, value, (feature == NULL) ? "0" : SObjectGetString(feature, &error2));
					if (S_CHK_ERR(&error2, S_CONTERR,
						      "EXTRACTFEATURE",
						      "Call to \"SObjectGetString\" failed"))
						break;
					if (S_CHK_ERR(error, S_CONTERR,
						      "EXTRACTFEATURE",
						      "Call to \"s_asprintf\" failed"))
						break;

					tmp = _s_escape_tobi_string (tmp, error);

				}
			}
			if (tmp != NULL) s_sappend(appendTo, tmp, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "EXTRACTFEATURE",
				      "Call to \"s_sappend\" failed"))
				break;
		} while(0);
		if (tmp != NULL) S_FREE(tmp);
	}
	return 0;
}

/**
 * @hideinitializer
 * @def EXTRACTPHONEME
 * Extract from @a data the phoneme with key @a featureName.
 * The feature then is unboxed, fixed using @c CheckPhoneme, and a string is generated
 * as defined by @a outputString, and appended to @a appendTo.
 * If the feature @a featureName is not present,  @a nullPhoneme (taken from @c self)
 * is used instead.
 *
 * @param self self pointer (type #SHTSLabelsGeneratorItFeatProc*)
 * @param data Data container (type #SHTSLabelsDataCollectorFeatProc*).
 * @param featureName Key of the feature (type #char*).
 * @param outputString String to format the feature (type #char*).
 * @param appendTo String on which we append the output (type #char*).
 * @param ERROR Pointer to error code variable to set (type #s_erc*).
 */
#define EXTRACTPHONEME(self, data, featureName, outputString, appendTo, error)					\
do {														\
	char *tmp = NULL;											\
	do {													\
		const SObject *feature;										\
		const char *phone;										\
		s_bool isPresent;										\
		S_CLR_ERR(error);										\
														\
		isPresent= S_HTSLABELSDATACOLLECTOR_CALL(data, has_feature)(data, featureName, error);		\
		if (S_CHK_ERR(error, S_CONTERR,									\
					"EXTRACTPHONEME",							\
					"Call to \"SHTSLabelDataCollectorFeatureIsPresent\" failed"))		\
			break;											\
		if (isPresent)											\
		{												\
			feature = S_HTSLABELSDATACOLLECTOR_CALL(data, get_feature)(data, featureName, error);	\
			if (S_CHK_ERR(error, S_CONTERR,								\
					"EXTRACTPHONEME",							\
					"Call to \"SHTSLabelDataCollectorGetFeature\" failed"))			\
				break;										\
			phone = (char*) SObjectGetString(feature, error);					\
			if (S_CHK_ERR(error, S_CONTERR,								\
					"EXTRACTPHONEME",							\
					"Call to \"SObjectGetString\" failed"))					\
				break;										\
			CheckPhoneme(self, &phone, error);							\
			if (S_CHK_ERR(error, S_CONTERR,								\
					"EXTRACTPHONEME",							\
					"Call to \"CheckPhoneme\" failed"))					\
				break;										\
			s_asprintf(&tmp, error, outputString, phone);						\
		}												\
		else												\
		{												\
			s_asprintf(&tmp, error, outputString, self->nullPhoneme);				\
			if (S_CHK_ERR(error, S_CONTERR,								\
					"EXTRACTPHONEME",							\
					"Call to \"s_asprintf\" failed"))					\
			break;											\
		}												\
		s_sappend(&appendTo, tmp, error);								\
		if (S_CHK_ERR(error, S_CONTERR,									\
					"EXTRACTPHONEME",							\
					"Call to \"s_sappend\" failed"))					\
			break;											\
	} while(0);												\
	if (tmp != NULL) S_FREE(tmp);										\
} while(0);


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

	if (SMapObjectPresent(labelsGenerator_data, "phones_map", error))
	{
		if (S_CHK_ERR(error, S_CONTERR,
				  "LoadConfiguration",
				  "Call to \"SVoiceGetFeature\" failed"))
			return;

		self->specialPhones = S_MAP(SMapGetObject(labelsGenerator_data, "phones_map", error));
		if (S_CHK_ERR(error, S_CONTERR,
					"LoadConfiguration",
					"Call to \"SMapGetObject\" failed"))
			return;
	}
	/* if the map doesn't exist, set it to NULL for the caller */
	else
	{
		self->specialPhones = NULL;
	}

	if (SMapObjectPresent(labelsGenerator_data, "features_map_int", error))
	{
		if (S_CHK_ERR(error, S_CONTERR,
				  "LoadConfiguration",
				  "Call to \"SVoiceGetFeature\" failed"))
			return;

		self->specialFeatures = S_MAP(SMapGetObject(labelsGenerator_data, "features_map_int", error));
		if (S_CHK_ERR(error, S_CONTERR,
					"LoadConfiguration",
					"Call to \"SMapGetObject\" failed"))
			return;
	}
	else
	{
		self->specialFeatures = NULL;
	}

	if (SMapObjectPresent(labelsGenerator_data, "null_phone", error))
	{
		if (S_CHK_ERR(error, S_CONTERR,
				  "LoadConfiguration",
				  "Call to \"SVoiceGetFeature\" failed"))
			return;

		self->nullPhoneme = SMapGetString(labelsGenerator_data, "null_phone", error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "LoadConfiguration",
				  "Call to \"SMapGetString\" failed"))
			return;
	}
	else
	{
		self->nullPhoneme = NULL;
	}
}

static void CheckPhoneme(SHTSLabelsGeneratorItFeatProc *self, const char **phone, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->specialPhones != NULL)
	{
		*phone = (char*) SMapGetStringDef(self->specialPhones, *phone, *phone, error);
	}
}

static void Init(void *obj, s_erc *error)
{
	S_CLR_ERR(error);

	SHTSLabelsGeneratorItFeatProc* self = (SHTSLabelsGeneratorItFeatProc*) obj;
	self->specialPhones = NULL;
	self->nullPhoneme = NULL;
	self->specialFeatures = NULL;
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
	self->specialPhones = NULL;
	self->nullPhoneme = NULL;
	self->specialFeatures = NULL;
}

static SObject *Run(const SFeatProcessor *self, const SItem *item,
					s_erc *error)
{
	SHTSLabelsGeneratorItFeatProc* labelGenerator = (SHTSLabelsGeneratorItFeatProc*) self;
	const SHTSLabelsDataCollectorFeatProc *data;
	char *resultString = NULL;
	char *tmp = NULL;

	S_CLR_ERR(error);

	/* load configuration if it's the first run */
	if (labelGenerator->specialPhones == NULL || labelGenerator->nullPhoneme == NULL ||
		labelGenerator->specialFeatures == NULL)
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
				  "Failed to load null phone string.");
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
	EXTRACTPHONEME(labelGenerator, data, "p.p.phone", "%s", resultString, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Execution of \"EXTRACTPHONEME\" failed"))
		goto quit_error;

	/* p2 */
	EXTRACTPHONEME(labelGenerator, data, "p.phone", "^%s", resultString, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Execution of \"EXTRACTPHONEME\" failed"))
		goto quit_error;

	/* p3 */
	EXTRACTPHONEME(labelGenerator, data, "phone", "-%s", resultString, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Execution of \"EXTRACTPHONEME\" failed"))
		goto quit_error;

	/* p4 */
	EXTRACTPHONEME(labelGenerator, data, "n.phone", "+%s", resultString, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Execution of \"EXTRACTPHONEME\" failed"))
		goto quit_error;

	/* p5 */
	EXTRACTPHONEME(labelGenerator, data, "n.n.phone", "=%s||", resultString, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Execution of \"EXTRACTPHONEME\" failed"))
		goto quit_error;

	/* Features */
	SIterator *itrFeatureMap = NULL;
	if (labelGenerator->specialFeatures != NULL)
	{
		/* iterator for the loop on the the map */
		itrFeatureMap = S_ITERATOR_GET(labelGenerator->specialFeatures, error);
		if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Execution of \"S_ITERATOR_GET\" failed"))
			goto quit_error;

		while (itrFeatureMap != NULL)
		{
			const char *keyString = SIteratorKey(itrFeatureMap, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "Run",
				      "Execution of \"SIteratorObject\" failed"))
				goto quit_error;

			EXTRACTFEATURE(labelGenerator, data, keyString, &resultString, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "Run",
				      "Execution of \"EXTRACTFEATURE\" failed"))
				goto quit_error;

			itrFeatureMap = SIteratorNext(itrFeatureMap);
		}
	}
	s_sappend(&resultString, "|", error);
	return SObjectSetString(resultString, error);

	/* error cleanup */
quit_error:
	if (data != NULL)
	{
		S_DELETE(data, "Run", error);
	}
	if (itrFeatureMap != NULL)
	{
		S_DELETE(itrFeatureMap, "Run", error);
	}
	if (tmp != NULL)
	{
		S_FREE(tmp);
	}
	if (resultString != NULL)
	{
		S_FREE(resultString);
	}

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
	NULL,               /* initialize */
	Run                  /* run     */
};
