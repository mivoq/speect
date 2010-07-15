/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* DATE    : 2 September 2008                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* EBML data source writer class.                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "ebml_write.h"
#include "ebml_def.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SEbmlWriteClass EbmlWriteClass; /* SEbmlWrite class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_ebml_write_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&EbmlWriteClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_ebml_write_class_reg",
			  "Failed to register SEbmlWriteClass");
}


S_LOCAL void _s_ebml_write_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&EbmlWriteClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_ebml_write_class_free",
			  "Failed to free SEbmlWriteClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void s_ebml_free_levels(void *le, s_erc *error);

static void s_ebml_write_id(SEbmlWrite *self, uint32 id, s_erc *error);

static void s_ebml_write_size(SEbmlWrite *self, uint32 size, s_erc *error);

static void s_ebml_write_binary(SEbmlWrite *self, s_byte *data, uint32 size, s_erc *error);

static uint32 s_ebml_get_sint_size(sint32 i);

static uint32 s_ebml_get_uint_size(uint32 i);

static void s_ebml_write_header(SEbmlWrite *self, s_erc *error);

static void write_init(SEbmlWrite **self, SDatasource *ds,
					   s_ebml_header *header, s_erc *error);

static void write_uint(SEbmlWrite *self, uint32 id, uint32 i, s_erc *error);

static void write_sint(SEbmlWrite *self, uint32 id, sint32 i, s_erc *error);

static void write_float(SEbmlWrite *self, uint32 id, float f, s_erc *error);

static void write_double(SEbmlWrite *self, uint32 id, double d, s_erc *error);

static void write_ascii(SEbmlWrite *self, uint32 id, const char *string, s_erc *error);

static void write_utf8(SEbmlWrite *self, uint32 id, const char *string, s_erc *error);

static void write_binary(SEbmlWrite *self, uint32 id, const s_byte *data, uint32 size,
						 s_erc *error);

static void write_object(SEbmlWrite *self, uint32 id,
						 const SObject *object, s_erc *error);

static void write_start_container(SEbmlWrite *self, uint32 id, s_erc *error);

static void write_stop_container(SEbmlWrite *self, s_erc *error);

static long ebml_tell(SEbmlWrite *self, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void s_ebml_free_levels(void *le, s_erc *error)
{
	long *master_end = NULL;


	S_CLR_ERR(error);
	master_end = (long*)le;
	S_FREE(master_end);
}


static void s_ebml_write_id(SEbmlWrite *self, uint32 id, s_erc *error)
{
	uint length = self->header->max_id_width;
	uint mask = 0x10;
	int test;
	s_byte *data;
	uint i;


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

	test = id & (mask << ((length - 1) * 8));

	while ((test == 0) && (length >= 1))
	{
		mask <<= 1;
		length--;
		test = id & (mask << ((length - 1) * 8));
	}

	if (length == 0)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_ebml_write_id",
				  "Invalid ebml id length (%d) for id %d, check if id is valid", length, id);
		return;
	}

	data = S_MALLOC(s_byte, length);
	if (data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_ebml_write_id",
				  "Failed to allocate memory to data for writing");
		return;
	}

	/* little endian insertion into data buffer */
	for (i = 0; i < length; i++)
	{
		data[i] = id & 0xff;
		id >>= 8;
	}

	data = s_swap_be_bytes(data, length, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_ebml_write_id",
				  "Call to s_swap_be_bytes failed"))
	{
		S_FREE(data);
		return;
	}

	s_ebml_write_binary(self, data, length, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_ebml_write_id",
				  "Call to s_ebml_write_binary failed"))
	{
		S_FREE(data);
		return;
	}

	S_FREE(data);
}


static void s_ebml_write_size(SEbmlWrite *self, uint32 size, s_erc *error)
{
	uint mask;
	uint length;
	s_byte *data;
	uint i;


	S_CLR_ERR(error);

	if (size < 127) /* 2^7 - 1 */
	{
		length = 1;
		mask = 0x80;
	}
	else if (size < 16383) /* 2^14 - 1 */
	{
		length = 2;
		mask = 0x4000;
	}
	else if (size < 2097151) /* 2^21 - 1 */
	{
		length = 3;
		mask = 0x200000;
	}
	else if (size < 268435455) /* 2^28 - 1 */
	{
		length = 4;
		mask = 0x10000000;
	}
	else
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_ebml_write_size",
				  "Invalid element byte size (%d)", size);
		return;
	}

	size = size | mask;

	data = S_MALLOC(s_byte, length);
	if (data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_ebml_write_size",
				  "Failed to allocate memory to data for writing");
		return;
	}

	/* little endian */
	for (i = 0; i < length; i++)
	{
		data[i] = size & 0xff;
		size >>= 8;
	}

	data = s_swap_be_bytes(data, length, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_ebml_write_id",
				  "Call to s_swap_be_bytes failed"))
	{
		S_FREE(data);
		return;
	}

	s_ebml_write_binary(self, data, length, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_ebml_write_id",
				  "Call to s_ebml_write_binary failed"))
	{
		S_FREE(data);
		return;
	}

	S_FREE(data);
}


static void s_ebml_write_binary(SEbmlWrite *self, s_byte *data, uint32 size, s_erc *error)
{
	uint i;


	S_CLR_ERR(error);

	for (i = 0; i < size; i++)
	{
		SDatasourceWrite(S_DATAWRITER(self)->ds, (void*)&data[i], sizeof(uint8),
						 1, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_ebml_write_binary",
					  "Call to SDatasourceWrite failed"))
			return;
	}
}


static uint32 s_ebml_get_sint_size(sint32 i)
{
	uint32 size;


	if (i <= 0x7F && i >= (-0x80))
	{
		size = 1;
	}
	else if (i <= 0x7FFF && i >= (-0x8000))
	{
		size = 2;
	}
	else if (i <= 0x7FFFFF && i >= (-0x800000))
	{
		size = 3;
	}
	else
	{
		/* if (i <= 0x7FFFFFFF && i >= (-0x80000000))  */
		size = 4;
	}

	return size;
}


static uint32 s_ebml_get_uint_size(uint32 i)
{
	uint32 size;


	if (i <= 0xFF)
	{
		size = 1;
	}
	else if (i <= 0xFFFF)
	{
		size = 2;
	}
	else if (i <= 0xFFFFFF)
	{
		size = 3;
	}
	else
	{
		/* if (i <= 0xFFFFFFFF)  */
		size = 4;
	}

	return size;
}


static void write_init(SEbmlWrite **self, SDatasource *ds,
					   s_ebml_header *header, s_erc *error)
{
	S_CLR_ERR(error);

	/* make sure the byte order of the data source is big endian (EBML specs).*/
	SDatasourceSetByteOrder(ds, S_BYTEORDER_BE, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_init",
				  "Failed to set data source byte order to big endian"))
		return;

	S_DATAWRITER((*self))->ds = ds;
	(*self)->header = header;

	(*self)->level = s_list_new(NULL, &s_ebml_free_levels, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_init",
				  "Failed to allocate container level list"))
	{
		/* release data source and header, caller will delete */
		S_DATAREADER((*self))->ds = NULL;
		(*self)->header = NULL;
		S_DELETE(*self, "write_init", error);
		*self = NULL;
		return;
	}

	/* write header */
	s_ebml_write_header(*self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_init",
				  "Failed to write the header to the data source"))
	{
		/* release data source and header, caller will delete */
		S_DATAWRITER(self)->ds = NULL;
		(*self)->header = NULL;
		S_DELETE(*self, "write_init", error);
		*self = NULL;
		return;
	}
}


static void write_uint(SEbmlWrite *self, uint32 id, uint32 i, s_erc *error)
{
	uint32 size;
	s_byte *data;
	uint j;


	S_CLR_ERR(error);

	s_ebml_write_id(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_uint",
				  "Failed to write id"))
		return;

	if (i == 0) /* A zero byte integer represents the integer value 0. */
	{
		s_ebml_write_size(self, 0, error);
		S_CHK_ERR(error, S_CONTERR,
				  "write_uint",
				  "Failed to write size");

		return;
	}

	size = s_ebml_get_uint_size(i);
	if (size > self->header->max_size_width)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "write_uint",
				  "Uint size larger than max_size_width (header)");
		return;
	}

	s_ebml_write_size(self, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_uint",
				  "Failed to write size"))
		return;

	data = S_MALLOC(s_byte, size);
	if (data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "write_uint",
				  "Failed to allocate memory to data for writing");
		return;
	}

	/* little endian */
	for (j = 0; j < size; j++)
		data[j] = ((uint8*)&i)[j];

	data = s_swap_be_bytes(data, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_uint",
				  "Call to \"s_swap_be_bytes\" failed"))
	{
		S_FREE(data);
		return;
	}

	s_ebml_write_binary(self, data, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_uint",
				  "Call to \"s_ebml_write_binary\" failed"))
	{
		S_FREE(data);
		return;
	}

	S_FREE(data);
}


static void write_sint(SEbmlWrite *self, uint32 id, sint32 i, s_erc *error)
{
	uint32 size;
	uint32 udata;
	s_byte *data;
	uint j;


	S_CLR_ERR(error);

	s_ebml_write_id(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_sint",
				  "Failed to write id"))
		return;

	if (i == 0) /* A zero byte integer represents the integer value 0. */
	{
		s_ebml_write_size(self, 0, error);
		S_CHK_ERR(error, S_CONTERR,
				  "write_sint",
				  "Failed to write size");

		return;
	}

	size = s_ebml_get_sint_size(i);

	/* Signed integer, represented in two's complement notation */
	if (i >= 0)
	{
		udata = i;
	}
	else
	{
		if ((i >= INT_MAX) || (i < INT_MIN))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "write_sint",
					  "Sint is to large to be represented in two's complement");
			return;
		}

		udata = S_ABS(i);
		udata = ~udata;
		udata = udata + 1;
	}

	if (size > self->header->max_size_width)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "write_sint",
				  "Sint size larger than max_size_width (header)");
		return;
	}

	s_ebml_write_size(self, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_sint",
				  "Failed to write size"))
		return;

	data = S_MALLOC(s_byte, size);
	if (data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "write_sint",
				  "Failed to allocate memory to data for writing");
		return;
	}

	for (j = 0; j < size; j++)
		data[j] = ((uint8*)&udata)[j];

	data = s_swap_be_bytes(data, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_sint",
				  "Call to \"s_swap_be_bytes\" failed"))
	{
		S_FREE(data);
		return;
	}

	s_ebml_write_binary(self, data, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_sint",
				  "Call to \"s_ebml_write_binary\" failed"))
	{
		S_FREE(data);
		return;
	}

	S_FREE(data);
}


static void write_float(SEbmlWrite *self, uint32 id, float f, s_erc *error)
{
  	uint32 size;


	S_CLR_ERR(error);

	s_ebml_write_id(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_float",
				  "Failed to write id"))
		return;

	if (s_float_equal(f, 0.0) == TRUE)  /* A zero byte float represents the float value 0.0. */
	{
		s_ebml_write_size(self, 0, error);
		S_CHK_ERR(error, S_CONTERR,
				  "write_float",
				  "Failed to write size");

		return;
	}

	/*
	 * float size is expected to be 4 and tested
	 * in speect/engine/src/base/utils/types.h during compilation.
	 */
	size = 4;

 	s_ebml_write_size(self, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_float",
				  "Failed to write size"))
		return;

	s_float_write(S_DATAWRITER(self)->ds, f, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_float",
			  "Failed to write float to data source");
}


static void write_double(SEbmlWrite *self, uint32 id, double d, s_erc *error)
{
  	uint32 size;


	S_CLR_ERR(error);

	s_ebml_write_id(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_double",
				  "Failed to write id"))
		return;

	if (s_float_equal(d, 0.0) == TRUE)  /* A zero byte float represents the float value 0.0. */
	{
		s_ebml_write_size(self, 0, error);
		S_CHK_ERR(error, S_CONTERR,
				  "write_double",
				  "Failed to write size");

		return;
	}

	/*
	 * double size is expected to be 8 and tested
	 * in speect/engine/src/base/utils/types.h during compilation.
	 */
	size = 8;

 	s_ebml_write_size(self, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_double",
				  "Failed to write size"))
		return;

	s_double_write(S_DATAWRITER(self)->ds, d, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_double",
			  "Failed to write double to data source");
}


static void write_ascii(SEbmlWrite *self, uint32 id, const char *string, s_erc *error)
{
	uint32 size;


	S_CLR_ERR(error);

	s_ebml_write_id(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_ascii",
				  "Failed to write id"))
		return;

	if (string != NULL)
	{
		size = s_strsize(string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_ascii",
					  "Call to s_strsize failed"))
			return;
	}
	else
		size = 0;

	s_ebml_write_size(self, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_ascii",
				  "Failed to write size"))
		return;

	if (size == 0) /*  Note that a string can be of zero length. */
		return;

	SDatasourceWrite(S_DATAWRITER(self)->ds, (void*)string, sizeof(char), size, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_ascii",
			  "Failed to write string to data source");
}


static void write_utf8(SEbmlWrite *self, uint32 id, const char *string, s_erc *error)
{
	uint32 size;
	s_bool valid_utf8;


	S_CLR_ERR(error);

	if (string != NULL)
	{
		/* first check if it is a valid utf8 string */
		valid_utf8 = s_isvalid(string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_utf8",
					  "Call to s_isvalid failed"))
			return;

		if (valid_utf8 != TRUE)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "write_utf8",
					  "Given string is not valid UTF8");
			return;
		}
	}

 	s_ebml_write_id(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_utf8",
				  "Failed to write id"))
		return;

	if (string != NULL)
	{
		size = s_strsize(string, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_utf8",
					  "Call to s_strsize failed"))
			return;
	}
	else
		size = 0;

	s_ebml_write_size(self, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_utf8",
				  "Failed to write size"))
		return;

	if (size == 0) /*  Note that a string can be of zero length. */
		return;

	SDatasourceWrite(S_DATAWRITER(self)->ds, (void*)string, sizeof(char), size, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_utf8",
			  "Failed to write string to data source");
}


static void write_binary(SEbmlWrite *self, uint32 id, const s_byte *data, uint32 size, s_erc *error)
{
	S_CLR_ERR(error);

	s_ebml_write_id(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_binary",
				  "Failed to write id"))
		return;

	s_ebml_write_size(self, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_binary",
				  "Failed to write size"))
		return;

	if (size == 0)
		return;

	SDatasourceWrite(S_DATAWRITER(self)->ds, (void*)&data, sizeof(s_byte), size, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_binary",
			  "Failed to write string to data source");
}


static void write_object(SEbmlWrite *self, uint32 id,
						 const SObject *object, s_erc *error)
{
	const char *class_name;
	const SSerializedObject *formatter;


	S_CLR_ERR(error);

	formatter = SSerializedObjectGet(object, "spct_ebml", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_object",
				  "Call to \"SSerializedObjectGet\" failed"))
		return;

	if (formatter == NULL)
	{
		/* ignore this object, issue a warning */
		S_WARNING(S_WARNERR,
				  "write_object",
				  "No serialized object formatter for object of type '%s' and "
				  "format 'spct_ebml', object not written",
				  SObjectType(object, error));
		return;
	}

	class_name = s_class_name(object->cls, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_object",
				  "Failed to get object class name"))
		return;

	write_start_container(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_object",
				  "Failed to start object container"))
		return;

	write_utf8(self, S_EBML_ID_OBJECT_CLASSNAME, class_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_object",
				  "Failed to write object class name '%s'", class_name))
		return;

	/* give control to the object to write itself */
	SObjectWrite(object, formatter, S_DATAWRITER(self), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_object",
				  "Call to \"SObjectWrite\" failed"))
		return;

	write_stop_container(self, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_object",
			  "Failed to close object container");
}


static void write_start_container(SEbmlWrite *self, uint32 id, s_erc *error)
{
	long *master_start;


	S_CLR_ERR(error);

	s_ebml_write_id(self, id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_start_container",
				  "Failed to write id"))
		return;

	master_start = S_MALLOC(long, 1);
	if (master_start == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "write_start_container",
				  "Failed to allocate memory to object");
		return;
	}

	*master_start = SDatasourceTell(S_DATAWRITER(self)->ds, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_start_container",
				  "Call to SDatasourceTell failed"))
	{
		S_FREE(master_start);
		return;
	}

	s_ebml_write_size(self, S_EBML_MASTER_ELEMENT_DUMMY_SIZE, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_start_container",
				  "Failed to write size"))
		return;

	s_list_push(self->level, (void*)master_start, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_start_container",
			  "Failed to push container size into list");
}


static void write_stop_container(SEbmlWrite *self, s_erc *error)
{
	long *master_start = NULL;
	long size;
	long master_current;
	s_byte *data;
	int i;


	S_CLR_ERR(error);

	/* get starting position of this master */
	master_start = s_list_pop(self->level, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_stop_container",
				  "Call to \"s_list_pop\" failed"))
	{
		if (master_start != NULL)
			S_FREE(master_start);
		return;
	}

	if (master_start == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "write_stop_container",
				  "Unknown starting position of container");
		return;
	}

	/* current position */
	master_current = SDatasourceTell(S_DATAWRITER(self)->ds, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_stop_container",
				  "Call to \"SDatasourceTell\" failed"))
	{
		S_FREE(master_start);
		return;
	}

	/* get the size */
	size = master_current - (*master_start);

	/* goto starting position */
	SDatasourceSeek(S_DATAWRITER(self)->ds, (*master_start), S_SEEK_SET, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_stop_container",
				  "Call to \"SDatasourceSeek\" failed"))
	{
		S_FREE(master_start);
		return;
	}

	/*
	 * write the size, we cant use the normal routine because
	 * we didnt know the size beforehand. Thus, we must write the size
	 * with a width of 4.
	 */
	size = size | (16 << 24);

	data = S_MALLOC(s_byte, 4);
	if (data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "write_stop_container",
				  "Failed to allocate memory to object");
		S_FREE(master_start);
		return;
	}

	for (i = 0; i < 4; i++)
	{
		data[i] = size & 0xff;
		size >>= 8;
	}

	data = s_swap_be_bytes(data, 4, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_stop_container",
				  "Call to \"s_swap_be_bytes\" failed"))
	{
		S_FREE(data);
		S_FREE(master_start);
		return;
	}

	s_ebml_write_binary(self, data, 4, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_stop_container",
				  "Call to \"s_ebml_write_binary\" failed"))
	{
		S_FREE(data);
		S_FREE(master_start);
		return;
	}

	S_FREE(data);

	/* go back to current position */
	SDatasourceSeek(S_DATAWRITER(self)->ds, master_current, S_SEEK_SET, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_stop_container",
				  "Call to \"SDatasourceSeek\" failed"))
	{
		S_FREE(master_start);
		return;
	}

	/* free the starting point */
	S_FREE(master_start);
}


static long ebml_tell(SEbmlWrite *self, s_erc *error)
{
	long pos;


	S_CLR_ERR(error);
	pos = SDatasourceTell(S_DATAWRITER(self)->ds, error);
	S_CHK_ERR(error, S_CONTERR,
			  "tell",
			  "Call to \"SDatasourceTell\" failed");

	return pos;
}


static void s_ebml_write_header(SEbmlWrite *self, s_erc *error)
{
	S_CLR_ERR(error);

	write_start_container(self, S_EBML_ID_HEADER, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_ebml_write_header",
				  "Failed to start header container"))
		return;

	write_uint(self, S_EBML_ID_EBMLVERSION, self->header->ebml_version, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_ebml_write_header",
				  "Failed to write ebml version"))
		return;

	write_uint(self, S_EBML_ID_EBMLREADVERSION, self->header->ebml_read_version, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_ebml_write_header",
				  "Failed to write ebml reader version"))
		return;

	write_uint(self, S_EBML_ID_EBMLMAXIDLENGTH, self->header->max_id_width, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_ebml_write_header",
				  "Failed to write ebml max id length"))
		return;

	write_uint(self, S_EBML_ID_EBMLMAXSIZELENGTH, self->header->max_size_width, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_ebml_write_header",
				  "Failed to write ebml max size length"))
		return;

	write_ascii(self, S_EBML_ID_DOCTYPE, self->header->doctype, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_ebml_write_header",
				  "Failed to write ebml doctype"))
		return;

	write_uint(self, S_EBML_ID_DOCTYPEVERSION, self->header->doctype_version, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_ebml_write_header",
				  "Failed to write ebml doctype version"))
		return;

	write_uint(self, S_EBML_ID_DOCTYPEREADVERSION, self->header->doctype_read_version, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_ebml_write_header",
				  "Failed to write ebml doctype reader version"))
		return;

	write_stop_container(self, error);
	S_CHK_ERR(error, S_CONTERR,
			  "s_ebml_write_header",
			  "Failed to close header container");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitEbmlWrite(void *obj, s_erc *error)
{
	SEbmlWrite *self = obj;


	S_CLR_ERR(error);
	self->header = NULL;
	self->level = NULL;

	s_mutex_init(&(self->ebml_mutex));
}


static void DestroyEbmlWrite(void *obj, s_erc *error)
{
	SEbmlWrite *self = obj;


	S_CLR_ERR(error);

	s_mutex_lock(&(self->ebml_mutex));

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
				  "DestroyEbmlWrite",
				  "Failed to delete ebml container levels");
	}

	s_mutex_unlock(&(self->ebml_mutex));
 	s_mutex_destroy(&(self->ebml_mutex));
}


static void DisposeEbmlWrite(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void WriteInit(SEbmlWrite **self, SDatasource *ds,
					  s_ebml_header *header, s_erc *error)
{
	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteInit",
				  "Argument \"self\" is NULL");
		return;
	}

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteInit",
				  "Argument \"ds\" is NULL");
		return;
	}

	if (header == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteInit",
				  "Argument \"header\" is NULL");
		return;
	}

	s_mutex_lock(&((*self)->ebml_mutex));
	write_init(self, ds, header, error);
	s_mutex_unlock(&((*self)->ebml_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "WriteInit",
			  "Call to \"write_init\" failed");
}


static void WriteUint(SEbmlWrite *self, uint32 id,
					  uint32 i, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteUint",
				  "Argument \"self\" is NULL");
		return;
	}

	s_mutex_lock(&(self->ebml_mutex));
	write_uint(self, id, i, error);
	s_mutex_unlock(&(self->ebml_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "WriteUint",
			  "Call to \"write_uint\" failed");
}


static void WriteSint(SEbmlWrite *self, uint32 id,
					  sint32 i, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteSint",
				  "Argument \"self\" is NULL");
		return;
	}

	s_mutex_lock(&(self->ebml_mutex));
	write_sint(self, id, i, error);
	s_mutex_unlock(&(self->ebml_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "WriteSint",
			  "Call to \"write_sint\" failed");
}


static void WriteFloat(SEbmlWrite *self, uint32 id,
					   float f, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteFloat",
				  "Argument \"self\" is NULL");
		return;
	}

	s_mutex_lock(&(self->ebml_mutex));
	write_float(self, id, f, error);
	s_mutex_unlock(&(self->ebml_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "WriteFloat",
			  "Call to \"write_float\" failed");
}


static void WriteDouble(SEbmlWrite *self, uint32 id,
						double d, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteDouble",
				  "Argument \"self\" is NULL");
		return;
	}

	s_mutex_lock(&(self->ebml_mutex));
	write_double(self, id, d, error);
	s_mutex_unlock(&(self->ebml_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "WriteDouble",
			  "Call to \"write_double\" failed");
}


static void WriteAscii(SEbmlWrite *self, uint32 id,
					   const char *string, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteAscii",
				  "Argument \"self\" is NULL");
		return;
	}

	s_mutex_lock(&(self->ebml_mutex));
	write_ascii(self, id, string, error);
	s_mutex_unlock(&(self->ebml_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "WriteAscii",
			  "Call to \"write_ascii\" failed");
}


static void WriteUtf8(SEbmlWrite *self, uint32 id,
					  const char *string, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteUtf8",
				  "Argument \"self\" is NULL");
		return;
	}

	s_mutex_lock(&(self->ebml_mutex));
	write_utf8(self, id, string, error);
	s_mutex_unlock(&(self->ebml_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "WriteUtf8",
			  "Call to \"write_utf8\" failed");
}


static void WriteBinary(SEbmlWrite *self, uint32 id, const s_byte *data,
						uint32 size, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteBinary",
				  "Argument \"self\" is NULL");
		return;
	}

	if (data == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteBinary",
				  "Argument \"data\" is NULL");
		return;
	}

	s_mutex_lock(&(self->ebml_mutex));
	write_binary(self, id, data, size, error);
	s_mutex_unlock(&(self->ebml_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "WriteBinary",
			  "Call to \"write_binary\" failed");
}


static void WriteObject(SEbmlWrite *self, uint32 id,
						const SObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteObject",
				  "Argument \"self\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteObject",
				  "Argument \"object\" is NULL");
		return;
	}


	s_mutex_lock(&(self->ebml_mutex));
	write_object(self, id, object, error);
	s_mutex_unlock(&(self->ebml_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "WriteObject",
			  "Call to \"write_object\" failed");
}


static void WriteStartContainer(SEbmlWrite *self, uint32 id, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteStartContainer",
				  "Argument \"self\" is NULL");
		return;
	}

	s_mutex_lock(&(self->ebml_mutex));
	write_start_container(self, id, error);
	s_mutex_unlock(&(self->ebml_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "WriteStartContainer",
			  "Call to \"write_start_container\" failed");
}


static void WriteStopContainer(SEbmlWrite *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "WriteStopContainer",
				  "Argument \"self\" is NULL");
		return;
	}

	s_mutex_lock(&(self->ebml_mutex));
	write_stop_container(self, error);
	s_mutex_unlock(&(self->ebml_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "WriteStopContainer",
			  "Call to \"write_stop_container\" failed");
}

static long Tell(SEbmlWrite *self, s_erc *error)
{
	long pos;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "Tell",
				  "Argument \"self\" is NULL");
		return 0;
	}

	s_mutex_lock(&(self->ebml_mutex));
	pos = ebml_tell(self, error);
	s_mutex_unlock(&(self->ebml_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
				  "Tell",
				  "Call to \"tell\" failed"))
		return 0;

	return pos;
}


/************************************************************************************/
/*                                                                                  */
/* SEbmlWrite Class initialization                                                  */
/*                                                                                  */
/************************************************************************************/

static SEbmlWriteClass EbmlWriteClass =
{
	{
		/* SObjectClass */
		{
			"SDataWriter:SEbmlWrite",
			sizeof(SEbmlWrite),
			{ 0, 1},
			InitEbmlWrite,     /* init    */
			DestroyEbmlWrite,  /* destroy */
			DisposeEbmlWrite,  /* dispose */
			NULL,              /* compare */
			NULL,              /* print   */
			NULL,              /* copy    */
		},
		/* SDataWriter */
		/* nothing */
	},
	/* SEbmlWriteClass */
	WriteInit,                           /* write_init        */
	WriteUint,                           /* write_uint         */
	WriteSint,                           /* write_sint         */
	WriteFloat,                          /* write_float        */
	WriteDouble,                         /* write_double       */
	WriteAscii,                          /* write_ascii        */
	WriteUtf8,                           /* write_utf8         */
	WriteBinary,                         /* write_binary       */
	WriteObject,                         /* write_object       */
	WriteStartContainer,                 /* start_container    */
	WriteStopContainer,                  /* stop_container     */
   	Tell                                 /* tell               */
};
