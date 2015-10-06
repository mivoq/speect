/************************************************************************************/
/* Copyright (c) 2015 Mivoq SRL <info@mivoq.it>                                     */
/*                                                                                  */
/* Contributors:  Matteo Lisotto                                                    */
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
#define ENCODING "UTF-8"

/************************************************************************************/
/*                                                                                  */
/* Relations names                                                                  */
/*                                                                                  */
/************************************************************************************/

static const char * TOKEN_RELATION = "Token";
static const char * PHRASE_RELATION = "Phrase";

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

S_LOCAL void s_write_utt_maryxml(const SUtterance *utt, SDatasource *ds, s_erc *error)
{
	int rc;
	s_bool isPresent;
	s_bool mtuOpen = FALSE;
	xmlTextWriterPtr writer;
	xmlOutputBufferPtr out;
	const SItem * itrPhrases;
	const SItem * itrPhraseWords;
	SItem * next = NULL;
	SItem * tokenWord = NULL;
	SItem * nextTokenWord = NULL;
	const char * maryNms = "http://mary.dfki.de/2002/MaryXML";
	const char * maryXsi = "http://www.w3.org/2001/XMLSchema-instance";

	S_CLR_ERR(error);

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

	rc = xmlTextWriterWriteFormatAttributeNS(writer, BAD_CAST "xml", BAD_CAST "lang", NULL, "it");
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterFormatAttribute\" failed");
		goto s_write_utt_exit;
	}

	/* Check if present at least one phrase */
	isPresent = SUtteranceRelationIsPresent(utt, PHRASE_RELATION, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_write_utt_maryxml",
		      "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto s_write_utt_exit;

	if (isPresent)
	{
		/* Write the tag p */
		rc = xmlTextWriterStartElement(writer, BAD_CAST "p");
		if (rc < 0)
		{
			S_CTX_ERR(error, S_CONTERR,
				  "s_write_utt_maryxml",
				  "Call to \"xmlTextWriterStartElement\" failed");
			goto s_write_utt_exit;
		}

		/* Get the first phrase */
		itrPhrases = SRelationHead(SUtteranceGetRelation(utt, PHRASE_RELATION, error), error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "s_write_utt_maryxml",
			      "Call to \"SUtteranceGetRelation\" failed"))
			goto s_write_utt_exit;

		while (itrPhrases != NULL)
		{
			/* Write the tag s */
			rc = xmlTextWriterStartElement(writer, BAD_CAST "s");
			if (rc < 0)
			{
				S_CTX_ERR(error, S_CONTERR,
					  "s_write_utt_maryxml",
					  "Call to \"xmlTextWriterStartElement\" failed");
				goto s_write_utt_exit;
			}

			/* Get the first child from the current phrase */
			itrPhraseWords = SItemDaughter(itrPhrases, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "s_write_utt_maryxml",
				      "Call to \"SItemDaughter\" failed"))
				goto s_write_utt_exit;

			while (itrPhraseWords != NULL)
			{
				/* Get the next item */
				next = SItemNext(itrPhraseWords, error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "s_write_utt_maryxml",
					      "Call to \"SItemDaughter\" failed"))
					goto s_write_utt_exit;

				/* Get the Token from Word */
				tokenWord = SItemAs(itrPhraseWords, TOKEN_RELATION, error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "s_write_utt_maryxml",
					      "Call to \"SItemAs\" failed"))
					goto s_write_utt_exit;

				tokenWord = SItemParent(tokenWord, error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "s_write_utt_maryxml",
					      "Call to \"SItemParent\" failed"))
					goto s_write_utt_exit;

				/* Check if the token has pre-punctuation */
				isPresent = SItemFeatureIsPresent(tokenWord, "prepunc", error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "s_write_utt_maryxml",
					      "Call to \"SItemFeatureIsPresent\" failed"))
					goto s_write_utt_exit;

				if (isPresent)
				{
					const char * prepunc = SItemGetString(tokenWord,"prepunc", error);
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

				if (next != NULL)
				{
					/* Get the Token from next */
					nextTokenWord = SItemAs(next, TOKEN_RELATION, error);
					if (S_CHK_ERR(error, S_CONTERR,
						      "s_write_utt_maryxml",
						      "Call to \"SItemAs\" failed"))
						goto s_write_utt_exit;

					nextTokenWord = SItemParent(nextTokenWord, error);
					if (S_CHK_ERR(error, S_CONTERR,
						      "s_write_utt_maryxml",
						      "Call to \"SItemParent\" failed"))
						goto s_write_utt_exit;

					if ((tokenWord == nextTokenWord) && !mtuOpen)
					{
						/* Open tag mtu */
						mtuOpen = TRUE;
						rc = xmlTextWriterStartElement(writer, BAD_CAST "mtu");
						if (rc < 0)
						{
							S_CTX_ERR(error, S_CONTERR,
								  "s_write_utt_maryxml",
								  "Call to \"xmlTextWriterStartElement\" failed");
							goto s_write_utt_exit;
						}

						/* Write the attribute orig */
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "orig", BAD_CAST SItemGetName(tokenWord, error));
						if (rc < 0)
						{
							S_CTX_ERR(error, S_CONTERR,
								  "s_write_utt_maryxml",
								  "Call to \"xmlTextWriterWriteAttribute\" failed");
							goto s_write_utt_exit;
						}
					}
				}

				/* Check if the multiToken is composed to only one word */
				if (s_strcmp(SItemGetName(tokenWord, error), SItemGetName(itrPhraseWords, error), error) != 0)
				{
					/* Open tag mtu */
					mtuOpen = TRUE;
					rc = xmlTextWriterStartElement(writer, BAD_CAST "mtu");
					if (rc < 0)
					{
						S_CTX_ERR(error, S_CONTERR,
							  "s_write_utt_maryxml",
							  "Call to \"xmlTextWriterStartElement\" failed");
						goto s_write_utt_exit;
					}

					/* Write the attribute orig */
					rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "orig", BAD_CAST SItemGetName(tokenWord, error));
					if (rc < 0)
					{
						S_CTX_ERR(error, S_CONTERR,
							  "s_write_utt_maryxml",
							  "Call to \"xmlTextWriterWriteAttribute\" failed");
						goto s_write_utt_exit;
					}
				}

				/* Write the tag t */
				rc = xmlTextWriterWriteElement(writer, BAD_CAST "t", BAD_CAST SItemGetName(itrPhraseWords, error));
				if (rc < 0)
				{
					S_CTX_ERR(error, S_CONTERR,
						  "s_write_utt_maryxml",
						  "Call to \"xmlTextWriterWriteElement\" failed");
					goto s_write_utt_exit;
				}

				if ((((next != NULL) && (tokenWord != nextTokenWord)) || next == NULL) && mtuOpen)
				{
					mtuOpen = FALSE;
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

				/* Check if the token has post-punctuation */
				isPresent = SItemFeatureIsPresent(tokenWord, "postpunc", error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "s_write_utt_maryxml",
					      "Call to \"SItemFeatureIsPresent\" failed"))
					goto s_write_utt_exit;

				if (isPresent)
				{
					const char * prepunc = SItemGetString(tokenWord,"postpunc", error);
					if (S_CHK_ERR(error, S_CONTERR,
						      "s_write_utt_maryxml",
						      "Call to \"SItemGetString\" failed"))
						goto s_write_utt_exit;

					/* Write the tag t with post-punctuation */
					rc = xmlTextWriterWriteElement(writer, BAD_CAST "t", BAD_CAST prepunc);
					if (rc < 0)
					{
						S_CTX_ERR(error, S_CONTERR,
							  "s_write_utt_maryxml",
							  "Call to \"xmlTextWriterWriteElement\" failed");
						goto s_write_utt_exit;
					}
				}

				itrPhraseWords = SItemNext(itrPhraseWords, error);
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

			itrPhrases = SItemNext(itrPhrases, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "s_write_utt_maryxml",
				      "Call to \"SItemNext\" failed"))
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
		/* Close the tag maryxml */
		rc = xmlTextWriterEndElement(writer);
		if (rc < 0)
		{
			S_CTX_ERR(error, S_CONTERR,
				  "s_write_utt_maryxml",
				  "Call to \"xmlTextWriterEndDocument\" failed");
			goto s_write_utt_exit;
		}
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
