/************************************************************************************/
/* Copyright (c) 2015 Mivoq SRL <info@mivoq.it>                                     */
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                Matteo Lisotto                                                    */
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
/* SSerializedFile implementation for MaryXML format utterances.                    */
/* Write utterances to MaryXML formatted files.                                     */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "utt_htslabelsexport.h"
#include <libxml/xmlwriter.h>

#define ENCODING "UTF-8"


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

typedef SSerializedFile SUttHTSLabelsExport;

typedef SSerializedFileClass SUttHTSLabelsExportClass;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SUttHTSLabelsExportClass UttHTSLabelsExportClass; /* SMaryXMLUttFile class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_serialized_htslabels_utt_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&UttHTSLabelsExportClass), error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "_s_serialized_htslabels_utt_reg",
		      "Failed to register SUttHTSLabelsExportClass"))
		return;
	SSerializedFileRegister(&UttHTSLabelsExportClass, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "_s_serialized_htslabels_utt_reg",
		      "Failed to add serialized file class SUttHTSLabelsExportClass"))
	{
		s_erc local_err = S_SUCCESS;
		s_class_free(S_OBJECTCLASS(&UttHTSLabelsExportClass), &local_err);
		return;
	}
}


S_LOCAL void _s_serialized_htslabels_utt_free(s_erc *error)
{
	s_erc local_err;

	S_CLR_ERR(&local_err);
	S_CLR_ERR(error);

	SSerializedFileFree(&UttHTSLabelsExportClass, &local_err);
	S_CHK_ERR(&local_err, S_CONTERR,
		  "_s_serialized_htslabels_utt_free",
		  "Failed to remove serialized file class SUttHTSLabelsExportClass");

	s_class_free(S_OBJECTCLASS(&UttHTSLabelsExportClass), error);
	S_CHK_ERR(error, S_CONTERR,
		  "_s_serialized_htslabels_utt_free",
		  "Failed to free SUttHTSLabelsExportClass");

	if ((local_err != S_SUCCESS)
	    && (error != NULL)
	    && (*error == S_SUCCESS))
		*error = local_err;
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


static SObject *Load(const char *path, s_erc *error)
{
	S_CTX_ERR(error, S_FAILURE,
		  "Load",
		  "Failed to load data from file '%s', load method not implemented",
		  path);

	return NULL;

	S_UNUSED(path);
}

static int _ds_write(void * context, const char * buffer, int len)
{
	int ret = 0;
	s_erc error;
	S_CLR_ERR(&error);
	ret = SDatasourceWrite((SDatasource* ) context, buffer, sizeof(char), len, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
		      "_ds_write",
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
		S_DELETE(ds, "_ds_close", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
		      "_ds_close",
		      "Call to \"xmlOutputClose\" failed"))
		return -1;
	return 0;
}


static void Save(const SObject *object, const char *path, s_erc *error)
{
	int rc;
	SUtterance *utt = S_UTTERANCE(object);
	SDatasource *ds;
	xmlTextWriterPtr writer;
	xmlOutputBufferPtr out;
	const char * xsi = "http://www.w3.org/2001/XMLSchema-instance";


	S_CLR_ERR(error);
	ds = SFilesourceOpenFile(path, "wt", error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Save",
		      "Call to \"SFilesourceOpenFile\" failed"))
		return;

	out = xmlOutputBufferCreateIO(_ds_write,
				      _ds_close,
				      ds,
				      NULL);
	if (out == NULL)
	{
		S_CTX_ERR(error, S_CONTERR,
			  "Save",
			  "Call to \"xmlOutputBufferCreateIO\" failed");
		return;
	}
	writer = xmlNewTextWriter(out);
	if (writer == NULL)
	{
                xmlOutputBufferClose(out);
		S_CTX_ERR(error, S_CONTERR,
			  "Save",
			  "Call to \"xmlNewTextWriter\" failed");
		return;
	}

	/* Start Document */
	rc = xmlTextWriterStartDocument(writer, NULL, ENCODING, NULL);
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "Save",
			  "Call to \"xmlTextWriterStartDocument\" failed");
		goto s_write_utt_exit;
	}

	/* Write the maryxml namespace */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "xml");
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "Save",
			  "Call to \"xmlTextWriterStartElement\" failed");
		goto s_write_utt_exit;
	}

	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:xsi", BAD_CAST xsi);
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "Save",
			  "Call to \"xmlTextWriterWriteAttribute\" failed");
		goto s_write_utt_exit;
	}

	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "version", BAD_CAST "0.5");
	if (rc < 0) {
		S_CTX_ERR(error, S_CONTERR,
			  "Save",
			  "Call to \"xmlTextWriterWriteAttribute\" failed");
		goto s_write_utt_exit;
	}


	// print labels
	/* get to the first syllable of the current word */
	const SItem* itrSegments = SRelationHead(SUtteranceGetRelation(utt, "Segment", error), error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "Save",
		      "Call to \"SItemPathToItem\" failed"))
 		goto s_write_utt_exit;
	while (itrSegments != NULL)
	{
		/* get segment content */
		const char* label = SItemGetString(itrSegments, "hts_label", error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Save",
			      "Call to \"SItemGetName\" failed"))
			goto s_write_utt_exit;

		/* get next segment */
		itrSegments = SItemNext(itrSegments, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "Save",
			      "Call to \"SItemNext\" failed"))
			goto s_write_utt_exit;

		xmlTextWriterWriteElement(writer, BAD_CAST "label", BAD_CAST label);

	}

	/* Close the tag xml */
	rc = xmlTextWriterEndElement(writer);
	if (rc < 0)
	{
		S_CTX_ERR(error, S_CONTERR,
			  "Save",
			  "Call to \"xmlTextWriterEndDocument\" failed");
		goto s_write_utt_exit;
	}
	/* Close the document */
	rc = xmlTextWriterEndDocument(writer);
	if (rc < 0)
	{
		S_CTX_ERR(error, S_CONTERR,
			  "Save",
			  "Call to \"xmlTextWriterEndDocument\" failed");
		goto s_write_utt_exit;
	}
s_write_utt_exit:
        xmlFreeTextWriter(writer);
}


/************************************************************************************/
/*                                                                                  */
/* SUttHTSLabelsExportClass class initialization                                        */
/*                                                                                  */
/************************************************************************************/

static SUttHTSLabelsExportClass UttHTSLabelsExportClass =
{
	/* SObjectClass */
	{
		"SSerializedFile:SUttHTSLabelsExport",
		sizeof(SSerializedFile),
		{ 0, 1},
		NULL,                  /* init    */
		NULL,                  /* destroy */
		Dispose,               /* dispose */
		NULL,                  /* compare */
		NULL,                  /* print   */
		NULL,                  /* copy    */
	},
	/* SSerializedFileClass */
	"spct_utt_htslabelsXML",       /* format  */
	Load,                          /* load    */
	Save                           /* save    */
};
