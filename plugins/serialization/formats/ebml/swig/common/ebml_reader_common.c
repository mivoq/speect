/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
/* SWIG common C convenience functions for SEbmlRead.                               */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Extend the SEbmlRead class                                                       */
/*                                                                                  */
/************************************************************************************/

typedef struct
{
	%extend
	{
		const int ebml_version;
		const int reader_version;
		const int max_id_width;
		const int max_size_width;
		const int doc_type_version;
		const int doc_type_reader_version;
	}
} SEbmlRead;


%types(SEbmlRead = SObject);


%extend SEbmlRead
{
	SEbmlRead(const char *path, s_erc *error)
	{
		SDatasource *ds;
		SEbmlRead *ebmlReader;


		S_CLR_ERR(error);

		/*
		 * create a data source, "rb" = read, binary
		 */
		ds = SFilesourceOpenFile(path, "rb", error);
		if (*error != S_SUCCESS)
			return NULL;

		/*
		 * create ebml reader object
		 */
		ebmlReader = (SEbmlRead*)S_NEW("SEbmlRead", error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(ds, "SEbmlRead()", error);
			return NULL;
		}

		/*
		 * initialize ebml reader object
		 */
		S_EBMLREAD_CALL(ebmlReader, read_init)(&ebmlReader, ds, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(ds, "SEbmlRead()", error);
			return NULL;
		}

		return ebmlReader;
	}


	~SEbmlRead()
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE($self, "~SEbmlRead()", &error);
	}


	const char *doc_type(void)
	{
		return (const char*)$self->header->doctype;
	}


	uint32 peek_id(s_erc *error)
	{
		uint32 id;


		S_CLR_ERR(error);
		id = S_EBMLREAD_CALL($self, peek_id)($self, error);
		if (*error != S_SUCCESS)
			return 0;

		return id;
	}


	void skip_element(s_erc *error)
	{
		S_CLR_ERR(error);
		S_EBMLREAD_CALL($self, element_skip)($self, error);
	}


	uint32 open_container(s_erc *error)
	{
		uint32 id;


		S_CLR_ERR(error);
		id = S_EBMLREAD_CALL($self, container)($self, error);
		if (*error != S_SUCCESS)
			return 0;

		return id;
	}


	s_bool container_exhausted(s_erc *error)
	{
		s_bool query;


		S_CLR_ERR(error);
		query = S_EBMLREAD_CALL($self, container_at_end)($self, error);
		if (*error != S_SUCCESS)
			return FALSE;

		return query;
	}
};


%{
	const int SEbmlRead_ebml_version_get(SEbmlRead *self)
	{
		return (int)self->header->ebml_version;
	}

	const int SEbmlRead_reader_version_get(SEbmlRead *self)
	{
		return (int)self->header->ebml_read_version;
	}

	const int SEbmlRead_max_id_width_get(SEbmlRead *self)
	{
		return (int)self->header->max_id_width;
	}

	const int SEbmlRead_max_size_width_get(SEbmlRead *self)
	{
		return (int)self->header->max_size_width;
	}

	const int SEbmlRead_doc_type_version_get(SEbmlRead *self)
	{
		return (int)self->header->doctype_version;
	}

	const int SEbmlRead_doc_type_reader_version_get(SEbmlRead *self)
	{
		return (int)self->header->doctype_read_version;
	}
%}
