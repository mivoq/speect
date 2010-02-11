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

typedef struct
{
} SEbmlWrite;


%types(SEbmlWrite = SObject);

%extend SEbmlWrite
{
	SEbmlWrite(const char *path, int ebml_version,
			   int ebml_read_version, int max_id_width,
			   int max_size_width, const char *doc_type,
			   int doc_type_version, int doc_type_read_version,
			   s_erc *error)
	{
		SDatasource *ds;
		SEbmlWrite *ebmlWriter;
		s_ebml_header *header;

		/*
		 * create a data source, "wb" = write, binary
		 */
		ds = SFilesourceOpenFile(path, "wb", error);
		if (*error != S_SUCCESS)
			return NULL;

		/*
		 * create ebml header
		 */
		header = S_MALLOC(s_ebml_header, 1);
		if (header == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "SEbmlWrite()",
					  "Failed to allocate memory for 's_ebml_header' object");
			S_DELETE(ds, "SEbmlWrite()", error);
			return NULL;
		}

		header->ebml_version = (uint8)ebml_version;
		header->ebml_read_version = (uint8)ebml_read_version;
		header->max_id_width = (uint8)max_id_width;
		header->max_size_width = (uint8)max_size_width;
		header->doctype_version = (uint8)doc_type_version;
		header->doctype_read_version = (uint8)doc_type_read_version;
		header->doctype = s_strdup(doc_type, error);

		if (*error != S_SUCCESS)
		{
			S_DELETE(ds, "SEbmlWrite()", error);
			S_FREE(header);
			return NULL;
		}

		/*
		 * create ebml reader object
		 */
		ebmlWriter = (SEbmlWrite*)S_NEW("SEbmlWrite", error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(ds, "SEbmlWrite()", error);
			S_FREE(header);
			return NULL;
		}

		/*
		 * initialize ebml writer object
		 */
		S_EBMLWRITE_CALL(ebmlWriter, write_init)(&ebmlWriter, ds, header, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(ds, "SEbmlWrite()", error);
			S_FREE(header);
			return NULL;
		}

		return ebmlWriter;
	}


	void write_uint(uint32 id, uint32 val, s_erc *error)
	{
		S_EBMLWRITE_CALL($self, write_uint)($self, id, val, error);
	}


	void write_sint(uint32 id, sint32 val, s_erc *error)
	{
		S_EBMLWRITE_CALL($self, write_sint)($self, id, val, error);
	}


	void write_double(uint32 id, double val, s_erc *error)
	{
		S_EBMLWRITE_CALL($self, write_double)($self, id, val, error);
	}


	void write_str(uint32 id, const char *val, s_erc *error)
	{
		S_EBMLWRITE_CALL($self, write_utf8)($self, id, val, error);
	}


	void write_object(uint32 id, const SObject *val, s_erc *error)
	{
		S_EBMLWRITE_CALL($self, write_object)($self, id, val, error);
	}


	void start_container(uint32 id, s_erc *error)
	{
		S_EBMLWRITE_CALL($self, start_container)($self, id, error);
	}


	void end_container(s_erc *error)
	{
		S_EBMLWRITE_CALL($self, stop_container)($self, error);
	}
}
