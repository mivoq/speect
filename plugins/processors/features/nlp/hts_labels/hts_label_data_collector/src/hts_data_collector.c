/************************************************************************************/
/* Copyright (c) Mivoq SRL                                                          */
/*                                                                                  */
/* Contributors:  Simone Daminato, Federico Rossetto, Giulio Paci                   */
/*                                                                                  */
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
/* DATE    : December 2015                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Generate data for HTS Labels for italian voices.                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "hts_data_collector.h"
#include "phoneset.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

#define MAX_PHONE_NAME_LENGTH 8
#define SELFPARAMETERTYPE SHTSLabelsDataCollectorFeatProc


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SHTSLabelsConfigurableFeatProc class declaration. */
static SHTSLabelsDataCollectorFeatProcClass HTSLabelsDataCollectorFeatProcClass;



/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/


/**
 * Query if named feature is present in the given Label Data Collector.
 * @public @memberof SHTSLabelsDataCollectorFeatProc
 * @param self The given Label Data Collector.
 * @param name The feature name.
 * @param error Error code.
 * @return #TRUE if present or #FALSE if not.
 */
S_API s_bool SHTSLabelDataCollectorFeatureIsPresent(const SHTSLabelsDataCollectorFeatProc *self,
					   const char *name,
					   s_erc *error);


/**
 * Get the feature object of the named key from the given Label Data Collector.
 * @public @memberof SHTSLabelsDataCollectorFeatProc
 * @param self The given Label Data Collector.
 * @param key The string key of the feature processor object to get.
 * @param error Error code.
 * @return Pointer to #SObject object feature value.
 */
S_API const SObject *SHTSLabelDataCollectorGetFeature(const SHTSLabelsDataCollectorFeatProc *self, const char *name,
					     s_erc *error);


/**
 * Set the feature of the named key to the given Label Data Collector.
 * If the named key already exists then it's #SObject will be deleted
 * (if not referenced) and replaced.
 * @private @memberof SHTSLabelsDataCollectorFeatProc
 * @param self The given Label Data Collector.
 * @param key The string key of the feature object to set.
 * @param object The feature object (value).
 * @param error Error code.
 */
S_API void SHTSLabelDataCollectorSetFeature(SHTSLabelsDataCollectorFeatProc *self, const char *name,
				   const SObject *object, s_erc *error);


/**
 * Delete the feature of the named key of the the given Label Data Collector.
 * The key is removed and the feature object deleted if it is not referenced.
 * @private @memberof SHTSLabelsDataCollectorFeatProc
 * @param self The given Label Data Collector.
 * @param name The feature name (key).
 * @param error Error code.
 */
S_API void SHTSLabelDataCollectorDelFeature(SHTSLabelsDataCollectorFeatProc *self, const char *name,
				   s_erc *error);



/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static sint32 get_stress_level(const SObject *stressFeat, s_erc *error);

static sint32 get_accent_level(const SObject *accentFeat, s_erc *error);

static s_bool segment_is_pause(const SItem *item, s_erc *error);

static SObject* compute_stress (const SItem *item, const char* path, s_erc *error);


/*
 * creates the phoneme context data
 */
static void create_phone_context(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error);


/*
 * creates the syllable context data
 */
static void create_syl_context(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error);

/*
 * creates the syllable context data in case it is a pause.
 */
static void create_syl_context_pause(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error);

/*
 * creates the word context data
 */
static void create_word_context(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error);

/*
 * creates the word context data in case it is a pause.
 */
static void create_word_context_pause(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error);

/*
 * creates the phrase context data
 */
static void create_phrase_context(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error);

/*
 * creates the phrase context data in case it is a pause.
 * differs from create_phrase_context in that either next or previous
 * phone's phrase is used (if prev exists use it, else use next)
 */
static void create_phrase_context_pause(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error);

/*
 * creates the utterance context data
 */
static void create_utterance_context(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error);

/*
 * creates the utterance context data in case it is a pause.
 */
static void create_utterance_context_pause(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error);

/*
 * creates the stress context data
 */
static void create_stress_context(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error);

/*
 * creates the stress context data
 * Differs from create_stress_context in that previous/next phone's syllable is queried
 * and not previous/next syllable
 */
static void create_stress_context_pause(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error);

/*
 * creates the accent context data
 */
static void create_accent_context(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error);

/*
 * creates the accent context data
 * Differs from create_accent_context in that previous/next phone's syllable is queried
 * and not previous/next syllable
 */
static void create_accent_context_pause(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error);

/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/


S_API s_bool SHTSLabelDataCollectorFeatureIsPresent(const SHTSLabelsDataCollectorFeatProc *self,
						    const char *name,
						    s_erc *error)
{
	s_bool feat_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
					  "SHTSLabelDataCollectorFeatureIsPresent",
					  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
					  "SHTSLabelDataCollectorFeatureIsPresent",
					  "Argument \"name\" is NULL");
		return FALSE;
	}

	feat_present = SMapObjectPresent(self->features, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SHTSLabelDataCollectorFeatureIsPresent",
				  "Call to \"SMapObjectPresent\" failed"))
		return FALSE;

	return feat_present;
}

S_API const SObject *SHTSLabelDataCollectorGetFeature(const SHTSLabelsDataCollectorFeatProc *self, const char *name,
						      s_erc *error)
{
	const SObject *object;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
					"SHTSLabelDataCollectorGetFeature",
					"Argument \"self\" is NULL");
		return NULL;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
					"SHTSLabelDataCollectorGetFeature",
					"Argument \"name\" is NULL");
		return NULL;
	}

	object = SMapGetObject(self->features, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SHTSLabelDataCollectorGetFeature",
				  "Call to \"SMapGetObject\" failed"))
		return NULL;

	return object;
}

S_API void SHTSLabelDataCollectorSetFeature(SHTSLabelsDataCollectorFeatProc *self, const char *name,
					    const SObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
					"SHTSLabelDataCollectorSetFeature",
					"Argument \"self\" is NULL");
		return;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
					"SHTSLabelDataCollectorSetFeature",
					"Argument \"name\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
					"SHTSLabelDataCollectorSetFeature",
					"Argument \"object\" is NULL");
		return;
	}

	SMapSetObject(self->features, name, object, error);
	S_CHK_ERR(error, S_CONTERR,
				"SHTSLabelDataCollectorSetFeature",
				"Call to \"SMapSetObject\" failed");
}


S_API void SHTSLabelDataCollectorDelFeature(SHTSLabelsDataCollectorFeatProc *self, const char *name,
					    s_erc *error)
{
	s_bool feat_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
					"SHTSLabelDataCollectorDelFeature",
					"Argument \"self\" is NULL");
		return;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
					"SHTSLabelDataCollectorDelFeature",
					"Argument \"name\" is NULL");
		return;
	}

	if (!S_HTSLABELSDATACOLLECTOR_METH_VALID(self, has_feature))
	{
		S_CTX_ERR(error, S_METHINVLD,
					"SHTSLabelDataCollectorDelFeature",
					"Method \"has_feature\" is NULL");
		return;
	}

	feat_present = S_HTSLABELSDATACOLLECTOR_CALL(self, has_feature)(self, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				"SHTSLabelDataCollectorDelFeature",
				"Call to \"SUttProcessorFeatPresent\" failed"))
		return;

	if (feat_present)
	{
		SMapObjectDelete(self->features, name, error);
		S_CHK_ERR(error, S_CONTERR,
					"SHTSLabelDataCollectorDelFeature",
					"Call to \"SMapObjectDelete\" failed");
	}
}



/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_hts_labels_data_collector_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&HTSLabelsDataCollectorFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_hts_labels_data_collector_class_reg",
			  "Failed to register SHTSLabelsDataCollectorFeatProc");
}


S_LOCAL void _s_hts_labels_data_collector_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&HTSLabelsDataCollectorFeatProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_hts_labels_data_collector_class_free",
			  "Failed to free SHTSLabelsDataCollectorFeatProc");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/*
 * this is for Georg's stress levels:
 * unstressed : 0
 * primary    : 1
 * secondary  : 2
 */
static sint32 get_stress_level(const SObject *stressFeat, s_erc *error)
{
	const char *stress_feat;


	S_CLR_ERR(error);
	stress_feat = SObjectGetString(stressFeat, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_stress_level",
				  "Call to \"SObjectGetString\" failed"))
		return 0;

	if (s_strcmp(stress_feat, "unstressed", error) == 0)
	{
		S_CHK_ERR(error, S_CONTERR,
				  "get_stress_level",
				  "Call to \"s_strcmp\" failed");
		return 0;
	}

	if (s_strcmp(stress_feat, "primary", error) == 0)
	{
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_stress_level",
					  "Call to \"s_strcmp\" failed"))
			return 0;

		return 1;
	}

	if (s_strcmp(stress_feat, "secondary", error) == 0)
	{
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_stress_level",
					  "Call to \"s_strcmp\" failed"))
			return 0;

		return 2;
	}

	return 0; /* unknown */
}


/*
 * this is for Georg's accent levels:
 * unaccented : 0
 * accented   : 1
 */
static sint32 get_accent_level(const SObject *accentFeat, s_erc *error)
{
	const char *accent_feat;


	S_CLR_ERR(error);
	accent_feat = SObjectGetString(accentFeat, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_accent_level",
				  "Call to \"SObjectGetString\" failed"))
		return 0;

	if (s_strcmp(accent_feat, "unaccented", error) == 0)
	{
		S_CHK_ERR(error, S_CONTERR,
				  "get_accent_level",
				  "Call to \"s_strcmp\" failed");
		return 0;
	}

	if (s_strcmp(accent_feat, "accented", error) == 0)
	{
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_accent_level",
					  "Call to \"s_strcmp\" failed"))
			return 0;

		return 1;
	}

	return 0; /* unknown */
}


static s_bool segment_is_pause(const SItem *item, s_erc *error)
{
	const SVoice *voice;
	const SPhoneset *phoneset;
	s_bool is_pause;


	/* we cannot use _get_phoneset (as in syllable_vowel processor)
	 * here because the pause items do not have tokens
	 */
	S_CLR_ERR(error);

	voice = SItemVoice(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "segment_is_pause",
				  "Call to \"SItemVoice\" failed"))
		return FALSE;

	if (voice == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "segment_is_pause",
				  "Item voice is NULL, voice is required to get phoneset");
		return FALSE;
	}

	phoneset = S_PHONESET(SVoiceGetData(voice, "phoneset", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "segment_is_pause",
				  "Call to \"SVoiceGetData\" failed"))
		return FALSE;

	if (phoneset == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "segment_is_pause",
				  "Phoneset is NULL, phoneset is required to get silence phone");
		return FALSE;
	}

	is_pause = S_PHONESET_CALL(phoneset, phone_has_feature)(phoneset,
															SItemGetName(item, error),
															"pause",
															error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "segment_is_pause",
				  "Call to \"phone_has_feature/SItemGetName\" failed"))
		return FALSE;

	return is_pause;
}

static SObject* compute_stress (const SItem *item, const char* path, s_erc *error)
{
	const SObject *featPath;
	int level = -1;
	const char* syllablepart = NULL;

	S_CLR_ERR(error);

	item = SItemPathToItem (item, path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "compute_stress",
				  "Call to \"SItemPathToItem\" failed"))
		return NULL;

	/* syllablepart */
	featPath = SItemPathToFeature(item, "syllablepart", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "compute_stress",
				  "Call to \"SItemPathToFeature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		/* unboxing the value of onset-nucleus-coda for the stress computation */
		syllablepart = SObjectGetString (featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "compute_stress",
					  "Call to \"SObjectGetString\" failed"))
			return NULL;
	}

	/* current syllable stress */
	featPath = SItemPathToFeature(item, "R:SylStructure.parent.R:Syllable.stress", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "compute_stress",
				  "Call to \"SItemPathToFeature\" failed"))
		return NULL;

	if (featPath != NULL)
	{
		level = get_stress_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_stress_level",
					  "Call to \"SItemPathToFeature\" failed"))
			return NULL;
	}

	if ( level != -1 && syllablepart != NULL )
	{
		if ( s_strcmp (syllablepart, "nucleus", error) == 0)
		{
			if (level != 0)
				return SObjectSetString ("+", error);
			else
				return SObjectSetString ("-", error);
		}
		if (S_CHK_ERR(error, S_CONTERR,
					  "compute_stress",
					  "Call to \"s_strcmp\" failed"))
			return NULL;
	}
	return NULL; /* No stress is defined */
}


/*
 * creates the phoneme context data
 */
static void create_phone_context(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error)
{
	const SObject *featPath;
	int segStart=0, segTot=0;

	S_CLR_ERR(error);

	/* p.p.name */
	featPath = SItemPathToFeature(item, "p.p.name", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SItemPathToFeature\" failed"))
		return;

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "p.p.phone", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			return;

	}

	/* p.name */
	featPath = SItemPathToFeature(item, "p.name", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SItemPathToFeature\" failed"))
		return;

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "p.phone", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			return;
	}

	/* name */
	featPath = SItemPathToFeature(item, "name", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SItemGetName\" failed"))
		return;

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phone", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			return;
	}

	/* n.name */
	featPath = SItemPathToFeature(item, "n.name", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SItemPathToFeature\" failed"))
		return;

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "n.phone", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			return;
	}

	/* n.n.name */
	featPath = SItemPathToFeature(item, "n.n.name", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SItemPathToFeature\" failed"))
		return;

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "n.n.phone", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			return;
	}

	/* prev prev duration */
	featPath = SItemPathToFeature(item, "p.p.duration", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SItemPathToFeature\" failed"))
		return;

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "prev.prev.duration", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			return;
	}

	/* prev duration */
	featPath = SItemPathToFeature(item, "p.duration", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SItemPathToFeature\" failed"))
		return;

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "prev.duration", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			return;
	}

	/* next duration */
	featPath = SItemPathToFeature(item, "n.duration", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SItemPathToFeature\" failed"))
		return;

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "next.duration", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			return;
	}

		/*next next duration */
	featPath = SItemPathToFeature(item, "n.n.duration", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SItemPathToFeature\" failed"))
		return;

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "next.next.duration", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			return;
	}

	/* Segments from the word start */
	featPath = SItemPathToFeatProc(item, "segment_pos_word", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SItemPathToFeatProc\" failed"))
		return;

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phones.from.word.start", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			return;

		segStart = SObjectGetInt (featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"SObjectGetInt\" failed"))
			return;
	}

	/* Segments from the word end*/
	featPath = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.word_num_phones", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SItemPathToFeatProc\" failed"))
		return;

	if (featPath != NULL)
	{
		segTot = SObjectGetInt (featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"SObjectGetInt\" failed"))
			return;
	}

	if (segTot != 0)
	{
		featPath = SObjectSetInt ( segTot - segStart - 1, error );
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phone_context",
					  "Call to \"SObjectSetInt\" failed"))
			return;

		SHTSLabelDataCollectorSetFeature(self, "phones.to.word.end", featPath, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "create_phone_context",
						  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
				return;
	}


	/* Phone next_is_pause */
	SItem* segment = SItemNext (item, error );
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
		return;

	if( segment != NULL )
	{
		s_bool isPause = segment_is_pause( segment, error);

		if (isPause == TRUE)
		{
			featPath = SObjectSetInt (1, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "create_phone_context",
						  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
				return;

			SHTSLabelDataCollectorSetFeature(self, "next.is.pause", featPath, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "create_phone_context",
						  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
				return;
		}
	}

	segment = SItemPrev (item, error );
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phone_context",
				  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
		return;

	if( segment != NULL )
	{
		s_bool isPause = segment_is_pause( segment, error);

		if (isPause == TRUE)
		{
			featPath = SObjectSetInt (1, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "create_phone_context",
						  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
				return;

			SHTSLabelDataCollectorSetFeature(self, "prev.is.pause", featPath, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "create_phone_context",
						  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
				return;
		}
	}

}


/*
 * creates the syllable context data
 */
static void create_syl_context(SELFPARAMETERTYPE* self, const SItem* item, s_erc* error)
{
	const SObject *dFeat;

	S_CLR_ERR(error);

	/* pos from start */
	dFeat = SItemPathToFeatProc(item, "segment_pos_syl", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phones.from.syl.start", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_cleanup;

		SHTSLabelDataCollectorSetFeature(self, "position.in.syl", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_cleanup;

	}

	/* pos from end */
	dFeat = SItemPathToFeatProc(item, "segment_pos_syl_rev", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phones.from.syl.end", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_cleanup;

	}

	/* number of phonemes in the previous syllable */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.R:Syllable.p.syllable_num_phones", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "p.syl.phones.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_cleanup;

	}

	/* number of phonemes in this syllable */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.R:Syllable.syllable_num_phones", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "syl.phones.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_cleanup;

	}

	/* number of phonemes in the next syllable */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.R:Syllable.n.syllable_num_phones", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "n.syl.phones.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_cleanup;

	}

	/* name of the vowel in the current syllable */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.R:Syllable.syllable_vowel", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "vowel.name", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_cleanup;

	}

		/* type of accent of the current syllable*/
	dFeat = SItemPathToFeature(item, "R:SylStructure.parent.R:Syllable.accent", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "tobi.accent", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_cleanup;

		const char *extractFeature = SObjectGetString ( dFeat, error );
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SObjectGetString\" failed"))
			goto syl_context_cleanup;

		if( s_strcmp ( extractFeature, "", error ) == 0 )
			dFeat = SObjectSetInt ( 0, error);
		else
			dFeat = SObjectSetInt ( 1, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SObjectSetInt\" failed"))
			goto syl_context_cleanup;

		SHTSLabelDataCollectorSetFeature(self, "accented", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_cleanup;

	}

	/* type of accent of the next syllable*/
	dFeat = SItemPathToFeature(item, "R:SylStructure.parent.R:Syllable.n.accent", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "next.tobi.accent", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_cleanup;

	}

	/* type of accent of the next next syllable*/
	dFeat = SItemPathToFeature(item, "R:SylStructure.parent.R:Syllable.n.n.accent", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "next.next.tobi.accent", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_cleanup;

	}

	/* Position type of syllable in the word*/
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.R:Syllable.syllable_pos_type", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "syllable.position.type", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_cleanup;

	}

return;

syl_context_cleanup:
	if (dFeat != NULL)
		S_DELETE(dFeat, "create_syl_context", error);
}

/*
 * creates the syllable context data in case of a pause
 */
static void create_syl_context_pause(SELFPARAMETERTYPE* self, const SItem* item, s_erc* error)
{
	const SObject *dFeat;

	S_CLR_ERR(error);

	/* pos from start and end doesn't make sense*/

	/* number of phonemes in the previous syllable */
	dFeat = SItemPathToFeatProc(item, "p.R:SylStructure.parent.R:Syllable.syllable_num_phones", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "p.syl.phones.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_pause_cleanup;

	}

	/* number of phonemes in this syllable: not in pause */

	/* number of phonemes in the next syllable */
	dFeat = SItemPathToFeatProc(item, "n.R:SylStructure.parent.R:Syllable.syllable_num_phones", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_syl_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "n.syl.phones.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_syl_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto syl_context_pause_cleanup;

	}

	return;

syl_context_pause_cleanup:
	if (dFeat != NULL)
		S_DELETE(dFeat, "create_syl_context_pause", error);
}

/*
 * creates the word context data
 */
static void create_word_context(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error)
{
	const SObject *dFeat;

	S_CLR_ERR(error);

	/* number of syllables before this one */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.R:Syllable.syllable_pos_word", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "syls.from.word.start", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_cleanup;

	}

	/* number of syllables after this one */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.R:Syllable.syllable_pos_word_rev", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "syls.from.word.end", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_cleanup;

	}

	/* guess-part-of-speech of previous word */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.p.pos", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "p.word.gpos", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_cleanup;

	}

	/* guess-part-of-speech of this word */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.pos", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "word.gpos", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_cleanup;

	}

	/* guess-part-of-speech of next word */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.n.pos", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "n.word.gpos", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_cleanup;

	}

	/* the number of syllables in the previous word */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.p.word_num_syls", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "p.word.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_cleanup;

	}

	/* the number of syllables in this word */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.word_num_syls", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "word.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_cleanup;

	}

	/* the number of phones in this word */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.word_num_phones", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "word.phones.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_cleanup;

	}

	/* the number of words from the beginning of the phrase */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.words_from_phrase_start", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "words.from.phrase.start", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_cleanup;
	}

	/* the number of words from the ending of the phrase */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.words_from_phrase_end", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "words.from.phrase.end", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_cleanup;
	}

	/* the number of syllables in the next word */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.n.word_num_syls", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "n.word.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_cleanup;

	}

	return;

word_context_cleanup:
	if (dFeat != NULL)
		S_DELETE(dFeat, "create_word_context", error);
}

/*
 * creates the word context data in case it is a pause.
 */
static void create_word_context_pause(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error)
{
	const SObject *dFeat;

	S_CLR_ERR(error);

	/* guess-part-of-speech of previous word */
	dFeat = SItemPathToFeatProc(item, "p.R:SylStructure.parent.parent.R:Word.pos", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "p.word.gpos", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_pause_cleanup;

	}

	/* guess-part-of-speech of this word: not possible */

	/* guess-part-of-speech of next word */
	dFeat = SItemPathToFeatProc(item, "n.R:SylStructure.parent.parent.R:Word.pos", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "n.word.gpos", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_pause_cleanup;

	}

	/* the number of syllables in the previous word */
	dFeat = SItemPathToFeatProc(item, "p.R:SylStructure.parent.parent.R:Word.word_num_syls", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "p.word.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_pause_cleanup;

	}

	/* the number of syllables in this word: not possible */

	/* the number of syllables in the next word */
	dFeat = SItemPathToFeatProc(item, "n.R:SylStructure.parent.parent.R:Word.word_num_syls", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "n.word.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_pause_cleanup;

	}

	/* type of tobi accent of the current word*/
	dFeat = SItemPathToFeature(item, "R:SylStructure.parent.parent.R:Token.parent.accent", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_word_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "tobi.accent", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_word_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto word_context_pause_cleanup;

	}


word_context_pause_cleanup:
	if (dFeat != NULL)
		S_DELETE(dFeat, "create_word_context_pause", error);
}

/*
 * creates the phrase context data
 */
static void create_phrase_context(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error)
{
	const SObject *dFeat;
	int start = 0, end = 0;

	S_CLR_ERR(error);

	/* number of syllables before this one */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.R:Syllable.syllable_pos_phrase", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	start = SObjectGetInt ( dFeat, error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SObjectGetInt\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "syls.from.phrase.start", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;

	}

	/* number of syllables after this one */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.R:Syllable.syllable_pos_phrase_rev", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	end = SObjectGetInt ( dFeat, error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SObjectGetInt\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "syls.from.phrase.end", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;

	}

	/* number of syllables in this phrase */
	dFeat = SObjectSetInt(start + end, error);

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phrase.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;

	}

	/* number of words before this in phrase */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.word_pos_phrase", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "words.from.phrase.start", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;

	}

	/* number of words after this in phrase */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.word_pos_phrase_rev", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "words.from.phrase.end", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;

	}

	/* The following lines are commented because the required features are, apparently,
	   missing and no plugin seems available to compute them. */
	/* the number of content words before the current word in the current phrase */
	/*dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.word_content_in", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");
	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "content.words.before", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;
	}*/

	/* the number of content words after the current word in the current phrase */
	/*dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.word_content_out", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");
	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "content.words.after", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;
	}*/

	/* the number of words from the previous content word to the current word */
	/*dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.word_content_all_in", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");
	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "words.from.prev.cont", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;
	}*/

	/* the number of words from the current word to the next content word */
	/*dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Word.word_content_all_out", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");
	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "words.from.next.cont", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;
	}*/

	/* the number of syllables in the previous phrase */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Phrase.parent.p.phrase_num_syls", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "p.phrase.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;


	}

	/* the number of syllables in this phrase */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_syls", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phrase.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;

	}

	/* the number of syllables in the next phrase */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Phrase.parent.n.phrase_num_syls", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "n.phrase.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;

	}

	/* the number of words in the previous phrase */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Phrase.parent.p.phrase_num_words", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "p.phrase.words.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;

	}

	/* the number of words in this phrase */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_words", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phrase.words.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;

	}

	/* the number of words in the next phrase */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Phrase.parent.n.phrase_num_words", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "n.phrase.words.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;

	}

	/* the type of the current phrase */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Phrase.parent.phrase_type", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phrase.type", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;

	}

	/* the punctuation of the current phrase*/
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Phrase.parent.phrase_punc", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phrase.punc", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;

	}

	/* TOBI endtone of the current phrase */
	/* item, syllable, word, phrase, last word, last syllable */
	dFeat = SItemPathToFeature(item, "R:SylStructure.parent.parent.R:Phrase.parent.daughtern.R:SylStructure.daughtern.endtone",
								error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "tobi.endtone", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_cleanup;

	}

	return;

phrase_context_cleanup:
	if (dFeat)
		S_DELETE(dFeat, "create_phrase_context", error);
}

/*
 * creates the phrase context data in case it is a pause.
 * differs from create_phrase_context in that either next or previous
 * phone's phrase is used (if prev exists use it, else use next)
 */
static void create_phrase_context_pause(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error)
{
	const SObject *dFeat;
	SItem *tmp;
	s_bool use_next;

	S_CLR_ERR(error);

	tmp = SItemPrev(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context_pause",
				  "Call to \"SItemPrev\" failed"))
		return;

	if (tmp != NULL)
		use_next = FALSE;
	else
		use_next = TRUE;

	/* the number of syllables in the previous phrase */
	dFeat = SItemPathToFeatProc(item, "p.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_syls", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "p.phrase.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_pause_cleanup;

	}

	/* the number of syllables in this phrase */
	if (use_next)
	{
		dFeat = SItemPathToFeatProc(item, "n.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_syls",
								   error);
	}
	else
	{
		dFeat = SItemPathToFeatProc(item, "p.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_syls",
								   error);
	}
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_syls", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phrase.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_pause_cleanup;

	}

	/* the number of syllables in the next phrase */
	dFeat = SItemPathToFeatProc(item, "n.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_syls", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "n.phrase.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_pause_cleanup;

	}

	/* the number of words in the previous phrase */
	dFeat = SItemPathToFeatProc(item, "p.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_words", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "p.phrase.words.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_pause_cleanup;

	}

	/* the number of words in the this phrase */
	if (use_next)
	{
		dFeat = SItemPathToFeatProc(item, "n.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_words",
								   error);
	}
	else
	{
		dFeat = SItemPathToFeatProc(item, "p.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_words",
								   error);
	}
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phrase.words.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_pause_cleanup;

	}

	/* the number of words in the next phrase */
	dFeat = SItemPathToFeatProc(item, "n.R:SylStructure.parent.parent.R:Phrase.parent.phrase_num_words", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "n.phrase.words.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_pause_cleanup;

	}

	/* the type of the current phrase */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Phrase.parent.phrase_type", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phrase.type", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_pause_cleanup;

	}

	/* TOBI endtone of the current phrase */
	/* item, syllable, word, phrase, last word, last syllable */
	if (use_next)
	{
		dFeat = SItemPathToFeature(item, "n.R:SylStructure.parent.parent.R:Phrase.parent.daughtern.R:SylStructure.daughtern.endtone",
									error);
	}
	else
	{
		dFeat = SItemPathToFeature(item, "p.R:SylStructure.parent.parent.R:Phrase.parent.daughtern.R:SylStructure.daughtern.endtone",
									error);
	}
	S_CHK_ERR(error, S_CONTERR,
				  "create_phrase_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "tobi.endtone", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_phrase_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto phrase_context_pause_cleanup;

	}

	return;

phrase_context_pause_cleanup:
	if (dFeat != NULL)
		S_DELETE(dFeat, "create_phrase_context_pause", error);
}

/*
 * creates the utterance context data
 */
static void create_utterance_context(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error)
{
	const SObject *dFeat;

	S_CLR_ERR(error);

	/* number of phrases before this in utterance */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Phrase.parent.phrase_pos_utt", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_utterance_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phrases.from.utt.start", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_utterance_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto create_utterance_context_cleanup;

	}

	/* number of phrases after this in utterance */
	dFeat = SItemPathToFeatProc(item, "R:SylStructure.parent.parent.R:Phrase.parent.phrase_pos_utt_rev", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_utterance_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phrases.from.utt.end", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_utterance_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto create_utterance_context_cleanup;

	}

	/* the number of syllables in this utterence */
	dFeat = SItemPathToFeatProc(item, "utt_num_syls", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_utterance_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "utterance.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_utterance_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto create_utterance_context_cleanup;

	}

	/* the number of words in this utterence */
	dFeat = SItemPathToFeatProc(item, "utt_num_words", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_utterance_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "utterance.words.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_utterance_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto create_utterance_context_cleanup;

	}

	/* the number of phrases in this utterence */
	dFeat = SItemPathToFeatProc(item, "utt_num_phrases", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_utterance_context",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "utterance.phrases.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_utterance_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto create_utterance_context_cleanup;

	}

	return;

create_utterance_context_cleanup:
	if (dFeat != NULL)
		S_DELETE(dFeat, "create_utterance_context", error);
}

/*
 * creates the utterance context data in case it is a pause.
 */
static void create_utterance_context_pause(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error)
{
	const SObject *dFeat;
	SItem *tmp;
	s_bool use_next;

	S_CLR_ERR(error);

	tmp = SItemPrev(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_utterance_context_pause",
				  "Call to \"SItemPrev\" failed"))
		return;

	if (tmp != NULL)
		use_next = FALSE;
	else
		use_next = TRUE;

	/* number of phrases before this in utterance */
	if (use_next)
	{
		dFeat = SItemPathToFeatProc(item, "n.R:SylStructure.parent.parent.R:Phrase.parent.phrase_pos_utt",
								   error);
	}
	else
	{
		dFeat = SItemPathToFeatProc(item, "p.R:SylStructure.parent.parent.R:Phrase.parent.phrase_pos_utt",
								   error);
	}
	S_CHK_ERR(error, S_CONTERR,
				  "create_utterance_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phrases.from.utt.start", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_utterance_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto create_utterance_context_pause_cleanup;

	}

	/* number of phrases after this in utterance */
	if (use_next)
	{
		dFeat = SItemPathToFeatProc(item, "n.R:SylStructure.parent.parent.R:Phrase.parent.phrase_pos_utt_rev",
								   error);
	}
	else
	{
		dFeat = SItemPathToFeatProc(item, "p.R:SylStructure.parent.parent.R:Phrase.parent.phrase_pos_utt_rev",
								   error);
	}
	S_CHK_ERR(error, S_CONTERR,
				  "create_utterance_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "phrases.from.utt.end", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_utterance_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto create_utterance_context_pause_cleanup;

	}

	/* the number of syllables in this utterence */
	dFeat = SItemPathToFeatProc(item, "utt_num_syls", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_utterance_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "utterance.syls.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_utterance_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto create_utterance_context_pause_cleanup;

	}

	/* the number of words in this utterence */
	dFeat = SItemPathToFeatProc(item, "utt_num_words", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_utterance_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "utterance.words.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_utterance_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto create_utterance_context_pause_cleanup;

	}

	/* the number of phrases in this utterence */
	dFeat = SItemPathToFeatProc(item, "utt_num_phrases", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_utterance_context_pause",
				  "Call to \"SItemPathToFeatProc\" failed");

	if (dFeat != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "utterance.phrases.num", dFeat, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_utterance_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto create_utterance_context_pause_cleanup;

	}

	return;

create_utterance_context_pause_cleanup:
	if (dFeat != NULL)
		S_DELETE(dFeat, "create_utterance_context_pause", error);
}

/*
 * creates the stress context data
 */
static void create_stress_context(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error)
{
	const SObject *featPath;
	const SObject *newfeatPath;
	int level;
	const char* syllablepart;

	S_CLR_ERR(error);

	/* syllablepart */
	featPath = SItemPathToFeature(item, "syllablepart", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context",
				  "Call to \"SItemPathToFeature\" failed"))
		return;

	if (featPath != NULL)
	{
		/* unboxing */
		syllablepart = SObjectGetString (featPath, error);

		/* convert 'nucleus' value in 'coda' */
		if( s_strcmp (syllablepart, "nucleus", error) == 0 )
			featPath = SObjectSetString ( "coda", error );

		SHTSLabelDataCollectorSetFeature(self, "syl.part", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			return;
	}

	/* current syllable stress */
	featPath = SItemPathToFeature(item, "R:SylStructure.parent.R:Syllable.stress",
				      error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		level = get_stress_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context",
					  "Call to \"get_stress_level\" failed"))
			goto stress_cleanup;
		newfeatPath = SObjectSetInt(level, error);
		if (S_CHK_ERR(error, S_CONTERR,
						"create_stress_context",
						"Call to \"SObjectSetInt\" failed"))
			goto stress_cleanup;

		SHTSLabelDataCollectorSetFeature(self, "stress", newfeatPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto stress_cleanup;
	}

	/* previous previous phoneme stress */
	featPath = compute_stress(item, "p.p", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "prev.prev.stress", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto stress_cleanup;
	}

	/* previous phoneme stress */
	featPath = compute_stress(item, "p", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "prev.stress", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto stress_cleanup;
	}

	/* next phoneme stress */
	featPath = compute_stress(item, "n", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "next.stress", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto stress_cleanup;
	}

	/* next next phoneme stress */
	featPath = compute_stress(item, "n.n", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "next.next.stress", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto stress_cleanup;
	}

	/* the number of stressed syllables before the current syllable in the current phrase */
	featPath = SItemPathToFeatProc(item, "R:SylStructure.parent.R:Syllable.syllable_stress_in", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "stressed.syls.before", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto stress_cleanup;
	}

	/* the number of stressed syllables after the current syllable in the current phrase */
	featPath = SItemPathToFeatProc(item, "R:SylStructure.parent.R:Syllable.syllable_stress_out", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "stressed.syls.after", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto stress_cleanup;
	}

	/* the number of syllables from the previous stressed syllable to the current syllable */
	featPath = SItemPathToFeatProc(item, "R:SylStructure.parent.R:Syllable.syllable_stress_all_in", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "syls.from.prev.stress", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto stress_cleanup;
	}

	/* the number of syllables from the current syllable to the next stressed syllable */
	featPath = SItemPathToFeatProc(item, "R:SylStructure.parent.R:Syllable.syllable_stress_all_out", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "syls.to.next.stress", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto stress_cleanup;
	}

	return;

stress_cleanup:
	if (featPath != NULL)
		S_DELETE(featPath, "create_stress_context", error);
}

/*
 * creates the stress context data
 * differs from create_stress_context in that previous/next phone's syllable is queried
 * and not previous/next syllable
 */
static void create_stress_context_pause(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error)
{
	const SObject *featPath;

	S_CLR_ERR(error);

	/* previous previous phoneme stress */
	featPath = compute_stress(item, "p.p", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context_pause",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "prev.prev.stress", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto stress_pause_cleanup;
	}

	/* previous phoneme stress */
	featPath = compute_stress(item, "p", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context_pause",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "prev.stress", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto stress_pause_cleanup;
	}

	/* next phoneme stress */
	featPath = compute_stress(item, "n", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context_pause",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "next.stress", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto stress_pause_cleanup;
	}

	/* next next phoneme stress */
	featPath = compute_stress(item, "n.n", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_stress_context_pause",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "next.next.stress", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_stress_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto stress_pause_cleanup;
	}

	return;

stress_pause_cleanup:
	if (featPath != NULL)
		S_DELETE(featPath, "create_stress_context_pause", error);
}

/*
 * creates the accent context data
 */
static void create_accent_context(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error)
{
	const SObject *featPath;
	sint32 level;

	S_CLR_ERR(error);

	/* previous syllable accent */
	featPath = SItemPathToFeature(item, "R:SylStructure.parent.R:Syllable.p.accent",
				      error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_accent_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		level = get_stress_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context",
					  "Call to \"get_stress_level\" failed"))
			goto accent_cleanup;
		featPath = SObjectSetInt(level, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context",
					  "Call to \"SObjectSetInt\" failed"))
			goto accent_cleanup;

		SHTSLabelDataCollectorSetFeature(self, "p.accent", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto accent_cleanup;
		S_DELETE(featPath, "create_accent_context", error);
	}

	/* current syllable accent */
	featPath = SItemPathToFeature(item, "R:SylStructure.parent.R:Syllable.accent",
					error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_accent_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		level = get_stress_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context",
					  "Call to \"get_stress_level\" failed"))
			goto accent_cleanup;
		featPath = SObjectSetInt(level, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context",
					  "Call to \"SObjectSetInt\" failed"))
			goto accent_cleanup;

		SHTSLabelDataCollectorSetFeature(self, "accent", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto accent_cleanup;
		S_DELETE(featPath, "create_accent_context", error);
	}

	/* next syllable accent */
	featPath = SItemPathToFeature(item, "R:SylStructure.parent.R:Syllable.n.accent",
					error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_accent_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		level = get_stress_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context",
					  "Call to \"get_stress_level\" failed"))
			goto accent_cleanup;
		featPath = SObjectSetInt(level, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context",
					  "Call to \"SObjectSetInt\" failed"))
			goto accent_cleanup;

		SHTSLabelDataCollectorSetFeature(self, "n.accent", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto accent_cleanup;
		S_DELETE(featPath, "create_accent_context", error);
	}

	/* the number of acceted syllables before the current syllable in the current phrase */
	featPath = SItemPathToFeature(item, "R:SylStructure.parent.R:Syllable.syllable_accent_in", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_accent_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "accented.syls.before", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto accent_cleanup;
	}

	/* the number of stressed syllables after the current syllable in the current phrase */
	featPath = SItemPathToFeature(item, "R:SylStructure.parent.R:Syllable.syllable_accent_out", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_accent_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "accented.syls.after", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto accent_cleanup;
	}

	/* the number of syllables from the previous accented syllable to the current syllable */
	featPath = SItemPathToFeature(item, "R:SylStructure.parent.R:Syllable.syllable_accent_all_in", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_accent_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "syls.from.prev.accent", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto accent_cleanup;
	}

	/* the number of syllables from the current syllable to the next accented syllable */
	featPath = SItemPathToFeature(item, "R:SylStructure.parent.R:Syllable.syllable_accent_all_out", error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_accent_context",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		SHTSLabelDataCollectorSetFeature(self, "syls.to.next.accent", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto accent_cleanup;
	}

	return;

accent_cleanup:
	if (featPath != NULL)
		S_DELETE(featPath, "create_accent_context", error);
}

/*
 * creates the accent context data
 * differs from create_accent_context in that previous/next phone's syllable is queried
 * and not previous/next syllable
 */
static void create_accent_context_pause(SELFPARAMETERTYPE *self, const SItem *item, s_erc *error)
{
	const SObject *featPath;
	sint32 level;

	S_CLR_ERR(error);

	/* previous syllable accent */
	featPath = SItemPathToFeature(item, "p.R:SylStructure.parent.R:Syllable.accent",
				      error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_accent_context_pause",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		level = get_accent_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context_pause",
					  "Call to \"get_accent_level\" failed"))
			goto accent_pause_cleanup;
		featPath = SObjectSetInt(level, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context_pause",
					  "Call to \"SObjectSetInt\" failed"))
			goto accent_pause_cleanup;

		SHTSLabelDataCollectorSetFeature(self, "p.accent", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto accent_pause_cleanup;
		S_DELETE(featPath, "create_accent_context_pause", error);
	}

	/* current syllable accent: not present in pause */

	/* next syllable accent */
	featPath = SItemPathToFeature(item, "n.R:SylStructure.parent.R:Syllable.accent",
					error);
	S_CHK_ERR(error, S_CONTERR,
				  "create_accent_context_pause",
				  "Call to \"SItemPathToFeature\" failed");

	if (featPath != NULL)
	{
		level = get_accent_level(featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context_pause",
					  "Call to \"get_accent_level\" failed"))
			goto accent_pause_cleanup;
		featPath = SObjectSetInt(level, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context_pause",
					  "Call to \"SObjectSetInt\" failed"))
			goto accent_pause_cleanup;

		SHTSLabelDataCollectorSetFeature(self, "n.accent", featPath, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_accent_context_pause",
					  "Call to \"SHTSLabelDataCollectorSetFeature\" failed"))
			goto accent_pause_cleanup;
		S_DELETE(featPath, "create_accent_context_pause", error);
	}

	return;

accent_pause_cleanup:
	if (featPath != NULL)
		S_DELETE(featPath, "create_accent_context_pause", error);
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	S_CLR_ERR(error);

	SHTSLabelsDataCollectorFeatProc *self = obj;

	S_CLR_ERR(error);

	self->features = S_MAP(S_NEW(SMapList, error));
	if (S_CHK_ERR(error, S_CONTERR,
			  "InitLabelsDataCollector",
			  "Failed to create new map-list features"))
		self->features = NULL;
}

static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}

static void Destroy(void *obj, s_erc *error)
{
	SHTSLabelsDataCollectorFeatProc *self = obj;


	S_CLR_ERR(error);

	if (self->features != NULL)
		S_DELETE(self->features, "Destroy", error);
}


static SObject *Run(const SFeatProcessor *self, const SItem *item,
					s_erc *error)
{
	//SELFPARAMETERTYPE *HTSProc = (SELFPARAMETERTYPE *)self;
	const SItem *segItem;
	s_bool is_pause;


	S_CLR_ERR(error);

	SELFPARAMETERTYPE *HTSProc = S_NEW( SELFPARAMETERTYPE, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"S_NEW\" failed"))
		goto quit_error;

	if (item == NULL)
		return NULL;

	segItem = SItemAs(item, "Segment", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SItemAs\" failed"))
		goto quit_error;

	if (segItem == NULL)
		return NULL;

	is_pause = segment_is_pause(segItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"segment_is_pause\" failed"))
		goto quit_error;

	/* get phone context */
	create_phone_context(HTSProc, segItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"create_phone_context\" failed"))
		goto quit_error;

	if (is_pause)
	{
		/* syllable context */
		create_syl_context_pause(HTSProc, segItem, error);
		S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_syl_context_pause\" failed");

		/* word context */
		create_word_context_pause(HTSProc, segItem, error);
		S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_word_context_pause\" failed");

		/* phrase context */
		create_phrase_context_pause(HTSProc, segItem, error);
		S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_phrase_context_pause\" failed");

		/* utterance context */
		create_utterance_context_pause(HTSProc, segItem, error);
		S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_utterance_context_pause\" failed");

		/* stress context */
		create_stress_context_pause(HTSProc, segItem, error);
		S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_stress_context_pause\" failed");

		/* accent context */
		create_accent_context_pause(HTSProc, segItem, error);
		S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_accent_context_pause\" failed");
	}
	else
	{
		/* syllable context */
		create_syl_context(HTSProc, segItem, error);
		S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_syl_context\" failed");

		/* word context */
		create_word_context(HTSProc, segItem, error);
		S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_word_context\" failed");

		/* phrase context */
		create_phrase_context(HTSProc, segItem, error);
		S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_phrase_context\" failed");

		/* utterance context */
		create_utterance_context(HTSProc, segItem, error);
		S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_utterance_context\" failed");

		/* stress context */
		create_stress_context(HTSProc, segItem, error);
		S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_stress_context\" failed");

		/* accent context */
		create_accent_context(HTSProc, segItem, error);
		S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"create_accent_context\" failed");
	}

	/* prepare the object to be returned */
	//SObjectIncRef((SObject*) HTSProc);

	/* all OK here */
	return (SObject*) HTSProc;

	/* error cleanup */
quit_error:
	if (HTSProc)
	{
		S_DELETE(HTSProc, "error destruction in Run", error);
	}
	return NULL;

	S_UNUSED(self);
}


/************************************************************************************/
/*                                                                                  */
/* SHTSLabelsConfigurableFeatProc class initialization                                   */
/*                                                                                  */
/************************************************************************************/

static SHTSLabelsDataCollectorFeatProcClass HTSLabelsDataCollectorFeatProcClass =
{
	/* SFeatProcessorClass */
	{
		/* SObjectClass */
		{
			"SFeatProcessor:SHTSLabelsDataCollectorFeatProc",
			sizeof(SHTSLabelsDataCollectorFeatProc),
			{ 0, 1},
			Init,            /* init    */
			Destroy,         /* destroy */
			Dispose,         /* dispose */
			NULL,            /* compare */
			NULL,            /* print   */
			NULL,            /* copy    */
		},
		NULL,               /* initialize */
		Run,                  /* run     */
	},
	/* HTSLabelsDataCollectorFeatProcClass */
	SHTSLabelDataCollectorGetFeature,       /* get_feature    */
	SHTSLabelDataCollectorFeatureIsPresent,	/* has_feature    */
};
