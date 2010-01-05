/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* Read a halfphone database from an EBML format file.                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "serialized_halfphone_db.h"
#include "element_id.h"
#include "ebml.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void read_halfphone_db_def(SHalfphoneDBEbml *db, SEbmlRead *ebmlReader,
								  s_erc *error);

static void read_feature(SEbmlRead *ebmlReader,
						 char **feat_name, uint32 feat_name_id,
						 SObject **featObject, uint32 feat_object_id,
						 s_erc *error);

static void read_halfphone_db_features(SHalfphoneDBEbml *db, SEbmlRead *ebmlReader,
									   s_erc *error);

static void read_halfphone_db_units(SHalfphoneDBEbml *db, SEbmlRead *ebmlReader,
									s_erc *error);

static void set_unit(SHalfphoneDBEbml *db, SItem *unit, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SHalfphoneDBEbml *s_read_halfphone_db_ebml(SDatasource *ds, s_erc *error)
{
	SHalfphoneDBEbml *db = NULL;
	SEbmlRead *ebmlReader = NULL;
	uint32 id;
	int scomp;
	s_bool container_exhausted;


	S_CLR_ERR(error);

	/* create and initialize ebml reader */
	ebmlReader = (SEbmlRead*)S_NEW("SEbmlRead", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_halfphone_db_ebml",
				  "Failed to create new 'SEbmlRead' object"))
		goto quit_error;

	S_EBMLREAD_CALL(ebmlReader, read_init)(&ebmlReader, ds, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_halfphone_db_ebml",
				  "Failed to initialize 'SEbmlRead' object"))
		goto quit_error;

	/* check that we have the correct doc type */
	scomp = s_strcmp(ebmlReader->header->doctype, "spct_halfphone_db", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_halfphone_db_ebml",
				  "Call to \"s_strcmp\" failed"))
		goto quit_error;

	if (scomp != 0)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_read_halfphone_db_ebml",
				  "Ebml file format not of type 'spct_halfphone_db', read format is '%s'",
				  ebmlReader->header->doctype);
		goto quit_error;
	}

	/* create halfphone database */
	db = (SHalfphoneDBEbml*)S_NEW("SHalfphoneDBEbml", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_halfphone_db_ebml",
				  "Failed to create new 'SHalfphoneDBEbml' object"))
		goto quit_error;

	/* read top level container */
	id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_halfphone_db_ebml",
				  "ebmlRead method \"container\" failed"))
		goto quit_error;

	/* sanity check id */
	S_EBML_ID_SANITY(id, S_HALFPHONE_DB_EBML_ID,
					 "s_read_halfphone_db_ebml",
					 "ID mismatch", error);

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_halfphone_db_ebml",
					  "ebmlRead method \"container_at_end\" failed"))
			goto quit_error;

		if (container_exhausted)
			break; /* we are finished reading the halfphone database file */

		/* peek id for halfphone database elements */
		id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_halfphone_db_ebml",
					  "ebmlRead method \"peek_id\" failed"))
			goto quit_error;

		switch(id)
		{
		case S_HALFPHONE_DB_EBML_DEF:
		{
			read_halfphone_db_def(db, ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_read_halfphone_db_ebml",
						  "Call to \"read_halfphone_db_def\" failed"))
				goto quit_error;
			break;
		}
		case S_HALFPHONE_DB_EBML_FEATURES:
		{
			read_halfphone_db_features(db, ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_read_halfphone_db_ebml",
						  "Call to \"read_halfphone_db_features\" failed"))
				goto quit_error;
			break;
		}
		case S_HALFPHONE_DB_EBML_UNITS:
		{
			read_halfphone_db_units(db, ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_read_halfphone_db_ebml",
						  "Call to \"read_halfphone_db_units\" failed"))
				goto quit_error;

			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "s_read_halfphone_db_ebml",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_read_halfphone_db_ebml",
						  "ebmlRead method \"element_skip\" failed"))
				goto quit_error;
		}
	}

	/* if we get here then everything was OK */
	goto quit;


	/* errors start clean up code here */
quit_error:
	if (db != NULL)
		S_DELETE(db, "s_read_halfphone_db_ebml", error); /* sets db = NULL */

	/* normal exit start clean up code here */
quit:
	if (ebmlReader != NULL)
		S_DELETE(ebmlReader, "s_read_halfphone_db_ebml", error);

	return db;
}

/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void read_halfphone_db_def(SHalfphoneDBEbml *db, SEbmlRead *ebmlReader, s_erc *error)
{
	SHalfphoneDB *baseDB = S_HALFPHONE_DB(db);
	uint32 id;
	s_bool container_exhausted;


	S_CLR_ERR(error);

	/* read S_HALFPHONE_DB_EBML_DEF container */
	id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_halfphone_db_def",
				  "ebmlRead method \"container\" failed"))
		return;

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_halfphone_db_def",
					  "ebmlRead method \"container_at_end\" failed"))
			return;

		if (container_exhausted)
			break;

		/* peek id  */
		id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_halfphone_db_def",
					  "ebmlRead method \"peek_id\" failed"))
			return;

		switch(id)
		{
		case S_HALFPHONE_DB_EBML_DEF_NAME:
		{
			baseDB->info->name = S_EBMLREAD_CALL(ebmlReader, read_ascii)(ebmlReader, &id,
																		 error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_halfphone_db_def",
						  "ebmlRead method \"read_ascii\" failed"))
				return;

			break;
		}
		case S_HALFPHONE_DB_EBML_DEF_LANGUAGE:
		{
			baseDB->info->language = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id,
																			error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_halfphone_db_def",
						  "ebmlRead method \"read_utf8\" failed"))
				return;

			break;
		}
		case S_HALFPHONE_DB_EBML_DEF_LANG_CODE:
		{
			baseDB->info->lang_code = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id,
																			 error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_halfphone_db_def",
						  "ebmlRead method \"read_utf8\" failed"))
				return;

			break;
		}
		case S_HALFPHONE_DB_EBML_DEF_DESCRIPTION:
		{
			baseDB->info->description = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id,
																			   error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_halfphone_db_def",
						  "ebmlRead method \"read_utf8\" failed"))
				return;

			break;
		}
		case S_HALFPHONE_DB_EBML_DEF_VERSION:
		{
			/* read S_HALFPHONE_DB_EBML_DEF_VERSION container */
			id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_halfphone_db_def",
						  "ebmlRead method \"container\" failed"))
				return;

			while (1)
			{
				container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_halfphone_db_def",
							  "ebmlRead method \"container_at_end\" failed"))
					return;

				if (container_exhausted)
					break;

				/* peek id  */
				id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_halfphone_db_def",
							  "ebmlRead method \"peek_id\" failed"))
					return;

				switch(id)
				{
				case S_HALFPHONE_DB_EBML_DEF_VERSION_MAJOR:
				{
					baseDB->info->version.major = S_EBMLREAD_CALL(ebmlReader, read_uint)(ebmlReader, &id,
																						 error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_halfphone_db_def",
								  "ebmlRead method \"read_uint\" failed"))
						return;

					break;
				}
				case S_HALFPHONE_DB_EBML_DEF_VERSION_MINOR:
				{
					baseDB->info->version.minor = S_EBMLREAD_CALL(ebmlReader, read_uint)(ebmlReader, &id,
																						 error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_halfphone_db_def",
								  "ebmlRead method \"read_uint\" failed"))
						return;

					break;
				}
				default:
					/* unknown elements, skip */
					S_WARNING(S_FAILURE,
							  "read_halfphone_db_def",
							  "Skipping element with unknown id '0x%x'",
							  id);

					S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_halfphone_db_def",
								  "ebmlRead method \"element_skip\" failed"))
						return;
				}
			}
			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_halfphone_db_def",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_halfphone_db_def",
						  "ebmlRead method \"element_skip\" failed"))
				return;
		}
	}

	return;
}


static void read_feature(SEbmlRead *ebmlReader,
						 char **feat_name, uint32 feat_name_id,
						 SObject **featObject, uint32 feat_object_id,
						 s_erc *error)
{
	uint32 id;


	S_CLR_ERR(error);

	*featObject = NULL;
	*feat_name = NULL;

	*feat_name = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_feature",
				  "ebmlRead method \"read_utf8\" failed"))
		return;

	/* sanity check id */
	S_EBML_ID_SANITY(id, feat_name_id,
					 "read_feature",
					 "ID mismatch", error);
	/*
	 * elements are ordered, so feat object must be next or error
	 */
	id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_feature",
				  "ebmlRead method \"peek_id\" failed"))
		goto quit_error;

	/* sanity check id */
	S_EBML_ID_SANITY(id, feat_object_id,
					 "read_feature",
					 "ID mismatch", error);

	*featObject = S_EBMLREAD_CALL(ebmlReader, read_object)(ebmlReader, &id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_feature",
				  "ebmlRead method \"read_object\" failed"))
		goto quit_error;

	if (*featObject == NULL)
	{
		/* could not read object, not an error so ebmlReader->read_object
		 * will log a warning
		 */
		S_FREE(*feat_name);
	}

	/* here everything went OK */
	return;

	/* errors here, clean up code */
quit_error:
	if (*featObject != NULL)
		S_FORCE_DELETE(*featObject, "read_feature", error);

	if (*feat_name != NULL)
		S_FREE(*feat_name);
}


static void read_halfphone_db_features(SHalfphoneDBEbml *db, SEbmlRead *ebmlReader,
									   s_erc *error)
{
	uint32 id;
	char *feat_name = NULL;
	SObject *featObject = NULL;
	s_bool container_exhausted;
	SHalfphoneDB *baseDB = S_HALFPHONE_DB(db);


	S_CLR_ERR(error);

	/* read S_HALFPHONE_DB_EBML_FEATURES container */
	id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_halfphone_db_features",
				  "ebmlRead method \"container\" failed"))
		goto quit_error;

	/* create features object for db, if not one already */
	if (baseDB->features == NULL)
	{
		baseDB->features = (SMap*)S_NEW("SMapList", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_halfphone_db_features",
					  "Failed to create new 'SMapList' object"))
			goto quit_error;;

		SMapListInit(&(baseDB->features), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_halfphone_db_features",
					  "Failed to initialize new 'SMapList' object"))
			goto quit_error;
	}

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_halfphone_db_features",
					  "ebmlRead method \"container_at_end\" failed"))
			goto quit_error;

		if (container_exhausted)
			break; /* we are finished reading the halfphone db features */

		/* peek id for halfphone db feature elements */
		id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_halfphone_db_features",
					  "ebmlRead method \"peek_id\" failed"))
			goto quit_error;

		switch(id)
		{
		case S_HALFPHONE_DB_EBML_FEATURES:
		{
			/* feature elements are ordered (feat name, feat object),
			 * so we do it in one go
			 */
			read_feature(ebmlReader,
						 &feat_name, S_HALFPHONE_DB_EBML_FEAT_NAME,
						 &featObject, S_HALFPHONE_DB_EBML_FEAT_OBJECT,
						 error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_halfphone_db_features",
						  "Call to \"read_feature\" failed"))
				goto quit_error;

			/* set feature */
			SMapSetObject(baseDB->features, feat_name, featObject, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_halfphone_db_features",
						  "Call to \"SMapSetObject\" failed"))
				goto quit_error;

			/* feat name is copied */
			S_FREE(feat_name); /* sets to NULL */
			featObject = NULL;
			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_halfphone_db_features",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_halfphone_db_features",
						  "ebmlRead method \"element_skip\" failed"))
				goto quit_error;
		}
	}

	/* here everything went OK */
	return;

	/* errors here, clean up code */
quit_error:
	if (feat_name != NULL)
		S_FREE(feat_name);

	if (featObject != NULL)
		S_DELETE(featObject, "read_halfphone_db_features", error);
}


static void read_halfphone_db_units(SHalfphoneDBEbml *db, SEbmlRead *ebmlReader,
									s_erc *error)
{
	uint32 id;
	s_bool container_exhausted;
	SItem *unit;
	char *feat_name = NULL;
	SObject *featObject = NULL;


	S_CLR_ERR(error);

	/* read S_HALFPHONE_DB_EBML_UNITS container */
	id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_halfphone_db_units",
				  "ebmlRead method \"container\" failed"))
		goto quit_error;

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_halfphone_db_units",
					  "ebmlRead method \"container_at_end\" failed"))
			goto quit_error;

		if (container_exhausted)
			break; /* we are finished reading the halfphone db units */

		/* peek id for halfphone db unit elements */
		id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_halfphone_db_units",
					  "ebmlRead method \"peek_id\" failed"))
			goto quit_error;

		/* create a new unit item */
		unit = SRelationAppend(db->units, NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_halfphone_db_units",
					  "Call to \"SRelationAppend\" failed"))
			goto quit_error;

		switch(id)
		{
		case S_HALFPHONE_DB_EBML_UNIT_FEATURES:
		{
			/* read S_HALFPHONE_DB_EBML_UNIT_FEATURES container */
			id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_halfphone_db_units",
						  "ebmlRead method \"container\" failed"))
				goto quit_error;

			while (1)
			{
				container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_halfphone_db_units",
							  "ebmlRead method \"container_at_end\" failed"))
					goto quit_error;

				if (container_exhausted)
					break; /* we are finished reading the halfphone db unit features */

				/* peek id for halfphone db unit feature elements */
				id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_halfphone_db_units",
							  "ebmlRead method \"peek_id\" failed"))
					goto quit_error;

				switch(id)
				{
				case S_HALFPHONE_DB_EBML_UNIT_FEAT_NAME:
				{
					char *feat_name = NULL;
					SObject *featObject = NULL;

					/* feature elements are ordered (feat name, feat object),
					 * so we do it in one go
					 */
					read_feature(ebmlReader,
								 &feat_name, S_HALFPHONE_DB_EBML_UNIT_FEAT_NAME,
								 &featObject, S_HALFPHONE_DB_EBML_UNIT_FEAT_OBJECT,
								 error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_halfphone_db_units",
								  "Call to \"read_feature\" failed"))
						goto quit_error;

					/* set feature */
					SItemSetObject(unit, feat_name, featObject, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_halfphone_db_units",
								  "Call to \"SItemSetObject\" failed"))
						goto quit_error;

					/* feat name is copied */
					S_FREE(feat_name); /* sets to NULL */
					featObject = NULL;
					break;
				}
				default:
					/* unknown elements, skip */
					S_WARNING(S_FAILURE,
							  "read_halfphone_db_units",
							  "Skipping element with unknown id '0x%x'",
							  id);

					S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_halfphone_db_units",
								  "ebmlRead method \"element_skip\" failed"))
						goto quit_error;
				}
			}

			/* features are read, set unit */
			set_unit(db, unit, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_halfphone_db_units",
						  "Call to \"set_unit\" failed"))
				goto quit_error;

			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_halfphone_db_units",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_halfphone_db_units",
						  "ebmlRead method \"element_skip\" failed"))
				goto quit_error;
		}
	}

	/* here everything went OK */
	return;

	/* errors here, clean up code */
quit_error:
	if (feat_name != NULL)
		S_FREE(feat_name);

	if (featObject != NULL)
		S_DELETE(featObject, "read_halfphone_db_units", error);
}


static void set_unit(SHalfphoneDBEbml *db, SItem *unit, s_erc *error)
{
	const char *unit_name;
	const char *unit_left_context;
	const char *unit_right_context;
	s_bool is_present;
	SMap *unitContainer;
	SList *allUnits;


	S_CLR_ERR(error);

	/* get unit name */
	unit_name = SItemGetName(unit, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_unit",
				  "Call to \"SItemGetName\" failed"))
		return;

	/* get unit left context, if present */
	is_present = SItemFeatureIsPresent(unit, "left_context", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_unit",
				  "Call to \"SItemFeatureIsPresent\" failed"))
		return;

	if (is_present)
	{
		unit_left_context = SItemGetString(unit, "left_context", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Call to \"SItemGetString\" failed"))
			return;
	}
	else
		unit_left_context = NULL;

	/* get unit right context, if present */
	is_present = SItemFeatureIsPresent(unit, "right_context", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_unit",
				  "Call to \"SItemFeatureIsPresent\" failed"))
		return;

	if (is_present)
	{
		unit_right_context = SItemGetString(unit, "right_context", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Call to \"SItemGetString\" failed"))
			return;
	}
	else
		unit_right_context = NULL;

	/* get unit container */
	unitContainer = (SMap*)SMapGetObjectDef(db->catalogue, unit_name, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_unit",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (unitContainer == NULL)
	{
		/* create a new one */
		unitContainer = (SMap*)S_NEW("SMapHashTable", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Failed to create new 'SMapHashTable' object"))
			return;

		SMapHashTableInit(&unitContainer, 10, error); /* it will resize if needed */
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Failed to initialize new 'SMapHashTable' object"))
			return;

		/* add it to the catalogue */
		SMapSetObject(db->catalogue, unit_name, S_OBJECT(unitContainer), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Failed to initialize new 'SMapHashTable' object"))
		{
			S_DELETE(unitContainer, "set_unit", error);
			return;
		}
	}

	/* add the contexts of this unit to the catalogue */
	if ((unit_left_context != NULL) && (unit_right_context != NULL))
	{
		/* unit has left and right context */
		SList *leftRightContext;
		char *context = NULL;
		size_t size;


		size = s_strsize(unit_left_context, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Call to \"s_strsize\" failed"))
			return;

		size += s_strsize(unit_right_context, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Call to \"s_strsize\" failed"))
			return;

		context = S_CALLOC(char, size + 2);
		if (context == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "set_unit",
					  "Failed to allocate memory for 'char' object");
			return;
		}

		s_strcat(context, unit_left_context, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Call to \"s_strcat\" failed"))
		{
			S_FREE(context);
			return;
		}

		s_strcat(context, ":", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Call to \"s_strcat\" failed"))
		{
			S_FREE(context);
			return;
		}

		s_strcat(context, unit_right_context, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Call to \"s_strcat\" failed"))
		{
			S_FREE(context);
			return;
		}

		leftRightContext = (SList*)SMapGetObjectDef(unitContainer, context, NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Call to \"SMapGetObjectDef\" failed"))
		{
			S_FREE(context);
			return;
		}

		if (leftRightContext == NULL)
		{
			/* this context has not been seen, create and add it */
			leftRightContext = (SList*)S_NEW("SListList", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "set_unit",
						  "Failed to create new 'SListList' object"))
			{
				S_FREE(context);
				return;
			}

			SListListInit(&leftRightContext, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "set_unit",
						  "Failed to initialize new 'SListList' object"))
			{
				S_FREE(context);
				return;
			}

			SMapSetObject(unitContainer, context, S_OBJECT(leftRightContext), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "set_unit",
						  "Call to \"SMapSetObject\" failed"))
			{
				S_FREE(context);
				S_DELETE(leftRightContext, "set_unit", error);
				return;
			}
		}

		S_FREE(context);

		/* finally add this unit to this context */
		SListAppend(leftRightContext, S_OBJECT(unit), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Call to \"SListAppend\" failed"))
			return;
	}

	/* the left context gets added even if left-right was added. */
	if (unit_left_context != NULL)
	{
		/* unit has only left context */
		SList *leftContext;


		leftContext = (SList*)SMapGetObjectDef(unitContainer, unit_left_context, NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Call to \"SMapGetObjectDef\" failed"))
			return;

		if (leftContext == NULL)
		{
			/* this context has not been seen, create and add it */
			leftContext = (SList*)S_NEW("SListList", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "set_unit",
						  "Failed to create new 'SListList' object"))
				return;

			SListListInit(&leftContext, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "set_unit",
						  "Failed to initialize new 'SListList' object"))
				return;

			SMapSetObject(unitContainer, unit_left_context, S_OBJECT(leftContext), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "set_unit",
						  "Call to \"SMapSetObject\" failed"))
			{
				S_DELETE(leftContext, "set_unit", error);
				return;
			}
		}

		/* finally add this unit to this context */
		SListAppend(leftContext, S_OBJECT(unit), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Call to \"SListAppend\" failed"))
			return;
	}

	/* and add unit to "all-units" container */
	allUnits = (SList*)SMapGetObjectDef(unitContainer, "all-units", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_unit",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (allUnits == NULL)
	{
		/* this context has not been seen, create and add it */
		allUnits = (SList*)S_NEW("SListList", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Failed to create new 'SListList' object"))
			return;

		SListListInit(&allUnits, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Failed to initialize new 'SListList' object"))
			return;

		SMapSetObject(unitContainer, "all-units", S_OBJECT(allUnits), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_unit",
					  "Call to \"SMapSetObject\" failed"))
		{
			S_DELETE(allUnits, "set_unit", error);
			return;
		}
	}

	/* finally add this unit to this context */
	SListAppend(allUnits, S_OBJECT(unit), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_unit",
				  "Call to \"SListAppend\" failed"))
		return;
}

