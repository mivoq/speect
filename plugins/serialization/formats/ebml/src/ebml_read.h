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
/* DATE    : 20 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* EBML data source reader class.                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_EBML_READ_H__
#define _SPCT_EBML_READ_H__


/**
 * @file ebml_read.h
 * EBML data reader plug-in.
 */


/**
 * @ingroup SEbml
 * @defgroup SEbmlReader Reader
 * Plug-in class for reading data from an EBML format file.
 * @{
 * @example example_read.c
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include "speect.h"
#include "ebml_id.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given #SEbmlRead child/parent class object as a
 * #SEbmlRead object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SEbmlRead* type.
 * @note This casting is not safety checked.
 */
#define S_EBMLREAD(SELF)    ((SEbmlRead *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SEbmlRead,
 * see full description #S_EBMLREAD_CALL for usage.
 *
 * @param SELF The given #SEbmlRead*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_EBMLREAD_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_EBMLREAD_CALL(SELF, FUNC)					\
	((SEbmlReadClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SEbmlRead
 * can be called.
 *
 * @param SELF The given #SEbmlRead*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_EBMLREAD_METH_VALID(SELF, FUNC)			\
	S_EBMLREAD_CALL(SELF, FUNC) ? TRUE : FALSE


/**
 * @hideinitializer
 * Check if the read id of an EBML element is what was
 * expected. If not, a context error is set and code execution is
 * jumped to the "quit_error" label.
 * The context error message is printed in the format:
 * @code ("%s, read id 0x%x, want id 0x%x", ERROR_MSG, READ_ID, WANT_ID) @endcode
 *
 * @param READ_ID The ebml element id which was read.
 * @param WANT_ID The ebml element id which was expected.
 * @param FUNC The function name string (used for the context error).
 * @param ERROR_MSG The error message string to set.
 * @param ERROR The error variable pointer.
 *
 * @note Define "quit_error" label to clean up allocated memory and
 * return.
 */
#define S_EBML_ID_SANITY(READ_ID, WANT_ID, FUNC, ERROR_MSG, ERROR)	\
	do{															\
		if (READ_ID != WANT_ID)									\
		{														\
			S_CTX_ERR(ERROR, S_FAILURE,							\
					  FUNC,										\
					  "%s, read id '0x%x', want id '0x%x'",		\
					  ERROR_MSG, READ_ID, WANT_ID);				\
			goto quit_error;									\
		}														\
	} while (0)


/************************************************************************************/
/*                                                                                  */
/* SEbmlRead definition                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * The ebml data source reader structure.
 * @extends SDataReader
 */
typedef struct
{
	/**
	 * @protected Inherit from #SDataReader.
	 */
	SDataReader    obj;

	/**
	 * @protected EBML header information.
	 */
	s_ebml_header *header;

	/**
	 * @protected EBML element level container.
	 */
	s_list        *level;

	/* no need for mutex as data source (in SDataReader) has one */
} SEbmlRead;


/************************************************************************************/
/*                                                                                  */
/* SEbmlReadClass definition                                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * The ebml data source reader class structure.
 * @extends SDataReaderClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SDataReaderClass.
	 */
	SDataReaderClass  _inherit;

	/* Class methods */
	/**
	 * @protected ReadInit function pointer.
	 * Initialize an Ebml reader with a data source. The ebml header
	 * is read and the #SEbmlRead::header structure is filled.
	 *
	 * @param self The Ebml reader to initialize.
	 * @param ds The ebml data source.
	 * @param error Error code.
	 *
	 * @note The Ebml reader takes hold of the data source (on
	 * successful initialization), and it is deleted when the Ebml
	 * reader is deleted.
	 * @note If this function fails the Ebml reader will be deleted
	 * and the @c self pointer will be set to @c NULL. The data source
	 * must then be deleted by the caller.
	 */
	void     (*read_init)        (SEbmlRead **self, SDatasource *ds, s_erc *error);

	/**
	 * @protected ReadID function pointer.
	 * Read the ID of the next element in the ebml data source.
	 *
	 * @param self The ebml data source reader.
	 * @param error Error code.
	 *
	 * @return The read ID.
	 */
	uint32   (*read_id)          (SEbmlRead *self, s_erc *error);

	/**
	 * @protected PeekID function pointer.
	 * Peek the ID of the next element in the ebml data source.
	 *
	 * @param self The ebml data source reader.
	 * @param error Error code.
	 *
	 * @return The peeked ID.
	 */
	uint32   (*peek_id)          (SEbmlRead *self, s_erc *error);

	/**
	 * @protected Seek function pointer.
	 * Seeks to the given position in the ebml data source, where mode is one of:
	 * #s_seek_mode.
	 *
	 * @param self The ebml data source reader.
	 * @param pos The position.
	 * @param mode The position relative to #s_seek_mode.
	 * @param error Error code.
	 */
	void     (*seek)             (SEbmlRead *self, long pos, s_seek_mode mode, s_erc *error);

	/**
	 * @protected ElementSize function pointer.
	 * Get the size of the current ebml element in bytes.
	 *
	 * @param self The ebml data source reader.
	 * @param error Error code.
	 *
	 * @return The current element size in bytes. If -1 then element has an
	 * unknown size (stream data), see #S_EBML_SIZE_UNKNOWN.
	 */
	uint32   (*element_size)     (SEbmlRead *self, s_erc *error);

	/**
	 * @protected ElementSkip function pointer.
	 * Skip the current ebml element.
	 *
	 * @param self The ebml data source reader.
	 * @param error Error code.
	 */
	void     (*element_skip)     (SEbmlRead *self, s_erc *error);

	/**
	 * @protected ReadUint function pointer.
	 * Read current element as an unsigned integer.
	 *
	 * @param self The ebml data source reader.
	 * @param id The ID of the unsigned integer element.
	 * @param error Error code.
	 *
	 * @return Unsigned integer data.
	 * @todo check spct_swap_be32
	 */
	uint32   (*read_uint)        (SEbmlRead *self, uint32 *id, s_erc *error);

	/**
	 * @protected ReadSint function pointer.
	 * Read current element as an signed integer.
	 *
	 * @param self The ebml data source reader.
	 * @param id The ID of the signed integer element.
	 * @param error Error code.
	 *
	 * @return Signed integer data.
	 */
	sint32   (*read_sint)        (SEbmlRead *self, uint32 *id, s_erc *error);

	/**
	 * @protected ReadFloat function pointer.
	 * Read current element as a float.
	 *
	 * @param self The ebml data source reader.
	 * @param id The ID of the float element.
	 * @param error Error code.
	 *
	 * @return Float data.
	 */
	float    (*read_float)       (SEbmlRead *self, uint32 *id, s_erc *error);

	/**
	 * @protected ReadDouble function pointer.
	 * Read current element as a double.
	 *
	 * @param self The ebml data source reader.
	 * @param id The ID of the double element.
	 * @param error Error code.
	 *
	 * @return Double data.
	 */
	double   (*read_double)      (SEbmlRead *self, uint32 *id, s_erc *error);

	/**
	 * @protected ReadAscii function pointer.
	 * Read current element as an ascii string.
	 *
	 * @param self The ebml data source reader.
	 * @param id The ID of the ascii string element.
	 * @param error Error code.
	 *
	 * @return Ascii data.
	 */
	char    *(*read_ascii)       (SEbmlRead *self, uint32 *id, s_erc *error);

	/**
	 * @protected ReadUTF8 function pointer.
	 * Read current element as an utf8 string.
	 *
	 * @param self The ebml data source reader.
	 * @param id The ID of the utf8 string element.
	 * @param error Error code.
	 *
	 * @return Utf8 data.
	 */
	char    *(*read_utf8)        (SEbmlRead *self, uint32 *id, s_erc *error);

	/**
	 * @protected ReadBinary function pointer.
	 * Read current element as binary data (not to be interpreted).
	 *
	 * @param self The ebml data source reader.
	 * @param id The ID of the binary element.
	 * @param size The size of the binary element.
	 * @param error Error code.
	 *
	 * @return Binary data.
	 */
	s_byte  *(*read_binary)      (SEbmlRead *self, uint32 *id, uint32 *size, s_erc *error);

	/**
	 * @protected ReadObject function pointer.
	 * Read current element as an #SObject.
	 *
	 * @param self The ebml data source reader.
	 * @param id The ID of the #SObject element.
	 * @param error Error code.
	 *
	 * @return #SObject.
	 */
	SObject *(*read_object)      (SEbmlRead *self, uint32 *id, s_erc *error);

	/**
	 * @protected Container function pointer.
	 * Read current element as a container.
	 *
	 * @param self The ebml data source reader.
	 * @param error Error code.
	 *
	 * @return The ID of the container element.
	 */
	uint32   (*container)   (SEbmlRead *self, s_erc *error);

	/**
	 * @protected ContainerAtEnd function pointer.
	 * Query if the current opened container is at it's end.
	 *
	 * @param self The ebml data source reader.
	 * @param error Error code.
	 *
	 * @return #TRUE or #FALSE.
	 */
	s_bool   (*container_at_end) (SEbmlRead *self, s_erc *error);
} SEbmlReadClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SEbmlRead plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_ebml_read_class_reg(s_erc *error);


/**
 * Free the #SEbmlRead plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_ebml_read_class_free(s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


/**
 * @}
 * end documentation
 */

#endif /* _SPCT_EBML_READ_H__ */

