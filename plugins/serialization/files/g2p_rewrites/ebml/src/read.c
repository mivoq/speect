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
/* Read a g2p rewrites from an Ebml format file.                                    */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "serialized_g2p_rewrites.h"
#include "element_id.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void read_g2p_rewrites_def(SG2PRewrites *g2p, SEbmlRead *ebmlReader, s_erc *error);

static void read_g2p_rewrites_rules(SG2PRewrites *g2p, SEbmlRead *ebmlReader, s_erc *error);

static void read_g2p_rewrites_zeros(SG2PRewrites *g2p, SEbmlRead *ebmlReader, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SG2PRewrites *s_read_g2p_rewrites(SDatasource *ds, s_erc *error)
{
	SG2PRewrites *g2p = NULL;
	SEbmlRead *ebmlReader = NULL;
	uint32 id;
	s_bool container_exhausted;
	int scomp;


	S_CLR_ERR(error);

	/* create and initialize ebml reader */
	ebmlReader = S_NEW(SEbmlRead, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_g2p_rewrites",
				  "Failed to create new SEbmlRead object"))
		goto quit_error;

	S_EBMLREAD_CALL(ebmlReader, read_init)(&ebmlReader, ds, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_g2p_rewrites",
				  "Failed to initialize SEbmlRead object"))
		goto quit_error;

	/* check that we have the correct doc type */
	scomp = s_strcmp(ebmlReader->header->doctype, "spct_g2p_rewrites", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_g2p_rewrites",
				  "Call to \"s_strcmp\" failed"))
		goto quit_error;

	if (scomp != 0)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_read_g2p_rewrites",
				  "Ebml file format not of type 'spct_g2p_rewrites', read format is '%s'",
				  ebmlReader->header->doctype);
		goto quit_error;
	}

	/* create and initialize new g2p rewrites object */
	g2p = S_NEW(SG2PRewrites, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_g2p_rewrites",
				  "Failed to create new 'SG2PRewrites' object"))
		goto quit_error;

	/* read top level container */
	id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_g2p_rewrites",
				  "ebmlRead method \"container\" failed"))
		goto quit_error;

	/* sanity check id */
	S_EBML_ID_SANITY(id, S_G2PREWRITES_EBML,
					 "s_read_g2p_rewrites",
					 "ID mismatch", error);

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_g2p_rewrites",
					  "ebmlRead method \"container_at_end\" failed"))
			goto quit_error;

		if (container_exhausted)
			break; /* we are finished reading the g2p rewrites file */

		/* peek id for g2p rewrites elements */
		id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_g2p_rewrites",
					  "ebmlRead method \"peek_id\" failed"))
			goto quit_error;

		switch(id)
		{
		case S_G2PREWRITES_EBML_DEF:
		{
			read_g2p_rewrites_def(g2p, ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_read_g2p_rewrites",
						  "Call to \"read_g2p_rewrites_def\" failed"))
				goto quit_error;
			break;
		}
		case S_G2PREWRITES_EBML_SETS:
		{
			S_WARNING(S_WARNERR,
					  "s_read_g2p_rewrites",
					  "G2P Rewrites sets not yet implemented, skipping");

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_read_g2p_rewrites",
						  "ebmlRead method \"element_skip\" failed"))
				goto quit_error;
			break;
		}
		case S_G2PREWRITES_EBML_RULES_CONTAINER:
		{
			read_g2p_rewrites_rules(g2p, ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_read_g2p_rewrites",
						  "Call to \"read_g2p_rewrites_rules\" failed"))
				goto quit_error;

			break;
		}
		case S_G2PREWRITES_EBML_ZEROS:
		{
			read_g2p_rewrites_zeros(g2p, ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_read_g2p_rewrites",
						  "Call to \"read_g2p_rewrites_zeros\" failed"))
				goto quit_error;

			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "s_read_g2p_rewrites",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_read_g2p_rewrites",
						  "ebmlRead method \"element_skip\" failed"))
				goto quit_error;
		}
	}

	/* if we get here then everything was OK */
	goto quit;


	/* errors start clean up code here */
quit_error:
	if (g2p != NULL)
		S_DELETE(g2p, "s_read_g2p_rewrites", error); /* sets g2p = NULL */

	/* normal exit start clean up code here */
quit:
	if (ebmlReader != NULL)
		S_DELETE(ebmlReader, "s_read_g2p_rewrites", error);

	return g2p;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void read_g2p_rewrites_def(SG2PRewrites *g2p, SEbmlRead *ebmlReader, s_erc *error)
{
	SG2P *baseG2P = S_G2P(g2p);
	uint32 id;
	s_bool container_exhausted;


	S_CLR_ERR(error);

	/* read S_G2PREWRITES_EBML_DEF container */
	id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_g2p_rewrites_def",
				  "ebmlRead method \"container\" failed"))
		return;

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_g2p_rewrites_def",
					  "ebmlRead method \"container_at_end\" failed"))
			return;

		if (container_exhausted)
			break;

		/* peek id  */
		id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_g2p_rewrites_def",
					  "ebmlRead method \"peek_id\" failed"))
			return;

		switch(id)
		{
		case S_G2PREWRITES_EBML_DEF_NAME:
		{
			baseG2P->info->name = S_EBMLREAD_CALL(ebmlReader, read_ascii)(ebmlReader, &id,
																		  error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_def",
						  "ebmlRead method \"read_ascii\" failed"))
				return;

			break;
		}
		case S_G2PREWRITES_EBML_DEF_LANGUAGE:
		{
			baseG2P->info->language = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id,
																			 error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_def",
						  "ebmlRead method \"read_utf8\" failed"))
				return;

			break;
		}
		case S_G2PREWRITES_EBML_DEF_LANG_CODE:
		{
			baseG2P->info->lang_code = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id,
																			  error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_def",
						  "ebmlRead method \"read_utf8\" failed"))
				return;

			break;
		}
		case S_G2PREWRITES_EBML_DEF_DESCRIPTION:
		{
			baseG2P->info->description = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id,
																				error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_def",
						  "ebmlRead method \"read_utf8\" failed"))
				return;

			break;
		}
		case S_G2PREWRITES_EBML_DEF_VERSION:
		{
			/* read S_G2PREWRITES_EBML_DEF_VERSION container */
			id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_def",
						  "ebmlRead method \"container\" failed"))
				return;

			while (1)
			{
				container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_g2p_rewrites_def",
							  "ebmlRead method \"container_at_end\" failed"))
					return;

				if (container_exhausted)
					break;

				/* peek id  */
				id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_g2p_rewrites_def",
							  "ebmlRead method \"peek_id\" failed"))
					return;

				switch(id)
				{
				case S_G2PREWRITES_EBML_DEF_VERSION_MAJOR:
				{
					baseG2P->info->version.major = S_EBMLREAD_CALL(ebmlReader, read_uint)(ebmlReader, &id,
																						  error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_g2p_rewrites_def",
								  "ebmlRead method \"read_uint\" failed"))
						return;

					break;
				}
				case S_G2PREWRITES_EBML_DEF_VERSION_MINOR:
				{
					baseG2P->info->version.minor = S_EBMLREAD_CALL(ebmlReader, read_uint)(ebmlReader, &id,
																						  error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_g2p_rewrites_def",
								  "ebmlRead method \"read_uint\" failed"))
						return;

					break;
				}
				default:
					/* unknown elements, skip */
					S_WARNING(S_FAILURE,
							  "read_g2p_rewrites_def",
							  "Skipping element with unknown id '0x%x'",
							  id);

					S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_g2p_rewrites_def",
								  "ebmlRead method \"element_skip\" failed"))
						return;
				}
			}
			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_g2p_rewrites_def",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_def",
						  "ebmlRead method \"element_skip\" failed"))
				return;
		}
	}

	return;
}


static void read_g2p_rewrites_rules(SG2PRewrites *g2p, SEbmlRead *ebmlReader, s_erc *error)
{
	uint32 id;
	s_bool container_exhausted;
	SList *graphemeRuleList = NULL;
	SG2PRewritesRule *rule = NULL;


	S_CLR_ERR(error);

	/* read S_G2PREWRITES_EBML_RULES_CONTAINER container */
	id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_g2p_rewrites_rules",
				  "ebmlRead method \"container\" failed"))
		return;

	/* create rules SMap */
	g2p->rules = S_MAP(S_NEW(SMapHashTable, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_g2p_rewrites_rules",
				  "Failed to create new 'SMap' object"))
		return;

	/* 64 should be enough for graphemes */
	SMapHashTableResize(S_MAPHASHTABLE(g2p->rules), 64, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_g2p_rewrites_rules",
				  "Call to \"SMapHashTableResize\" failed"))
	{
		S_DELETE(g2p->rules, "read_g2p_rewrites_rules", error);
		return;
	}

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_g2p_rewrites_rules",
					  "ebmlRead method \"container_at_end\" failed"))
			return;

		if (container_exhausted)
			break;

		/* peek id  */
		id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_g2p_rewrites_rules",
					  "ebmlRead method \"peek_id\" failed"))
			return;

		switch(id)
		{
		case S_G2PREWRITES_EBML_RULES_GRAPHEME:
		{
			char *grapheme;


			grapheme = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_rules",
						  "ebmlRead method \"read_utf8\" failed"))
				return;

			/* create rule list for this grapheme */
			graphemeRuleList = S_LIST(S_NEW(SListList, error));
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_rules",
						  "Failed to create new 'SList' object"))
				return;

			/* add it to the rules SMap */
			SMapSetObject(g2p->rules, grapheme, S_OBJECT(graphemeRuleList), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_rules",
						  "Call to \"SMapSetObject\" failed"))
			{
				S_DELETE(graphemeRuleList, "read_g2p_rewrites_rules", error);
				S_FREE(grapheme);
				return;
			}

			S_FREE(grapheme);
			break;
		}
		case S_G2PREWRITES_EBML_SINGLE_RULE_CONTAINER:
		{
			/* read S_G2PREWRITES_EBML_SINGLE_RULE_CONTAINER container */
			id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_rules",
						  "ebmlRead method \"container\" failed"))
				return;

			while (1)
			{
				container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_g2p_rewrites_rules",
							  "ebmlRead method \"container_at_end\" failed"))
					return;

				if (container_exhausted)
					break;

				/* peek id  */
				id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_g2p_rewrites_rules",
							  "ebmlRead method \"peek_id\" failed"))
					return;

				switch(id)
				{
				case S_G2PREWRITES_EBML_SINGLE_RULE_LEFT_CONTEXT:
				{
					/* elements are ordered so we create a rule here */
					rule = S_NEW(SG2PRewritesRule, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_g2p_rewrites_rules",
								  "Failed to create new 'SG2PRewritesRule' object"))
						return;

					/* add it to the list */
					SListAppend(graphemeRuleList, S_OBJECT(rule), error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_g2p_rewrites_rules",
								  "Call to \"SListAppend\" failed"))
					{
						S_DELETE(rule, "read_g2p_rewrites_rules", error);
						return;
					}

					rule->left_context = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id,
																				error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_g2p_rewrites_rules",
								  "ebmlRead method \"read_utf8\" failed"))
						return;



					break;
				}
				case S_G2PREWRITES_EBML_SINGLE_RULE_RIGHT_CONTEXT:
				{
					rule->right_context = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id,
																				 error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_g2p_rewrites_rules",
								  "ebmlRead method \"read_utf8\" failed"))
						return;

					break;
				}
				case S_G2PREWRITES_EBML_SINGLE_RULE_PHONEME:
				{
					rule->phone = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id,
																		 error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_g2p_rewrites_rules",
								  "ebmlRead method \"read_utf8\" failed"))
						return;

					break;
				}
				default:
					/* unknown elements, skip */
					S_WARNING(S_FAILURE,
							  "read_g2p_rewrites_rules",
							  "Skipping element with unknown id '0x%x'",
							  id);

					S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_g2p_rewrites_rules",
								  "ebmlRead method \"element_skip\" failed"))
						return;
				}
			}
			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_g2p_rewrites_rules",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_rules",
						  "ebmlRead method \"element_skip\" failed"))
				return;
		}
	}

	return;
}


static void read_g2p_rewrites_zeros(SG2PRewrites *g2p, SEbmlRead *ebmlReader, s_erc *error)
{
	uint32 id;
	SList *tmpZeros;
	size_t list_size;
	SIterator *itr;
	s_bool container_exhausted;
	s_gzero *tmp;


	S_CLR_ERR(error);

	/* read S_G2PREWRITES_EBML_ZEROS container */
	id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_g2p_rewrites_zeros",
				  "ebmlRead method \"container\" failed"))
		return;

	/* create a temporary SList storage */
	tmpZeros = S_LIST(S_NEW(SListList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_g2p_rewrites_zeros",
				  "Failed to create new 'SList' object"))
		return;

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_g2p_rewrites_zeros",
					  "ebmlRead method \"container_at_end\" failed"))
		{
			S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
			return;
		}

		if (container_exhausted)
			break;

		/* peek id  */
		id = S_EBMLREAD_CALL(ebmlReader, peek_id)(ebmlReader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_g2p_rewrites_zeros",
					  "ebmlRead method \"peek_id\" failed"))
		{
			S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
			return;
		}


		switch(id)
		{
		case S_G2PREWRITES_EBML_ZEROS_SYMBOL:
		{
			char *symbol;


			symbol = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_zeros",
						  "ebmlRead method \"read_utf8\" failed"))
			{
				S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
				return;
			}

			/* put symbol in list */
			SListAppend(tmpZeros, SObjectSetString(symbol, error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_zeros",
						  "Call to \"SListAppend\" failed"))
			{
				S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
				S_FREE(symbol);
				return;
			}

			S_FREE(symbol);
			break;
		}
		case S_G2PREWRITES_EBML_ZEROS_REPLACEMENT:
		{
			char *replacement;


			replacement = S_EBMLREAD_CALL(ebmlReader, read_utf8)(ebmlReader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_zeros",
						  "ebmlRead method \"read_utf8\" failed"))
			{
				S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
				return;
			}

			/* put replacement in list */
			SListAppend(tmpZeros, SObjectSetString(replacement, error), error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_zeros",
						  "Call to \"SListAppend\" failed"))
			{
				S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
				S_FREE(replacement);
				return;
			}

			S_FREE(replacement);
			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_g2p_rewrites_zeros",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(ebmlReader, element_skip)(ebmlReader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_g2p_rewrites_zeros",
						  "ebmlRead method \"element_skip\" failed"))
				return;
		}
	}

	/* get list size */
	list_size = SListSize(tmpZeros, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_g2p_rewrites_zeros",
				  "Call to \"SListSize\" failed"))
	{
		S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
		return;
	}

	/* make sure it is divisible by 2 */
	if (list_size%2 != 0)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_g2p_rewrites_zeros",
				  "Number of read symbol/replacement zero's not divisible by 2");
		S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
		return;
	}

	/* create zeros structure */
	g2p->zeros = S_CALLOC(s_gzero, list_size + 1);
	if (g2p->zeros == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "read_g2p_rewrites_zeros",
				  "Failed to allocate memory for 's_gzero' object");
		S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
		return;
	}

	itr = S_ITERATOR_GET(tmpZeros, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_g2p_rewrites_zeros",
				  "Call to \"S_ITERATOR_GET\" failed"))
	{
		S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
		return;
	}

	tmp = g2p->zeros;

	while (itr != NULL)
	{
		const char *symbol;
		const char *replacement;


		symbol = SObjectGetString(SIteratorObject(itr, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_g2p_rewrites_zeros",
					  "Call to \"SIteratorObject/SObjectGetString\" failed"))
		{
			S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
			S_DELETE(itr, "read_g2p_rewrites_zeros", error);
			return;
		}

		tmp->symbol = s_strdup(symbol, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_g2p_rewrites_zeros",
					  "Call to \"s_strdup\" failed"))
		{
			S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
			S_DELETE(itr, "read_g2p_rewrites_zeros", error);
			return;
		}

		itr = SIteratorNext(itr);
		if (itr == NULL)
		{
			S_CTX_ERR(error, S_CONTERR,
					  "read_g2p_rewrites_zeros",
					  "Expected replacement zero is NULL");
			S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
			return;
		}

		replacement = SObjectGetString(SIteratorObject(itr, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_g2p_rewrites_zeros",
					  "Call to \"SListIterator/SObjectGetString\" failed"))
		{
			S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
			S_DELETE(itr, "read_g2p_rewrites_zeros", error);
			return;
		}

		tmp->replacement = s_strdup(replacement, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_g2p_rewrites_zeros",
					  "Call to \"s_strdup\" failed"))
		{
			S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
			S_DELETE(itr, "read_g2p_rewrites_zeros", error);
			return;
		}

		itr = SIteratorNext(itr);
		tmp++;
	}

	S_DELETE(tmpZeros, "read_g2p_rewrites_zeros", error);
	return;
}
