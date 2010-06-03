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

%define ebml_reader_DOCSTRING
"""
SEbmlRead(path)


EBML data reader class. Create an Ebml reader object with a path to an EBML format file
from which to read. The header of the file is read.

:param path: Full path and file name of the file to read from.
:type path: string

:return: EBML data reader object.
:rtype: SEbmlRead
"""
%enddef

%feature("autodoc", ebml_reader_DOCSTRING) SEbmlRead;


%define ebml_version_DOCSTRING
"""
Get the EBML version of the file.
The version of EBML to which the document conforms to.

:return: EBML version.
:rtype: int
"""
%enddef

%feature("autodoc", ebml_version_DOCSTRING) ebml_version;


%define reader_version_DOCSTRING
"""
Get the EBML reader version of the file.
The minimum EBML version a parser has to support in order to read the document.

:return: EBML reader version.
:rtype: int
"""
%enddef

%feature("autodoc", reader_version_DOCSTRING) reader_version;


%define max_id_width_DOCSTRING
"""
Get the maximum ``ID`` width of the file.
The maximum width of the ``IDs`` used in this document.

:return: maximum ``ID`` width.
:rtype: int
:note: Speect can not read EBML format file with a ``ID`` width greater than 4.
"""
%enddef

%feature("autodoc", max_id_width_DOCSTRING) max_id_width;


%define max_size_width_DOCSTRING
"""
Get the maximum size width of the file.
The maximum width of the size descriptors used in this document.

:return: maximum size width.
:rtype: int
:note: Speect can not read EBML format file with a size width greater than 4.
"""
%enddef

%feature("autodoc", max_size_width_DOCSTRING) max_size_width;


%define doc_type_version_DOCSTRING
"""
Get the document type version of the file.
The version of document type to which the document conforms to.

:return: document type version.
:rtype: int
"""
%enddef

%feature("autodoc", doc_type_version_DOCSTRING) doc_type_version;


%define doc_type_read_version_DOCSTRING
"""
Get the document type reader version of the file.
The minimum ``doc_type_version`` version an interpreter has to
support in order to read the document.

:return: document type reader version.
:rtype: int
"""
%enddef

%feature("autodoc", doc_type_read_version_DOCSTRING) doc_type_reader_version;


%define doc_type_DOCSTRING
"""
doctype()

Get the document type of the file.
An ASCII string that identifies the type of the document.

:return: document type.
:rtype: string
"""
%enddef

%feature("autodoc", doc_type_DOCSTRING) SEbmlRead::doc_type;


%define peek_id_DOCSTRING
"""
peek_id()

Peek the ``ID`` of the next element in the ebml data source.


:return: Ebml ID.
:rtype: int
"""
%enddef

%feature("autodoc", peek_id_DOCSTRING) SEbmlRead::peek_id;


%define skip_element_DOCSTRING
"""
skip_element()

Skip the current ebml element.
"""
%enddef

%feature("autodoc", skip_element_DOCSTRING) SEbmlRead::skip_element;


%define read_uint_DOCSTRING
"""
read_uint()

Read current element as an unsigned integer.

:return: [ID, element]
:rtype: tuple
"""
%enddef

%feature("autodoc", read_uint_DOCSTRING) SEbmlRead::read_uint;


%define read_sint_DOCSTRING
"""
read_sint()

Read current element as a signed integer.

:return: [ID, element]
:rtype: tuple
"""
%enddef

%feature("autodoc", read_sint_DOCSTRING) SEbmlRead::read_sint;


%define read_double_DOCSTRING
"""
read_double()

Read current element as a double.

:return: [ID, element]
:rtype: tuple
"""
%enddef

%feature("autodoc", read_double_DOCSTRING) SEbmlRead::read_double;


%define read_str_DOCSTRING
"""
read_str()

Read current element as a str.

:return: [ID, element]
:rtype: tuple
"""
%enddef

%feature("autodoc", read_str_DOCSTRING) SEbmlRead::read_str;


%define open_container_DOCSTRING
"""
open_container()

Open a container element and return it's ``ID``.

:return: ID
:rtype: int
"""
%enddef

%feature("autodoc", open_container_DOCSTRING) SEbmlRead::open_container;


%define container_exhausted_DOCSTRING
"""
container_exhausted()

Query if all of a currently open container's elements have been read.
Thus, if true then the container is 'exhausted'.

:return: True of False.
:rtype: bool
"""
%enddef

%feature("autodoc", container_exhausted_DOCSTRING) SEbmlRead::container_exhausted;



%apply unsigned int *INOUT { uint32 *id };

%inline
%{
	SObject *sebml_reader_read_object(SEbmlRead *self, uint32 *id, s_erc *error)
	{
		SObject *val;


		val = S_EBMLREAD_CALL(self, read_object)(self, id, error);
		if (*error != S_SUCCESS)
			return NULL;

		return val;
	}
%}


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

	const char *doc_type(void)
	{
		return (const char*)$self->header->doctype;
	}

	~SEbmlRead()
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE($self, "~SEbmlRead()", &error);
	}


	uint32 peek_id(s_erc *error)
	{
		uint32 id;


		id = S_EBMLREAD_CALL($self, peek_id)($self, error);
		if (*error != S_SUCCESS)
			return 0;

		return id;
	}


	void skip_element(s_erc *error)
	{
		S_EBMLREAD_CALL($self, element_skip)($self, error);
	}


	PyObject *read_uint(s_erc *error)
	{
		PyObject *tuple;
		PyObject *object;
		uint32 val;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_uint)($self, &id, error);
		if (*error != S_SUCCESS)
			return NULL;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_uint",
					  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = PyInt_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);

		object = PyInt_FromLong((ulong) val);
		PyTuple_SET_ITEM(tuple, 1, object);

		return tuple;
	}


	PyObject *read_sint(s_erc *error)
	{
		PyObject *tuple;
		PyObject *object;
		sint32 val;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_sint)($self, &id, error);
		if (*error != S_SUCCESS)
			return NULL;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_sint",
					  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = PyInt_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);

		object = PyInt_FromLong((long) val);
		PyTuple_SET_ITEM(tuple, 1, object);

		return tuple;
	}


	PyObject *read_double(s_erc *error)
	{
		PyObject *tuple;
		PyObject *object;
		double val;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_double)($self, &id, error);
		if (*error != S_SUCCESS)
			return NULL;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_double",
					  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = PyInt_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);

		object = PyFloat_FromDouble(val);
		PyTuple_SET_ITEM(tuple, 1, object);

		return tuple;
	}


	PyObject *read_str(s_erc *error)
	{
		PyObject *tuple;
		PyObject *object;
		char *val;
		size_t len;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_utf8)($self, &id, error);
		if (*error != S_SUCCESS)
			return NULL;

		len = s_strsize(val, error);
		if (*error != S_SUCCESS)
			return NULL;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_str",
					  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = PyInt_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);

		object = PyString_FromStringAndSize(val, len);
		PyTuple_SET_ITEM(tuple, 1, object);
		S_FREE(val);

		return tuple;
	}


%pythoncode
%{
def read_object(self):
    """
    Read current element as an object.

    :return: [ID, element]
    :rtype: tuple
    :note: The object must have an appropriate *serialization formatter*
           defined and registered for the ``spct_ebml`` format. If not,
           then the object can not be read.
    """

    import speect

    ebml_id = 0
    sobject = sebml_reader_read_object(self, ebml_id)

    # Python takes ownership, TRUE
    pyobject = speect.sobject_2_pobject(sobject, TRUE)

    return [ ebml_id, pobject ]
%}


	uint32 open_container(s_erc *error)
	{
		uint32 id;


		id = S_EBMLREAD_CALL($self, container)($self, error);
		if (*error != S_SUCCESS)
			return 0;

		return id;
	}


	s_bool container_exhausted(s_erc *error)
	{
		s_bool query;


		query = S_EBMLREAD_CALL($self, container_at_end)($self, error);
		if (*error != S_SUCCESS)
			return FALSE;

		return query;
	}
}


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
