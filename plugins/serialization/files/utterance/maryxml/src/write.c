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
	s_bool mtuOpen;
	xmlTextWriterPtr writer;
	xmlOutputBufferPtr out;
	const SItem * itrPhrases;
	const SItem * itrPhraseWords;
	const SItem * itrWords;
	SItem * next = NULL;
	SItem * currentTokenAsWord = NULL;
	SItem * nextTokenAsWord = NULL;
	const char * maryNms = "http://mary.dfki.de/2002/MaryXML";
	const char * maryXsi = "http://www.w3.org/2001/XMLSchema-instance";
	
	S_CLR_ERR(error);
	out = xmlOutputBufferCreateIO(_ds_write,
				      _ds_close,
				      ds,
				      NULL);
	writer = xmlNewTextWriter(out);
	if (writer == NULL)
	{
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
		return;
	}

	/* Write the maryxml namespace */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "maryxml");
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterStartElement\" failed");
		return;
	}

	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST maryNms);
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterWriteAttribute\" failed");
		return;
	}

	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:xsi", BAD_CAST maryXsi);
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterWriteAttribute\" failed");
		return;
	}

	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "version", BAD_CAST "0.5");
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterWriteAttribute\" failed");
		return;
	}

	rc = xmlTextWriterWriteFormatAttributeNS(writer, BAD_CAST "xml", BAD_CAST "lang", NULL, "it");
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt_maryxml",
			  "Call to \"xmlTextWriterFormatAttribute\" failed");
		return;
	}

	/* Check if present at least one phrase */
	isPresent = SUtteranceRelationIsPresent(utt, "Phrase", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_write_utt_maryxml",
		      "Call to \"SUtteranceRelationIsPresent\" failed"))
		return;

	if (isPresent)
	{
		/* Write the tag p */
		rc = xmlTextWriterStartElement(writer, BAD_CAST "p");
		if (rc < 0)
		{
			S_CTX_ERR(error, S_CONTERR,
				  "s_write_utt:maryxml",
				  "Call to \"xmlTextWriterStartElement\" failed");
			return;
		}

		/* Get the first phrase */
		itrPhrases = (SItem*) SRelationHead(SUtteranceGetRelation(utt, "Phrase", error), error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "s_write_utt_maryxml",
			      "Call to \"SUtteranceGetRelation\" failed"))
			return;

		/* Get the first word */
		itrWords = (SItem*) SRelationHead(SUtteranceGetRelation(utt, "Word", error), error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "s_write_utt_maryxml",
			      "Call to \"SUtteranceGetRelation\" failed"))
			return;

		mtuOpen = FALSE;
		while (itrPhrases != NULL)
		{
			/* Write the tag s */
			rc = xmlTextWriterStartElement(writer, BAD_CAST "s");
			if (rc < 0)
			{
				S_CTX_ERR(error, S_CONTERR,
					  "s_write_utt:maryxml",
					  "Call to \"xmlTextWriterStartElement\" failed");
				return;
			}
			/* Get the first child from the current phrase */
			itrPhraseWords = SItemDaughter(itrPhrases, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "s_write_utt_maryxml",
				      "Call to \"SItemDaughter\" failed"))
				return;

			while (itrPhraseWords != NULL)
			{
				/* Get the next item */
				next = SItemNext(itrPhraseWords, error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "s_write_utt_maryxml",
					      "Call to \"SItemDaughter\" failed"))
					return;

				/* Get the Token of the current word */
				currentTokenAsWord = SItemAs(itrWords, "Token", error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "s_write_utt_maryxml",
					      "Call to \"SItemDaughter\" failed"))
					return;

				/* Write pre-punctuation */
				if (SItemFeatureIsPresent(currentTokenAsWord, "prepunc", error))
				{
					const char * prepunc = SItemGetString(currentTokenAsWord,"prepunc", error);

					/* Write the tag pre-punctuation */
					rc = xmlTextWriterWriteElement(writer, BAD_CAST "t", BAD_CAST prepunc);
					if (rc < 0)
					{
						S_CTX_ERR(error, S_CONTERR,
							  "s_write_utt:maryxml",
							  "Call to \"xmlTextWriterWriteElement\" failed");
						return;
					}
				}

				if (next != NULL)
				{
					/* Get the Token of the next word */
					nextTokenAsWord = SItemAs(SItemNext(itrWords, error), "Token", error);
					if (S_CHK_ERR(error, S_CONTERR,
						      "s_write_utt_maryxml",
						      "Call to \"SItemAs\" failed"))
						return;

					if ((currentTokenAsWord == nextTokenAsWord) && !mtuOpen)
					{
						/* Open tag mtu */
						mtuOpen = TRUE;
						rc = xmlTextWriterStartElement(writer, BAD_CAST "mtu");
						if (rc < 0)
						{
							S_CTX_ERR(error, S_CONTERR,
								  "s_write_utt:maryxml",
								  "Call to \"xmlTextWriterWriteElement\" failed");
							return;
						}

						/* Write the attribute orig */
						rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "orig", BAD_CAST SItemGetName(currentTokenAsWord, error));
						if (rc < 0)
						{
							S_CTX_ERR(error, S_CONTERR,
								  "s_write_utt:maryxml",
								  "Call to \"xmlTextWriterWriteElement\" failed");
							return;
						}
					}
				}

				/* Write the tag t */
				rc = xmlTextWriterWriteElement(writer, BAD_CAST "t", BAD_CAST SItemGetName(itrPhraseWords, error));
				if (rc < 0)
				{
					S_CTX_ERR(error, S_CONTERR,
						  "s_write_utt:maryxml",
						  "Call to \"xmlTextWriterWriteElement\" failed");
					return;
				}

				if ((currentTokenAsWord != nextTokenAsWord) && mtuOpen)
				{
					mtuOpen = FALSE;

					/* Close the tag mtu */
					rc = xmlTextWriterEndElement(writer);
					if (rc < 0)
					{
						S_CTX_ERR(error, S_CONTERR,
							  "s_write_utt:maryxml",
							  "Call to \"xmlTextWriterEndDocument\" failed");
						return;
					}
				}

				/* Write post-punctuation */
				if (SItemFeatureIsPresent(currentTokenAsWord, "postpunc", error))
				{
					const char * postpunc = SItemGetString(currentTokenAsWord, "Postpunc", error);

					/* Write the tag pre-punctuation */
					rc = xmlTextWriterWriteElement(writer, BAD_CAST "t", BAD_CAST postpunc);
					if (rc < 0)
					{
						S_CTX_ERR(error, S_CONTERR,
							  "s_write_utt:maryxml",
							  "Call to \"xmlTextWriterWriteElement\" failed");
						return;
					}
				}

				itrPhraseWords = SItemNext(itrPhraseWords, error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "s_write_utt_maryxml",
					      "Call to \"SItemNext\" failed"))
					return;

				itrWords = SItemNext(itrWords, error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "s_write_utt_maryxml",
					      "Call to \"SItemNext\" failed"))
					return;
			}
			/* Close the tag s */
			rc = xmlTextWriterEndElement(writer);
			if (rc < 0)
			{
				S_CTX_ERR(error, S_CONTERR,
					  "s_write_utt:maryxml",
		  			  "Call to \"xmlTextWriterEndDocument\" failed");
				return;
			}
			itrPhrases = SItemNext(itrPhrases, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "s_write_utt_maryxml",
				      "Call to \"SItemNext\" failed"))
				return;
		}

		/* Close the tag p */
		rc = xmlTextWriterEndElement(writer);
		if (rc < 0)
		{
			S_CTX_ERR(error, S_CONTERR,
				  "s_write_utt:maryxml",
	  			  "Call to \"xmlTextWriterEndDocument\" failed");
			return;
		}
		/* Close the tag maryxml */
		rc = xmlTextWriterEndElement(writer);
		if (rc < 0)
		{
			S_CTX_ERR(error, S_CONTERR,
				  "s_write_utt:maryxml",
				  "Call to \"xmlTextWriterEndDocument\" failed");
			return;
		}
	}
	rc = xmlTextWriterEndDocument(writer);
	if (rc < 0)
	{
		S_CTX_ERR(error, S_CONTERR,
			  "s_write_utt:maryxml",
			  "Call to \"xmlTextWriterEndDocument\" failed");
		return;
	}	
}
