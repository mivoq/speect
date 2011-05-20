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
/* DATE    : 15 October 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Initialize and quit serialization module.                                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_SERIALIZATION_H__
#define _SPCT_SERIALIZATION_H__


/**
 * @file serialization.h
 * Initialize and quit serialization module.
*/


/**
 * @ingroup SSerializeObjects
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "serialization/json/json.h"           /* JSON data source parser.                                  */
#include "serialization/serialize.h"           /* Reading/Writing and loading/saving of serialized objects. */
#include "serialization/serialized_file.h"     /* An abstract class for a serialized file data source.      */
#include "serialization/serialized_object.h"   /* An abstract class for a serialized object data source.    */


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
 * Initialize serialization module.
 * Used internally by #speect_init.
 *
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_serialization_init(s_erc *error);


/**
 * Quit serialization module.
 * Used internally by #speect_quit.
 *
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_serialization_quit(s_erc *error);


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

#endif /* _SPCT_SERIALIZATION_H__ */

