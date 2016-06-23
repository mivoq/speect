/************************************************************************************/
/* Copyright (c) 2015 Mivoq SRL <info@mivoq.it>                                     */
/*                                                                                  */
/* Contributors:  Matteo Lisotto                                                    */
/*                Simone Daminato                                                   */
/*                Giovanni Mazzocchin                                               */
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
/* AUTHOR  : Matteo Lisotto <matteo.lisotto@gmail.com>                              */
/* DATE    : 21 September 2015                                                      */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Write an utterance to a MaryXML format file.                                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "utt_maryxml.h"
#include <libxml/xmlwriter.h>
#include <string.h>
#define ENCODING "UTF-8"

/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

static int _ds_write(void * context, const char * buffer, int len)
{
	int ret = 0;
	s_erc error;
	S_CLR_ERR(&error);
	ret = SDatasourceWrite((SDatasource* ) context, buffer, sizeof(char), len, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
		      "s_write_utt_maryxml",
		      "Call to \"xmlOutputWrite\" failed"))
		return -1;
	return ret;
}

static int _ds_close(void * context)
{
	s_erc error;
	SDatasource * ds = (SDatasource*) context;
	S_CLR_ERR(&error);
	if (ds != NULL)
		S_DELETE(ds, "s_write_utt_maryxml", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
		      "s_write_utt_maryxml",
		      "Call to \"xmlOutputClose\" failed"))
		return -1;
	return 0;
}

static void write_syllable_tags(xmlTextWriterPtr writer, const SItem* wordSItem, s_erc* error)
{
	int rc;
	S_CLR_ERR(error);

	if (wordSItem == NULL)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "write_syllable_tags",
				   "no content to write in the tags");
		return;
	}

	/* get the first syllable of the current word */
	const SItem* itrSyllables = SItemPathToItem(wordSItem, "R:SylStructure.daughter",error);
	if (S_CHK_ERR(error, S_CONTERR,
		"write_word",
		"Call to \"SItemPathToItem\" failed"))
		return;

	/* for each syllable */
	while (itrSyllables != NULL)
	{
	    /* open syllable tag */
		rc = xmlTextWriterStartElement(writer, BAD_CAST "syllable");
		if (rc < 0)
		{
			S_CTX_ERR(error, S_CONTERR,
				  "write_syllable_tags",
				  "Call to \"xmlTextWriterStartElement\" failed");
			return;
		}
		/* get the first segment */
		const SItem* itrSegments = SItemDaughter(itrSyllables, error);
		if (S_CHK_ERR(error, S_CONTERR,
			"write_syllable_tags",
			"Call to \"SItemDaughter\" failed"))
			return;

		/* loop for ph tags */
		while (itrSegments != NULL)
		{
			rc = xmlTextWriterStartElement(writer, BAD_CAST "ph");
			if (rc < 0)
			{
				S_CTX_ERR(error, S_CONTERR,
					   "write_syllable_tags",
					   "Call to \"xmlTextWriterStartElement\" failed");
				return;
			}
			/* get segment content */
			const char* segmentName = SItemGetName(itrSegments, error);
			if (S_CHK_ERR(error, S_CONTERR,
				"write_syllable_tags",
				"Call to \"SItemGetName\" failed"))
				return;

			rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "p", BAD_CAST segmentName);
			if (rc < 0)
			{
				S_CTX_ERR(error, S_CONTERR,
					   "write_syllable_tags",
					   "Call to \"xmlTextWriterWriteAttribute\" failed");
				return;
			}

			rc = xmlTextWriterEndElement(writer);
			if (rc < 0)
			{
				S_CTX_ERR(error, S_CONTERR,
					  "write_syllable_tags",
					  "Call to \"xmlTextWriterEndElement\" failed");
				return;
			}

			/* get the next segment */
			itrSegments = SItemNext(itrSegments, error);
			if (S_CHK_ERR(error, S_CONTERR,
				"write_syllable_tags",
				"Call to \"SItemNext\" failed"))
				return;
		}

		/* close syllable tag */
		rc = xmlTextWriterEndElement(writer);
		if (rc < 0)
		{
			S_CTX_ERR(error, S_CONTERR,
				"write_syllable_tags",
				"Call to \"xmlTextWriterEndElement\" failed");
			return;
		}
		/* get next syllable */
		/* get the next syllable of the word */
		itrSyllables = SItemNext(itrSyllables, error);
		if (S_CHK_ERR(error, S_CONTERR,
			"write_syllable_tags",
			"Call to \"SItemNext\" failed"))
			return;
		}
}

static void write_t_tag(xmlTextWriterPtr writer, const char* content, const SItem* wordSItem, const char* pos, const char* segments, s_erc *error)
{
	int rc;

	S_CLR_ERR(error);

	if (content == NULL)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "write_t_tag",
				  "no content to write in the tag.");
		return;
	}

	/* open the t tag */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "t");
	if (rc < 0)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "write_t_tag",
				  "Call to \"xmlTextWriterStartElement\" failed");
		return;
	}

	/* write pos-tagging data if present */
	if (pos != NULL)
	{
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "pos", BAD_CAST pos);
		if (rc < 0)
		{
			S_CTX_ERR(error, S_CONTERR,
					  "write_t_tag",
					  "Call to \"xmlTextWriterWriteAttribute\" failed");
			return;
		}
	}

	/* write segments if present */
	if (segments != NULL)
	{
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "ph", BAD_CAST segments);
		if (rc < 0)
		{
			S_CTX_ERR(error, S_CONTERR,
					  "write_t_tag",
					  "Call to \"xmlTextWriterWriteAttribute\" failed");
			return;
		}
	}

	/* write the content */
	rc = xmlTextWriterWriteString(writer, BAD_CAST content);
	if (rc < 0)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "write_t_tag",
				  "Call to \"xmlTextWriterWriteString\" failed");
		return;
	}

	/* call write_syllable_tags */
	/* there's not any syllable when wordSItem is NULL */
	if (wordSItem)
	{
		S_CLR_ERR(error);
		write_syllable_tags(writer, wordSItem, error);
	}

	/* close t tag */
	rc = xmlTextWriterEndElement(writer);
	if (rc < 0)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "write_t_tag",
				  "Call to \"xmlTextWriterEndDocument\" failed");
		return;
	}
}

static void write_word(xmlTextWriterPtr writer, const SItem* wordSItem, const char* pos_string, s_bool ShouldWriteSegments, s_erc *error)
{
	S_CLR_ERR(error);

	const char* segments_string = NULL;
	const char* word = NULL;

	/* create segments string */
	s_buffer *buffer = s_buffer_new(error);
	if (S_CHK_ERR(error, S_CONTERR,
				"write_word",
				"Call to \"s_buffer_new\" failed"))
		return;

	if (ShouldWriteSegments)
	{
		/* get to the first syllable of the current word */
		const SItem* itrSyllables = SItemPathToItem(wordSItem, "R:SylStructure.daughter",error);
		if (S_CHK_ERR(error, S_CONTERR,
			"write_word",
			"Call to \"SItemPathToItem\" failed"))
			goto cleanup;
		/* for every syllable */
		while (itrSyllables != NULL)
		{
			/* get the first segment */
			const SItem* itrSegments = SItemDaughter(itrSyllables, error);
			if (S_CHK_ERR(error, S_CONTERR,
				"write_word",
				"Call to \"SItemDaughter\" failed"))
				goto cleanup;
			while (itrSegments != NULL)
			{
				/* get segment content */
				const char* segmentName = SItemGetName(itrSegments, error);
				if (S_CHK_ERR(error, S_CONTERR,
					"write_word",
					"Call to \"SItemGetName\" failed"))
					goto cleanup;

				/* append it */
				s_buffer_append(buffer, segmentName, strlen(segmentName), error);
				if (S_CHK_ERR(error, S_CONTERR,
					"write_word",
					"Call to \"s_buffer_append\" failed"))
					goto cleanup;

				/* get next segment */
				itrSegments = SItemNext(itrSegments, error);
				if (S_CHK_ERR(error, S_CONTERR,
					"write_word",
					"Call to \"SItemNext\" failed"))
					goto cleanup;

				if (itrSegments != NULL)
				{
					/* append the segment separator */
					s_buffer_append(buffer, " ", 1, error);
					if (S_CHK_ERR(error, S_CONTERR,
						"write_word",
						"Call to \"s_buffer_append\" failed"))
						goto cleanup;
				}
			}

			/* get next syllable */
			itrSyllables = SItemNext(itrSyllables, error);
			if (S_CHK_ERR(error, S_CONTERR,
				"write_word",
				"Call to \"SItemNext\" failed"))
				goto cleanup;

			if (itrSyllables != NULL)
			{
				/* append the syllable separator */
				s_buffer_append(buffer, " - ", 3, error);
				if (S_CHK_ERR(error, S_CONTERR,
					"write_word",
					"Call to \"s_buffer_append\" failed"))
					goto cleanup;
			}
		}

		/* get segments if present */
		size_t buff_size = s_buffer_size(buffer, error);
		if (buff_size != 0)
		{
			segments_string = (const char*)s_buffer_data(buffer, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "write_word",
						  "Call to \"s_buffer_data\" failed"))
				goto cleanup;
		}
	}

	word = SItemGetName(wordSItem, error);
	if (S_CHK_ERR(error, S_CONTERR,
		"write_word",
		"Call to \"SItemGetName\" failed"))
		goto cleanup;

	if (word != NULL /* && s_strcmp(word, "", error) != 0 */)
	{
		/* write the word */
		write_t_tag(writer, word, wordSItem, pos_string, segments_string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					"write_word",
					"Call to \"write_t_tag\" failed"))
			goto cleanup;
	}


cleanup:
	s_buffer_delete(buffer, error);
	S_CHK_ERR(error, S_CONTERR,
		  "write_word",
		  "Call to \"s_buffer_delete\" failed");
}

S_LOCAL void s_write_utt_maryxml(const SUtterance *utt, SDatasource *ds, s_erc *error)
{
	int rc;
	s_bool isPresent;
	xmlTextWriterPtr writer;
	xmlOutputBufferPtr out;
	const SItem * itrTokens = NULL;
	const SItem * currentPhrase = NULL;
	const char * maryNms = "http://mary.dfki.de/2002/MaryXML";
	const char * maryXsi = "http://www.w3.org/2001/XMLSchema-instance";
	const char * pos_string = NULL;

	s_bool isSegmentRelationPresent;
	s_bool isPhraseRelationPresent;

	S_CLR_ERR(error);

	/* init relation bools */
	isPresent = SUtteranceRelationIsPresent(utt, "Token", error);
	if(S_CHK_ERR(error, S_CONTERR,
		      "s_write_utt_maryxml",
		      "Call to \"SUtteranceRelationIsPresent\" failed"))
		return;

	/* check that we have a token relation graph. If not, we have nothing to do, so just return */
	if(!isPresent)
		return;

	isSegmentRelationPresent = SUtteranceRelationIsPresent(utt, "Segment", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_write_utt_maryxml",
		      "Call to \"SUtteranceRelationIsPresent\" failed"))
		return;

	isPhraseRelationPresent = SUtteranceRelationIsPresent(utt, "Phrase", error);
	if (S_CHK_ERR(error, S_CONTERR,
		"s_write_utt_maryxml",
	       "Call to \"SUtteranceRelationIsPresent\" failed"))
		return;

	out = xmlOutputBufferCreateIO(_ds_write,
				      _ds_close,
				      ds,
				      NULL);
	if (out == NULL)
	{
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlOutputBufferCreateIO\" failed");
		return;
	}
	writer = xmlNewTextWriter(out);
	if (writer == NULL)
	{
                xmlOutputBufferClose(out);
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlNewTextWriter\" failed");
		return;
	}

	/* Start Document */
	rc = xmlTextWriterStartDocument(writer, NULL, ENCODING, NULL);
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterStartDocument\" failed");
		goto s_write_utt_exit;
	}

	/* Write the maryxml namespace */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "maryxml");
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterStartElement\" failed");
		goto s_write_utt_exit;
	}

	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST maryNms);
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterWriteAttribute\" failed");
		goto s_write_utt_exit;
	}

	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:xsi", BAD_CAST maryXsi);
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterWriteAttribute\" failed");
		goto s_write_utt_exit;
	}

	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "version", BAD_CAST "0.5");
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterWriteAttribute\" failed");
		goto s_write_utt_exit;
	}

	const SVoice* voice = SUtteranceVoice(utt, error);
	if (S_CHK_ERR(error, S_CONTERR,
		"s_write_utt_maryxml",
		"Call to \"SUtteranceVoice\" failed"))
		goto s_write_utt_exit;

	const char* lang = SVoiceGetLangCode(voice, error);
	if (S_CHK_ERR(error, S_CONTERR,
		"s_write_utt_maryxml",
		"Call to \"SVoiceGetLangCode\" failed"))
		goto s_write_utt_exit;

	if (lang == NULL)
	{
		lang = "en-US";
	}
	rc = xmlTextWriterWriteFormatAttributeNS(writer, BAD_CAST "xml", BAD_CAST "lang", NULL, lang);
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterFormatAttribute\" failed");
		goto s_write_utt_exit;
	}

	/**** begin of relation travelling ****/

	// Write the tag p
	rc = xmlTextWriterStartElement(writer, BAD_CAST "p");
	if (rc < 0)
	{
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterStartElement\" failed");
		goto s_write_utt_exit;
	}

	/* Write the tag s */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "s");
	if (rc < 0)
	{
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterStartElement\" failed");
		goto s_write_utt_exit;
	}

	/* get the first token */
	itrTokens = SRelationHead(SUtteranceGetRelation(utt, "Token", error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				"s_write_utt_maryxml",
				"Call to \"SUtteranceGetRelation\" failed"))
		goto s_write_utt_exit;

	if (isPhraseRelationPresent && itrTokens != NULL)
	{
		currentPhrase = SItemPathToItem(itrTokens, "R:Token.daughter.R:Phrase.parent", error);
		if (S_CHK_ERR(error, S_CONTERR,
					"s_write_utt_maryxml",
					"Call to \"SItemPathToItem\" failed"))
			goto s_write_utt_exit;
	}

	/* iterate through the tokens */
	while (itrTokens != NULL)
	{
		/* get this token's data: name and pos */
		const char * TokenName = SItemGetName(itrTokens, error);
		if (S_CHK_ERR(error, S_CONTERR,
					"s_write_utt_maryxml",
					"Call to \"SItemGetName\" failed"))
			goto s_write_utt_exit;

		isPresent = SItemFeatureIsPresent(itrTokens, "POS", error);
		if (S_CHK_ERR(error, S_CONTERR,
					"s_write_utt_maryxml",
					"Call to \"SItemFeatureIsPresent\" failed"))
			goto s_write_utt_exit;
		if (isPresent)
		{
			pos_string = SItemGetString(itrTokens, "POS", error);
			if (S_CHK_ERR(error, S_CONTERR,
						"s_write_utt_maryxml",
						"Call to \"SItemGetString\" failed"))
				goto s_write_utt_exit;
		}

		const SItem* itrWord = SItemPathToItem(itrTokens, "daughter.R:Word.name", error);
		if (S_CHK_ERR(error, S_CONTERR,
					"s_write_utt_maryxml",
					"Call to \"SItemPathToItem\" failed"))
			goto s_write_utt_exit;

		/* if we have one or more words for this token */
		if (itrWord != NULL)
		{
			/* check if we need an MTU */
			s_bool is_MTU_needed = FALSE;

			const char * word = SItemGetName(itrWord, error);
			if (S_CHK_ERR(error, S_CONTERR,
				"s_write_utt_maryxml",
				"Call to \"SItemPathToFeature\" failed"))
				goto s_write_utt_exit;

			/* if the word is different than the token, we need a MTU */
			is_MTU_needed = (s_strcmp(TokenName, word, error) != 0);

			if ( is_MTU_needed )
			{
				// Open tag mtu
				rc = xmlTextWriterStartElement(writer, BAD_CAST "mtu");
				if (rc < 0)
				{
					S_CTX_ERR(error, S_CONTERR,
						"s_write_utt_maryxml",
						"Call to \"xmlTextWriterStartElement\" failed");
					goto s_write_utt_exit;
				}

				/* Write the attribute orig */
				rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "orig", BAD_CAST TokenName);
				if (rc < 0)
				{
					S_CTX_ERR(error, S_CONTERR,
						"s_write_utt_maryxml",
						"Call to \"xmlTextWriterWriteAttribute\" failed");
					goto s_write_utt_exit;
				}

				/* write all the words that are daughters of the current token */
				while (itrWord != NULL && SItemPathToItem(itrWord, "R:Token.parent", error) == itrTokens )
				{
					write_word(writer, itrWord, pos_string, isSegmentRelationPresent, error);
					if (S_CHK_ERR(error, S_CONTERR,
						"s_write_utt_maryxml",
						"Call to \"write_word\" failed"))
						goto s_write_utt_exit;

					itrWord = SItemNext(itrWord, error);
					if (S_CHK_ERR(error, S_CONTERR,
						"s_write_utt_maryxml",
						"Call to \"SItemNext\" failed"))
						goto s_write_utt_exit;
				}


				/* Close the tag mtu */
				rc = xmlTextWriterEndElement(writer);
				if (rc < 0)
				{
					S_CTX_ERR(error, S_CONTERR,
						  "s_write_utt_maryxml",
						  "Call to \"xmlTextWriterEndElement\" failed");
					goto s_write_utt_exit;
				}

			}
			else
			{
				/* we don't need a MTU, just a simple t */
				write_word(writer, itrWord, pos_string, isSegmentRelationPresent, error);
				if (S_CHK_ERR(error, S_CONTERR,
							"s_write_utt_maryxml",
							"Call to \"write_word\" failed"))
					goto s_write_utt_exit;
			}
		}
		else
		{
			/* fallback in case we have only tokens */
			write_t_tag(writer, TokenName, itrWord, pos_string, NULL, error);
			if (S_CHK_ERR(error, S_CONTERR,
						"s_write_utt_maryxml",
						"Call to \"write_t_tag\" failed"))
				goto s_write_utt_exit;
		}

		/* Go to next token */
		itrTokens = SItemNext(itrTokens, error);
		if (S_CHK_ERR(error, S_CONTERR,
					"s_write_utt_maryxml",
					"Call to \"SItemNext\" failed"))
			goto s_write_utt_exit;

		/* check for phrase change */
		if (isPhraseRelationPresent && itrTokens != NULL)
		{
			const SItem * newTokenPhrase = SItemPathToItem(itrTokens, "R:Token.daughter.R:Phrase.parent", error);
			if (S_CHK_ERR(error, S_CONTERR,
						"s_write_utt_maryxml",
						"Call to \"SItemPathToItem\" failed"))
				goto s_write_utt_exit;

			if (newTokenPhrase != NULL && newTokenPhrase != currentPhrase)
			{
				/* close and reopen s tag */
				rc = xmlTextWriterEndElement(writer);
				if (rc < 0)
				{
					S_CTX_ERR(error, S_CONTERR,
							"s_write_utt_maryxml",
							"Call to \"xmlTextWriterEndDocument\" failed");
					goto s_write_utt_exit;
				}
				rc = xmlTextWriterStartElement(writer, BAD_CAST "s");
				if (rc < 0)
				{
					S_CTX_ERR(error, S_CONTERR,
							"s_write_utt_maryxml",
							"Call to \"xmlTextWriterStartElement\" failed");
					goto s_write_utt_exit;
				}

				currentPhrase = newTokenPhrase;
			}
		}
	}

	/* Close the tag s */
	rc = xmlTextWriterEndElement(writer);
	if (rc < 0)
	{
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
	    "Call to \"xmlTextWriterEndDocument\" failed");
		goto s_write_utt_exit;
	}

	/* Close the tag p */
	rc = xmlTextWriterEndElement(writer);
	if (rc < 0)
	{
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
	    "Call to \"xmlTextWriterEndDocument\" failed");
		goto s_write_utt_exit;
	}

	/**** end of relation travelling ****/

	/* Close the tag maryxml */
	rc = xmlTextWriterEndElement(writer);
	if (rc < 0)
	{
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterEndDocument\" failed");
		goto s_write_utt_exit;
	}
	/* Close the document */
	rc = xmlTextWriterEndDocument(writer);
	if (rc < 0)
	{
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterEndDocument\" failed");
		goto s_write_utt_exit;
	}
s_write_utt_exit:
        xmlFreeTextWriter(writer);
        return;
}
