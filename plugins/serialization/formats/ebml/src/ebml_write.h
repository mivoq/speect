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

#ifndef _SPCT_EBML_WRITE_H__
#define _SPCT_EBML_WRITE_H__


/**
 * @file ebml_write.h
 * EBML data writer plug-in.
 */


/**
 * @ingroup SEbml
 * @defgroup SEbmlWriter Writer
 * Plug-in class for writing data to an EBML format file.
 * @{
 * @example example_write.c
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

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
 * Return the given #SEbmlWrite child/parent class object as a
 * #SEbmlWrite object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SEbmlWrite* type.
 * @note This casting is not safety checked.
 */
#define S_EBMLWRITE(SELF)    ((SEbmlWrite *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SEbmlWrite,
 * see full description #S_EBMLWRITE_CALL for usage.
 *
 * @param SELF The given #SEbmlWrite*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_EBMLWRITE_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_EBMLWRITE_CALL(SELF, FUNC)				\
	((SEbmlWriteClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SEbmlWrite
 * can be called.
 *
 * @param SELF The given #SEbmlWrite*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_EBMLWRITE_METH_VALID(SELF, FUNC)			\
	S_EBMLWRITE_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* SEbmlWrite definition                                                            */
/*                                                                                  */
/************************************************************************************/

/**
 * The ebml data source writer structure.
 * @extends SDataWriter
 */
typedef struct
{
	/**
	 * @protected Inherit from #SDataWriter.
	 */
	SDataWriter    obj;

	/**
	 * @protected EBML header information.
	 */
	s_ebml_header *header;

	/**
	 * @protected EBML element level container.
	 */
	s_list        *level;

	/**
	 * @protected Locking mutex
	 */
	S_DECLARE_MUTEX(ebml_mutex);
} SEbmlWrite;


/************************************************************************************/
/*                                                                                  */
/* SEbmlWriteClass definition                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The ebml data source writer class structure.
 * @extends SDataWriterClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SDataWriterClass.
	 */
	SDataWriterClass  _inherit;

	/* Class methods */
	/**
	 * @protected WriteInit function pointer.
	 * Initialize an Ebml writer with a data source and a populated header
	 * structure. The header will be written to the data source.
	 *
	 * @param self The Ebml writer to initialize.
	 * @param ds The ebml data source.
	 * @param header A pointer to a header data structure (must be
	 * initialized with data).
	 * @param error Error code.
	 *
	 * @note The Ebml writer takes hold of the header data structure
	 * (on successful initialization), and it is freed (by the Ebml
	 * writer) when the Ebml writer is deleted.
	 * @note The Ebml writer takes hold of the data source (on
	 * successful initialization), and it is deleted when the Ebml
	 * writer is deleted.
	 * @note If this function fails the Ebml writer will be deleted
	 * and the @c self pointer will be set to @c NULL. The data source
	 * must then be deleted by the caller, and the header freed by the
	 * caller.
	 */
	void (*write_init)      (SEbmlWrite **self, SDatasource *ds,
							 s_ebml_header *header, s_erc *error);


	/**
	 * @protected WriteUint function pointer.
	 * Write an unsigned integer to the ebml data source.
	 *
	 * @param self The ebml data source writer.
	 * @param id The ID of the unsigned integer element.
	 * @param i The unsigned integer to write.
	 * @param error Error code.
	 */
	void (*write_uint)      (SEbmlWrite *self, uint32 id, uint32 i, s_erc *error);

	/**
	 * @protected WriteSint function pointer.
	 * Write a signed integer to the ebml data source.
	 *
	 * @param self The ebml data source writer.
	 * @param id The ID of the signed integer element.
	 * @param i The signed integer to write.
	 * @param error Error code.
	 */
	void (*write_sint)      (SEbmlWrite *self, uint32 id, sint32 i, s_erc *error);

	/**
	 * @protected WriteFloat function pointer.
	 * Write a float to the ebml data source.
	 *
	 * @param self The ebml data source writer.
	 * @param id The ID of the float element.
	 * @param f The float to write.
	 * @param error Error code.
	 */
	void (*write_float)     (SEbmlWrite *self, uint32 id, float f, s_erc *error);

	/**
	 * @protected WriteDouble function pointer.
	 * Write a double to the ebml data source.
	 *
	 * @param self The ebml data source writer.
	 * @param id The ID of the double element.
	 * @param d The double to write.
	 * @param error Error code.
	 */
	void (*write_double)    (SEbmlWrite *self, uint32 id, double d, s_erc *error);

	/**
	 * @protected WriteAscii function pointer.
	 * Write an ascii string to the ebml data source.
	 *
	 * @param self The ebml data source writer.
	 * @param id The ID of the ascii string element.
	 * @param string The ascii string to write.
	 * @param error Error code.
	 *
	 * @note It is possible to write a @c NULL string.
	 */
	void (*write_ascii)     (SEbmlWrite *self, uint32 id, const char *string, s_erc *error);

	/**
	 * @protected WriteUTF8 function pointer.
	 * Write an utf8 string to the ebml data source.
	 *
	 * @param self The ebml data source writer.
	 * @param id The ID of the utf8 string element.
	 * @param string The utf8 string to write.
	 * @param error Error code.
	 *
	 * @note It is possible to write a @c NULL string.
	 */
	void (*write_utf8)      (SEbmlWrite *self, uint32 id, const char *string, s_erc *error);

	/**
	 * @protected WriteBinary function pointer.
	 * Write binary data to the ebml data source.
	 *
	 * @param self The ebml data source writer.
	 * @param id The ID of the binary data element.
	 * @param data The binary data to write.
	 * @param size The size of the data in bytes.
	 * @param error Error code.
	 */
	void (*write_binary)    (SEbmlWrite *self, uint32 id, const s_byte *data,
							 uint32 size, s_erc *error);

	/**
	 * @protected WriteObject function pointer.
	 * Write an SObject object to the ebml data source.
	 *
	 * @param self The ebml data source writer.
	 * @param id The ID of the SObject object element.
	 * @param object The SObject object to write.
	 * @param error Error code.
	 *
	 * @note The @c object must have an appropriate #SSerializedObject
	 * defined and registered (#SSerializedObjectRegister) for the
	 * "spct_ebml" format. If not #SSerializedObject has been
	 * registered for the given @c object, then it will not be written
	 * and the function will return (a warning will be written to
	 * log). Test if object is writable with #SObjectIsWritable.
	 */
	void (*write_object)    (SEbmlWrite *self, uint32 id,
							 const SObject *object, s_erc *error);

	/**
	 * @protected StartContainer function pointer.
	 * Start a container element.
	 *
	 * @param self The ebml data source writer.
	 * @param id The ID of the container element.
	 * @param error Error code.
	 */
	void (*start_container) (SEbmlWrite *self, uint32 id, s_erc *error);

	/**
	 * @protected StopContainer function pointer.
	 * Stop a container element.
	 *
	 * @param self The ebml data source writer.
	 * @param error Error code.
	 */
	void (*stop_container)  (SEbmlWrite *self, s_erc *error);

	/**
	 * @protected Tell function pointer.
	 * Returns the current position in the ebml data source.
	 *
	 * @param self The ebml data source writer.
	 * @param error Error code.
	 * @return The offset position from the beginning of the file.
	 */
	long (*tell)            (SEbmlWrite *self, s_erc *error);
} SEbmlWriteClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SEbmlWrite plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_ebml_write_class_reg(s_erc *error);

/**
 * Free the #SEbmlWrite plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_ebml_write_class_free(s_erc *error);


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

#endif /* _S_EBML_WRITE_H__ */

