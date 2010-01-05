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
/* AUTHOR  : Richard Carlson, Aby Louw                                              */
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SEbmlWrite Python wrapper.                           */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	SObject *py_sebml_writer_new(const char *path,
								 int ebml_version,
								 int ebml_read_version,
								 int max_id_width,
								 int max_size_width,
								 const char *doc_type,
								 int doc_type_version,
								 int doc_type_read_version)
	{
		s_erc rv = S_SUCCESS;
		SDatasource *ds;
		SEbmlWrite *ebmlWriter;
		s_ebml_header *header;

		/*
		 * create a data source, "wb" = write, binary
		 */
		ds = SFilesourceOpenFile(path, "wb", &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to open file");
			return NULL;
		}

		/*
		 * create ebml header
		 */
		header = S_MALLOC(s_ebml_header, 1);
		if (header == NULL)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to create header structure");
			S_DELETE(ds, "py_sebml_writer_new", &rv);
			return NULL;
		}

		header->ebml_version = (uint8)ebml_version;
		header->ebml_read_version = (uint8)ebml_read_version;
		header->max_id_width = (uint8)max_id_width;
		header->max_size_width = (uint8)max_size_width;
		header->doctype_version = (uint8)doc_type_version;
		header->doctype_read_version = (uint8)doc_type_read_version;
		header->doctype = s_strdup(doc_type, &rv);

		if (S_CHK_ERR(&rv, S_CONTERR,
					  "py_sebml_writer_new",
					  "Call to \"s_strdup\" failed"))
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to copy header document type");
			S_DELETE(ds, "py_sebml_writer_new", &rv);
			S_FREE(header);
			return NULL;
		}

		/*
		 * create ebml reader object
		 */
		ebmlWriter = (SEbmlWrite*)S_NEW("SEbmlWrite", &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to create ebml writer");
			S_DELETE(ds, "py_sebml_read_new", &rv);
			S_FREE(header);
			return NULL;
		}

		/*
		 * initialize ebml writer object
		 */
		S_EBMLWRITE_CALL(ebmlWriter, write_init)(&ebmlWriter, ds, header, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to initialize ebml writer");
			S_DELETE(ds, "py_sebml_read_new", &rv);
			S_FREE(header);
			return NULL;
		}

		return S_OBJECT(ebmlWriter);
	}


	void py_sebml_writer_write_uint(SObject *ebmlWrite, uint32 id, uint32 val)
	{
		SEbmlWrite *self = S_EBMLWRITE(ebmlWrite);
		s_erc rv = S_SUCCESS;


		S_EBMLWRITE_CALL(self, write_uint)(self, id, val, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to write uint");
	}


	void py_sebml_writer_write_sint(SObject *ebmlWrite, uint32 id, sint32 val)
	{
		SEbmlWrite *self = S_EBMLWRITE(ebmlWrite);
		s_erc rv = S_SUCCESS;


		S_EBMLWRITE_CALL(self, write_sint)(self, id, val, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to write sint");
	}


	void py_sebml_writer_write_double(SObject *ebmlWrite, uint32 id, double val)
	{
		SEbmlWrite *self = S_EBMLWRITE(ebmlWrite);
		s_erc rv = S_SUCCESS;


		S_EBMLWRITE_CALL(self, write_double)(self, id, val, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to write double");
	}


	void py_sebml_writer_write_utf8(SObject *ebmlWrite, uint32 id, const char *val)
	{
		SEbmlWrite *self = S_EBMLWRITE(ebmlWrite);
		s_erc rv = S_SUCCESS;


		S_EBMLWRITE_CALL(self, write_utf8)(self, id, val, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to write utf8");
	}


	void py_sebml_writer_write_object(SObject *ebmlWrite, uint32 id, const SObject *val)
	{
		SEbmlWrite *self = S_EBMLWRITE(ebmlWrite);
		s_erc rv = S_SUCCESS;


		S_EBMLWRITE_CALL(self, write_object)(self, id, val, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to write object");
	}


	void py_sebml_writer_start_container(SObject *ebmlWrite, uint32 id)
	{
		SEbmlWrite *self = S_EBMLWRITE(ebmlWrite);
		s_erc rv = S_SUCCESS;


		S_EBMLWRITE_CALL(self, start_container)(self, id, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to start container");
	}


	void py_sebml_writer_stop_container(SObject *ebmlWrite)
	{
		SEbmlWrite *self = S_EBMLWRITE(ebmlWrite);
		s_erc rv = S_SUCCESS;


		S_EBMLWRITE_CALL(self, stop_container)(self, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to stop container");
	}
/*
 * Do not delete this delimiter, required for SWIG
 */
%}
