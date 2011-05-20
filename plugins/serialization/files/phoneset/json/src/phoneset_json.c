/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Phoneset class implementation with the phones in a SMap                        */
/* structure read from a JSON format file. The phones in the phoneset               */
/* have binary features, i.e. a phone either has a specific named                   */
/* feature or it doesn't.                                                           */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "phoneset_json.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SPhonesetJSONClass PhonesetJSONClass; /* SPhonesetJSON class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_phoneset_json_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&PhonesetJSONClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phoneset_json_class_reg",
			  "Failed to register SPhonesetJSONClass");
}


S_LOCAL void _s_phoneset_json_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&PhonesetJSONClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_phoneset_json_class_free",
			  "Failed to free SPhonesetJSONClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SPhonesetJSON *self = obj;


	S_CLR_ERR(error);
	self->phones = NULL;
}


static void Destroy(void *obj, s_erc *error)
{
	SPhonesetJSON *self = obj;


	S_CLR_ERR(error);
	if (self->phones != NULL)
		S_DELETE(self->phones, "Destroy", error);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static const char *GetName(const SPhoneset *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->name;
}


static const char *GetDescription(const SPhoneset *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->description;
}


static const char *GetLanguage(const SPhoneset *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->language;
}


static const char *GetLangCode(const SPhoneset *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->lang_code;
}


static const s_version *SGetVersion(const SPhoneset *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const s_version*)&(self->info->version);
}

static const SObject *GetFeature(const SPhoneset *self, const char *key,
								 s_erc *error)
{
	const SObject *feature;


	S_CLR_ERR(error);
	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetFeature",
				  "Argument \"key\" is NULL");
		return NULL;
	}

	if (self->features == NULL)
		return NULL;

	feature = SMapGetObjectDef(self->features, key, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetFeature",
				  "Call to \"SMapGetObjectDef\" failed"))
		return NULL;

	return feature;
}

static s_bool PhoneHasFeature(const SPhoneset *self, const char *phone,
							  const char *feature, s_erc *error)
{
	SPhonesetJSON *phoneset = S_PHONESET_JSON(self);
	s_bool is_present;
	const SObject *tmp;
	const SList *phoneFeatureList;
	SObject *featureString;


	S_CLR_ERR(error);
	if (phone == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "PhoneHasFeature",
				  "Argument \"phone\" is NULL");
		return FALSE;
	}

	if (feature == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "PhoneHasFeature",
				  "Argument \"feature\" is NULL");
		return FALSE;
	}

	tmp = SMapGetObjectDef(phoneset->phones, phone, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "PhoneHasFeature",
				  "Call to \"SMapGetObjectDef\" failed"))
		return FALSE;

	if (tmp == NULL)
		return FALSE; /* phone not in phoneset */

	/* cast to make sure */
	phoneFeatureList = S_CAST(tmp, SList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "PhoneHasFeature",
				  "Phoneset features is not a list"))
		return FALSE;

	featureString = SObjectSetString(feature, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "PhoneHasFeature",
				  "Call to \"SObjectSetString\" failed"))
		return FALSE;

	is_present = SListValPresent(phoneFeatureList, featureString, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "PhoneHasFeature",
				  "Call to \"SListValPresent\" failed"))
	{
		S_DELETE(featureString, "PhoneHasFeature", error);
		return FALSE;
	}

	S_DELETE(featureString, "PhoneHasFeature", error);
	return is_present;
}


static s_bool HasPhone(const SPhoneset *self, const char *phone, s_erc *error)
{
	const SObject *tmp;
	SPhonesetJSON *phoneset = S_PHONESET_JSON(self);


	S_CLR_ERR(error);
	if (phone == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "HasPhone",
				  "Argument \"phone\" is NULL");
		return FALSE;
	}

	tmp = SMapGetObjectDef(phoneset->phones, phone, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "HasPhone",
				  "Call to \"SMapGetObjectDef\" failed"))
		return FALSE;

	if (tmp == NULL)
		return FALSE; /* phone not in phoneset */

	return TRUE;
}


static const SList *GetPhoneFeatures(const SPhoneset *self, const char *phone,
									 s_erc *error)
{
	const SObject *tmp;
	const SList *phoneFeatureList;
	SPhonesetJSON *phoneset = S_PHONESET_JSON(self);


	S_CLR_ERR(error);
	if (phone == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetPhoneFeatures",
				  "Argument \"phone\" is NULL");
		return NULL;
	}

	tmp = SMapGetObjectDef(phoneset->phones, phone, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetPhoneFeatures",
				  "Call to \"SMapGetObjectDef\" failed"))
		return NULL;

	if (tmp == NULL)
		return NULL; /* phone not in phoneset */

	/* cast to make sure */
	phoneFeatureList = S_CAST(tmp, SList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetPhoneFeatures",
				  "Phoneset features is not a list"))
		return NULL;

	return phoneFeatureList;
}


static SList *GetPhoneList(const SPhoneset *self, s_erc *error)
{
	SList *phoneList;
	SPhonesetJSON *phoneset = S_PHONESET_JSON(self);


	S_CLR_ERR(error);
	phoneList = SMapGetKeys(phoneset->phones, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetPhoneList",
				  "Call to \"SMapGetKeys\" failed"))
		return NULL;

	return phoneList;
}


/************************************************************************************/
/*                                                                                  */
/* SPhoneset class initialization                                                   */
/*                                                                                  */
/************************************************************************************/

static SPhonesetJSONClass PhonesetJSONClass =
{
	/* SObjectClass */
	{
		"SPhoneset:SPhonesetJSON",
		sizeof(SPhonesetJSON),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SPhonesetClass */
	GetName,             /* get_name           */
	GetDescription,      /* get_description    */
	GetLanguage,         /* get_language       */
	GetLangCode,         /* get_lang_code      */
	SGetVersion,         /* get_version        */
	GetFeature,          /* get_feature        */
	PhoneHasFeature,     /* phone_has_feature  */
	HasPhone,            /* has_phone          */
	GetPhoneFeatures,    /* get_phone_features */
	GetPhoneList         /* get_phone_list     */
};
