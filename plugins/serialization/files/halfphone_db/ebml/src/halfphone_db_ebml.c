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
/* A halfphone database class implementation with the halfphone database entries in */
/* a SMap structure read from a EBML format file.                                   */
/* Inherits from SHalfphoneDB.                                                      */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "halfphone_db_ebml.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SHalfphoneDBEbmlClass HalfphoneDBEbmlClass; /* SHalfphoneDBEbml class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_halfphone_db_ebml_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&HalfphoneDBEbmlClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_halfphone_db_ebml_class_reg",
			  "Failed to register SHalfphoneDBEbmlClass");
}


S_LOCAL void _s_halfphone_db_ebml_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&HalfphoneDBEbmlClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_halfphone_db_ebml_class_reg",
			  "Failed to free SHalfphoneDBEbmlClass");
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

static void Init(void *obj, s_erc *error)
{
	SHalfphoneDBEbml *self = obj;


	S_CLR_ERR(error);
	self->units = NULL;
	self->catalogue = (SMap*)S_NEW("SMapHashTable", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Init",
				  "Failed to create new 'SMapHashTable' object"))
		return;

	SMapHashTableResize(S_MAPHASHTABLE(self->catalogue), 100, error); /* working on about (50 phones * 2) */
	if (S_CHK_ERR(error, S_CONTERR,
				  "Init",
				  "Call to \"SMapHashTableResize\" failed"))
	{
		S_DELETE(self->catalogue, "Init", error);
		return;
	}

	/* create a relation to hold all units */
	self->units = (SRelation*)S_NEW("SRelation", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Init",
				  "Failed to create new 'SRelation' object"))
		return;

	SRelationInit(&(self->units), "halfphone_units", error);
	S_CHK_ERR(error, S_CONTERR,
			  "Init",
			  "Failed to initialize new 'SRelation' object");
}



static void Destroy(void *obj, s_erc *error)
{
	SHalfphoneDBEbml *self = obj;


	S_CLR_ERR(error);
	if (self->catalogue != NULL)
		S_DELETE(self->catalogue, "Destroy", error);

	if (self->units != NULL)
		S_FORCE_DELETE(self->units, "Destroy", error);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static const char *GetName(const SHalfphoneDB *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->name;
}


static const char *GetDescription(const SHalfphoneDB *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->description;
}


static const char *GetLanguage(const SHalfphoneDB *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->language;
}


static const char *GetLangCode(const SHalfphoneDB *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const char*)self->info->lang_code;
}


static const s_version *SGetVersion(const SHalfphoneDB *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self->info == NULL)
		return NULL;


	return (const s_version*)&(self->info->version);
}

static const SObject *GetFeature(const SHalfphoneDB *self, const char *key,
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


static const SList *GetUnits(const SHalfphoneDB *self, const char *unit_type,
							 const char *left_context, const char *right_context,
							 uint min_triphones, uint min_diphones, s_erc *error)
{
	const SList *candidates;
	const SMap *unitContainer;
	const SHalfphoneDBEbml *db = (SHalfphoneDBEbml*)self;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetUnits",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (unit_type == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetUnits",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	unitContainer = (SMap*)SMapGetObjectDef(db->catalogue, unit_type, NULL,
											error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetUnits",
				  "Call to \"SMapGetObject\" failed"))
		return NULL;

	if (unitContainer == NULL)
		return NULL;

	if ((right_context !=  NULL) && (left_context != NULL))
	{
		/* unit has left and right context */
		char *context;
		size_t size;


		size = s_strsize(left_context, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetUnits",
					  "Call to \"s_strsize\" failed"))
			return NULL;

		size += s_strsize(right_context, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetUnits",
					  "Call to \"s_strsize\" failed"))
			return NULL;

		context = S_CALLOC(char, size + 2);
		if (context == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "GetUnits",
					  "Failed to allocate memory for 'char' object");
			return NULL;
		}

		s_strcat(context, left_context, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetUnits",
					  "Call to \"s_strcat\" failed"))
		{
			S_FREE(context);
			return NULL;
		}

		s_strcat(context, ":", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetUnits",
					  "Call to \"s_strcat\" failed"))
		{
			S_FREE(context);
			return NULL;
		}

		s_strcat(context, right_context, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetUnits",
					  "Call to \"s_strcat\" failed"))
		{
			S_FREE(context);
			return NULL;
		}

		candidates = (SList*)SMapGetObject(unitContainer, context, error);
		S_FREE(context);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetUnits",
					  "Call to \"SMapGetObject\" failed"))
			return NULL;

		if (candidates != NULL)
		{
			size = SListSize(candidates, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "GetUnits",
						  "Call to \"SListSize\" failed"))
				return NULL;

			if (size >= min_triphones)
				return candidates;
		}

		/* else we continue */
	}

	if (left_context != NULL)
	{
		/* unit has left context */
		size_t size;


		candidates = (SList*)SMapGetObject(unitContainer, left_context, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "GetUnits",
					  "Call to \"SMapGetObject\" failed"))
			return NULL;

		if (candidates != NULL)
		{
			size = SListSize(candidates, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "GetUnits",
						  "Call to \"SListSize\" failed"))
				return NULL;

			if (size >= min_diphones)
				return candidates;
		}

		/* else we continue */
	}

	candidates = (SList*)SMapGetObject(unitContainer, "all-units", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetUnits",
				  "Call to \"SMapGetObject\" failed"))
		return NULL;

	return candidates;
}


/************************************************************************************/
/*                                                                                  */
/* SHalfphoneDBEbml class initialization                                            */
/*                                                                                  */
/************************************************************************************/

static SHalfphoneDBEbmlClass HalfphoneDBEbmlClass =
{
	/* SObjectClass */
	{
		"SHalfphoneDB:SHalfphoneDBEbml",
		sizeof(SHalfphoneDBEbml),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SHalfphoneDBClass */
	GetName,             /* get_name        */
	GetDescription,      /* get_description */
	GetLanguage,         /* get_language    */
	GetLangCode,         /* get_lang_code   */
	SGetVersion,         /* get_version     */
	GetFeature,          /* get_feature     */
	GetUnits             /* get_units       */
};
