/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Tokenization utterance processor.                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_UTTPROCESSOR_TOKENIZATION_H__
#define _SPCT_PLUGIN_UTTPROCESSOR_TOKENIZATION_H__


/**
 * @file tokenizer_proc.h
 * A Tokenization utterance processor.
 */


/**
 * @ingroup SUttProc
 * @defgroup SUttProcTokenization Tokenization
 * A Tokenization utterance processor.
 *
 * Depends on the @e tokenizer plug-in.
 * Features that can be set for the processor:
 * <ul> <li> A "tokenizer symbols" #SMap with the following entries:
 *  <ul>
 *      <li> "whitespace" : white-space symbols used in tokenization. </li>
 *      <li> "single-char" : single-character symbols used in tokenization. </li>
 *      <li> "pre-punctuation" : pre-punctuation symbols used in tokenization. </li>
 *      <li> "post-punctuation" : post-punctuation symbols used in tokenization. </li>
 *  </ul>
 *   </li> </ul>
 *
 * Creates the @c Token relation, with items that correspond to the
 * tokens extracted with the #STokenizerString class. The item names
 * are the token strings.
 *
 * Item features that are set:
 * <ul>
 * <li> @c whitespace : white-space symbols extracted from
 * tokenization </li>
 * <li> @c prepunc : pre-punctuation symbols extracted from
 * tokenization </li>
 * <li> @c postpunc : post-punctuation symbols extracted from
 * tokenization </li>
 * </ul>
 *
 * If this processor fails then it will delete the relation and all
 * the created items.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Typedef                                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef of the tokenization utterance processor. Does not add any
 * new members, therefore exactly the same as #SUttProcessor.
 */
typedef SUttProcessor STokenizationUttProc;


/**
 * Typedef of the tokenization utterance processor class. Does not add any
 * new methods, therefore exactly the same as #SUttProcessor.
 */
typedef SUttProcessorClass STokenizationUttProcClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #STokenizationUttProc plug-in class with the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_tokenization_utt_proc_class_reg(s_erc *error);


/**
 * Free the #STokenizationUttProc plug-in class from the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_tokenization_utt_proc_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_UTTPROCESSOR_TOKENIZATION_H__ */
