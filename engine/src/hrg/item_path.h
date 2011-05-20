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
/* DATE    : 7 November 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Extract items, item features and item functions by navigating given paths on     */
/* the HRG structure.                                                               */
/*                                                                                  */
/* Loosely based on cst_ffunction of Flite, http://cmuflite.org (1.3)               */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_ITEM_PATH_H__
#define _SPCT_ITEM_PATH_H__


/************************************************************************************/
/*  Flite license, cst_ffunction                                                    */
/*                                                                                  */
/*                       Language Technologies Institute                            */
/*                          Carnegie Mellon University                              */
/*                             Copyright (c) 1999                                   */
/*                             All Rights Reserved.                                 */
/*                                                                                  */
/*       Permission is hereby granted, free of charge, to use and distribute        */
/*       this software and its documentation without restriction, including         */
/*       without limitation the rights to use, copy, modify, merge, publish,        */
/*       distribute, sublicense, and/or sell copies of this work, and to            */
/*       permit persons to whom this work is furnished to do so, subject to         */
/*       the following conditions:                                                  */
/*        1. The code must retain the above copyright notice, this list of          */
/*           conditions and the following disclaimer.                               */
/*        2. Any modifications must be clearly marked as such.                      */
/*        3. Original authors' names are not deleted.                               */
/*        4. The authors' names are not used to endorse or promote products         */
/*           derived from this software without specific prior written              */
/*           permission.                                                            */
/*                                                                                  */
/*       CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK               */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE            */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author:  Alan W Black                                                      */
/*                                                                                  */
/************************************************************************************/


/**
 * @file item_path.h
 * Extract item info by navigating HRG.
 */


/**
 * @ingroup SItem
 * @defgroup SItemPaths Path Extractor
 * Extract items, item features and item functions by navigating given paths on
 * the HRG structure.
 * Paths are strings that are composed from tokens describing the path of the
 * requested item, relative to a given item. The possible tokens are:
 *
 * <table>
 *  <tr>
 *   <td> p </td>
 *   <td> previous item </td>
 *  </tr>
 *  <tr>
 *   <td> n </td>
 *   <td> next item </td>
 *  </tr>
 *  <tr>
 *   <td> daughter </td>
 *   <td> first daughter item </td>
 *  </tr>
 *  <tr>
 *   <td> daughtern </td>
 *   <td> last daughter item </td>
 *  </tr>
 *  <tr>
 *   <td> parent </td>
 *   <td> parent item </td>
 *  </tr>
 *  <tr>
 *   <td> R:<i>relname</i> </td>
 *   <td> item as it is in the given relation <i>relname</i> </td>
 *  </tr>
 * </table>
 *
 *
 * @todo explain "ph_" phone constant features.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"
#include "hrg/item.h"


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
 * @name Path functions
 * @{
 */


/**
 * Extract an @b item, from the given path, relative to the given
 * item.
 * @relates SItem
 *
 * @param item The given path is relative to this item.
 * @param path The path to the desired @b item, relative to the given
 * item.
 * @param error Error code.
 *
 * @return The @b item from the path, relative to the given item.
 */
S_API const SItem *s_path_to_item(const SItem *item, const char *path, s_erc *error);


/**
 * Extract an item @b feature, from the given path, relative to the
 * given item.
 * @relates SItem
 *
 * @param item The given path is relative to this item.
 * @param path The path to the desired item @b feature, relative to
 * the given item.
 * @param error Error code.
 *
 * @return The item @b feature from the path, relative to the given
 * item.
 */
S_API const SObject *s_path_to_feature(const SItem *item, const char *path,
									   s_erc *error);


/**
 * Execute the given <i> feature processor </i>, on the item from the
 * given path, relative to the given item. The name of the feature
 * processor is the last element of the path.
 * @relates SItem
 *
 * @param item The given path is relative to this item.
 * @param path The path to the desired item, relative to the given
 * item, with the last element being the feature processor.
 * @param error Error code.
 *
 * @return The result of the feature processor executed on the item
 * from the path, relative to the given item.
 */
S_API SObject *s_path_to_featproc(const SItem *item, const char *path,
								  s_erc *error);


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

#endif /* _SPCT_ITEM_PATH_H__ */
