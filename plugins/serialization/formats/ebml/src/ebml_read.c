/************************************************************************************/
/* Copyright (c) 2008-2011 The Department of Arts and Culture,                      */
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
/* AUTHOR  : Gerrit Botha, Aby Louw                                                 */
/* DATE    : 20 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* EBML data source reader class.                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "ebml_read.h"
#include "ebml_def.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SEbmlReadClass EbmlReadClass; /* SEbmlRead class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_ebml_read_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&EbmlReadClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_ebml_read_class_reg",
			  "Failed to register SEbmlReadClass");
}


S_LOCAL void _s_ebml_read_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&EbmlReadClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_ebml_read_class_free",
			  "Failed to free SEbmlReadClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void s_ebml_free_levels(void *le, s_erc *error);

static void read_init(SEbmlRead **self, SDatasource *ds, s_erc *error);

static uint32 read_id(SEbmlRead *self, s_erc *error);

static void seek(SEbmlRead *self, long pos, s_seek_mode mode, s_erc *error);

static uint32 peek_id(SEbmlRead *self, s_erc *error);

static uint32 read_element_size(SEbmlRead *self, s_erc *error);

static void skip_element(SEbmlRead *self, s_erc *error);

static s_byte *read_binary(SEbmlRead *self, uint32 *id, uint32 *size, s_erc *error);

static uint32 read_uint(SEbmlRead *self, uint32 *id, s_erc *error);

static sint32 read_sint(SEbmlRead *self, uint32 *id, s_erc *error);

static float read_float(SEbmlRead *self, uint32 *id, s_erc *error);

static double read_double(SEbmlRead *self, uint32 *id, s_erc *error);

static char *read_ascii(SEbmlRead *self, uint32 *id, s_erc *error);

static char *read_utf8(SEbmlRead *self, uint32 *id, s_erc *error);

static SObject *read_object(SEbmlRead *self, uint32 *id, s_erc *error);

static uint32 read_container(SEbmlRead *self, s_erc *error);

static s_bool read_container_at_end(SEbmlRead *self, s_erc *error);

static void read_header(SEbmlRead *self, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void read_init(SEbmlRead **self, SDatasource *ds, s_erc *error)
{
	S_CLR_ERR(error);

	/* make sure the byte order of the data source is big endian (EBML specs).*/
	SDatasourceSetByteOrder(ds, S_BYTEORDER_BE, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_init",
				  "Failed to set data source byte order to big endian"))
		return;

	S_DATAREADER((*self))->ds = ds;

	(*self)->header = S_MALLOC(s_ebml_header, 1);
	if ((*self)->header == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "read_init",
				  "Failed to allocate memory for header structure");
		/* release data source, caller will delete */
		S_DATAREADER((*self))->ds = NULL;
		S_DELETE(*self, "read_init", error);
		*self = NULL;
		return;
	}

	(*self)->header->doctype = NULL;

	(*self)->level = s_list_new(NULL, &s_ebml_free_levels, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_init",
				  "Failed to allocate container level list"))
	{
		/* release data source, caller will delete */
		S_DATAREADER((*self))->ds = NULL;
		S_DELETE(*self, "read_init", error);
		*self = NULL;
		return;
	}

	/* read header */
	read_header(*self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_init",
				  "Failed to read the header of the ebml data source"))
	{
		/* release data source, caller will delete */
		S_DATAREADER((*self))->ds = NULL;
		S_DELETE(*self, "read_init", error);
		*self = NULL;
		return;
	}
}


static uint32 read_id(SEbmlRead *self, s_erc *error)
{
	uint8 buf;
	int mask = 0x80;
	int length;
	int i;
	uint32 id;


	/*
	 * Element ID is encoded as a variable integer as in "Extensible Binary Markup Language"
	 * document ebml-1.0.txt.
	 *
	 * VINT = VINT_WIDTH VINT_MARKER VINT_DATA
	 * VINT_WIDTH = *%b0
	 * VINT_MARKER = %b1
	 * VINT_DATA = *BIT *BYTE
	 *
	 * VINT_DATA should be the Element ID, but we we don't follow the specs exactly,
	 * we use VINT_MARKER as part of VINT_DATA (this is done in matroska format) as
	 * it makes it much easier to see in a hex-viewer. Of course then
	 * the given ID must be valid in terms of the specification.
	 *
	 */

	S_CLR_ERR(error);

	if (self->header->max_id_width == 0)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_id",
				  "maximum id width is 0");
		return 0;
	}

	/* get the first byte to determine the length of the id */
	SDatasourceRead(S_DATAREADER(self)->ds, &buf, sizeof(uint8), 1, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "read_id",
				  "Failed to read first byte of id"))
		return 0;

	length = 1; /* already read first byte */
	while ((mask & buf) == 0)
	{
		length++;
		mask >>= 1;

		if (length > self->header->max_id_width)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_id",
					  "id length exceeds maximum width");
			return 0;
		}
	}

	id = buf;

	/* already read first byte, read the rest */
	for (i = 1; i < length; i++)
	{
		buf = s_uint8_read(S_DATAREADER(self)->ds, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "read_id",
					  "Failed to read byte %d of id", i))
			return 0;

		id = (id << 8) | buf;
	}

	/* do not swap the id */
	return id;
}


static void seek(SEbmlRead *self, long pos, s_seek_mode mode, s_erc *error)
{
	S_CLR_ERR(error);
	SDatasourceSeek(S_DATAREADER(self)->ds, pos, mode, error);
	S_CHK_ERR(error, S_CONTERR,
			  "seek",
			  "Failed to seek ebml data source");
}


static uint32 peek_id(SEbmlRead *self, s_erc *error)
{
	long pos;
	uint32 id;


	S_CLR_ERR(error);

	/* store current position. */
	pos = SDatasourceTell(S_DATAREADER(self)->ds, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "peek_id",
				  "Failed to tell ebml data source"))
		return 0;

	id = read_id(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "PeekID",
				  "Failed to read id"))
		return 0;

	SDatasourceSeek(S_DATAREADER(self)->ds, pos, S_SEEK_SET, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "PeekID",
				  "Failed to seek ebml data source to stored position"))
		return 0;

	return id;
}


static uint32 read_element_size(SEbmlRead *self, s_erc *error)
{
	uint8 buf;
	int mask = 0x80;
	int length;
	int i;
	uint32 size;


	S_CLR_ERR(error);

	if (self->header->max_size_width == 0)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_element_size",
				  "maximum size width is 0");
		return 0;
	}

	/* get the first byte to determine the length of the size */
	SDatasourceRead(S_DATAREADER(self)->ds, &buf, sizeof(uint8), 1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_element_size",
				  "Failed to read first byte of size"))
		return 0;

	length = 1; /* already read first byte */
	while ((mask & buf) == 0)
	{
		length++;
		mask >>= 1;

		if (length > self->header->max_id_width)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_element_size",
					  "size length exceeds maximum width");
			return 0;
		}
	}

	/* we dont want VINT_MARKER in the size. */
	buf &= (mask - 1);

	size = buf;

	/* already read first byte, read the rest */
	for (i = 1; i < length; i++)
	{
		buf = s_uint8_read(S_DATAREADER(self)->ds, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "read_element_size",
					  "Failed to read byte %d of size", i))
			return 0;

		/* shift left and add */
		size = (size << 8) | buf;
	}

	return size;
}


static void skip_element(SEbmlRead *self, s_erc *error)
{
	uint32 id;
	uint32 size;


	S_CLR_ERR(error);
	id = read_id(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "skip_element",
				  "Failed to read element id"))
		return;

	size = read_element_size(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "skip_element",
				  "Failed to read element size"))
		return;

	if (size == S_EBML_SIZE_UNKNOWN) /* unknown size (stream data) */
		return;

	SDatasourceSeek(S_DATAREADER(self)->ds, (long)size, S_SEEK_CUR, error);
	S_CHK_ERR(error, S_CONTERR,
			  "skip_element",
			  "Failed to seek ebml data source to skip element");
	S_UNUSED(id);
}


static s_byte *read_binary(SEbmlRead *self, uint32 *id, uint32 *size, s_erc *error)
{
	s_byte *data;


	S_CLR_ERR(error);

	*id = read_id(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_binary",
				  "Failed to read element id"))
		return NULL;

	*size = read_element_size(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_binary",
				  "Failed to read element size"))
		return NULL;

	if (*size <= 0) /* some elements can have a zero size, -1 for stream data */
		return NULL;

	/* elements cannot have sizes bigger than uint32, only containers can. */
	data = S_MALLOC(s_byte, (*size));
	if (data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "read_binary",
				  "Failed to allocate memory for data");
		return NULL;
	}

	SDatasourceRead(S_DATAREADER(self)->ds, (void*)data, sizeof(s_byte), (*size), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_binary",
				  "Failed to read from data source"))
	{
		S_FREE(data);
		return NULL;
	}

	return data;
}


static uint32 read_uint(SEbmlRead *self, uint32 *id, s_erc *error)
{
	s_byte *data;
	uint32 size;
	uint i;
	uint32 val;


	S_CLR_ERR(error);
	data = read_binary(self, id, &size, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "read_uint",
				  "Failed to read binary data"))
		return 0;

	if (size > self->header->max_size_width)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_uint",
				  "Invalid integer size %d.\n", size);
		return 0;
	}

	val = 0;

	if (size > 0)
	{
		for (i = 0; i < size; i++)
			val = (val << 8) | data[i];

		S_FREE(data);

		/* 	(*data) = spct_swap_be32(*data); */
	}

	/* A zero byte integer represents the integer value 0. */
	return val;
}


static sint32 read_sint(SEbmlRead *self, uint32 *id, s_erc *error)
{
	s_byte *data;
	uint32 size;
	s_bool is_negative;
	uint i;
	sint32 val;


	S_CLR_ERR(error);

	data = read_binary(self, id, &size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_sint",
				  "Failed to read binary data"))
	{
		if (data != NULL)
			S_FREE(data);
		return 0;
	}

	if (size > self->header->max_size_width)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_sint",
				  "Invalid integer size %d.\n", size);
		return 0;
	}

	val = 0;
	is_negative = FALSE;

	if (size > 0)
	{
		data = s_swap_be_bytes(data, size, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "read_sint",
					  "Call to s_swap_be_bytes failed"))
		{
			if (data != NULL)
				S_FREE(data);
			return 0;
		}

		if (data[size - 1] & 0x80)
		{
			is_negative = TRUE;
			val = 0xffffffff;
		}

		for (i = 0; i < size; i++)
			((uint8*)&val)[i] = data[i];

		S_FREE(data);

		if (is_negative == TRUE)
		{
			val = ~val;
			val = 0 - (val + 1);
		}
	}

	/* A zero byte integer represents the integer value 0. */
	return val;
}


static float read_float(SEbmlRead *self, uint32 *id, s_erc *error)
{
	uint32 size;
	float val;


	S_CLR_ERR(error);

	*id = read_id(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_float",
				  "Failed to read element id"))
		return 0.0;

	size = read_element_size(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_float",
				  "Failed to read element size"))
		return 0.0;

	if ((size != 0) && (size != 4))
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_float",
				  "Invalid float size %d (valid sizes 0, 4)", size);
		return 0.0;
	}

	val = 0.0;

	if (size != 0)
	{
		val = s_float_read(S_DATAREADER(self)->ds, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_float",
					  "Failed to read element data"))
			return 0.0;
	}

	/* A zero byte float represents the float value 0.0. */
	return val;
}


static double read_double(SEbmlRead *self, uint32 *id, s_erc *error)
{
	uint32 size;
	double val;


	S_CLR_ERR(error);

	*id = read_id(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_double",
				  "Failed to read element id"))
		return 0.0;

	size = read_element_size(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_double",
				  "Failed to read element size"))
		return 0.0;

	if ((size != 0) && (size != 8))
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_double",
				  "Invalid double size %d (valid sizes 0, 8)", size);
		return 0.0;
	}

	val = 0.0;

	if (size != 0)
	{
		val = s_double_read(S_DATAREADER(self)->ds, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_double",
					  "Failed to read element data"))
			return 0.0;
	}

	/* A zero byte double represents the double value 0.0. */
	return val;
}


static char *read_ascii(SEbmlRead *self, uint32 *id, s_erc *error)
{
	s_byte *data;
	uint32 size;
	char *buf;


	S_CLR_ERR(error);
	data = read_binary(self, id, &size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_ascii",
				  "Failed to read binary data"))
		return NULL;

	buf = NULL;

	if (size != 0)
	{
		buf = S_MALLOC(char, size + 1);
		if (buf == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "read_ascii",
					  "Failed to allocate memory for data");
			return NULL;
		}

		memcpy(buf, data, size);
		S_FREE(data);
		buf[size] = '\0';
	}

	/*  Note that a string can be of zero length. */
	return buf;
}


static char *read_utf8(SEbmlRead *self, uint32 *id, s_erc *error)
{
	s_byte *data;
	uint32 size;
	char *buf;
	s_bool validity;


	S_CLR_ERR(error);
	data = read_binary(self, id, &size, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "read_utf8",
				  "Failed to read binary data"))
		return NULL;

	buf = NULL;

	if (size != 0)
	{
		buf = S_MALLOC(char, size + 1);
		if (buf == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "read_utf8",
					  "Failed to allocate memory for data");
			return NULL;
		}

		memcpy(buf, data, size);
		S_FREE(data);
		buf[size] = '\0';

		validity = s_isvalid(buf, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_utf8",
					  "Failed to validate utf8 string"))
		{
			S_FREE(buf);
			return NULL;
		}

		if (validity == FALSE)
		{
			S_CTX_ERR(error, S_MEMERROR,
					  "read_utf8",
					  "Read invalid utf8 string");
			S_FREE(buf);
			return NULL;
		}
	}

	/*  Note that a string can be of zero length. */
	return buf;
}


static SObject *read_object(SEbmlRead *self, uint32 *id, s_erc *error)
{
	char *class_name = NULL;
	uint32 tmp_id;
	const SObjectClass *class;
	SObject *object = NULL;
	s_bool container_closed;
	s_bool is_readable;
	const SSerializedObject *formatter;
	s_erc local_err = S_SUCCESS;


	/*  The definition of SObject as written in ebml is as follows:
	 *   object := container
	 *   {
	 *       ObjectClassName := 81 string;
	 *       ObjectData = 82 binary;
	 *   }
	 */

	S_CLR_ERR(error);

	/* read object container */
	*id = read_container(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_object",
				  "Failed to read object container"))
		return NULL;

	/* sanity */
	tmp_id = peek_id(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_object",
				  "Failed to peek object id"))
		return NULL;

	if (tmp_id != S_EBML_ID_OBJECT_CLASSNAME)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_object",
				  "Expected object class name id (0x%X), read id (0x%X)",
				  S_EBML_ID_OBJECT_CLASSNAME, tmp_id);
		return NULL;
	}

	class_name = read_utf8(self, &tmp_id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_object",
				  "Failed to read object class name"))
		return NULL;

	/* check if we have this class type registered in speect */
	class = s_class_find(class_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_object",
				  "Call to \"s_class_find\" failed"))
	{
		S_FREE(class_name);
		return NULL;
	}

	if (class == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_object",
				  "Read object's class '%s' is not registered",
				  class_name);
		goto skip_object_error;
	}


	/* query if object of this class can be read in this format */
	is_readable = SObjectTypeIsReadable(class_name, "spct_ebml", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_object",
				  "Call to \"SObjectTypeIsReadable\" failed"))
		goto skip_object_error;

	if (!is_readable)
	{
		/*
		 * we cant read this object,
		 * not an error, just unlucky!
		 * skip it
		 */
		goto skip_object;
	}

	/* create a new object */
	object = S_NEW_FROM_NAME(class_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_object",
				  "Failed to create new object of class '%s'",
				  class_name))
		goto skip_object_error;

	S_FREE(class_name); /* sets class_name = NULL */

	/* get the ebml formatter for the object */
	formatter = SSerializedObjectGet(object, "spct_ebml", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_object",
				  "Call to \"SSerializedObjectGet\" failed"))
	{
		S_DELETE(object, "read_object", error); /* sets object = NULL */
		goto skip_object_error;
	}

	/* check that we have a formatter */
	if (formatter == NULL)
	{
		/* it cannot read data */
		S_CTX_ERR(error, S_METHINVLD,
				  "read_object",
				  "Object of class '%s' cannot read itself",
				  class_name);

		S_DELETE(object, "read_object", error);  /* sets object = NULL */
		goto skip_object_error;
	}

	/* sanity */
	tmp_id = peek_id(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_object",
				  "Failed to peek object data id"))
	{
		S_DELETE(object, "read_object", error); /* sets object = NULL */
		goto skip_object_error;
	}

	if (tmp_id != S_EBML_ID_OBJECT_DATA)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_object",
				  "Expected object data id (0x%X), read id (0x%X)",
				  S_EBML_ID_OBJECT_DATA, tmp_id);

		S_DELETE(object, "read_object", error);  /* sets object = NULL */
		goto skip_object_error;
	}

	/* read object */
	SObjectRead(object, formatter, S_DATAREADER(self), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_object",
				  "Call to \"SObjectRead\" failed"))
	{
		S_DELETE(object, "read_object", error);
		return NULL;
	}

	/* close the container */
	container_closed = read_container_at_end(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_object",
				  "Call to \"read_container_at_end\" for read object failed"))
	{
		S_DELETE(object, "read_object", error);
		return NULL;
	}

	if (container_closed != TRUE)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_object",
				  "Failed to close container of read object");
		S_DELETE(object, "read_object", error);
		return NULL;
	}

	return object;

/* cleanup code */
skip_object_error:
	local_err = S_FAILURE;

skip_object:
	if (class_name != NULL)
		S_FREE(class_name);

	skip_element(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_object",
				  "Call to \"skip_element\" failed"))
		return NULL;

	container_closed = read_container_at_end(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_object",
				  "Call to \"read_container_at_end\" for failed"))
		return NULL;

	if (container_closed != TRUE)
	{
		S_CTX_ERR(error, S_CONTERR,
				  "read_object",
				  "Failed to close container of object");
		return NULL;
	}

	if (local_err != S_SUCCESS)
		/* restore error as calls to skip_element and read_container_at_end cleared it */
		S_NEW_ERR(error, S_FAILURE);

	return NULL;
}


static uint32 read_container(SEbmlRead *self, s_erc *error)
{
	uint32 size;
	uint32 id;
	long *master_end;
	long current_pos;


	S_CLR_ERR(error);

	id = read_id(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_container",
				  "Failed to read container id"))
		return 0;

	current_pos = SDatasourceTell(S_DATAREADER(self)->ds, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_container",
				  "Failed to tell data source position"))
		return 0;

	size = read_element_size(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_container",
				  "Failed to read container size"))
		return 0;

	/* store the ending position of the master in the file */
	master_end = S_MALLOC(long, 1);
	if (master_end == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "read_container",
				  "Failed to allocate memory for ending position of container");
		return 0;
	}

	*master_end = current_pos + size;

	s_list_push(self->level, (void*)master_end, error);

	/* caller must decide on action */
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_container",
				  "Failed to push container size into list"))
		return 0;

	return id;
}


static s_bool read_container_at_end(SEbmlRead *self, s_erc *error)
{
	long pos;
	const s_list_element *current_master;
	long *master_end;


	S_CLR_ERR(error);

	/* get the current position. */
	pos = SDatasourceTell(S_DATAREADER(self)->ds, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_container_at_end",
				  "Failed to tell data source position"))
		return FALSE;

	/* get the start position of current master. */
	current_master = s_list_last(self->level, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_container_at_end",
				  "Call to s_list_last failed"))
		return FALSE;

	if (current_master == NULL) /* there are no masters, so it's true that its finished. */
		return TRUE;

	/* cast away const */
	master_end = (long*)s_list_element_get(current_master, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_container_at_end",
				  "Call to s_list_element_get failed"))
		return FALSE;

	if (pos >= (*master_end))
	{
		/*
		 * this master is finished, pop positional value and
		 * free it.
		 */
		master_end = s_list_pop(self->level, error);

		if (master_end != NULL)
			S_FREE(master_end);

		S_CHK_ERR(error, S_CONTERR,
				  "read_container_at_end",
				  "Call to s_list_pop failed");

		return TRUE;
	}

	/* stil in master. */
	return FALSE;
}


static void read_header(SEbmlRead *self, s_erc *error)
{
	uint32 id;
	s_bool container_closed;


	S_CLR_ERR(error);

	/*
	 * need some values for max widths while header
	 * is being read. Just to stop anything crazy.
	 */
	self->header->max_id_width = 4;
	self->header->max_size_width = 4;

	id = peek_id(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_header",
				  "Failed to peek header id"))
		return;

	if (id != S_EBML_ID_HEADER)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_header",
				  "Expected header id (0x%X), peeked id (0x%X)",
				  S_EBML_ID_HEADER, id);
		return;
	}

	id = read_container(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_header",
				  "Failed to read header container"))
		return;

	while (1)
	{
		container_closed = read_container_at_end(self, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_header",
					  "Call to ReadContainerClosed failed"))
			return;

		if (container_closed == TRUE)
			break;

		id = peek_id(self, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_header",
					  "Failed to peek element id"))
			return;

		switch (id)
		{
		case S_EBML_ID_EBMLREADVERSION :
		{
			self->header->ebml_read_version = read_uint(self, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_header",
						  "Failed to read EBML version"))
				return;

			break;
		}
		case S_EBML_ID_EBMLMAXSIZELENGTH:
		{
			self->header->max_size_width = read_uint(self, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_header",
						  "Failed to read EBML max size width"))
				return;

			break;
		}
		case S_EBML_ID_EBMLMAXIDLENGTH:
		{
			self->header->max_id_width = read_uint(self, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_header",
						  "Failed to read EBML max id width"))
				return;

			break;
		}
		case S_EBML_ID_DOCTYPE:
		{
			self->header->doctype = read_ascii(self, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_header",
						  "Failed to read EBML doctype"))
				return;

			break;
		}
		case S_EBML_ID_DOCTYPEREADVERSION:
		{
			self->header->doctype_read_version = read_uint(self, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_header",
						  "Failed to read EBML doctype read version"))
				return;

			break;
		}
		case S_EBML_ID_VOID:
			break;
		case S_EBML_ID_EBMLVERSION:
		{
			self->header->ebml_version = read_uint(self, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_header",
						  "Failed to read EBML version"))
				return;

			break;
		}
		case S_EBML_ID_DOCTYPEVERSION:
		{
			self->header->doctype_version = read_uint(self, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_header",
						  "Failed to read EBML doctype version"))
				return;

			break;
		}
		default:
		{
			S_WARNING(S_WARNERR,
					  "read_header",
					  "Unknown data type 0x%x in EBML header (ignored)", id);

			/* skip unknown data */
			skip_element(self, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_header",
						  "Failed to skip unread data type 0x%x in EBML header", id))
				return;

			break;
		}
		}
	}
}


static void s_ebml_free_levels(void *le, s_erc *error)
{
	long *master_end = NULL;


	S_CLR_ERR(error);
	master_end = (long*)le;
	S_FREE(master_end);
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitEbmlRead(void *obj, s_erc *error)
{
	SEbmlRead *self = obj;


	S_CLR_ERR(error);
 	self->header = NULL;
	self->level = NULL;
}


static void DisposeEbmlRead(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void DestroyEbmlRead(void *obj, s_erc *error)
{
	SEbmlRead *self = obj;


	S_CLR_ERR(error);

	if (self->header != NULL)
	{
		if (self->header->doctype != NULL)
			S_FREE(self->header->doctype);

		S_FREE(self->header);
	}

	if (self->level)
	{
		s_list_delete(self->level, error);
		S_CHK_ERR(error, S_CONTERR,
				  "DestroyEbmlRead",
				  "Failed to delete ebml container levels");
	}
}


static void ReadInit(SEbmlRead **self, SDatasource *ds, s_erc *error)
{
	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadInit",
				  "Argument \"self\" is NULL");
		return;
	}

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadInit",
				  "Argument \"ds\" is NULL");
		return;
	}

	read_init(self, ds, error);
	S_CHK_ERR(error, S_CONTERR,
			  "ReadInit",
			  "Call to \"read_init\" failed");
}


static uint32 ReadID(SEbmlRead *self, s_erc *error)
{
	uint32 id;

	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadID",
				  "Argument \"self\" is NULL");
		return 0;
	}

	id = read_id(self, error);
	S_CHK_ERR(error, S_CONTERR,
			  "ReadID",
			  "Call to \"read_id\" failed");

	return id;
}


static void Seek(SEbmlRead *self, long pos, s_seek_mode mode, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "Seek",
				  "Argument \"self\" is NULL");
		return;
	}

	seek(self, pos, mode, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Seek",
			  "Call to \"seek\" failed");
}


static uint32 PeekID(SEbmlRead *self, s_erc *error)
{
	uint32 id;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "PeekID",
				  "Argument \"self\" is NULL");
		return 0;
	}

	id = peek_id(self, error);
	S_CHK_ERR(error, S_CONTERR,
			  "PeekID",
			  "Call to \"peek_id\" failed");

	return id;
}


static uint32 ReadElementSize(SEbmlRead *self, s_erc *error)
{
	uint32 size;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadElementSize",
				  "Argument \"self\" is NULL");
		return 0;
	}

	size = read_element_size(self, error);
	S_CHK_ERR(error, S_CONTERR,
			  "ReadElementSize",
			  "Call to \"read_element_size\" failed");

	return size;
}


static void SkipElement(SEbmlRead *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SkipElement",
				  "Argument \"self\" is NULL");
		return;
	}

	skip_element(self, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SkipElement",
			  "Call to \"skip_element\" failed");
}


static s_byte *ReadBinary(SEbmlRead *self, uint32 *id, uint32 *size, s_erc *error)
{
	s_byte *data;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadBinary",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (id == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadBinary",
				  "Argument \"id\" is NULL");
		return NULL;
	}

	if (size == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadBinary",
				  "Argument \"size\" is NULL");
		return NULL;
	}

	data = read_binary(self, id, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ReadBinary",
				  "Call to \"read_binary\" failed"))
		return NULL;

	return data;
}


static uint32 ReadUint(SEbmlRead *self, uint32 *id, s_erc *error)
{
	uint32 val;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadUint",
				  "Argument \"self\" is NULL");
		return 0;
	}

	if (id == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadUint",
				  "Argument \"id\" is NULL");
		return 0;
	}

	val = read_uint(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ReadUint",
				  "Call to \"read_uint\" failed"))
		return 0;

	return val;
}


static sint32 ReadSint(SEbmlRead *self, uint32 *id, s_erc *error)
{
	sint32 val;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadSint",
				  "Argument \"self\" is NULL");
		return 0;
	}

	if (id == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadSint",
				  "Argument \"id\" is NULL");
		return 0;
	}

	val = read_sint(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ReadSint",
				  "Call to \"read_sint\" failed"))
		return 0;

	return val;
}


static float ReadFloat(SEbmlRead *self, uint32 *id, s_erc *error)
{
	float val;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadFloat",
				  "Argument \"self\" is NULL");
		return 0.0;
	}

	if (id == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadFloat",
				  "Argument \"id\" is NULL");
		return 0.0;
	}

	val = read_float(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ReadFloat",
				  "Call to \"read_float\" failed"))
		return 0.0;

	return val;
}


static double ReadDouble(SEbmlRead *self, uint32 *id, s_erc *error)
{
	double val;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadDouble",
				  "Argument \"self\" is NULL");
		return 0.0;
	}

	if (id == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadDouble",
				  "Argument \"id\" is NULL");
		return 0.0;
	}

	val = read_double(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ReadDouble",
				  "Call to \"read_double\" failed"))
		return 0.0;

	return val;
}


static char *ReadAscii(SEbmlRead *self, uint32 *id, s_erc *error)
{
	char *buf;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadAscii",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (id == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadAscii",
				  "Argument \"id\" is NULL");
		return NULL;
	}

	buf = read_ascii(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ReadAscii",
				  "Call to \"read_ascii\" failed"))
		return NULL;

	return buf;
}


static char *ReadUtf8(SEbmlRead *self, uint32 *id, s_erc *error)
{
	char *buf;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadUtf8",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (id == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadUtf8",
				  "Argument \"id\" is NULL");
		return NULL;
	}

	buf = read_utf8(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ReadUtf8",
				  "Call to \"read_utf8\" failed"))
		return NULL;

	return buf;
}


static SObject *ReadObject(SEbmlRead *self, uint32 *id, s_erc *error)
{
	SObject *object;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadObject",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (id == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadObject",
				  "Argument \"id\" is NULL");
		return NULL;
	}

	object = read_object(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ReadObject",
				  "Call to \"read_object\" failed"))
		return NULL;

	return object;
}


static uint32 ReadContainer(SEbmlRead *self, s_erc *error)
{
	uint32 id;

	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadContainer",
				  "Argument \"self\" is NULL");
		return 0;
	}

	id = read_container(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ReadContainer",
				  "Call to \"read_container\" failed"))
		return 0;

	return id;
}


static s_bool ReadContainerAtEnd(SEbmlRead *self, s_erc *error)
{
	s_bool at_end;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ReadContainerAtEnd",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	at_end = read_container_at_end(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ReadContainerAtEnd",
				  "Call to \"read_container\" failed"))
		return FALSE;

	return at_end;
}


/************************************************************************************/
/*                                                                                  */
/* SEbmlRead class initialization                                                   */
/*                                                                                  */
/************************************************************************************/

static SEbmlReadClass EbmlReadClass =
{
	{
		/* SObjectClass */
		{
			"SDataReader:SEbmlRead",
			sizeof(SEbmlRead),
			{ 0, 1},
			InitEbmlRead,    /* init    */
			DestroyEbmlRead, /* destroy */
			DisposeEbmlRead, /* dispose */
			NULL,            /* compare */
			NULL,            /* print   */
			NULL,            /* copy    */
		},
		/* SDataReader */
		/* nothing */
	},
	/* SEbmlReadClass */
	ReadInit,                        /* read_init        */
	ReadID,                          /* read_id          */
	PeekID,                          /* peek_id          */
	Seek,                            /* seek             */
	ReadElementSize,                 /* element_size     */
	SkipElement,                     /* element_skip     */
	ReadUint,                        /* read_uint        */
	ReadSint,                        /* read_sint        */
	ReadFloat,                       /* read_float       */
	ReadDouble,                      /* read_double      */
	ReadAscii,                       /* read_ascii       */
	ReadUtf8,                        /* read_utf8        */
	ReadBinary,                      /* read_binary      */
	ReadObject,                      /* read_object      */
	ReadContainer,                   /* container        */
	ReadContainerAtEnd               /* container_at_end */
};
