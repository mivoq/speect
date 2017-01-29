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
/* DATE    : 10 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Initialize and quit datasource module.                                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_DATASOURCES_H__
#define _SPCT_DATASOURCES_H__


/**
 * @file datasources.h
 * Initialize and quit datasources module.
*/


/**
 * @ingroup SDatasource
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "datasources/data_source.h"      /* Abstract class from which other data sources can be derived. */
#include "datasources/file_source.h"      /* Class for reading from and writing to files.                 */
#include "datasources/mmapfile_source.h"  /* Class for reading from memory mapped files.                  */
#include "datasources/generic_source.h"   /* Class for reading from and writing to generic sources.       */
#include "datasources/data_reader.h"      /* Abstract _structured_ data format reader class.              */
#include "datasources/data_writer.h"      /* Abstract _structured_ data format writer class.              */
#include "datasources/load_real.h"        /* Data source reading and writing floating point values.       */
#include "datasources/load_int.h"         /* Data source reading and writing integer values.              */


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/*  Function prototypes                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize datasources module.
 * Used internally by #speect_init.
 *
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_datasources_init(s_erc *error);


/**
 * Quit datasources module.
 * Used internally by #speect_quit.
 *
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_datasources_quit(s_erc *error);


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

#endif /* _SPCT_DATASOURCES_H__ */

