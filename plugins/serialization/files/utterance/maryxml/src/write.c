/************************************************************************************/
/* Copyright (c) 2015 Mivoq SRL <info@mivoq.it>                                     */
/*                                                                                  */
/* Contributors:  Matteo Lisotto                                                    */
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
/* Relations names                                                                  */
/*                                                                                  */
/************************************************************************************/

static const char * TOKEN_RELATION = "Token";
static const char * PHRASE_RELATION = "Phrase";
static const char * WORD_RELATION = "Word";
static const char * SYLLABLE_RELATION = "Syllable";
static const char * SEGMENT_RELATION = "Segment";
static const char * SYLSTRUCT_RELATION = "SylStructure";

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
		S_DELETE(ds, "s_write_utt_textgrid", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
		      "s_write_utt_maryxml",
		      "Call to \"xmlOutputClose\" failed"))
		return -1;
	return 0;
}

static void write_word(xmlTextWriterPtr writer, SItem* wordSItem, s_bool ShouldWriteSegments, s_erc *error)
{
	int rc;

	S_CLR_ERR(error);
	s_buffer *buffer = s_buffer_new(error);

	if (!ShouldWriteSegments)
	{
		/* since we don't have segments, it's a plain t */
		rc = xmlTextWriterWriteElement(writer, BAD_CAST "t", BAD_CAST SItemGetName(wordSItem, error));
		if (rc < 0)
		{
			S_CTX_ERR(error, S_CONTERR,
				"s_write_utt_maryxml",
				"Call to \"xmlTextWriterWriteElement\" failed");
			goto cleanup;
		}
	}
	else
	{
		/* create segments string */
		if (S_CHK_ERR(error, S_CONTERR,
			"write_word",
			"Call to \"s_buffer_new\" failed"))
			goto cleanup;

		/* get to the first syllable of the current word */
		SItem* itrSyllables = SItemPathToItem(wordSItem, "R:SylStructure.daughter",error);
		if (S_CHK_ERR(error, S_CONTERR,
			"write_word",
			"Call to \"SItemPathToItem\" failed"))
			goto cleanup;
		/* for every syllable */
		while (itrSyllables != NULL)
		{
			/* get the first segment */
			SItem* itrSegments = SItemDaughter(itrSyllables, error);
			if (S_CHK_ERR(error, S_CONTERR,
				"write_word",
				"Call to \"SItemDaughter\" failed"))
				goto cleanup;
			while (itrSegments != NULL)
			{
				/* get segment content */
				char* segmentName = SItemGetName(itrSegments, error);
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

				/* append the segment separator */
				s_buffer_append(buffer, " ", 1, error);
				if (S_CHK_ERR(error, S_CONTERR,
					"write_word",
					"Call to \"s_buffer_append\" failed"))
					goto cleanup;
			}

			/* get next syllable */
			itrSyllables = SItemNext(itrSyllables, error);
			if (S_CHK_ERR(error, S_CONTERR,
				"write_word",
				"Call to \"SItemNext\" failed"))
				goto cleanup;

			/* append the syllable separator */
			s_buffer_append(buffer, " - ", 3, error);
			if (S_CHK_ERR(error, S_CONTERR,
				"write_word",
				"Call to \"s_buffer_append\" failed"))
				goto cleanup;
		}

		char* word = SItemGetName(wordSItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
			"write_word",
			"Call to \"SItemGetName\" failed"))
			goto cleanup;

		if (word != NULL && s_strcmp(word, "", error) != 0)
		{
			/* open the t tag */
			rc = xmlTextWriterStartElement(writer, BAD_CAST "t");
			if (rc < 0)
			{
				S_CTX_ERR(error, S_CONTERR,
					"write_word",
					"Call to \"xmlTextWriterStartElement\" failed");
				goto cleanup;
			}

			size_t buff_size = s_buffer_size(buffer, error);
			if (buff_size != 0)
			{
				/* write segments */
				rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "ph", BAD_CAST (const char*)s_buffer_data(buffer, error));
				if (rc < 0)
				{
					S_CTX_ERR(error, S_CONTERR,
						"write_word",
						"Call to \"xmlTextWriterWriteAttribute\" failed");
					goto cleanup;
				}
			}


			/* write the word */
			rc = xmlTextWriterWriteString(writer, BAD_CAST word);
			if (rc < 0)
			{
				S_CTX_ERR(error, S_CONTERR,
					"write_word",
					"Call to \"xmlTextWriterWriteString\" failed");
				goto cleanup;
			}

			/* close t tag */
			rc = xmlTextWriterEndElement(writer);
			if (rc < 0)
			{
				S_CTX_ERR(error, S_CONTERR,
					"write_word",
					"Call to \"xmlTextWriterEndDocument\" failed");
				goto cleanup;
			}
		}
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
	const SItem * itrTokens;
	const char * maryNms = "http://mary.dfki.de/2002/MaryXML";
	const char * maryXsi = "http://www.w3.org/2001/XMLSchema-instance";

	s_bool isTokenRelationPresent;
	s_bool isWordRelationPresent;
	s_bool isPhraseRelationPresent;
	s_bool isSegmentRelationPresent;

	S_CLR_ERR(error);

	/* init relation bools */
	isTokenRelationPresent = SUtteranceRelationIsPresent(utt, TOKEN_RELATION, error);
	if(S_CHK_ERR(error, S_CONTERR,
		      "s_write_utt_maryxml",
		      "Call to \"SUtteranceRelationIsPresent\" failed"))
		return;

	/* check that we have a token relation graph. If not, we have nothing to do, so just return */
	if(!isTokenRelationPresent)
		return;

	isWordRelationPresent = SUtteranceRelationIsPresent(utt, WORD_RELATION, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_write_utt_maryxml",
		      "Call to \"SUtteranceRelationIsPresent\" failed"))
		return;

	isPhraseRelationPresent = SUtteranceRelationIsPresent(utt, PHRASE_RELATION, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_write_utt_maryxml",
		      "Call to \"SUtteranceRelationIsPresent\" failed"))
		return;

	isSegmentRelationPresent = SUtteranceRelationIsPresent(utt, SEGMENT_RELATION, error);
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

	/* take the language if present, else default to eng-US */
	isPresent = SUtteranceFeatureIsPresent(utt, "lang-code", error);
	if (S_CHK_ERR(error, S_CONTERR,
		"s_write_utt_maryxml",
		"Call to \"SUtteranceFeatureIsPresent\" failed"))
		goto s_write_utt_exit;;

	char* lang = "en-US";

	if (isPresent)
	{
		lang = SObjectGetString(SUtteranceGetFeature(utt, "lang-code", error), error);
		if (S_CHK_ERR(error, S_CONTERR,
			"s_write_utt_maryxml",
			"Call to \"SUtteranceGetFeature\" failed"))
			goto s_write_utt_exit;;
	}
	rc = xmlTextWriterWriteFormatAttributeNS(writer, BAD_CAST "xml", BAD_CAST "lang", NULL, BAD_CAST lang);
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

	/* iterate throught the tokens */
	itrTokens = SRelationHead(SUtteranceGetRelation(utt, TOKEN_RELATION, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
		"s_write_utt_maryxml",
		"Call to \"SUtteranceGetRelation\" failed"))
		goto s_write_utt_exit;

	while (itrTokens != NULL)
	{
		/* Check if the token has pre-punctuation */
		isPresent = SItemFeatureIsPresent(itrTokens, "prepunc", error);
		if (S_CHK_ERR(error, S_CONTERR,
			"s_write_utt_maryxml",
			"Call to \"SItemFeatureIsPresent\" failed"))
			goto s_write_utt_exit;

		if (isPresent)
		{
			const char * prepunc = SItemGetString(itrTokens,"prepunc", error);
			if (S_CHK_ERR(error, S_CONTERR,
				"s_write_utt_maryxml",
				"Call to \"SItemGetString\" failed"))
				goto s_write_utt_exit;

			/* Write the tag t with pre-punctuation */
			rc = xmlTextWriterWriteElement(writer, BAD_CAST "t", BAD_CAST prepunc);
			if (rc < 0)
			{
				S_CTX_ERR(error, S_CONTERR,
						"s_write_utt_maryxml",
						"Call to \"xmlTextWriterWriteElement\" failed");
				goto s_write_utt_exit;
			}
		}

		/* write down this token's data */

		const char * TokenName = SItemGetName(itrTokens, error);
		if (S_CHK_ERR(error, S_CONTERR,
			"s_write_utt_maryxml",
			"Call to \"SItemGetName\" failed"))
			goto s_write_utt_exit;

		/* if we have the Word relation graph */
		if ( isWordRelationPresent )
		{
			/* get Word representation */
			char pathTarget[255];
			sprintf( pathTarget, "daughter.R:%s.name", WORD_RELATION );

			SItem* itrWord = SItemPathToItem(itrTokens, pathTarget,error);
			const char * word = SItemGetName(itrWord, error);
			if (S_CHK_ERR(error, S_CONTERR,
				"s_write_utt_maryxml",
				"Call to \"SItemPathToFeature\" failed"))
				goto s_write_utt_exit;

			/* if the word is different than the token, we need a MTU */
			if ( s_strcmp(TokenName, word, error) != 0)
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

				sprintf( pathTarget, "R:%s.parent", TOKEN_RELATION );
				/* write all the words that are daughters of the current token */
				while (itrWord != NULL && SItemPathToItem(itrWord, pathTarget,error) == itrTokens )
				{
					write_word(writer, itrWord, isSegmentRelationPresent, error);
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
				write_word(writer, itrWord, isSegmentRelationPresent, error);
				if (S_CHK_ERR(error, S_CONTERR,
					"s_write_utt_maryxml",
					"Call to \"write_word\" failed"))
					goto s_write_utt_exit;
			}
		}
		else
		{
			/* fallback in case we have only tokens */
			rc = xmlTextWriterWriteElement(writer, BAD_CAST "t", BAD_CAST TokenName);
			if (rc < 0)
			{
				S_CTX_ERR(error, S_CONTERR,
					"s_write_utt_maryxml",
					"Call to \"xmlTextWriterWriteElement\" failed");
				goto s_write_utt_exit;
			}
		}

		/* Check if the token has post-punctuation */
		isPresent = SItemFeatureIsPresent(itrTokens, "postpunc", error);
		if (S_CHK_ERR(error, S_CONTERR,
			"s_write_utt_maryxml",
			"Call to \"SItemFeatureIsPresent\" failed"))
			goto s_write_utt_exit;

		if (isPresent)
		{
			const char * postpunc = SItemGetString(itrTokens,"postpunc", error);
			if (S_CHK_ERR(error, S_CONTERR,
				"s_write_utt_maryxml",
				"Call to \"SItemGetString\" failed"))
				goto s_write_utt_exit;

			/* Write the tag t with post-punctuation */
			rc = xmlTextWriterWriteElement(writer, BAD_CAST "t", BAD_CAST postpunc);
			if (rc < 0)
			{
				S_CTX_ERR(error, S_CONTERR,
						"s_write_utt_maryxml",
						"Call to \"xmlTextWriterWriteElement\" failed");
				goto s_write_utt_exit;
			}
		}

		/* Go to next token */
		itrTokens = SItemNext(itrTokens, error);
		if (S_CHK_ERR(error, S_CONTERR,
			"s_write_utt_maryxml",
			"Call to \"SItemNext\" failed"))
			goto s_write_utt_exit;
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
