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
/* DATE    : 17 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Float I/O routines.                                                              */
/*                                                                                  */
/* Adapted from Agar toolkit http://libagar.org/ (1.3.2)                            */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LOAD_REAL_H__
#define _SPCT_LOAD_REAL_H__


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
 * @file load_real.h
 * Floating point I/O.
 */


/**
 * @ingroup SDatasource
 * @defgroup SfloatIO Floating Point I/O Functions
 * Data source floating point I/O.
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
 * @name float Read/Write
 * @{
 */


/**
 * Read a float value from a datasource.
 * @param ds The datasource form which to read the float value.
 * @param error Error code.
 * @return The read float value.
 */
S_API float s_float_read(SDatasource *ds, s_erc *error);


/**
 * Read a float value from a datasource starting at a given offset position.
 * @param ds The datasource form which to read the float value.
 * @param pos The offset to start reading from.
 * @param error Error code.
 * @return The read float value.
 */
S_API float s_float_read_at(SDatasource *ds, long pos, s_erc *error);


/**
 * Write a float value to a data source.
 * @param ds The datasource which to write the float value to.
 * @param f The float value to write.
 * @param error Error code.
 */
S_API void s_float_write(SDatasource *ds, float f, s_erc *error);


/**
 * Write a float value to a datasource with an offset.
 * @param ds The datasource which to write the float value to.
 * @param f The float value to write.
 * @param pos The offset.
 * @param error Error code.
 */
S_API void s_float_write_at(SDatasource *ds, float f, long pos, s_erc *error);


/**
 * @}
 */


/**
 * @name double Read/Write
 * @{
 */


/**
 * Read a double value from a datasource.
 * @param ds The datasource form which to read the double value.
 * @param error Error code.
 * @return The read double value.
 */
S_API double s_double_read(SDatasource *ds, s_erc *error);


/**
 * Read a double value from a datasource starting at a given offset position.
 * @param ds The datasource form which to read the double value.
 * @param pos The offset to start reading from.
 * @param error Error code.
 * @return The read double value.
 */
S_API double s_double_read_at(SDatasource *ds, long pos, s_erc *error);


/**
 * Write a double value to a data source.
 * @param ds The datasource which to write the double value to.
 * @param d The double value to write.
 * @param error Error code.
 */
S_API void s_double_write(SDatasource *ds, double d, s_erc *error);


/**
 * Write a double value to a datasource with an offset.
 * @param ds The datasource which to write the double value to.
 * @param d The double value to write.
 * @param pos The offset.
 * @param error Error code.
 */
S_API void s_double_write_at(SDatasource *ds, double d, long pos, s_erc *error);


/**
 * @}
 */


#ifdef SPCT_LONG_DOUBLE
/**
 * @name long double Read/Write
 * @{
 */


/**
 * Read a long double value from a datasource.
 * @param ds The datasource form which to read the long double value.
 * @param error Error code.
 * @return The read long double value.
 */
S_API long double s_ldouble_read(SDatasource *ds, s_erc *error);


/**
 * Read a long double value from a datasource starting at a given offset position.
 * @param ds The datasource form which to read the long double value.
 * @param pos The offset to start reading from.
 * @param error Error code.
 * @return The read long double value.
 */
S_API long double s_ldouble_read_at(SDatasource *ds, long pos, s_erc *error);


/**
 * Write a long double value to a data source.
 * @param ds The datasource which to write the long double value to.
 * @param ld The long double value to write.
 * @param error Error code.
 */
S_API void s_ldouble_write(SDatasource *ds, long double ld, s_erc *error);


/**
 * Write a long double value to a datasource with an offset.
 * @param ds The datasource which to write the long double value to.
 * @param ld The long double value to write.
 * @param pos The offset.
 * @param error Error code.
 */
S_API void s_ldouble_write_at(SDatasource *ds, long double ld, long pos, s_erc *error);


/**
 * @}
 */
#endif /* SPCT_LONG_DOUBLE */


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

#endif /* _SPCT_LOAD_REAL_H__ */
