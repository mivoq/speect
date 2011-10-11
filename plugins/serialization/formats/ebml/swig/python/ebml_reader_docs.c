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
/* Python documentation strings for SEbmlRead.                                      */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%define ebml_reader_DOCSTRING
"""
SEbmlRead(path)


EBML data reader class. Create an Ebml reader object with a path to an EBML format file
from which to read. The header of the file is read on successful opening of the file.
The file is closed when the reader is deleted.

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


%define read_object_DOCSTRING
"""
read_object()

Read current element as an object.

:return: [ID, element]
:rtype: tuple
:note: The object must have an appropriate *serialization formatter*
       defined and registered for the ``spct_ebml`` format. If not,
       then the object can not be read.
"""
%enddef

%feature("autodoc", read_object_DOCSTRING) SEbmlRead::read_object;


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

