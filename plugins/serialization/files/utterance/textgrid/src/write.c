/************************************************************************************/
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                Simone Daminato                                                   */
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
/* DATE    : June 2012                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Write an utterance to a Praat TextGrid format file.                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "utt_textgrid.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void textgrid_header(SDatasource *ds, float end_time, int size, s_erc *error);

static float get_segment_start(const SItem *segment, s_erc *error);

static float get_segment_end(const SItem *segment, s_erc *error);

static float get_syllable_start(const SItem *syllable, s_erc *error);

static float get_syllable_end(const SItem *syllable, s_erc *error);

static float get_word_start(const SItem *word, s_erc *error);

static float get_word_end(const SItem *word, s_erc *error);

static float get_phrase_start(const SItem *phrase, s_erc *error);

static float get_phrase_end(const SItem *phrase, s_erc *error);

static float get_start(const SItem *item, const char *relname, s_erc *error);

static float get_end(const SItem *item, const char *relname, s_erc *error);

static void add_info(SList *info, float start, float end, const char *name, s_erc *error);

static int write_relation(SDatasource *ds, const SRelation *rel,
						  int num_tiers, float end_time, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

typedef float (*get_item_time)(const SItem *phrase, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

typedef struct
{
	const char *name;
	get_item_time start_func;
	get_item_time end_func;
} s_relation_time;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static const s_relation_time rt[] =
{
	{ "Phrase", get_phrase_start, get_phrase_end },
	{ "Word", get_word_start, get_word_end },
	{ "Syllable", get_syllable_start, get_syllable_end },
	{ "Segment", get_segment_start, get_segment_end },
	{ NULL, NULL, NULL}
};


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void s_write_utt_textgrid(const SUtterance *utt, SDatasource *ds, s_erc *error)
{
	const SRelation *phraseRelation = NULL;
	const SRelation *wordRelation = NULL;
	const SRelation *syllableRelation = NULL;
	const SRelation *segmentRelation = NULL;
	int num_tiers = 0;
	int size = 0;
	s_bool is_present;
	const SItem *tmp;
	float end_time = 0.0;


	S_CLR_ERR(error);

	/* get relations, Phrase */
	is_present = SUtteranceRelationIsPresent(utt, "Phrase", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_textgrid",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto quit;

	if (is_present)
	{
		phraseRelation = SUtteranceGetRelation(utt, "Phrase", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_write_utt_textgrid",
					  "Call to \"SUtteranceGetRelation\" failed"))
			goto quit;

		size += 1;
	}

	/* get relations, Word */
	is_present = SUtteranceRelationIsPresent(utt, "Word", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_textgrid",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto quit;

	if (is_present)
	{
		wordRelation = SUtteranceGetRelation(utt, "Word", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_write_utt_textgrid",
					  "Call to \"SUtteranceGetRelation\" failed"))
			goto quit;
		size += 1;
	}

	/* get relations, Syllable */
	is_present = SUtteranceRelationIsPresent(utt, "Syllable", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_textgrid",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto quit;

	if (is_present)
	{
		syllableRelation = SUtteranceGetRelation(utt, "Syllable", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_write_utt_textgrid",
					  "Call to \"SUtteranceGetRelation\" failed"))
			goto quit;

		size += 1;
	}

	/* get relations, Segment */
	is_present = SUtteranceRelationIsPresent(utt, "Segment", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_textgrid",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto quit;

	if (!is_present)
	{
		/* require at least segment relation */
		S_CTX_ERR(error, S_FAILURE,
				  "s_write_utt_textgrid",
				  "Failed to find 'Segment' relation in utterance");
		goto quit;
	}
	else
	{
		segmentRelation = SUtteranceGetRelation(utt, "Segment", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_write_utt_textgrid",
					  "Call to \"SUtteranceGetRelation\" failed"))
			goto quit;

		size += 1;
	}

	tmp = SRelationTail(segmentRelation, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_textgrid",
				  "Call to \"SRelationTail\" failed"))
		goto quit;

	end_time = SItemGetFloat(tmp, "end", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_textgrid",
				  "Call to \"SItemGetFloat\" failed"))
		goto quit;

	/* write textgrid header */
	textgrid_header(ds, end_time, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_textgrid",
				  "Call to \"textgrid_header\" failed"))
		goto quit;

	num_tiers = write_relation(ds, phraseRelation, num_tiers, end_time, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_textgrid",
				  "Call to \"write_relation\" failed"))
		goto quit;

	num_tiers = write_relation(ds, wordRelation, num_tiers, end_time, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_textgrid",
				  "Call to \"write_relation\" failed"))
		goto quit;

	num_tiers = write_relation(ds, syllableRelation, num_tiers, end_time, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_textgrid",
				  "Call to \"write_relation\" failed"))
		goto quit;

	num_tiers = write_relation(ds, segmentRelation, num_tiers, end_time, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_write_utt_textgrid",
				  "Call to \"write_relation\" failed"))
		goto quit;

/* cleanup */
quit:
	if (ds != NULL)
		S_DELETE(ds, "s_write_utt_textgrid", error);
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void textgrid_header(SDatasource *ds, float end_time, int size, s_erc *error)
{
	const char *str = "File type = \"ooTextFile\"\nObject class = \"TextGrid\"\n\nxmin = 0\nxmax = %f\ntiers? <exists>\nsize = %d\nitem []:\n";
	char *buff = NULL;
	size_t str_size;


	S_CLR_ERR(error);
	s_asprintf(&buff, error, str, end_time, size);
	if (S_CHK_ERR(error, S_CONTERR,
				  "textgrid_header",
				  "Call to \"s_asprintf\" failed"))
		return;

	str_size = s_strsize(buff, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "textgrid_header",
				  "Call to \"s_strsize\" failed"))
	{
		S_FREE(buff);
		return;
	}

	SDatasourceWrite(ds, buff, sizeof(char), str_size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "textgrid_header",
				  "Call to \"SDatasourceWrite\" failed"))
	{
		S_FREE(buff);
		return;
	}

	S_FREE(buff);
}


static float get_segment_start(const SItem *segment, s_erc *error)
{
	float start = 0.0;


	S_CLR_ERR(error);
	start = SItemGetFloat(segment, "start", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_segment_start",
				  "Call to \"SItemGetFloat\" failed"))
		return 0.0;

	return start;
}


static float get_segment_end(const SItem *segment, s_erc *error)
{
	float end = 0.0;


	S_CLR_ERR(error);
	end = SItemGetFloat(segment, "end", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_segment_end",
				  "Call to \"SItemGetFloat\" failed"))
		return 0.0;

	return end;
}


static float get_syllable_start(const SItem *syllable, s_erc *error)
{
	const SItem *syllableSylStructure;
	const SItem *firstSegment;
	float start = 0.0;


	S_CLR_ERR(error);
	syllableSylStructure = SItemAs(syllable, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_syllable_start",
				  "Call to \"SItemAs\" failed"))
		return 0.0;

	firstSegment = SItemDaughter(syllableSylStructure, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_syllable_start",
				  "Call to \"SItemDaughter\" failed"))
		return 0.0;

	start = get_segment_start(firstSegment, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_syllable_start",
				  "Call to \"get_segment_start\" failed"))
		return 0.0;

	return start;
}


static float get_syllable_end(const SItem *syllable, s_erc *error)
{
	const SItem *syllableSylStructure;
	const SItem *lastSegment;
	float end = 0.0;


	S_CLR_ERR(error);
	syllableSylStructure = SItemAs(syllable, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_syllable_end",
				  "Call to \"SItemAs\" failed"))
		return 0.0;

	lastSegment = SItemLastDaughter(syllableSylStructure, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_syllable_end",
				  "Call to \"SItemLastDaughter\" failed"))
		return 0.0;

	end = get_segment_end(lastSegment, error);
	if (S_CHK_ERR(error, S_CONTERR,
					  "get_syllable_end",
					  "Call to \"get_segment_end\" failed"))
			return 0.0;

	return end;
}


static float get_word_start(const SItem *word, s_erc *error)
{
	const SItem *wordSylStructure;
	const SItem *firstSyllable;
	float start = 0.0;


	S_CLR_ERR(error);
	wordSylStructure = SItemAs(word, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_word_start",
				  "Call to \"SItemAs\" failed"))
		return 0.0;

	if (wordSylStructure == NULL) return start;
	firstSyllable = SItemDaughter(wordSylStructure, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_word_start",
				  "Call to \"SItemDaughter\" failed"))
		return 0.0;

	if (firstSyllable == NULL) return start;
	start = get_syllable_start(firstSyllable, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_word_start",
				  "Call to \"get_syllable_start\" failed"))
		return 0.0;

	return start;
}


static float get_word_end(const SItem *word, s_erc *error)
{
	const SItem *wordSylStructure;
	const SItem *lastSyllable;
	float end = 0.0;


	S_CLR_ERR(error);
	wordSylStructure = SItemAs(word, "SylStructure", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_word_end",
				  "Call to \"SItemAs\" failed"))
		return 0.0;

	if (wordSylStructure == NULL) return end;
	lastSyllable = SItemLastDaughter(wordSylStructure, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_word_end",
				  "Call to \"SItemLastDaughter\" failed"))
		return 0.0;

	if (lastSyllable == NULL) return end;
	end = get_syllable_end(lastSyllable, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_word_end",
				  "Call to \"get_syllable_end\" failed"))
		return 0.0;

	return end;
}


static float get_phrase_start(const SItem *phrase, s_erc *error)
{
	const SItem *firstWord;
	float start = 0.0;


	S_CLR_ERR(error);
	firstWord = SItemDaughter(phrase, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_phrase_start",
				  "Call to \"SItemDaughter\" failed"))
		return 0.0;

	start = get_word_start(firstWord, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_phrase_start",
				  "Call to \"get_word_start\" failed"))
		return 0.0;

	return start;
}


static float get_phrase_end(const SItem *phrase, s_erc *error)
{
	const SItem *lastWord;
	float end = 0.0;


	S_CLR_ERR(error);
	lastWord = SItemLastDaughter(phrase, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_phrase_end",
				  "Call to \"SItemLastDaughter\" failed"))
		return 0.0;

	end = get_word_end(lastWord, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_phrase_end",
				  "Call to \"get_word_end\" failed"))
		return 0.0;

	return end;
}


static float get_start(const SItem *item, const char *relname, s_erc *error)
{
	const s_relation_time *i;
	get_item_time start_func = NULL;
	int cmp;
	float start = 0.0;


	S_CLR_ERR(error);
	i = rt;
	while (i->name != NULL)
	{
		cmp = s_strcmp(relname, i->name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_start",
					  "Call to \"s_strcmp\" failed"))
			return 0.0;

		if (cmp == 0)
		{
			start_func = i->start_func;
			break;
		}

		i++;
	}

	if (start_func == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "get_start",
				  "Failed to find start function of relation \"%s\"", relname);
		return 0.0;
	}

	start = (start_func)(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_start",
				  "Call to \"start_func\" of relation \"%s\" failed", relname))
		return 0.0;

	return start;
}


static float get_end(const SItem *item, const char *relname, s_erc *error)
{
	const s_relation_time *i;
	get_item_time end_func = NULL;
	int cmp;
	float end = 0.0;


	S_CLR_ERR(error);
	i = rt;
	while (i->name != NULL)
	{
		cmp = s_strcmp(relname, i->name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_end",
					  "Call to \"s_strcmp\" failed"))
			return 0.0;

		if (cmp == 0)
		{
			end_func = i->end_func;
			break;
		}

		i++;
	}

	if (end_func == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "get_end",
				  "Failed to find end function of relation \"%s\"", relname);
		return 0.0;
	}

	end = (end_func)(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_end",
				  "Call to \"end_func\" of relation \"%s\" failed", relname))
		return 0.0;

	return end;
}


static int write_relation(SDatasource *ds, const SRelation *rel,
						  int num_tiers, float end_time, s_erc *error)
{
	const char *relname;
	const SItem *head = NULL;
	const char *tier_header = "\titem [%d]:\n\t\tclass = \"IntervalTier\"\n\t\tname = \"%s\"\n";
	const char *tier_start_end = "\t\txmin = 0\n\t\txmax = %f\n";
	const char *tier_intervals = "\t\tintervals: size = %d\n";
	const char *tier_item = " \t\tintervals [%d]:\n\t\t\txmin = %f\n\t\t\txmax = %f\n\t\t\ttext = \"%s\"\n";
	char *buff;
	size_t str_size;
	float prev_end = 0.0;
	const SItem *itr;
	float item_start = 0.0;
	float item_end = 0.0;
	const char *item_name = NULL;
	SList *info;
	size_t l_size;
	int counter;
	SIterator *litr;


	S_CLR_ERR(error);
	if (rel == NULL)
		return num_tiers;

	num_tiers += 1;
	relname = SRelationName(rel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Call to \"SRelationName\" failed"))
		return 0;

	head = SRelationHead(rel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Call to \"SRelationHead\" failed"))
		return 0;

	/* tier header */
	s_asprintf(&buff, error, tier_header, num_tiers, relname);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Call to \"s_asprintf\" failed"))
		return 0;

	str_size = s_strsize(buff, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Call to \"s_strsize\" failed"))
	{
		S_FREE(buff);
		return 0;
	}

	SDatasourceWrite(ds, buff, sizeof(char), str_size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Call to \"SDatasourceWrite\" failed"))
	{
		S_FREE(buff);
		return 0;
	}

	S_FREE(buff);

	/* start time and end time (always 0 and utt end) */
	s_asprintf(&buff, error, tier_start_end, end_time);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Call to \"s_asprintf\" failed"))
		return 0;

	str_size = s_strsize(buff, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Call to \"s_strsize\" failed"))
	{
		S_FREE(buff);
		return 0;
	}

	SDatasourceWrite(ds, buff, sizeof(char), str_size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Call to \"SDatasourceWrite\" failed"))
	{
		S_FREE(buff);
		return 0;
	}

	S_FREE(buff);

	info = S_LIST(S_NEW(SListList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Failed to create new list for info cache"))
		return 0;

	prev_end = 0.0;
	itr = head;
	while (itr != NULL)
	{
		item_start = get_start(itr, relname, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"get_start\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			return 0;
		}

		item_end = get_end(itr, relname, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"get_end\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			return 0;
		}

		if (!s_float_equal(item_start, prev_end))
		{
			/* prepend extra "SIL" */
			add_info(info, prev_end, item_start, "SIL", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "write_relation",
						  "Call to \"add_info\" failed"))
			{
				S_DELETE(info, "write_relation", error);
				return 0;
			}
		}

		item_name = SItemGetName(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"SItemGetName\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			return 0;
		}

		add_info(info, item_start, item_end, item_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"add_info\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			return 0;
		}

		prev_end = item_end;
		itr = SItemNext(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"SItemNext\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			return 0;
		}
	}

	if (!s_float_equal(item_end, end_time))
	{
		add_info(info, item_end, end_time, "SIL", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"add_info\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			return 0;
		}
	}

	l_size = SListSize(info, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Call to \"SListSize\" failed"))
	{
		S_DELETE(info, "write_relation", error);
		return 0;
	}

	/* tier intervals */
	s_asprintf(&buff, error, tier_intervals, l_size);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Call to \"s_asprintf\" failed"))
	{
		S_DELETE(info, "write_relation", error);
		return 0;
	}

	str_size = s_strsize(buff, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Call to \"s_strsize\" failed"))
	{
		S_FREE(buff);
		S_DELETE(info, "write_relation", error);
		return 0;
	}

	SDatasourceWrite(ds, buff, sizeof(char), str_size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Call to \"SDatasourceWrite\" failed"))
	{
		S_FREE(buff);
		S_DELETE(info, "write_relation", error);
		return 0;
	}

	S_FREE(buff);

	litr = S_ITERATOR_GET(info, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_relation",
				  "Call to \"S_ITERATOR_GET\" failed"))
	{
		S_DELETE(info, "write_relation", error);
		return 0;
	}

	for (counter = 1; litr != NULL; litr = SIteratorNext(litr), counter++)
	{
		SList *itemList;
		SObject *tmp;
		char *name;


		itemList = S_LIST(SIteratorObject(litr, error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"SIteratorObject\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			S_DELETE(litr, "write_relation", error);
			return 0;
		}

		tmp = SListPop(itemList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"SIteratorObject\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			S_DELETE(litr, "write_relation", error);
			return 0;
		}

		item_name = SObjectGetString(tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"SObjectGetString\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			S_DELETE(litr, "write_relation", error);
			S_DELETE(tmp, "write_relation", error);
			return 0;
		}

		name = s_strdup(item_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"s_strdup\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			S_DELETE(litr, "write_relation", error);
			S_DELETE(tmp, "write_relation", error);
			return 0;
		}

		S_DELETE(tmp, "write_relation", error);

		tmp = SListPop(itemList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"SIteratorObject\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			S_DELETE(litr, "write_relation", error);
			S_FREE(name);
			return 0;
		}

		item_end = SObjectGetFloat(tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"SObjectGetFloat\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			S_DELETE(litr, "write_relation", error);
			S_DELETE(tmp, "write_relation", error);
			S_FREE(name);
			return 0;
		}

		S_DELETE(tmp, "write_relation", error);

		tmp = SListPop(itemList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"SIteratorObject\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			S_DELETE(litr, "write_relation", error);
			S_FREE(name);
			return 0;
		}

		item_start = SObjectGetFloat(tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"SObjectGetFloat\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			S_DELETE(litr, "write_relation", error);
			S_DELETE(tmp, "write_relation", error);
			S_FREE(name);
			return 0;
		}

		S_DELETE(tmp, "write_relation", error);

		/* tier item */
		s_asprintf(&buff, error, tier_item, counter, item_start, item_end, name);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"s_asprintf\" failed"))
		{
			S_DELETE(info, "write_relation", error);
			S_DELETE(litr, "write_relation", error);
			S_FREE(name);
			return 0;
		}

		str_size = s_strsize(buff, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"s_strsize\" failed"))
		{
			S_FREE(buff);
			S_DELETE(info, "write_relation", error);
			S_DELETE(litr, "write_relation", error);
			S_FREE(name);
			return 0;
		}

		SDatasourceWrite(ds, buff, sizeof(char), str_size, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_relation",
					  "Call to \"SDatasourceWrite\" failed"))
		{
			S_FREE(buff);
			S_DELETE(info, "write_relation", error);
			S_DELETE(litr, "write_relation", error);
			S_FREE(name);
			return 0;
		}

		S_FREE(buff);
		S_FREE(name);
	}

	S_DELETE(info, "write_relation", error);
	return num_tiers;
}


static void add_info(SList *info, float start, float end, const char *name, s_erc *error)
{
	SList *itemList = NULL;
	SObject *tmp = NULL;


	S_CLR_ERR(error);
	itemList = S_LIST(S_NEW(SListList, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_info",
				  "Failed to create new list for item info"))
		goto quit;

	tmp = SObjectSetFloat(start, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_info",
				  "Call to \"SObjectSetFloat\" failed"))
		goto quit;

	SListAppend(itemList, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_info",
				  "Call to \"SListAppend\" failed"))
		goto quit;

	tmp = SObjectSetFloat(end, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_info",
				  "Call to \"SObjectSetFloat\" failed"))
		goto quit;

	SListAppend(itemList, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_info",
				  "Call to \"SListAppend\" failed"))
		goto quit;

	tmp = SObjectSetString(name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_info",
				  "Call to \"SObjectSetString\" failed"))
		goto quit;

	SListAppend(itemList, tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_info",
				  "Call to \"SListAppend\" failed"))
		goto quit;

	tmp = NULL;
	SListAppend(info, S_OBJECT(itemList), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "add_info",
				  "Call to \"SListAppend\" failed"))
		goto quit;

	itemList = NULL;

quit:
	if (itemList != NULL)
		S_DELETE(itemList, "add_info", error);

	if (tmp != NULL)
		S_DELETE(tmp, "add_info", error);
}
