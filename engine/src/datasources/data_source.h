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
/* AUTHOR  : Aby Louw                                                               */
/* DATE    : 10 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* An abstract data source class.                                                   */
/*                                                                                  */
/* Adapted from Agar toolkit http://libagar.org/ (1.3.2)                            */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_DATA_SOURCE_H__
#define _SPCT_DATA_SOURCE_H__


/************************************************************************************/
/* libagar license                                                                  */
/*                                                                                  */
/* Copyright (c) 2002-2007 Hypertriton, Inc. <http://hypertriton.com/>              */
/* All rights reserved.                                                             */
/*                                                                                  */
/* Redistribution and use in source and binary forms, with or without               */
/* modification, are permitted provided that the following conditions               */
/* are met:                                                                         */
/* 1. Redistributions of source code must retain the above copyright                */
/*    notice, this list of conditions and the following disclaimer.                 */
/* 2. Redistributions in binary form must reproduce the above copyright             */
/*    notice, this list of conditions and the following disclaimer in the           */
/*    documentation and/or other materials provided with the distribution.          */
/*                                                                                  */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"      */
/* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE        */
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE       */
/* ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR       */
/* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL           */
/* DAMAGES (INCLUDING BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR        */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER       */
/* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,    */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE        */
/* USE OF THIS SOFTWARE EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.          */
/*                                                                                  */
/************************************************************************************/


/**
 * @file data_source.h
 * Definition of data source classes.
 */


/**
 * @ingroup Speect
 * @defgroup SDatasource Data Sources
 * Definition of data source classes.
 * Provides a generic interface to different data sources, where a data
 * source can be anything that can be read from and/or written to.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "include/common.h"
#include "base/objsystem/objsystem.h"
#include "base/errdbg/errdbg.h"


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
 * @name Utility Macros
 * @{
 */


/**
 * @hideinitializer
 * Return the given #SDatasource child class object as a #SDatasource object.
 * @param SELF The given object.
 * @return Given object as #SDatasource* type.
 * @note This casting is not safety checked.
 */
#define S_DATASOURCE(SELF)    ((SDatasource *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SDatasource,
 * see full description #S_DATASOURCE_CALL for usage.
 * @param SELF The given #SDatasource*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage: @code S_DATASOURCE_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_DATASOURCE_CALL(SELF, FUNC)				\
	((SDatasourceClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SDatasource
 * can be called.
 * @param SELF The given #SDatasource*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_DATASOURCE_METH_VALID(SELF, FUNC)			\
	S_DATASOURCE_CALL(SELF, FUNC) ? TRUE : FALSE


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * An enum to define the byte order of a data source.
 */
typedef enum
{
	S_BYTEORDER_BE,    /*!< big endian byte order.    */
	S_BYTEORDER_LE     /*!< little endian byte order. */
} s_byte_order;


/**
 *  An enum to define the relative position of seek.
 */
typedef enum
{
	S_SEEK_SET = SEEK_SET,  /*!< Relative to beginning.        */
	S_SEEK_CUR = SEEK_CUR,  /*!< Relative to current position. */
	S_SEEK_END = SEEK_END  /*!< Relative to end.              */
} s_seek_mode;


/************************************************************************************/
/*                                                                                  */
/* SDatasource definition                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * The data source structure. Provides a generic interface to different
 * data sources, where a data source can be anything that can be read
 * from and/or written to. @ref SintIO and @ref SfloatIO
 * are also provided for data source I/O.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject        obj;

	/**
	 * @protected  Byte order of the source.
	 */
	s_byte_order   byte_order;

	/**
	 * @protected Last write count (bytes).
	 */
	size_t         wr_last;

	/**
	 * @protected Last read count (bytes).
	 */
	size_t         rd_last;

	/**
	 * @protected Total write count (bytes).
	 */
	size_t         wr_total;

	/**
	 * @protected Total read count (bytes).
	 */
	size_t         rd_total;

	/**
	 * @protected Locking mutex.
	 */
	S_DECLARE_MUTEX(data_mutex);
} SDatasource;


/************************************************************************************/
/*                                                                                  */
/* SDatasourceClass definition                                                      */
/*                                                                                  */
/************************************************************************************/

/**
 * The data source class structure.
 * @extends SObjectClass
 */
typedef struct SDatasourceClass_s
{
	/* Class members */
	/**
	 * @protected Inherit from #SObjectClass.
	 */
	SObjectClass  _inherit;

	/* Class methods */
	/**
	 * @protected Read function pointer.
	 * Read from a data source.
	 * Reads @a nmemb items of @a size bytes from the data source into a buffer.
	 * @param self The data source to read from.
	 * @param ptr The buffer to read the data into.
	 * @param size The size of the items.
	 * @param nmemb Number of items to read.
	 * @param error Error code.
	 * @return The number of bytes read.
	 */
	size_t (*read)     (SDatasource *self, void *ptr, size_t size,
						size_t nmemb, s_erc *error);

	/**
	 * @protected ReadAt function pointer.
	 * Read from a data source starting at a given offset position.
	 * Reads @a nmemb items of @a size bytes from the data source into a buffer
	 * starting at @a pos.
	 * @param self The data source to read from.
	 * @param ptr The buffer to read the data into.
	 * @param size The size of the items.
	 * @param nmemb Number of items to read.
	 * @param pos The offset to start reading from.
	 * @param error Error code.
	 * @return The number of bytes read.
	 */
	size_t (*read_at)  (SDatasource *self, void *ptr, size_t size,
						size_t nmemb, long pos, s_erc *error);

	/**
	 * @protected Write function pointer.
	 * Write to a data source.
	 * Writes @a nmemb items of @a size bytes from a buffer into the data source.
	 * @param self The data source to write from.
	 * @param ptr The buffer containing the data.
	 * @param size The size of the items.
	 * @param nmemb Number of items to write.
	 * @param error Error code.
	 * @return The number of bytes written.
	 */
	size_t (*write)    (SDatasource *self, const void *ptr, size_t size,
						size_t nmemb, s_erc *error);

	/**
	 * @protected WriteAt function pointer.
	 * Write to a data source starting at a given offset position.
	 * Writes @a nmemb items of @a size bytes from a buffer into the data source starting at @a pos.
	 * @param self The data source to write from.
	 * @param ptr The buffer containing the data.
	 * @param size The size of the items.
	 * @param nmemb Number of items to write.
	 * @param pos The offset to start writing from.
	 * @param error Error code.
	 * @return The number of bytes written.
	 */

	size_t (*write_at) (SDatasource *self, const void *ptr, size_t size,
						size_t nmemb, long pos, s_erc *error);

	/**
	 * @protected Tell function pointer.
	 * Returns the current position in the data source.
	 * If the underlying data source does not support this operation, a value of -1 is returned.
	 * @param self The data source.
	 * @param error Error code.
	 * @return The offset position.
	 */
	long  (*tell)      (SDatasource *self, s_erc *error);

	/**
	 * @protected Seek function pointer.
	 * Seeks to the given position in the data source, where mode is one of:
	 * #s_seek_mode.
	 * @param self The data source.
	 * @param pos The position.
	 * @param mode The position relative to #s_seek_mode.
	 * @param error Error code.
	 */
	void  (*seek)      (SDatasource *self, long pos, s_seek_mode mode, s_erc *error);
} SDatasourceClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * @name Read
 * @{
 */


/**
 * Read from a data source.
 * Reads @a nmemb items of @a size bytes from the data source into a
 * buffer.
 * @public @memberof SDatasource

 * @param self The data source to read from.
 * @param ptr The buffer to read the data into.
 * @param size The size of the items.
 * @param nmemb Number of items to read.
 * @param error Error code.
 *
 * @return The number of bytes read.
 *
 * @note Can set @c S_IOEOF (end-of-file) error, which must be interpreted by the
 * caller if it is an error or not.
 */
S_API size_t SDatasourceRead(SDatasource *self, void *ptr, size_t size,
							 size_t nmemb, s_erc *error);


/**
 * Read from a data source starting at a given offset position.
 * Reads @a nmemb items of @a size bytes from the data source into a
 * buffer starting at @a pos.
 * @public @memberof SDatasource
 *
 * @param self The data source to read from.
 * @param ptr The buffer to read the data into.
 * @param size The size of the items.
 * @param nmemb Number of items to read.
 * @param pos The offset to start reading from.
 * @param error Error code.
 *
 * @return The number of bytes read.
 *
 * @note Can set @c S_IOEOF (end-of-file) error, which must be interpreted by the
 * caller if it is an error or not.
 */
S_API size_t SDatasourceReadAt(SDatasource *self, void *ptr, size_t size,
							   size_t nmemb, long pos, s_erc *error);


/**
 * @}
 */


/**
 * @name Write
 * @{
 */


/**
 * Write to a data source.
 * Writes @a nmemb items of @a size bytes from a buffer into the data source.
 * @public @memberof SDatasource
 * @param self The data source to write from.
 * @param ptr The buffer containing the data.
 * @param size The size of the items.
 * @param nmemb Number of items to write.
 * @param error Error code.
 * @return The number of bytes written.
 */
S_API size_t SDatasourceWrite(SDatasource *self, const void *ptr, size_t size,
							  size_t nmemb, s_erc *error);


/**
 * Write to a data source starting at a given offset position.
 * Writes @a nmemb items of @a size bytes from a buffer into the data source starting at @a pos.
 * @public @memberof SDatasource
 * @param self The data source to write from.
 * @param ptr The buffer containing the data.
 * @param size The size of the items.
 * @param nmemb Number of items to write.
 * @param pos The offset to start writing from.
 * @param error Error code.
 * @return The number of bytes written.
 */
S_API size_t SDatasourceWriteAt(SDatasource *self, const void *ptr, size_t size,
								size_t nmemb, long pos, s_erc *error);


/**
 * @}
 */


/**
 * @name Position/Byte Order
 * @{
 */


/**
 * Returns the current position in the data source.
 * If the underlying data source does not support this operation, a
 * value of -1 is returned.
 * @public @memberof SDatasource
 *
 * @param self The data source.
 * @param error Error code.
 * @return The offset position.
 */
S_API long SDatasourceTell(SDatasource *self, s_erc *error);


/**
 * Seeks to the given position in the data source, where mode is one of:
 * #s_seek_mode.
 * @public @memberof SDatasource
 * @param self The data source.
 * @param pos The position.
 * @param mode The position relative to #s_seek_mode.
 * @param error Error code.
 */
S_API void SDatasourceSeek(SDatasource *self, long pos, s_seek_mode mode, s_erc *error);


/**
 * Configures the byte order to be used by integer read/write operations.
 * Order may be one of #s_byte_order.
 * @public @memberof SDatasource
 * @param self The data source.
 * @param order The order of the source, @sa s_byte_order.
 * @param error Error code.
 * @note At initialization of any object derived from SDatasource the byte
 * order is set to @b big @b endian.
 */
S_API void SDatasourceSetByteOrder(SDatasource *self, s_byte_order order, s_erc *error);


/**
 * @}
 */


/**
 * Add the SDatasource class to the object system.
 * @private @memberof SDatasource
 * @param error Error code.
 */
S_LOCAL void _s_data_source_class_add(s_erc *error);


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

#endif /* _SPCT_DATA_SOURCE_H__ */

