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
/* A MorphDecomp utterance processor.                                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "g2p.h"
#include "morphdecomp_proc.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SMorphDecompUttProcClass MorphDecompUttProcClass; /* SMorphDecompUttProcClass class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void s_get_lexical_objects(SUtterance *utt, SG2P **g2p,
								  SFeatProcessor **morphDecompFeatProc, s_erc *error);

static const SItem *word_get_next_item(const SItem *wordItem, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_morphdecomp_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&MorphDecompUttProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_morphdecomp_utt_proc_class_reg",
			  "Failed to register SMorphDecompUttProcClass");
}


S_LOCAL void _s_morphdecomp_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&MorphDecompUttProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_morphdecomp_utt_proc_class_free",
			  "Failed to free SMorphDecompUttProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void s_get_lexical_objects(SUtterance *utt, SG2P **g2p,
								  SFeatProcessor **morphDecompFeatProc, s_erc *error)
{
	const SVoice *voice;


	S_CLR_ERR(error);

	voice = SUtteranceVoice(utt, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_lexical_objects",
				  "Call to \"SUtteranceVoice\" failed"))
		return;

	*g2p = (SG2P*)SVoiceGetData(voice , "g2p", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_lexical_objects",
				  "Call to \"SVoiceGetData\" failed"))
		return;

	*morphDecompFeatProc = (SFeatProcessor*)SVoiceGetFeatProc(voice, "morphdecomp", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_lexical_objects",
				  "Call to \"SVoiceGetFeatProc\" failed"))
		return;

	/* now check for least requirements */
	if ((*g2p == NULL) || (*morphDecompFeatProc == NULL))
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_lexical_objects",
				  "No g2p or morphological decomposition feature processor found in voice");
		return;
	}
}


static const SItem *word_get_next_item(const SItem *wordItem, s_erc *error)
{
	const SItem *nextItem;
	const SItem *syl;


	S_CLR_ERR(error);
	if (wordItem == NULL)
		return NULL;

	nextItem = SItemNext(wordItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "word_get_next_item",
				  "Call to \"SItemNext\" failed"))
		return NULL;

	if (nextItem != NULL)
		return nextItem;  /* easy */

	/* now check syllables */
	syl = s_path_to_item(wordItem, "R:SylStructure.parent", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "word_get_next_item",
				  "Call to \"s_path_to_item\" failed"))
		return NULL;

	if (syl == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "word_get_next_item",
				  "Failed to get item's syllable");
		return NULL;
	}

	syl = SItemNext(syl, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "word_get_next_item",
				  "Call to \"SItemNext\" failed"))
		return NULL;

	if (syl == NULL)
		return NULL;

	nextItem = SItemDaughter(syl, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "word_get_next_item",
				  "Call to \"SItemDaughter\" failed"))
		return NULL;

	return nextItem;
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void Run(const SUttProcessor *self, SUtterance *utt,
				s_erc *error)
{
	SG2P *g2p = NULL;
	SFeatProcessor *morphDecompFeatProc = NULL;
	const SRelation *wordRel;
	SRelation *morphRel = NULL;
	SRelation *morphStructRel = NULL;
	s_bool is_present;
	const SItem *wordItem;
	SMap *morphInfo = NULL;
	char *downcase_word = NULL;
	SIterator *morphItr = NULL;
	SIterator *morphClassItr = NULL;


	S_CLR_ERR(error);
	s_get_lexical_objects(utt, &g2p, &morphDecompFeatProc, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_get_lexical_objects\" failed"))
		goto quit_error;

	/* we require the word relation */
	is_present = SUtteranceRelationIsPresent(utt, "Word", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto quit_error;

	if (!is_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Word' relation in utterance");
		goto quit_error;
	}

	wordRel = SUtteranceGetRelation(utt, "Word", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
		goto quit_error;

	/* make sure the Segment relation exists*/
	is_present = SUtteranceRelationIsPresent(utt, "Segment", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto quit_error;

	if (!is_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Segment' relation in utterance");
		goto quit_error;
	}

	/* create relations */
	morphRel = SUtteranceNewRelation(utt, "Morphemes", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceNewRelation\" failed"))
		goto quit_error;

	morphStructRel = SUtteranceNewRelation(utt, "MorphStruct", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceNewRelation\" failed"))
		goto quit_error;


	/* iterate through words and get morphemes */
	wordItem = SRelationHead(wordRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	while (wordItem != NULL)
	{
		const SList *morphList;
		const SList *classList;
		SItem *morphStructureWordItem;
		const SItem *segmentItem;
		size_t word_size;
		uint word_index_counter = 0;

		/* get word and downcase it */
		downcase_word = s_strlwr(s_strdup(SItemGetName(wordItem, error), error), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Failed to down-case word item"))
			goto quit_error;

		word_size = s_strlen(downcase_word, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_strlen\" failed"))
			goto quit_error;

		/* get first segment (phone) of this word */
		segmentItem = s_path_to_item(wordItem, "R:SylStructure.daughter.daughter", error);

		/* get morphemes and their classes */
		morphInfo = (SMap*)SFeatProcessorRun(morphDecompFeatProc, wordItem, error);
		if (S_CHK_ERR(error, S_FAILURE,
					  "Run",
					  "Call to morphological decomposition feature processor failed"))
			goto quit_error;

		/* morphemes */
		morphList = (SList*)SMapGetObject(morphInfo, "morphemes", error);

		/* and their classes */
		classList = (SList*)SMapGetObject(morphInfo, "classes", error);


		/* create new morpheme structure word item, shares content
		 * with word item.
		 */
		morphStructureWordItem = SRelationAppend(morphStructRel, wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SRelationAppend\" failed"))
			goto quit_error;

		/* iterate over morphemes and their classes */
		morphItr = S_ITERATOR_GET(morphList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"S_ITERATOR_GET\" failed"))
			goto quit_error;

		morphClassItr = S_ITERATOR_GET(classList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"S_ITERATOR_GET\" failed"))
			goto quit_error;

		while ((morphItr != NULL) && (morphClassItr != NULL))
		{
			SItem *morphemeItem;
			SItem *morphStructMorphItem;
			const SObject *morphemeName;
			const SObject *morphemeClass;
			const char *morpheme_graphemes;
			size_t morpheme_graphemes_size;
			uint i;


			/* new item in morpheme relation */
			morphemeItem = SRelationAppend(morphRel, NULL, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SRelationAppend\" failed"))
				goto quit_error;

			/* get morpheme name and set it in item */
			morphemeName = SIteratorObject(morphItr, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SIteratorObject\" failed"))
				goto quit_error;

			SItemSetObject(morphemeItem, "name", morphemeName, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemSetObject\" failed"))
				goto quit_error;

			morpheme_graphemes = SObjectGetString(morphemeName, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SObjectGetString\" failed"))
				goto quit_error;

			morpheme_graphemes_size = s_strlen(morpheme_graphemes, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"s_strlen\" failed"))
				goto quit_error;

			/* get morpheme class and set it in item */
			morphemeClass = SIteratorObject(morphClassItr, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SIteratorObject\" failed"))
				goto quit_error;

			SItemSetObject(morphemeItem, "class", morphemeClass, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemSetObject\" failed"))
				goto quit_error;

			/* daughter of above item, but in MorphStructure */
			morphStructMorphItem = SItemAddDaughter(morphStructureWordItem,
													morphemeItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to \"SItemAddDaughter\" failed"))
				goto quit_error;

			/* iterate through morpheme graphemes to get the mapping
			 * to the segment items
			 */
			for (i = word_index_counter; i < morpheme_graphemes_size + word_index_counter; i++)
			{
				const char *phone;
				SItem *morphStructSegItem;


				phone = S_G2P_CALL(g2p, apply_at)(g2p, downcase_word, i, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Run",
							  "Call to method \"apply_at\" (SG2P) failed"))
					goto quit_error;

				if (phone != NULL)
				{
					/* we have a match with segment, create a new daughter */
					morphStructSegItem = SItemAddDaughter(morphStructMorphItem,
														  segmentItem, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "Run",
								  "Call to \"SItemAddDaughter\" failed"))
						goto quit_error;

					/* move segment item on */
					segmentItem = word_get_next_item(segmentItem, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "Run",
								  "Call to \"word_get_next_item\" failed"))
						goto quit_error;

					if ((segmentItem == NULL)
						 && (i < morpheme_graphemes_size))

					{
						S_CTX_ERR(error, S_FAILURE,
								  "Run",
								  "Expected more segments (phones) in word");
						goto quit_error;
					}
				}
			}

			word_index_counter += morpheme_graphemes_size;

			morphItr = SIteratorNext(morphItr);
			morphClassItr = SIteratorNext(morphClassItr);
		}

		if ((morphItr != NULL) && (morphClassItr != NULL))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "Run",
					  "Number of morphemes and number of classes are not the same");
			goto quit_error;
		}

		S_DELETE(morphInfo, "Run", error);

		wordItem = SItemNext(wordItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;

		S_FREE(downcase_word);
	}

	/* here all is OK */
	return;


	/* error clean-up code */
quit_error:
	if (morphRel != NULL)
	{
		SUtteranceDelRelation(utt, "Morphemes", error);
		S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceDelRelation\" failed");
	}

    if (morphStructRel != NULL)
    {
	    SUtteranceDelRelation(utt, "MorphStruct", error);
	    S_CHK_ERR(error, S_CONTERR,
		    	  "Run",
		    	  "Call to \"SUtteranceDelRelation\" failed");
    }

	if (morphInfo != NULL)
		S_DELETE(morphInfo, "Run", error);

	if (downcase_word != NULL)
		S_FREE(downcase_word);

	if (morphItr != NULL)
		S_DELETE(morphItr, "Run", error);

	if (morphClassItr != NULL)
		S_DELETE(morphClassItr, "Run", error);

	self = NULL;
}


/************************************************************************************/
/*                                                                                  */
/* SMorphDecompUttProc class initialization                                         */
/*                                                                                  */
/************************************************************************************/

static SMorphDecompUttProcClass MorphDecompUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:SMorphDecompUttProc",
		sizeof(SMorphDecompUttProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SUttProcessorClass */
	NULL,                /* initialize */
	Run                  /* run        */
};
