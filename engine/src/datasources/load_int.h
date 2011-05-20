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
/* AUTHOR  : Aby Louw                                                               */
/* DATE    : 15 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Integer I/O routines.                                                            */
/*                                                                                  */
/* Adapted from Agar toolkit http://libagar.org/ (1.3.2)                            */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LOAD_INT_H__
#define _SPCT_LOAD_INT_H__


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
 * @file load_int.h
 * Integer I/O.
 */


/**
 * @ingroup SDatasource
 * @defgroup SintIO Integer I/O Functions
 * Data source integer I/O.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
#include "base/errdbg/errdbg.h"
#include "datasources/data_source.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/


/**
 * @name uint8 Read/Write
 * @{
 */


/**
 * Read a uint8 value from a data source.
 * @param ds The datasource form which to read the uint8 value.
 * @param error Error code.
 * @return The read uint8 value.
 */
S_API uint8 s_uint8_read(SDatasource *ds, s_erc *error);


/**
 * Read a uint8 value from a data source starting at a given offset position.
 * @param ds The datasource form which to read the uint8 value.
 * @param pos The offset to start reading from.
 * @param error Error code.
 * @return The read uint8 value.
 */
S_API uint8 s_uint8_read_at(SDatasource *ds, long pos, s_erc *error);


/**
 * Write a uint8 value to a data source.
 * @param ds The datasource which to write the uint8 value to.
 * @param i The uint8 value to write.
 * @param error Error code.
 */
S_API void s_uint8_write(SDatasource *ds, uint8 i, s_erc *error);


/**
 * Write a uint8 value to a data source with an offset.
 * @param ds The datasource which to write the uint8 value to.
 * @param i The uint8 value to write.
 * @param pos The offset.
 * @param error Error code.
 */
S_API void s_uint8_write_at(SDatasource *ds, uint8 i, long pos, s_erc *error);


/**
 * @}
 */


/**
 * @name uint16 Read/Write
 * @{
 */


/**
 * Read a uint16 value from a datasource.
 * @param ds The datasource form which to read the uint16 value.
 * @param error Error code.
 * @return The read uint16 value.
 */
S_API uint16 s_uint16_read(SDatasource *ds, s_erc *error);


/**
 * Read a uint16 value from a datasource starting at a given offset position.
 * @param ds The datasource form which to read the uint16 value.
 * @param pos The offset to start reading from.
 * @param error Error code.
 * @return The read uint16 value.
 */
S_API uint16 s_uint16_read_at(SDatasource *ds, long pos, s_erc *error);


/**
 * Write a uint16 value to a datasource.
 * @param ds The datasource which to write the uint16 value to.
 * @param i The uint16 value to write.
 * @param error Error code.
 */
S_API void s_write_uint16(SDatasource *ds, uint16 i, s_erc *error);


/**
 * Write a uint16 value to a datasource with an offset.
 * @param ds The datasource which to write the uint16 value to.
 * @param i The uint16 value to write.
 * @param pos The offset.
 * @param error Error code.
 */
S_API void s_write_uint16_at(SDatasource *ds, uint16 i, long pos, s_erc *error);


/**
 * @}
 */


/**
 * @name uint32 Read/Write
 * @{
 */


/**
 * Read a uint32 value from a datasource.
 * @param ds The datasource form which to read the uint32 value.
 * @param error Error code.
 * @return The read uint32 value.
 */
S_API uint32 s_uint32_read(SDatasource *ds, s_erc *error);


/**
 * Read a uint32 value from a datasource starting at a given offset position.
 * @param ds The datasource form which to read the uint32 value.
 * @param pos The offset to start reading from.
 * @param error Error code.
 * @return The read uint32 value.
 */
S_API uint32 s_uint32_read_at(SDatasource *ds, long pos, s_erc *error);


/**
 * Write a uint32 value to a datasource.
 * @param ds The datasource which to write the uint32 value to.
 * @param i The uint32 value to write.
 * @param error Error code.
 */
S_API void s_write_uint32(SDatasource *ds, uint32 i, s_erc *error);


/**
 * Write a uint32 value to a datasource with an offset.
 * @param ds The datasource which to write the uint32 value to.
 * @param i The uint32 value to write.
 * @param pos The offset.
 * @param error Error code.
 */
S_API void s_write_uint32_at(SDatasource *ds, uint32 i, long pos, s_erc *error);


/**
 * @}
 */


/**
 * @name sint8 Read/Write
 * @{
 */


/**
 * Read a sint8 value from a data source.
 * @param ds The datasource form which to read the sint8 value.
 * @param error Error code.
 * @return The read sint8 value.
 */
S_API sint8 s_sint8_read(SDatasource *ds, s_erc *error);


/**
 * Read a sint8 value from a data source starting at a given offset position.
 * @param ds The datasource form which to read the sint8 value.
 * @param pos The offset to start reading from.
 * @param error Error code.
 * @return The read sint8 value.
 */
S_API sint8 s_sint8_read_at(SDatasource *ds, long pos, s_erc *error);


/**
 * Write a sint8 value to a data source.
 * @param ds The datasource which to write the sint8 value to.
 * @param i The sint8 value to write.
 * @param error Error code.
 */
S_API void s_sint8_write(SDatasource *ds, sint8 i, s_erc *error);


/**
 * Write a sint8 value to a data source with an offset.
 * @param ds The datasource which to write the sint8 value to.
 * @param i The sint8 value to write.
 * @param pos The offset.
 * @param error Error code.
 */
S_API void s_sint8_write_at(SDatasource *ds, sint8 i, long pos, s_erc *error);


/**
 * @}
 */


/**
 * @name sint16 Read/Write
 * @{
 */


/**
 * Read a sint16 value from a datasource.
 * @param ds The datasource form which to read the sint16 value.
 * @param error Error code.
 * @return The read sint16 value.
 */
S_API sint16 s_sint16_read(SDatasource *ds, s_erc *error);


/**
 * Read a sint16 value from a datasource starting at a given offset position.
 * @param ds The datasource form which to read the sint16 value.
 * @param pos The offset to start reading from.
 * @param error Error code.
 * @return The read sint16 value.
 */
S_API sint16 s_sint16_read_at(SDatasource *ds, long pos, s_erc *error);


/**
 * Write a sint16 value to a datasource.
 * @param ds The datasource which to write the sint16 value to.
 * @param i The sint16 value to write.
 * @param error Error code.
 */
S_API void s_write_sint16(SDatasource *ds, sint16 i, s_erc *error);


/**
 * Write a sint16 value to a datasource with an offset.
 * @param ds The datasource which to write the sint16 value to.
 * @param i The sint16 value to write.
 * @param pos The offset.
 * @param error Error code.
 */
S_API void s_write_sint16_at(SDatasource *ds, sint16 i, long pos, s_erc *error);


/**
 * @}
 */


/**
 * @name sint32 Read/Write
 * @{
 */


/**
 * Read a sint32 value from a datasource.
 * @param ds The datasource form which to read the sint32 value.
 * @param error Error code.
 * @return The read sint32 value.
 */
S_API sint32 s_sint32_read(SDatasource *ds, s_erc *error);


/**
 * Read a sint32 value from a datasource starting at a given offset position.
 * @param ds The datasource form which to read the sint32 value.
 * @param pos The offset to start reading from.
 * @param error Error code.
 * @return The read sint32 value.
 */
S_API sint32 s_sint32_read_at(SDatasource *ds, long pos, s_erc *error);


/**
 * Write a sint32 value to a datasource.
 * @param ds The datasource which to write the sint32 value to.
 * @param i The sint32 value to write.
 * @param error Error code.
 */
S_API void s_write_sint32(SDatasource *ds, sint32 i, s_erc *error);


/**
 * Write a sint32 value to a datasource with an offset.
 * @param ds The datasource which to write the sint32 value to.
 * @param i The sint32 value to write.
 * @param pos The offset.
 * @param error Error code.
 */
S_API void s_write_sint32_at(SDatasource *ds, sint32 i, long pos, s_erc *error);


/**
 * @}
 */


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

#endif /* _SPCT_LOAD_INT_H__ */


