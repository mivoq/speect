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
/* C convenience functions for SEbmlRead Python wrapper.                            */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

/*
 * Do not delete these delimiters, required for SWIG
 */
%apply unsigned int *INOUT { uint32 *id };


%inline
%{
	SObject *py_sebml_reader_new(const char *path)
	{
		s_erc rv = S_SUCCESS;
		SDatasource *ds;
		SEbmlRead *ebmlReader;


		/*
		 * create a data source, "rb" = read, binary
		 */
		ds = SFilesourceOpenFile(path, "rb", &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to open file");
			return NULL;
		}

		/*
		 * create ebml reader object
		 */
		ebmlReader = (SEbmlRead*)S_NEW("SEbmlRead", &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to create ebml reader");
			S_DELETE(ds, "py_sebml_read_new", &rv);
			return NULL;
		}

		/*
		 * initialize ebml reader object
		 */
		S_EBMLREAD_CALL(ebmlReader, read_init)(&ebmlReader, ds, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to initialize ebml reader");
			S_DELETE(ds, "py_sebml_read_new", &rv);
			return NULL;
		}

		return S_OBJECT(ebmlReader);
	}


	int py_sebml_reader_get_header_version(const SObject *ebmlRead)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);


		return self->header->ebml_version;
	}


	int py_sebml_reader_get_header_reader_version(const SObject *ebmlRead)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);


		return self->header->ebml_read_version;
	}


	int py_sebml_reader_get_header_max_id_width(const SObject *ebmlRead)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);


		return self->header->max_id_width;
	}


	int py_sebml_reader_get_header_max_size_width(const SObject *ebmlRead)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);


		return self->header->max_size_width;
	}


	const char *py_sebml_reader_get_header_doctype(const SObject *ebmlRead)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);


		return (const char*)self->header->doctype;
	}


	int py_sebml_reader_get_header_doctype_version(const SObject *ebmlRead)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);


		return self->header->doctype_version;
	}


	int py_sebml_reader_get_header_doctype_reader_version(const SObject *ebmlRead)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);


		return self->header->doctype_read_version;
	}


	uint32 py_sebml_reader_peek_id(SObject *ebmlRead)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);
		s_erc rv = S_SUCCESS;
		uint32 id;


		id = S_EBMLREAD_CALL(self, peek_id)(self, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to peek id");
			return 0;
		}

		return id;
	}


	void py_sebml_reader_element_skip(SObject *ebmlRead)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);
		s_erc rv = S_SUCCESS;


		S_EBMLREAD_CALL(self, element_skip)(self, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to skip element");
	}


	uint32 py_sebml_reader_read_uint(SObject *ebmlRead, uint32 *id)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);
		s_erc rv = S_SUCCESS;
		uint32 val;


		val = S_EBMLREAD_CALL(self, read_uint)(self, id, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to read uint");
			return 0;
		}

		return val;
	}


	sint32 py_sebml_reader_read_sint(SObject *ebmlRead, uint32 *id)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);
		s_erc rv = S_SUCCESS;
		sint32 val;


		val = S_EBMLREAD_CALL(self, read_sint)(self, id, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to read sint");
			return 0;
		}

		return val;
	}


	double py_sebml_reader_read_double(SObject *ebmlRead, uint32 *id)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);
		s_erc rv = S_SUCCESS;
		double val;


		val = S_EBMLREAD_CALL(self, read_double)(self, id, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to read double");
			return 0;
		}

		return val;
	}


	char *py_sebml_reader_read_utf8(SObject *ebmlRead, uint32 *id)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);
		s_erc rv = S_SUCCESS;
		char *val;


		val = S_EBMLREAD_CALL(self, read_utf8)(self, id, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to read utf8");
			return NULL;
		}

		return val;
	}


	SObject *py_sebml_reader_read_object(SObject *ebmlRead, uint32 *id)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);
		s_erc rv = S_SUCCESS;
		SObject *val;


		val = S_EBMLREAD_CALL(self, read_object)(self, id, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to read object");
			return NULL;
		}

		return val;
	}


	uint32 py_sebml_reader_read_container(SObject *ebmlRead)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);
		s_erc rv = S_SUCCESS;
		uint32 id;


		id = S_EBMLREAD_CALL(self, container)(self, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to read container");
			return 0;
		}

		return id;
	}


	s_bool py_sebml_reader_container_at_end(SObject *ebmlRead)
	{
		SEbmlRead *self = S_EBMLREAD(ebmlRead);
		s_erc rv = S_SUCCESS;
		s_bool query;


		query = S_EBMLREAD_CALL(self, container_at_end)(self, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to query if container is at end");
			return FALSE;
		}

		return query;
	}
/*
 * Do not delete this delimiter, required for SWIG
 */
%}
