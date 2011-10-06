/************************************************************************************/
/* Copyright (c) 2011 The Department of Arts and Culture,                           */
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
/* DATE    : October 2011                                                           */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* HRG example. Creating the utterance structure as given in the HRG                */
/* topic guide in the documentation.                                                */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


#include <stdio.h>
#include "speect.h"


/* name of utterance ebml read/write plug-in */
static const char *utt_plugin_path = "utt_ebml.spi";

/* structure to hold word-syllable combinations */
typedef struct
{
	const char *word;
	const char * const * syllables;
} word_syls;

# define num_words 2

/*
 * phone strings with syllable boundaries marked by a digit that
 * indicates syllable stress
 */
static const char * const syllable_0[] = { "t", "w", "eh", "n", "1", "t", "iy", "0", NULL };
static const char * const syllable_1[] = { "f", "@", "th", "1", NULL };

/* word - syllables */
static const word_syls lexical_info[] = {
	{ "twenty", syllable_0 },
	{ "fifth", syllable_1 },
	{ NULL, NULL }
};



int main()
{
	s_erc error = S_SUCCESS;
	SUtterance *utt;
	SRelation *wordRel;
	SRelation *sylRel;
	SRelation *segmentRel;
	SRelation *sylStructRel;
	SItem *wordItem;
	SItem *sylItem;
	SItem *segmentItem;
	SItem *sylStructWordItem;
	SItem *sylStructSylItem;
	int i;
	int j;
	SPlugin *uttEBMLplugin = NULL;



	/*
	 * initialize speect
	 */
	error = speect_init(NULL);
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}

	/*
	 * load the utt-ebml plug-in
	 */
	uttEBMLplugin = s_pm_load_plugin(utt_plugin_path, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load plug-in at '%s'",utt_plugin_path))
	{
		printf("Failed to load plug-in\n");
		goto quit;
	}

	/* Create a new utterance */
	utt = S_NEW(SUtterance, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new utterance"))
		goto quit;

	/* initialize utterance */
	SUtteranceInit(&utt, NULL, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to initialize new utterance"))
		goto quit;

	/* Create a word relation  */
	wordRel = SUtteranceNewRelation(utt, "Word", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new relation"))
		goto quit;

	/* Create a syllable relation  */
	sylRel = SUtteranceNewRelation(utt, "Syllable", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new relation"))
		goto quit;

	/* Create a segment relation  */
	segmentRel = SUtteranceNewRelation(utt, "Segment", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new relation"))
		goto quit;

	/* Create a syllable structure relation  */
	sylStructRel = SUtteranceNewRelation(utt, "SylStructure", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new relation"))
		goto quit;

	for (i = 0; lexical_info[i].word; i++)
	{
		/* add word in word relation */
		wordItem = SRelationAppend(wordRel, NULL, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to create new appended item"))
			goto quit;

		SItemSetName(wordItem, lexical_info[i].word, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to set item name"))
			goto quit;

		/* add word in syllable structure relation,
		 * note the shared content wordItem
		 */
		sylStructWordItem = SRelationAppend(sylStructRel, wordItem, &error);

		/* create a syllable item in the syllable relation */
		sylItem = SRelationAppend(sylRel, NULL, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to create new appended item"))
			goto quit;

		/* syllable in syllable structure relation is daughter of
		 * word item in syllable structure relation, note
		 * shared content with syllable item */
		sylStructSylItem = SItemAddDaughter(sylStructWordItem, sylItem, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to add daughter item of item"))
			goto quit;

		for (j = 0; lexical_info[i].syllables[j]; j++)
		{
			int isdigit;
			uint32 c;
			const char *phone = lexical_info[i].syllables[j];
			int stress;

			c = s_getc(phone, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
						  "main",
						  "Failed to get first character of phone as UTF8 character"))
				goto quit;

			isdigit = s_isdigit(c, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
						  "main",
						  "Failed to determine if UTF8 character is a digit"))
				goto quit;

			if (!isdigit)
			{
				/* create a segment item in the segment relation */
				segmentItem = SRelationAppend(segmentRel, NULL, &error);
				if (S_CHK_ERR(&error, S_CONTERR,
							  "main",
							  "Failed to create new appended item"))
					goto quit;

				SItemSetName(segmentItem, phone, &error);
				if (S_CHK_ERR(&error, S_CONTERR,
							  "main",
							  "Failed to set item name"))
					goto quit;

				/* segment item is daughter of syllable item in
				   syllable structure relation */
				SItemAddDaughter(sylStructSylItem, segmentItem, &error);
				if (S_CHK_ERR(&error, S_CONTERR,
							  "main",
							  "Failed to add daughter item of item"))
					goto quit;
			}
			else
			{
				/* get stress value */
				stress = (int)s_strtol(phone, NULL, 10, &error);
				if (S_CHK_ERR(&error, S_CONTERR,
							  "main",
							  "Failed to convert string to integer"))
					goto quit;

				SItemSetInt(sylItem, "stress", stress, &error);
				if (S_CHK_ERR(&error, S_CONTERR,
							  "main",
							  "Failed to set item integer feature"))
					goto quit;

				if (lexical_info[i].syllables[j + 1] != NULL)
				{
					/* create a syllable item in the syllable relation */
					sylItem = SRelationAppend(sylRel, NULL, &error);
					if (S_CHK_ERR(&error, S_CONTERR,
								  "main",
								  "Failed to create new appended item"))
						goto quit;

					/* syllable in syllable structure relation is daughter of
					 * word item in syllable structure relation, note
					 * shared content with syllable item */
					sylStructSylItem = SItemAddDaughter(sylStructWordItem, sylItem, &error);
					if (S_CHK_ERR(&error, S_CONTERR,
								  "main",
								  "Failed to add daughter item of item"))
						goto quit;
				}
			}
		}
	}

	/* save utterance */
	SObjectSave(S_OBJECT(utt), "test.utt", "spct_utt", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to save utterance"))
		goto quit;


quit:
	/*
	 * utterance will delete the
	 * relation which in turn will
	 * delete it's items.
	 */
	if (utt != NULL)
		S_DELETE(utt, "main", &error);

	/* unload plug-ins by deleting them */
	if (uttEBMLplugin != NULL)
		S_DELETE(uttEBMLplugin, "main", &error);

	/*
	 * quit speect
	 */
	error = speect_quit();
	if (error != S_SUCCESS)
	{
		printf("Call to 'speect_quit' failed\n");
		return 1;
	}

	return 0;
}


