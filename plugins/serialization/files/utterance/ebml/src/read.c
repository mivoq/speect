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
/* DATE    : April 2009                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Read an utterance from an Ebml format file.                                      */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "utt_ebml.h"


/************************************************************************************/
/*                                                                                  */
/* Data Types                                                                       */
/*                                                                                  */
/************************************************************************************/

typedef struct
{
	SItmContent *content;
	uint         used;
} s_items_content_container;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void read_feature(SEbmlRead *ebmlReader,
						 char **feat_name, uint32 feat_name_id,
						 SObject **featObject, uint32 feat_object_id,
						 s_erc *error);

static void read_utt_features(SUtterance *utt, SEbmlRead *ebmlReader, s_erc *error);

static void items_content_table_free_fp(void *key, void *data, s_erc *error);

static void delete_unused_items_content(s_hash_table *items_content_table, s_erc *error);

static s_hash_table *read_item_contents(SEbmlRead *ebmlReader, s_erc *error);

static void read_utt_relations(SEbmlRead *ebmlReader, SUtterance *utt,
							   s_hash_table *items_content_table, s_erc *error);

static void relation_items_table_free_fp(void *key, void *data, s_erc *error);

static void read_relation_items(SEbmlRead *ebmlReader, SRelation *rel,
								s_hash_table *items_content_table, s_erc *error);

static SItem *get_item(s_hash_table *relation_items, uint32 item_node_number, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SUtterance *s_read_utt_ebml(SDatasource *ds, s_erc *error)
{
	SUtterance *utt = NULL;
	SEbmlRead *ebmlReader = NULL;
	s_hash_table *items_content_table = NULL;
	uint32 id;
	int scomp;
	s_bool container_exhausted;


	S_CLR_ERR(error);

	/* create and initialize ebml reader */
	ebmlReader = (SEbmlRead*)S_NEW("SEbmlRead", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_utt_ebml",
				  "Failed to create new SEbmlRead object"))
		goto quit_error;

	S_EBMLREAD_CALL(ebmlReader, read_init)(&ebmlReader, ds, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_utt_ebml",
				  "Failed to initialize SEbmlRead object"))
		goto quit_error;

	/* check that we have the correct doc type */
	scomp = s_strcmp(ebmlReader->header->doctype, "spct_utt", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_utt_ebml",
				  "Call to \"s_strcmp\" failed"))
		goto quit_error;

	if (scomp != 0)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_read_utt_ebml",
				  "Ebml file format not of type 'spct_utt', read format is '%s'",
				  ebmlReader->header->doctype);
		goto quit_error;
	}

	/* create and initialize new utterance */
	utt = (SUtterance*)S_NEW("SUtterance", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_utt_ebml",
				  "Failed to create new utterance"))
		goto quit_error;

	SUtteranceInit(&utt, NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_utt_ebml",
				  "Failed to initialize new utterance"))
		goto quit_error;

	/* read top level container */
	id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_utt_ebml",
				  "ebmlRead method \"container\" failed"))
		goto quit_error;

	/* sanity check id */
	S_EBML_ID_SANITY(id, S_UTT_EBML,
				"s_read_utt_ebml",
				"ID mismatch", error);

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_utt_ebml",
					  "ebmlRead method \"container_at_end\" failed"))
			goto quit_error;

		if (container_exhausted)
			break; /* we are finished reading the utterance file */

		/* peek id for utterance elements */
		id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_utt_ebml",
					  "ebmlRead method \"peek_id\" failed"))
			goto quit_error;

		switch(id)
		{
		case S_UTT_EBML_FEATURES:
		{
			read_utt_features(utt, ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_read_utt_ebml",
						  "Call to \"read_utt_features\" failed"))
				goto quit_error;
			break;
		}
		case S_UTT_EBML_ITEMS_CONTENTS:
		{
			items_content_table = read_item_contents(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_read_utt_ebml",
						  "Call to \"read_item_contents\" failed"))
				goto quit_error;
			break;
		}
		case S_UTT_EBML_RELATIONS:
		{
			read_utt_relations(ebmlReader, utt, items_content_table, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_read_utt_ebml",
						  "Call to \"read_utt_relations\" failed"))
				goto quit_error;

			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "s_read_utt_ebml",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_read_utt_ebml",
						  "ebmlRead method \"element_skip\" failed"))
				goto quit_error;
		}
	}

	/* if we get here then everything was OK */
	goto quit;


	/* errors start clean up code here */
quit_error:
	if (utt != NULL)
		S_DELETE(utt, "s_read_utt_ebml", error); /* sets utt = NULL */

	if (items_content_table != NULL)
	{
		s_erc local_err = S_SUCCESS;

		s_hash_table_delete(items_content_table, &local_err);
		items_content_table = NULL;
	}

	/* normal exit start clean up code here */
quit:
	if (items_content_table != NULL)
		delete_unused_items_content(items_content_table, error);

	if (ebmlReader != NULL)
		S_DELETE(ebmlReader, "s_read_utt_ebml", error);

	return utt;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

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


static void read_utt_features(SUtterance *utt, SEbmlRead *ebmlReader, s_erc *error)
{
	uint32 id;
	char *feat_name = NULL;
	SObject *featObject = NULL;
	s_bool container_exhausted;


	S_CLR_ERR(error);

	/* read container */
	id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_utt_features",
				  "ebmlRead method \"container\" failed"))
		goto quit_error;

	/* sanity check id */
	S_EBML_ID_SANITY(id, S_UTT_EBML_FEATURES,
				"read_utt_features",
				"ID mismatch", error);

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_utt_features",
					  "ebmlRead method \"container_at_end\" failed"))
			goto quit_error;

		if (container_exhausted)
			break; /* we are finished reading the utterance features */

		/* peek id for utterance feature elements */
		id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_utt_features",
					  "ebmlRead method \"peek_id\" failed"))
			goto quit_error;

		switch(id)
		{
		case S_UTT_EBML_FEAT_NAME:
		{
			/* feature elements are ordered (feat name, feat object),
			 * so we do it in one go
			 */
			read_feature(ebmlReader,
						 &feat_name, S_UTT_EBML_FEAT_NAME,
						 &featObject, S_UTT_EBML_FEAT_OBJECT,
						 error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_utt_features",
						  "Call to \"read_feature\" failed"))
				goto quit_error;

			/* set utterance feature */
			SUtteranceSetFeature(utt, feat_name, featObject, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_utt_features",
						  "Call to \"SUtteranceSetFeature\" failed"))
				goto quit_error;

			/* feat name is copied */
			S_FREE(feat_name); /* sets to NULL */
			featObject = NULL;
			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_utt_features",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_utt_features",
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
		S_DELETE(featObject, "read_utt_features", error);
}


static void items_content_table_free_fp(void *key, void *data, s_erc *error)
{
	uint32 *item_content_id;
	s_items_content_container *ic;


	S_CLR_ERR(error);

	item_content_id = key;
	ic = data;

	S_FREE(item_content_id);

	if (ic != NULL)
	{
		if (ic->content != NULL)
			S_FORCE_DELETE(ic->content, "items_content_table_free_fp", error);
		S_FREE(ic);
	}
}


static void delete_unused_items_content(s_hash_table *items_content_table, s_erc *error)
{
	s_hash_element *hte;
	s_erc local_err = S_SUCCESS;

	hte = s_hash_table_first(items_content_table, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "delete_unused_item_contents",
				  "Call to \"s_hash_table_first\" failed"))
		return;

	while (hte != NULL)
	{
		uint32 *num = NULL;
		s_items_content_container *ic = NULL;
		s_hash_element *next = NULL;


		next = s_hash_element_next(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "delete_unused_item_contents",
					  "Call to \"s_hash_element_next\" failed"))
		{
			next = NULL;
			local_err = *error;
		}

		num = s_hash_element_key(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "delete_unused_item_contents",
					  "Call to \"s_hash_element_key\" failed"))
			local_err = *error;

		if (num != NULL)
			S_FREE(num);

		ic = s_hash_element_get_data(hte, error);
		if (!S_CHK_ERR(error, S_CONTERR,
					   "delete_unused_item_contents",
					   "Call to \"s_hash_element_get_data\" failed"))
			local_err = *error;

		if (ic != NULL)
		{
			if ((ic->content != NULL) && (ic->used == 0))
			{
				S_FORCE_DELETE(ic->content, "delete_unused_item_contents",
							   &local_err);
			}

			S_FREE(ic);
		}

		s_hash_element_unlink(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "delete_unused_item_contents",
					  "Call to \"s_hash_element_unlink\" failed"))
			local_err = *error;

		hte = next;
	}

	s_hash_table_delete(items_content_table, error);
	S_CHK_ERR(error, S_CONTERR,
			  "delete_unused_item_contents",
			  "Call to \"s_hash_table_delete\" failed");

	if ((local_err != S_SUCCESS) && (*error == S_SUCCESS))
		*error = local_err;
}


static s_hash_table *read_item_contents(SEbmlRead *ebmlReader, s_erc *error)
{
	s_hash_table *items_content_table = NULL;
	s_items_content_container *ic = NULL;
	uint32 *item_content_id = NULL;
	uint32 id;
	s_bool container_exhausted;


	S_CLR_ERR(error);

	/* read S_UTT_EBML_ITEMS_CONTENTS container */
	id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_item_contents",
				  "ebmlRead method \"container\" failed"))
		goto quit_error;

	/* 2^9 = 512 item contents for a start */
	items_content_table = s_hash_table_new(&items_content_table_free_fp, 9, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_item_contents",
				  "Call to \"s_hash_table_new\" failed"))
		goto quit_error;

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_item_contents",
					  "ebmlRead method \"container_at_end\" failed"))
			goto quit_error;

		if (container_exhausted)
			break;

		/* peek id  */
		id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_item_contents",
					  "ebmlRead method \"peek_id\" failed"))
			goto quit_error;

		switch(id)
		{
		case S_UTT_EBML_ITEMS_CONTENT_ID:
		{
			item_content_id = S_MALLOC(uint32, 1);
			if (item_content_id == NULL)
			{
				S_FTL_ERR(error, S_MEMERROR,
						  "read_item_contents",
						  "Failed to allocate memory for 'uint32' object");
				goto quit_error;
			}

			*item_content_id = S_EBMLREAD_CALL(ebmlReader, read_uint)(ebmlReader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_item_contents",
						  "ebmlRead method \"read_uint\" failed"))
				goto quit_error;

			/* make new item contents container */
			ic = S_CALLOC(s_items_content_container, 1);
			if (ic == NULL)
			{
				S_FTL_ERR(error, S_MEMERROR,
						  "read_item_contents",
						  "Failed to allocate memory for 's_items_content_container' object");
				goto quit_error;
			}

			/* create a new item contents */
			ic->content =  (SItmContent*)S_NEW("SItmContent", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_item_contents",
						  "Failed to create new item content"))
				goto quit_error;

			break;
		}
		case S_UTT_EBML_ITEMS_CONTENT_FEATURES:
		{
			/* read S_UTT_EBML_ITEMS_CONTENT_FEATURES container */
			id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_item_contents",
						  "ebmlRead method \"container\" failed"))
				goto quit_error;

			while (1)
			{
				container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_item_contents",
							  "ebmlRead method \"container_at_end\" failed"))
					goto quit_error;

				if (container_exhausted)
				{
					/* add item contents container to hash table */
					s_hash_table_add(items_content_table, (void*)item_content_id,
									 sizeof(uint32), ic, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_item_contents",
								  "Call to \"s_hash_table_add\" failed"))
						goto quit_error;

					item_content_id = NULL;
					ic = NULL;
					break; /* we are finished reading the item content
							* features */
				}

				/* peek id for item content feature elements */
				id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_item_contents",
							  "ebmlRead method \"peek_id\" failed"))
					goto quit_error;

				switch(id)
				{
				case S_UTT_EBML_ITEMS_CONTENT_FEAT_NAME:
				{
					char *feat_name;
					SObject *featObject;


					/* feature elements are ordered (feat name, feat object),
					 * so we do it in one go
					 */
					read_feature(ebmlReader,
								 &feat_name, S_UTT_EBML_ITEMS_CONTENT_FEAT_NAME,
								 &featObject, S_UTT_EBML_ITEMS_CONTENT_FEAT_OBJECT,
								 error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_item_contents",
								  "Call to \"read_feature\" failed"))
						goto quit_error;

					/* set item content feature */
					SMapSetObject(ic->content->features, feat_name, featObject, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_item_contents",
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
							  "read_item_contents",
							  "Skipping element with unknown id '0x%x'",
							  id);

					S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_item_contents",
								  "ebmlRead method \"element_skip\" failed"))
						goto quit_error;
				}
			}
			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_item_contents",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_item_contents",
						  "ebmlRead method \"element_skip\" failed"))
				goto quit_error;
		}
	}

	/* here everything went OK */
	return items_content_table;

	/* errors here, clean up code */
quit_error:
	if (ic != NULL)
	{
		if (ic->content != NULL)
			S_FORCE_DELETE(ic->content, "read_item_contents", error);
		S_FREE(ic);
	}

	if (items_content_table != NULL)
	{
		s_erc local_err = S_SUCCESS;

		s_hash_table_delete(items_content_table, &local_err);
		S_CHK_ERR(&local_err, S_CONTERR,
				  "read_item_contents",
				  "Call to \"s_hash_table_delete\" failed");
	}

	if (item_content_id != NULL)
		S_FREE(item_content_id);

	return NULL;
}


static void read_utt_relations(SEbmlRead *ebmlReader, SUtterance *utt,
							   s_hash_table *items_content_table, s_erc *error)
{
	uint32 id;
	SRelation *rel;
	s_bool container_exhausted;


	S_CLR_ERR(error);

	/* read S_UTT_EBML_RELATIONS container */
	id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_utt_relations",
				  "ebmlRead method \"container\" failed"))
		return;

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_utt_relations",
					  "ebmlRead method \"container_at_end\" failed"))
			return;

		if (container_exhausted)
			break;

		/* peek id  */
		id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_utt_relations",
					  "ebmlRead method \"peek_id\" failed"))
			return;

		switch(id)
		{
		case S_UTT_EBML_RELATION_NAME:
		{
			char *name;

			name = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_utt_relations",
						  "ebmlRead method \"read_utf8\" failed"))
				return;

			/* create new relation */
			rel = SUtteranceNewRelation(utt, name, error);
			/* name is copied */
			S_FREE(name);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_utt_relations",
						  "Call to \"SUtteranceNewRelation\" failed"))
				return;

			break;
		}
		case S_UTT_EBML_RELATION_ITEMS:
		{
			read_relation_items(ebmlReader, rel, items_content_table, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_utt_relations",
						  "Call to \"read_relation_items\" failed"))
				return;

			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_utt_relations",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_utt_relations",
						  "ebmlRead method \"element_skip\" failed"))
				return;
		}
	}
}


static void relation_items_table_free_fp(void *key, void *data, s_erc *error)
{
	uint32 *item_node_number;


	S_CLR_ERR(error);

	item_node_number = key;

	/*
	 * data are items. As soon as get_item is called (the item is
	 * either retrieved from the table or created and added to the table)
	 * then the item is also connected to the relation. So we do not need
	 * to delete the item, as when we delete the relation the item will go
	 * as well.
	 */
	data = NULL;

	if (item_node_number != NULL)
		S_FREE(item_node_number);
}


static void read_relation_items(SEbmlRead *ebmlReader, SRelation *rel,
								s_hash_table *items_content_table, s_erc *error)
{
	uint32 id;
	SItem *item;
	s_hash_table *relation_items = NULL;
	s_bool container_exhausted;


	S_CLR_ERR(error);

	/* read S_UTT_EBML_RELATION_ITEMS container */
	id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_relation_items",
				  "ebmlRead method \"container\" failed"))
		goto quit_error;

	/* 2^8 = 256 items */
	relation_items = s_hash_table_new(&relation_items_table_free_fp, 8, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_relation_items",
				  "Call to \"relation_items\" failed"))
		goto quit_error;

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_relation_items",
					  "ebmlRead method \"container_at_end\" failed"))
			goto quit_error;

		if (container_exhausted)
		{
			SItem *current;
			SItem *next;


			/* connect relation head and tail items
			 * find the first item.
			 */
			current = get_item(relation_items, 1, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "Call to \"get_item\" failed"))
				goto quit_error;

			rel->head = current;

			/* find the last item*/
			while (current != NULL)
			{
				next = SItemNext(current, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_relation_items",
							  "Call to \"SItemNext\" failed"))
					goto quit_error;

				if (next == NULL)
				{
					rel->tail = current;
					break;
				}

				current = next;
			}

			s_hash_table_delete(relation_items, error);
			S_CHK_ERR(error, S_CONTERR,
					  "read_relation_items",
					  "Call to \"s_hash_table_delete\" failed");

			break;
		}

		/* peek id  */
		id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_relation_items",
					  "ebmlRead method \"peek_id\" failed"))
			goto quit_error;

		switch(id)
		{
		case S_UTT_EBML_RELATION_ITEM_NODE:
		{
			uint32 item_node_number;


			item_node_number = S_EBMLREAD_CALL(ebmlReader, read_uint)(ebmlReader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "ebmlRead method \"read_uint\" failed"))
				goto quit_error;

			/* find the item. */
			item = get_item(relation_items, item_node_number, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "Call to \"get_item\" failed"))
				goto quit_error;

			item->relation = rel;

			break;
		}
		case S_UTT_EBML_RELATION_ITEM_CONTENT_ID:
		{
			uint32 item_content_id;
			s_hash_element *he;
			s_items_content_container *ic;


			item_content_id = S_EBMLREAD_CALL(ebmlReader, read_uint)(ebmlReader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "ebmlRead method \"read_uint\" failed"))
				goto quit_error;

			/* find the item content */
			he = s_hash_table_find(items_content_table, (void*)&item_content_id,
								   sizeof(uint32), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "Call to \"s_hash_table_find\" failed"))
				goto quit_error;

			if (he == NULL)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "read_relation_items",
						  "Failed to find item content for read content id");
				goto quit_error;
			}

			ic = s_hash_element_get_data(he, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "Call to \"s_hash_element_get_data\" failed"))
				goto quit_error;

			item->content = ic->content;
			ic->used = 1;

			/* add the item in item's content relations. This is for
			 * the SItemAs and SItemIn item functions.
			 */
			SItmContentAdd(item->content, rel->name, item, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "Call to \"SItmContentAdd\" failed"))
				goto quit_error;

			break;
		}
		case S_UTT_EBML_RELATION_ITEM_UP_NODE:
		{
			uint32 item_node_number;
			SItem *itemParent;


			item_node_number = S_EBMLREAD_CALL(ebmlReader, read_uint)(ebmlReader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "ebmlRead method \"read_uint\" failed"))
				goto quit_error;

			/* find the item. */
			itemParent = get_item(relation_items, item_node_number, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "Call to \"get_item\" failed"))
				goto quit_error;

			item->up = itemParent;
			break;
		}
		case S_UTT_EBML_RELATION_ITEM_DOWN_NODE:
		{
			uint32 item_node_number;
			SItem *itemDaughter;


			item_node_number = S_EBMLREAD_CALL(ebmlReader, read_uint)(ebmlReader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "ebmlRead method \"read_uint\" failed"))
				goto quit_error;

			/* find the item. */
			itemDaughter = get_item(relation_items, item_node_number, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "Call to \"get_item\" failed"))
				goto quit_error;

			item->down = itemDaughter;
			break;
		}
		case S_UTT_EBML_RELATION_ITEM_NEXT_NODE:
		{
			uint32 item_node_number;
			SItem *itemNext;


			item_node_number = S_EBMLREAD_CALL(ebmlReader, read_uint)(ebmlReader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "ebmlRead method \"read_uint\" failed"))
				goto quit_error;

			/* find the item. */
			itemNext = get_item(relation_items, item_node_number, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "Call to \"get_item\" failed"))
				goto quit_error;

			item->next = itemNext;
			break;
		}
		case S_UTT_EBML_RELATION_ITEM_PREV_NODE:
		{
			uint32 item_node_number;
			SItem *itemPrev;


			item_node_number = S_EBMLREAD_CALL(ebmlReader, read_uint)(ebmlReader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "ebmlRead method \"read_uint\" failed"))
				goto quit_error;

			/* find the item. */
			itemPrev = get_item(relation_items, item_node_number, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "Call to \"get_item\" failed"))
				goto quit_error;;

			item->prev = itemPrev;
			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_relation_items",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_relation_items",
						  "ebmlRead method \"element_skip\" failed"))
				goto quit_error;
		}
	}

	/* here everything went OK */
	return;

	/* errors here, clean up code */
quit_error:

	/* disconnect items from relation */
	rel->head = NULL;
	rel->tail = NULL;

	/* delete them, can't use normal s_hash_table_delete
	 * because the delete function pointer
	 * (relation_items_table_free_fp) does not delete the items.
	 */
	if (relation_items != NULL)
	{
		s_hash_element *hte;
		s_hash_element *next;
		s_erc local_err = S_SUCCESS;

		hte = s_hash_table_first(relation_items, &local_err);
		if (S_CHK_ERR(&local_err, S_CONTERR,
					  "read_relation_items",
					  "Call to \"s_hash_table_first\" failed"))
			return;

		while (hte != NULL)
		{
			SItem *item;
			uint32 *item_node_number;


			next = s_hash_element_next(hte, &local_err);
			if (S_CHK_ERR(&local_err, S_CONTERR,
						  "read_relation_items",
						  "Call to \"s_hash_element_next\" failed"))
				next = NULL;

			item_node_number = s_hash_element_key(hte, &local_err);
			if (!S_CHK_ERR(&local_err, S_CONTERR,
						   "read_relation_items",
						   "Call to \"s_hash_element_key\" failed"))
				S_FREE(item_node_number);

			item = s_hash_element_get_data(hte, &local_err);
			if (!S_CHK_ERR(&local_err, S_CONTERR,
						   "read_relation_items",
						   "Call to \"s_hash_element_get_data\" failed"))
				S_FORCE_DELETE(item, "read_relation_items", &local_err);

			s_hash_element_unlink(hte, &local_err);
			S_CHK_ERR(&local_err, S_CONTERR,
					  "read_relation_items",
					  "Call to \"s_hash_element_unlink\" failed");

			hte = next;
		}

		s_hash_table_delete(relation_items, &local_err);
		S_CHK_ERR(&local_err, S_CONTERR,
				  "read_relation_items",
				  "Call to \"s_hash_table_delete\" failed");
	}
}


/* search for item, if not found create a new one and add it. */
static SItem *get_item(s_hash_table *relation_items, uint32 item_node_number, s_erc *error)
{
	uint32 *item_node_number_copy;
	SItem *item;
	s_hash_element *he;


	/* search for item, if not found create a new one and add it. */
	he = s_hash_table_find(relation_items, (void*)&item_node_number,
						   sizeof(uint32), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_item",
				  "Call to \"s_hash_table_find\" failed"))
		return NULL;

	if (he == NULL)
	{
		/* create new item */
		item = (SItem*)S_NEW("SItem", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_item",
					  "Failed to create new Item"))
			return NULL;

		item_node_number_copy = S_MALLOC(uint32, 1);
		if (item_node_number_copy == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "get_item",
					  "Failed to allocated memory for 'uint32' object");
			S_FORCE_DELETE(item, "get_item", error);
			return NULL;
		}

		*item_node_number_copy = item_node_number;

		/* add to hash table */
		s_hash_table_add(relation_items, (void*)item_node_number_copy,
						 sizeof(uint32), (void*)item, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_item",
					  "Call to \"s_hash_table_add\" failed"))
		{
			S_FREE(item_node_number_copy);
			S_FORCE_DELETE(item, "get_item", error);
			return NULL;
		}
	}
	else
	{
		item = s_hash_element_get_data(he, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_item",
					  "Call to \"s_hash_element_get_data\" failed"))
			return NULL;
	}

	return item;
}

