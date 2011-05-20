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
/* DATE    : April 2009                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Write an utterance to an Ebml format file.                                       */
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
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void write_feature(SEbmlWrite *ebmlWriter,
						  const char *feat_name, uint32 feat_name_id,
						  const SObject *featObject, uint32 feat_object_id,
						  s_erc *error);

static void write_utt_features(SEbmlWrite *ebmlWriter, const SUtterance *utt,
							   s_erc *error);

static void write_this_content(SEbmlWrite *ebmlWriter, const SItem *item,
							   s_hash_table *items_content_table,
							   uint32 *item_content_id, s_erc *error);

static void write_this_item_content(SEbmlWrite *ebmlWriter, const SItem *item,
									s_hash_table *items_content_table,
									uint32 *item_content_id, s_erc *error);

static void items_content_table_free_fp(void *key, void *data, s_erc *error);

static s_hash_table *write_item_contents(SEbmlWrite *ebmlWriter, const SUtterance *utt,
										 s_erc *error);

static void write_items_recursive(SEbmlWrite *ebmlWriter, const SItem *item,
								  s_hash_table *item_contents_table,
								  s_hash_table *item_nodes,
								  uint32 *node_count,
								  s_erc *error);

static void items_nodes_free_fp(void *key, void *data, s_erc *error);

static void write_relation_items(SEbmlWrite *ebmlWriter, const SItem *relItemHead,
								 s_hash_table *item_contents_table, s_erc *error);

static void write_utt_relations(SEbmlWrite *ebmlWriter, const SUtterance *utt,
								s_hash_table *item_contents_table, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void s_write_utt_ebml(const SUtterance *utt, SDatasource *ds, s_erc *error)
{
	SEbmlWrite *ebmlWriter = NULL;
	s_ebml_header *header = NULL;
	s_hash_table *item_contents_table = NULL;


	S_CLR_ERR(error);

	/* create ebml header */
	header = S_CALLOC(s_ebml_header, 1);
	if (header == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_write_utt_ebml",
				  "Failed to allocate memory for 's_ebml_header' object");
		goto quit;
	}

	header->ebml_version = 1;
	header->ebml_read_version = 1;
	header->max_id_width = 4;
	header->max_size_width = 4;
	header->doctype_version = 1;
	header->doctype_read_version = 1;
	header->doctype = s_strdup("spct_utt", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "main",
				  "Call to \"s_strdup\" failed"))
		goto quit;

	/* create and initialize ebml writer */
	ebmlWriter = (SEbmlWrite*)S_NEW("SEbmlWrite", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_ebml",
				  "Failed to create new SEbmlWrite object"))
		goto quit;

	S_EBMLWRITE_CALL(ebmlWriter, write_init)(&ebmlWriter, ds, header, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_ebml",
				  "Failed to initialize SEbmlRead object"))
		goto quit;
	else
	{
		/*
		 * ebml writer takes hold of the header and the data
		 * source, we dont want a reference to it anymore (for
		 * quit to work).
		 */
		header = NULL;
		ds = NULL;
	}

	/* start top level container, S_UTT_EBML */
	S_EBMLWRITE_CALL(ebmlWriter, start_container)(ebmlWriter, S_UTT_EBML, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_ebml",
				  "ebmlWriter method \"start_container\" failed"))
		goto quit;

	/* write utterance features */
	write_utt_features(ebmlWriter, utt, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_ebml",
				  "Call to \"write_utt_features\" failed"))
		goto quit;

	/*
	 * Write the item contents of each item in the utterance.
	 * First set up a hash table to store the item id's.
	 */
	item_contents_table = write_item_contents(ebmlWriter, utt, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_ebml",
				  "Call to \"write_item_contents\" failed"))
		goto quit;

	/* write relations */
	write_utt_relations(ebmlWriter, utt, item_contents_table, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_ebml",
				  "Call to \"write_utt_relations\" failed"))
		goto quit;

	/* stop top level container, S_UTT_EBML */
	S_EBMLWRITE_CALL(ebmlWriter, stop_container)(ebmlWriter, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_ebml",
				  "ebmlWriter method \"stop_container\" failed"))
		goto quit;

/* cleanup */
quit:
	if (header != NULL)
		S_FREE(header);

	if (ds != NULL)
		S_DELETE(ds, "s_write_utt_ebml", error);

	/* delete item contents table */
	if (item_contents_table != NULL)
		s_hash_table_delete(item_contents_table, error);

	if (ebmlWriter != NULL)
		S_DELETE(ebmlWriter, "s_write_utt_ebml", error);
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void write_feature(SEbmlWrite *ebmlWriter,
						  const char *feat_name, uint32 feat_name_id,
						  const SObject *featObject, uint32 feat_object_id,
						  s_erc *error)
{
	s_bool writeable;


	S_CLR_ERR(error);

	/* test if we can write this object type */
	writeable = SObjectIsWritable(featObject, "spct_ebml", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_feature",
				  "Call to \"SObjectIsWritable\" failed"))
		return;

	if (!writeable)
	{
		const char *type;

		/* ignore this object, issue a warning */
		type = SObjectType(featObject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_feature",
					  "Call to \"SObjectType\" failed"))
			type = "Object type unknown";

		S_WARNING(S_WARNERR,
				  "write_feature",
				  "Given feature pair (%s, %s) cannot be written, object is not writable",
				  feat_name, type);
		return;
	}

	/* write feature name */
	S_EBMLWRITE_CALL(ebmlWriter, write_utf8)(ebmlWriter, feat_name_id, feat_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_feature",
				  "ebmlWriter method \"write_utf8\" failed"))
		return;

	/* write feature object */
	S_EBMLWRITE_CALL(ebmlWriter, write_object)(ebmlWriter, feat_object_id, featObject, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_feature",
			  "ebmlWriter method \"write_object\" failed");
}


static void write_utt_features(SEbmlWrite *ebmlWriter, const SUtterance *utt,
							   s_erc *error)
{
	SIterator *itr = NULL;
	const char *feat_name;
	const SObject *featObject;
	size_t size;


	S_CLR_ERR(error);

	size = SMapSize(utt->features, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_utt_features",
				  "Call to \"SMapSize\" failed"))
		goto quit_error;

	if (size == 0) /* no utt features, nothing to write */
		return;

	/* start container, S_UTT_EBML_FEATURES */
	S_EBMLWRITE_CALL(ebmlWriter, start_container)(ebmlWriter, S_UTT_EBML_FEATURES, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_utt_features",
				  "ebmlWriter method \"start_container\" failed"))
		goto quit_error;

	/* get an iterator to the utterance features */
	itr = S_ITERATOR_GET(utt->features, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_utt_features",
				  "Call to \"S_ITERATOR_GET\" failed"))
		goto quit_error;

	for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
	{
		feat_name = SIteratorKey(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_utt_features",
					  "Call to \"SIteratorKey\" failed"))
			goto quit_error;

		featObject = SIteratorObject(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_utt_features",
					  "Call to \"SIteratorObject\" failed"))
			goto quit_error;

		write_feature(ebmlWriter,
					  feat_name, S_UTT_EBML_FEAT_NAME,
					  featObject, S_UTT_EBML_FEAT_OBJECT,
					  error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_utt_features",
					  "Call to \"write_feature\" failed"))
			goto quit_error;
	}

	/* stop container, S_UTT_EBML_FEATURES */
	S_EBMLWRITE_CALL(ebmlWriter, stop_container)(ebmlWriter, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_utt_features",
				  "ebmlWriter method \"stop_container\" failed"))
		goto quit_error;

	/* here everything went OK */
	return;

	/* errors here, clean up code */
quit_error:
	if (itr != NULL)
		S_DELETE(itr, "write_utt_features", error);
}


static void write_this_content(SEbmlWrite *ebmlWriter, const SItem *item,
							   s_hash_table *items_content_table,
							   uint32 *item_content_id, s_erc *error)
{
	sint32 item_id;
	sint32 *item_id_copy = NULL;
	uint32 *item_content_id_copy = NULL;
	const s_hash_element *he;
	SItmContent *itmContent;
	SIterator *itr = NULL;
	const char *feat_name;
	const SObject *featObject;


	S_CLR_ERR(error);

	item_id = SItemGetInt(item, "_id", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_this_content",
				  "Call to \"SItemGetInt\" failed"))
		goto quit_error;

	he = s_hash_table_find(items_content_table, (void*)&item_id,
						   sizeof(sint32), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_this_content",
				  "Call to \"s_hash_table_find\" failed"))
		goto quit_error;

	if (he == NULL)
	{
		/*
		 * write this contents, it has not been written yet.
		 * write item content id, S_UTT_EBML_ITEMS_CONTENT_ID
		 */
		S_EBMLWRITE_CALL(ebmlWriter, write_sint)(ebmlWriter, S_UTT_EBML_ITEMS_CONTENT_ID,
												 *item_content_id, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_this_content",
					  "ebmlWriter method \"write_sint\" failed"))
			goto quit_error;

		itmContent = item->content;

		/* start container, S_UTT_EBML_ITEMS_CONTENT_FEATURES */
		S_EBMLWRITE_CALL(ebmlWriter, start_container)(ebmlWriter, S_UTT_EBML_ITEMS_CONTENT_FEATURES,
													  error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_this_content",
					  "ebmlWriter method \"start_container\" failed"))
			goto quit_error;

		/* iterate through item content features and write them */
		itr = S_ITERATOR_GET(itmContent->features, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_this_content",
					  "Call to \"S_ITERATOR_GET\" failed"))
			goto quit_error;


		for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
		{
			feat_name = SIteratorKey(itr, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "write_this_content",
						  "Call to \"SIteratorKey\" failed"))
				goto quit_error;

			featObject = SIteratorObject(itr, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "write_this_content",
						  "Call to \"SIteratorObject\" failed"))
				goto quit_error;

			write_feature(ebmlWriter,
						  feat_name, S_UTT_EBML_ITEMS_CONTENT_FEAT_NAME,
						  featObject, S_UTT_EBML_ITEMS_CONTENT_FEAT_OBJECT,
						  error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "write_this_content",
						  "Call to \"write_feature\" failed"))
				goto quit_error;
		}

		/* stop container, S_UTT_EBML_ITEMS_CONTENT_FEATURES */
		S_EBMLWRITE_CALL(ebmlWriter, stop_container)(ebmlWriter, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_this_content",
					  "ebmlWriter method \"stop_container\" failed"))
			goto quit_error;

		/* add this item contents info to hash table */
		item_id_copy = S_MALLOC(sint32, 1);
		if (item_id_copy == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "write_this_content",
					  "Failed to allocate memory for 'sint32' object");
			goto quit_error;
		}
		*item_id_copy = item_id;

		item_content_id_copy = S_MALLOC(uint32, 1);
		if (item_content_id_copy == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "write_this_content",
					  "Failed to allocate memory for 'uint32' object");
			goto quit_error;
		}
		*item_content_id_copy = *item_content_id;

		/* add it to the hash table */
		s_hash_table_add(items_content_table,
						 (void*)item_id_copy, sizeof(sint32),
						 (void*)item_content_id_copy,
						 error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_this_content",
					  "Call to \"s_hash_table_add\" failed"))
			goto quit_error;

		(*item_content_id)++;
	}

	/* here everything went OK */
	return;

	/* errors here, clean up code */
quit_error:
	if (itr != NULL)
		S_DELETE(itr, "write_utt_features", error);

	if (item_content_id_copy != NULL)
		S_FREE(item_content_id_copy);

	if (item_id_copy != NULL)
		S_FREE(item_id_copy);
}


static void write_this_item_content(SEbmlWrite *ebmlWriter, const SItem *item,
									s_hash_table *items_content_table,
									uint32 *item_content_id, s_erc *error)
{
	SItem *nextItem;
	SItem *daughterItem;


	S_CLR_ERR(error);

	if (item == NULL)
		return;

	write_this_content(ebmlWriter, item, items_content_table,
					   item_content_id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_this_content",
				  "Call to \"write_this_item_content\" failed"))
		return;

	/* recursively write next items */
	nextItem = SItemNext(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_this_item_content",
				  "Call to \"SItemNext\" failed"))
		return;

	write_this_item_content(ebmlWriter, nextItem, items_content_table,
							item_content_id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_this_item_content",
				  "Call to \"write_this_item_content\" failed"))
		return;

	/* recursively write daughter items */
	daughterItem = SItemDaughter(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_this_item_content",
				  "Call to \"SItemDaughter\" failed"))
		return;

	write_this_item_content(ebmlWriter, daughterItem, items_content_table,
							item_content_id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_this_item_content",
				  "Call to \"write_this_item_content\" failed"))
		return;
}


static void items_content_table_free_fp(void *key, void *data, s_erc *error)
{
	sint32 *item_id;
	uint32 *item_content_id;


	S_CLR_ERR(error);

	item_id = key;
	item_content_id = data;

	S_FREE(item_id);
	S_FREE(item_content_id);
}


static s_hash_table *write_item_contents(SEbmlWrite *ebmlWriter, const SUtterance *utt,
										 s_erc *error)
{
	s_hash_table *items_content_table = NULL;
	uint32 max_num_contents;
	SIterator *itr = NULL;
	SRelation *rel;
	uint32 item_content_id;


	S_CLR_ERR(error);

	/*
	 * Write the item content of each item in each relation. The
	 * maximum number of item content will be the number of items.
	 */
	max_num_contents = SMapGetInt(utt->features, "_id", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_item_contents",
				  "Call to \"SMapGetInt\" failed"))
		goto quit_error;

	/* create item content table */
	items_content_table = s_hash_table_new(&items_content_table_free_fp,
										   (size_t)ceil(s_log2(max_num_contents)), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_item_contents",
				  "Call to \"s_hash_table_new\" failed"))
		goto quit_error;

	/* start container, S_UTT_EBML_ITEMS_CONTENTS */
	S_EBMLWRITE_CALL(ebmlWriter, start_container)(ebmlWriter, S_UTT_EBML_ITEMS_CONTENTS,
												  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_item_contents",
				  "ebmlWriter method \"start_container\" failed"))
		goto quit_error;

	/* iterate through the relations and add item contents	 */
	itr = S_ITERATOR_GET(utt->relations, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_item_contents",
				  "Call to \"S_ITERATOR_GET\" failed"))
		goto quit_error;


	item_content_id = 1;

	for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
	{
		const SItem *relHeadItem;


		rel = (SRelation*)SIteratorObject(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_item_contents",
					  "Call to \"SIteratorObject\" failed"))
			goto quit_error;

		relHeadItem = SRelationHead(rel, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_item_contents",
					  "Call to \"SRelationHead\" failed"))
			goto quit_error;

		write_this_item_content(ebmlWriter, relHeadItem, items_content_table,
								&item_content_id, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_item_contents",
					  "Call to \"write_this_item_content\" failed"))
			goto quit_error;
	}


	/* stop container, S_UTT_EBML_ITEMS_CONTENTS */
	S_EBMLWRITE_CALL(ebmlWriter, stop_container)(ebmlWriter, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_item_contents",
				  "ebmlWriter method \"stop_container\" failed"))
		goto quit_error;

	/* here everything went OK */
	return items_content_table;


	/* errors here, clean up code */
quit_error:
	if (items_content_table != NULL)
	{
		s_erc local_err = S_SUCCESS;

		s_hash_table_delete(items_content_table, &local_err);
		S_CHK_ERR(&local_err, S_CONTERR,
				  "write_item_contents",
				  "Call to \"s_hash_table_delete\" failed");
	}

	if (itr != NULL)
		S_DELETE(itr, "write_item_contents", error);

	return NULL;
}


static void write_items_recursive(SEbmlWrite *ebmlWriter, const SItem *item,
								  s_hash_table *item_contents_table,
								  s_hash_table *item_nodes,
								  uint32 *node_count,
								  s_erc *error)
{
	uint32 *node_number = NULL;
	sint32 id;
	const uint32 *item_content_id;
	SItem *itemToWrite;
	const s_hash_element *he;


	/*
	 * Don't clear error here, check for error and return as this
	 * function is called recursively by itself.
	 */
	if ((error != NULL) && (*error != S_SUCCESS))
		return;

	if (item == NULL)
		return;

	node_number = S_MALLOC(uint32, 1);
	if (node_number == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "write_items_recursive",
				  "Failed to allocate memory for 'uint32' object");
		return;
	}

	(*node_number) = (*node_count)++;

	/* add this item and node number */
	s_hash_table_add(item_nodes, (void*)item, SObjectSize(S_OBJECT(item), error),
					 (void*)node_number, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "Call to \"s_hash_table_add\" failed"))
	{
		S_FREE(node_number);
		return;
	}

	/* write next item */
	itemToWrite = SItemNext(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "Call to \"SItemNext\" failed"))
		return;

	write_items_recursive(ebmlWriter, itemToWrite, item_contents_table,
						  item_nodes, node_count, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "Call to \"write_items_recursive\" failed"))
		return;

	/* write daughter item */
	itemToWrite = SItemDaughter(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "Call to \"SItemDaughter\" failed"))
		return;

	write_items_recursive(ebmlWriter, itemToWrite, item_contents_table,
						  item_nodes, node_count, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "Call to \"write_items_recursive\" failed"))
		return;

	/* write node number */
	S_EBMLWRITE_CALL(ebmlWriter, write_uint)(ebmlWriter, S_UTT_EBML_RELATION_ITEM_NODE,
											 *node_number, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "ebmlWriter method \"write_uint\" failed"))
		return;

	/* get item id */
	id = SItemGetInt(item, "_id", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "Call to \"SItemGetInt\" failed"))
		return;

	/* get correspond item content id */
	he = s_hash_table_find(item_contents_table, (void*)&id, sizeof(sint32), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "Call to \"s_hash_table_find\" failed"))
		return;

	if (he == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "write_items_recursive",
				  "Failed to find item's content");
		return;
	}

	item_content_id = s_hash_element_get_data(he, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "Call to \"s_hash_element_get_data\" failed"))
		return;

	/* write content id */
	S_EBMLWRITE_CALL(ebmlWriter, write_uint)(ebmlWriter, S_UTT_EBML_RELATION_ITEM_CONTENT_ID,
											 *item_content_id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "ebmlWriter method \"write_uint\" failed"))
		return;

	/* check parent item (up) */
	itemToWrite = SItemParent(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "Call to \"SItemParent\" failed"))
		return;

	if (itemToWrite != NULL)
	{
		/* get node_number */
		he = s_hash_table_find(item_nodes, (void*)itemToWrite,
							   SObjectSize(S_OBJECT(itemToWrite), error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_items_recursive",
					  "Call to \"s_hash_table_find\" failed"))
			return;

		if (he == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "write_items_recursive",
					  "Failed to find 'up' item");
			return;
		}

		node_number = (uint32 *)s_hash_element_get_data(he, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_items_recursive",
					  "Call to \"s_hash_element_get_data\" failed"))
			return;

		/* write it */
		S_EBMLWRITE_CALL(ebmlWriter, write_uint)(ebmlWriter, S_UTT_EBML_RELATION_ITEM_UP_NODE,
												 *node_number, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_items_recursive",
					  "ebmlWriter method \"write_uint\" failed"))
			return;
	}

	/* check daughter item (down) */
	itemToWrite = SItemDaughter(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "Call to \"SItemDaughter\" failed"))
		return;

	if (itemToWrite != NULL)
	{
		/* get node_number */
		he = s_hash_table_find(item_nodes, (void*)itemToWrite,
							   SObjectSize(S_OBJECT(itemToWrite), error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_items_recursive",
					  "Call to \"s_hash_table_find\" failed"))
			return;

		if (he == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "write_items_recursive",
					  "Failed to find 'up' item");
			return;
		}

		node_number = (uint32 *)s_hash_element_get_data(he, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_items_recursive",
					  "Call to \"s_hash_element_get_data\" failed"))
			return;

		/* write it */
		S_EBMLWRITE_CALL(ebmlWriter, write_uint)(ebmlWriter, S_UTT_EBML_RELATION_ITEM_DOWN_NODE,
												 *node_number, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_items_recursive",
					  "ebmlWriter method \"write_uint\" failed"))
			return;
	}

	/* check next item */
	itemToWrite = SItemNext(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "Call to \"SItemNext\" failed"))
		return;

	if (itemToWrite != NULL)
	{
		/* get node_number */
		he = s_hash_table_find(item_nodes, (void*)itemToWrite,
							   SObjectSize(S_OBJECT(itemToWrite), error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_items_recursive",
					  "Call to \"s_hash_table_find\" failed"))
			return;

		if (he == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "write_items_recursive",
					  "Failed to find 'up' item");
			return;
		}

		node_number = (uint32 *)s_hash_element_get_data(he, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_items_recursive",
					  "Call to \"s_hash_element_get_data\" failed"))
			return;

		/* write it */
		S_EBMLWRITE_CALL(ebmlWriter, write_uint)(ebmlWriter, S_UTT_EBML_RELATION_ITEM_NEXT_NODE,
												 *node_number, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_items_recursive",
					  "ebmlWriter method \"write_uint\" failed"))
			return;
	}

	/* check previous item */
	itemToWrite = SItemPrev(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_items_recursive",
				  "Call to \"SItemPrev\" failed"))
		return;

	if (itemToWrite != NULL)
	{
		/* get node_number */
		he = s_hash_table_find(item_nodes, (void*)itemToWrite,
							   SObjectSize(S_OBJECT(itemToWrite), error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_items_recursive",
					  "Call to \"s_hash_table_find\" failed"))
			return;

		if (he == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "write_items_recursive",
					  "Failed to find 'up' item");
			return;
		}

		node_number = (uint32 *)s_hash_element_get_data(he, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_items_recursive",
					  "Call to \"s_hash_element_get_data\" failed"))
			return;

		/* write it */
		S_EBMLWRITE_CALL(ebmlWriter, write_uint)(ebmlWriter, S_UTT_EBML_RELATION_ITEM_PREV_NODE,
												 *node_number, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_items_recursive",
					  "ebmlWriter method \"write_uint\" failed"))
			return;
	}
}


static void items_nodes_free_fp(void *key, void *data, s_erc *error)
{
	uint32 *node;


	S_CLR_ERR(error);

	/* key's are items, we don't delete them */
	key = NULL;
	node = data;

	S_FREE(node);
}


static void write_relation_items(SEbmlWrite *ebmlWriter, const SItem *relItemHead,
								 s_hash_table *item_contents_table, s_erc *error)
{
	s_hash_table *item_nodes = NULL;
	uint32 node_count;


	S_CLR_ERR(error);

	/* start container, S_UTT_EBML_RELATION_ITEMS */
	S_EBMLWRITE_CALL(ebmlWriter, start_container)(ebmlWriter, S_UTT_EBML_RELATION_ITEMS,
												  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation_items",
				  "ebmlWriter method \"start_container\" failed"))
		goto quit_error;

	item_nodes = s_hash_table_new(&items_nodes_free_fp, 9, error); /* 2^9 = 512 nodes */
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation_items",
				  "Call to \"st_hash_table_new\" failed"))
		goto quit_error;

	node_count = 1;

	/* write all items in this relation */
	write_items_recursive(ebmlWriter, relItemHead, item_contents_table,
						  item_nodes, &node_count, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation_items",
				  "Call to \"write_items_recursive\" failed"))
		goto quit_error;

	/* stop container, S_UTT_EBML_RELATION_ITEMS */
	S_EBMLWRITE_CALL(ebmlWriter, stop_container)(ebmlWriter, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_relation_items",
			  "ebmlWriter method \"stop_container\" failed");

	/* errors go here as well as normal clean up code */
quit_error:
	if (item_nodes != NULL)
	{
		s_erc local_err = S_SUCCESS;

		s_hash_table_delete(item_nodes, &local_err);
		S_CHK_ERR(&local_err, S_CONTERR,
				  "write_relation_items",
				  "Call to \"s_hash_table_delete\" failed");
	}
}


static void write_utt_relations(SEbmlWrite *ebmlWriter, const SUtterance *utt,
								s_hash_table *item_contents_table, s_erc *error)
{
	SIterator *itr = NULL;


	S_CLR_ERR(error);

	/* start container, S_UTT_EBML_RELATIONS */
	S_EBMLWRITE_CALL(ebmlWriter, start_container)(ebmlWriter, S_UTT_EBML_RELATIONS,
												  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_utt_relations",
				  "ebmlWriter method \"start_container\" failed"))
		goto quit_error;

	/* iterate through the relations */
	itr = S_ITERATOR_GET(utt->relations, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_utt_relations",
				  "Call to \"S_ITERATOR_GET\" failed"))
		goto quit_error;

	for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
	{
		const char *rel_name;
		const SItem *relHeadItem;
		const SRelation *rel;


		rel = (SRelation*)SIteratorObject(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_utt_relations",
					  "Call to \"SIteratorObject\" failed"))
			goto quit_error;

		relHeadItem = SRelationHead(rel, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_utt_relations",
					  "Call to \"SRelationHead\" failed"))
			goto quit_error;

		/* don't write empty relations */
		if (relHeadItem == NULL)
			continue;

		/* write relation name */
		rel_name = SRelationName(rel, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_utt_relations",
					  "Call to \"SRelationName\" failed"))
			goto quit_error;

		S_EBMLWRITE_CALL(ebmlWriter, write_utf8)(ebmlWriter, S_UTT_EBML_RELATION_NAME,
												 rel_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_utt_relations",
					  "ebmlWriter method \"write_utf8\" failed"))
			goto quit_error;

		/* write relation items */
		write_relation_items(ebmlWriter, relHeadItem, item_contents_table, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_utt_relations",
					  "Call to \"write_relation_items\" failed"))
			goto quit_error;
	}

	/* stop container, S_UTT_EBML_RELATIONS */
	S_EBMLWRITE_CALL(ebmlWriter, stop_container)(ebmlWriter, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_utt_relations",
				  "ebmlWriter method \"stop_container\" failed"))
		goto quit_error;

	/* here everything went OK */
	return;

	/* errors here, clean up code */
quit_error:
	if (itr != NULL)
		S_DELETE(itr, "write_utt_relations", error);
}

